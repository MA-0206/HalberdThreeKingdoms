cmake_minimum_required(VERSION 3.19)
project(HalberdThreeKingdoms LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(HalberdThreeKingdoms
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/ui/screens/mainwindow.cpp
    src/ui/screens/mainwindow.h
    src/ui/screens/mainwindow.ui
    src/engine/gameview.h
    src/engine/gameview.cpp
    src/engine/animator.h
    src/engine/animator.cpp
    assets/images/characters/liubei/liubei.qrc
    assets/images/ui/loading/start_background.qrc
    assets/images/maps/level_map/level_yellowscarf.qrc
    src/entities/EntityState.h
    src/entities/entity.h src/entities/entity.cpp
    src/entities/player/player.h src/entities/player/player.cpp
    src/ui/hud/gamehud.h src/ui/hud/gamehud.cpp
    src/entities/enemy/enemy.h src/entities/enemy/enemy.cpp
    assets/images/enemies/soldier/yellowscarf/yellowscarf.qrc
    src/world/world.h src/world/world.cpp
)

target_link_libraries(HalberdThreeKingdoms
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(HalberdThreeKingdoms PRIVATE
    src
    src/engine
    src/entities
    src/entities/player
    src/entities/enemy
    src/ui
    src/ui/screens
    src/ui/hud
    src/world
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
