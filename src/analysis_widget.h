// analysis_widget.h - 分析面板主控件
// 包含子 Tab: 概览 | 工厂视角 | 联盟分析 | 迭代历史

#ifndef ANALYSIS_WIDGET_H
#define ANALYSIS_WIDGET_H

#include <QWidget>
#include <QJsonObject>

class QTabWidget;
class QLabel;

// 前向声明子面板
class OverviewPanel;
class PlantPanel;
class CoalitionPanel;
class IterationPanel;

class AnalysisWidget : public QWidget {
    Q_OBJECT

public:
    explicit AnalysisWidget(QWidget* parent = nullptr);

    // 加载结果数据
    void LoadResult(const QJsonObject& result);

    // 清空
    void Clear();

private:
    void SetupUI();

    QTabWidget* tab_widget_;
    QLabel* status_label_;

    // 子面板
    OverviewPanel* overview_panel_;
    PlantPanel* plant_panel_;
    CoalitionPanel* coalition_panel_;
    IterationPanel* iteration_panel_;

    // 当前数据
    QJsonObject current_result_;
};

#endif  // ANALYSIS_WIDGET_H
