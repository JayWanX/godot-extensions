#include "register_types.h"

#include "core/object/class_db.h"

#include "dict_utils.h"
#include "dir_utils.h"
#include "file_utils.h"
#include "prefab.h"
#include "uuid.h"

// 模块初始化入口，由引擎构建系统在相应阶段调用。
// 静态工具类声明为抽象类（@abstract），仅暴露静态方法，禁止实例化。
void initialize_ext_tools_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<UUID>();
	ClassDB::register_abstract_class<DictUtils>();
	ClassDB::register_abstract_class<FileUtils>();
	ClassDB::register_abstract_class<DirUtils>();
	ClassDB::register_abstract_class<Prefab>();
}

void uninitialize_ext_tools_module(ModuleInitializationLevel p_level) {
	// 本模块无全局单例或静态资源需要释放。
}