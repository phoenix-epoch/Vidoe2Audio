# 🌌 星空音频分离系统 (Starry Audio Extractor)

基于 **Qt (C++17)** 与 **FFmpeg** 打造的沉浸式星空科技感音视频音频分离与高精转码提取工具。

---

## ✨ 核心特性

1. **🌠 沉浸式星空科技视觉系统**：
   - 自研轻量级 `QPainter` 动态星空引擎（恒星微移、动态星云光晕、星光闪烁与随机流星掠过特效）。
   - 赛博朋克深空磨砂玻璃拟态 UI，搭配霓虹青 (`#00f0ff`) 与量子紫 (`#9d4edd`) 发光元素。
   - 炫酷发光渐变进度条、飞船控制台风格的实时彩色终端日志。

2. **⚡ 全能音频分离与提取 (FFmpeg 驱动)**：
   - **全面视频格式支持**：MP4, MKV, AVI, MOV, FLV, WMV, WEBM, TS, M4V, 3GP, RMVB, VOB, MPG, MPEG, OGV, MTS, M2TS 等。
   - **全面音频输出支持**：MP3, AAC, WAV, FLAC, M4A, OGG, OPUS, WMA, AC3, MKA, ALAC, AIFF 等。
   - **双提取模式**：
     - **⚡ 极速无损流拷贝 (`-c:a copy`)**：直接分离音轨，不进行二次重编码，毫秒级极速完成，100% 原始无损音质。
     - **🎛️ 格式重编码定制 (Transcode)**：自定义输出格式、码率 (320k/256k/192k/128k 等)、采样率 (48000/44100Hz 等) 及声道。
   - **实时精准进度**：多线程异步调用 `QProcess`，实时解析 FFmpeg 标准流时间戳换算百分比，UI 丝滑不卡顿。
   - **交互便利**：支持音视频文件**直接拖拽载入**，提取完成后支持一键“打开目录”与“播放音频”。
   - **FFmpeg 智能探测**：自动识别系统环境变量 `PATH` 与程序同级目录的 `ffmpeg.exe`，支持图形化选择。

---

## 🛠️ 项目结构

```
E:\AiToy\video/
├── CMakeLists.txt              # CMake 工程构建文件 (支持 Qt5 / Qt6)
├── StarryAudioExtractor.pro    # QMake 项目工程文件 (Qt Creator 原生支持)
├── src/
│   ├── main.cpp                # 应用程序入口与高 DPI 适配
│   ├── mainwindow.h            # 主界面定义
│   ├── mainwindow.cpp          # 主界面交互逻辑、拖拽与信号绑定
│   ├── starrybackground.h      # 动态星空粒子与流星特效头文件
│   ├── starrybackground.cpp    # 星空引擎渲染实现
│   ├── ffmpeghelper.h          # FFmpeg 进程调度、格式枚举与参数封装
│   ├── ffmpeghelper.cpp        # 异步进程执行与进度计算
│   └── style.h                 # 统一的星空科技感 QSS 样式定义
└── README.md                   # 本说明文档
```

---

## 🚀 编译与调试指南

本项目同时支持 **QMake** 与 **CMake**，兼容 **Qt 5 (5.12+)** 与 **Qt 6.x**，编译器支持 **MinGW** 或 **MSVC (2019/2022)**。

### 方式一：使用 Qt Creator (推荐)

1. 打开 **Qt Creator**。
2. 点击菜单栏 **【文件】 -> 【打开文件或项目】**。
3. 选择以下任意一个工程文件：
   - `E:\AiToy\video\StarryAudioExtractor.pro` (QMake 构建)
   - 或 `E:\AiToy\video\CMakeLists.txt` (CMake 构建)
4. 在 Kit 配置界面中勾选你的 Qt 编译套件（例如 `Desktop Qt 6.5.x MinGW 64-bit` 或 `MSVC2019 64-bit`）。
5. 点击左下角绿色 **【运行 (Ctrl+R)】** 或 **【调试 (F5)】** 按钮即可一键编译并启动！

### 方式二：命令行 CMake 构建

```bash
cd E:\AiToy\video
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

## 📦 FFmpeg 准备

本软件依赖 `ffmpeg.exe` 执行底层音视频解封装与转码：
1. **自动识别**：如果您已将 FFmpeg 安装并添加到了系统环境变量 `PATH`，软件启动后会自动检测到。
2. **免安装放置**：您也可以下载 Windows 版的 `ffmpeg.exe`，直接放置在编译生成的 `.exe` 同级目录下。
3. **手动指定**：也可以直接在软件界面右下角点击 **【🛠️ 指定 FFmpeg】** 按钮选择任意路径下的 `ffmpeg.exe`。
