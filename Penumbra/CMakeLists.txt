cmake_minimum_required(VERSION 2.8.9)
project(Penumbra)

set ( PROJECT_LINK_LIBS libEnGL.dll.a)
set ( PROJECT_LINK_LIBS libEnGL.dll)
link_directories( ../EnGL/build )

# Bring in Headers
include_directories(include ../EnGL/src ../EnGL/res/spdlog/include)
file(GLOB INCLUDES "../EnGL/src/*.h")

file(GLOB SOURCES "src/*.cpp")
file(GLOB RESOURCES "res/*.cpp")

if (WIN32)
	add_definitions(-DEGL_PLATFORM_WINDOWS)
endif (WIN32)

if (LINUX)
	add_definitions(-DEGL_PLATFORM_LINUX)
endif (LINUX)

add_executable(Penumbra ${SOURCES} ${RESOUCES} ${INCLUDES})
target_link_libraries(Penumbra ${PROJECT_LINK_LIBS})