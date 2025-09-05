cmake_minimum_required(VERSION 3.16)

project(qtproject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Sql)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    todopage.h todopage.cpp
    sidemenu.h sidemenu.cpp
    maincontent.h maincontent.cpp
    login.h login.cpp
    database.h database.cpp
    user.h user.cpp
    todo.h todo.cpp
    homepage.h homepage.cpp
)

qt_add_resources(PROJECT_RESOURCES resources.qrc)

qt_add_executable(qtproject
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
    profilepage.h profilepage.cpp
)

target_link_libraries(qtproject PRIVATE Qt6::Widgets Qt6::Core Qt6::Sql)
target_link_libraries(qtproject PRIVATE Qt6::Widgets)

set_target_properties(qtproject PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

include(GNUInstallDirs)
install(TARGETS qtproject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(qtproject)
