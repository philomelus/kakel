#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <Godot.hpp>
#include <InputEvent.hpp>
#include <PanelContainer.hpp>

namespace kakel
{
	class Game : public godot::PanelContainer
	{
		GODOT_CLASS(Game, godot::PanelContainer)

	public:
		Game();
		~Game();

		void _draw();
		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _physics_process(const float delta);
		void _process(const float delta);
		void _ready();
		void _unhandled_input(const godot::Ref<godot::InputEvent> ev);
		
		static void _register_methods();

	private:
		Game* _impl;
		Game(const Game&);
		Game& operator=(const Game&);
	};
}
#endif	// GAME_HPP_INCLUDED
