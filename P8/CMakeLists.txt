
set(SOURCELIST src/P8.cpp)
set(HEADERLIST include/P8.h)

target_sources(aoc PRIVATE ${SOURCELIST} ${HEADERLIST})

target_include_directories(aoc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# source_group("src" TREE src)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Sources" FILES ${SOURCELIST})