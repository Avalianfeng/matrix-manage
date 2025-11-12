# Matrix-Manage

一个用于线性代数计算的 C++ 矩阵管理工具 / A C++ matrix management tool for linear algebra calculations

## 📋 项目简介 / Overview

Matrix-Manage 是一个功能完善的矩阵运算工具，支持多矩阵管理、基础矩阵运算、行列变换、行列式计算等功能。程序提供中英文双语界面，支持小数和分数两种输出格式。

Matrix-Manage is a comprehensive matrix operation tool that supports multi-matrix management, basic matrix operations, row/column transformations, determinant calculations, and more. The program provides bilingual interfaces (Chinese/English) and supports both decimal and fraction output formats.

## ✨ 主要功能 / Key Features

- **矩阵管理 / Matrix Management**
  - 创建和管理多个矩阵
  - 矩阵的输入、输出和存储
  - 支持矩阵复制和删除

- **基础运算 / Basic Operations**
  - 矩阵加法、减法、乘法
  - 矩阵转置
  - 矩阵行列式计算

- **行列变换 / Row/Column Operations**
  - 交换行/列
  - 行/列数乘
  - 行/列倍加

- **显示格式 / Display Formats**
  - 小数格式输出
  - 分数格式输出（自动转换）
  - 整数优化显示

- **语言支持 / Language Support**
  - 中文界面
  - English interface

## 🗂️ 项目结构 / Project Structure

```
matrix-manage/
├── include/              # 头文件 / Header files
│   ├── Matrix.hpp        # 矩阵类定义
│   ├── Determinant.hpp   # 行列式类定义
│   ├── decimalToFraction.hpp  # 小数转分数工具
│   ├── safe_input.hpp    # 安全输入工具
│   └── shape.hpp         # 矩阵形状定义
├── src/                  # 源文件 / Source files
│   ├── main.cpp          # 主程序入口
│   ├── Matrix.cpp        # 矩阵类实现
│   ├── Determinant.cpp   # 行列式实现
│   ├── decimalToFraction.cpp  # 小数转分数实现
│   ├── safe_input.cpp    # 安全输入实现
│   └── shape.cpp         # 形状相关实现
├── CMakeLists.txt        # CMake 配置文件
└── README.md             # 项目说明文档
```

## 🛠️ 构建说明 / Build Instructions

### 使用 CMake（推荐）/ Using CMake (Recommended)

```bash
# 创建构建目录 / Create build directory
mkdir build
cd build

# 配置项目 / Configure project
cmake ..

# 编译 / Build
cmake --build .

# 运行程序 / Run the program
./bin/matrix-manage    # Linux/macOS
.\bin\matrix-manage.exe  # Windows
```

### 使用 VS Code

1. 打开项目文件夹 / Open project folder
2. 运行 "g++ build all files" 任务 / Run the "g++ build all files" task
3. 或使用 CMake Tools 扩展进行构建 / Or use CMake Tools extension to build

## 🚀 使用说明 / Usage

1. **启动程序** / Start the program
   - 选择语言（中文/英文）
   - 设置矩阵数量
   - 选择是否创建第一个矩阵

2. **矩阵操作** / Matrix Operations
   - 创建矩阵：指定行数和列数，输入矩阵元素
   - 矩阵运算：选择两个矩阵进行加、减、乘运算
   - 行列变换：对单个矩阵进行行列交换、数乘、倍加等操作
   - 计算行列式：对方阵计算行列式的值

3. **显示选项** / Display Options
   - 切换分数/小数显示模式
   - 查看当前所有矩阵
   - 输出指定矩阵

## 📦 依赖要求 / Requirements

- C++17 或更高版本 / C++17 or higher
- CMake 3.15 或更高版本 / CMake 3.15 or higher
- 支持的编译器 / Supported compilers:
  - GCC 7+
  - Clang 5+
  - MSVC 2017+

## 📝 示例 / Example

```
输入矩阵维度:
行数: 2
列数: 2

请输入矩阵元素:
1 2
3 4

矩阵输出:
1.00  2.00
3.00  4.00

行列式值: -2.00
```

## 🤝 贡献 / Contributing

欢迎提交问题和拉取请求！/ Issues and pull requests are welcome!

## 📄 许可证 / License

本项目采用 MIT 许可证 / This project is licensed under the MIT License.

## 👨‍💻 作者 / Author
邮箱/email：m19956272658@163.com
开发中... / In development...

---

**注意 / Note**: 本项目持续开发中，功能可能会有所更新。/ This project is under continuous development, features may be updated.
