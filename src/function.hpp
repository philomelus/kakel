#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <godot_cpp/variant/utility_functions.hpp>

#define FUNC_(name) FUNCTION ___func___(name)
#define FUNCQ_(name) FUNCTION ___func___(name, true)
#define FUNCV_(name) FUNCTION ___func___(name, false)
#define FUNCP_ ___func___.print
#define FUNCPF_ ___func___.fprint
#define FUNCQ() ___func___.quiet()
#define FUNCV() ___func___.verbose()

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
				UtilityFunctions::print(_name, ": --> enter");
		}
		~FUNCTION()
		{
			if (!_quiet)
				UtilityFunctions::print(_name, ": <-- exit");
		}

		template <typename... Args>
		void fprint(const Args&... args) { UtilityFunctions::print(_name, ": ", args...); }

		template <typename... Args>
		void print(const Args&... args) { if (!_quiet) UtilityFunctions::print(_name, ": ", args...); }

		void quiet() { _quiet = true; }
		
		void verbose() { _quiet = false; }
		
	private:
		bool _quiet;
		const char* const _name;
	};
}

#endif	// FUNCTION_HPP
