#ifndef TILE_CONTROL_HPP
#define TILE_CONTROL_HPP

#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot {

class TileControl : public Sprite {
    GODOT_CLASS(TileControl, Sprite)

private:
    float time_passed;
	float time_emit;
	float amplitude;
	float speed;
	
public:
    static void _register_methods();

    TileControl();
    ~TileControl();

    void _init();

    void _process(float delta);

	void set_speed(float p_speed);
	float get_speed();
};

}

#endif
