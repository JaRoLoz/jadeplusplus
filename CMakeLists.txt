cmake_minimum_required(VERSION 3.16)

project(jade++ LANGUAGES CXX)

find_package(boost_regex CONFIG REQUIRED)
find_package(boost_process CONFIG REQUIRED)
find_package(boost_asio CONFIG REQUIRED)
find_package(boost_filesystem CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_path(RANG_INCLUDE_DIRS "rang.hpp")

file(
    GLOB_RECURSE 
    SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(
    GLOB_RECURSE 
    HEADER_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(jade ${SRC_FILES} ${HEADER_FILES})
target_include_directories(jade PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(jade PRIVATE cxx_std_20)

target_compile_definitions(jade PRIVATE WIN32_LEAN_AND_MEAN)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

target_link_libraries(jade PRIVATE Boost::regex)
target_link_libraries(jade PRIVATE Boost::process)
target_link_libraries(jade PRIVATE Boost::asio)
target_link_libraries(jade PRIVATE Boost::filesystem)
target_link_libraries(jade PRIVATE pugixml)

target_include_directories(jade PRIVATE ${RANG_INCLUDE_DIRS})