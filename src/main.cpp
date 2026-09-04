#include <QApplication>
#include "mainwindow.h"
#include "style.h"

int main(int argc, char *argv[]) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt 6 高 DPI 屏幕平滑缩放策略 (避免 Windows 125%/150%/175% 缩放下字体和控件失真或像素取整跳跃)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
    // Qt 5 高 DPI 屏幕适配
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);

    // 应用程序元信息
    app.setApplicationName("StarryAudioExtractor");
    app.setApplicationDisplayName("🌌 星空音频分离系统 - Starry Audio Extractor");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("AiToy Studio");

    // 加载星空科技感 QSS 样式表
    app.setStyleSheet(StarryStyle::getApplicationStyle());

    // 启动主界面
    MainWindow w;
    w.show();

    return app.exec();
}
