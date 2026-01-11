// coalition_panel.h - 联盟分析面板
// 显示所有添加的约束联盟、违反量分析

#ifndef COALITION_PANEL_H
#define COALITION_PANEL_H

#include <QWidget>
#include <QJsonObject>

class QTableWidget;
class QLabel;
class QComboBox;

class CoalitionPanel : public QWidget {
    Q_OBJECT

public:
    explicit CoalitionPanel(QWidget* parent = nullptr);

    void LoadData(const QJsonObject& result);
    void Clear();

private slots:
    void OnFilterChanged(int index);

private:
    void SetupUI();
    void UpdateTable();

    QComboBox* filter_combo_;
    QTableWidget* coalition_table_;
    QLabel* stats_label_;
    QLabel* explanation_label_;

    // 缓存数据
    QJsonObject current_result_;
};

#endif  // COALITION_PANEL_H
