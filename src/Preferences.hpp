#ifndef PREFERENCES_HPP_INCLUDED
#define PREFERENCES_HPP_INCLUDED

#include <Godot.hpp>
#include <InputEvent.hpp>
#include <Node.hpp>

namespace kakel
{
	class Preferences : public godot::Node
	{
		GODOT_CLASS(Preferences, godot::Node)

	public:
		Preferences();
		~Preferences();

		void _draw();
		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _physics_process(const float delta);
		void _process(const float delta);
		void _ready();
		void _unhandled_input(const godot::Ref<godot::InputEvent> ev);
		
		static void _register_methods();

	private:
		Preferences* _impl;
		Preferences(const Preferences&);
		Preferences& operator=(const Preferences&);
	};
}
#endif	// PREFERENCES_HPP_INCLUDED
