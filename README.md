# GM-NTG-GUI: 多工厂博弈求解器图形界面

> GM-NTG-Core 约束生成求解器的 Qt 6 图形前端
>
> 功能: 求解控制 | 结果分析 | 实例生成

---

## 目录

### 第一部分: 功能概述

- 1 [项目简介](#1-项目简介)
- 2 [功能模块](#2-功能模块)

### 第二部分: 界面说明

- 3 [求解 Tab](#3-求解-tab)
- 4 [分析 Tab](#4-分析-tab)
- 5 [生成 Tab](#5-生成-tab)

### 第三部分: 代码实现

- 6 [程序架构](#6-程序架构)
- 7 [核心组件](#7-核心组件)
- 8 [构建与运行](#8-构建与运行)

---

# 第一部分: 功能概述

## 1. 项目简介

### 1.1 功能定位

GM-NTG-GUI 是 GM-NTG-Core 求解器的图形用户界面，提供:

- 可视化的参数配置和求解控制
- 实时的 MP-SP 迭代进度监控
- 求解结果的多维度可视化分析
- 测试实例的自动生成

### 1.2 核心求解器

调用 GM-NTG-Core.exe 进行实际求解:

- 算法框架: MP-SP 约束生成
- 主问题 (MP): 最小核分配 LP
- 分离子问题 (SP): 联盟成本计算

### 1.3 关联项目

| 项目 | 说明 |
|:-----|:-----|
| [GM-NTG-Core](https://github.com/94yumingzhao/GM-NTG-Core) | 核心求解器 |
| [GM-NTG-Data](https://github.com/94yumingzhao/GM-NTG-Data) | 测试数据 |

---

## 2. 功能模块

### 2.1 三大功能 Tab

| Tab | 功能 | 说明 |
|:---:|:-----|:-----|
| 求解 | 运行求解器 | 加载数据、配置参数、启动求解、监控进度 |
| 分析 | 结果可视化 | 加载结果、多面板分析、成本分配可视化 |
| 生成 | 创建测试实例 | 配置问题规模、生成随机实例 |

### 2.2 技术栈

| 项目 | 说明 |
|:----:|:-----|
| 编程语言 | C++17 |
| GUI 框架 | Qt 6.10 |
| 编译器 | MSVC 2022 |
| 构建系统 | CMake 3.24+ |

---

# 第二部分: 界面说明

## 3. 求解 Tab

### 3.1 左侧控制面板

**文件选择区**:
- 浏览按钮: 选择 CSV 数据文件
- 文件信息: 显示问题规模 (工厂数/产品数/家族数/周期数)

**参数配置**:
- 求解时限 (秒)
- CPLEX 线程数
- CPLEX 内存限制

**运行控制**:
- 开始求解
- 取消求解
- 导出日志

**结果摘要**:
- 大联盟成本 c(P)
- 最小核值 v*
- 核状态 (非空/空)
- 迭代次数
- 运行时间

### 3.2 右侧日志面板

**实时日志**:
- 求解器输出
- 迭代进度
- MP/SP 求解信息
- 约束添加记录

---

## 4. 分析 Tab

### 4.1 结果加载

- 加载 JSON: 选择 GM-NTG-Core 输出的结果文件
- 格式: `results/result_<timestamp>.json`

### 4.2 分析面板

**概览面板 (Overview)**:
- 关键指标卡片 (大联盟成本、最小核值、迭代次数、时间)
- 成本分配条形图
- 稳定性解释

**工厂面板 (Plant)**:
- 各工厂分配成本
- 工厂参与的联盟
- 边际贡献分析

**联盟面板 (Coalition)**:
- 已添加的联盟约束列表
- 联盟成本 c(S)
- 约束紧度

**迭代面板 (Iteration)**:
- 迭代历史
- 松弛量 v 收敛曲线
- 每轮添加的约束

---

## 5. 生成 Tab

### 5.1 规模配置

| 参数 | 范围 | 说明 |
|:----:|:----:|:-----|
| 工厂数 | 2-20 | 博弈参与者数量 |
| 产品数 | 2-50 | 产品种类数 |
| 家族数 | 1-10 | 产品家族数 |
| 周期数 | 3-52 | 计划周期数 |

### 5.2 成本参数

| 参数 | 说明 |
|:-----|:-----|
| 需求均值/标准差 | 需求随机生成参数 |
| 产能因子 | 产能相对需求倍数 |
| 换产成本 | 家族换产成本基准 |
| 库存成本 | 单位库存持有成本 |
| 转运成本 | 工厂间单位转运成本 |

### 5.3 生成模式

| 模式 | 特点 |
|:----:|:-----|
| Uniform | 均匀分布 |
| Demand-Heavy | 需求集中 |
| Capacity-Imbalanced | 产能不均 |
| Transfer-Sparse | 稀疏转运 |

---

# 第三部分: 代码实现

## 6. 程序架构

### 6.1 目录结构

```
GM-NTG-GUI/
+-- CMakeLists.txt
+-- CMakePresets.json
+-- README.md
+-- src/
    +-- main.cpp                # 程序入口
    +-- main_window.h/cpp       # 主窗口
    +-- parameter_widget.h/cpp  # 参数配置
    +-- log_widget.h/cpp        # 日志输出
    +-- results_widget.h/cpp    # 结果摘要
    +-- solver_worker.h/cpp     # 求解器后台线程
    +-- analysis_widget.h/cpp   # 分析 Tab 主控件
    +-- generator_widget.h/cpp  # 实例生成控件
    +-- panels/
        +-- overview_panel.h/cpp   # 概览面板
        +-- plant_panel.h/cpp      # 工厂面板
        +-- coalition_panel.h/cpp  # 联盟面板
        +-- iteration_panel.h/cpp  # 迭代面板
```

---

## 7. 核心组件

### 7.1 组件职责

| 组件 | 文件 | 职责 |
|:-----|:-----|:-----|
| MainWindow | main_window.cpp | 主窗口，管理 Tab 切换 |
| ParameterWidget | parameter_widget.cpp | 求解参数配置 |
| SolverWorker | solver_worker.cpp | 后台调用求解器 |
| AnalysisWidget | analysis_widget.cpp | 分析 Tab 容器 |
| GeneratorWidget | generator_widget.cpp | 实例生成 |
| OverviewPanel | overview_panel.cpp | 结果概览 |
| PlantPanel | plant_panel.cpp | 工厂分析 |
| CoalitionPanel | coalition_panel.cpp | 联盟分析 |
| IterationPanel | iteration_panel.cpp | 迭代分析 |

### 7.2 线程模型

```
+----------------+     信号/槽     +----------------+
|   主线程 (UI)   | <------------> | 求解器线程     |
+----------------+                +----------------+
       |                                  |
       v                                  v
  界面更新                           QProcess 调用
  日志显示                          GM-NTG-Core.exe
```

### 7.3 求解器信号

| 信号 | 参数 | 时机 |
|:-----|:-----|:-----|
| DataLoaded | P, N, G, T | 数据加载完成 |
| GrandCoalitionComputed | 成本, 时间 | 大联盟成本计算完成 |
| IterationStarted | 迭代号 | 新迭代开始 |
| MPSolved | 分配向量, v, 时间 | 主问题求解完成 |
| SPResult | 联盟, 成本, 违反量 | 分离子问题完成 |
| CutAdded | 联盟, 成本 | 添加新约束 |
| Converged | 精度 | 算法收敛 |
| Finished | 成功, 消息, 结果 | 求解结束 |

---

## 8. 构建与运行

### 8.1 环境要求

| 软件 | 版本 |
|:-----|:-----|
| Windows | 10/11 x64 |
| Visual Studio | 2022 |
| Qt | 6.10+ |
| CMake | 3.24+ |

### 8.2 Qt 配置

```cmake
set(CMAKE_PREFIX_PATH "D:/Tools-DV/Qt/6.10.1/msvc2022_64")
```

### 8.3 构建命令

```bash
cmake --preset release
cmake --build build/release --config Release
```

### 8.4 部署

```bash
D:\Tools-DV\Qt\6.10.1\msvc2022_64\bin\windeployqt.exe build/release/bin/Release/GM-NTG-GUI.exe
```

### 8.5 运行要求

- GM-NTG-Core.exe 在同目录或 PATH 中
- Qt 运行时库已部署

---

**文档版本**: 1.0
**更新日期**: 2026-01-11
