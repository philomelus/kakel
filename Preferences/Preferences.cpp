#include "Preferences.hpp"
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class PreferencesImpl : public kakel::Preferences
	{
		friend kakel::Preferences;
		
	protected:
		SceneTree* _tree;

	public:
		PreferencesImpl()
		{
		}

		~PreferencesImpl()
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
	
	inline PreferencesImpl* self(kakel::Preferences* obj) { return reinterpret_cast<PreferencesImpl*>(obj); }
}


namespace kakel
{
	void Preferences::_register_methods()
	{
		// API
		register_method("_draw", &Preferences::_draw);
		register_method("_init", &Preferences::_init);
		register_method("_input", &Preferences::_input);
		register_method("_physics_process", &Preferences::_physics_process);
		register_method("_process", &Preferences::_process);
		register_method("_ready", &Preferences::_ready);
		register_method("_unhandled_input", &Preferences::_unhandled_input);
	}

	Preferences::Preferences()
	{
		_impl = PreferencesImpl::_new();
	}

	Preferences::~Preferences()
	{
		PreferencesImpl* tmp = _impl;
		_impl = nullptr;
		godot::api->godot_free(_impl);
	}

	void Preferences::_draw()
	{
		self(_impl)->draw();
	}
	
	void Preferences::_init()
	{
		self(_impl)->_init();
	}

	void Preferences::_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->input(ev);
	}
	
	void Preferences::_physics_process(const float delta)
	{
		self(_impl)->physics_process(delta);
	}
	
	void Preferences::_process(const float delta)
	{
		self(_impl)->process(delta);
	}
	
	void Preferences::_ready()
	{
		self(_impl)->ready();
	}
	
	void Preferences::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	{
		self(_impl)->unhandled_input(ev);
	}
}
