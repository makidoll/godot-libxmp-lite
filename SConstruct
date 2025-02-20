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

# build libxmp
'''
mkdir -p libxmp/build libxmp/install
cmake libxmp -Blibxmp/build -GNinja -DBUILD_LITE=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=libxmp/install
cmake --build libxmp/build
cmake --install libxmp/build
'''

env.Append(CPPPATH=["src/", "libxmp/install/include"])

sources = Glob("src/*.cpp")
sources.append("libxmp/install/lib/libxmp-lite.a")

env["STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME"] = 1

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
# run: scons compiledb

env.Tool("compilation_db")
