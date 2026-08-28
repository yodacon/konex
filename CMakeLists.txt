cmake_minimum_required(VERSION 3.16)
project(konex CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# The 2005 sources predate a lot of C++ tightening; keep the noise down but
# do not silence real errors.
add_compile_options(-Wno-write-strings -Wno-deprecated-declarations
                    -Wno-format-security -Wno-unused-value)

set(KONEX_SOURCES
    src/ai.cpp src/config.cpp src/console.cpp src/entity.cpp src/explosion.cpp
    src/game.cpp src/in_main.cpp src/interface.cpp src/item.cpp src/konex.cpp
    src/map.cpp src/menu.cpp src/missile.cpp src/net_main.cpp src/planets.cpp
    src/player.cpp src/ships.cpp src/spawn.cpp src/stars.cpp src/sys_main.cpp
    src/target.cpp src/vid_image.cpp src/vid_main.cpp src/vid_model.cpp
    src/view.cpp src/xml.cpp
    src/tinyxml/tinystr.cpp src/tinyxml/tinyxml.cpp
    src/tinyxml/tinyxmlerror.cpp src/tinyxml/tinyxmlparser.cpp)

if(APPLE)
    # macOS: SDL2 window + OpenGL framework, no X11/GLX, no OpenAL.
    list(APPEND KONEX_SOURCES src/sys_linux.cpp src/tm_linux.cpp src/vid_sdl.cpp)
elseif(UNIX)
    list(APPEND KONEX_SOURCES src/sys_linux.cpp src/tm_linux.cpp src/vid_linux.cpp)
else()
    list(APPEND KONEX_SOURCES src/sys_win.cpp src/tm_win32.cpp src/vid_win32.cpp)
endif()

add_executable(konex ${KONEX_SOURCES})
target_include_directories(konex PRIVATE src src/tinyxml)

if(APPLE)
    find_package(SDL2 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(konex PRIVATE SDL2::SDL2 SDL2::SDL2main OpenGL::GL)
    target_link_libraries(konex PRIVATE "-framework OpenGL")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(konex PRIVATE OpenGL::GL OpenGL::GLU X11::X11 openal alut)
endif()

# The engine resolves data paths relative to the executable, so stage dist/
# (config.xml, data/) next to the binary after every build.
add_custom_command(TARGET konex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/dist $<TARGET_FILE_DIR:konex>)
