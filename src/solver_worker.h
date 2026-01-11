// solver_worker.h - 求解器后台线程

#ifndef SOLVER_WORKER_H
#define SOLVER_WORKER_H

#include <QObject>
#include <QProcess>
#include <QVariantMap>
#include <QVector>
#include <atomic>

class SolverWorker : public QObject {
    Q_OBJECT

public:
    explicit SolverWorker(QObject* parent = nullptr);
    ~SolverWorker();

    // 请求取消
    void RequestCancel();

public slots:
    // 运行求解器
    void Run(const QString& solver_path, const QString& data_file,
             const QVariantMap& params);

signals:
    // 数据加载完成
    void DataLoaded(int plants, int items, int families, int periods);

    // 大联盟成本计算完成
    void GrandCoalitionComputed(double cost, double runtime);

    // 迭代开始
    void IterationStarted(int iteration);

    // MP 求解完成
    void MPSolved(QVector<double> pi, double v, double runtime);

    // SP 结果
    void SPResult(QString coalition, double cost, double violation);

    // 算法收敛
    void Converged(double epsilon);

    // 添加了新的联盟约束
    void CutAdded(QString coalition, double cost);

    // 求解完成
    void Finished(bool success, QString message,
                  double final_v, int iterations, double runtime);

    // 日志消息
    void LogMessage(QString message);

private slots:
    void OnReadyReadStdout();
    void OnReadyReadStderr();
    void OnProcessFinished(int exit_code, QProcess::ExitStatus status);

private:
    // 解析输出行
    void ParseLine(const QString& line);

    // 解析状态码
    bool ParseStatusCode(const QString& line);

    QProcess* process_;
    std::atomic<bool> cancel_requested_;

    // 解析状态
    int current_iteration_;
    double final_v_;
    int total_iterations_;
    double total_runtime_;
    QVector<double> current_pi_;
};

#endif  // SOLVER_WORKER_H
