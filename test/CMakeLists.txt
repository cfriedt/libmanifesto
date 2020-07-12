cmake_policy(SET CMP0048 NEW)

find_package(PkgConfig)

if(PKG_CONFIG_FOUND)

pkg_search_module(GTEST REQUIRED gtest_main)

if(GTEST_FOUND)

add_executable (manifesto-test manifesto-test.cpp)
add_test (NAME manifesto-test COMMAND manifesto-test)
target_include_directories (manifesto-test PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_compile_options (manifesto-test PUBLIC ${GTEST_CFLAGS})
target_link_libraries (manifesto-test LINK_PUBLIC manifesto ${GTEST_LDFLAGS})

add_executable (manifestoxx-test manifestoxx-test.cpp)
add_test (NAME manifestoxx-test COMMAND manifestoxx-test)
target_include_directories (manifestoxx-test PUBLIC ${CMAKE_SOURCE_DIR}/src)
set_property(TARGET manifestoxx-test PROPERTY CXX_STANDARD 17)
target_compile_options (manifestoxx-test PUBLIC ${GTEST_CFLAGS})
target_link_libraries (manifestoxx-test LINK_PUBLIC manifesto ${GTEST_LDFLAGS})

endif()
endif()
