# NOI Linux 2.0 装机及使用指南

## 目录

- [1. 系统介绍](#1-系统介绍)
- [2. 安装前准备](#2-安装前准备)
- [3. VMware安装步骤](#3-vmware安装步骤)
- [4. 编程环境介绍](#4-编程环境介绍)
- [5. 常见问题解决](#5-常见问题解决)
- [6. 使用技巧和建议](#6-使用技巧和建议)
- [7. 参考资料](#7-参考资料)

---

## 1. 系统介绍

### 1.1 NOI Linux 2.0 基本信息

NOI Linux 2.0 是NOI竞赛委员会基于 **Ubuntu 20.04.1** 定制开发的专用操作系统，专门为信息学奥林匹克竞赛选手设计。

**关键信息：**
- **发布时间**：2021年7月16日
- **正式启用**：2021年9月1日起作为NOI系列比赛和CSP-J/S等活动的标准环境
- **基础系统**：Ubuntu 20.04.1 LTS
- **编译器版本**：G++ 9.3.0
- **默认C++标准**：C++14

### 1.2 系统特性和预装软件

**预装的编程工具：**
- **Code::Blocks** - 功能完整的C++ IDE
- **Sublime Text** - 轻量级代码编辑器（推荐使用）
- **VS Code** - 微软代码编辑器（功能受限）
- **Vim** - 命令行文本编辑器
- **Emacs** - 高级文本编辑器
- **Nano** - 简单文本编辑器
- **Gedit** - 图形界面文本编辑器

**其他工具：**
- **Arbiter** - 评测系统
- **Python 3** - 支持Python编程
- **GCC/G++** - C/C++编译器套件
- **终端** - 命令行界面

### 1.3 适用场景

- NOI系列比赛官方环境
- CSP-J/S认证考试环境
- 信息学竞赛训练和模拟
- 算法编程练习

---

## 2. 安装前准备

### 2.1 硬件要求

**最低配置：**
- **处理器**：64位处理器，支持虚拟化（Intel VT-x 或 AMD-V）
- **内存**：4GB RAM（推荐8GB以上）
- **硬盘空间**：30GB可用空间
  - 虚拟硬盘：20GB
  - 虚拟机信息：5GB
  - ISO文件：5GB
- **操作系统**：Windows 7及以上版本

**重要提示：**
⚠️ 安装前请确认BIOS设置中Intel VT-x或AMD-V处于开启状态，否则可能导致虚拟机安装失败！

### 2.2 软件要求

**必需软件：**
- VMware Workstation Player（免费版）或 VMware Workstation Pro
- NOI Linux 2.0 ISO镜像文件

### 2.3 下载链接

**NOI Linux 2.0 ISO文件：**
```
官方下载地址：https://noiresources.ccf.org.cn/ubuntu-noi-v2.0.iso
文件大小：约5GB
```

**VMware Workstation Player：**
```
官方网站：https://www.vmware.com/products/workstation-player.html
版本：免费版即可满足需求
```

**下载注意事项：**
- 确保下载文件夹所在硬盘有足够空间
- 下载过程可能需要10-30分钟，取决于网络速度
- 建议使用稳定的网络环境下载

---

## 3. VMware安装步骤

### 3.1 VMware Workstation Player 安装

1. **下载VMware Workstation Player**
   - 访问VMware官网
   - 选择"DOWNLOAD FOR FREE"
   - 下载适用于Windows的版本

2. **安装VMware**
   - 运行下载的安装包
   - 按照安装向导提示操作
   - 选择"免费将VMware Workstation Player用于非商业用途"

### 3.2 创建虚拟机

1. **启动VMware并创建新虚拟机**
   ```
   打开VMware Workstation Player → 点击"创建新虚拟机"
   ```

2. **选择安装方式**
   ```
   选择"稍后安装操作系统"（推荐）
   ❌ 不要选择"安装程序光盘映像文件"，避免简易安装的问题
   ```

3. **配置操作系统**
   ```
   操作系统：Linux
   版本：Ubuntu 64位
   ```

4. **设置虚拟机信息**
   ```
   虚拟机名称：NOI Linux 2.0（可自定义）
   位置：选择存储路径（确保路径不包含中文字符）
   ```

5. **配置磁盘**
   ```
   磁盘大小：20-30GB（推荐25GB）
   选择：将虚拟磁盘拆分为多个文件
   ```

6. **自定义硬件设置**
   ```
   点击"自定义硬件" → 选择"CD/DVD"
   选择"使用ISO映像文件" → 浏览选择下载的ubuntu-noi-v2.0.iso
   ```

### 3.3 安装NOI Linux 2.0系统

**重要提醒：安装系统前请断开网络连接，否则安装会很慢！**

1. **启动虚拟机**
   ```
   点击"播放虚拟机"启动安装
   ```

2. **跳过磁盘检查**
   ```
   出现"Checking disks"时，按Ctrl+C跳过
   注意：在虚拟机中操作前需先点击虚拟机屏幕
   ```

3. **选择语言和安装**
   ```
   在左侧语言栏选择"中文（简体）"
   点击"安装Ubuntu"
   ```

4. **安装配置**
   ```
   键盘布局：保持默认
   安装类型：选择"正常安装"
   其他选项：可选择"安装时下载更新"
   ```

5. **磁盘分区**
   ```
   安装类型：选择"其他选项"
   点击"新建分区表" → 确认继续
   创建分区：
   - 交换空间：4096MB（主分区）
   - 根分区：剩余空间，挂载点"/"
   ```

6. **用户设置**
   ```
   填写用户名和密码
   建议设置简单易记的密码
   ```

7. **等待安装完成**
   ```
   安装过程约需10-20分钟
   完成后点击"现在重启"
   ```

8. **首次启动**
   ```
   提示"Please remove the installation medium"时按Enter
   输入用户名和密码登录系统
   ```

---

## 4. 编程环境介绍

### 4.1 编译器信息

**G++ 编译器版本：9.3.0**

查看编译器默认C++标准：
```bash
g++ -dM -E -x c++ /dev/null | grep -F __cplusplus
```
结果显示默认支持 **C++14** 标准。

**编译命令示例：**
```bash
# 基本编译
g++ program.cpp -o program

# 带优化的编译
g++ program.cpp -o program -O2

# 指定C++标准
g++ program.cpp -o program -std=c++14
```

### 4.2 Code::Blocks 配置和使用

**特点：**
- 功能完整的C++ IDE
- 自动补全和代码提示功能正常
- 不需要网络连接即可正常使用
- 适合初学者和习惯IDE环境的用户

**开启O2优化：**
1. 点击"Settings" → "Compiler..."
2. 勾选"Have g++ follow the C++14 ISO language standard [-std=c++14]"
3. 勾选"[-O2] Optimize even more (for speed)"

**使用建议：**
- 适合需要项目管理功能的开发
- 调试功能较为完善
- 界面友好，学习成本低

### 4.3 Sublime Text 配置和使用（推荐）

**特点：**
- 轻量级，启动速度快
- 自动补全功能在离线环境下正常工作
- 支持单文件编译运行
- 界面美观，编程体验良好

**配置编译系统：**
1. 打开Sublime Text
2. 点击"Tools" → "Build System" → "New Build System"
3. 输入以下配置：

```json
{
    "shell_cmd": "g++ '${file}' -o '${file_path}/${file_base_name}' -Wall -Wextra -std=c++14 && '${file_path}/${file_base_name}'",
    "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
    "working_dir": "${file_path}",
    "selector": "source.c++",
    "variants":
    [
        {
            "name": "Run with Input",
            "shell_cmd": "g++ '${file}' -o '${file_path}/${file_base_name}' -Wall -Wextra -std=c++14 && '${file_path}/${file_base_name}' < '${file_path}/input.txt'"
        }
    ]
}
```

4. 保存为"C++.sublime-build"

**使用方法：**
- 编写代码后按Ctrl+B编译运行
- 按Ctrl+Shift+B选择不同的编译选项

### 4.4 VS Code 的限制说明

**存在的问题：**
- 没有安装中文语言包
- C/C++扩展功能不完整
- 在离线环境下功能受限
- 只能作为基本的文本编辑器使用

**不推荐原因：**
- 考场环境下无法安装扩展
- 代码补全和智能提示功能缺失
- 调试功能不可用

### 4.5 其他编辑器

**Vim：**
- 适合熟悉命令行操作的用户
- 学习曲线较陡峭
- 功能强大但需要配置

**Nano：**
- 简单易用的命令行编辑器
- 适合快速编辑小文件
- 功能相对简单

---

## 5. 常见问题解决

### 5.1 显示分辨率调整

**问题：** 全屏模式下显示区域太小

**解决步骤：**
1. 点击屏幕右上角设置图标
2. 选择"设置" → "显示器"
3. 将分辨率调整为适合的大小（如1360x768）
4. 点击"应用" → "保留更改"

### 5.2 VMware Tools 安装

**安装步骤：**
1. 连接网络
2. 打开终端，运行：
   ```bash
   sudo apt update
   sudo apt install open-vm-tools-desktop
   ```
3. 安装完成后重启虚拟机

**注意事项：**
- 考场环境可能需要手动安装VMware Tools
- 安装后可以实现更好的屏幕适配和文件共享

### 5.3 网络连接问题

**安装时断网：**
- 安装系统时必须断开网络连接
- 安装完成后再连接网络进行更新

**虚拟机网络设置：**
- 默认使用NAT模式即可
- 如需与主机共享文件，可设置共享文件夹

### 5.4 文件路径问题

**重要提醒：**
- 代码文件路径必须是英文路径
- 不要将代码存放在"桌面"或"下载"文件夹
- 建议在用户目录下创建专门的代码文件夹

---

## 6. 使用技巧和建议

### 6.1 考场环境模拟

**模拟真实考场环境：**
1. **断开网络连接**
   - 模拟考场无网络环境
   - 测试离线编程能力

2. **不安装额外软件**
   - 保持系统原始状态
   - 熟悉预装工具的使用

3. **练习文件操作**
   - 熟悉Linux基本命令
   - 练习文件的创建、复制、移动

### 6.2 推荐的编程工具选择

**按使用场景推荐：**

1. **考场推荐顺序：**
   - 首选：**Sublime Text**（轻量、稳定、功能完整）
   - 次选：**Code::Blocks**（IDE环境、调试方便）
   - 备选：**Vim**（高效但需要熟练掌握）

2. **不推荐：**
   - VS Code（功能受限）
   - Gedit（功能过于简单）

### 6.3 编程最佳实践

**代码组织：**
```
建议的文件夹结构：
~/code/
├── practice/          # 练习代码
├── contest/           # 比赛代码
└── templates/         # 代码模板
```

**编译脚本：**
创建快速编译脚本 `compile.sh`：
```bash
#!/bin/bash
g++ $1.cpp -o $1 -std=c++14 -O2 -Wall
```

**调试技巧：**
- 使用`gdb`进行调试
- 添加调试输出语句
- 使用断言检查程序状态

### 6.4 性能优化建议

**虚拟机性能优化：**
1. 分配足够的内存（建议4GB以上）
2. 启用虚拟化加速
3. 关闭不必要的视觉效果
4. 定期清理临时文件

**编程效率提升：**
1. 熟悉快捷键操作
2. 准备常用代码模板
3. 练习盲打和快速编码
4. 熟悉Linux命令行操作

---

## 7. 参考资料

### 7.1 官方资源

- **NOI Linux 2.0发布公告**  
  [https://www.noi.cn/gynoi/jsgz/2021-07-16/732450.shtml](https://www.noi.cn/gynoi/jsgz/2021-07-16/732450.shtml)

- **NOI Linux 2.0 ISO下载**  
  [https://noiresources.ccf.org.cn/ubuntu-noi-v2.0.iso](https://noiresources.ccf.org.cn/ubuntu-noi-v2.0.iso)

### 7.2 安装教程

- **洛谷安装指南**  
  [https://www.luogu.com/article/cc9w7tnb](https://www.luogu.com/article/cc9w7tnb)

- **详细安装教程（洛谷专栏）**  
  [https://www.luogu.com.cn/article/jyulnf3b](https://www.luogu.com.cn/article/jyulnf3b)

### 7.3 使用体验

- **NOI Linux 2.0 上手体验**  
  [https://blog.baoshuo.ren/post/noi-linux-2/](https://blog.baoshuo.ren/post/noi-linux-2/)

### 7.4 技术支持

**问题反馈：**
- NOI竞赛办公室邮箱：noi@ccf.org.cn
- 洛谷社区讨论区
- 各大OI论坛和QQ群

### 7.5 相关工具

- **VMware Workstation Player**  
  [https://www.vmware.com/products/workstation-player.html](https://www.vmware.com/products/workstation-player.html)

- **Sublime Text官网**  
  [https://www.sublimetext.com/](https://www.sublimetext.com/)

- **Code::Blocks官网**  
  [https://www.codeblocks.org/](https://www.codeblocks.org/)

---

## 结语

NOI Linux 2.0 相比之前的版本有了显著改进，提供了更好的编程环境和用户体验。通过本指南的详细说明，相信您能够顺利安装和使用NOI Linux 2.0，为信息学竞赛做好充分准备。

**重要提醒：**
- 建议在正式比赛前充分熟悉系统环境
- 多进行模拟练习，适应Linux操作方式
- 掌握至少一种编程工具的熟练使用
- 保持良好的代码编写习惯

祝您在信息学竞赛中取得优异成绩！

---

*最后更新时间：2025年*  
*适用版本：NOI Linux 2.0*
