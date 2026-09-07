# 🌌 星空音频分离系统 (Starry Audio Extractor)

基于 **Qt (C++17)**、**FFmpeg** 与 **yt-dlp** 打造的沉浸式星空科技感音视频音频分离与全网流媒体提取工具。

---

## ✨ 核心亮点与功能特性

### 1. 🌠 首页双模式星空选择大厅
- 采用 **QStackedWidget** 架构，启动进入全屏流星粒子深空导航大厅；
- 提供两枚赛博发光大卡片供用户自由切换工作台：
  - `📁 本地音视频提取模式`
  - `🌐 网络网址提取模式`
- 各工作台顶部均支持 `[ ⬅️ 返回模式选择 ]` 随心流转。

### 2. 📁 本地音视频提取工作台
- **全格式视频兼容**：支持 MP4, MKV, AVI, MOV, FLV, WMV, WEBM, TS, M4V, 3GP, RMVB, VOB, MPG 等 20+ 种格式；
- **智能格式后缀诊断**：直接在输入框输入扩展名（如 `mkv`、`rmvb`、`.ts`），系统瞬间反馈支持状态、容器类型与推荐提取模式；
- **动态音频大小预估**：载入视频后，根据视频时长与用户选择的目标格式/码率（320k/256k/192k/WAV/FLAC 等）**实时精准推算输出文件体积**；
- **小白级界面减负**：彻底隐去底层 FFmpeg 路径设置，后台静默自动调度；
- **输出默认保存至桌面**：提取完成自动出现在用户电脑桌面上，开箱即视。

### 3. 🌐 网络网址提取工作台 (方案 A)
- **海量流媒体平台支持**：基于 `yt-dlp` + 内置 `FFmpeg` 黄金搭档，支持 B站、抖音、YouTube、小红书、微博、Twitter 以及任意 m3u8 / mp4 直链；
- **一键粘贴与剪贴板识别**：进入界面自动侦测剪贴板网址，支持一键粘贴；
- **智能文件名解析**：自动解析网页真实视频标题命名音频文件；
- **合集与播放列表防呆控制**：默认勾选“仅提取单视频”，避免意外下载多集。

### 4. 📦 全内置绿色开箱即用架构
- 程序核心目录预置 `tools/` 独立工具箱（`tools/ffmpeg.exe` 与 `tools/yt-dlp.exe`）；
- 小白用户无需配置任何环境变量或第三方依赖，解压即用！

---

## 🛠️ 项目目录结构

```
E:\AiToy\video/
├── CMakeLists.txt              # CMake 跨平台构建文件 (Qt5 / Qt6)
├── StarryAudioExtractor.pro    # QMake 项目工程文件 (支持 Qt Creator 双击)
├── src/
│   ├── main.cpp                # 应用程序入口与高 DPI 适配
│   ├── mainwindow.h            # 主界面类定义
│   ├── mainwindow.cpp          # 双模式流转、后缀诊断、大小预估与业务逻辑
│   ├── mainwindow.ui           # Qt Designer 可视化设计器文件 (三页栈结构)
│   ├── starrybackground.h      # 动态星空粒子与流星特效引擎
│   ├── starrybackground.cpp    # 粒子绘制与星云渲染实现
│   ├── ffmpeghelper.h          # 本地 FFmpeg 调度、格式诊断与大小估算
│   ├── ffmpeghelper.cpp        # 异步进程解析与时长探测
│   ├── urlextractorhelper.h    # 网络流媒体 yt-dlp 调度引擎
│   ├── urlextractorhelper.cpp  # 网络下载速度与转码进度解析
│   └── style.h                 # 统一深空霓虹科技感 QSS 样式表
├── tools/                      # 内置独立引擎存放目录
├── Release_Package/            # 独立免安装运行发布包目录
└── StarryAudioExtractor_v1.0.0_Release_x64.zip # 独立分发压缩包 (约 26.9 MB)
```

---

## 🚀 编译与调试指南

1. 启动 **Qt Creator**；
2. 打开 `CMakeLists.txt` 或 `StarryAudioExtractor.pro`；
3. 选择 Kits 套件（如 `MinGW 64-bit` 或 `MSVC 64-bit`）；
4. 在左侧 `Forms` 下可双击 `mainwindow.ui` 进行可视化拖拽编辑；
5. 按快捷键 **Ctrl + R** 即可一键编译启动！
