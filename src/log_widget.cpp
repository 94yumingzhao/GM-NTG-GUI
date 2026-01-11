// log_widget.cpp - 日志显示控件实现

#include "log_widget.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>

LogWidget::LogWidget(QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    text_edit_ = new QTextEdit();
    text_edit_->setReadOnly(true);
    text_edit_->setLineWrapMode(QTextEdit::NoWrap);
    text_edit_->setFont(QFont("Consolas", 9));
    text_edit_->setStyleSheet(
        "QTextEdit {"
        "  background-color: #1e1e1e;"
        "  color: #d4d4d4;"
        "  border: 1px solid #3c3c3c;"
        "}");

    layout->addWidget(text_edit_);
}

void LogWidget::Append(const QString& message) {
    // 根据消息类型设置颜色
    QString colored_message = message;

    if (message.contains("[ERROR]") || message.contains("错误")) {
        colored_message = QString("<span style='color:#f44336;'>%1</span>").arg(message.toHtmlEscaped());
    } else if (message.contains("[WARN]") || message.contains("警告")) {
        colored_message = QString("<span style='color:#ff9800;'>%1</span>").arg(message.toHtmlEscaped());
    } else if (message.contains("[INFO]")) {
        colored_message = QString("<span style='color:#4caf50;'>%1</span>").arg(message.toHtmlEscaped());
    } else if (message.contains("[CUT]")) {
        colored_message = QString("<span style='color:#2196f3;'>%1</span>").arg(message.toHtmlEscaped());
    } else if (message.contains("[GRAND]") || message.contains("[CONVERGE]") || message.contains("[DONE]")) {
        colored_message = QString("<span style='color:#9c27b0;'>%1</span>").arg(message.toHtmlEscaped());
    } else {
        colored_message = message.toHtmlEscaped();
    }

    text_edit_->append(colored_message);

    // 滚动到底部
    QScrollBar* scrollbar = text_edit_->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}

void LogWidget::Clear() {
    text_edit_->clear();
}

bool LogWidget::ExportToFile(const QString& filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream << text_edit_->toPlainText();
    file.close();
    return true;
}

QString LogWidget::GetText() const {
    return text_edit_->toPlainText();
}
