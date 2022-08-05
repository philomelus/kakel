#include "Game.hpp"
#include "Main.hpp"
#include "NewGame.hpp"
#include "Preferences.hpp"
#include "Prefs.hpp"
#include "TilesControl.hpp"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
    godot::Godot::nativescript_init(handle);

    godot::register_class<kakel::Game>();
    godot::register_class<kakel::Main>();
    godot::register_class<kakel::NewGame>();
    godot::register_class<kakel::Preferences>();
    godot::register_class<kakel::Prefs>();
    godot::register_class<kakel::TilesControl>();
}
