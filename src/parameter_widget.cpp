// parameter_widget.cpp - 算法参数设置面板实现

#include "parameter_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>

ParameterWidget::ParameterWidget(QWidget* parent)
    : QGroupBox("参数设置", parent) {
    SetupUI();
    ResetDefaults();
}

void ParameterWidget::SetupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QFormLayout* form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight);

    // SP 求解策略 (单选按钮组)
    QWidget* sp_widget = new QWidget();
    QVBoxLayout* sp_layout = new QVBoxLayout(sp_widget);
    sp_layout->setContentsMargins(0, 0, 0, 0);
    sp_layout->setSpacing(2);

    // 单选按钮固定宽度 (对齐问号)
    const int kSpRadioWidth = 170;

    // 启发式选项
    QHBoxLayout* heuristic_row = new QHBoxLayout();
    heuristic_row->setContentsMargins(0, 0, 0, 0);
    heuristic_row->setSpacing(4);
    sp_heuristic_radio_ = new QRadioButton("启发式 - 枚举联盟候选");
    sp_heuristic_radio_->setFixedWidth(kSpRadioWidth);
    QToolButton* heuristic_help = new QToolButton();
    heuristic_help->setText("?");
    heuristic_help->setFixedSize(16, 16);
    heuristic_help->setStyleSheet("border: 1px solid #999; border-radius: 8px; font-size: 10px;");
    connect(heuristic_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("启发式策略");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "通过枚举可能违反的联盟候选来求解子问题。<br><br>"
            "<b>工作原理：</b><br>"
            "- 根据当前分配 π̂<sub>u</sub>，计算违反量 Δ = Σπ̂<sub>u</sub> - c(S)<br>"
            "- 选择 Δ 最大的联盟 S 添加约束<br><br>"
            "<b>适用场景：</b>|P| ≤ 5 时效率较高<br>"
            "<b>优点：</b>求解速度快，内存占用低<br>"
            "<b>缺点：</b>可能需要更多迭代次数");
        msg.exec();
    });
    heuristic_row->addWidget(sp_heuristic_radio_);
    heuristic_row->addWidget(heuristic_help);
    heuristic_row->addStretch();
    sp_layout->addLayout(heuristic_row);

    // 精确求解选项
    QHBoxLayout* exact_row = new QHBoxLayout();
    exact_row->setContentsMargins(0, 0, 0, 0);
    exact_row->setSpacing(4);
    sp_exact_radio_ = new QRadioButton("精确求解 - 完整MILP");
    sp_exact_radio_->setFixedWidth(kSpRadioWidth);
    QToolButton* exact_help = new QToolButton();
    exact_help->setText("?");
    exact_help->setFixedSize(16, 16);
    exact_help->setStyleSheet("border: 1px solid #999; border-radius: 8px; font-size: 10px;");
    connect(exact_help, &QToolButton::clicked, [this]() {
        QMessageBox msg(this);
        msg.setWindowTitle("精确求解策略");
        msg.setTextFormat(Qt::RichText);
        msg.setText(
            "通过求解完整的混合整数线性规划 (MILP) 模型找到最优违反联盟。<br><br>"
            "<b>工作原理：</b><br>"
            "- 构建包含所有可能联盟的MILP模型<br>"
            "- 求解 max<sub>S⊆P</sub> {Σ<sub>u∈S</sub>π̂<sub>u</sub> - c(S)}<br>"
            "- 使用CPLEX求解器找到全局最优解<br><br>"
            "<b>适用场景：</b>|P| > 5 或需要精确解时<br>"
            "<b>优点：</b>保证找到最优违反联盟，迭代次数少<br>"
            "<b>缺点：</b>单次迭代计算时间较长");
        msg.exec();
    });
    exact_row->addWidget(sp_exact_radio_);
    exact_row->addWidget(exact_help);
    exact_row->addStretch();
    sp_layout->addLayout(exact_row);

    form->addRow("子问题策略", sp_widget);

    // 收敛精度
    epsilon_spin_ = new QDoubleSpinBox(this);
    epsilon_spin_->setRange(1e-6, 1e-2);
    epsilon_spin_->setDecimals(4);
    epsilon_spin_->setSingleStep(1e-5);
    form->addRow("收敛精度", epsilon_spin_);

    // 最大迭代次数
    max_iter_spin_ = new QSpinBox(this);
    max_iter_spin_->setRange(1, 1000);
    form->addRow("最大迭代", max_iter_spin_);

    // 总时限
    time_limit_spin_ = new QDoubleSpinBox(this);
    time_limit_spin_->setRange(10, 36000);
    time_limit_spin_->setSuffix(" s");
    time_limit_spin_->setDecimals(0);
    time_limit_spin_->setSingleStep(60);
    form->addRow("总时限", time_limit_spin_);

    // MP 时限
    mp_time_spin_ = new QDoubleSpinBox(this);
    mp_time_spin_->setRange(1, 3600);
    mp_time_spin_->setSuffix(" s");
    mp_time_spin_->setDecimals(0);
    mp_time_spin_->setSingleStep(30);
    form->addRow("主问题时限", mp_time_spin_);

    // SP 时限
    sp_time_spin_ = new QDoubleSpinBox(this);
    sp_time_spin_->setRange(1, 3600);
    sp_time_spin_->setSuffix(" s");
    sp_time_spin_->setDecimals(0);
    sp_time_spin_->setSingleStep(30);
    form->addRow("子问题时限", sp_time_spin_);

    // 规划子问题时限
    planning_time_spin_ = new QDoubleSpinBox(this);
    planning_time_spin_->setRange(1, 3600);
    planning_time_spin_->setSuffix(" s");
    planning_time_spin_->setDecimals(0);
    planning_time_spin_->setSingleStep(30);
    form->addRow("规划时限", planning_time_spin_);

    // RF 参数 (启发式模式下使用)
    rf_window_spin_ = new QSpinBox(this);
    rf_window_spin_->setRange(1, 20);
    form->addRow("RF窗口大小", rf_window_spin_);

    rf_retries_spin_ = new QSpinBox(this);
    rf_retries_spin_->setRange(1, 10);
    form->addRow("RF重试次数", rf_retries_spin_);

    rf_sub_time_spin_ = new QDoubleSpinBox(this);
    rf_sub_time_spin_->setRange(1, 600);
    rf_sub_time_spin_->setSuffix(" s");
    rf_sub_time_spin_->setDecimals(0);
    rf_sub_time_spin_->setSingleStep(10);
    form->addRow("RF子问题时限", rf_sub_time_spin_);

    layout->addLayout(form);

    // 重置按钮
    reset_button_ = new QPushButton("重置", this);
    connect(reset_button_, &QPushButton::clicked, this, &ParameterWidget::ResetDefaults);
    layout->addWidget(reset_button_);

    layout->addStretch();
}

void ParameterWidget::ResetDefaults() {
    sp_heuristic_radio_->setChecked(true);
    epsilon_spin_->setValue(1e-4);
    max_iter_spin_->setValue(100);
    time_limit_spin_->setValue(3600);
    mp_time_spin_->setValue(300);
    sp_time_spin_->setValue(600);
    planning_time_spin_->setValue(300);
    rf_window_spin_->setValue(6);
    rf_retries_spin_->setValue(3);
    rf_sub_time_spin_->setValue(60);
}

QVariantMap ParameterWidget::GetParameters() const {
    QVariantMap params;

    // 算法参数
    params["sp_strategy"] = sp_exact_radio_->isChecked() ? "exact" : "heuristic";
    params["epsilon"] = epsilon_spin_->value();
    params["max_iterations"] = max_iter_spin_->value();

    // 时限参数
    params["time_limit"] = time_limit_spin_->value();
    params["mp_time_limit"] = mp_time_spin_->value();
    params["sp_time_limit"] = sp_time_spin_->value();
    params["planning_time_limit"] = planning_time_spin_->value();

    // RF 启发式参数
    params["rf_window"] = rf_window_spin_->value();
    params["rf_retries"] = rf_retries_spin_->value();
    params["rf_sub_time"] = rf_sub_time_spin_->value();

    return params;
}
