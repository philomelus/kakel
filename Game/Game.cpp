#include "Game.hpp"
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class GameImpl : public kakel::Game
	{
		friend kakel::Game;
		
	protected:
		SceneTree* _tree;

	public:
		GameImpl()
		{
		}

		~GameImpl()
		{
		}
		
	protected:
		void draw()
		{
		}

		void init()
		{
			_tree = get_tree();
		}
		
		void input(const godot::Ref<godot::InputEvent> ev)
		{
		}
	
		void physics_process(const float delta)
		{
		}
	
		void _process(const float delta)
		{
		}
	
		void ready()
		{
		}
	
		void unhandled_input(const godot::Ref<godot::InputEvent> ev)
		{
		}
	};
	
	inline GameImpl* self(kakel::Game* obj) { return reinterpret_cast<GameImpl*>(obj); }
}


namespace kakel
{
	void Game::_register_methods()
	{
		// API
		register_method("_draw", &Game::_draw);
		register_method("_init", &Game::_init);
		register_method("_input", &Game::_input);
		register_method("_physics_process", &Game::_physics_process);
		register_method("_process", &Game::_process);
		register_method("_ready", &Game::_ready);
		register_method("_unhandled_input", &Game::_unhandled_input);
	}

	Game::Game()
	{
		_impl = GameImpl::_new();
	}

	Game::~Game()
	{
		GameImpl* tmp = _impl;
		_impl = nullptr;
		godot::api->godot_free(_impl);
	}

	void Game::_draw()
	{
		self(_impl)->draw();
	}
	
	void Game::_init()
	{
		self(_impl)->_init();
	}

	void Game::_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->input(ev);
	}
	
	void Game::_physics_process(const float delta)
	{
		self(_impl)->physics_process(delta);
	}
	
	void Game::_process(const float delta)
	{
		self(_impl)->process(delta);
	}
	
	void Game::_ready()
	{
		self(_impl)->ready();
	}
	
	void Game::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->unhandled_input(ev);
	}
}
