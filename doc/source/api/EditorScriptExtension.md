# EditorScriptExtension

继承自：[EditorScript](https://docs.godotengine.org/en/stable/classes/class_editorscript.html)

EditorScript 的便捷基类：统一管理生命周期并封装常用编辑器辅助方法。

GDScript 子类通过 extends EditorScriptExtension 重写 main() 实现具体逻辑。

## 方法：


返回值                                                                                                 | 函数签名                                                                                                                                                                                                                                                                                                                                                                                                                
--------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
[AcceptDialog](https://docs.godotengine.org/en/stable/classes/class_acceptdialog.html)              | [create_accept_dialog](#i_create_accept_dialog) ( [String](https://docs.godotengine.org/en/stable/classes/class_string.html) dialog_text, [String](https://docs.godotengine.org/en/stable/classes/class_string.html) title="", [String](https://docs.godotengine.org/en/stable/classes/class_string.html) ok_button_text="" )                                                                                       
[ConfirmationDialog](https://docs.godotengine.org/en/stable/classes/class_confirmationdialog.html)  | [create_confirmation_dialog](#i_create_confirmation_dialog) ( [String](https://docs.godotengine.org/en/stable/classes/class_string.html) dialog_text, [String](https://docs.godotengine.org/en/stable/classes/class_string.html) title="", [String](https://docs.godotengine.org/en/stable/classes/class_string.html) ok_button_text="" )                                                                           
[SceneTreeTimer](https://docs.godotengine.org/en/stable/classes/class_scenetreetimer.html)          | [create_timer](#i_create_timer) ( [float](https://docs.godotengine.org/en/stable/classes/class_float.html) time_sec, [bool](https://docs.godotengine.org/en/stable/classes/class_bool.html) process_always=true, [bool](https://docs.godotengine.org/en/stable/classes/class_bool.html) process_in_physics=false, [bool](https://docs.godotengine.org/en/stable/classes/class_bool.html) ignore_time_scale=false )  
[Tween](https://docs.godotengine.org/en/stable/classes/class_tween.html)                            | [create_tween](#i_create_tween) ( )                                                                                                                                                                                                                                                                                                                                                                                 
[Window](https://docs.godotengine.org/en/stable/classes/class_window.html)                          | [create_window](#i_create_window) ( )                                                                                                                                                                                                                                                                                                                                                                               
[SceneTree](https://docs.godotengine.org/en/stable/classes/class_scenetree.html)                    | [get_tree](#i_get_tree) ( )                                                                                                                                                                                                                                                                                                                                                                                         
[void](#)                                                                                           | [main](#i_main) ( ) virtual required                                                                                                                                                                                                                                                                                                                                                                                
<p></p>

## 方法描述

### [AcceptDialog](https://docs.godotengine.org/en/stable/classes/class_acceptdialog.html)<span id="i_create_accept_dialog"></span> **create_accept_dialog**( [String](https://docs.godotengine.org/en/stable/classes/class_string.html) dialog_text, [String](https://docs.godotengine.org/en/stable/classes/class_string.html) title="", [String](https://docs.godotengine.org/en/stable/classes/class_string.html) ok_button_text="" ) 

创建一个以编辑器为主控件为父级的确认框。

**dialog_text：** 对话框文本

**title：** 窗口标题

**ok_button_text：** 确认按钮文本

新建的 AcceptDialog，关闭时自动释放。

### [ConfirmationDialog](https://docs.godotengine.org/en/stable/classes/class_confirmationdialog.html)<span id="i_create_confirmation_dialog"></span> **create_confirmation_dialog**( [String](https://docs.godotengine.org/en/stable/classes/class_string.html) dialog_text, [String](https://docs.godotengine.org/en/stable/classes/class_string.html) title="", [String](https://docs.godotengine.org/en/stable/classes/class_string.html) ok_button_text="" ) 

创建一个"确定/取消"对话框。

**dialog_text：** 对话框文本

**title：** 窗口标题

**ok_button_text：** 确认按钮文本

新建的 ConfirmationDialog，关闭时自动释放。

### [SceneTreeTimer](https://docs.godotengine.org/en/stable/classes/class_scenetreetimer.html)<span id="i_create_timer"></span> **create_timer**( [float](https://docs.godotengine.org/en/stable/classes/class_float.html) time_sec, [bool](https://docs.godotengine.org/en/stable/classes/class_bool.html) process_always=true, [bool](https://docs.godotengine.org/en/stable/classes/class_bool.html) process_in_physics=false, [bool](https://docs.godotengine.org/en/stable/classes/class_bool.html) ignore_time_scale=false ) 

创建场景树计时器。

**time_sec：** 计时秒数

**process_always：** 是否在时间缩放时仍计时

**process_in_physics：** 是否在物理帧更新

**ignore_time_scale：** 是否忽略时间缩放

### [Tween](https://docs.godotengine.org/en/stable/classes/class_tween.html)<span id="i_create_tween"></span> **create_tween**( ) 

创建场景树补间器。

新建的 Tween 补间器。

### [Window](https://docs.godotengine.org/en/stable/classes/class_window.html)<span id="i_create_window"></span> **create_window**( ) 

创建一个以编辑器为主控件为父级的窗口。

新建的 Window，关闭时自动释放。

### [SceneTree](https://docs.godotengine.org/en/stable/classes/class_scenetree.html)<span id="i_get_tree"></span> **get_tree**( ) 

获取当前主循环的 SceneTree。

当前主循环的 SceneTree。

### [void](#)<span id="i_main"></span> **main**( ) 

供 GDScript 重写的抽象入口。

_生成于 2026-09-12_
