#include "Main.hpp"
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <FileDialog.hpp>
#include <GlobalConstants.hpp>
#include <Label.hpp>
#include <SceneTree.hpp>
#include <VBoxContainer.hpp>

using namespace godot;

namespace {
	class MainImpl : public kakel::Main
	{
		friend kakel::Main;
		
	protected:
		CenterContainer* _centerContainer;
		VBoxContainer* _vboxContainer;
		Label* _label;
		Button* _load;
		FileDialog* _loadDialog;
		Button* _prefs;
		Button* _quit;
		Button* _start;
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
			godot::Godot::print(String("MainImpl::init called."));
			
			// Create controls
			_centerContainer = CenterContainer::_new();
			_vboxContainer = VBoxContainer::_new();
			_label = Label::_new();
			_load = Button::_new();
			_loadDialog = FileDialog::_new();
			_prefs = Button::_new();
			_quit = Button::_new();
			_start = Button::_new();
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
			// _tree = get_tree();
			String msg("MainImpl::ready called.");
			godot::Godot::print(msg);
			
			// Set our settings
			set_anchors_preset(Control::LayoutPreset::PRESET_WIDE);
			set_margin(GlobalConstants::MARGIN_LEFT, 0);
			set_margin(GlobalConstants::MARGIN_RIGHT, 0);
			set_margin(GlobalConstants::MARGIN_TOP, 0);
			set_margin(GlobalConstants::MARGIN_BOTTOM, 0);
			set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
			set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
			set_h_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
			set_v_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
			show();
			
			// Add center contianer
			add_child(_centerContainer);
			_centerContainer->set_margin(GlobalConstants::MARGIN_LEFT, 7);
			_centerContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 1273);
			_centerContainer->set_margin(GlobalConstants::MARGIN_TOP, 7);
			_centerContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 713);
			_centerContainer->show();
			
			// Add v box container
			_centerContainer->add_child(_vboxContainer);
			_vboxContainer->set_margin(GlobalConstants::MARGIN_LEFT, 561);
			_vboxContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 704);
			_vboxContainer->set_margin(GlobalConstants::MARGIN_TOP, 209);
			_vboxContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 496);
			_vboxContainer->show();

			// Add label
			_vboxContainer->add_child(_label);
			_label->set_text("KAKEL");
			_label->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
			_label->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
			_label->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
			_label->set_margin(GlobalConstants::MARGIN_BOTTOM, 67);
			_label->show();
			
			// Add new button
			_vboxContainer->add_child(_start);
			_start->set_text("New");
			
			// Add load button
			_vboxContainer->add_child(_load);
			_load->set_text("Load");
			
			// Add preferences button
			_vboxContainer->add_child(_prefs);
			_prefs->set_text("Options");
			
			// Add quit button
			_vboxContainer->add_child(_quit);
			_quit->set_text("Quit");
			
			// Add load dialog
			add_child(_loadDialog);
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
		// register_method("_draw", &Main::_draw);
		register_method("_init", &Main::_init);
		// register_method("_input", &Main::_input);
		// register_method("_physics_process", &Main::_physics_process);
		// register_method("_process", &Main::_process);
		// register_method("_ready", &Main::_ready);
		// register_method("_unhandled_input", &Main::_unhandled_input);
	}

	Main::Main()
	{
		_impl = MainImpl::_new();
	}

	Main::~Main()
	{
		MainImpl* tmp = self(_impl);
		_impl = nullptr;
		godot::api->godot_free(_impl);
	}

	// void Main::_draw()
	// {
	// 	PanelContainer::_draw();
	// 	self(_impl)->draw();
	// }
	
	void Main::_init()
	{
		CRASH_NOW();
		godot::Godot::print(String("Main::_init called."));

		PanelContainer::_init();
		self(_impl)->_init();
	}

	// void Main::_input(const godot::Ref<godot::InputEvent> ev)
	// {
	// 	self(_impl)->input(ev);
	// }
	
	// void Main::_physics_process(const float delta)
	// {
	// 	self(_impl)->physics_process(delta);
	// }
	
	// void Main::_process(const float delta)
	// {
	// 	self(_impl)->process(delta);
	// }
	
	// void Main::_ready()
	// {
	// 	PanelContainer::_ready();
	// 	self(_impl)->ready();
	// }
	
	// void Main::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	// {
	// 	self(_impl)->unhandled_input(ev);
	// }
}
