SET(SRC_FILES 
    main.cpp )

add_executable(ig_test_multiple_runtimes ${SRC_FILES})
target_link_libraries(ig_test_multiple_runtimes PRIVATE ig_lib_common)

set(scenes ${PROJECT_SOURCE_DIR}/scenes/diamond_scene.json ${PROJECT_SOURCE_DIR}/scenes/furnance.json ${PROJECT_SOURCE_DIR}/scenes/primitives.json)
add_test(NAME ignis_test_multiple_runtimes COMMAND ig_test_multiple_runtimes ${scenes})
