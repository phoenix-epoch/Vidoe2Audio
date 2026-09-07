#ifndef STYLE_H
#define STYLE_H

#include <QString>

namespace StarryStyle {

inline QString getApplicationStyle() {
    return QString(R"(
        /* 全局字体与基础设定 */
        QWidget {
            font-family: "Segoe UI", "Microsoft YaHei", "PingFang SC", sans-serif;
            color: #d1e8ff;
            font-size: 13px;
        }

        /* 主窗口与透明画布 */
        QMainWindow {
            background-color: transparent;
        }

        /* 滚动区域透明化 */
        QScrollArea {
            background-color: transparent;
            border: none;
        }

        QScrollArea > QWidget > QWidget {
            background-color: transparent;
        }

        /* 科技感半透明卡片容器 (Glassmorphism) */
        /* 科技感半透明卡片容器 (Glassmorphism 柔和微光) */
        QGroupBox {
            background-color: rgba(11, 18, 36, 0.78);
            border: 1px solid rgba(0, 240, 255, 0.20);
            border-radius: 12px;
            margin-top: 18px;
            padding: 14px 12px 12px 12px;
            font-weight: bold;
            font-size: 13px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 14px;
            top: 1px;
            padding: 2px 10px;
            color: #38bdf8;
            background-color: rgba(8, 14, 30, 0.96);
            border: 1px solid rgba(0, 240, 255, 0.35);
            border-radius: 6px;
        }

        /* 标签 */
        QLabel {
            color: #c0daf5;
            background: transparent;
        }
        
        QLabel#headerTitle {
            font-size: 22px;
            font-weight: bold;
            color: #00f0ff;
            letter-spacing: 1px;
        }

        QLabel#headerSubtitle {
            font-size: 12px;
            color: #7b9cc4;
        }

        QLabel#portalTitle {
            font-size: 28px;
            font-weight: bold;
            color: #00f0ff;
            letter-spacing: 2px;
        }

        QLabel#portalSubtitle {
            font-size: 14px;
            color: #94a3b8;
        }

        /* 首页双模式大卡片按钮 (柔和超椭圆流光卡片) */
        QPushButton#modeCardLocal, QPushButton#modeCardUrl {
            background: rgba(11, 20, 42, 0.78);
            border: 1.5px solid rgba(0, 240, 255, 0.25);
            border-radius: 18px;
            color: #ffffff;
            font-size: 16px;
            font-weight: bold;
            padding: 24px 20px;
            text-align: center;
        }

        QPushButton#modeCardLocal:hover, QPushButton#modeCardUrl:hover {
            background: rgba(0, 240, 255, 0.12);
            border: 1.5px solid rgba(0, 240, 255, 0.85);
            color: #ffffff;
        }

        QPushButton#modeCardLocal:pressed, QPushButton#modeCardUrl:pressed {
            background: rgba(0, 240, 255, 0.22);
            border: 1.5px solid #38ef7d;
        }

        /* 返回导航按钮 */
        QPushButton#backToPortalBtn, QPushButton#backFromLocalBtn, QPushButton#backFromUrlBtn {
            background-color: rgba(14, 26, 50, 0.85);
            border: 1px solid rgba(0, 240, 255, 0.35);
            border-radius: 8px;
            color: #00f0ff;
            padding: 6px 14px;
            font-weight: bold;
            font-size: 12px;
        }

        QPushButton#backToPortalBtn:hover, QPushButton#backFromLocalBtn:hover, QPushButton#backFromUrlBtn:hover {
            background-color: rgba(0, 240, 255, 0.20);
            border: 1px solid #00f0ff;
            color: #ffffff;
        }

        /* 输入框 (柔和圆角) */
        QLineEdit {
            background-color: rgba(9, 15, 30, 0.82);
            border: 1px solid rgba(0, 240, 255, 0.22);
            border-radius: 8px;
            padding: 5px 10px;
            color: #ffffff;
            selection-background-color: #00f0ff;
            selection-color: #090e1c;
            min-height: 24px;
        }

        QLineEdit:hover {
            border: 1px solid rgba(0, 240, 255, 0.50);
            background-color: rgba(12, 20, 40, 0.90);
        }

        QLineEdit:focus {
            border: 1px solid #00f0ff;
            background-color: rgba(14, 24, 48, 0.96);
        }

        QLineEdit:disabled {
            background-color: rgba(15, 20, 32, 0.45);
            border: 1px solid rgba(100, 120, 140, 0.18);
            color: #64748b;
        }

        /* 下拉选择框 (柔和圆角) */
        QComboBox {
            background-color: rgba(9, 15, 30, 0.82);
            border: 1px solid rgba(0, 240, 255, 0.22);
            border-radius: 8px;
            padding: 4px 12px;
            color: #ffffff;
            min-height: 24px;
        }

        QComboBox:hover {
            border: 1px solid rgba(0, 240, 255, 0.55);
            background-color: rgba(12, 20, 40, 0.90);
        }

        QComboBox:focus {
            border: 1px solid #00f0ff;
        }

        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 24px;
            border-left: 1px solid rgba(0, 240, 255, 0.18);
            border-top-right-radius: 8px;
            border-bottom-right-radius: 8px;
        }

        QComboBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 5px solid #00f0ff;
            width: 0px;
            height: 0px;
            margin-right: 4px;
        }

        QComboBox QAbstractItemView {
            background-color: #0b1122;
            border: 1px solid rgba(0, 240, 255, 0.45);
            border-radius: 8px;
            selection-background-color: #1e3a8a;
            selection-color: #00f0ff;
            color: #e2e8f0;
            padding: 4px;
            outline: none;
        }

        /* 按钮通用 (柔和 8px 圆角) */
        QPushButton {
            background-color: rgba(14, 28, 56, 0.80);
            border: 1px solid rgba(0, 240, 255, 0.30);
            border-radius: 8px;
            color: #d1f2ff;
            padding: 5px 14px;
            font-weight: 500;
            min-height: 24px;
        }

        QPushButton:hover {
            background-color: rgba(0, 240, 255, 0.18);
            border: 1px solid rgba(0, 240, 255, 0.70);
            color: #ffffff;
        }

        QPushButton:pressed {
            background-color: rgba(0, 240, 255, 0.32);
            border: 1px solid #38ef7d;
        }

        QPushButton:disabled {
            background-color: rgba(20, 28, 44, 0.45);
            border: 1px solid rgba(80, 100, 120, 0.20);
            color: #556980;
        }

        /* 开始执行主按钮 (流光渐变 10px 柔角) */
        QPushButton#primaryActionBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0284c7, stop:0.5 #3b82f6, stop:1 #6366f1);
            border: 1px solid rgba(0, 240, 255, 0.75);
            border-radius: 10px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 8px 20px;
            letter-spacing: 1px;
            min-height: 28px;
        }

        QPushButton#primaryActionBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0ea5e9, stop:0.5 #60a5fa, stop:1 #818cf8);
            border: 1px solid #ffffff;
            color: #ffffff;
        }

        QPushButton#primaryActionBtn:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0369a1, stop:1 #1d4ed8);
        }

        QPushButton#cancelActionBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #9f1239, stop:1 #e11d48);
            border: 1px solid rgba(255, 75, 114, 0.75);
            border-radius: 10px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 8px 18px;
            min-height: 28px;
        }

        QPushButton#cancelActionBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #be123c, stop:1 #f43f5e);
            border: 1px solid #ffffff;
        }

        /* 单选框与复选框 */
        QRadioButton, QCheckBox {
            color: #cbd5e1;
            spacing: 6px;
        }

        QRadioButton:hover, QCheckBox:hover {
            color: #00f0ff;
        }

        QRadioButton::indicator {
            width: 14px;
            height: 14px;
            border: 1px solid rgba(0, 240, 255, 0.45);
            border-radius: 7px;
            background-color: rgba(10, 16, 32, 0.8);
        }

        QRadioButton::indicator:checked {
            background-color: #00f0ff;
            border: 2px solid #090e1c;
        }

        /* 柔和发光进度条 (9px 柔角) */
        QProgressBar {
            background-color: rgba(8, 13, 26, 0.92);
            border: 1px solid rgba(0, 240, 255, 0.22);
            border-radius: 9px;
            text-align: center;
            color: #ffffff;
            font-weight: bold;
            font-size: 11px;
            height: 20px;
            min-height: 20px;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8b5cf6, stop:0.45 #0ea5e9, stop:1 #00f0ff);
            border-radius: 8px;
        }

        /* 终端控制台日志区 (10px 柔角) */
        QTextEdit#consoleLog, QTextEdit#urlConsoleLog, QTextEdit#consoleOutput {
            background-color: rgba(5, 9, 18, 0.92);
            border: 1px solid rgba(0, 240, 255, 0.20);
            border-radius: 10px;
            color: #94a3b8;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 11px;
            padding: 8px;
            line-height: 1.35;
        }

        /* 滚动条 */
        QScrollBar:vertical {
            background: rgba(10, 16, 32, 0.5);
            width: 7px;
            margin: 0px;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical {
            background: rgba(0, 240, 255, 0.35);
            min-height: 20px;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical:hover {
            background: rgba(0, 240, 255, 0.7);
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QToolTip {
            background-color: #0f172a;
            color: #00f0ff;
            border: 1px solid #00f0ff;
            padding: 5px;
            border-radius: 4px;
            font-size: 11px;
        }

        /* ========================================================
           🌌 星空科技感自定义标题栏
           ======================================================== */
        QWidget#starryTitleBar {
            background-color: rgba(6, 11, 24, 0.94);
        }

        QLabel#titleIconLabel {
            font-size: 16px;
            padding-left: 2px;
        }

        QLabel#titleMainLabel {
            font-size: 13px;
            font-weight: bold;
            color: #00f0ff;
            letter-spacing: 1px;
        }

        QLabel#titleSubLabel {
            font-size: 10px;
            font-weight: bold;
            color: #64748b;
            letter-spacing: 0.8px;
            margin-left: 3px;
        }

        QLabel#titleBadgeLabel {
            font-size: 10px;
            font-weight: bold;
            color: #38ef7d;
            background-color: rgba(56, 239, 125, 0.12);
            border: 1px solid rgba(56, 239, 125, 0.38);
            border-radius: 4px;
            padding: 1px 6px;
            margin-left: 4px;
        }

        QPushButton#titleBtnMin, QPushButton#titleBtnMax {
            background-color: transparent;
            border: none;
            border-radius: 5px;
            color: #94a3b8;
            font-family: "Segoe UI Symbol", "Segoe UI", "Consolas", sans-serif;
            font-size: 13px;
            font-weight: bold;
            min-width: 42px;
            max-width: 42px;
            min-height: 28px;
            max-height: 28px;
            padding: 0px;
        }

        QPushButton#titleBtnMin:hover, QPushButton#titleBtnMax:hover {
            background-color: rgba(0, 240, 255, 0.22);
            color: #00f0ff;
        }

        QPushButton#titleBtnMin:pressed, QPushButton#titleBtnMax:pressed {
            background-color: rgba(0, 240, 255, 0.38);
            color: #ffffff;
        }

        QPushButton#titleBtnClose {
            background-color: transparent;
            border: none;
            border-radius: 5px;
            color: #94a3b8;
            font-family: "Segoe UI Symbol", "Segoe UI", "Consolas", sans-serif;
            font-size: 13px;
            font-weight: bold;
            min-width: 42px;
            max-width: 42px;
            min-height: 28px;
            max-height: 28px;
            padding: 0px;
        }

        QPushButton#titleBtnClose:hover {
            background-color: #ff3366;
            color: #ffffff;
        }

        QPushButton#titleBtnClose:pressed {
            background-color: #d91b4c;
            color: #ffffff;
        }
    )");
}

} // namespace StarryStyle

#endif // STYLE_H
