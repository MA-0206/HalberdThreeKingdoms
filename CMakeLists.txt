cmake_minimum_required(VERSION 3.19)
project(HalberdThreeKingdoms LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(HalberdThreeKingdoms
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    gameview.h gameview.cpp
    animator.h animator.cpp
    assets/images/characters/liubei/liubei.qrc
    assets/images/ui/loading/start_background.qrc
    assets/images/maps/level_map/level_yellowscarf.qrc
)

target_link_libraries(HalberdThreeKingdoms
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS HalberdThreeKingdoms
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET HalberdThreeKingdoms
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
