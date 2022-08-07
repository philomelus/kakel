#include "gdexample.hpp"

using namespace godot;

void GDExample::_register_methods()
{
    register_method("_process", &GDExample::_process);
    register_property<GDExample, float>("amplitude", &GDExample::amplitude, 10.0);
    register_property<GDExample, float>("speed", &GDExample::set_speed, &GDExample::get_speed, 1.0);
}

GDExample::GDExample()
{
}

GDExample::~GDExample()
{
}

void GDExample::_init()
{
	godot::Godot::print(String("GDExample::_init called."));
						
    time_passed = 0.0;
    amplitude = 10.0;
    speed = 1.0;
}

void GDExample::_process(float delta)
{
    time_passed += speed * delta;

    Vector2 new_position = Vector2(
        amplitude + (amplitude * sin(time_passed * 2.0)),
        amplitude + (amplitude * cos(time_passed * 1.5))
    );

    set_position(new_position);
}

void GDExample::set_speed(float p_speed)
{
    speed = p_speed;
}

float GDExample::get_speed()
{
    return speed;
}
