// overview_panel.h - 概览面板
// 显示关键指标、分配可视化、稳定性解释

#ifndef OVERVIEW_PANEL_H
#define OVERVIEW_PANEL_H

#include <QWidget>
#include <QJsonObject>

class QLabel;
class QProgressBar;
class QFrame;

class OverviewPanel : public QWidget {
    Q_OBJECT

public:
    explicit OverviewPanel(QWidget* parent = nullptr);

    void LoadData(const QJsonObject& result);
    void Clear();

private:
    void SetupUI();
    QFrame* CreateMetricCard(const QString& title, QLabel*& value_label,
                              const QString& color = "#333");

    // 指标卡片
    QLabel* grand_cost_value_;
    QLabel* v_star_value_;
    QLabel* iterations_value_;
    QLabel* runtime_value_;

    // 分配可视化
    QWidget* allocation_chart_;
    QVector<QProgressBar*> allocation_bars_;
    QVector<QLabel*> allocation_labels_;

    // 稳定性解释
    QLabel* stability_title_;
    QLabel* stability_desc_;

    // 问题信息
    QLabel* problem_info_;
};

#endif  // OVERVIEW_PANEL_H
