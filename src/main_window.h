// main_window.h - 主窗口
// Tab 结构: 求解 | 分析 | 实例生成

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QJsonObject>

class QTabWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QSplitter;
class QSpinBox;
class ParameterWidget;
class LogWidget;
class ResultsWidget;
class SolverWorker;
class AnalysisWidget;
class GeneratorWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // 文件操作
    void OnBrowseFile();
    void OnLoadResultFile();

    // 运行控制
    void OnStartSolver();
    void OnCancelSolver();

    // 求解器信号响应
    void OnDataLoaded(int plants, int items, int families, int periods);
    void OnGrandCoalitionComputed(double cost, double runtime);
    void OnIterationStarted(int iteration);
    void OnMPSolved(const QVector<double>& pi, double v, double runtime);
    void OnSPResult(const QString& coalition, double cost, double violation);
    void OnConverged(double epsilon);
    void OnCutAdded(const QString& coalition, double cost);
    void OnSolverFinished(bool success, const QString& message,
                          double final_v, int iterations, double runtime);
    void OnLogMessage(const QString& message);

    // 日志导出
    void OnExportLog();

    // Tab 切换
    void OnTabChanged(int index);

private:
    // 初始化界面
    void SetupUI();
    void SetupMenuBar();
    void SetupSolveTab();
    void SetupAnalysisTab();
    void SetupGeneratorTab();
    void SetupConnections();
    void UpdateUIState(bool is_running);

    // 获取求解器路径
    QString GetSolverExePath() const;

    // 加载结果 JSON
    bool LoadResultJson(const QString& filepath);

    // 主 Tab 控件
    QTabWidget* main_tab_widget_;

    // ========== 求解 Tab ==========
    QWidget* solve_tab_;
    QSplitter* solve_splitter_;

    // 左侧面板
    QLineEdit* file_path_edit_;
    QPushButton* browse_button_;
    QLabel* file_info_label_;
    ParameterWidget* parameter_widget_;
    QPushButton* start_button_;
    QPushButton* cancel_button_;
    QLabel* status_label_;
    QPushButton* export_log_button_;
    ResultsWidget* results_widget_;

    // 右侧面板
    QSpinBox* cplex_threads_spin_;
    QSpinBox* cplex_memory_spin_;
    LogWidget* log_widget_;

    // 求解结果摘要
    QLabel* grand_cost_label_;
    QLabel* v_star_label_;
    QLabel* core_status_label_;
    QLabel* iterations_label_;
    QLabel* runtime_label_;

    // ========== 分析 Tab ==========
    QWidget* analysis_tab_;
    AnalysisWidget* analysis_widget_;
    QPushButton* load_result_button_;

    // ========== 实例生成 Tab ==========
    QWidget* generator_tab_;
    GeneratorWidget* generator_widget_;

    // 求解器线程
    QThread* solver_thread_;
    SolverWorker* solver_worker_;

    // 状态
    bool is_running_;
    QString current_file_;

    // 当前结果数据 (用于分析)
    QJsonObject current_result_;
    int num_plants_;
};

#endif  // MAIN_WINDOW_H
