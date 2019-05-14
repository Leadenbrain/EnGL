# EnGL
OpenGL Engine

# Building EnGL

1) In EnGL/EnGL/ make a build directory (example: mkdir build)

2) Move into directory, and compile CMakeLists.txt from EnGL/EnGL (example: cd build; cmake ..)

Note: I used MinGW from cmd on Windows, this required me to tell cmake to use the MinGW generator files
(example: cmake -G "MinGW Makefiles" ..)

3) Use the make file to install the shared library (.dll) into program directory and C:\MinGW (mingw32-make install)

# Compiling Program

1) In EnGL/Penumbra/ make a build directory (example: mkdir build)

2) Move into directory, and compile CMakeLists.txt from EnGL/Penumbra (example: cd build; cmake ..)

3) Use the makefile to compile the program, this requires the EnGL .dll to be in (one of) the include directories folder
(example: mingw32-make)
