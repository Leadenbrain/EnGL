cmake_minimum_required(VERSION 2.8.9)
project(EnGL)

# Bring in Headers
include_directories(include "res/include/" )
SET(INCLUDES "src/EnGL/eglpch.h")
file(GLOB INCLUDES_ADD "res/spdlog/include/spdlog/*.h include/*/*.h")
LIST(APPEND ${INCLUDES} ${INCLUDES_ADD})

#Libraries
link_directories("C:\\MinGW\\mingw64\\lib")
link_libraries(opengl32)
link_libraries(glfw3dll)
link_libraries(libglad.a)

SET(PCH "src/EnGL/eglpch.cpp")
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

add_library (EnGL SHARED ${PCH} ${SOURCES} ${RESOUCES} ${INCLUDES})
#add_library (EnGL STATIC ${PCH} ${SOURCES} ${RESOUCES} ${INCLUDES})

install(TARGETS EnGL DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Penumbra/build)
install(TARGETS EnGL DESTINATION "C:\\MinGW\\mingw64\\lib")