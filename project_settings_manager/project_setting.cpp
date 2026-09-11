#include "project_setting.h"

#include "core/object/class_db.h"

void ProjectSetting::_bind_methods() {
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "setting_path"), "_set_setting_path", "_get_setting_path");
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "default_value", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "_set_default_value", "_get_default_value");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "value_type", PROPERTY_HINT_ENUM, "nil,bool,int,float,String,StringName,Vector2,Vector2i,Rect2,Rect2i,Vector3,Vector3i,Transform2D,Vector4,Vector4i,Plane,Quaternion,AABB,Basis,Transform3D,Color,Object,Callable,Signal,Dictionary,Array,Transform3D"), "_set_value_type", "_get_value_type");
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "initial_value", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT), "_set_initial_value", "_get_initial_value");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type_hint"), "_set_type_hint", "_get_type_hint");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "hint_string"), "_set_hint_string", "_get_hint_string");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "basic"), "_set_basic", "_get_basic");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "internal"), "_set_internal", "_get_internal");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "restart_if_changed"), "_set_restart_if_changed", "_get_restart_if_changed");
}