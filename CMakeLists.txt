cmake_minimum_required(VERSION 3.12)
project(GreedGame)

set(CMAKE_CXX_STANDARD 17)

# Все исходные файлы
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(greed_game ${SOURCES} ${HEADERS})
target_include_directories(greed_game PRIVATE include)

add_custom_target(run
    COMMAND ./greed_game
    DEPENDS greed_game
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
