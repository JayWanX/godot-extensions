# extensions 模块自动文档生成计划

## Context（背景）

extensions 是一个 Godot C++ 模块（`D:\Code\Godot\custom_modules\extensions`），目前头文件用普通 `//` 注释，Godot 的 `--doctool` 无法识别、无法据此生成编辑器 API 文档。同目录的 voxel 模块已有一套成熟流程：C++ 用 `##` 文档注释 → 用编译进该模块的 Godot 编辑器跑 `--doctool` 生成 `doc/classes/*.xml` → 脚本把 XML 转成 Markdown API 文档。

目标：为 extensions 建立同样的流程，并从 `D:\Code\Godot\projects\Addons\addons\extensions`（GDScript 封装插件）复用其中现成的中文注释/描述，把 C++ 头文件的 `//` 注释升级为 `##` 文档注释。

范围（已与用户确认）：
- 产出形式：仅 XML + Markdown（不搭 mkdocs 站点）。
- 注释：本次一并迁移所有头文件注释。
- 验证：doctool 运行需要一台编译进 extensions 的编辑器，本次不验证，仅搭好管道与注释。

## 文件与实现

### 1. 搭建 doc 目录（复制自 voxel 并精简）

在 `D:\Code\Godot\custom_modules\extensions\` 下新建 `doc/`，从 voxel 复用：

| 目标 | 来源 | 处理 |
|------|------|------|
| `doc/tools/bbcode.py` | voxel `doc/tools/bbcode.py` | 原样复制 |
| `doc/tools/markdown.py` | voxel `doc/tools/markdown.py` | 原样复制（本地 markdown 渲染替代 pip 的 markdown 包） |
| `doc/tools/bbcode_to_markdown.py` | voxel 同名 | 原样复制 |
| `doc/tools/xml_to_markdown.py` | voxel 同名 | 原样复制 |
| `doc/tools/build.py` | voxel `doc/tools/build.py` | **适配**：删除 mkdocs 相关逻辑（`update_mkdocs_file`、`-m` 参数、`all_classes` 特判），仅保留 `-d`(运行 doctool) / `-a`(XML→Markdown)；`godot_repo_root` 默认指向 `parents[4]` 不变，把默认 bin 搜索路径与真实引擎 `sources_stable/godot-4.7.2-stable/bin` 对齐（仍可用 `-g` 覆盖） |
| `doc/classes/README.md` | voxel 同名 | 原样复制（说明这是 doctool 输出目录） |
| `doc/source/api/` | — | 新建空目录，Markdown 输出位置（build.py 的 `md_path` 指向此处） |

> `extensions/config.py` 已包含 `get_doc_classes()` 与 `get_doc_path()`（返回 `doc/classes`），无需改动，doctool 会自动把类 XML 写到该目录。

### 2. 头文件注释 `//` → `##`（从 addon 复用/移植）

遍历全部头文件，把类、枚举、成员、方法上的 `//` 注释改写为 `##` 文档注释；风格沿用 Godot/voxel 惯例（首行一句话概括，`[br]` 分段，`[param]`/`[return]` 标签），中文描述优先复用 addon `.gd` 中已有文本：

| 头文件 | 参照 addon | 要点 |
|--------|-----------|------|
| `uuid/uuid.h` | `uuid/uuid.gd` | `generate()` 随机字节 GUID 说明；`equal_with()`、构造函数 |
| `collection/dict_utils.h` | 无对应用例 | 按现有注释拟中文文档；`has_same_keys_structure()` 仅比键形状 |
| `file_system/file_utils.h` | `file_utils.gd` | `is_text_file()` 快路径+NUL 兜底描述、`[param file]`/`[param max_read_bytes]`；私有白名单访问器一致化 |
| `file_system/dir_utils.h` | `dir_utils.gd` | 7 个公共方法逐一移植：`compute_dir_hash`、`compute_dir_hash_recursive`、`copy_recursive`、`delete_directory_recursive`、`get_directory_tree`、`walk_directory_tree`、`delete_files_if`；3 个私有方法一行概括 |
| `prefab/prefab.h` | `prefab.gd` | `FreeMode` 枚举 3 值注释；`pack()` 与 owner 修复说明 |
| `project_settings_manager/project_setting.h` | `project_setting.gd` | 9 个 exported 成员字段各自一句话；私有访问器不写（getter/setter）或极简 |
| `project_settings_manager/project_settings_manager.h` | `project_settings_manager.gd` | 5 个公共方法 + 4 个私有方法，含 `[param]`/（必要时）`[return]` |
| `editor_script/editor_script_extension.h` | `editor_script.gd` | 类说明、`get_tree`、`create_timer`/`create_tween`（`[param]` 齐全）、`create_window`/`create_accept_dialog`/`create_confirmation_dialog`。`run()`、GDVIRTUAL0_REQUIRED(main) 按内置/引擎写法处理 |

> EditorScriptExtension.h 整体在 `#ifdef TOOLS_ENABLED` 内，文档注释不受影响。私有成员（`_` 前缀）用一行 `##`；内置虚方法/信号回调不写文档注释（本模块基本无信号）。

### 3. Markdown 生成（受 doctool 依赖限制）

原则是"改好注释 + 管道就绪，doctool 出的真实 XML 一把生成"。由于约定本次不验证 doctool：
- 用 `xml_to_markdown.py` 时缺少真实 `doc/classes/*.xml`。
- 为验证管道本身可跑通，可手写 1 个最小 `class_DictUtils.xml`（仅类+1 方法，忠实于头文件）作为冒烟样本，跑 `build.py -a` 验证生成 `doc/source/api/DictUtils.md`。
- 该样本仅作为管道自检；真实 XML 待将来加载 extensions 的编辑器运行 `build.py -d && build.py -a` 后覆盖生成。

## 验证

- `python doc/tools/build.py -a`：在含手写样本 XML 时能产出 `doc/source/api/DictUtils.md`，无异常退出。
- 目视各头文件 `##` 注释符合"一句话 + [br] + [param]/[return]"风格，中文表述与 addon `.gd` 一致。
- 重新加载 IntelliSense，确认无新增报错（`##` 注释不改动逻辑，只有注释变更）。
- 注：C++ 实现 `.cpp` 不改动；仅头文件注释变化不影响编译产物正确性。

## 提交

两个独立 Git 库分别提交：
- `extensions`：doc 管道 + 头文件注释升级。
- （voxel 无改动，不提交。）