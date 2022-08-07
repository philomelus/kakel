#ifndef GDEXAMPLE_HPP
#define GDEXAMPLE_HPP

#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot
{
	class GDExample : public Sprite
	{
		GODOT_CLASS(GDExample, Sprite)

	private:
		float time_passed;
		float amplitude;
		float speed;
	
	public:
		static void _register_methods();

		GDExample();
		~GDExample();

		void _init();
		void _process(float delta);
	
		void set_speed(float p_speed);
		float get_speed();
	};
}

#endif
