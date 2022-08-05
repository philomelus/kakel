#include "Main.hpp"
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class MainImpl : public kakel::Main
	{
		friend kakel::Main;
		
	protected:
		SceneTree* _tree;

	public:
		MainImpl()
		{
		}

		~MainImpl()
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
	
	inline MainImpl* self(kakel::Main* obj) { return reinterpret_cast<MainImpl*>(obj); }
}


namespace kakel
{
	void Main::_register_methods()
	{
		// API
		register_method("_draw", &Main::_draw);
		register_method("_init", &Main::_init);
		register_method("_input", &Main::_input);
		register_method("_physics_process", &Main::_physics_process);
		register_method("_process", &Main::_process);
		register_method("_ready", &Main::_ready);
		register_method("_unhandled_input", &Main::_unhandled_input);
	}

	Main::Main()
	{
		_impl = MainImpl::_new();
	}

	Main::~Main()
	{
		MainImpl* tmp = _impl;
		_impl = nullptr;
		godot::api->godot_free(_impl);
	}

	void Main::_draw()
	{
		self(_impl)->draw();
	}
	
	void Main::_init()
	{
		self(_impl)->_init();
	}

	void Main::_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->input(ev);
	}
	
	void Main::_physics_process(const float delta)
	{
		self(_impl)->physics_process(delta);
	}
	
	void Main::_process(const float delta)
	{
		self(_impl)->process(delta);
	}
	
	void Main::_ready()
	{
		self(_impl)->ready();
	}
	
	void Main::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->unhandled_input(ev);
	}
}
