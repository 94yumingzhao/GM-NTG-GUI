// main_window.cpp - 主窗口实现

#include "main_window.h"
#include "parameter_widget.h"
#include "log_widget.h"
#include "results_widget.h"
#include "solver_worker.h"
#include "analysis_widget.h"
#include "generator_widget.h"

#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QSplitter>
#include <QTabWidget>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , solver_thread_(nullptr)
    , solver_worker_(nullptr)
    , is_running_(false)
    , num_plants_(0) {
    SetupUI();
    SetupMenuBar();
    SetupConnections();
    UpdateUIState(false);

    setWindowTitle("GM-NTG-GUI");
    resize(1000, 700);
}

MainWindow::~MainWindow() {
    if (solver_thread_ && solver_thread_->isRunning()) {
        if (solver_worker_) {
            solver_worker_->RequestCancel();
        }
        solver_thread_->quit();
        solver_thread_->wait(3000);
    }
}

void MainWindow::SetupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* main_layout = new QVBoxLayout(central);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // 主 Tab 控件
    main_tab_widget_ = new QTabWidget();

    SetupSolveTab();
    SetupAnalysisTab();
    SetupGeneratorTab();

    main_tab_widget_->addTab(solve_tab_, "求解");
    main_tab_widget_->addTab(analysis_tab_, "分析");
    main_tab_widget_->addTab(generator_tab_, "算例生成");

    main_layout->addWidget(main_tab_widget_);

    statusBar()->showMessage("就绪");
}

void MainWindow::SetupMenuBar() {
    QMenu* file_menu = menuBar()->addMenu("文件(&F)");

    QAction* open_action = new QAction("打开(&O)...", this);
    open_action->setShortcut(QKeySequence::Open);
    connect(open_action, &QAction::triggered, this, &MainWindow::OnBrowseFile);
    file_menu->addAction(open_action);

    QAction* export_action = new QAction("导出日志(&E)...", this);
    export_action->setShortcut(QKeySequence::Save);
    connect(export_action, &QAction::triggered, this, &MainWindow::OnExportLog);
    file_menu->addAction(export_action);

    file_menu->addSeparator();

    QAction* exit_action = new QAction("退出(&X)", this);
    exit_action->setShortcut(QKeySequence::Quit);
    connect(exit_action, &QAction::triggered, this, &QWidget::close);
    file_menu->addAction(exit_action);

    QMenu* help_menu = menuBar()->addMenu("帮助(&H)");
    QAction* about_action = new QAction("关于(&A)", this);
    connect(about_action, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于",
            "GM-NTG-GUI\n\n"
            "版本 1.0.0\n\n"
            "多工厂NTG博弈求解器图形界面\n"
            "基于Benders分解算法");
    });
    help_menu->addAction(about_action);
}

void MainWindow::SetupSolveTab() {
    solve_tab_ = new QWidget();
    QHBoxLayout* tab_layout = new QHBoxLayout(solve_tab_);
    tab_layout->setSpacing(8);
    tab_layout->setContentsMargins(8, 8, 8, 8);

    // ========== 左侧面板 ==========
    QWidget* left_panel = new QWidget();
    left_panel->setMinimumWidth(280);
    left_panel->setMaximumWidth(420);
    QVBoxLayout* left_layout = new QVBoxLayout(left_panel);
    left_layout->setSpacing(8);
    left_layout->setContentsMargins(0, 0, 0, 0);

    // 文件选择组
    QGroupBox* file_group = new QGroupBox("数据文件");
    QVBoxLayout* file_layout = new QVBoxLayout(file_group);
    file_layout->setSpacing(4);
    file_layout->setContentsMargins(8, 12, 8, 8);

    QHBoxLayout* file_row = new QHBoxLayout();
    file_row->setSpacing(4);
    file_path_edit_ = new QLineEdit();
    file_path_edit_->setReadOnly(true);
    file_path_edit_->setPlaceholderText("选择CSV数据文件...");
    browse_button_ = new QPushButton("...");
    browse_button_->setFixedWidth(32);
    file_row->addWidget(file_path_edit_);
    file_row->addWidget(browse_button_);
    file_layout->addLayout(file_row);

    file_info_label_ = new QLabel("--");
    file_info_label_->setStyleSheet("color: #666; font-size: 9pt;");
    file_layout->addWidget(file_info_label_);

    left_layout->addWidget(file_group);

    // 参数设置组
    parameter_widget_ = new ParameterWidget();
    left_layout->addWidget(parameter_widget_);

    // 运行控制组
    QGroupBox* control_group = new QGroupBox("运行");
    QVBoxLayout* control_layout = new QVBoxLayout(control_group);
    control_layout->setSpacing(6);
    control_layout->setContentsMargins(8, 12, 8, 8);

    QHBoxLayout* button_row = new QHBoxLayout();
    button_row->setSpacing(6);
    start_button_ = new QPushButton("运行");
    start_button_->setMinimumHeight(32);
    start_button_->setStyleSheet("font-weight: bold;");
    cancel_button_ = new QPushButton("取消");
    cancel_button_->setMinimumHeight(32);
    button_row->addWidget(start_button_);
    button_row->addWidget(cancel_button_);
    control_layout->addLayout(button_row);

    status_label_ = new QLabel("就绪");
    status_label_->setAlignment(Qt::AlignCenter);
    status_label_->setStyleSheet("color: #666;");
    control_layout->addWidget(status_label_);

    left_layout->addWidget(control_group);

    // 结果表格 (分配方案 + 联盟约束)
    results_widget_ = new ResultsWidget();
    left_layout->addWidget(results_widget_);

    // 导出按钮
    export_log_button_ = new QPushButton("导出日志...");
    left_layout->addWidget(export_log_button_);

    left_layout->addStretch();

    // ========== 右侧面板 ==========
    QWidget* right_panel = new QWidget();
    QVBoxLayout* right_layout = new QVBoxLayout(right_panel);
    right_layout->setSpacing(8);
    right_layout->setContentsMargins(0, 0, 0, 0);

    // CPLEX 设置
    QGroupBox* cplex_group = new QGroupBox("CPLEX");
    QHBoxLayout* cplex_layout = new QHBoxLayout(cplex_group);
    cplex_layout->setContentsMargins(8, 12, 8, 8);
    cplex_layout->setSpacing(12);

    cplex_layout->addWidget(new QLabel("线程"));
    cplex_threads_spin_ = new QSpinBox();
    cplex_threads_spin_->setRange(0, 64);
    cplex_threads_spin_->setValue(0);
    cplex_threads_spin_->setSpecialValueText("自动");
    cplex_threads_spin_->setFixedWidth(90);
    cplex_layout->addWidget(cplex_threads_spin_);

    cplex_layout->addWidget(new QLabel("内存"));
    cplex_memory_spin_ = new QSpinBox();
    cplex_memory_spin_->setRange(512, 65536);
    cplex_memory_spin_->setValue(4096);
    cplex_memory_spin_->setSuffix(" MB");
    cplex_memory_spin_->setSingleStep(512);
    cplex_memory_spin_->setFixedWidth(90);
    cplex_layout->addWidget(cplex_memory_spin_);

    cplex_layout->addStretch();
    right_layout->addWidget(cplex_group);

    // 运行日志
    QGroupBox* log_group = new QGroupBox("运行日志");
    QVBoxLayout* log_layout = new QVBoxLayout(log_group);
    log_layout->setContentsMargins(8, 12, 8, 8);
    log_widget_ = new LogWidget();
    log_layout->addWidget(log_widget_);
    right_layout->addWidget(log_group, 1);

    // 求解结果摘要
    QGroupBox* summary_group = new QGroupBox("求解结果");
    QGridLayout* summary_layout = new QGridLayout(summary_group);
    summary_layout->setContentsMargins(8, 12, 8, 8);
    summary_layout->setSpacing(4);

    summary_layout->addWidget(new QLabel("大联盟成本 c(P):"), 0, 0);
    grand_cost_label_ = new QLabel("-");
    grand_cost_label_->setStyleSheet("font-weight: bold;");
    summary_layout->addWidget(grand_cost_label_, 0, 1);

    summary_layout->addWidget(new QLabel("最小核松弛 v*:"), 0, 2);
    v_star_label_ = new QLabel("-");
    v_star_label_->setStyleSheet("font-weight: bold;");
    summary_layout->addWidget(v_star_label_, 0, 3);

    summary_layout->addWidget(new QLabel("核状态:"), 1, 0);
    core_status_label_ = new QLabel("-");
    summary_layout->addWidget(core_status_label_, 1, 1);

    summary_layout->addWidget(new QLabel("迭代/时间:"), 1, 2);
    iterations_label_ = new QLabel("-");
    summary_layout->addWidget(iterations_label_, 1, 3);

    runtime_label_ = new QLabel();  // 隐藏，合并到iterations_label_
    runtime_label_->hide();

    right_layout->addWidget(summary_group);

    // Splitter
    solve_splitter_ = new QSplitter(Qt::Horizontal);
    solve_splitter_->addWidget(left_panel);
    solve_splitter_->addWidget(right_panel);
    solve_splitter_->setStretchFactor(0, 0);
    solve_splitter_->setStretchFactor(1, 1);
    solve_splitter_->setHandleWidth(8);
    solve_splitter_->setStyleSheet(
        "QSplitter::handle { background-color: transparent; "
        "    border-left: 1px solid #e0e0e0; margin-left: 3px; margin-right: 4px; }"
        "QSplitter::handle:hover { border-left: 1px solid #bdbdbd; }"
        "QSplitter::handle:pressed { border-left: 1px solid #9e9e9e; }");

    tab_layout->addWidget(solve_splitter_);
}

void MainWindow::SetupAnalysisTab() {
    analysis_tab_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(analysis_tab_);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);

    // 顶部工具栏
    QHBoxLayout* toolbar = new QHBoxLayout();
    load_result_button_ = new QPushButton("加载结果文件...");
    toolbar->addWidget(load_result_button_);
    toolbar->addStretch();
    layout->addLayout(toolbar);

    // 分析面板
    analysis_widget_ = new AnalysisWidget();
    layout->addWidget(analysis_widget_);
}

void MainWindow::SetupGeneratorTab() {
    generator_tab_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(generator_tab_);
    layout->setContentsMargins(8, 8, 8, 8);

    generator_widget_ = new GeneratorWidget();
    layout->addWidget(generator_widget_);
}

void MainWindow::SetupConnections() {
    // 文件操作
    connect(browse_button_, &QPushButton::clicked, this, &MainWindow::OnBrowseFile);
    connect(load_result_button_, &QPushButton::clicked, this, &MainWindow::OnLoadResultFile);

    // 运行控制
    connect(start_button_, &QPushButton::clicked, this, &MainWindow::OnStartSolver);
    connect(cancel_button_, &QPushButton::clicked, this, &MainWindow::OnCancelSolver);

    // 导出日志
    connect(export_log_button_, &QPushButton::clicked, this, &MainWindow::OnExportLog);

    // Tab 切换
    connect(main_tab_widget_, &QTabWidget::currentChanged, this, &MainWindow::OnTabChanged);
}

void MainWindow::UpdateUIState(bool is_running) {
    is_running_ = is_running;

    browse_button_->setEnabled(!is_running);
    parameter_widget_->setEnabled(!is_running);
    start_button_->setEnabled(!is_running && !current_file_.isEmpty());
    cancel_button_->setEnabled(is_running);
    export_log_button_->setEnabled(!is_running);

    if (is_running) {
        status_label_->setText("运行中...");
        statusBar()->showMessage("运行中...");
    } else {
        status_label_->setText("就绪");
        statusBar()->showMessage("就绪");
    }
}

QString MainWindow::GetSolverExePath() const {
    QStringList paths = {
        "../../../GM-NTG-Core/build/release/bin/Release/GM-NTG-Core.exe",
        "../../GM-NTG-Core/build/release/bin/Release/GM-NTG-Core.exe",
        "../GM-NTG-Core/build/release/bin/Release/GM-NTG-Core.exe",
        "D:/YM-Code/GM-NTG-Core/build/release/bin/Release/GM-NTG-Core.exe"
    };

    for (const QString& path : paths) {
        if (QFileInfo::exists(path)) {
            return QFileInfo(path).absoluteFilePath();
        }
    }

    return QString();
}

void MainWindow::OnBrowseFile() {
    QString default_dir = "D:/YM-Code/GM-NTG-Core/data";
    if (!QDir(default_dir).exists()) {
        default_dir = QDir::currentPath();
    }

    QString file = QFileDialog::getOpenFileName(
        this, "选择数据文件", default_dir,
        "CSV 文件 (*.csv);;所有文件 (*)");

    if (!file.isEmpty()) {
        current_file_ = file;
        QFileInfo info(file);
        file_path_edit_->setText(info.fileName());
        file_path_edit_->setToolTip(file);

        file_info_label_->setText(QString("%1 (%2 KB)")
            .arg(info.fileName()).arg(info.size() / 1024));
        file_info_label_->setStyleSheet("color: black; font-size: 9pt;");

        UpdateUIState(false);
        log_widget_->Append("文件: " + file);
    }
}

void MainWindow::OnLoadResultFile() {
    QString default_dir = "D:/YM-Code/GM-NTG-Core/results";
    if (!QDir(default_dir).exists()) {
        default_dir = QDir::currentPath();
    }

    QString file = QFileDialog::getOpenFileName(
        this, "选择结果文件", default_dir,
        "JSON 文件 (*.json);;所有文件 (*)");

    if (!file.isEmpty() && LoadResultJson(file)) {
        analysis_widget_->LoadResult(current_result_);
    }
}

bool MainWindow::LoadResultJson(const QString& filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + filepath);
        return false;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, "错误", "JSON 解析错误: " + error.errorString());
        return false;
    }

    current_result_ = doc.object();
    return true;
}

void MainWindow::OnStartSolver() {
    if (current_file_.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择数据文件");
        return;
    }

    QString solver_path = GetSolverExePath();
    if (solver_path.isEmpty()) {
        QMessageBox::critical(this, "错误",
            "找不到求解器 GM-NTG-Core.exe\n"
            "请确保已编译 GM-NTG-Core 项目");
        return;
    }

    log_widget_->Clear();
    results_widget_->Clear();

    // 清空摘要
    grand_cost_label_->setText("-");
    v_star_label_->setText("-");
    core_status_label_->setText("-");
    core_status_label_->setStyleSheet("");
    iterations_label_->setText("-");

    solver_thread_ = new QThread(this);
    solver_worker_ = new SolverWorker();
    solver_worker_->moveToThread(solver_thread_);

    connect(solver_thread_, &QThread::started, solver_worker_, [this, solver_path]() {
        QVariantMap params = parameter_widget_->GetParameters();
        // 添加 CPLEX 参数
        params["cplex_threads"] = cplex_threads_spin_->value();
        params["cplex_memory"] = cplex_memory_spin_->value();
        solver_worker_->Run(solver_path, current_file_, params);
    });
    connect(solver_worker_, &SolverWorker::DataLoaded, this, &MainWindow::OnDataLoaded);
    connect(solver_worker_, &SolverWorker::GrandCoalitionComputed, this, &MainWindow::OnGrandCoalitionComputed);
    connect(solver_worker_, &SolverWorker::IterationStarted, this, &MainWindow::OnIterationStarted);
    connect(solver_worker_, &SolverWorker::MPSolved, this, &MainWindow::OnMPSolved);
    connect(solver_worker_, &SolverWorker::SPResult, this, &MainWindow::OnSPResult);
    connect(solver_worker_, &SolverWorker::Converged, this, &MainWindow::OnConverged);
    connect(solver_worker_, &SolverWorker::CutAdded, this, &MainWindow::OnCutAdded);
    connect(solver_worker_, &SolverWorker::Finished, this, &MainWindow::OnSolverFinished);
    connect(solver_worker_, &SolverWorker::LogMessage, this, &MainWindow::OnLogMessage);

    connect(solver_worker_, &SolverWorker::Finished, solver_thread_, &QThread::quit);
    connect(solver_thread_, &QThread::finished, solver_worker_, &QObject::deleteLater);
    connect(solver_thread_, &QThread::finished, solver_thread_, &QObject::deleteLater);
    connect(solver_thread_, &QThread::finished, this, [this]() {
        solver_thread_ = nullptr;
        solver_worker_ = nullptr;
    });

    UpdateUIState(true);
    log_widget_->Append("开始求解...");
    solver_thread_->start();
}

void MainWindow::OnCancelSolver() {
    if (solver_worker_) {
        solver_worker_->RequestCancel();
        status_label_->setText("取消中...");
        statusBar()->showMessage("取消中...");
    }
}

void MainWindow::OnDataLoaded(int plants, int items, int families, int periods) {
    num_plants_ = plants;
    file_info_label_->setText(QString("P=%1 N=%2 G=%3 T=%4")
        .arg(plants).arg(items).arg(families).arg(periods));
    file_info_label_->setStyleSheet("color: black; font-size: 9pt;");
    results_widget_->SetProblemSize(plants);
}

void MainWindow::OnGrandCoalitionComputed(double cost, double runtime) {
    grand_cost_label_->setText(QString::number(cost, 'f', 2));
    log_widget_->Append(QString("大联盟成本 c(P) = %1 (用时 %2s)")
        .arg(cost, 0, 'f', 2).arg(runtime, 0, 'f', 2));
}

void MainWindow::OnIterationStarted(int iteration) {
    status_label_->setText(QString("迭代 %1...").arg(iteration));
    statusBar()->showMessage(QString("迭代 %1...").arg(iteration));
}

void MainWindow::OnMPSolved(const QVector<double>& pi, double v, double runtime) {
    Q_UNUSED(runtime);
    results_widget_->SetAllocation(pi);
    v_star_label_->setText(QString::number(v, 'f', 6));

    // 更新核状态
    const double eps = 1e-6;
    if (v <= eps) {
        core_status_label_->setText("核非空");
        core_status_label_->setStyleSheet("color: #4CAF50; font-weight: bold;");
    } else {
        core_status_label_->setText(QString("核为空 (v*=%1)").arg(v, 0, 'f', 4));
        core_status_label_->setStyleSheet("color: #FB8C00; font-weight: bold;");
    }
}

void MainWindow::OnSPResult(const QString& coalition, double cost, double violation) {
    results_widget_->AddCoalitionCut(coalition, cost, violation);
}

void MainWindow::OnConverged(double epsilon) {
    log_widget_->Append(QString("算法收敛 (epsilon = %1)").arg(epsilon, 0, 'e', 2));
}

void MainWindow::OnCutAdded(const QString& coalition, double cost) {
    log_widget_->Append(QString("添加联盟约束 S=%1, c(S)=%2")
        .arg(coalition).arg(cost, 0, 'f', 2));
}

void MainWindow::OnSolverFinished(bool success, const QString& message,
                                   double final_v, int iterations, double runtime) {
    UpdateUIState(false);

    if (success) {
        status_label_->setText("完成");
        statusBar()->showMessage("优化完成");

        // 更新摘要
        v_star_label_->setText(QString::number(final_v, 'f', 6));
        iterations_label_->setText(QString("%1 次 / %2 s").arg(iterations).arg(runtime, 0, 'f', 2));

        // 更新核状态
        const double eps = 1e-6;
        if (final_v <= eps) {
            core_status_label_->setText("核非空");
            core_status_label_->setStyleSheet("color: #4CAF50; font-weight: bold;");
        } else {
            core_status_label_->setText(QString("核为空 (v*=%1)").arg(final_v, 0, 'f', 4));
            core_status_label_->setStyleSheet("color: #FB8C00; font-weight: bold;");
        }

        // 自动加载最新结果到分析面板
        QString results_dir = "D:/YM-Code/GM-NTG-Core/results";
        QDir dir(results_dir);
        QStringList filters;
        filters << "result_*.json";
        QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Time);
        if (!files.isEmpty()) {
            if (LoadResultJson(files.first().absoluteFilePath())) {
                analysis_widget_->LoadResult(current_result_);
            }
        }
    } else {
        status_label_->setText("失败");
        statusBar()->showMessage("优化失败");
        QMessageBox::warning(this, "求解失败", message);
    }
}

void MainWindow::OnLogMessage(const QString& message) {
    log_widget_->Append(message);
}

void MainWindow::OnExportLog() {
    QString file = QFileDialog::getSaveFileName(
        this, "导出日志",
        QDir::currentPath() + "/solver_log.txt",
        "文本文件 (*.txt)");

    if (!file.isEmpty()) {
        if (log_widget_->ExportToFile(file)) {
            QMessageBox::information(this, "导出成功",
                QString("日志已导出到: %1").arg(file));
        } else {
            QMessageBox::warning(this, "导出失败", "无法写入文件");
        }
    }
}

void MainWindow::OnTabChanged(int index) {
    Q_UNUSED(index);
}
