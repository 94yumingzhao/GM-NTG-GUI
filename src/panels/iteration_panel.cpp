// iteration_panel.cpp - 迭代历史面板实现
// 显示迭代过程、收敛情况、每次迭代的详细信息

#include "iteration_panel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QGroupBox>
#include <QProgressBar>
#include <QJsonArray>
#include <cmath>

IterationPanel::IterationPanel(QWidget* parent)
    : QWidget(parent) {
    SetupUI();
}

void IterationPanel::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(16);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // ========== 收敛曲线 (简化为条形可视化) ==========
    QGroupBox* chart_group = new QGroupBox("收敛趋势 (v* 变化)");
    QVBoxLayout* chart_layout = new QVBoxLayout(chart_group);

    chart_placeholder_ = new QWidget();
    QVBoxLayout* bars_layout = new QVBoxLayout(chart_placeholder_);
    bars_layout->setSpacing(3);

    // 预留 20 个迭代的可视化条
    for (int i = 0; i < 20; ++i) {
        QHBoxLayout* row = new QHBoxLayout();

        QLabel* label = new QLabel(QString("Iter %1").arg(i + 1));
        label->setFixedWidth(50);
        label->setVisible(false);

        QProgressBar* bar = new QProgressBar();
        bar->setRange(0, 1000);
        bar->setValue(0);
        bar->setTextVisible(true);
        bar->setStyleSheet(R"(
            QProgressBar {
                border: 1px solid #e0e0e0;
                background: #f5f5f5;
                height: 16px;
            }
            QProgressBar::chunk { background: #4CAF50; }
        )");
        bar->setVisible(false);

        row->addWidget(label);
        row->addWidget(bar);
        bars_layout->addLayout(row);
    }

    chart_layout->addWidget(chart_placeholder_);

    convergence_label_ = new QLabel();
    convergence_label_->setStyleSheet("color: #666;");
    chart_layout->addWidget(convergence_label_);

    main_layout->addWidget(chart_group);

    // ========== 迭代详情表格 ==========
    QGroupBox* table_group = new QGroupBox("迭代详情");
    QVBoxLayout* table_layout = new QVBoxLayout(table_group);

    iteration_table_ = new QTableWidget();
    iteration_table_->setColumnCount(6);
    iteration_table_->setHorizontalHeaderLabels({
        "迭代", "v_hat", "Delta", "违反联盟",
        "联盟成本", "状态"
    });

    iteration_table_->horizontalHeader()->setStretchLastSection(true);
    iteration_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    iteration_table_->setColumnWidth(0, 50);
    iteration_table_->setColumnWidth(3, 120);
    iteration_table_->setAlternatingRowColors(true);
    iteration_table_->setSelectionBehavior(QAbstractItemView::SelectRows);

    table_layout->addWidget(iteration_table_);
    main_layout->addWidget(table_group);

    // ========== 汇总信息 ==========
    summary_label_ = new QLabel();
    summary_label_->setWordWrap(true);
    summary_label_->setStyleSheet("color: #555;");
    main_layout->addWidget(summary_label_);
}

void IterationPanel::LoadData(const QJsonObject& result) {
    // 获取迭代历史
    QJsonArray history = result["iteration_history"].toArray();
    int num_iters = history.size();

    v_history_.clear();
    delta_history_.clear();

    iteration_table_->setRowCount(num_iters);

    double max_v = 0;
    for (int i = 0; i < num_iters; ++i) {
        QJsonObject iter = history[i].toObject();

        double v_hat = iter["v_hat"].toDouble();
        double delta = iter["delta"].toDouble();
        QJsonArray coalition = iter["coalition"].toArray();
        double coalition_cost = iter["coalition_cost"].toDouble();
        bool converged = iter["converged"].toBool(false);

        v_history_.append(v_hat);
        delta_history_.append(delta);
        max_v = qMax(max_v, v_hat);

        // 联盟成员字符串
        QStringList member_strs;
        for (int j = 0; j < coalition.size(); ++j) {
            member_strs << QString::number(coalition[j].toInt());
        }
        QString coalition_str = "{" + member_strs.join(", ") + "}";

        // 填充表格
        iteration_table_->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        iteration_table_->setItem(i, 1, new QTableWidgetItem(QString::number(v_hat, 'f', 6)));
        iteration_table_->setItem(i, 2, new QTableWidgetItem(QString::number(delta, 'f', 6)));
        iteration_table_->setItem(i, 3, new QTableWidgetItem(coalition_str));
        iteration_table_->setItem(i, 4, new QTableWidgetItem(QString::number(coalition_cost, 'f', 2)));

        QTableWidgetItem* status_item = new QTableWidgetItem();
        if (converged) {
            status_item->setText("收敛");
            status_item->setForeground(QColor("#4CAF50"));
        } else if (delta > v_hat + 1e-6) {
            status_item->setText("添加约束");
            status_item->setForeground(QColor("#FB8C00"));
        } else {
            status_item->setText("继续");
            status_item->setForeground(QColor("#666"));
        }
        iteration_table_->setItem(i, 5, status_item);
    }

    // 更新收敛曲线可视化
    UpdateConvergenceChart();

    // 汇总信息
    double final_v = result["least_core_slack"].toDouble();
    double runtime = result["runtime_seconds"].toDouble();

    QString status_str;
    if (final_v < 1e-6) {
        status_str = "核非空，存在稳定分配";
    } else {
        status_str = QString("核为空，最小松弛 v* = %1").arg(final_v, 0, 'f', 6);
    }

    summary_label_->setText(QString(
        "总迭代次数: %1\n"
        "总运行时间: %2 秒\n"
        "最终状态: %3\n\n"
        "收敛条件: Delta <= v_hat + epsilon\n"
        "当 Delta <= v_hat 时，表示没有联盟对当前分配有足够强的抱怨，算法收敛。")
        .arg(num_iters)
        .arg(runtime, 0, 'f', 2)
        .arg(status_str));
}

void IterationPanel::UpdateConvergenceChart() {
    // 更新条形图可视化
    QVBoxLayout* bars_layout = qobject_cast<QVBoxLayout*>(chart_placeholder_->layout());
    if (!bars_layout) return;

    double max_v = 0.01;  // 避免除零
    for (double v : v_history_) {
        max_v = qMax(max_v, v);
    }

    int display_count = qMin(v_history_.size(), 20);

    for (int i = 0; i < 20; ++i) {
        QLayoutItem* item = bars_layout->itemAt(i);
        if (!item) continue;

        QHBoxLayout* row = qobject_cast<QHBoxLayout*>(item->layout());
        if (!row) continue;

        QLabel* label = qobject_cast<QLabel*>(row->itemAt(0)->widget());
        QProgressBar* bar = qobject_cast<QProgressBar*>(row->itemAt(1)->widget());

        if (i < display_count) {
            label->setVisible(true);
            bar->setVisible(true);

            double v = v_history_[i];
            int bar_value = static_cast<int>(v / max_v * 1000);
            bar->setValue(bar_value);
            bar->setFormat(QString::number(v, 'f', 4));

            // 统一使用主色
            bar->setStyleSheet(R"(
                QProgressBar {
                    border: 1px solid #e0e0e0;
                    background: #f5f5f5;
                    height: 16px;
                }
                QProgressBar::chunk { background: #4CAF50; }
            )");
        } else {
            label->setVisible(false);
            bar->setVisible(false);
        }
    }

    // 收敛趋势说明
    if (v_history_.size() >= 2) {
        double first_v = v_history_.first();
        double last_v = v_history_.last();
        double reduction = first_v - last_v;
        double reduction_rate = (first_v > 0) ? (reduction / first_v * 100) : 0;

        convergence_label_->setText(QString(
            "初始 v = %1, 最终 v = %2, 减少 %3 (%4%)")
            .arg(first_v, 0, 'f', 4)
            .arg(last_v, 0, 'f', 4)
            .arg(reduction, 0, 'f', 4)
            .arg(reduction_rate, 0, 'f', 1));
    } else {
        convergence_label_->setText("");
    }
}

void IterationPanel::Clear() {
    v_history_.clear();
    delta_history_.clear();
    iteration_table_->setRowCount(0);
    summary_label_->setText("");
    convergence_label_->setText("");

    // 隐藏所有条形图
    QVBoxLayout* bars_layout = qobject_cast<QVBoxLayout*>(chart_placeholder_->layout());
    if (bars_layout) {
        for (int i = 0; i < 20; ++i) {
            QLayoutItem* item = bars_layout->itemAt(i);
            if (!item) continue;
            QHBoxLayout* row = qobject_cast<QHBoxLayout*>(item->layout());
            if (!row) continue;
            if (row->itemAt(0) && row->itemAt(0)->widget())
                row->itemAt(0)->widget()->setVisible(false);
            if (row->itemAt(1) && row->itemAt(1)->widget())
                row->itemAt(1)->widget()->setVisible(false);
        }
    }
}
