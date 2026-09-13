extends Node
## extensions 模块冒烟测试：覆盖 UUID、字典/文件/目录工具、预制体与项目设置。

func _ready() -> void:
	var runner := TestRunner.new()
	_test_uuid(runner)
	_test_dict_utils(runner)
	_test_file_utils(runner)
	_test_dir_utils(runner)
	_test_prefab(runner)
	_test_project_settings(runner)
	runner.report()
	var exit_code: int = 0 if runner.is_pass() else 1
	get_tree().quit(exit_code)

## 测试 UUID 生成与比较。
func _test_uuid(runner: TestRunner) -> void:
	var first_uuid: StringName = UUID.generate()
	runner.assert_eq(first_uuid.length(), 36, "UUID 文本长度应为 36")
	var segments: PackedStringArray = String(first_uuid).split("-")
	runner.assert_eq(segments.size(), 5, "UUID 应按连字符分为 5 段")
	runner.assert_eq(segments[0].length(), 8, "第一段长度为 8")
	runner.assert_eq(segments[1].length(), 4, "第二段长度为 4")
	runner.assert_eq(segments[2].length(), 4, "第三段长度为 4")
	runner.assert_eq(segments[3].length(), 4, "第四段长度为 4")
	runner.assert_eq(segments[4].length(), 12, "第五段长度为 12")
	var second_uuid: StringName = UUID.generate()
	runner.assert_true(first_uuid != second_uuid, "两次生成结果应不同")
	var object_a := UUID.new()
	var object_b := UUID.new()
	runner.assert_true(not object_a.equal_with(object_b), "两个独立 UUID 实例应互不相等")
	runner.assert_true(object_a.equal_with(object_a), "同一 UUID 实例应与自身相等")

## 测试字典键结构比较。
func _test_dict_utils(runner: TestRunner) -> void:
	var nested_a := {"name": "a", "nested": {"key": 1}}
	var nested_b := {"name": "b", "nested": {"key": 2}}
	var flat := {"name": "a", "nested": 9}
	runner.assert_true(DictUtils.has_same_keys_structure(nested_a, nested_b), "嵌套键结构一致应返回 true")
	runner.assert_false(DictUtils.has_same_keys_structure(nested_a, flat), "键结构不同应返回 false")
	runner.assert_true(DictUtils.has_same_keys_structure({}, {}), "两个空字典键结构一致")

## 测试文本/二进制文件判定。
func _test_file_utils(runner: TestRunner) -> void:
	runner.assert_true(FileUtils.is_text_file("res://project.godot"), "project.godot 应判定为文本文件")
	var binary_path: String = "user://test_binary.bin"
	_write_file(binary_path, PackedByteArray([0, 1, 2, 0, 255]))
	runner.assert_false(FileUtils.is_text_file(binary_path), "含 NUL 字节的文件应判定为二进制")
	runner.assert_false(FileUtils.is_text_file("user://not_exists.bin"), "不存在的文件应返回 false")
	DirAccess.remove_absolute(binary_path)

## 测试目录树、哈希、复制与删除。
func _test_dir_utils(runner: TestRunner) -> void:
	var base_path: String = "user://test_dir_utils"
	var make_error: Error = DirAccess.make_dir_recursive_absolute(base_path)
	runner.assert_eq(make_error, OK, "创建测试目录应成功")
	_write_file(base_path + "/a.txt", "hello".to_utf8_buffer())
	_write_file(base_path + "/b.dat", "binary".to_utf8_buffer())
	var dir_hash: String = DirUtils.compute_dir_hash(base_path)
	runner.assert_eq(dir_hash.length(), 32, "目录 MD5 应为 32 位")
	var filtered_hash: String = DirUtils.compute_dir_hash(base_path, PackedStringArray(["dat"]))
	runner.assert_eq(filtered_hash.length(), 32, "带扩展名过滤的哈希长度应为 32")
	var tree: Dictionary = DirUtils.get_directory_tree(base_path)
	runner.assert_keys(tree, ["name", "path", "directories", "files"], "目录树字典应含固定键")
	runner.assert_eq((tree["files"] as Array).size(), 2, "目录树应包含 2 个文件")
	var copy_path: String = "user://test_dir_utils_copy"
	runner.assert_eq(DirUtils.copy_recursive(base_path, copy_path), OK, "递归复制应返回 OK")
	runner.assert_true(DirAccess.dir_exists_absolute(copy_path), "复制的目标目录应存在")
	runner.assert_true(DirUtils.delete_directory_recursive(copy_path), "递归删除目标目录应成功")
	runner.assert_false(DirAccess.dir_exists_absolute(copy_path), "删除后目标目录应不存在")
	DirUtils.delete_directory_recursive(base_path)

## 测试节点树打包为 PackedScene。
func _test_prefab(runner: TestRunner) -> void:
	var root_node := Node.new()
	root_node.name = "Root"
	var child_node := Node.new()
	child_node.name = "Child"
	root_node.add_child(child_node)
	child_node.owner = root_node
	var packed_scene: PackedScene = Prefab.pack(root_node, Prefab.FREE_MODE_NONE)
	runner.assert_true(packed_scene != null, "打包应返回非空 PackedScene")
	var instance_node: Node = packed_scene.instantiate()
	runner.assert_true(instance_node != null, "打包结果应可实例化")
	runner.assert_eq(instance_node.name, &"Root", "实例根节点名应为 Root")
	runner.assert_true(instance_node.find_child("Child") != null, "实例应保留子节点")
	instance_node.queue_free()
	root_node.free()

## 测试项目设置的注册、读取与移除。
func _test_project_settings(runner: TestRunner) -> void:
	var setting: ProjectSetting = ProjectSetting.create("test/smoke_setting", "default_val", TYPE_STRING, "default_val")
	runner.assert_eq(setting.setting_path, "test/smoke_setting", "设置路径应正确")
	ProjectSettingsManager.add_setting(setting)
	runner.assert_eq(ProjectSettingsManager.get_setting(setting), "default_val", "读取设置应返回初始值")
	ProjectSettingsManager.remove_setting(setting)
	runner.assert_eq(ProjectSettingsManager.get_setting(setting), "default_val", "移除后读取应回落默认值")

## 以字节数组写入文件，用于构造测试数据。[br][br]
## [param path] 文件路径。[br]
## [param bytes] 要写入的原始字节。
func _write_file(path: String, bytes: PackedByteArray) -> void:
	var file := FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return
	file.store_buffer(bytes)
	file.close()