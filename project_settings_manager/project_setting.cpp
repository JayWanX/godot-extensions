#include "project_setting.h"

#include "core/object/class_db.h"

Ref<ProjectSetting> ProjectSetting::create(
		const String &p_setting_path,
		const Variant &p_default_value,
		Variant::Type p_value_type,
		const Variant &p_initial_value,
		PropertyHint p_type_hint,
		const String &p_hint_string,
		bool p_basic,
		bool p_internal,
		bool p_restart_if_changed) {
	Ref<ProjectSetting> setting;
	setting.instantiate();
	setting->setting_path = p_setting_path;
	setting->default_value = p_default_value;
	setting->value_type = p_value_type;
	setting->initial_value = p_initial_value;
	setting->type_hint = p_type_hint;
	setting->hint_string = p_hint_string;
	setting->basic = p_basic;
	setting->internal = p_internal;
	setting->restart_if_changed = p_restart_if_changed;
	return setting;
}

void ProjectSetting::_bind_methods() {
	// 静态工厂：为带多字段初始化的创建提供入口（引擎类的 new() 固定 0 参数，无法传参）。
	// 除首参外均提供默认值，使脚本侧可省略尾部参数（仅连续省略末端参数）。
	ClassDB::bind_static_method("ProjectSetting", D_METHOD("create", "setting_path", "default_value", "value_type", "initial_value", "type_hint", "hint_string", "basic", "internal", "restart_if_changed"), &ProjectSetting::create,
			Variant(),
			Variant(Variant::Type::NIL),
			Variant(),
			Variant(PropertyHint::PROPERTY_HINT_NONE),
			String(),
			true,
			false,
			false);

	// 属性名通过 ClassDB 按名称解析 setter/getter，因此必须先注册这些私有存取方法。
	ClassDB::bind_method(D_METHOD("_set_setting_path", "value"), &ProjectSetting::_set_setting_path);
	ClassDB::bind_method(D_METHOD("_get_setting_path"), &ProjectSetting::_get_setting_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "setting_path"), "_set_setting_path", "_get_setting_path");

	ClassDB::bind_method(D_METHOD("_set_default_value", "value"), &ProjectSetting::_set_default_value);
	ClassDB::bind_method(D_METHOD("_get_default_value"), &ProjectSetting::_get_default_value);
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "default_value", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "_set_default_value", "_get_default_value");

	ClassDB::bind_method(D_METHOD("_set_value_type", "value"), &ProjectSetting::_set_value_type);
	ClassDB::bind_method(D_METHOD("_get_value_type"), &ProjectSetting::_get_value_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "value_type", PROPERTY_HINT_ENUM, "nil,bool,int,float,String,StringName,Vector2,Vector2i,Rect2,Rect2i,Vector3,Vector3i,Transform2D,Vector4,Vector4i,Plane,Quaternion,AABB,Basis,Transform3D,Color,Object,Callable,Signal,Dictionary,Array,Transform3D"), "_set_value_type", "_get_value_type");

	ClassDB::bind_method(D_METHOD("_set_initial_value", "value"), &ProjectSetting::_set_initial_value);
	ClassDB::bind_method(D_METHOD("_get_initial_value"), &ProjectSetting::_get_initial_value);
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "initial_value", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "_set_initial_value", "_get_initial_value");

	ClassDB::bind_method(D_METHOD("_set_type_hint", "value"), &ProjectSetting::_set_type_hint);
	ClassDB::bind_method(D_METHOD("_get_type_hint"), &ProjectSetting::_get_type_hint);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type_hint"), "_set_type_hint", "_get_type_hint");

	ClassDB::bind_method(D_METHOD("_set_hint_string", "value"), &ProjectSetting::_set_hint_string);
	ClassDB::bind_method(D_METHOD("_get_hint_string"), &ProjectSetting::_get_hint_string);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "hint_string"), "_set_hint_string", "_get_hint_string");

	ClassDB::bind_method(D_METHOD("_set_basic", "value"), &ProjectSetting::_set_basic);
	ClassDB::bind_method(D_METHOD("_get_basic"), &ProjectSetting::_get_basic);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "basic"), "_set_basic", "_get_basic");

	ClassDB::bind_method(D_METHOD("_set_internal", "value"), &ProjectSetting::_set_internal);
	ClassDB::bind_method(D_METHOD("_get_internal"), &ProjectSetting::_get_internal);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "internal"), "_set_internal", "_get_internal");

	ClassDB::bind_method(D_METHOD("_set_restart_if_changed", "value"), &ProjectSetting::_set_restart_if_changed);
	ClassDB::bind_method(D_METHOD("_get_restart_if_changed"), &ProjectSetting::_get_restart_if_changed);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "restart_if_changed"), "_set_restart_if_changed", "_get_restart_if_changed");
}