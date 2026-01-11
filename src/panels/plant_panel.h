// plant_panel.h - 工厂视角面板
// 显示各工厂的分配、收益分析

#ifndef PLANT_PANEL_H
#define PLANT_PANEL_H

#include <QWidget>
#include <QJsonObject>

class QTableWidget;
class QLabel;

class PlantPanel : public QWidget {
    Q_OBJECT

public:
    explicit PlantPanel(QWidget* parent = nullptr);

    void LoadData(const QJsonObject& result);
    void Clear();

private:
    void SetupUI();

    QTableWidget* plant_table_;
    QLabel* summary_label_;
    QLabel* rationality_label_;
};

#endif  // PLANT_PANEL_H
