// coalition_panel.cpp - 联盟分析面板实现
// 显示所有添加的约束联盟、违反量分析、联盟结构

#include "coalition_panel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QJsonArray>

CoalitionPanel::CoalitionPanel(QWidget* parent)
    : QWidget(parent) {
    SetupUI();
}

void CoalitionPanel::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(16);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // ========== 过滤器 ==========
    QHBoxLayout* filter_layout = new QHBoxLayout();

    QLabel* filter_label = new QLabel("筛选:");
    filter_combo_ = new QComboBox();
    filter_combo_->addItems({
        "全部联盟",
        "仅违反约束",
        "仅紧约束",
        "按规模排序"
    });
    connect(filter_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CoalitionPanel::OnFilterChanged);

    filter_layout->addWidget(filter_label);
    filter_layout->addWidget(filter_combo_);
    filter_layout->addStretch();

    main_layout->addLayout(filter_layout);

    // ========== 联盟表格 ==========
    QGroupBox* table_group = new QGroupBox("约束联盟列表");
    QVBoxLayout* table_layout = new QVBoxLayout(table_group);

    coalition_table_ = new QTableWidget();
    coalition_table_->setColumnCount(6);
    coalition_table_->setHorizontalHeaderLabels({
        "迭代", "联盟 S", "规模 |S|", "联盟成本 c(S)",
        "分配之和", "松弛量"
    });

    coalition_table_->horizontalHeader()->setStretchLastSection(true);
    coalition_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    coalition_table_->setColumnWidth(0, 60);
    coalition_table_->setColumnWidth(1, 150);
    coalition_table_->setColumnWidth(2, 60);
    coalition_table_->setAlternatingRowColors(true);
    coalition_table_->setSelectionBehavior(QAbstractItemView::SelectRows);

    table_layout->addWidget(coalition_table_);
    main_layout->addWidget(table_group);

    // ========== 统计信息 ==========
    QGroupBox* stats_group = new QGroupBox("联盟统计");
    QVBoxLayout* stats_layout = new QVBoxLayout(stats_group);

    stats_label_ = new QLabel();
    stats_label_->setWordWrap(true);
    stats_label_->setStyleSheet("color: #555;");

    explanation_label_ = new QLabel();
    explanation_label_->setWordWrap(true);
    explanation_label_->setStyleSheet("color: #888;");

    stats_layout->addWidget(stats_label_);
    stats_layout->addWidget(explanation_label_);

    main_layout->addWidget(stats_group);
    main_layout->addStretch();
}

void CoalitionPanel::LoadData(const QJsonObject& result) {
    current_result_ = result;
    UpdateTable();
}

void CoalitionPanel::OnFilterChanged(int /*index*/) {
    UpdateTable();
}

void CoalitionPanel::UpdateTable() {
    if (current_result_.isEmpty()) return;

    // 获取联盟约束列表
    QJsonArray cuts = current_result_["cuts"].toArray();
    QJsonArray alloc_array = current_result_["allocation_array"].toArray();
    double v_star = current_result_["least_core_slack"].toDouble();

    int filter = filter_combo_->currentIndex();

    // 收集要显示的联盟
    struct CoalitionInfo {
        int iteration;
        QJsonArray members;
        double cost;
        double alloc_sum;
        double slack;
    };
    QVector<CoalitionInfo> display_list;

    for (int i = 0; i < cuts.size(); ++i) {
        QJsonObject cut = cuts[i].toObject();
        QJsonArray members = cut["coalition"].toArray();
        double cost = cut["cost"].toDouble();

        // 计算分配之和
        double alloc_sum = 0;
        for (int j = 0; j < members.size(); ++j) {
            int u = members[j].toInt();
            if (u < alloc_array.size()) {
                alloc_sum += alloc_array[u].toDouble();
            }
        }

        double slack = cost + v_star - alloc_sum;

        CoalitionInfo info;
        info.iteration = cut["iteration"].toInt(i + 1);
        info.members = members;
        info.cost = cost;
        info.alloc_sum = alloc_sum;
        info.slack = slack;

        // 根据筛选条件决定是否显示
        bool show = true;
        if (filter == 1) {
            // 仅违反约束 (slack < 0)
            show = (slack < -1e-6);
        } else if (filter == 2) {
            // 仅紧约束 (slack ~= 0)
            show = (qAbs(slack) < 1e-4);
        }

        if (show) {
            display_list.append(info);
        }
    }

    // 按规模排序
    if (filter == 3) {
        std::sort(display_list.begin(), display_list.end(),
            [](const CoalitionInfo& a, const CoalitionInfo& b) {
                return a.members.size() > b.members.size();
            });
    }

    // 填充表格
    coalition_table_->setRowCount(display_list.size());

    int tight_count = 0;
    int violated_count = 0;

    for (int i = 0; i < display_list.size(); ++i) {
        const CoalitionInfo& info = display_list[i];

        // 联盟成员字符串
        QStringList member_strs;
        for (int j = 0; j < info.members.size(); ++j) {
            member_strs << QString::number(info.members[j].toInt());
        }
        QString coalition_str = "{" + member_strs.join(", ") + "}";

        coalition_table_->setItem(i, 0, new QTableWidgetItem(QString::number(info.iteration)));
        coalition_table_->setItem(i, 1, new QTableWidgetItem(coalition_str));
        coalition_table_->setItem(i, 2, new QTableWidgetItem(QString::number(info.members.size())));
        coalition_table_->setItem(i, 3, new QTableWidgetItem(QString::number(info.cost, 'f', 2)));
        coalition_table_->setItem(i, 4, new QTableWidgetItem(QString::number(info.alloc_sum, 'f', 2)));

        QTableWidgetItem* slack_item = new QTableWidgetItem(QString::number(info.slack, 'f', 4));
        if (qAbs(info.slack) < 1e-4) {
            slack_item->setForeground(QColor("#FB8C00"));
            slack_item->setText(QString::number(info.slack, 'f', 4) + " (紧)");
            tight_count++;
        } else if (info.slack < 0) {
            slack_item->setForeground(QColor("#e53935"));
            violated_count++;
        } else {
            slack_item->setForeground(QColor("#4CAF50"));
        }
        coalition_table_->setItem(i, 5, slack_item);
    }

    // 统计信息
    stats_label_->setText(QString(
        "约束总数: %1\n"
        "紧约束数: %2\n"
        "当前显示: %3\n"
        "最小核松弛 v* = %4")
        .arg(cuts.size())
        .arg(tight_count)
        .arg(display_list.size())
        .arg(v_star, 0, 'f', 6));

    // 解释说明
    explanation_label_->setText(
        "联盟约束: sum_{u in S}(pi_u) <= c(S) + v\n\n"
        "松弛量 = c(S) + v - sum(pi_u)\n"
        "- 松弛量 = 0: 紧约束，该联盟恰好满足理性条件\n"
        "- 松弛量 > 0: 约束有余量\n"
        "- 松弛量 < 0: 约束被违反 (不应在最优解中出现)");
}

void CoalitionPanel::Clear() {
    current_result_ = QJsonObject();
    coalition_table_->setRowCount(0);
    stats_label_->setText("");
    explanation_label_->setText("");
}
