#ifndef NEWGAME_HPP_INCLUDED
#define NEWGAME_HPP_INCLUDED

#include <Godot.hpp>
#include <InputEvent.hpp>
#include <PanelContainer.hpp>

namespace kakel
{
	class NewGame : public godot::PanelContainer
	{
		GODOT_CLASS(NewGame, godot::PanelContainer)

	public:
		NewGame();
		~NewGame();

		void _draw();
		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _physics_process(const float delta);
		void _process(const float delta);
		void _ready();
		void _unhandled_input(const godot::Ref<godot::InputEvent> ev);
		
		static void _register_methods();

	private:
		NewGame* _impl;
		NewGame(const NewGame&);
		NewGame& operator=(const NewGame&);
	};
}
#endif	// NEWGAME_HPP_INCLUDED
