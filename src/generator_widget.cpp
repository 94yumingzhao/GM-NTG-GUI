// generator_widget.cpp - 实例生成器控件实现
// 用于生成 GM-NTG-Core 测试实例

#include "generator_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QToolButton>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QSplitter>
#include <cmath>

GeneratorWidget::GeneratorWidget(QWidget* parent)
    : QWidget(parent) {
    SetupUI();
}

void GeneratorWidget::SetupUI() {
    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->setSpacing(8);
    main_layout->setContentsMargins(0, 0, 0, 0);

    // ========== 左侧: 参数设置 ==========
    QWidget* params_widget = new QWidget();
    params_widget->setMinimumWidth(280);
    params_widget->setMaximumWidth(400);
    QVBoxLayout* params_layout = new QVBoxLayout(params_widget);
    params_layout->setSpacing(8);
    params_layout->setContentsMargins(0, 0, 0, 0);

    // 问题规模
    QGroupBox* size_group = new QGroupBox("问题规模");
    QFormLayout* size_form = new QFormLayout(size_group);
    size_form->setLabelAlignment(Qt::AlignRight);

    num_plants_spin_ = new QSpinBox();
    num_plants_spin_->setRange(2, 10);
    num_plants_spin_->setValue(3);
    size_form->addRow("工厂数 P", num_plants_spin_);

    num_items_spin_ = new QSpinBox();
    num_items_spin_->setRange(1, 50);
    num_items_spin_->setValue(10);
    size_form->addRow("产品数 N", num_items_spin_);

    num_families_spin_ = new QSpinBox();
    num_families_spin_->setRange(1, 20);
    num_families_spin_->setValue(4);
    size_form->addRow("组别数 G", num_families_spin_);

    num_periods_spin_ = new QSpinBox();
    num_periods_spin_->setRange(1, 52);
    num_periods_spin_->setValue(12);
    size_form->addRow("时期数 T", num_periods_spin_);

    params_layout->addWidget(size_group);

    // 成本参数
    QGroupBox* cost_group = new QGroupBox("成本参数");
    QVBoxLayout* cost_layout = new QVBoxLayout(cost_group);
    cost_layout->setSpacing(4);

    QString help_style_small = "border: 1px solid #999; border-radius: 8px; font-size: 10px;";

    // 成本参数布局常量
    const int kCostLabelWidth = 70;
    const int kCostSpinWidth = 80;

    // 需求均值
    QHBoxLayout* demand_mean_row = new QHBoxLayout();
    demand_mean_row->setSpacing(4);
    QLabel* demand_mean_label = new QLabel("需求均值");
    demand_mean_label->setFixedWidth(kCostLabelWidth);
    demand_mean_spin_ = new QDoubleSpinBox();
    demand_mean_spin_->setRange(10, 1000);
    demand_mean_spin_->setValue(100);
    demand_mean_spin_->setFixedWidth(kCostSpinWidth);
    QToolButton* demand_mean_help = new QToolButton();
    demand_mean_help->setText("?");
    demand_mean_help->setFixedSize(16, 16);
    demand_mean_help->setStyleSheet(help_style_small);
    connect(demand_mean_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("需求均值");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "D<sub>uit</sub> 的期望值，即工厂 u 产品 i 在周期 t 的需求。<br><br>"
            "生成时：D<sub>uit</sub> ~ N(均值, 标准差)<br>"
            "建议范围：50 - 200");
        msg.exec();
    });
    demand_mean_row->addWidget(demand_mean_label);
    demand_mean_row->addWidget(demand_mean_spin_);
    demand_mean_row->addWidget(demand_mean_help);
    demand_mean_row->addStretch();
    cost_layout->addLayout(demand_mean_row);

    // 需求标准差
    QHBoxLayout* demand_std_row = new QHBoxLayout();
    demand_std_row->setSpacing(4);
    QLabel* demand_std_label = new QLabel("需求标准差");
    demand_std_label->setFixedWidth(kCostLabelWidth);
    demand_std_spin_ = new QDoubleSpinBox();
    demand_std_spin_->setRange(0, 500);
    demand_std_spin_->setValue(30);
    demand_std_spin_->setFixedWidth(kCostSpinWidth);
    QToolButton* demand_std_help = new QToolButton();
    demand_std_help->setText("?");
    demand_std_help->setFixedSize(16, 16);
    demand_std_help->setStyleSheet(help_style_small);
    connect(demand_std_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("需求标准差");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "D<sub>uit</sub> 的标准差，控制需求波动程度。<br><br>"
            "标准差越大，各工厂/产品/周期的需求差异越大。<br>"
            "设为 0 表示需求固定不变。<br>"
            "建议：约为均值的 20% - 30%");
        msg.exec();
    });
    demand_std_row->addWidget(demand_std_label);
    demand_std_row->addWidget(demand_std_spin_);
    demand_std_row->addWidget(demand_std_help);
    demand_std_row->addStretch();
    cost_layout->addLayout(demand_std_row);

    // 产能系数
    QHBoxLayout* cap_factor_row = new QHBoxLayout();
    cap_factor_row->setSpacing(4);
    QLabel* cap_factor_label = new QLabel("产能系数");
    cap_factor_label->setFixedWidth(kCostLabelWidth);
    capacity_factor_spin_ = new QDoubleSpinBox();
    capacity_factor_spin_->setRange(0.5, 3.0);
    capacity_factor_spin_->setValue(1.2);
    capacity_factor_spin_->setSingleStep(0.1);
    capacity_factor_spin_->setFixedWidth(kCostSpinWidth);
    QToolButton* cap_factor_help = new QToolButton();
    cap_factor_help->setText("?");
    cap_factor_help->setFixedSize(16, 16);
    cap_factor_help->setStyleSheet(help_style_small);
    connect(cap_factor_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("产能系数");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "总产能 ΣC<sub>ut</sub> 与总需求 ΣD<sub>uit</sub> 的比值。<br><br>"
            "= 1.0：产能刚好满足需求<br>"
            "> 1.0：产能有余量（推荐 1.1 ~ 1.3）<br>"
            "< 1.0：产能不足，问题可能无解");
        msg.exec();
    });
    cap_factor_row->addWidget(cap_factor_label);
    cap_factor_row->addWidget(capacity_factor_spin_);
    cap_factor_row->addWidget(cap_factor_help);
    cap_factor_row->addStretch();
    cost_layout->addLayout(cap_factor_row);

    // 换型成本
    QHBoxLayout* setup_row = new QHBoxLayout();
    setup_row->setSpacing(4);
    QLabel* setup_label = new QLabel("换型成本");
    setup_label->setFixedWidth(kCostLabelWidth);
    setup_cost_spin_ = new QDoubleSpinBox();
    setup_cost_spin_->setRange(10, 10000);
    setup_cost_spin_->setValue(500);
    setup_cost_spin_->setFixedWidth(kCostSpinWidth);
    QToolButton* setup_help = new QToolButton();
    setup_help->setText("?");
    setup_help->setFixedSize(16, 16);
    setup_help->setStyleSheet(help_style_small);
    connect(setup_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("换型成本");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "c<sup>Y</sup><sub>g</sub>：切换生产不同产品组别 g 时的固定成本。<br><br>"
            "包括设备调整、清洗、校准等。<br>"
            "c<sup>Y</sup><sub>g</sub> 较大 → 鼓励批量生产，减少换型次数");
        msg.exec();
    });
    setup_row->addWidget(setup_label);
    setup_row->addWidget(setup_cost_spin_);
    setup_row->addWidget(setup_help);
    setup_row->addStretch();
    cost_layout->addLayout(setup_row);

    // 库存成本
    QHBoxLayout* holding_row = new QHBoxLayout();
    holding_row->setSpacing(4);
    QLabel* holding_label = new QLabel("库存成本");
    holding_label->setFixedWidth(kCostLabelWidth);
    holding_cost_spin_ = new QDoubleSpinBox();
    holding_cost_spin_->setRange(0.01, 10);
    holding_cost_spin_->setValue(0.5);
    holding_cost_spin_->setSingleStep(0.1);
    holding_cost_spin_->setFixedWidth(kCostSpinWidth);
    QToolButton* holding_help = new QToolButton();
    holding_help->setText("?");
    holding_help->setFixedSize(16, 16);
    holding_help->setStyleSheet(help_style_small);
    connect(holding_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("库存成本");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "c<sup>I</sup><sub>i</sub>：产品 i 每单位每时期的持有成本。<br><br>"
            "包括仓储、资金占用、折旧等。<br>"
            "c<sup>I</sup><sub>i</sub> 较大 → 鼓励即时生产，减少库存");
        msg.exec();
    });
    holding_row->addWidget(holding_label);
    holding_row->addWidget(holding_cost_spin_);
    holding_row->addWidget(holding_help);
    holding_row->addStretch();
    cost_layout->addLayout(holding_row);

    // 转运成本
    QHBoxLayout* transfer_cost_row = new QHBoxLayout();
    transfer_cost_row->setSpacing(4);
    QLabel* transfer_label = new QLabel("转运成本");
    transfer_label->setFixedWidth(kCostLabelWidth);
    transfer_cost_spin_ = new QDoubleSpinBox();
    transfer_cost_spin_->setRange(0, 100);
    transfer_cost_spin_->setValue(5);
    transfer_cost_spin_->setFixedWidth(kCostSpinWidth);
    QToolButton* transfer_help = new QToolButton();
    transfer_help->setText("?");
    transfer_help->setFixedSize(16, 16);
    transfer_help->setStyleSheet(help_style_small);
    connect(transfer_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("转运成本");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "c<sup>T</sup><sub>uv</sub>：从工厂 u 到工厂 v 每单位产品的调拨成本。<br><br>"
            "包括运输、装卸、损耗等。<br>"
            "c<sup>T</sup><sub>uv</sub> 较大 → 鼓励本地生产，减少跨厂调拨");
        msg.exec();
    });
    transfer_cost_row->addWidget(transfer_label);
    transfer_cost_row->addWidget(transfer_cost_spin_);
    transfer_cost_row->addWidget(transfer_help);
    transfer_cost_row->addStretch();
    cost_layout->addLayout(transfer_cost_row);

    params_layout->addWidget(cost_group);

    // 生成模式
    QGroupBox* mode_group = new QGroupBox("生成模式");
    QVBoxLayout* mode_layout = new QVBoxLayout(mode_group);
    mode_layout->setSpacing(4);

    // 帮助按钮样式
    QString help_style = "border: 1px solid #999; border-radius: 8px; font-size: 10px;";

    // 单选按钮固定宽度 (对齐问号)
    const int kModeRadioWidth = 200;

    // 均匀分布
    QHBoxLayout* uniform_row = new QHBoxLayout();
    uniform_row->setContentsMargins(0, 0, 0, 0);
    uniform_row->setSpacing(4);
    pattern_uniform_radio_ = new QRadioButton("需求/产能均匀（基准模式）");
    pattern_uniform_radio_->setFixedWidth(kModeRadioWidth);
    pattern_uniform_radio_->setChecked(true);
    QToolButton* uniform_help = new QToolButton();
    uniform_help->setText("?");
    uniform_help->setFixedSize(16, 16);
    uniform_help->setStyleSheet(help_style);
    connect(uniform_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("需求/产能均匀（基准模式）");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "所有工厂的需求和产能分布相对均匀。<br><br>"
            "<b>特点：</b><br>"
            "- 各工厂 D<sub>uit</sub> 相近，服从正态分布<br>"
            "- 各工厂 C<sub>ut</sub> 相同<br>"
            "- 转运成本 c<sup>T</sup><sub>uv</sub> 适中<br><br>"
            "<b>用途：</b>作为基准测试，验证算法基本正确性");
        msg.exec();
    });
    uniform_row->addWidget(pattern_uniform_radio_);
    uniform_row->addWidget(uniform_help);
    uniform_row->addStretch();
    mode_layout->addLayout(uniform_row);

    // 需求集中
    QHBoxLayout* demand_row = new QHBoxLayout();
    demand_row->setContentsMargins(0, 0, 0, 0);
    demand_row->setSpacing(4);
    pattern_demand_radio_ = new QRadioButton("需求集中（部分工厂需求高）");
    pattern_demand_radio_->setFixedWidth(kModeRadioWidth);
    QToolButton* demand_help = new QToolButton();
    demand_help->setText("?");
    demand_help->setFixedSize(16, 16);
    demand_help->setStyleSheet(help_style);
    connect(demand_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("需求集中（部分工厂需求高）");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "部分工厂的需求显著高于其他工厂。<br><br>"
            "<b>特点：</b><br>"
            "- D<sub>0it</sub> = 4 × D<sub>uit</sub> (u > 0)<br>"
            "- 产能 C<sub>ut</sub> 分布均匀<br>"
            "- 高需求工厂可能需要从其他工厂转运<br><br>"
            "<b>用途：</b>测试负载均衡和转运决策能力");
        msg.exec();
    });
    demand_row->addWidget(pattern_demand_radio_);
    demand_row->addWidget(demand_help);
    demand_row->addStretch();
    mode_layout->addLayout(demand_row);

    // 产能不均
    QHBoxLayout* capacity_row = new QHBoxLayout();
    capacity_row->setContentsMargins(0, 0, 0, 0);
    capacity_row->setSpacing(4);
    pattern_capacity_radio_ = new QRadioButton("产能不均（工厂产能差异大）");
    pattern_capacity_radio_->setFixedWidth(kModeRadioWidth);
    QToolButton* capacity_help = new QToolButton();
    capacity_help->setText("?");
    capacity_help->setFixedSize(16, 16);
    capacity_help->setStyleSheet(help_style);
    connect(capacity_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("产能不均（工厂产能差异大）");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "各工厂的生产产能存在显著差异。<br><br>"
            "<b>特点：</b><br>"
            "- 需求 D<sub>uit</sub> 分布均匀<br>"
            "- C<sub>ut</sub> ∈ [0.5C̄, 2.0C̄]，线性递增<br>"
            "- 低产能工厂可能无法满足本地需求<br><br>"
            "<b>用途：</b>测试产能受限下的生产和转运协调");
        msg.exec();
    });
    capacity_row->addWidget(pattern_capacity_radio_);
    capacity_row->addWidget(capacity_help);
    capacity_row->addStretch();
    mode_layout->addLayout(capacity_row);

    // 高转运成本
    QHBoxLayout* transfer_mode_row = new QHBoxLayout();
    transfer_mode_row->setContentsMargins(0, 0, 0, 0);
    transfer_mode_row->setSpacing(4);
    pattern_transfer_radio_ = new QRadioButton("转运成本高（跨厂调拨昂贵）");
    pattern_transfer_radio_->setFixedWidth(kModeRadioWidth);
    QToolButton* transfer_mode_help = new QToolButton();
    transfer_mode_help->setText("?");
    transfer_mode_help->setFixedSize(16, 16);
    transfer_mode_help->setStyleSheet(help_style);
    connect(transfer_mode_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("转运成本高（跨厂调拨昂贵）");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "工厂间的转运成本较高。<br><br>"
            "<b>特点：</b><br>"
            "- 需求 D<sub>uit</sub> 和产能 C<sub>ut</sub> 分布均匀<br>"
            "- c<sup>T</sup><sub>uv</sub> = 3 × c<sup>T</sup><sub>base</sub><br>"
            "- 鼓励本地生产而非跨厂调拨<br><br>"
            "<b>用途：</b>测试本地生产 vs 转运的权衡决策");
        msg.exec();
    });
    transfer_mode_row->addWidget(pattern_transfer_radio_);
    transfer_mode_row->addWidget(transfer_mode_help);
    transfer_mode_row->addStretch();
    mode_layout->addLayout(transfer_mode_row);

    params_layout->addWidget(mode_group);

    // 按钮
    QHBoxLayout* btn_layout = new QHBoxLayout();
    generate_btn_ = new QPushButton("生成算例");
    generate_btn_->setMinimumHeight(32);
    generate_btn_->setStyleSheet("font-weight: bold;");
    connect(generate_btn_, &QPushButton::clicked, this, &GeneratorWidget::OnGenerate);

    save_btn_ = new QPushButton("保存文件");
    save_btn_->setMinimumHeight(32);
    save_btn_->setEnabled(false);
    connect(save_btn_, &QPushButton::clicked, this, &GeneratorWidget::OnSaveInstance);

    btn_layout->addWidget(generate_btn_);
    btn_layout->addWidget(save_btn_);
    params_layout->addLayout(btn_layout);

    params_layout->addStretch();

    // ========== 右侧: 预览 ==========
    QGroupBox* preview_group = new QGroupBox("算例预览");
    QVBoxLayout* preview_layout = new QVBoxLayout(preview_group);

    preview_edit_ = new QTextEdit();
    preview_edit_->setReadOnly(true);
    preview_edit_->setFont(QFont("Consolas", 9));

    status_label_ = new QLabel();
    status_label_->setStyleSheet("color: #666;");

    preview_layout->addWidget(preview_edit_);
    preview_layout->addWidget(status_label_);

    // Splitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(params_widget);
    splitter->addWidget(preview_group);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setHandleWidth(8);
    splitter->setStyleSheet(
        "QSplitter::handle { background-color: transparent; "
        "    border-left: 1px solid #e0e0e0; margin-left: 3px; margin-right: 4px; }"
        "QSplitter::handle:hover { border-left: 1px solid #bdbdbd; }"
        "QSplitter::handle:pressed { border-left: 1px solid #9e9e9e; }");

    main_layout->addWidget(splitter);
}

void GeneratorWidget::OnGenerate() {
    generated_content_ = GenerateInstanceCSV();
    preview_edit_->setPlainText(generated_content_);
    save_btn_->setEnabled(true);

    int lines = generated_content_.count('\n') + 1;
    status_label_->setText(QString("已生成算例: %1 行").arg(lines));
}

void GeneratorWidget::OnSaveInstance() {
    if (generated_content_.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先生成算例");
        return;
    }

    QString file_path = QFileDialog::getSaveFileName(
        this, "保存算例文件", "", "CSV 文件 (*.csv);;所有文件 (*)");

    if (file_path.isEmpty()) return;

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法保存文件: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << generated_content_;
    file.close();

    status_label_->setText(QString("已保存: %1").arg(file_path));
    emit InstanceGenerated(file_path);
}

void GeneratorWidget::OnUpdatePreview() {
    // 可以在参数改变时自动更新预览
}

QString GeneratorWidget::GenerateInstanceCSV() {
    QRandomGenerator* rng = QRandomGenerator::global();

    int P = num_plants_spin_->value();
    int N = num_items_spin_->value();
    int G = num_families_spin_->value();
    int T = num_periods_spin_->value();

    double demand_mean = demand_mean_spin_->value();
    double demand_std = demand_std_spin_->value();
    double cap_factor = capacity_factor_spin_->value();
    double setup_cost = setup_cost_spin_->value();
    double holding_cost = holding_cost_spin_->value();
    double transfer_cost = transfer_cost_spin_->value();

    // 获取选中的模式
    int pattern = 0;
    if (pattern_demand_radio_->isChecked()) pattern = 1;
    else if (pattern_capacity_radio_->isChecked()) pattern = 2;
    else if (pattern_transfer_radio_->isChecked()) pattern = 3;

    QString csv;
    QTextStream out(&csv);

    // 文件头
    out << "# GM-NTG-Core Multi-Plant Lot Sizing Game Data File\n";
    out << "# Generated by GM-NTG-GUI\n";
    out << "#\n";

    // Section 1: 维度
    out << "[DIMENSIONS]\n";
    out << "P," << P << "\n";
    out << "N," << N << "\n";
    out << "G," << G << "\n";
    out << "T," << T << "\n";
    out << "\n";

    // 计算总需求 (用于确定产能)
    double total_demand = 0;

    // Section 3: 需求 D[u][i][t]
    out << "[DEMAND]\n";
    out << "# u,i,t,value\n";

    QVector<QVector<QVector<double>>> demand(P, QVector<QVector<double>>(N, QVector<double>(T, 0)));

    for (int u = 0; u < P; ++u) {
        double plant_factor = 1.0;
        if (pattern == 1) {
            // 需求集中模式: 第一个工厂需求高
            plant_factor = (u == 0) ? 2.0 : 0.5;
        }

        for (int i = 0; i < N; ++i) {
            for (int t = 0; t < T; ++t) {
                // Box-Muller 正态分布
                double u1 = rng->generateDouble();
                double u2 = rng->generateDouble();
                double z = std::sqrt(-2.0 * std::log(u1 + 1e-10)) * std::cos(2 * M_PI * u2);
                double d = demand_mean * plant_factor + demand_std * z;
                d = std::max(0.0, d);

                demand[u][i][t] = d;
                total_demand += d;

                out << u << "," << i << "," << t << "," << QString::number(d, 'f', 1) << "\n";
            }
        }
    }
    out << "\n";

    // Section 2: 产能 C[u][t]
    out << "[CAPACITY]\n";
    out << "# u,t,value\n";

    double avg_period_demand = total_demand / T;
    double base_capacity = avg_period_demand * cap_factor / P;

    for (int u = 0; u < P; ++u) {
        double cap_mult = 1.0;
        if (pattern == 2) {
            // 产能不均模式
            cap_mult = 0.5 + (double)u / P * 1.5;
        }

        for (int t = 0; t < T; ++t) {
            double cap = base_capacity * cap_mult;
            out << u << "," << t << "," << QString::number(cap, 'f', 1) << "\n";
        }
    }
    out << "\n";

    // Section 4: 初始库存 I0[u][i]
    out << "[INIT_INVENTORY]\n";
    out << "# u,i,value\n";
    for (int u = 0; u < P; ++u) {
        for (int i = 0; i < N; ++i) {
            double init_inv = demand_mean * 0.1 * rng->generateDouble();
            out << u << "," << i << "," << QString::number(init_inv, 'f', 1) << "\n";
        }
    }
    out << "\n";

    // Section 5: 产品-家族映射 h[i][g]
    out << "[ITEM_FAMILY]\n";
    out << "# i,g\n";
    for (int i = 0; i < N; ++i) {
        int g = i % G;  // 简单循环分配
        out << i << "," << g << "\n";
    }
    out << "\n";

    // Section 6: 生产参数
    out << "[PRODUCTION]\n";
    out << "# i,cap_usage,cost\n";
    for (int i = 0; i < N; ++i) {
        double cap_usage = 1.0 + 0.5 * rng->generateDouble();
        double prod_cost = 5.0 + 10.0 * rng->generateDouble();
        out << i << "," << QString::number(cap_usage, 'f', 2)
            << "," << QString::number(prod_cost, 'f', 2) << "\n";
    }
    out << "\n";

    // Section 7: 换型参数
    out << "[SETUP]\n";
    out << "# g,cap_usage,cost\n";
    for (int g = 0; g < G; ++g) {
        double cap_usage = 30.0 + 20.0 * rng->generateDouble();
        double cost = setup_cost * (0.8 + 0.4 * rng->generateDouble());
        out << g << "," << QString::number(cap_usage, 'f', 2)
            << "," << QString::number(cost, 'f', 2) << "\n";
    }
    out << "\n";

    // Section 8: 库存成本
    out << "[HOLDING]\n";
    out << "# i,cost\n";
    for (int i = 0; i < N; ++i) {
        double cost = holding_cost * (0.8 + 0.4 * rng->generateDouble());
        out << i << "," << QString::number(cost, 'f', 3) << "\n";
    }
    out << "\n";

    // Section 9: 转运成本
    out << "[TRANSFER]\n";
    out << "# u,v,cost (uniform)\n";
    for (int u = 0; u < P; ++u) {
        for (int v = 0; v < P; ++v) {
            if (u != v) {
                double cost = transfer_cost;
                if (pattern == 3) {
                    // 高转运成本模式
                    cost *= 3.0;
                }
                cost *= (0.8 + 0.4 * rng->generateDouble());
                out << u << "," << v << "," << QString::number(cost, 'f', 2) << "\n";
            }
        }
    }
    out << "\n";

    // Section 10: Big-M
    out << "[BIG_M]\n";
    out << "# i,value\n";
    for (int i = 0; i < N; ++i) {
        double big_m = demand_mean * T * 2;  // 保守上界
        out << i << "," << QString::number(big_m, 'f', 0) << "\n";
    }

    return csv;
}
