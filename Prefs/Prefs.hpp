#ifndef PREFS_HPP_INCLUDED
#define PREFS_HPP_INCLUDED

#include <Godot.hpp>
#include <InputEvent.hpp>
#include <PanelContainer.hpp>

namespace kakel
{
	class Prefs : public godot::PanelContainer
	{
		GODOT_CLASS(Prefs, godot::PanelContainer)

	public:
		Prefs();
		~Prefs();

		void _draw();
		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _physics_process(const float delta);
		void _process(const float delta);
		void _ready();
		void _unhandled_input(const godot::Ref<godot::InputEvent> ev);
		
		static void _register_methods();

	private:
		Prefs* _impl;
		Prefs(const Prefs&);
		Prefs& operator=(const Prefs&);
	};
}
#endif	// PREFS_HPP_INCLUDED
