# CSP复赛必备：STL容器的使用和选择

## 📋 目录

### 🔸 序列式容器

- [1. 序列式容器概述](#1-序列式容器概述)
- [2. 三大核心容器详解](#2-三大核心容器详解)
  - [2.1 vector`<T>` - 动态数组](#21-vectort---动态数组)
  - [2.2 list`<T>` - 双向链表](#22-listt---双向链表)
  - [2.3 deque`<T>` - 双端队列](#23-dequet---双端队列)
- [3. 性能对比与时间复杂度](#3-性能对比与时间复杂度)
  - [3.1 时间复杂度对比表](#31-时间复杂度对比表)
  - [3.2 内存使用特点](#32-内存使用特点)
- [4. 容器选择决策指南](#4-容器选择决策指南)
  - [4.1 快速选择流程图](#41-快速选择流程图)
  - [4.2 CSP竞赛场景分析](#42-csp竞赛场景分析)
- [5. 实战代码示例](#5-实战代码示例)
  - [5.1 vector实用技巧](#51-vector实用技巧)
  - [5.2 list高效操作](#52-list高效操作)
  - [5.3 deque双端应用](#53-deque双端应用)
- [6. CSP竞赛最佳实践](#6-csp竞赛最佳实践)
  - [6.1 常见使用场景](#61-常见使用场景)
  - [6.2 性能优化技巧](#62-性能优化技巧)
  - [6.3 避免常见陷阱](#63-避免常见陷阱)

### 🔸 关联容器

- [7. 关联容器简介](#7-关联容器简介)
- [8. Set容器详解](#8-set容器详解)
  - [8.1 set`<T>` - 有序集合](#81-sett---有序集合)
  - [8.2 multiset`<T>` - 多重集合](#82-multisett---多重集合)
- [9. Map容器详解](#9-map容器详解)
  - [9.1 mapk,v - 有序映射](#91-mapkv---有序映射)
  - [9.2 multimapk,v - 多重映射](#92-multimapkv---多重映射)
- [10. 关联容器在CSP中的应用](#10-关联容器在csp中的应用)
  - [10.1 去重与查找问题](#101-去重与查找问题)
  - [10.2 计数与统计问题](#102-计数与统计问题)
  - [10.3 排序与范围查询](#103-排序与范围查询)

---

## 1. 序列式容器概述

**序列式容器**是指以线性排列方式存储元素的容器，类似于普通数组的存储方式。这类容器不会自动对存储的元素进行排序，保持元素的插入顺序。

### 🎯 核心特点

- **线性存储**：元素按照插入顺序排列
- **类型安全**：支持任意类型T（int、double、自定义类等）
- **动态大小**：容器大小可以在运行时改变
- **迭代器支持**：提供统一的遍历接口

### 📦 主要容器类型

C++ STL中常用的序列式容器包括：

| 容器类型      | 中文名称 | 底层实现 | 主要特点                   |
| ------------- | -------- | -------- | -------------------------- |
| `vector<T>` | 动态数组 | 连续内存 | 随机访问，尾部高效         |
| `list<T>`   | 双向链表 | 链式结构 | 任意位置插入删除高效       |
| `deque<T>`  | 双端队列 | 分段数组 | 两端操作高效，支持随机访问 |

> **注意**：`stack<T>` 和 `queue<T>` 本质上是容器适配器，通常基于 `deque` 实现。

---

## 2. 三大核心容器详解

### 2.1 vector`<T>` - 动态数组

#### 🔧 核心特性

**vector** 是最常用的序列容器，提供类似数组的功能，但支持动态扩容。

**优势：**

- ✅ **随机访问**：支持 `[]` 操作符，O(1) 时间复杂度
- ✅ **尾部高效**：`push_back()` 和 `pop_back()` 为 O(1)
- ✅ **内存连续**：缓存友好，遍历效率高
- ✅ **兼容性好**：与C风格数组兼容

**劣势：**

- ❌ **中间插入慢**：需要移动元素，O(n) 时间复杂度
- ❌ **扩容开销**：可能触发整体内存重新分配
- ❌ **头部操作慢**：不支持 `push_front()`

#### 💡 适用场景

```cpp
// CSP竞赛中的典型应用
vector<int> scores;           // 存储分数序列
vector<pair<int,int>> points; // 存储坐标点
vector<string> words;         // 存储字符串列表
```

其他参考： https://www.runoob.com/cplusplus/cpp-vector.html

### 2.2 list `<T>` - 双向链表

#### 🔧 核心特性

**list** 基于双向链表实现，擅长频繁的插入和删除操作。

**优势：**

- ✅ **插入删除高效**：任意位置 O(1) 时间复杂度
- ✅ **内存灵活**：按需分配，无需连续内存
- ✅ **迭代器稳定**：插入删除不影响其他迭代器
- ✅ **双向遍历**：支持前向和后向遍历

**劣势：**

- ❌ **无随机访问**：不支持 `[]` 操作符
- ❌ **遍历较慢**：需要沿链表逐个访问
- ❌ **内存开销大**：每个节点需要额外指针空间

#### 💡 适用场景

```cpp
// 需要频繁插入删除的场景
list<int> task_queue;         // 任务队列
list<Player> player_list;     // 玩家列表（频繁加入退出）
```

### 2.3 deque`<T>` - 双端队列

#### 🔧 核心特性

**deque** 结合了 vector 和 list 的优点，支持两端高效操作。

**优势：**

- ✅ **双端高效**：头尾插入删除都是 O(1)
- ✅ **随机访问**：支持 `[]` 操作符
- ✅ **无扩容开销**：分段存储，避免整体重新分配
- ✅ **功能全面**：兼具数组和队列特性

**劣势：**

- ❌ **中间操作慢**：中间插入删除仍为 O(n)
- ❌ **内存不连续**：缓存性能不如 vector
- ❌ **复杂度高**：实现相对复杂

#### 💡 适用场景

```cpp
// 需要双端操作的场景
deque<int> sliding_window;    // 滑动窗口
deque<char> palindrome_check; // 回文检查
```

---

## 3. 性能对比与时间复杂度

### 3.1 时间复杂度对比表

| 操作类型                  | vector     | list       | deque      | 说明                     |
| ------------------------- | ---------- | ---------- | ---------- | ------------------------ |
| **访问操作**        |            |            |            |                          |
| 随机访问 `[]`           | O(1)       | ❌         | O(1)       | list不支持随机访问       |
| 首元素 `front()`        | O(1)       | O(1)       | O(1)       | 所有容器都支持           |
| 尾元素 `back()`         | O(1)       | O(1)       | O(1)       | 所有容器都支持           |
| **插入操作**        |            |            |            |                          |
| 头部插入 `push_front()` | ❌         | O(1)       | O(1)       | vector不支持             |
| 尾部插入 `push_back()`  | O(1)*      | O(1)       | O(1)       | *可能触发扩容            |
| 中间插入 `insert()`     | O(n)       | O(1)       | O(n)       | list在已知位置插入为O(1) |
| **删除操作**        |            |            |            |                          |
| 头部删除 `pop_front()`  | ❌         | O(1)       | O(1)       | vector不支持             |
| 尾部删除 `pop_back()`   | O(1)       | O(1)       | O(1)       | 所有容器都支持           |
| 中间删除 `erase()`      | O(n)       | O(1)       | O(n)       | list在已知位置删除为O(1) |
| **查找操作**        |            |            |            |                          |
| 线性查找 `find()`       | O(n)       | O(n)       | O(n)       | 都需要遍历               |
| **其他操作**        |            |            |            |                          |
| 排序 `sort()`           | O(n log n) | O(n log n) | O(n log n) | list有专门的sort()方法   |
| 大小 `size()`           | O(1)       | O(1)       | O(1)       | 所有容器都是常数时间     |

### 3.2 内存使用特点

#### 📊 内存布局对比

```cpp
// vector: 连续内存布局
[元素1][元素2][元素3][元素4]...
// 优点：缓存友好，遍历快速
// 缺点：扩容时需要整体迁移

// list: 链式内存布局  
[prev|元素1|next] -> [prev|元素2|next] -> [prev|元素3|next]
// 优点：灵活分配，插入删除无需移动
// 缺点：额外指针开销，缓存不友好

// deque: 分段连续布局
[段1: 元素1,元素2] [段2: 元素3,元素4] [段3: 元素5,元素6]
// 优点：避免整体迁移，支持双端操作
// 缺点：实现复杂，内存不完全连续
```

#### 💾 空间复杂度分析

| 容器   | 每元素开销         | 额外开销   | 总空间复杂度  |
| ------ | ------------------ | ---------- | ------------- |
| vector | sizeof(T)          | 预留空间   | O(n) + 预留   |
| list   | sizeof(T) + 16字节 | 头尾指针   | O(n) × 1.5+  |
| deque  | sizeof(T)          | 段管理开销 | O(n) + 段开销 |

---

## 4. 容器选择决策指南

### 4.1 快速选择流程图

```
开始选择容器
       ↓
   需要随机访问？
   ↙        ↘
  是          否
  ↓           ↓
需要频繁双端操作？  需要频繁中间插入删除？
↙        ↘      ↙              ↘
是        否     是              否
↓         ↓      ↓               ↓
deque   vector  list           vector
```

### 4.2 CSP竞赛场景分析

#### 🎯 选择 vector 的场景

```cpp
// 1. 需要随机访问的算法
vector<int> arr(n);
sort(arr.begin(), arr.end());  // 排序算法
int mid = arr[n/2];            // 中位数查找

// 2. 动态规划状态存储
vector<vector<int>> dp(n, vector<int>(m, 0));

// 3. 图的邻接表表示
vector<vector<int>> graph(n);
graph[u].push_back(v);         // 添加边

// 4. 坐标点存储
vector<pair<int,int>> points;
points.push_back({x, y});
```

#### 🎯 选择 list 的场景

```cpp
// 1. 模拟链表操作
list<int> josephus_circle;     // 约瑟夫环问题

// 2. 需要频繁插入删除的模拟
list<Task> task_queue;
auto it = task_queue.begin();
advance(it, pos);
task_queue.insert(it, new_task);  // 中间插入

// 3. 合并有序序列
list<int> list1, list2;
list1.merge(list2);           // 高效合并
```

#### 🎯 选择 deque 的场景

```cpp
// 1. 滑动窗口问题
deque<int> window;
window.push_back(new_element);
window.pop_front();           // 维护窗口

// 2. 双端队列BFS
deque<pair<int,int>> bfs_queue;
bfs_queue.push_back({start_x, start_y});

// 3. 回文串检查
deque<char> palindrome;
while(!palindrome.empty() && 
      palindrome.front() == palindrome.back()) {
    palindrome.pop_front();
    palindrome.pop_back();
}
```

---

## 5. 实战代码示例

### 5.1 vector实用技巧

#### 📝 基础操作优化

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // 1. 预分配空间避免频繁扩容
    vector<int> nums;
    nums.reserve(1000);  // 预分配空间
  
    // 2. 使用emplace_back减少拷贝
    vector<pair<int,int>> points;
    points.emplace_back(1, 2);  // 直接构造，避免临时对象
  
    // 3. 范围初始化
    vector<int> arr(10, 5);     // 10个元素，都初始化为5
    vector<int> copy_arr(arr);  // 拷贝构造
  
    // 4. 高效查找和排序
    sort(arr.begin(), arr.end());
    bool found = binary_search(arr.begin(), arr.end(), 5);
  
    // 5. 范围遍历（C++11）
    for(const auto& num : arr) {
        cout << num << " ";
    }
  
    return 0;
}
```

#### 🚀 CSP竞赛中的vector应用

```cpp
// 示例：动态规划 - 最长递增子序列
#include <vector>
#include <algorithm>
using namespace std;

int lengthOfLIS(vector<int>& nums) {
    vector<int> dp;
    for(int num : nums) {
        auto it = lower_bound(dp.begin(), dp.end(), num);
        if(it == dp.end()) {
            dp.push_back(num);
        } else {
            *it = num;
        }
    }
    return dp.size();
}
```

### 5.2 list高效操作

#### 📝 链表特有操作

```cpp
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

int main() {
    list<int> mylist;
  
    // 1. 双端插入
    mylist.push_back(20);
    mylist.push_back(100);
    mylist.push_front(1);
  
    // 2. 链表专用排序（比通用sort更高效）
    mylist.sort();
  
    // 3. 去重操作（需要先排序）
    mylist.unique();
  
    // 4. 反转链表
    mylist.reverse();
  
    // 5. 条件删除
    mylist.remove_if([](int x) { return x > 50; });
  
    // 6. 合并有序链表
    list<int> other_list = {5, 15, 25};
    other_list.sort();
    mylist.merge(other_list);  // other_list会被清空
  
    // 7. 范围遍历
    for(int val : mylist) {
        cout << val << " ";
    }
  
    return 0;
}
```

#### 🚀 CSP竞赛中的list应用

```cpp
// 示例：约瑟夫环问题
#include <list>
using namespace std;

int josephus(int n, int k) {
    list<int> circle;
    for(int i = 1; i <= n; i++) {
        circle.push_back(i);
    }
  
    auto it = circle.begin();
    while(circle.size() > 1) {
        // 移动k-1步
        for(int i = 0; i < k-1; i++) {
            ++it;
            if(it == circle.end()) it = circle.begin();
        }
      
        // 删除当前元素
        it = circle.erase(it);
        if(it == circle.end()) it = circle.begin();
    }
  
    return circle.front();
}
```

### 5.3 deque双端应用

#### 📝 双端队列操作

```cpp
#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;

int main() {
    deque<int> dq;
  
    // 1. 双端插入
    dq.push_back(20);
    dq.push_back(100);
    dq.push_front(1);
  
    // 2. 随机访问
    cout << "中间元素: " << dq[1] << endl;
  
    // 3. 双端删除
    dq.pop_front();
    dq.pop_back();
  
    // 4. 支持所有vector的算法
    sort(dq.begin(), dq.end());
  
    // 5. 范围遍历
    for(int val : dq) {
        cout << val << " ";
    }
  
    return 0;
}
```

#### 🚀 CSP竞赛中的deque应用

```cpp
// 示例：滑动窗口最大值
#include <deque>
#include <vector>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;  // 存储数组下标
    vector<int> result;
  
    for(int i = 0; i < nums.size(); i++) {
        // 移除超出窗口的元素
        while(!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
      
        // 维护递减队列
        while(!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
      
        dq.push_back(i);
      
        // 窗口形成后记录最大值
        if(i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
  
    return result;
}
```

---

## 6. CSP竞赛最佳实践

### 6.1 常见使用场景

#### 🎯 按问题类型选择容器

| 问题类型           | 推荐容器     | 理由                       |
| ------------------ | ------------ | -------------------------- |
| **排序算法** | vector       | 支持随机访问，sort()效率高 |
| **动态规划** | vector       | 状态数组，需要随机访问     |
| **图算法**   | vector       | 邻接表，需要快速访问邻居   |
| **模拟题**   | deque        | 可能需要双端操作           |
| **链表操作** | list         | 频繁插入删除               |
| **滑动窗口** | deque        | 双端队列特性               |
| **栈/队列**  | vector/deque | 作为底层容器               |

#### 📊 性能关键场景

```cpp
// 1. 大数据量排序 - 使用vector
vector<int> big_data(1000000);
sort(big_data.begin(), big_data.end());  // 最优性能

// 2. 频繁中间插入 - 使用list
list<int> frequent_insert;
auto it = frequent_insert.begin();
advance(it, pos);
frequent_insert.insert(it, value);  // O(1)插入

// 3. 双端操作 - 使用deque
deque<int> double_ended;
double_ended.push_front(1);  // O(1)
double_ended.push_back(2);   // O(1)
```

### 6.2 性能优化技巧

#### ⚡ vector优化技巧

```cpp
// 1. 预分配空间
vector<int> nums;
nums.reserve(expected_size);  // 避免频繁扩容

// 2. 使用emplace系列函数
vector<pair<int,int>> points;
points.emplace_back(x, y);    // 比push_back(make_pair(x,y))更高效

// 3. 避免不必要的拷贝
const vector<int>& getVector();  // 返回引用
for(const auto& item : vec) {}   // 范围遍历使用引用

// 4. 批量操作
vector<int> source, target;
target.insert(target.end(), source.begin(), source.end());  // 批量插入
```

#### ⚡ list优化技巧

```cpp
// 1. 使用splice进行高效移动
list<int> list1, list2;
list1.splice(list1.end(), list2);  // 移动整个list2到list1末尾

// 2. 利用list专用算法
list1.sort();           // 比std::sort更高效
list1.merge(list2);     // 合并有序链表
list1.unique();         // 去除连续重复元素

// 3. 避免随机访问
// 错误：list不支持[]操作
// auto it = mylist.begin();
// advance(it, index);  // 使用advance移动迭代器
```

#### ⚡ deque优化技巧

```cpp
// 1. 充分利用双端特性
deque<int> dq;
dq.push_front(1);  // 比vector的insert(begin(), 1)更高效
dq.push_back(2);

// 2. 作为队列使用
deque<int> queue;
queue.push_back(item);   // 入队
int front = queue.front(); queue.pop_front();  // 出队

// 3. 滑动窗口应用
deque<int> window;
// 维护窗口大小
if(window.size() >= k) window.pop_front();
window.push_back(new_item);
```

### 6.3 避免常见陷阱

#### ⚠️ 容器选择陷阱

```cpp
// 陷阱1：对list使用随机访问
list<int> mylist;
// 错误：mylist[i]  // list不支持[]操作
// 正确：
auto it = mylist.begin();
advance(it, i);
int value = *it;

// 陷阱2：vector频繁头部插入
vector<int> vec;
// 低效：vec.insert(vec.begin(), item);  // O(n)操作
// 建议：使用deque或重新设计算法

// 陷阱3：忽略容器扩容成本
vector<int> vec;
for(int i = 0; i < 1000000; i++) {
    vec.push_back(i);  // 可能触发多次扩容
}
// 优化：vec.reserve(1000000);
```

#### ⚠️ 迭代器失效陷阱

```cpp
// 陷阱4：vector迭代器失效
vector<int> vec = {1, 2, 3, 4, 5};
for(auto it = vec.begin(); it != vec.end(); ++it) {
    if(*it == 3) {
        vec.erase(it);  // 危险：it失效
        // 正确：it = vec.erase(it); --it;
    }
}

// 陷阱5：容器扩容导致迭代器失效
vector<int> vec;
auto it = vec.begin();
vec.push_back(1);  // 可能导致扩容，it失效
// 解决：重新获取迭代器或使用索引
```

#### ⚠️ 性能陷阱

```cpp
// 陷阱6：不必要的拷贝
vector<string> strings;
for(const string& s : input) {
    strings.push_back(s);  // 拷贝
}
// 优化：strings.emplace_back(s); 或使用move

// 陷阱7：错误的容器大小估计
vector<int> vec(1000000, 0);  // 预分配过大
// 建议：根据实际需求合理估计大小
```

---

## 7. 关联容器简介

**关联容器**是指根据键（key）来存储和访问元素的容器，与序列式容器不同，关联容器会自动对元素进行排序，并提供高效的查找、插入和删除操作。

### 🎯 核心特点

- **自动排序**：元素按照键值自动排序（默认升序）
- **高效查找**：基于红黑树实现，查找时间复杂度为 O(log n)
- **唯一性控制**：可以控制键值是否允许重复
- **范围操作**：支持范围查询和迭代

### 📦 主要容器类型

C++ STL中常用的关联容器包括：

| 容器类型          | 中文名称 | 键值特点         | 主要用途         |
| ----------------- | -------- | ---------------- | ---------------- |
| `set<T>`        | 有序集合 | 键即值，不重复   | 去重、查找、排序 |
| `multiset<T>`   | 多重集合 | 键即值，可重复   | 计数、排序       |
| `map<K,V>`      | 有序映射 | 键值对，键不重复 | 字典、索引、计数 |
| `multimap<K,V>` | 多重映射 | 键值对，键可重复 | 一对多映射       |

### 🔍 与序列式容器的区别

| 特性               | 序列式容器     | 关联容器          |
| ------------------ | -------------- | ----------------- |
| **存储方式** | 按插入顺序     | 按键值排序        |
| **访问方式** | 索引/迭代器    | 键值查找          |
| **查找效率** | O(n) 线性查找  | O(log n) 二分查找 |
| **插入位置** | 可指定位置     | 自动确定位置      |
| **主要用途** | 数组、列表操作 | 查找、去重、计数  |

### 💡 CSP竞赛中的应用价值

```cpp
// 序列式容器：适合模拟、动态规划
vector<int> dp(n);           // 状态数组
deque<int> window;           // 滑动窗口

// 关联容器：适合查找、去重、计数
set<int> unique_nums;        // 去重
map<string, int> word_count; // 计数
```

---

## 8. Set容器详解

### 8.1 set`<T>` - 有序集合

#### 🔧 核心特性

**set** 是一个有序的集合容器，存储唯一的元素，并自动按照键值排序。

**优势：**

- ✅ **自动排序**：元素始终保持有序状态
- ✅ **快速查找**：O(log n) 时间复杂度
- ✅ **自动去重**：不允许重复元素
- ✅ **范围操作**：支持 lower_bound、upper_bound

**劣势：**

- ❌ **无随机访问**：不支持 `[]` 操作符
- ❌ **插入位置固定**：无法指定插入位置
- ❌ **内存开销**：红黑树结构有额外开销

#### 📝 基础操作

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int> s;
  
    // 1. 插入元素（自动排序）
    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);  // 重复元素，不会插入
  
    // 2. 查找元素
    if(s.find(20) != s.end()) {
        cout << "找到元素 20" << endl;
    }
  
    // 3. 计数（对于set，结果只能是0或1）
    cout << "元素10的个数: " << s.count(10) << endl;
  
    // 4. 删除元素
    s.erase(10);
  
    // 5. 遍历（自动有序）
    for(int x : s) {
        cout << x << " ";  // 输出：20 30
    }
  
    return 0;
}
```

#### 🚀 CSP竞赛中的set应用

```cpp
// 示例1：数组去重并排序
#include <set>
#include <vector>
using namespace std;

vector<int> removeDuplicates(vector<int>& nums) {
    set<int> unique_set(nums.begin(), nums.end());
    return vector<int>(unique_set.begin(), unique_set.end());
}

// 示例2：判断是否存在重复元素
bool containsDuplicate(vector<int>& nums) {
    set<int> seen;
    for(int num : nums) {
        if(seen.count(num)) return true;
        seen.insert(num);
    }
    return false;
}

// 示例3：范围查询
void rangeQuery(set<int>& s, int left, int right) {
    auto it_left = s.lower_bound(left);   // >= left的第一个元素
    auto it_right = s.upper_bound(right); // > right的第一个元素
  
    for(auto it = it_left; it != it_right; ++it) {
        cout << *it << " ";
    }
}
```

### 8.2 multiset`<T>` - 多重集合

#### 🔧 核心特性

**multiset** 允许存储重复元素的有序集合，常用于需要计数和排序的场景。

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    multiset<int> ms;
  
    // 1. 插入重复元素
    ms.insert(10);
    ms.insert(20);
    ms.insert(10);
    ms.insert(30);
    ms.insert(10);
  
    // 2. 计数
    cout << "元素10的个数: " << ms.count(10) << endl;  // 输出：3
  
    // 3. 删除操作
    ms.erase(ms.find(10));  // 删除一个10
    cout << "删除后10的个数: " << ms.count(10) << endl;  // 输出：2
  
    ms.erase(10);  // 删除所有10
    cout << "全部删除后10的个数: " << ms.count(10) << endl;  // 输出：0
  
    // 4. 遍历（有序且包含重复）
    for(int x : ms) {
        cout << x << " ";  // 输出：20 30
    }
  
    return 0;
}
```

#### 🚀 CSP竞赛中的multiset应用

```cpp
// 示例：维护动态中位数
#include <set>
using namespace std;

class MedianFinder {
private:
    multiset<int> left_half;   // 存储较小的一半（最大堆效果）
    multiset<int> right_half;  // 存储较大的一半（最小堆效果）
  
public:
    void addNum(int num) {
        if(left_half.empty() || num <= *left_half.rbegin()) {
            left_half.insert(num);
        } else {
            right_half.insert(num);
        }
      
        // 平衡两个集合的大小
        if(left_half.size() > right_half.size() + 1) {
            right_half.insert(*left_half.rbegin());
            left_half.erase(left_half.find(*left_half.rbegin()));
        } else if(right_half.size() > left_half.size() + 1) {
            left_half.insert(*right_half.begin());
            right_half.erase(right_half.begin());
        }
    }
  
    double findMedian() {
        if(left_half.size() == right_half.size()) {
            return (*left_half.rbegin() + *right_half.begin()) / 2.0;
        } else if(left_half.size() > right_half.size()) {
            return *left_half.rbegin();
        } else {
            return *right_half.begin();
        }
    }
};
```

---

## 9. Map容器详解

### 9.1 map<K,V> - 有序映射

#### 🔧 核心特性

**map** 存储键值对（key-value pairs），键唯一且自动排序，提供高效的键值查找。

**优势：**

- ✅ **键值映射**：一对一的键值关系
- ✅ **自动排序**：按键值自动排序
- ✅ **快速查找**：O(log n) 时间复杂度
- ✅ **方便访问**：支持 `[]` 操作符

**劣势：**

- ❌ **键不可修改**：插入后键值不能改变
- ❌ **内存开销**：红黑树结构开销
- ❌ **插入位置固定**：按键值自动排序

#### 📝 基础操作

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> word_count;
  
    // 1. 插入键值对
    word_count["apple"] = 5;
    word_count["banana"] = 3;
    word_count.insert({"orange", 7});
    word_count.insert(make_pair("grape", 2));
  
    // 2. 访问和修改
    cout << "apple的数量: " << word_count["apple"] << endl;
    word_count["apple"]++;  // 增加计数
  
    // 3. 查找
    if(word_count.find("banana") != word_count.end()) {
        cout << "找到banana，数量: " << word_count["banana"] << endl;
    }
  
    // 4. 检查键是否存在
    if(word_count.count("watermelon") == 0) {
        cout << "watermelon不存在" << endl;
    }
  
    // 5. 删除
    word_count.erase("grape");
  
    // 6. 遍历（按键值排序）
    for(const auto& pair : word_count) {
        cout << pair.first << ": " << pair.second << endl;
    }
  
    return 0;
}
```

#### 🚀 CSP竞赛中的map应用

```cpp
// 示例1：字符频次统计
#include <map>
#include <string>
using namespace std;

map<char, int> countCharacters(const string& s) {
    map<char, int> freq;
    for(char c : s) {
        freq[c]++;  // 自动创建或增加计数
    }
    return freq;
}

// 示例2：数组元素计数
map<int, int> countElements(const vector<int>& nums) {
    map<int, int> count;
    for(int num : nums) {
        count[num]++;
    }
    return count;
}

// 示例3：坐标压缩
vector<int> coordinateCompression(vector<int>& coords) {
    map<int, int> compress_map;
    set<int> unique_coords(coords.begin(), coords.end());
  
    int index = 0;
    for(int coord : unique_coords) {
        compress_map[coord] = index++;
    }
  
    vector<int> result;
    for(int coord : coords) {
        result.push_back(compress_map[coord]);
    }
    return result;
}

// 示例4：前缀和查询
class PrefixSumMap {
private:
    map<int, long long> prefix_sum;
  
public:
    void insert(int index, int value) {
        auto it = prefix_sum.upper_bound(index);
        long long sum = (it == prefix_sum.begin()) ? value : 
                       prev(it)->second + value;
        prefix_sum[index] = sum;
      
        // 更新后续所有前缀和
        while(it != prefix_sum.end()) {
            it->second += value;
            ++it;
        }
    }
  
    long long query(int index) {
        auto it = prefix_sum.upper_bound(index);
        return (it == prefix_sum.begin()) ? 0 : prev(it)->second;
    }
};
```

### 9.2 multimap<K,V> - 多重映射

#### 🔧 核心特性

**multimap** 允许一个键对应多个值，适用于一对多的映射关系。

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    multimap<string, int> student_scores;
  
    // 1. 插入一对多关系
    student_scores.insert({"Alice", 85});
    student_scores.insert({"Alice", 92});
    student_scores.insert({"Bob", 78});
    student_scores.insert({"Alice", 88});
  
    // 2. 查找某个键的所有值
    auto range = student_scores.equal_range("Alice");
    cout << "Alice的所有成绩: ";
    for(auto it = range.first; it != range.second; ++it) {
        cout << it->second << " ";
    }
    cout << endl;
  
    // 3. 计数
    cout << "Alice的成绩个数: " << student_scores.count("Alice") << endl;
  
    // 4. 删除特定键值对
    auto it = student_scores.find("Alice");
    if(it != student_scores.end()) {
        student_scores.erase(it);  // 只删除找到的第一个
    }
  
    return 0;
}
```

---

## 10. 关联容器在CSP中的应用

### 10.1 去重与查找问题

#### 🎯 使用set进行去重

```cpp
// 问题：给定数组，返回去重后的有序数组
#include <set>
#include <vector>
using namespace std;

vector<int> removeDuplicatesAndSort(vector<int>& nums) {
    set<int> unique_set(nums.begin(), nums.end());
    return vector<int>(unique_set.begin(), unique_set.end());
}

// 问题：判断两个数组是否有公共元素
bool hasCommonElement(vector<int>& arr1, vector<int>& arr2) {
    set<int> set1(arr1.begin(), arr1.end());
    for(int num : arr2) {
        if(set1.count(num)) return true;
    }
    return false;
}

// 问题：快速查找第k小的元素
int findKthSmallest(vector<int>& nums, int k) {
    set<int> sorted_unique(nums.begin(), nums.end());
    if(k > sorted_unique.size()) return -1;
  
    auto it = sorted_unique.begin();
    advance(it, k-1);
    return *it;
}
```

#### 🎯 使用map进行快速查找

```cpp
// 问题：两数之和
#include <map>
#include <vector>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    map<int, int> num_to_index;
    for(int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if(num_to_index.count(complement)) {
            return {num_to_index[complement], i};
        }
        num_to_index[nums[i]] = i;
    }
    return {};
}

// 问题：字符串中第一个不重复字符
char firstUniqueChar(string s) {
    map<char, int> freq;
    for(char c : s) {
        freq[c]++;
    }
    for(char c : s) {
        if(freq[c] == 1) return c;
    }
    return '\0';
}
```

### 10.2 计数与统计问题

#### 🎯 使用map进行计数

```cpp
// 问题：统计数组中每个元素的出现次数
#include <map>
#include <vector>
using namespace std;

map<int, int> countFrequency(vector<int>& nums) {
    map<int, int> freq;
    for(int num : nums) {
        freq[num]++;
    }
    return freq;
}

// 问题：找出出现次数最多的元素
int findMostFrequent(vector<int>& nums) {
    map<int, int> freq;
    for(int num : nums) {
        freq[num]++;
    }
  
    int max_count = 0, result = 0;
    for(const auto& pair : freq) {
        if(pair.second > max_count) {
            max_count = pair.second;
            result = pair.first;
        }
    }
    return result;
}

// 问题：统计字符串中单词频次
map<string, int> wordFrequency(const string& text) {
    map<string, int> freq;
    string word = "";
  
    for(char c : text + " ") {  // 添加空格确保最后一个单词被处理
        if(isalpha(c)) {
            word += tolower(c);
        } else if(!word.empty()) {
            freq[word]++;
            word = "";
        }
    }
    return freq;
}
```

#### 🎯 使用multiset进行排序计数

```cpp
// 问题：维护动态数据的中位数
#include <set>
using namespace std;

class DynamicMedian {
private:
    multiset<int> data;
  
public:
    void addNumber(int num) {
        data.insert(num);
    }
  
    double getMedian() {
        int n = data.size();
        auto it = data.begin();
        advance(it, n / 2);
      
        if(n % 2 == 1) {
            return *it;
        } else {
            double right = *it;
            --it;
            double left = *it;
            return (left + right) / 2.0;
        }
    }
};

// 问题：滑动窗口中的最大值（使用multiset）
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> result;
    multiset<int> window;
  
    for(int i = 0; i < nums.size(); i++) {
        window.insert(nums[i]);
      
        if(window.size() > k) {
            window.erase(window.find(nums[i - k]));
        }
      
        if(window.size() == k) {
            result.push_back(*window.rbegin());  // 最大值
        }
    }
    return result;
}
```

### 10.3 排序与范围查询

#### 🎯 使用set进行范围操作

```cpp
// 问题：范围内元素查询
#include <set>
#include <vector>
using namespace std;

vector<int> rangeQuery(set<int>& data, int left, int right) {
    vector<int> result;
    auto it_left = data.lower_bound(left);   // >= left
    auto it_right = data.upper_bound(right); // > right
  
    for(auto it = it_left; it != it_right; ++it) {
        result.push_back(*it);
    }
    return result;
}

// 问题：最接近目标值的元素
int closestValue(set<int>& data, int target) {
    auto it = data.lower_bound(target);
  
    if(it == data.end()) {
        return *data.rbegin();  // 最大值
    }
    if(it == data.begin()) {
        return *it;  // 最小值
    }
  
    int right = *it;
    --it;
    int left = *it;
  
    return (target - left <= right - target) ? left : right;
}

// 问题：区间合并
vector<pair<int,int>> mergeIntervals(vector<pair<int,int>>& intervals) {
    map<int, int> events;  // start -> +1, end+1 -> -1
  
    for(const auto& interval : intervals) {
        events[interval.first]++;
        events[interval.second + 1]--;
    }
  
    vector<pair<int,int>> result;
    int count = 0, start = -1;
  
    for(const auto& event : events) {
        if(count == 0 && event.second > 0) {
            start = event.first;
        }
        count += event.second;
        if(count == 0 && start != -1) {
            result.push_back({start, event.first - 1});
        }
    }
  
    return result;
}
```

---

## 📚 总结

### 🎯 快速决策表

#### 序列式容器选择

| 需求场景            | 首选容器         | 备选方案 | 关键考虑因素 |
| ------------------- | ---------------- | -------- | ------------ |
| 随机访问 + 尾部操作 | **vector** | deque    | 内存连续性   |
| 频繁中间插入删除    | **list**   | -        | 操作位置已知 |
| 双端操作 + 随机访问 | **deque**  | vector   | 操作频率     |
| 大量数据排序        | **vector** | -        | 缓存友好性   |
| 动态规划状态        | **vector** | -        | 随机访问需求 |
| 模拟队列/栈         | **deque**  | vector   | 操作类型     |

#### 关联容器选择

| 需求场景     | 首选容器           | 备选方案             | 关键考虑因素            |
| ------------ | ------------------ | -------------------- | ----------------------- |
| 去重 + 排序  | **set**      | vector+sort          | 动态性                  |
| 元素计数     | **map**      | unordered_map        | 是否需要排序            |
| 快速查找     | **set/map**  | vector+binary_search | 数据规模                |
| 重复元素排序 | **multiset** | vector               | 是否需要动态操作        |
| 一对多映射   | **multimap** | map<K,vector`<V>`> | 操作复杂度              |
| 范围查询     | **set/map**  | -                    | lower_bound/upper_bound |

### 🚀 CSP竞赛建议

#### 序列式容器

1. **默认选择vector**：90%的情况下vector都是最佳选择
2. **需要双端操作时选择deque**：特别是滑动窗口问题
3. **频繁中间操作时选择list**：如约瑟夫环、链表模拟
4. **注意预分配空间**：大数据量时使用 `reserve()`

#### 关联容器

5. **去重问题优先set**：自动排序+去重，比手动实现简单
6. **计数问题优先map**：`map[key]++`比手动计数方便
7. **查找问题考虑set/map**：O(log n)比线性查找快
8. **善用范围操作**：`lower_bound`、`upper_bound`解决范围查询

### 📊 容器性能总结

| 容器类型         | 查找     | 插入     | 删除     | 排序       | 随机访问 |
| ---------------- | -------- | -------- | -------- | ---------- | -------- |
| **vector** | O(n)     | O(1)*    | O(n)     | O(n log n) | O(1)     |
| **list**   | O(n)     | O(1)     | O(1)     | O(n log n) | ❌       |
| **deque**  | O(n)     | O(1)*    | O(n)     | O(n log n) | O(1)     |
| **set**    | O(log n) | O(log n) | O(log n) | 自动       | ❌       |
| **map**    | O(log n) | O(log n) | O(log n) | 自动       | ❌       |

*尾部插入为O(1)，其他位置为O(n)

### 🎯 问题类型与容器映射

| 问题类型           | 推荐容器组合   | 典型例子             |
| ------------------ | -------------- | -------------------- |
| **模拟题**   | vector + deque | 数组操作、队列模拟   |
| **搜索题**   | vector + set   | BFS + 去重           |
| **统计题**   | map + set      | 频次统计 + 去重      |
| **排序题**   | vector + set   | 快速排序 + 去重      |
| **查找题**   | set/map        | 快速判断存在性       |
| **动态维护** | multiset + map | 动态中位数、滑动窗口 |

### 📖 进阶学习

#### 序列式容器进阶

- 深入理解容器的内存模型和迭代器失效
- 学习自定义比较器和分配器
- 掌握容器适配器（stack、queue、priority_queue）

#### 关联容器进阶

- 掌握自定义比较函数（less、greater）
- 学习unordered_set/unordered_map的使用场景
- 了解红黑树的基本原理和性能特点
- 练习复杂的范围查询和区间操作



其他参考 https://www.runoob.com/cplusplus/cpp-stl-tutorial.html

---

*最后更新：2025年*
*适用范围：CSP-J/S竞赛备考*
