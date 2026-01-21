cmake_minimum_required(VERSION 3.12)
project(GreedGame)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(greed_game ${SOURCES} ${HEADERS})

add_custom_target(run
    COMMAND ./greed_game
    DEPENDS greed_game
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
