// results_widget.cpp - 结果表格面板实现 (分配方案 + 联盟约束)

#include "results_widget.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>

ResultsWidget::ResultsWidget(QWidget* parent)
    : QWidget(parent)
    , num_plants_(0) {
    SetupUI();
    Clear();
}

void ResultsWidget::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(8);

    // ========== 分配方案表格 ==========
    QGroupBox* alloc_group = new QGroupBox("分配方案");
    QVBoxLayout* alloc_layout = new QVBoxLayout(alloc_group);
    alloc_layout->setContentsMargins(8, 12, 8, 8);

    allocation_table_ = new QTableWidget();
    allocation_table_->setColumnCount(3);
    allocation_table_->setHorizontalHeaderLabels({"工厂", "分配", "占比"});
    allocation_table_->horizontalHeader()->setStretchLastSection(true);
    allocation_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    allocation_table_->verticalHeader()->setVisible(false);
    allocation_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allocation_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    allocation_table_->setMaximumHeight(120);
    alloc_layout->addWidget(allocation_table_);

    main_layout->addWidget(alloc_group);

    // ========== 联盟约束表格 ==========
    QGroupBox* coalition_group = new QGroupBox("联盟约束");
    QVBoxLayout* coalition_layout = new QVBoxLayout(coalition_group);
    coalition_layout->setContentsMargins(8, 12, 8, 8);

    coalition_table_ = new QTableWidget();
    coalition_table_->setColumnCount(3);
    coalition_table_->setHorizontalHeaderLabels({"联盟", "成本", "违反"});
    coalition_table_->horizontalHeader()->setStretchLastSection(true);
    coalition_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    coalition_table_->verticalHeader()->setVisible(false);
    coalition_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    coalition_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    coalition_table_->setMaximumHeight(100);
    coalition_layout->addWidget(coalition_table_);

    main_layout->addWidget(coalition_group);
}

void ResultsWidget::Clear() {
    allocation_table_->setRowCount(0);
    coalition_table_->setRowCount(0);
    num_plants_ = 0;
}

void ResultsWidget::SetProblemSize(int plants) {
    num_plants_ = plants;

    // 初始化分配表格
    allocation_table_->setRowCount(plants);
    for (int i = 0; i < plants; ++i) {
        allocation_table_->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        allocation_table_->setItem(i, 1, new QTableWidgetItem("-"));
        allocation_table_->setItem(i, 2, new QTableWidgetItem("-"));
    }
}

void ResultsWidget::SetAllocation(const QVector<double>& pi) {
    // 计算总和
    double total = 0.0;
    for (int i = 0; i < pi.size() && i < allocation_table_->rowCount(); ++i) {
        total += pi[i];
    }

    // 更新分配表格
    for (int i = 0; i < pi.size() && i < allocation_table_->rowCount(); ++i) {
        allocation_table_->item(i, 1)->setText(QString::number(pi[i], 'f', 2));

        double ratio = (total > 0) ? (pi[i] / total * 100.0) : 0.0;
        allocation_table_->item(i, 2)->setText(QString("%1%").arg(ratio, 0, 'f', 1));
    }
}

void ResultsWidget::AddCoalitionCut(const QString& coalition, double cost, double violation) {
    int row = coalition_table_->rowCount();
    coalition_table_->insertRow(row);

    coalition_table_->setItem(row, 0, new QTableWidgetItem(coalition));
    coalition_table_->setItem(row, 1, new QTableWidgetItem(QString::number(cost, 'f', 2)));
    coalition_table_->setItem(row, 2, new QTableWidgetItem(QString::number(violation, 'f', 4)));
}
