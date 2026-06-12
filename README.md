# 栈迁移 + BSS段利用 CTF 题目合集

专门收集栈迁移（Stack Pivoting / Stack Migration）到 BSS 段的二进制安全练习题目。

## 题目列表

### 1. ROP Emporium - pivot

- **难度**: ★★☆ (入门)
- **架构**: x86_64, ELF
- **保护**: NX enabled
- **技术点**: `xchg rsp, rax; ret` 栈迁移, ROP chain 到堆区域
- **特点**: 专门设计练习栈迁移，栈空间有限（~30字节后不可写），需要将 ROP 链 pivot 到可控区域
- **文件**: `pivot` (二进制), `libpivot.so`, `flag.txt`
- **来源**: https://ropemporium.com/challenge/pivot.html

### 2. NCKUCTF Pivoting Lab

- **难度**: ★★☆ (入门)
- **架构**: x86_64, 静态链接
- **保护**: NX, RELRO, no canary
- **技术点**: 伪造 rbp 实现栈迁移到 BSS 段的 `name` 数组, ret2syscall
- **特点**: 有全局 BSS 数组 name[0x100], buf 溢出 0x10 字节覆盖 rbp+ret, read 写入 BSS 段
- **文件**: `chal.c` (源码), `chal` (预编译), `Makefile`
- **来源**: https://github.com/pwn2ooown/2024-NCKUCTF-Pwn-Course

### 3. HITCON-Training lab5 - simplerop

- **难度**: ★★★ (进阶)
- **架构**: i386, 静态链接
- **保护**: NX enabled
- **技术点**: ret2syscall, 将 "/bin/sh" 写入 BSS 段, ROP 链构造
- **特点**: read 最多 100 字节, buf 偏移 32 字节, 需通过 ROP 将字符串写入 BSS 然后 execve
- **文件**: `simplerop` (二进制), `simplerop.c` (源码), `simplerop.py` (参考 exp)
- **来源**: https://github.com/scwuaptx/HITCON-Training

### 4. Defcon Quals 2019 - speedrun s4

- **难度**: ★★★ (进阶)
- **架构**: x86_64, 静态链接
- **保护**: NX enabled, no canary
- **技术点**: 单字节 rbp overflow, ret slide 栈迁移, BSS 写入 "/bin/sh" + SYS_execve
- **特点**: 溢出 1 字节修改 saved rbp, 通过 ret slide 概率性 pivot 到 ROP 链, 利用 mov [rax],rdx gadget 写 BSS
- **文件**: `speedrun-004` (二进制), `exploit.py` (参考 exp), `readme.md` (详细分析)
- **来源**: Defcon Quals 2019

### 5. Insomnihack Teaser 2018 - onewrite

- **难度**: ★★★★ (困难)
- **架构**: x86_64, static-pie
- **保护**: PIE, NX, Stack Canary
- **技术点**: `_fini_array` 劫持 + 栈迁移 + BSS 写入 "/bin/sh" + ROP
- **特点**: 8字节任意写, 通过修改 `_fini_array` 建立写循环, 逐步构造 ROP 链, `add rsp` 栈 pivot
- **文件**: `onewrite-*` (二进制), `exploit.py` (参考 exp), `readme.md` (详细分析)
- **来源**: Insomnihack Teaser 2018

### 6. ciscn_2019_es_2 [需编译]

- **难度**: ★★★ (进阶)
- **架构**: i386, 动态链接
- **保护**: NX enabled, no canary, no PIE
- **技术点**: 经典 leave;ret 栈迁移, printf 泄露 ebp, 两次 read 阶段式利用
- **特点**: 32位经典栈迁移题, 溢出仅 8 字节 (ebp+ret), 第一次 read 泄露栈地址, 第二次 read 完成迁移+执行 system
- **文件**: `ciscn_2019_es_2.c` (重构源码), `Makefile`
- **编译**: `make` (需要 gcc-multilib 支持 32位编译)
- **来源**: BUUCTF / CISCN 2019

## 学习路径建议

按难度递增：

1. **NCKUCTF Pivoting** - 理解 leave;ret 和 rbp 伪造基础
2. **ROP Emporium pivot** - 掌握 xchg rsp 型栈迁移
3. **ciscn_2019_es_2** - 练习 32位经典两阶段栈迁移
4. **HITCON simplerop** - ret2syscall + BSS 字符串写入
5. **speedrun s4** - 单字节溢出 + ret slide 概率性利用
6. **onewrite** - 高级：任意写循环 + fini_array 劫持

## 栈迁移核心知识点

### leave;ret 栈迁移原理

```
leave = mov rsp, rbp; pop rbp
ret   = pop rip
```

通过伪造 rbp 指向可控区域（如 BSS 段），执行 leave 后 rsp 迁移到可控区域，
再 ret 执行该区域的 ROP 链。

### BSS 段特点

- 未初始化全局变量存储区
- 地址固定（无 PIE 时）
- 可读写
- 适合作为栈迁移目标

### 常见 BSS 迁移场景

1. 有全局 buffer 在 BSS → 直接写入后 pivot
2. 通过 read/gets 写入 BSS → 伪造栈帧
3. ROP 链写 "/bin/sh" 到 BSS → execve 执行

## 更多资源

- CTF-Wiki 栈迁移: https://ctf-wiki.org/pwn/linux/user-mode/stackoverflow/x86/stack-pivoting/
- BUUCTF 在线平台: https://buuoj.cn (搜索 ciscn_2019_es_2 可在线上环境做题)
- ROP Emporium: https://ropemporium.com
