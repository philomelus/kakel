#include "Prefs.hpp"
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class PrefsImpl : public kakel::Prefs
	{
		friend kakel::Prefs;
		
	protected:
		SceneTree* _tree;

	public:
		PrefsImpl()
		{
		}

		~PrefsImpl()
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
	
	inline PrefsImpl* self(kakel::Prefs* obj) { return reinterpret_cast<PrefsImpl*>(obj); }
}


namespace kakel
{
	void Prefs::_register_methods()
	{
		// API
		register_method("_draw", &Prefs::_draw);
		register_method("_init", &Prefs::_init);
		register_method("_input", &Prefs::_input);
		register_method("_physics_process", &Prefs::_physics_process);
		register_method("_process", &Prefs::_process);
		register_method("_ready", &Prefs::_ready);
		register_method("_unhandled_input", &Prefs::_unhandled_input);
	}

	Prefs::Prefs()
	{
		_impl = PrefsImpl::_new();
	}

	Prefs::~Prefs()
	{
		PrefsImpl* tmp = _impl;
		_impl = nullptr;
		godot::api->godot_free(_impl);
	}

	void Prefs::_draw()
	{
		self(_impl)->draw();
	}
	
	void Prefs::_init()
	{
		self(_impl)->_init();
	}

	void Prefs::_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->input(ev);
	}
	
	void Prefs::_physics_process(const float delta)
	{
		self(_impl)->physics_process(delta);
	}
	
	void Prefs::_process(const float delta)
	{
		self(_impl)->process(delta);
	}
	
	void Prefs::_ready()
	{
		self(_impl)->ready();
	}
	
	void Prefs::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->unhandled_input(ev);
	}
}
