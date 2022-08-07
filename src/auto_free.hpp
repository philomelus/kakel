#ifndef AUTO_FREE_HPP
#define AUTO_FREE_HPP

namespace godot
{
    // Guaranteed memory release
	template <typename T>
	class auto_free
	{
	public:
		auto_free(T* v) { _v = v; }
		~auto_free() { godot::api->godot_free(_v); }
		T* operator->() { return _v; }

	private:
		T* _v;
	};
}

#endif	// AUTO_FREE_HPP
