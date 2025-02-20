#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/", "/home/maki/git/libxmp/install/include"])

# env.Append(LIBS=["libxmp-lite.a"])
# env.Append(LIBPATH=["/home/maki/git/libxmp/install/lib"])

sources = Glob("src/*.cpp")

sources.append("/home/maki/git/libxmp/install/lib/libxmp-lite.a")

env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libxmp-lite.{}.{}.framework/libxmp-lite.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "demo/bin/libxmp-lite.{}.{}.simulator.a".format(
                env["platform"], env["target"]
            ),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "demo/bin/libxmp-lite.{}.{}.a".format(
                env["platform"], env["target"]
            ),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "demo/bin/libxmp-lite{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)

# compile_commands.json for vscode

env.Tool("compilation_db")
