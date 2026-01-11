// results_widget.h - 结果表格面板 (分配方案 + 联盟约束)

#ifndef RESULTS_WIDGET_H
#define RESULTS_WIDGET_H

#include <QWidget>
#include <QVector>

class QTableWidget;

class ResultsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget* parent = nullptr);

    // 清空结果
    void Clear();

    // 设置问题规模 (初始化表格)
    void SetProblemSize(int plants);

    // 设置分配方案
    void SetAllocation(const QVector<double>& pi);

    // 添加联盟约束
    void AddCoalitionCut(const QString& coalition, double cost, double violation);

private:
    void SetupUI();

    // 分配表格
    QTableWidget* allocation_table_;

    // 联盟约束表格
    QTableWidget* coalition_table_;

    // 数据
    int num_plants_;
};

#endif  // RESULTS_WIDGET_H
