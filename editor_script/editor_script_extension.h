#pragma once

#ifdef TOOLS_ENABLED

#include "editor/script/editor_script.h"

class SceneTree;
class SceneTreeTimer;
class Tween;
class Window;
class AcceptDialog;
class ConfirmationDialog;

/// EditorScript 的便捷基类：统一管理生命周期并封装常用编辑器辅助方法。
/// GDScript 子类通过 extends EditorScriptExtension 重写 main() 实现具体逻辑。
class EditorScriptExtension : public EditorScript {
	GDCLASS(EditorScriptExtension, EditorScript);

	static void _bind_methods();

protected:
	/// 供 GDScript 重写的抽象入口。
	GDVIRTUAL0_REQUIRED(main)

public:
	/// 运行入口，供编辑器调用，内部转发到 GDScript 的 main()。
	void run() override;

	/// 获取当前主循环的 SceneTree。
	/// [return] 当前主循环的 SceneTree。
	SceneTree *get_tree();
	/// 创建场景树计时器。
	/// [param p_time_sec] 计时秒数
	/// [param p_process_always] 是否在时间缩放时仍计时
	/// [param p_process_in_physics] 是否在物理帧更新
	/// [param p_ignore_time_scale] 是否忽略时间缩放
	RequiredResult<SceneTreeTimer> create_timer(float p_time_sec, bool p_process_always = true,
			bool p_process_in_physics = false, bool p_ignore_time_scale = false);
	/// 创建场景树补间器。
	/// [return] 新建的 Tween 补间器。
	RequiredResult<Tween> create_tween();
	/// 创建一个以编辑器为主控件为父级的窗口。
	/// [return] 新建的 Window，关闭时自动释放。
	Window *create_window();
	/// 创建一个以编辑器为主控件为父级的确认框。
	/// [param p_dialog_text] 对话框文本
	/// [param p_title] 窗口标题
	/// [param p_ok_button_text] 确认按钮文本
	/// [return] 新建的 AcceptDialog，关闭时自动释放。
	AcceptDialog *create_accept_dialog(const String &p_dialog_text, const String &p_title = "",
			const String &p_ok_button_text = "");
	/// 创建一个"确定/取消"对话框。
	/// [param p_dialog_text] 对话框文本
	/// [param p_title] 窗口标题
	/// [param p_ok_button_text] 确认按钮文本
	/// [return] 新建的 ConfirmationDialog，关闭时自动释放。
	ConfirmationDialog *create_confirmation_dialog(const String &p_dialog_text, const String &p_title = "",
			const String &p_ok_button_text = "");
};

#endif // TOOLS_ENABLED