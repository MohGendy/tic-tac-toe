# (optional) add compile‐time flags
# target_compile_definitions(bcrypt PRIVATE BCRYPT_USE_CUSTOM_ALLOCATOR)
cmake_minimum_required(VERSION 3.10)
project(bcrypt CXX)

# collect all the .c files from the amalgamation or src/
file(GLOB BCYPTE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Build bcrypt as a static library
add_library(bcrypt STATIC ${BCYPTE_SOURCES})

# Install headers (if you need install() later)
target_include_directories(bcrypt
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional compile-time flags: tweak as you like
# e.g. target_compile_definitions(bcrypt PRIVATE SQLITE_THREADSAFE=1)

# (You no longer need add_executable here!)