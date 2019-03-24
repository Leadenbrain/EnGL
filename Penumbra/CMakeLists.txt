cmake_minimum_required(VERSION 2.8.9)
project(Penumbra)

set ( PROJECT_LINK_LIBS "libEnGL.dll")
link_directories( ../EnGL/build )
link_directories("C:\\MinGW\\mingw64\\lib")
link_libraries(opengl32)
link_libraries(glfw3dll)
link_libraries(libglad.a)

# Bring in Headers
file(GLOB INCLUDES "../EnGL/src/EnGL/eglpch.h ../EnGL/src/*.h ../EnGL/res/spdlog/include/spdlog/*.h")
include_directories(include ../EnGL/src ../EnGL/res/include)

SET(PCH "../EnGL/src/EnGL/eglpch.cpp")
file(GLOB SOURCES "src/*.cpp")
file(GLOB RESOURCES "res/*.cpp")

if (WIN32)
	add_definitions(-DEGL_PLATFORM_WINDOWS)
endif (WIN32)

if (LINUX)
	add_definitions(-DEGL_PLATFORM_LINUX)
endif (LINUX)

add_executable(Penumbra ${PCH} ${SOURCES} ${RESOUCES} ${INCLUDES})
target_link_libraries(Penumbra ${PROJECT_LINK_LIBS})