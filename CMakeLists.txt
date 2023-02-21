cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(alloc VERSION ${PROJECT_VESRION})

# ++for google test
include(FetchContent)   # run ..\CMake\share\cmake-X.YY\Modules\FetchContent.cmake
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

enable_testing()    # Enables testing for this directory and below
add_executable(alloc_test alloc_test.cpp)
target_link_libraries(alloc_test GTest::gtest_main)
# --for google test

add_executable(alloc alloc.cpp)

set_target_properties(alloc PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(alloc
    PRIVATE "${CMAKE_BINARY_DIR}"
)
if (MSVC)
    target_compile_options(alloc PRIVATE
        /W4
    )
else ()
    target_compile_options(alloc PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

include(GoogleTest)               # run ..\CMake\share\cmake-X.YY\Modules\GoogleTest.cmake
gtest_discover_tests(alloc_test)  # discover tests by asking the compiled test executable to enumerate its tests

install(TARGETS alloc alloc_test RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
