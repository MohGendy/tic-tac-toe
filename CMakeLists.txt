cmake_minimum_required(VERSION 3.16)

project(my_program VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# look in ./ui for all .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# pull in googletest and sqlite subdirs
add_subdirectory(googletest)
add_subdirectory(sqlite)
add_subdirectory(bcrypt)

# your own include dirs (if you need them)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# collect sources
file(GLOB APP_SOURCES "src/main.cpp" "src/*.cpp" "ui/*.ui" "assets/*.qrc" "include/*.h")
file(GLOB TEST_SOURCES
    "tests/performance_ai.cpp"
    "src/gameStructure.cpp"
    "src/ai.cpp"
    # "src/database.cpp"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(my_program
        MANUAL_FINALIZATION
        ${APP_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET my_program APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(my_program SHARED
            ${APP_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(my_program
            ${APP_SOURCES}
        )
    endif()
endif()
# executables
# add_executable(my_program ${APP_SOURCES})
add_executable(runTests  ${TEST_SOURCES})

# link libraries 
#  - sqlite3 (your own)
#  - gtest, gtest_main (googletest)
#  - Threads if you use pthread
find_package(Threads REQUIRED)

target_link_libraries(my_program
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    sqlite3
    bcrypt
    Threads::Threads
)

target_link_libraries(runTests
  PRIVATE
    # sqlite3
    # bcrypt
    gtest
    gtest_main
    Threads::Threads
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.my_program)
endif()
set_target_properties(my_program PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS my_program
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(my_program)
endif()

# Installation rules.
include(GNUInstallDirs)
install(TARGETS my_program
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(my_program)
    qt_generate_deploy_app_script(
        TARGET my_program
        OUTPUT_SCRIPT deploy_my_program
    )
    install(SCRIPT ${deploy_my_program})
endif()

# CPack configuration – choose your generator.
set(CPACK_GENERATOR "NSIS")  # or "ZIP" or any preferred generator.
set(CPACK_PACKAGE_NAME "my_program")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)

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
