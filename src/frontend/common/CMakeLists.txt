CPMAddPackage(
    NAME cli11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG main
    DOWNLOAD_ONLY YES
)

add_library(ig_lib_common STATIC CameraProxy.h IO.h IO.cpp ProgramOptions.cpp ProgramOptions.h SPPMode.h)
target_link_libraries(ig_lib_common PUBLIC ig_lib_runtime TBB::tbb)
target_include_directories(ig_lib_common PRIVATE ${cli11_SOURCE_DIR}/include)
target_include_directories(ig_lib_common PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
add_checks(ig_lib_common)
