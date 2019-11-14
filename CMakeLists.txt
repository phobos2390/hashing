cmake_minimum_required(VERSION 2.6.0)
set(PROJ_NAME phobos2390)
set(REPO_NAME hasher)

project("${PROJ_NAME}-${REPO_NAME}")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_ROOT_DIR ${PROJ_DIR}/src/${PROJ_NAME}/${REPO_NAME})

include_directories("${PROJ_DIR}/catch2/single_include")
include_directories("${PROJ_DIR}/src/${PROJ_NAME}")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

#set(CMAKE_CXX_COMPILER '/opt/rh/devtoolset-4/root/bin/g++')
#set(CMAKE_C_COMPILER '/opt/rh/devtoolset-4/root/bin/gcc')

set(CMAKE_CXX_FLAGS "-g -std=c++11")

#message("moving ${CMAKE_CURRENT_SOURCE_DIR}/res to binary ${CMAKE_CURRENT_BINARY_DIR}/src/${PROJ_NAME}/${REPO_NAME}/")
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res"
#     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src/${PROJ_NAME}/${REPO_NAME}/")

set(SOURCE_DIRECTORIES ${SRC_ROOT_DIR})

set(all_source_files "")
foreach(source_dir ${SOURCE_DIRECTORIES})
  message("Finding everything in ${source_dir}")
  file(GLOB dir_src_files ${source_dir}/*.cpp)
  set(all_source_files ${all_source_files}
                       ${dir_src_files})
endforeach()

set(LIB_NAME_SHARED ${PROJ_NAME}_${REPO_NAME})
set(LIB_NAME_STATIC ${PROJ_NAME}_${REPO_NAME}_s)
set(PHOBOS2390_HASHER_STATIC_LIB ${LIB_NAME_STATIC})
set(PHOBOS2390_HASHER_SHARED_LIB ${LIB_NAME_SHARED})

message("Building static library ${LIB_NAME_STATIC} with ${all_source_files}")
add_library(${LIB_NAME_STATIC} STATIC ${all_source_files})
message("Building shared library ${LIB_NAME_SHARED} with ${all_source_files}")
add_library(${LIB_NAME_SHARED} SHARED ${all_source_files})

add_executable(hasher ${SRC_ROOT_DIR}/app/hasher_app.cpp)
target_link_libraries(hasher ${LIB_NAME_STATIC})
add_executable(hasher_test ${SRC_ROOT_DIR}/test/hasher_test.cpp
                           ${SRC_ROOT_DIR}/test/digest_test.cpp)
target_link_libraries(hasher_test ${LIB_NAME_STATIC})