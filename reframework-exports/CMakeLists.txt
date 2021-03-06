# This file is automatically generated from cmake.toml - DO NOT EDIT
# See https://github.com/build-cpp/cmkr for more information

cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-tree builds are not supported. Run CMake from a separate directory: cmake -B build")
endif()

# Regenerate CMakeLists.txt automatically in the root project
set(CMKR_ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(CMKR_ROOT_PROJECT ON)

	# Bootstrap cmkr
	include(cmkr.cmake OPTIONAL RESULT_VARIABLE CMKR_INCLUDE_RESULT)
	if(CMKR_INCLUDE_RESULT)
		cmkr()
	endif()

	# Enable folder support
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Create a configure-time dependency on cmake.toml to improve IDE support
if(CMKR_ROOT_PROJECT)
	configure_file(cmake.toml cmake.toml COPYONLY)
endif()

project(reframework-exports)

# Target reframework
set(CMKR_TARGET reframework)
set(reframework_SOURCES "")

set(CMKR_SOURCES ${reframework_SOURCES})
add_library(reframework INTERFACE)

if(reframework_SOURCES)
	target_sources(reframework INTERFACE ${reframework_SOURCES})
endif()

target_include_directories(reframework INTERFACE
	"deps/reframework/include"
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

# Target reframework-not-a-plugin
set(CMKR_TARGET reframework-not-a-plugin)
set(reframework-not-a-plugin_SOURCES "")

list(APPEND reframework-not-a-plugin_SOURCES
	"example/NotAPlugin.cpp"
)

list(APPEND reframework-not-a-plugin_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${reframework-not-a-plugin_SOURCES})
add_library(reframework-not-a-plugin SHARED)

if(reframework-not-a-plugin_SOURCES)
	target_sources(reframework-not-a-plugin PRIVATE ${reframework-not-a-plugin_SOURCES})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${reframework-not-a-plugin_SOURCES})

target_compile_features(reframework-not-a-plugin PUBLIC
	cxx_std_20
)

target_link_libraries(reframework-not-a-plugin PUBLIC
	reframework
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

# Target reframework-exports
set(CMKR_TARGET reframework-exports)
set(reframework-exports_SOURCES "")

list(APPEND reframework-exports_SOURCES
	"src/Main.cpp"
)

list(APPEND reframework-exports_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${reframework-exports_SOURCES})
add_library(reframework-exports SHARED)

if(reframework-exports_SOURCES)
	target_sources(reframework-exports PRIVATE ${reframework-exports_SOURCES})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${reframework-exports_SOURCES})

target_compile_features(reframework-exports PUBLIC
	cxx_std_20
)

target_link_libraries(reframework-exports PUBLIC
	reframework
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

install(
	TARGETS
		reframework-exports
		reframework-not-a-plugin
	DESTINATION
		bin
	COMPONENT
		reframework-exports
)
