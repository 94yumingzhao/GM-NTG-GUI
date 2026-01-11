// solver_worker.cpp - 求解器后台线程实现

#include "solver_worker.h"

#include <QRegularExpression>
#include <QStringList>
#include <QDebug>

SolverWorker::SolverWorker(QObject* parent)
    : QObject(parent)
    , process_(nullptr)
    , cancel_requested_(false)
    , current_iteration_(0)
    , final_v_(0.0)
    , total_iterations_(0)
    , total_runtime_(0.0) {
}

SolverWorker::~SolverWorker() {
    if (process_) {
        process_->kill();
        process_->waitForFinished(1000);
    }
}

void SolverWorker::RequestCancel() {
    cancel_requested_ = true;
    if (process_ && process_->state() == QProcess::Running) {
        process_->kill();
    }
}

void SolverWorker::Run(const QString& solver_path, const QString& data_file,
                        const QVariantMap& params) {
    cancel_requested_ = false;
    current_iteration_ = 0;
    final_v_ = 0.0;
    total_iterations_ = 0;
    total_runtime_ = 0.0;
    current_pi_.clear();

    // 构建命令行参数
    QStringList args;
    args << data_file;

    // 添加详细输出标志
    args << "-v";

    // 时限参数 (如果求解器支持)
    // args << "-t" << QString::number(params["time_limit"].toDouble());

    // 创建进程
    process_ = new QProcess(this);

    connect(process_, &QProcess::readyReadStandardOutput,
            this, &SolverWorker::OnReadyReadStdout);
    connect(process_, &QProcess::readyReadStandardError,
            this, &SolverWorker::OnReadyReadStderr);
    connect(process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SolverWorker::OnProcessFinished);

    emit LogMessage(QString("[GUI] 启动求解器: %1").arg(solver_path));
    emit LogMessage(QString("[GUI] 数据文件: %1").arg(data_file));

    // 启动进程
    process_->start(solver_path, args);

    if (!process_->waitForStarted(5000)) {
        emit LogMessage("[ERROR] 无法启动求解器进程");
        emit Finished(false, "无法启动求解器进程", 0, 0, 0);
        return;
    }

    // 等待完成 (阻塞当前线程，但不阻塞GUI)
    process_->waitForFinished(-1);
}

void SolverWorker::OnReadyReadStdout() {
    while (process_->canReadLine()) {
        QString line = QString::fromLocal8Bit(process_->readLine()).trimmed();
        if (!line.isEmpty()) {
            ParseLine(line);
        }
    }
}

void SolverWorker::OnReadyReadStderr() {
    QString error = QString::fromLocal8Bit(process_->readAllStandardError()).trimmed();
    if (!error.isEmpty()) {
        emit LogMessage("[STDERR] " + error);
    }
}

void SolverWorker::OnProcessFinished(int exit_code, QProcess::ExitStatus status) {
    // 读取剩余输出
    OnReadyReadStdout();

    if (cancel_requested_) {
        emit Finished(false, "用户取消", final_v_, total_iterations_, total_runtime_);
    } else if (status == QProcess::CrashExit) {
        emit Finished(false, "求解器崩溃", final_v_, total_iterations_, total_runtime_);
    } else if (exit_code != 0) {
        emit Finished(false, QString("求解器返回错误码 %1").arg(exit_code),
                      final_v_, total_iterations_, total_runtime_);
    } else {
        emit Finished(true, "求解完成", final_v_, total_iterations_, total_runtime_);
    }

    process_->deleteLater();
    process_ = nullptr;
}

void SolverWorker::ParseLine(const QString& line) {
    // 尝试解析状态码
    if (ParseStatusCode(line)) {
        return;
    }

    // 发送原始日志消息
    emit LogMessage(line);
}

bool SolverWorker::ParseStatusCode(const QString& line) {
    // [LOAD:OK:P:N:G:T] - 数据加载成功
    QRegularExpression load_regex(R"(\[LOAD:OK:(\d+):(\d+):(\d+):(\d+)\])");
    auto load_match = load_regex.match(line);
    if (load_match.hasMatch()) {
        int p = load_match.captured(1).toInt();
        int n = load_match.captured(2).toInt();
        int g = load_match.captured(3).toInt();
        int t = load_match.captured(4).toInt();
        emit DataLoaded(p, n, g, t);
        emit LogMessage(line);
        return true;
    }

    // [GRAND:cost:time] - 大联盟成本
    QRegularExpression grand_regex(R"(\[GRAND:([\d.]+):([\d.]+)\])");
    auto grand_match = grand_regex.match(line);
    if (grand_match.hasMatch()) {
        double cost = grand_match.captured(1).toDouble();
        double time = grand_match.captured(2).toDouble();
        emit GrandCoalitionComputed(cost, time);
        emit LogMessage(line);
        return true;
    }

    // [ITER:k:START] - 迭代开始
    QRegularExpression iter_regex(R"(\[ITER:(\d+):START\])");
    auto iter_match = iter_regex.match(line);
    if (iter_match.hasMatch()) {
        current_iteration_ = iter_match.captured(1).toInt();
        emit IterationStarted(current_iteration_);
        emit LogMessage(line);
        return true;
    }

    // [MP:pi0:pi1:...:v] - MP 结果
    QRegularExpression mp_regex(R"(\[MP:([\d.:]+)\])");
    auto mp_match = mp_regex.match(line);
    if (mp_match.hasMatch()) {
        QStringList parts = mp_match.captured(1).split(':');
        if (parts.size() >= 2) {
            current_pi_.clear();
            for (int i = 0; i < parts.size() - 1; ++i) {
                current_pi_.append(parts[i].toDouble());
            }
            double v = parts.last().toDouble();
            emit MPSolved(current_pi_, v, 0.0);
        }
        emit LogMessage(line);
        return true;
    }

    // [SP:coalition:cost:violation] - SP 结果
    QRegularExpression sp_regex(R"(\[SP:(\{[^}]+\}):([\d.]+):([\d.-]+)\])");
    auto sp_match = sp_regex.match(line);
    if (sp_match.hasMatch()) {
        QString coalition = sp_match.captured(1);
        double cost = sp_match.captured(2).toDouble();
        double violation = sp_match.captured(3).toDouble();
        emit SPResult(coalition, cost, violation);
        emit LogMessage(line);
        return true;
    }

    // [CUT:coalition:cost] - 添加约束
    QRegularExpression cut_regex(R"(\[CUT:(\{[^}]+\}):([\d.]+)\])");
    auto cut_match = cut_regex.match(line);
    if (cut_match.hasMatch()) {
        QString coalition = cut_match.captured(1);
        double cost = cut_match.captured(2).toDouble();
        emit CutAdded(coalition, cost);
        emit LogMessage(line);
        return true;
    }

    // [CONVERGE:eps] - 收敛
    QRegularExpression conv_regex(R"(\[CONVERGE:([\d.e+-]+)\])");
    auto conv_match = conv_regex.match(line);
    if (conv_match.hasMatch()) {
        double eps = conv_match.captured(1).toDouble();
        emit Converged(eps);
        emit LogMessage(line);
        return true;
    }

    // [DONE:status:v:iter:time] - 完成
    QRegularExpression done_regex(R"(\[DONE:(\w+):([\d.-]+):(\d+):([\d.]+)\])");
    auto done_match = done_regex.match(line);
    if (done_match.hasMatch()) {
        final_v_ = done_match.captured(2).toDouble();
        total_iterations_ = done_match.captured(3).toInt();
        total_runtime_ = done_match.captured(4).toDouble();
        emit LogMessage(line);
        return true;
    }

    return false;
}
