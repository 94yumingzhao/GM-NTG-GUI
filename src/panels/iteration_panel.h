// iteration_panel.h - 迭代历史面板
// 显示迭代过程、收敛曲线、每次迭代的详细信息

#ifndef ITERATION_PANEL_H
#define ITERATION_PANEL_H

#include <QWidget>
#include <QJsonObject>

class QTableWidget;
class QLabel;
class QCustomPlot;

class IterationPanel : public QWidget {
    Q_OBJECT

public:
    explicit IterationPanel(QWidget* parent = nullptr);

    void LoadData(const QJsonObject& result);
    void Clear();

private:
    void SetupUI();
    void UpdateConvergenceChart();

    QTableWidget* iteration_table_;
    QWidget* chart_placeholder_;  // 收敛曲线占位 (可扩展为 QCustomPlot)
    QLabel* convergence_label_;
    QLabel* summary_label_;

    // 缓存数据
    QVector<double> v_history_;
    QVector<double> delta_history_;
};

#endif  // ITERATION_PANEL_H
