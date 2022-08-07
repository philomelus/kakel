#include "Main.hpp"
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <FileDialog.hpp>
#include <GlobalConstants.hpp>
#include <Label.hpp>
#include <SceneTree.hpp>
#include <VBoxContainer.hpp>

using namespace godot;

namespace godot
{
	void Main::_register_methods()
	{
		// API
		register_method("_ready", &Main::_ready);
		register_method("on_new", &Main::on_new);
		register_method("on_load", &Main::on_load);
		register_method("on_prefs", &Main::on_prefs);
		register_method("on_quit", &Main::on_quit);
		register_method("on_loadDialog_fileSelected", &Main::on_loadDialog_fileSelected);
	}

	Main::Main()
	{
	}

	Main::~Main()
	{
	}

	void Main::_init()
	{
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

	void Main::_ready()
	{
		_tree = get_tree();

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
			
		// Add center contianer
		add_child(_centerContainer);
		_centerContainer->set_owner(this);
		_centerContainer->set_margin(GlobalConstants::MARGIN_LEFT, 7);
		_centerContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 1273);
		_centerContainer->set_margin(GlobalConstants::MARGIN_TOP, 7);
		_centerContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 713);
			
		// Add v box container
		_centerContainer->add_child(_vboxContainer);
		_vboxContainer->set_owner(_centerContainer);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_LEFT, 561);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 704);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_TOP, 209);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 496);

		// Add label
		_vboxContainer->add_child(_label);
		_label->set_owner(_vboxContainer);
		_label->set_text("KAKEL");
		_label->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_label->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_label->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_label->set_margin(GlobalConstants::MARGIN_BOTTOM, 67);
			
		// Add new button
		_vboxContainer->add_child(_start);
		_start->set_owner(_vboxContainer);
		_start->set_margin(GlobalConstants::MARGIN_TOP, 71);
		_start->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_start->set_margin(GlobalConstants::MARGIN_BOTTOM, 122);
		_start->set_tooltip("Start a new game.");
		_start->set_text("New");
			
		// Add load button
		_vboxContainer->add_child(_load);
		_load->set_owner(_vboxContainer);
		_load->set_margin(GlobalConstants::MARGIN_TOP, 126);
		_load->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_load->set_margin(GlobalConstants::MARGIN_BOTTOM, 177);
		_load->set_tooltip("Load a previously saved game.");
		_load->set_text("Load");
			
		// Add preferences button
		_vboxContainer->add_child(_prefs);
		_prefs->set_owner(_vboxContainer);
		_prefs->set_margin(GlobalConstants::MARGIN_TOP, 181);
		_prefs->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_prefs->set_margin(GlobalConstants::MARGIN_BOTTOM, 232);
		_prefs->set_tooltip("Change game settings.");
		_prefs->set_text("Options");
			
		// Add quit button
		_vboxContainer->add_child(_quit);
		_quit->set_owner(_vboxContainer);
		_prefs->set_margin(GlobalConstants::MARGIN_TOP, 236);
		_prefs->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_prefs->set_margin(GlobalConstants::MARGIN_BOTTOM, 287);
		_prefs->set_tooltip("Exit the game.");
		_quit->set_text("Quit");
			
		// Add load dialog
		add_child(_loadDialog);
		_loadDialog->set_owner(_vboxContainer);
		_loadDialog->set_pause_mode(Node::PauseMode::PAUSE_MODE_PROCESS);
		_loadDialog->set_margin(GlobalConstants::MARGIN_RIGHT, 316);
		_loadDialog->set_margin(GlobalConstants::MARGIN_BOTTOM, 149);
		_loadDialog->set_custom_minimum_size(Vector2(480, 320));
		_loadDialog->set_exclusive(true);
		_loadDialog->set_title("Open A Game");
		_loadDialog->set_resizable(true);
		_loadDialog->set_mode_overrides_title(false);
		_loadDialog->set_mode(FileDialog::Mode::MODE_OPEN_FILE);
		_loadDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
		PoolStringArray filters;
		filters.append("*.kakel;Kakel Games");
		_loadDialog->set_filters(filters);
		
		// Connect signals
		_start->connect("pressed", this, "on_new");
		_load->connect("pressed", this, "on_load");
		_prefs->connect("pressed", this, "on_prefs");
		_quit->connect("pressed", this, "on_quit");
		_loadDialog->connect("file_selected", this, "on_loadDialog_fileSelected");
	}

	void Main::on_load()
	{
		godot::Godot::print(String("Main::on_load called."));
	}
	
	void Main::on_loadDialog_fileSelected(const String path)
	{
		godot::Godot::print(String("Main::on_loadDialog_fileSelected called."));
	}
	
	void Main::on_new()
	{
		godot::Godot::print(String("Main::on_new called."));
	}
	
	void Main::on_prefs()
	{
		godot::Godot::print(String("Main::on_prefs called."));
	}
	
	void Main::on_quit()
	{
		godot::Godot::print(String("Main::on_quit called."));
	}
}
