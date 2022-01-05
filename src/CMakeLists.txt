cmake_minimum_required(VERSION 3.13)  # CMake version check
project(playgroundC)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# g++ -std=c++0x -o straw main.cpp straw.cpp -lcurl -lz
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -lcurl -lz")

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES playground.cpp)
add_executable(playground ${SOURCE_FILES})

target_link_libraries(playground curl z)