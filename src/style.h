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
        QGroupBox {
            background-color: rgba(13, 20, 38, 0.82);
            border: 1px solid rgba(0, 240, 255, 0.28);
            border-radius: 8px;
            margin-top: 18px;
            padding: 12px 10px 10px 10px;
            font-weight: bold;
            font-size: 13px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            top: 1px;
            padding: 2px 8px;
            color: #00f0ff;
            background-color: rgba(9, 14, 28, 0.95);
            border: 1px solid rgba(0, 240, 255, 0.45);
            border-radius: 4px;
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

        /* 首页双模式大卡片按钮 */
        QPushButton#modeCardLocal, QPushButton#modeCardUrl {
            background: rgba(12, 22, 45, 0.85);
            border: 2px solid rgba(0, 240, 255, 0.35);
            border-radius: 16px;
            color: #ffffff;
            font-size: 16px;
            font-weight: bold;
            padding: 24px 20px;
            text-align: center;
        }

        QPushButton#modeCardLocal:hover, QPushButton#modeCardUrl:hover {
            background: rgba(0, 240, 255, 0.15);
            border: 2px solid #00f0ff;
            color: #ffffff;
        }

        QPushButton#modeCardLocal:pressed, QPushButton#modeCardUrl:pressed {
            background: rgba(0, 240, 255, 0.28);
            border: 2px solid #38ef7d;
        }

        /* 返回导航按钮 */
        QPushButton#backToPortalBtn {
            background-color: rgba(14, 26, 50, 0.85);
            border: 1px solid rgba(0, 240, 255, 0.45);
            border-radius: 6px;
            color: #00f0ff;
            padding: 6px 14px;
            font-weight: bold;
            font-size: 12px;
        }

        QPushButton#backToPortalBtn:hover {
            background-color: rgba(0, 240, 255, 0.25);
            border: 1px solid #00f0ff;
            color: #ffffff;
        }

        /* 输入框 */
        QLineEdit {
            background-color: rgba(10, 16, 32, 0.85);
            border: 1px solid rgba(0, 240, 255, 0.35);
            border-radius: 6px;
            padding: 5px 8px;
            color: #ffffff;
            selection-background-color: #00f0ff;
            selection-color: #090e1c;
            min-height: 22px;
        }

        QLineEdit:hover {
            border: 1px solid rgba(0, 240, 255, 0.65);
            background-color: rgba(14, 22, 44, 0.92);
        }

        QLineEdit:focus {
            border: 1px solid #00f0ff;
            background-color: rgba(16, 26, 52, 0.98);
        }

        QLineEdit:disabled {
            background-color: rgba(15, 20, 32, 0.5);
            border: 1px solid rgba(100, 120, 140, 0.2);
            color: #64748b;
        }

        /* 下拉选择框 */
        QComboBox {
            background-color: rgba(10, 16, 32, 0.85);
            border: 1px solid rgba(0, 240, 255, 0.35);
            border-radius: 6px;
            padding: 4px 10px;
            color: #ffffff;
            min-height: 22px;
        }

        QComboBox:hover {
            border: 1px solid rgba(0, 240, 255, 0.7);
            background-color: rgba(14, 22, 44, 0.92);
        }

        QComboBox:focus {
            border: 1px solid #00f0ff;
        }

        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 22px;
            border-left: 1px solid rgba(0, 240, 255, 0.2);
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
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
            border: 1px solid #00f0ff;
            border-radius: 6px;
            selection-background-color: #1e3a8a;
            selection-color: #00f0ff;
            color: #e2e8f0;
            padding: 4px;
            outline: none;
        }

        /* 按钮通用 */
        QPushButton {
            background-color: rgba(14, 30, 60, 0.85);
            border: 1px solid rgba(0, 240, 255, 0.4);
            border-radius: 6px;
            color: #d1f2ff;
            padding: 5px 12px;
            font-weight: 500;
            min-height: 22px;
        }

        QPushButton:hover {
            background-color: rgba(0, 240, 255, 0.22);
            border: 1px solid #00f0ff;
            color: #ffffff;
        }

        QPushButton:pressed {
            background-color: rgba(0, 240, 255, 0.38);
            border: 1px solid #38ef7d;
        }

        QPushButton:disabled {
            background-color: rgba(20, 28, 44, 0.5);
            border: 1px solid rgba(80, 100, 120, 0.25);
            color: #556980;
        }

        /* 开始执行大按钮 */
        QPushButton#primaryActionBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0052d4, stop:0.5 #4364f7, stop:1 #6fb1fc);
            border: 1px solid #00f0ff;
            border-radius: 8px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 8px 20px;
            letter-spacing: 1px;
            min-height: 26px;
        }

        QPushButton#primaryActionBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0066ff, stop:0.5 #5777ff, stop:1 #00f0ff);
            border: 1px solid #ffffff;
            color: #ffffff;
        }

        QPushButton#primaryActionBtn:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #003bb3, stop:1 #2f4fc9);
        }

        QPushButton#cancelActionBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8b0032, stop:1 #e52d27);
            border: 1px solid #ff4b72;
            border-radius: 8px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 8px 18px;
            min-height: 26px;
        }

        QPushButton#cancelActionBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #b30040, stop:1 #ff4b72);
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
            border: 1px solid rgba(0, 240, 255, 0.5);
            border-radius: 7px;
            background-color: rgba(10, 16, 32, 0.8);
        }

        QRadioButton::indicator:checked {
            background-color: #00f0ff;
            border: 2px solid #090e1c;
        }

        /* 发光进度条 */
        QProgressBar {
            background-color: rgba(9, 14, 28, 0.9);
            border: 1px solid rgba(0, 240, 255, 0.35);
            border-radius: 7px;
            text-align: center;
            color: #ffffff;
            font-weight: bold;
            font-size: 11px;
            height: 20px;
            min-height: 20px;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #9d4edd, stop:0.4 #0077ff, stop:1 #00f0ff);
            border-radius: 6px;
        }

        /* 终端控制台日志区 */
        QTextEdit#consoleOutput {
            background-color: rgba(6, 10, 20, 0.94);
            border: 1px solid rgba(0, 240, 255, 0.3);
            border-radius: 6px;
            color: #94a3b8;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 11px;
            padding: 6px;
            line-height: 1.3;
        }

        /* 滚动条 */
        QScrollBar:vertical {
            background: rgba(10, 16, 32, 0.6);
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
    )");
}

} // namespace StarryStyle

#endif // STYLE_H
