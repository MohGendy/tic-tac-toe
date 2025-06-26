cmake_minimum_required(VERSION 3.16)
project(my_program VERSION 0.1 LANGUAGES CXX)

# Enable Qt goodies
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)  # We'll use qt6_add_resources instead

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt modules
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Multimedia)

# Add resource file explicitly
qt6_add_resources(APP_RESOURCES assets/resources.qrc)

# Disable gmock/gtest install rules to avoid packaging errors
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# Pull in dependencies
add_subdirectory(googletest)
add_subdirectory(sqlite)
add_subdirectory(bcrypt)
add_subdirectory(windows_stub)

# Define include paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Gather sources
file(GLOB APP_SOURCES "src/main.cpp" "src/*.cpp" "ui/*.ui" "include/*.h")
file(GLOB TEST_SOURCES "src/gameStructure.cpp" "src/ai.cpp" "tests/*.cpp" "src/database.cpp")



# Create main app
qt6_add_executable(my_program
    MANUAL_FINALIZATION
    ${APP_SOURCES}
    ${APP_RESOURCES}
)


# Add Windows icon
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/assets/app_icon.rc")
    target_sources(my_program PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()

# Create test executable
add_executable(runTests ${TEST_SOURCES})

# Link main app
target_link_libraries(my_program PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    sqlite3
    bcrypt
    Threads::Threads
    #-mwindows  # For Windows GUI applications
)
if(MINGW)
    target_link_options(my_program PRIVATE -mwindows)
endif()

if(MSVC)
    set(GUI_FLAG WIN32)  # MSVC needs WIN32
endif()

# Link tests
target_link_libraries(runTests PRIVATE
    sqlite3
    bcrypt
    windows_stub
    gtest
    gtest_main
    Threads::Threads
)

# Finalize target and version info
set_target_properties(my_program PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.my_program
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Finalize Qt executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(my_program)
    qt_generate_deploy_app_script(
        TARGET my_program
        OUTPUT_SCRIPT deploy_my_program
    )
    install(SCRIPT ${deploy_my_program})
endif()

# Install rules
include(GNUInstallDirs)
install(TARGETS my_program
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Testing + Packaging
enable_testing()
set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "my_program")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
