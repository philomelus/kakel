#ifndef FUNCTION_HPP
#define FUNCTION_HPP

namespace godot
{
	class FUNCTION
	{
	public:
		FUNCTION(const char* func, bool quiet=true)
			: _name(func),
			  _quiet(quiet)
		{
			if (!_quiet)
				godot::Godot::print("{0}: --> enter", _name);
		}
		~FUNCTION()
		{
			if (!_quiet)
				godot::Godot::print("{0}: <-- exit", _name);
		}

	private:
		const bool _quiet;
		const char* const _name;
	};

#ifndef FUNC_
#define FUNC_(name) FUNCTION ___func(name);
#define FUNCQ_(name) FUNCTION ___func(name, true)
#endif
}

#endif	// FUNCTION_HPP
