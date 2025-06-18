cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)

# pull in googletest and sqlite subdirs
add_subdirectory(googletest)
add_subdirectory(sqlite)

# your own include dirs (if you need them)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# collect sources
file(GLOB APP_SOURCES "src/*.cpp")
file(GLOB TEST_SOURCES
    "tests/*.cpp"
    "src/gameStructure.cpp"
    "src/ai.cpp"
)

# executables
add_executable(my_program ${APP_SOURCES})
add_executable(runTests  ${TEST_SOURCES})

# link libraries 
#  - sqlite3 (your own)
#  - gtest, gtest_main (googletest)
#  - Threads if you use pthread
find_package(Threads REQUIRED)

target_link_libraries(my_program
  PRIVATE
    sqlite3
    Threads::Threads
)

target_link_libraries(runTests
  PRIVATE
    sqlite3
    gtest
    gtest_main
    Threads::Threads
)
# cmake_minimum_required(VERSION 3.10)
# project(MyProject)

# # Include directories
# add_subdirectory(googletest)
# add_subdirectory(sqlite)
# include_directories(googletest/include)
# include_directories(sqlite)
# include_directories(include)

# # Gather source files
# file(GLOB SOURCES "src/*.cpp")

# # Gather test sources and include game logic files
# file(GLOB TEST_SOURCES "tests/*.cpp" "src/gameStructure.cpp" "src/ai.cpp")

# # Add executables
# add_executable(my_program ${SOURCES})
# add_executable(runTests ${TEST_SOURCES})

# # Link libraries
# target_link_libraries(my_program gtest gtest_main pthread)
# target_link_libraries(runTests gtest gtest_main pthread)
################################

# cmake_minimum_required(VERSION 3.19)
# project(my_program LANGUAGES CXX)

# find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# qt_standard_project_setup()

# qt_add_executable(my_program
#     WIN32 MACOSX_BUNDLE
#     main.cpp
#     mainwindow.cpp
#     mainwindow.h
#     mainwindow.ui
# )

# target_link_libraries(my_program
#     PRIVATE
#         Qt::Core
#         Qt::Widgets
# )

# include(GNUInstallDirs)

# install(TARGETS my_program
#     BUNDLE  DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# qt_generate_deploy_app_script(
#     TARGET my_program
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )

# install(SCRIPT ${deploy_script})