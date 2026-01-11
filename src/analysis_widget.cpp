// analysis_widget.cpp - 分析面板主控件实现

#include "analysis_widget.h"
#include "panels/overview_panel.h"
#include "panels/plant_panel.h"
#include "panels/coalition_panel.h"
#include "panels/iteration_panel.h"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>

AnalysisWidget::AnalysisWidget(QWidget* parent)
    : QWidget(parent) {
    SetupUI();
}

void AnalysisWidget::SetupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 状态标签
    status_label_ = new QLabel("请加载结果文件或运行求解器");
    status_label_->setStyleSheet("color: #999;");

    // Tab 控件 - 与主窗口风格一致
    tab_widget_ = new QTabWidget();
    tab_widget_->setStyleSheet(R"(
        QTabWidget::pane {
            border: none;
            border-top: 1px solid #e0e0e0;
        }
        QTabBar::tab {
            padding: 5px 14px;
            margin-right: 2px;
            border: none;
            background: transparent;
            color: #666;
        }
        QTabBar::tab:selected {
            color: #333;
            border-bottom: 2px solid #4CAF50;
        }
    )");

    // 创建子面板
    overview_panel_ = new OverviewPanel();
    plant_panel_ = new PlantPanel();
    coalition_panel_ = new CoalitionPanel();
    iteration_panel_ = new IterationPanel();

    tab_widget_->addTab(overview_panel_, "概览");
    tab_widget_->addTab(plant_panel_, "工厂视角");
    tab_widget_->addTab(coalition_panel_, "联盟分析");
    tab_widget_->addTab(iteration_panel_, "迭代历史");

    layout->addWidget(status_label_);
    layout->addWidget(tab_widget_);
}

void AnalysisWidget::LoadResult(const QJsonObject& result) {
    current_result_ = result;

    if (result.isEmpty()) {
        Clear();
        return;
    }

    // 隐藏提示
    status_label_->setVisible(false);

    // 加载到各个子面板
    overview_panel_->LoadData(result);
    plant_panel_->LoadData(result);
    coalition_panel_->LoadData(result);
    iteration_panel_->LoadData(result);
}

void AnalysisWidget::Clear() {
    current_result_ = QJsonObject();

    status_label_->setVisible(true);

    overview_panel_->Clear();
    plant_panel_->Clear();
    coalition_panel_->Clear();
    iteration_panel_->Clear();
}
