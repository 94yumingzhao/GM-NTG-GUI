// log_widget.h - 日志显示控件

#ifndef LOG_WIDGET_H
#define LOG_WIDGET_H

#include <QWidget>

class QTextEdit;

class LogWidget : public QWidget {
    Q_OBJECT

public:
    explicit LogWidget(QWidget* parent = nullptr);

    // 添加日志消息
    void Append(const QString& message);

    // 清空日志
    void Clear();

    // 导出到文件
    bool ExportToFile(const QString& filepath);

    // 获取全部文本
    QString GetText() const;

private:
    QTextEdit* text_edit_;
};

#endif  // LOG_WIDGET_H
