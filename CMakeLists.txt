cmake_minimum_required(VERSION 3.10)
project(HEICConverter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(ImageMagick REQUIRED COMPONENTS Magick++)

# Source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(HEICConverter ${SOURCES})

# Link libraries
target_link_libraries(HEICConverter PRIVATE ImageMagick::Magick++)

# Include directories
target_include_directories(HEICConverter PRIVATE
    ${ImageMagick_INCLUDE_DIRS}
)