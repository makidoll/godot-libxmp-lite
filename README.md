# Godot libxmp-lite

Still very work in progress. Plan to make a AudioStreamXMP to use in audio players.

## Build

```bash
mkdir -p libxmp/build libxmp/install
cmake libxmp -Blibxmp/build -GNinja -DBUILD_LITE=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=libxmp/install
cmake --build libxmp/build
cmake --install libxmp/build
```

```bash
scons target=linux
```
