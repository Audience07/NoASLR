# 🚀 NoASLR

一个用于修改 Windows PE 可执行文件 ASLR 标志的简单 C++ 工具。

## 🔧 说明

本项目通过加载指定的 PE 文件到内存，清除 `IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE` 标志，从而禁用该文件的 ASLR (Address Space Layout Randomization)。修改后的文件会保存为 `New.exe`。

## ✨ 功能

- 📂 加载目标 PE 文件
- 🛠️ 修改 PE 可执行文件头中的 ASLR 标志
- ℹ️ 输出当前 ASLR 状态
- 💾 将修改后的文件写入 `New.exe`

## ▶️ 使用方法

1. 使用 Visual Studio 打开 `PE ASLR.vcxproj` 并编译项目。
2. 或者在命令行下使用支持 MSVC 的环境编译 `main.cpp`。
3. 运行程序并传入要修改的 PE 文件路径：

```bash
PE_ASLR.exe target.exe
```

程序执行后，会输出 ASLR 当前状态，并在同目录生成 `New.exe`。

## ⚠️ 注意

- 该工具仅适用于 Windows PE 可执行文件。
- 输入文件应为有效的 PE 文件。
- 程序不会自动备份原始文件，请自行保存备份。

## 📁 文件

- `main.cpp`：程序主逻辑
- `PE ASLR.vcxproj`：Visual Studio 工程文件
- `README.md`：项目说明文档
