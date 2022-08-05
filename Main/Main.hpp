#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <Godot.hpp>
#include <InputEvent.hpp>
#include <PanelContainer.hpp>

namespace kakel
{
	class Main : public godot::PanelContainer
	{
		GODOT_CLASS(Main, godot::PanelContainer)

	public:
		Main();
		~Main();

		void _draw();
		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _physics_process(const float delta);
		void _process(const float delta);
		void _ready();
		void _unhandled_input(const godot::Ref<godot::InputEvent> ev);
		
		static void _register_methods();

	private:
		Main* _impl;
		Main(const Main&);
		Main& operator=(const Main&);
	};
}
#endif	// MAIN_HPP_INCLUDED
