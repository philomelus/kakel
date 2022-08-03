extends Control

func _on_Sprite_position_changed(node: Node, new_pos: Vector2) -> void:
	print("The position of " + node.name + " is now " + str(new_pos))

