#include "TileControl.hpp"

using namespace godot;

void TileControl::_register_methods()
{
    register_method("_process", &TileControl::_process);
    register_property<TileControl, float>("amplitude", &TileControl::amplitude, 10.0);
    register_property<TileControl, float>("speed", &TileControl::set_speed, &TileControl::get_speed, 1.0);
	register_signal<TileControl>((char*) "position_changed", "node", GODOT_VARIANT_TYPE_OBJECT,
							   "new_pos", GODOT_VARIANT_TYPE_VECTOR2);
}

TileControl::TileControl()
{
}

TileControl::~TileControl()
{
}

void TileControl::_init()
{
    time_passed = 0.0;
    amplitude = 10.0;
    speed = 1.0;
	time_emit = 0.0;

	//some_other_node->connect("the_signal", this, "my_method");
}

void TileControl::_process(float delta)
{
    time_passed += speed * delta;

    Vector2 new_position = Vector2(
        amplitude + (amplitude * sin(time_passed * 2.0)),
        amplitude + (amplitude * cos(time_passed * 1.5))
    );

    set_position(new_position);

    time_emit += delta;
    if (time_emit > 1.0) {
        emit_signal("position_changed", this, new_position);
        time_emit = 0.0;
    }
}

void TileControl::set_speed(float p_speed)
{
	speed = p_speed;
}

float TileControl::get_speed()
{
	return speed;
}
