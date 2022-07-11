extends Control


onready var _acceptDialog: AcceptDialog = get_node("AcceptDialog")
onready var _control: Control = get_node(".")
onready var _fileDialog: FileDialog = get_node("FileDialog")
onready var _tree: SceneTree = get_tree()


func _on_AcceptDialog_confirmed():
	_tree.paused = false


func _on_FileDialog_file_selected(path):
	Globals.TilesLoading = true
	Globals.TilesLoadPath = path
	_tree.paused = false
	var _unused = _tree.change_scene("res://Game.tscn")


func _on_FileDialog_modal_closed():
	_tree.paused = false


func _on_FileDialog_popup_hide():
	_tree.paused = false


func _on_Load_pressed():
	_tree.paused = true
	_fileDialog.popup_centered()


func _on_New_pressed():
	new_game()


func _on_Quit_pressed():
	_tree.quit()


func _ready():
	# Start with appropriate screen size for detected OS
	_tree.paused = false
	match OS.get_name():
		"Windows":
			OS.min_window_size = Vector2(640, 480)
		_:
			OS.min_window_size = Vector2(240, 240)


func _unhandled_input(event):
	if event.is_action_pressed("quit"):
		accept_event()
		_tree.quit()
		return
	if event.is_action_pressed("new"):
		accept_event()
		new_game()
		return


func new_game():
	var _unused =  _tree.change_scene("res://NewGame.tscn")


#func _notification(what):
#	match what:
#		NOTIFICATION_APP_PAUSED:
#			print("notification: APP_PAUSED (%d)" % what)
#		NOTIFICATION_APP_RESUMED:
#			print("notification: APP_RESUMED (%d)" % what)
#		NOTIFICATION_CRASH:
#			print("notification: CRASH (%d)" % what)
#		NOTIFICATION_DRAG_BEGIN:
#			print("notification: DRAG_BEGIN (%d)" % what)
#		NOTIFICATION_DRAG_END:
#			print("notification: DRAG_END (%d)" % what)
#		NOTIFICATION_DRAW:
#			print("notification: DRAW (%d)" % what)
#		10000:
#		#NOTIFICATION_EDITOR_SETTINGS_CHANGED:
#			print("notification: EDITOR_SETTINGS_CHANGED (%d)" % what)
#		NOTIFICATION_ENTER_CANVAS:
#			print("notification: ENTER_CANVAS (%d)" % what)
#		45:
#		#NOTIFICATION_ENTER_GAMEPLAY:
#			print("notification: ENTER_GAMEPLAY (%d)" % what)
#		NOTIFICATION_ENTER_TREE:
#			print("notification: ENTER_TREE (%d)" % what)
#		41:
#		#NOTIFICATION_ENTER_WORLD:
#			print("notification: ENTER_WORLD (%d)" % what)
#		NOTIFICATION_EXIT_CANVAS:
#			print("notification: EXIT_CANVAS (%d)" % what)
#		46:
#		#NOTIFICATION_EXIT_GAMEPLAY:
#			print("notification: EXIT_GAMEPLAY (%d)" % what)
#		NOTIFICATION_EXIT_TREE:
#			print("notification: EXIT_TREE (%d)" % what)
#		42:
#		#NOTIFICATION_EXIT_WORLD:
#			print("notification: EXIT_WORLD (%d)" % what)
#		NOTIFICATION_FOCUS_ENTER:
#			print("notification: FOCUS_ENTER (%d)" % what)
#		NOTIFICATION_FOCUS_EXIT:
#			print("notification: FOCUS_EXIT (%d)" % what)
#		NOTIFICATION_INSTANCED:
#			print("notification: INSTANCED (%d)" % what)
#		NOTIFICATION_INTERNAL_PHYSICS_PROCESS:
#			print("notification: INTERNAL_PHYSICS_PROCESS (%d)" % what)
#		NOTIFICATION_INTERNAL_PROCESS:
#			print("notification: INTERNAL_PROCESS (%d)" % what)
#		NOTIFICATION_LOCAL_TRANSFORM_CHANGED:
#			print("notification: LOCAL_TRANSFORM_CHANGED (%d)" % what)
#		NOTIFICATION_MODAL_CLOSE:
#			print("notification: MODAL_CLOSE (%d)" % what)
#		NOTIFICATION_MOUSE_ENTER:
#			print("notification: MOUSE_ENTER (%d)" % what)
#		NOTIFICATION_MOUSE_EXIT:
#			print("notification: MOUSE_EXIT (%d)" % what)
#		NOTIFICATION_MOVED_IN_PARENT:
#			print("notification: MOVED_IN_PARENT (%d)" % what)
#		NOTIFICATION_OS_IME_UPDATE:
#			print("notification: OS_IME_UPDATE (%d)" % what)
#		NOTIFICATION_OS_MEMORY_WARNING:
#			print("notification: OS_MEMORY_WARNING (%d)" % what)
#		NOTIFICATION_PARENTED:
#			print("notification: PARENTED (%d)" % what)
#		NOTIFICATION_PATH_CHANGED:
#			print("notification: PATH_CHANGED (%d)" % what)
#		NOTIFICATION_PAUSED:
#			print("notification: PAUSED (%d)" % what)
#		NOTIFICATION_PHYSICS_PROCESS:
#			print("notification: PHYSICS_PROCESS (%d)" % what)
#		81:
#		#NOTIFICATION_POPUP_HIDE:
#			print("notification: POPUP_HIDE (%d)" % what)
#		NOTIFICATION_POST_ENTER_TREE:
#			print("notification: POST_ENTER_TREE (%d)" % what)
#		80:
#		#NOTIFICATION_POST_POPUP:
#			print("notification: POST_POPUP (%d)" % what)
#		NOTIFICATION_POSTINITIALIZE:
#			print("notification: POSTINITIALIZE (%d)" % what)
#		NOTIFICATION_PREDELETE:
#			print("notification: PREDELETE (%d)" % what)
#		NOTIFICATION_PROCESS:
#			print("notification: PROCESS (%d)" % what)
#		NOTIFICATION_READY:
#			print("notification: READY (%d)" % what)
#		NOTIFICATION_RESET_PHYSICS_INTERPOLATION:
#			print("notification: RESET_PHYSICS_INTERPOLATION (%d)" % what)
#		NOTIFICATION_RESIZED:
#			print("notification: RESIZED (%d)" % what)
#		NOTIFICATION_SCROLL_BEGIN:
#			print("notification: SCROLL_BEGIN (%d)" % what)
#		NOTIFICATION_SCROLL_END:
#			print("notification: SCROLL_END (%d)" % what)
#		50:
#		#NOTIFICATION_SORT_CHILDREN:
#			print("notification: SORT_CHILDREN or UPDATE_SKELETON (%d)" % what)
#		NOTIFICATION_THEME_CHANGED:
#			print("notification: THEME_CHANGED (%d)" % what)
#		NOTIFICATION_TRANSFORM_CHANGED:
#			print("notification: TRANSFORM_CHANGED (%d)" % what)
#		NOTIFICATION_TRANSLATION_CHANGED:
#			print("notification: TRANSLATION_CHANGED (%d)" % what)
#		NOTIFICATION_UNPARENTED:
#			print("notification: UNPARENTED (%d)" % what)
#		NOTIFICATION_UNPAUSED:
#			print("notification: UNPAUSED (%d)" % what)
#		#NOTIFICATION_UPDATE_SKELETON:
#		#	print("notification: UPDATE_SKELETON (%d)" % what)
#		NOTIFICATION_VISIBILITY_CHANGED:
#			print("notification: VISIBILITY_CHANGED (%d)" % what)
#		NOTIFICATION_WM_ABOUT:
#			print("notification: WM_ABOUT (%d)" % what)
#		NOTIFICATION_WM_FOCUS_IN:
#			print("notification: WM_FOCUS_IN (%d)" % what)
#		NOTIFICATION_WM_FOCUS_OUT:
#			print("notification: WM_FOCUS_OUT (%d)" % what)
#		NOTIFICATION_WM_GO_BACK_REQUEST:
#			print("notification: WM_GO_BACK_REQUEST (%d)" % what)
#		NOTIFICATION_WM_MOUSE_ENTER:
#			print("notification: WM_MOUSE_ENTER (%d)" % what)
#		NOTIFICATION_WM_MOUSE_EXIT:
#			print("notification: WM_MOUSE_EXIT (%d)" % what)
#		NOTIFICATION_WM_QUIT_REQUEST:
#			print("notification: WM_QUIT_REQUEST (%d)" % what)
#		NOTIFICATION_WM_UNFOCUS_REQUEST:
#			print("notification: WM_UNFOCUS_REQUEST (%d)" % what)
#		_:
#			print("notification: unknown - %d" % what)


