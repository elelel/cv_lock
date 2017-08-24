cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(cv_lock_test)

find_package(Threads REQUIRED)

get_filename_component(CV_LOCK_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

set(INCLUDE_DIRS
  "${CV_LOCK_DIR}/contrib/Catch/include"
  )

set(TEST_SOURCES
  src/cv_lock.cpp
  )

add_executable(cv_lock_test ${TEST_SOURCES})
target_include_directories(cv_lock_test PRIVATE ${INCLUDE_DIRS})
target_link_libraries(cv_lock_test ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(cv_lock_test PRIVATE "CATCH_CONFIG_MAIN" "CATCH_CONFIG_COLOUR_NONE")
