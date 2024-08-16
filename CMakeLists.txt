cmake_minimum_required(VERSION 2.6)
project(monitor)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find and include Curses
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# Hardcode the Boost include and library directories
set(BOOST_INCLUDEDIR "/usr/include")
set(BOOST_LIBRARYDIR "/usr/lib64")

# Specify the Boost libraries to link against
set(BOOST_FILESYSTEM_LIB "${BOOST_LIBRARYDIR}/libboost_filesystem.so.1.66.0")
set(BOOST_SYSTEM_LIB "${BOOST_LIBRARYDIR}/libboost_system.so.1.66.0")

# Add include directories
include_directories(${BOOST_INCLUDEDIR})

# Include additional directories
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(monitor ${SOURCES})

# Link libraries explicitly
target_link_libraries(monitor ${CURSES_LIBRARIES} ${BOOST_FILESYSTEM_LIB} ${BOOST_SYSTEM_LIB})

# Compile options
target_compile_options(monitor PRIVATE -Wall -Wextra)