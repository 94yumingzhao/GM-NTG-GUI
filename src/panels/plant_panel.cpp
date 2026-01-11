// plant_panel.cpp - 工厂视角面板实现
// 显示各工厂的分配、收益分析、个体理性检查

#include "plant_panel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>

PlantPanel::PlantPanel(QWidget* parent)
    : QWidget(parent) {
    SetupUI();
}

void PlantPanel::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(16);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // ========== 工厂分配表格 ==========
    QGroupBox* table_group = new QGroupBox("工厂分配详情");
    QVBoxLayout* table_layout = new QVBoxLayout(table_group);

    plant_table_ = new QTableWidget();
    plant_table_->setColumnCount(6);
    plant_table_->setHorizontalHeaderLabels({
        "工厂", "独立成本 c({u})", "分配 pi_u",
        "节省额", "节省率", "状态"
    });

    plant_table_->horizontalHeader()->setStretchLastSection(true);
    plant_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    plant_table_->setAlternatingRowColors(true);
    plant_table_->setSelectionBehavior(QAbstractItemView::SelectRows);

    table_layout->addWidget(plant_table_);
    main_layout->addWidget(table_group);

    // ========== 汇总信息 ==========
    QGroupBox* summary_group = new QGroupBox("汇总分析");
    QVBoxLayout* summary_layout = new QVBoxLayout(summary_group);

    summary_label_ = new QLabel();
    summary_label_->setWordWrap(true);
    summary_label_->setStyleSheet("color: #555;");

    rationality_label_ = new QLabel();
    rationality_label_->setWordWrap(true);
    rationality_label_->setStyleSheet("padding: 8px;");

    summary_layout->addWidget(summary_label_);
    summary_layout->addWidget(rationality_label_);

    main_layout->addWidget(summary_group);
    main_layout->addStretch();
}

void PlantPanel::LoadData(const QJsonObject& result) {
    // 获取分配数组和单独联盟成本
    QJsonArray alloc_array = result["allocation_array"].toArray();
    QJsonObject singleton_costs = result["singleton_costs"].toObject();
    double grand_cost = result["grand_coalition_cost"].toDouble();

    int num_plants = alloc_array.size();
    plant_table_->setRowCount(num_plants);

    double total_alloc = 0;
    double total_singleton = 0;
    bool all_rational = true;

    for (int u = 0; u < num_plants; ++u) {
        double pi_u = alloc_array[u].toDouble();
        // 单独联盟成本 c({u})
        QString key = QString::number(u);
        double c_singleton = singleton_costs.contains(key)
            ? singleton_costs[key].toDouble()
            : 0.0;

        double savings = c_singleton - pi_u;
        double savings_rate = (c_singleton > 0) ? (savings / c_singleton * 100.0) : 0.0;
        bool is_rational = (pi_u <= c_singleton + 1e-6);

        total_alloc += pi_u;
        total_singleton += c_singleton;
        if (!is_rational) all_rational = false;

        // 填充表格
        plant_table_->setItem(u, 0, new QTableWidgetItem(QString("工厂 %1").arg(u)));
        plant_table_->setItem(u, 1, new QTableWidgetItem(QString::number(c_singleton, 'f', 2)));
        plant_table_->setItem(u, 2, new QTableWidgetItem(QString::number(pi_u, 'f', 2)));
        plant_table_->setItem(u, 3, new QTableWidgetItem(QString::number(savings, 'f', 2)));
        plant_table_->setItem(u, 4, new QTableWidgetItem(QString("%1%").arg(savings_rate, 0, 'f', 1)));

        // 状态列 - 检查个体理性
        QTableWidgetItem* status_item = new QTableWidgetItem();
        if (is_rational) {
            status_item->setText("满足");
            status_item->setForeground(QColor("#4CAF50"));
        } else {
            status_item->setText("违反");
            status_item->setForeground(QColor("#F44336"));
        }
        plant_table_->setItem(u, 5, status_item);

        // 高亮节省率最高的行
        if (savings_rate > 20) {
            for (int col = 0; col < 6; ++col) {
                plant_table_->item(u, col)->setBackground(QColor("#E8F5E9"));
            }
        }
    }

    // 汇总信息
    double total_savings = total_singleton - grand_cost;
    double overall_savings_rate = (total_singleton > 0)
        ? (total_savings / total_singleton * 100.0) : 0.0;

    summary_label_->setText(QString(
        "工厂数量: %1\n"
        "各工厂独立成本之和: %2\n"
        "大联盟成本 c(P): %3\n"
        "合作总节省: %4 (%5%)\n"
        "分配总额: %6")
        .arg(num_plants)
        .arg(total_singleton, 0, 'f', 2)
        .arg(grand_cost, 0, 'f', 2)
        .arg(total_savings, 0, 'f', 2)
        .arg(overall_savings_rate, 0, 'f', 1)
        .arg(total_alloc, 0, 'f', 2));

    // 个体理性检查结果
    if (all_rational) {
        rationality_label_->setText("个体理性: 全部满足");
        rationality_label_->setStyleSheet("padding: 8px; color: #4CAF50;");
    } else {
        rationality_label_->setText("个体理性: 存在违反");
        rationality_label_->setStyleSheet("padding: 8px; color: #e53935;");
    }
}

void PlantPanel::Clear() {
    plant_table_->setRowCount(0);
    summary_label_->setText("");
    rationality_label_->setText("");
    rationality_label_->setStyleSheet("padding: 8px;");
}
