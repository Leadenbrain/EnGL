cmake_minimum_required(VERSION 2.8.9)
project(EnGL)

# Bring in Headers
include_directories(include "res/include/" "src/")
SET(INCLUDES "src/EnGL/eglpch.h")
file(GLOB INCLUDES_ADD "res/spdlog/include/spdlog/*.h res/*/*.h src/*/*.h src/*/*/*.h src/Platform/*/*.h src/EnGL/ImGui/*.h")
LIST(APPEND ${INCLUDES} ${INCLUDES_ADD})

#Libraries

SET(PCH "src/EnGL/eglpch.cpp")
file(GLOB SOURCES "src/*/*.cpp")
file(GLOB PLATFORM "src/*/*/*.cpp")
file(GLOB RESOURCES "res/*.cpp")
file(GLOB IMGUI "res/imgui/*.cpp")

if (WIN32)
	link_directories("C:\\MinGW\\mingw64\\lib")
	add_definitions(-DEGL_PLATFORM_WINDOWS)
	add_definitions(-DEGL_BUILD_DLL)
	link_libraries(glfw3dll)
endif (WIN32)

link_libraries(opengl32)
link_libraries(freeimage)
link_libraries(libglad.a)

# Haven't tested, is CMAKE_SYSTEM_NAME better here?
if (UNIX)
	add_definitions(-DEGL_PLATFORM_LINUX)
	link_libraries(glfw3dll)
endif (UNIX)

SET(CMAKE_CXX_STANDARD 17)

add_library (EnGL SHARED ${PCH} ${SOURCES} ${PLATFORM} ${RESOUCES} ${IMGUI} ${INCLUDES})
#add_library (EnGL STATIC ${PCH} ${SOURCES} ${RESOUCES} ${INCLUDES})

install(TARGETS EnGL DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../Penumbra/build)
#install(TARGETS EnGL DESTINATION "C:\\MinGW\\mingw64\\lib")