Ray Tracing In A Weekend Playthrough
====================================

Working through the Ray Tracing In A Weekend series.

Below are the commands to setup the build environment:
```
conan install . --build=missing

cd build/Debug

source ./generators/conanbuild.sh

cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
```

And the command to build:
```
cmake --build .
```

ToDos
-----

 * Make the conanfile.py require C++20
