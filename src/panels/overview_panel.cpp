// overview_panel.cpp - 概览面板实现

#include "overview_panel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QProgressBar>
#include <QGroupBox>
#include <QJsonArray>

OverviewPanel::OverviewPanel(QWidget* parent)
    : QWidget(parent) {
    SetupUI();
}

void OverviewPanel::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(16);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // ========== 指标卡片行 ==========
    QHBoxLayout* cards_layout = new QHBoxLayout();
    cards_layout->setSpacing(12);

    // 统一使用简洁的灰度卡片，数值用主色
    cards_layout->addWidget(CreateMetricCard("大联盟成本 c(P)", grand_cost_value_, "#333"));
    cards_layout->addWidget(CreateMetricCard("最小核松弛 v*", v_star_value_, "#4CAF50"));
    cards_layout->addWidget(CreateMetricCard("迭代次数", iterations_value_, "#333"));
    cards_layout->addWidget(CreateMetricCard("运行时间", runtime_value_, "#333"));

    main_layout->addLayout(cards_layout);

    // ========== 分配可视化 ==========
    QGroupBox* alloc_group = new QGroupBox("分配方案可视化");
    QVBoxLayout* alloc_layout = new QVBoxLayout(alloc_group);

    allocation_chart_ = new QWidget();
    QVBoxLayout* chart_layout = new QVBoxLayout(allocation_chart_);
    chart_layout->setSpacing(8);

    // 预创建 10 个工厂的条形图 (动态隐藏不需要的)
    for (int i = 0; i < 10; ++i) {
        QHBoxLayout* row = new QHBoxLayout();

        QLabel* label = new QLabel(QString("工厂 %1").arg(i));
        label->setFixedWidth(60);
        allocation_labels_.append(label);

        QProgressBar* bar = new QProgressBar();
        bar->setRange(0, 100);
        bar->setValue(0);
        bar->setTextVisible(true);
        bar->setFormat("%v%");
        bar->setStyleSheet(R"(
            QProgressBar {
                border: 1px solid #e0e0e0;
                background: #f5f5f5;
                height: 18px;
            }
            QProgressBar::chunk {
                background: #4CAF50;
            }
        )");
        allocation_bars_.append(bar);

        row->addWidget(label);
        row->addWidget(bar);

        chart_layout->addLayout(row);

        // 初始隐藏
        label->setVisible(false);
        bar->setVisible(false);
    }

    alloc_layout->addWidget(allocation_chart_);
    main_layout->addWidget(alloc_group);

    // ========== 稳定性解释 ==========
    QGroupBox* stability_group = new QGroupBox("稳定性分析");
    QVBoxLayout* stability_layout = new QVBoxLayout(stability_group);

    stability_title_ = new QLabel();
    stability_title_->setStyleSheet("color: #333;");

    stability_desc_ = new QLabel();
    stability_desc_->setWordWrap(true);
    stability_desc_->setStyleSheet("color: #666;");

    stability_layout->addWidget(stability_title_);
    stability_layout->addWidget(stability_desc_);

    main_layout->addWidget(stability_group);

    // ========== 问题信息 ==========
    problem_info_ = new QLabel();
    problem_info_->setStyleSheet("color: #999;");
    main_layout->addWidget(problem_info_);

    main_layout->addStretch();
}

QFrame* OverviewPanel::CreateMetricCard(const QString& title, QLabel*& value_label,
                                         const QString& color) {
    QFrame* card = new QFrame();
    card->setFrameStyle(QFrame::NoFrame);
    card->setStyleSheet(R"(
        QFrame {
            background: white;
            border: 1px solid #e8e8e8;
            padding: 12px;
        }
    )");
    card->setMinimumHeight(80);

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setSpacing(4);

    QLabel* title_label = new QLabel(title);
    title_label->setStyleSheet("color: #888;");

    value_label = new QLabel("-");
    value_label->setStyleSheet(QString("font-size: 20px; color: %1;").arg(color));

    layout->addWidget(title_label);
    layout->addWidget(value_label);
    layout->addStretch();

    return card;
}

void OverviewPanel::LoadData(const QJsonObject& result) {
    // 加载指标
    double grand_cost = result["grand_coalition_cost"].toDouble();
    double v_star = result["least_core_slack"].toDouble();
    int iterations = result["iterations"].toInt();
    double runtime = result["runtime_seconds"].toDouble();

    grand_cost_value_->setText(QString::number(grand_cost, 'f', 2));
    v_star_value_->setText(QString::number(v_star, 'f', 6));
    iterations_value_->setText(QString::number(iterations));
    runtime_value_->setText(QString("%1 s").arg(runtime, 0, 'f', 2));

    // 加载分配
    QJsonArray alloc_array = result["allocation_array"].toArray();
    int num_plants = alloc_array.size();

    double total = 0;
    for (int i = 0; i < num_plants; ++i) {
        total += alloc_array[i].toDouble();
    }

    for (int i = 0; i < allocation_bars_.size(); ++i) {
        if (i < num_plants) {
            double pi = alloc_array[i].toDouble();
            double ratio = (total > 0) ? (pi / total * 100.0) : 0.0;

            allocation_labels_[i]->setVisible(true);
            allocation_bars_[i]->setVisible(true);
            allocation_bars_[i]->setValue(static_cast<int>(ratio));
            allocation_bars_[i]->setFormat(QString("%1 (%2%)")
                .arg(pi, 0, 'f', 2).arg(ratio, 0, 'f', 1));
        } else {
            allocation_labels_[i]->setVisible(false);
            allocation_bars_[i]->setVisible(false);
        }
    }

    // 稳定性分析
    QJsonObject interp = result["interpretation"].toObject();
    QString core_status = interp["core_status"].toString();

    if (core_status == "non-empty") {
        stability_title_->setText("核非空 - 分配稳定");
        stability_title_->setStyleSheet("color: #4CAF50;");
        stability_desc_->setText(
            "v* = 0 表示当前分配位于博弈的核 (Core) 内。"
            "没有任何子联盟有动机脱离大联盟独立运营。");
    } else {
        stability_title_->setText(QString("核为空 - 需补贴 %1").arg(v_star, 0, 'f', 4));
        stability_title_->setStyleSheet("color: #FB8C00;");
        stability_desc_->setText(QString(
            "v* = %1 > 0 表示非负核为空。当前分配是最小核解，"
            "最小化了最大联盟抱怨量。").arg(v_star, 0, 'f', 4));
    }

    // 问题信息
    QJsonObject problem_size = result["problem_size"].toObject();
    problem_info_->setText(QString("问题规模: P=%1 工厂, N=%2 产品, G=%3 家族, T=%4 周期")
        .arg(problem_size["num_plants"].toInt())
        .arg(problem_size["num_items"].toInt())
        .arg(problem_size["num_families"].toInt())
        .arg(problem_size["num_periods"].toInt()));
}

void OverviewPanel::Clear() {
    grand_cost_value_->setText("-");
    v_star_value_->setText("-");
    iterations_value_->setText("-");
    runtime_value_->setText("-");

    for (int i = 0; i < allocation_bars_.size(); ++i) {
        allocation_labels_[i]->setVisible(false);
        allocation_bars_[i]->setVisible(false);
        allocation_bars_[i]->setValue(0);
    }

    stability_title_->setText("");
    stability_desc_->setText("");
    problem_info_->setText("");
}
