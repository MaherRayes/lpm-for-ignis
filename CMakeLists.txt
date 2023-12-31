cmake_minimum_required(VERSION 3.9...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Prevent in source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	if(NOT DEFINED WITH_IN_SOURCE_BUILD)
		message(FATAL_ERROR "CMake generation for Ignis is not allowed within the source directory! Define WITH_IN_SOURCE_BUILD if absolutely necessary!" )
	endif()
endif()

# Omit superfluous "Up-to-date" messages.
if(NOT DEFINED CMAKE_INSTALL_MESSAGE)
	set(CMAKE_INSTALL_MESSAGE "LAZY")
endif()

# Set default to Release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

project(Ignis
	VERSION 0.3
	DESCRIPTION "Experimental ray tracer")

# For whatever reason Ignis might be used as a subproject...
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(IG_SUBPROJECT OFF)
else()
	set(IG_SUBPROJECT ON)
endif()

# Set corresponding output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_DEBUG_POSTFIX  "_d")

set(Ignis_VENDOR "Ignis project 2020-2022")
set(Ignis_DESCRIPTION "Device agnostic raytracing framework with shared codebase for offline and realtime rendering and for CPU and GPU")
set(Ignis_URL "https://github.com/PearCoding/Ignis")
include(Git)

message(STATUS "Building Ignis Rodent ${Ignis_VERSION}")

if(NOT IG_SUBPROJECT)
    include(CTest)
endif()

# Options
option(IG_WITH_CLI           "Build commandline interface igcli" ON)
option(IG_WITH_VIEWER        "Build interactive viewer igview" ON)
option(IG_WITH_TRACER        "Build tracing frontend igtrace" ON)
option(IG_WITH_PYTHON_API    "Build python API" ON)
option(IG_WITH_TOOLS         "Build tools" ON)
option(IG_WITH_DOCUMENTATION "Build the documentation if Sphinx is available on the system" ON)
option(IG_WITH_ASSERTS       "Build with asserts even in release. It is always enabled on debug" OFF)
option(IG_WITH_DENOISER      "Integrate Intel Open Image Denoise if available" ON)
option(IG_USE_LTO 			 "Use linked time optimization if available in release" ON)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.21)
	if(WIN32 OR (NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT))
		set(_default ON)
	else()
		set(_default OFF)
	endif()
	option(IG_INSTALL_RUNTIME_DEPENDENCIES "Install runtime libraries together with Ignis" ${_default})
endif()

if(SKBUILD)
	find_package(PythonExtensions REQUIRED)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	# set(CMAKE_SKIP_BUILD_RPATH FALSE)
	# set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
	set(_IG_RUNTIME_SET RUNTIME_DEPENDENCY_SET ignis_runtime_set)
    message(STATUS "Using scikit-build, rpath=${CMAKE_INSTALL_RPATH}")
elseif(IG_INSTALL_RUNTIME_DEPENDENCIES)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	set(_IG_RUNTIME_SET RUNTIME_DEPENDENCY_SET ignis_runtime_set)
endif()

# Setup artic clang flags
if(APPLE)
	set(IG_ARTIC_CLANG_FLAGS_DEFAULT -ffast-math)
else()
	set(IG_ARTIC_CLANG_FLAGS_DEFAULT -march=native -ffast-math)
endif()
set(IG_ARTIC_CLANG_FLAGS ${IG_ARTIC_CLANG_FLAGS_DEFAULT} CACHE STRING "Additional artic clang compilation options")
set(IG_ARTIC_FLAGS CACHE STRING "Additional artic compilation options")
mark_as_advanced(IG_ARTIC_CLANG_FLAGS IG_ARTIC_FLAGS)

# Setup CPM
include(cmake/LTO.cmake)
include(cmake/SetupCPM.cmake)

# Locate stuff for windows
# On Linux this is properly propagated
# So it needs a fixing in AnyDSL
if(WIN32)
	find_package(CUDAToolkit)
	find_package(OpenCL)
endif()

# Locate AnyDSL runtime
find_package(AnyDSL_runtime REQUIRED)

# Make sure the filesystem library is available
find_package(Filesystem REQUIRED)

# Locate Eigen
find_package(Eigen3 REQUIRED NO_MODULE)

# Locate tbb
find_package(TBB REQUIRED) 

# ZLib is used in the generator
find_package(ZLIB REQUIRED)

if(IG_WITH_DENOISER)
	find_package(OpenImageDenoise)
endif()

include(cmake/GetDependencies.cmake)
include(cmake/Install.cmake)
include(cmake/Bash.cmake)
include(cmake/ExtraUtils.cmake)

# Enable folders for Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Make sure we use march native
option(IG_OPTIMIZE_FOR_NATIVE "Build with -march=native if possible" ON)
include(CheckCXXCompilerFlag)
if(IG_OPTIMIZE_FOR_NATIVE)
	CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
	if(COMPILER_SUPPORTS_MARCH_NATIVE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	endif()
endif()

# Optional GUI
if(IG_WITH_VIEWER)
    find_package(SDL2)

    if(SDL2_FOUND)
        set(IG_HAS_UI ON)
	else()
    	message(WARNING "Interactive viewer was requested but no SDL2 libraries were found. Disabling viewer")
    endif()
endif()

if(IG_HAS_UI)
    message(STATUS "Building with interactive viewer")
endif()

if(IG_WITH_PYTHON_API)
	find_package (Python3 COMPONENTS Development Interpreter)
	if(Python3_FOUND)
		set(IG_HAS_PYTHON_API ON)
	endif()
endif()

if(IG_HAS_PYTHON_API)
	message(STATUS "Building with Python API")
endif()

configure_file(src/runtime/config/Version.h.in ${CMAKE_CURRENT_BINARY_DIR}/config/Version.h)

# Add warnings per default
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

# Traverse to underlying directories
add_subdirectory(src)

if(IG_WITH_DOCUMENTATION)
	add_subdirectory(docs)
endif()