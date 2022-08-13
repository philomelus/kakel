#ifndef AUTO_FREE_HPP
#define AUTO_FREE_HPP

#include <godot_cpp/core/memory.hpp>

namespace godot
{
    // Guaranteed memory release
	template <typename T>
	class auto_free
	{
	public:
		auto_free(T* v) { _v = v; }
		~auto_free() { memdelete(_v); }
		T* operator->() { return _v; }

	private:
		T* _v;
	};
}

#endif	// AUTO_FREE_HPP
