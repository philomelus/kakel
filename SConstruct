#!/usr/bin/env python

# User modifiable settings
godot_cpp_path = "godot-cpp"
godot_cpp_library = "libgodot-cpp"

# >>> Change nothing below here <<<

env = DefaultEnvironment()
opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux']))
opts.Add(EnumVariable('target', "Compilation target", '', ['', 'debug', 'release', 'release_debug']))
opts.Add(BoolVariable('use_mingw', 'Use MINGW_W64 on Windows or Linux', False))
opts.Add(BoolVariable('verbose', 'Enable verbose output for the compilation', False))
opts.Update(env)

# Make sure required options provided
if env['platform'] == '':
    print("No platform specififed: platform={windows,x11,linux}")
    quit();
if env['target'] == '':
    print("No target specififed: target={release,release_debug,debug}")
    quit();

# Setup for desired environment
out = 'bin/kakel'
out_ext = ''
if env['platform'] == 'windows':
    godot_cpp_library += '.windows'
    out += '.windows'
    out_ext = '.dll'
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
    godot_cpp_library += '.linux'
    out += '.linux'
    out_ext = '.so'
    print("LINUX/X11 support not implemented in SConstruct!")
    quit()

# Update actual name of godot library
if env['target'] == 'debug':
    godot_cpp_library += '.debug.x86_64'
    out += '.debug.x86_64'
else:
    godot_cpp_library += '.release.x86_64'
    out += '.release.x86_64'

# Add locations of godot_cpp and godot_headers
env.Append(CPPPATH=(godot_cpp_path + '/godot-headers',
                    godot_cpp_path + '/include',
                    godot_cpp_path + '/gen/include',))
env.Append(LIBPATH=(godot_cpp_path + '/bin',))
env.Append(LIBS=(godot_cpp_library,))

# Build
env.SharedLibrary(out + out_ext, ('src/Game.cpp',
                                  'src/register.cpp',
                                  'src/KakelPreferences.cpp',
                                  'src/KakelGlobals.cpp',
                                  'src/Main.cpp',
                                  'src/NewGame.cpp',
                                  'src/Prefs.cpp',
                                  'src/TilesControl.cpp'))

