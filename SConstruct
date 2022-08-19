#!/usr/bin/env python

# User modifiable settings
godot_headers_path = "godot-cpp/godot-headers/"
cpp_bindings_path = "godot-cpp/"
cpp_library = "libgodot-cpp"

# >>> Change nothing below here <<<

env = DefaultEnvironment()
opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable('bits', "Bit architecture on platform", '', ['', '32', '64']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux']))
opts.Add(EnumVariable('target', "Compilation target", '', ['', 'debug', 'release', 'release_debug']))
opts.Add(BoolVariable('use_mingw', 'Use MINGW_W64 on Windows or Linux', False))
opts.Add(BoolVariable('verbose', 'Enable verbose output for the compilation', False))
opts.Update(env)

# Make sure required options provided
if env['bits'] == '':
    print("No bits specififed: bits={32,64}")
    quit();
if env['platform'] == '':
    print("No platform specififed: platform={windows,x11,linux}")
    quit();
if env['target'] == '':
    print("No target specififed: target={release,release_debug,debug}")
    quit();

# Setup for desired environment
if env['platform'] == 'windows':
    cpp_library += '.windows'
    if env['use_mingw']:
        env.Tool('mingw')
        env.Append(CCFLAGS=['-std=c++17', '-Wwrite-strings'])
        env.Append(LINKFLAGS=['--static', '-Wl,--no-undefined', '-static-libgcc', '-static-libstdc++'])
        if env['target'] == 'debug':
            env.Append(CCFLAGS=['-g3', '-Og'])
        elif env['target'] == ['release', 'release_debug']:
            env.Append(CCFLAGS=['-g', '-O3'])
    else:
        env.Append(CPPDEFINES=['WIN32', '_WIN32', '_WINDOWS', '_CRT_SECURE_NO_WARNINGS'])
        env.Append(CCFLAGS=['-W3', '-GR', '-EHsc'])
        if env['target'] == 'debug':
            env.Append(CPPDEFINES=['_DEBUG'])
            env.Append(CCFLAGS=['-MDd', '-ZI'])
            env.Append(LINKFLAGS=['-DEBUG'])
        elif env['target'] in ['release', 'release_debug']:
            env.Append(CPPDEFINES=['NDEBUG'])
            env.Append(CCFLAGS=['-O2', '-MD'])
elif env['platform'] in ('x11', 'linux'):
    cpp_library += '.linux'
    print("LINUX/X11 support not implemented in SConstruct!")
    quit()

# Update actual name of godot library
if env['target'] == 'debug':
    cpp_library += '.debug'
else:
    cpp_library += '.release'
cpp_library += '.' + env['bits']

# Add locations of godot_cpp and godot_headers
env.Append(CPPPATH=[godot_headers_path,
                    cpp_bindings_path + 'include',
                    cpp_bindings_path + 'include/core',
                    cpp_bindings_path + 'include/gen'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin'])
env.Append(LIBS=[cpp_library])

# Build
env.SharedLibrary('bin/kakel', ('src/AppGlobals.cpp',
                                'src/AppPreferences.cpp',
                                'src/Game.cpp',
                                'src/register.cpp',
                                'src/Main.cpp',
                                'src/NewGame.cpp',
                                'src/Prefs.cpp',
                                'src/TilesControl.cpp'))

