// generator_widget.h - 实例生成器控件
// 用于生成 GM-NTG-Core 测试实例

#ifndef GENERATOR_WIDGET_H
#define GENERATOR_WIDGET_H

#include <QWidget>

class QSpinBox;
class QDoubleSpinBox;
class QRadioButton;
class QTextEdit;
class QLabel;
class QPushButton;

class GeneratorWidget : public QWidget {
    Q_OBJECT

public:
    explicit GeneratorWidget(QWidget* parent = nullptr);

signals:
    void InstanceGenerated(const QString& file_path);

private slots:
    void OnGenerate();
    void OnSaveInstance();
    void OnUpdatePreview();

private:
    void SetupUI();
    QString GenerateInstanceCSV();

    // 问题规模参数
    QSpinBox* num_plants_spin_;
    QSpinBox* num_items_spin_;
    QSpinBox* num_families_spin_;
    QSpinBox* num_periods_spin_;

    // 成本参数
    QDoubleSpinBox* demand_mean_spin_;
    QDoubleSpinBox* demand_std_spin_;
    QDoubleSpinBox* capacity_factor_spin_;
    QDoubleSpinBox* setup_cost_spin_;
    QDoubleSpinBox* holding_cost_spin_;
    QDoubleSpinBox* transfer_cost_spin_;

    // 生成模式
    QRadioButton* pattern_uniform_radio_;
    QRadioButton* pattern_demand_radio_;
    QRadioButton* pattern_capacity_radio_;
    QRadioButton* pattern_transfer_radio_;

    // 预览和输出
    QTextEdit* preview_edit_;
    QLabel* status_label_;

    // 按钮
    QPushButton* generate_btn_;
    QPushButton* save_btn_;

    // 生成的实例内容
    QString generated_content_;
};

#endif  // GENERATOR_WIDGET_H
