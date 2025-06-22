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
add_subdirectory(windows_stub)

# your own include dirs (if you need them)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# collect sources
file(GLOB APP_SOURCES "src/main.cpp" "src/*.cpp" "ui/*.ui" "assets/*.qrc" "include/*.h")
file(GLOB TEST_SOURCES
    #"tests/perfomance_gamestructure.cpp"
    "src/gameStructure.cpp"
    "src/ai.cpp"
    # "tests/database_googletest.cpp"
    # "tests/AItests.cpp"
    # "tests/pvptests.cpp"
    # "tests/megatests.cpp"
    "tests/PtestForAi.cpp"
    # "tests/performance_ai.cpp"
    # "tests/perfomance_gamestructure.cpp"
    # "src/database.cpp"
  )


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(my_program
        #WIN32  # Uncomment this line if you want a Windows GUI application
        MANUAL_FINALIZATION
        ${APP_SOURCES}
    )

if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/assets/app_icon.rc")
    target_sources(my_program PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()

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
    windows_stub
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
