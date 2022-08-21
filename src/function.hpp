#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#define FUNC_(name) FUNCTION ___func___(name)
#define FUNCQ_(name) FUNCTION ___func___(name, true)
#define FUNCV_(name) FUNCTION ___func___(name, false)
#define FUNCP_ Godot::print
#define FUNCPF_ Godot::print
#define FUNCQ() ___func___.quiet()
#define FUNCV() ___func___.verbose()

namespace godot
{
	class FUNCTION
	{
	public:
		FUNCTION(const char* func, bool quiet=false)
			: _name(func),
			  _quiet(quiet)
		{
			if (!_quiet)
				Godot::print("{0}: --> enter", _name);
		}
		~FUNCTION()
		{
			if (!_quiet)
				Godot::print("{0}: <-- exit", _name);
		}

		void quiet() { _quiet = true; }
		
		void verbose() { _quiet = false; }
		
	private:
		bool _quiet;
		const char* const _name;
	};
}

#endif	// FUNCTION_HPP
