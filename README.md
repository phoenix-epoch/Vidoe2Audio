# 🌌 星空音视频分离下载系统 (Starry Audio & Video Extractor)

基于 **Qt (C++17)**、**FFmpeg** 与 **yt-dlp** 打造的沉浸式星空科技感桌面应用。支持本地多格式音视频极速分离，以及一键输入网页链接下载完整高清视频（MP4）或分离纯音频。

---

> ### ⚡ 快速使用指南（普通用户开箱即用）
> **无需安装 Qt！无需配置任何 C++ 编译环境！无需单独下载配置 FFmpeg！**
> 
> 1. 直接下载本仓库根目录下的绿色便携压缩包：  
>    👉 **[`StarryAudioExtractor_v1.0.0_Release_x64.zip`](StarryAudioExtractor_v1.0.0_Release_x64.zip)**（或前往 [Releases 发布页](../../releases) 下载最新发行版）；
> 2. 将压缩包解压到任意文件夹（例如解压到桌面或 D 盘）；
> 3. 双击解压出来的 **`StarryAudioExtractor.exe`**，即可直接启动并使用！
> 
> *所有核心组件（包括 FFmpeg 核心引擎、yt-dlp 网页提取器、Qt 运行依赖）均已全量内置，解压即用，提取产物默认直接保存至电脑桌面。*

---

## ✨ 核心亮点与功能特性

### 1. 🌠 首页双模式星空选择大厅
- 采用 **QStackedWidget** 架构，启动即进入全屏流星粒子深空导航大厅；
- 提供两枚赛博发光大卡片供用户自由切换工作台：
  - `📁 本地音视频提取模式`
  - `🌐 网络视频/音频提取模式`
- 各工作台顶部均支持 `[ ⬅️ 返回模式选择 ]` 随心流转。

### 2. 📁 本地音视频提取工作台
- **全格式视频兼容**：支持 MP4, MKV, AVI, MOV, FLV, WMV, WEBM, TS, M4V, 3GP, RMVB, VOB, MPG 等 20+ 种格式；
- **智能格式后缀诊断**：直接在输入框输入扩展名（如 `mkv`、`rmvb`、`.ts`），系统瞬间反馈支持状态、容器类型与推荐提取模式；
- **动态音频大小预估**：载入视频后，根据视频时长与用户选择的目标格式/码率（320k/256k/192k/WAV/FLAC 等）**实时精准推算输出文件体积**；
- **引擎生命周期静默管控**：点击提取时后台自动唤醒内置 FFmpeg 引擎，提取完成后自动退出释放，绝不驻留后台；
- **输出默认保存至桌面**：提取完成自动出现在用户电脑桌面上，并可一键点击播放或打开所在目录。

### 3. 🌐 网络视频/音频提取工作台
- **🎬 高清完整视频下载（MP4）**：
  - 支持 **`🌟 最佳清晰度 (原画 4K/2K/1080P)`**、**`1080P 全高清`**、**`720P 高清`** 与 **`480P 标清`**；
  - 自动调度内置 FFmpeg 执行音视频最高轨下载与高精合流，并**强制容器重封装为标准的 MP4 格式**，主流播放器秒开秒播。
- **🎵 一键提取分离纯音频**：
  - 支持将网络视频直接抽取并转码为 MP3、M4A、WAV、FLAC 等纯音频文件，支持 320k 超清音质。
- **主流全网平台兼容**：
  - 支持 B站、抖音、YouTube、小红书、微博、Twitter 以及任意 m3u8 / mp4 直链；
  - 自动识别剪贴板网址并支持一键粘贴，支持单视频与合集分P智能控制。

### 4. 🎨 沉浸式星空科技感无边框设计
- **真·柔和圆角与双层极光微晕**：
  - 窗口外边缘采用 14px 抗锯齿平滑圆角，配合 **2.6px 外层极光微晕 + 1.0px 青蓝芯线**，呈现全息悬浮视觉；
  - 窗口最大化时自适应切换为 0px 直角紧密贴屏。
- **无边框定制标题栏 (StarryTitleBar)**：
  - 告别 Windows 原生生硬白框，融入渐隐式科技流光横线与霓虹按键（最小化、自适应最大化/还原、猩红警告关闭）；
  - 原生级硬件拖拽（支持 Win10/Win11 Aero Snap 吸附与分屏），支持四周 8 向全边缘拉伸缩放，完美避让系统任务栏。

---

## 🛠️ 项目目录结构

```
StarryAudioExtractor/
├── CMakeLists.txt              # CMake 跨平台构建文件 (推荐 CMake 3.16+)
├── StarryAudioExtractor.pro    # QMake 项目工程文件 (支持 Qt Creator 直接打开)
├── .gitattributes              # Git LFS 大文件存储追踪规则
├── .gitignore                  # 构建目录与临时中间件忽略规则
├── README.md                   # 项目使用与开发说明文档
├── StarryAudioExtractor_v1.0.0_Release_x64.zip # 🌟 官方正式开箱即用绿色免安装包
├── src/
│   ├── main.cpp                # 应用程序入口与高 DPI 适配
│   ├── mainwindow.h            # 主界面类定义与原生事件拦截
│   ├── mainwindow.cpp          # 工作台流转、格式诊断、大小预估与音视频业务逻辑
│   ├── mainwindow.ui           # Qt Designer 可视化设计器文件 (多页面栈结构)
│   ├── starrytitlebar.h        # 星空科技感自定义标题栏头文件
│   ├── starrytitlebar.cpp      # 标题栏绘制、流光割线与系统级拖拽交互
│   ├── starrybackground.h      # 动态深空粒子与流星特效引擎头文件
│   ├── starrybackground.cpp    # 柔和双层极光边框、星云与流星粒子渲染实现
│   ├── ffmpeghelper.h          # 本地 FFmpeg 调度、格式诊断与大小估算
│   ├── ffmpeghelper.cpp        # 异步进程解析与时长探测
│   ├── urlextractorhelper.h    # 网络音视频 yt-dlp 合流重封装引擎
│   ├── urlextractorhelper.cpp  # 多清晰度规则调度、进度与速度捕获
│   └── style.h                 # 统一深空极光霓虹科技感 QSS 样式体系
└── tools/                      # 内置独立免配置核心工具箱 (ffmpeg.exe, yt-dlp.exe)
```

---

## 💻 开发者编译与调试指南

如果您是开发者并希望从源码二次开发或编译：

### 依赖环境
- **C++17** 兼容编译器（MinGW 11.2+ 或 MSVC 2019/2022 64-bit）；
- **Qt 6.x**（推荐 Qt 6.5+ / Qt 6.11，包含 `Widgets`、`Core`、`Gui` 组件）；
- **CMake 3.16+** 或 **QMake**。

### 方式一：使用 Qt Creator 打开构建
1. 启动 **Qt Creator**；
2. 打开项目根目录下的 `CMakeLists.txt` 或 `StarryAudioExtractor.pro`；
3. 选择 Kits 套件（例如 `Desktop Qt 6.11.1 MinGW 64-bit` 或 `MSVC 2022 64-bit`）；
4. 直接按快捷键 **Ctrl + R** 即可一键编译并启动调试。

### 方式二：命令行 CMake 快速构建
```bash
# 1. 创建并进入构建目录
mkdir build && cd build

# 2. 配置 CMake (指定 Qt 路径)
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="E:/Qt/6.11.1/mingw_64"

# 3. 编译
cmake --build . --config Release
```

---

## 📜 开源协议与声明

- 本项目遵循开源规范；
- 软件内集成的 FFmpeg 与 yt-dlp 均遵循各自的 LGPL / GPL / Unlicense 开源协议；
- 仅供个人音视频学习、素材剪辑与研究使用，请勿用于任何侵犯版权的商业用途。
