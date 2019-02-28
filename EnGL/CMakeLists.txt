cmake_minimum_required(VERSION 2.8.9)
project(EnGL)

# Bring in Headers
include_directories(include)

file(GLOB SOURCES "src/*/*.cpp")
file(GLOB RESOURCES "res/*.cpp")

if (WIN32)
	add_definitions(-DEGL_PLATFORM_WINDOWS)
	add_definitions(-DEGL_BUILD_DLL)
endif (WIN32)

# Haven't tested, is CMAKE_SYSTEM_NAME better here?
if (UNIX)
	add_definitions(-DEGL_PLATFORM_LINUX)
endif (UNIX)

add_library (EnGL SHARED ${SOURCES} ${RESOUCES})

install(TARGETS EnGL DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Penumbra/build)