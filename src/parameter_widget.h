// parameter_widget.h - 算法参数设置面板

#ifndef PARAMETER_WIDGET_H
#define PARAMETER_WIDGET_H

#include <QGroupBox>
#include <QVariantMap>

class QRadioButton;
class QDoubleSpinBox;
class QSpinBox;
class QPushButton;
class QToolButton;

class ParameterWidget : public QGroupBox {
    Q_OBJECT

public:
    explicit ParameterWidget(QWidget* parent = nullptr);

    // 获取所有参数
    QVariantMap GetParameters() const;

private slots:
    void ResetDefaults();

private:
    void SetupUI();

    // 算法参数
    QRadioButton* sp_heuristic_radio_;  // SP启发式
    QRadioButton* sp_exact_radio_;      // SP精确求解
    QDoubleSpinBox* time_limit_spin_;   // 总时限
    QDoubleSpinBox* mp_time_spin_;      // MP时限
    QDoubleSpinBox* sp_time_spin_;      // SP时限
    QDoubleSpinBox* epsilon_spin_;      // 收敛精度
    QSpinBox* max_iter_spin_;           // 最大迭代次数
    QPushButton* reset_button_;         // 重置按钮
};

#endif  // PARAMETER_WIDGET_H
