#ifdef TOOLS_ENABLED

#include "editor_script_extension.h"

#include "core/core_bind.h"
#include "core/object/callable_mp.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "editor/editor_interface.h"
#include "scene/animation/tween.h"
#include "scene/gui/dialogs.h"
#include "scene/main/window.h"
#include "scene/main/node.h"
#include "scene/main/scene_tree.h"

void EditorScriptExtension::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tree"), &EditorScriptExtension::get_tree);
	ClassDB::bind_method(D_METHOD("create_timer", "time_sec", "process_always", "process_in_physics", "ignore_time_scale"),
			&EditorScriptExtension::create_timer, DEFVAL(true), DEFVAL(false), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("create_tween"), &EditorScriptExtension::create_tween);
	ClassDB::bind_method(D_METHOD("create_window"), &EditorScriptExtension::create_window);
	ClassDB::bind_method(D_METHOD("create_accept_dialog", "dialog_text", "title", "ok_button_text"),
			&EditorScriptExtension::create_accept_dialog, DEFVAL(""), DEFVAL(""));
	ClassDB::bind_method(D_METHOD("create_confirmation_dialog", "dialog_text", "title", "ok_button_text"),
			&EditorScriptExtension::create_confirmation_dialog, DEFVAL(""), DEFVAL(""));
	GDVIRTUAL_BIND(main);
}

void EditorScriptExtension::run() {
	GDVIRTUAL_CALL(main);
}

SceneTree *EditorScriptExtension::get_tree() {
	return Object::cast_to<SceneTree>(OS::get_singleton()->get_main_loop());
}

RequiredResult<SceneTreeTimer> EditorScriptExtension::create_timer(float p_time_sec, bool p_process_always,
		bool p_process_in_physics, bool p_ignore_time_scale) {
	return get_tree()->create_timer(p_time_sec, p_process_always, p_process_in_physics, p_ignore_time_scale);
}

RequiredResult<Tween> EditorScriptExtension::create_tween() {
	return get_tree()->create_tween();
}

Window *EditorScriptExtension::create_window() {
	Window *window = memnew(Window);
	window->set_initial_position(Window::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
	window->connect("close_requested", callable_mp(static_cast<Node *>(window), &Node::queue_free));
	EditorInterface::get_singleton()->get_base_control()->add_child(window);
	return window;
}

AcceptDialog *EditorScriptExtension::create_accept_dialog(const String &p_dialog_text,
		const String &p_title, const String &p_ok_button_text) {
	AcceptDialog *dialog = memnew(AcceptDialog);
	dialog->set_text(p_dialog_text);
	if (!p_title.is_empty()) {
		dialog->set_title(p_title);
	}
	if (!p_ok_button_text.is_empty()) {
		dialog->set_ok_button_text(p_ok_button_text);
	}
	dialog->connect("close_requested", callable_mp(static_cast<Node *>(dialog), &Node::queue_free));
	EditorInterface::get_singleton()->get_base_control()->add_child(dialog);
	return dialog;
}

ConfirmationDialog *EditorScriptExtension::create_confirmation_dialog(const String &p_dialog_text,
		const String &p_title, const String &p_ok_button_text) {
	ConfirmationDialog *dialog = memnew(ConfirmationDialog);
	dialog->set_text(p_dialog_text);
	if (!p_title.is_empty()) {
		dialog->set_title(p_title);
	}
	if (!p_ok_button_text.is_empty()) {
		dialog->set_ok_button_text(p_ok_button_text);
	}
	dialog->connect("close_requested", callable_mp(static_cast<Node *>(dialog), &Node::queue_free));
	EditorInterface::get_singleton()->get_base_control()->add_child(dialog);
	return dialog;
}

#endif // TOOLS_ENABLED