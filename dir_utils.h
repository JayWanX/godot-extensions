#pragma once

#include "core/object/object.h"
#include "core/templates/vector.h"
#include "core/variant/callable.h"
#include "core/variant/dictionary.h"
#include "core/variant/variant.h"

// 目录操作工具类：提供目录遍历、递归复制/删除与哈希等静态方法。
class DirUtils : public Object {
	GDCLASS(DirUtils, Object);

	static void _bind_methods();

	// 递归收集文件：以 base_path 为根，记录每个文件相对根的路径与其修改时间。
	// 扩展名过滤（lowered_exts）非空时启用。
	static void _collect_files_recursive(const String &p_base_path,
			const String &p_relative_prefix,
			const Vector<String> &p_lowered_exts, bool p_filter_enabled,
			Vector<String> &r_file_list);

	// 递归删除以指定条件命中的文件，命中判定交给 should_delete 回调。
	static void _delete_files_if_recursive(const String &p_root_path,
			const Callable &p_should_delete, int &r_count);

	// 将若干 "name:modified_time" 记录用换行拼接后计算其 MD5。
	static String _hash_joined_lines(Vector<String> &r_file_list);

public:
	// 计算目录哈希（仅当前层文件，不含子目录）。
	static String compute_dir_hash(const String &p_path, const PackedStringArray &p_extensions = {});
	// 递归计算目录及所有子目录的哈希。
	static String compute_dir_hash_recursive(const String &p_path,
			const PackedStringArray &p_extensions = {});
	// 递归复制目录（目标目录会自动创建）。
	static Error copy_recursive(const String &p_src, const String &p_dst);
	// 递归删除目录（含所有子目录与文件）。
	static bool delete_directory_recursive(const String &p_path);
	// 获取目录树结构（字典形式）。
	static Dictionary get_directory_tree(const String &p_path);
	// 通用的目录树遍历，支持文件/目录回调，回调返回 false 终止遍历。
	static bool walk_directory_tree(const String &p_path, const Callable &p_file_handler,
			const Callable &p_dir_handler = Callable(), bool p_recursive = true);
	// 根据条件删除文件，返回实际删除数量。
	static int delete_files_if(const String &p_root_path, const Callable &p_should_delete);
};