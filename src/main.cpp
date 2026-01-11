// main.cpp - GM-NTG-GUI 入口
// 多工厂NTG博弈求解器图形界面

#include "main_window.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 设置应用信息
    app.setApplicationName("GM-NTG-GUI");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("YM-Code");

    // 使用 Fusion 风格
    app.setStyle(QStyleFactory::create("Fusion"));

    // 创建并显示主窗口
    MainWindow window;
    window.show();

    return app.exec();
}
