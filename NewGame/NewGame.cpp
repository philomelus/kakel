#include "NewGame.hpp"
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class NewGameImpl : public kakel::NewGame
	{
		friend kakel::NewGame;
		
	protected:
		SceneTree* _tree;

	public:
		NewGameImpl()
		{
		}

		~NewGameImpl()
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
	
		void process(const float delta)
		{
		}
	
		void ready()
		{
		}
	
		void unhandled_input(const godot::Ref<godot::InputEvent> ev)
		{
		}
	};
	
	inline NewGameImpl* self(kakel::NewGame* obj) { return reinterpret_cast<NewGameImpl*>(obj); }
}


namespace kakel
{
	void NewGame::_register_methods()
	{
		// API
		register_method("_draw", &NewGame::_draw);
		register_method("_init", &NewGame::_init);
		register_method("_input", &NewGame::_input);
		register_method("_physics_process", &NewGame::_physics_process);
		register_method("_process", &NewGame::_process);
		register_method("_ready", &NewGame::_ready);
		register_method("_unhandled_input", &NewGame::_unhandled_input);
	}

	NewGame::NewGame()
	{
		_impl = NewGameImpl::_new();
	}

	NewGame::~NewGame()
	{
		NewGameImpl* tmp = self(_impl);
		_impl = nullptr;
		godot::api->godot_free(_impl);
	}

	void NewGame::_draw()
	{
		self(_impl)->draw();
	}
	
	void NewGame::_init()
	{
		self(_impl)->_init();
	}

	void NewGame::_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->input(ev);
	}
	
	void NewGame::_physics_process(const float delta)
	{
		self(_impl)->physics_process(delta);
	}
	
	void NewGame::_process(const float delta)
	{
		self(_impl)->process(delta);
	}
	
	void NewGame::_ready()
	{
		self(_impl)->ready();
	}
	
	void NewGame::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->unhandled_input(ev);
	}
}
