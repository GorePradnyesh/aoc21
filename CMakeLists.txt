cmake_minimum_required(VERSION 3.20)

# project
project(AOC21)

# includes 
include(FetchContent)
# include(${CMAKE_CURRENT_SOURCE_DIR}/Common/CMakeLists.txt)

# C++ global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall)
endif()


# =========== COMMON UTILS =========== #
#usage assign_source_group(${SRC})

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path "${_source_path}")
        source_group("${_source_path}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

# =========== BOOST =========== #
set(BOOST_URL "https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2" CACHE STRING "Boost 1.78.0 download URL")
set(BOOST_URL_SHA256 "8681f175d4bdb26c52222665793eef08490d7758529330f98d3b29dd0735bccc" CACHE STRING "Boost download URL SHA256 checksum")

FetchContent_Declare(
    Boost
    URL ${BOOST_URL}
    URL_HASH SHA256=${BOOST_URL_SHA256}
)
FetchContent_GetProperties(Boost)
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/native/external/boost/lib)
    message(STATUS "Fetching Boost")
    FetchContent_Populate(Boost)
    message(STATUS "Fetching Boost - done")

    # set options here if necessary
    set(BOOTSTRAP_CMD ${boost_SOURCE_DIR}/bootstrap.sh)
    set(PREFIX --prefix=${CMAKE_CURRENT_SOURCE_DIR}/native/external/boost)
    message(VERBOSE "prefix : " ${BOOTSTRAP_CMD})
    message(VERBOSE "Executing bootstrap cmd: ${BOOTSTRAP_CMD} ${PREFIX}")
    execute_process(COMMAND ${BOOTSTRAP_CMD} ${PREFIX} WORKING_DIRECTORY ${boost_SOURCE_DIR} COMMAND_ECHO STDOUT RESULT_VARIABLE ret)
    
    # TODO Move to helper function
    if(NOT ret EQUAL 0)
        message( FATAL_ERROR "Bootstrap failed. Bad exit status ${ret}")
    else()
        message(VERBOSE "Bootstrap completed successfully")
    endif()


    set(B2_CMD ${boost_SOURCE_DIR}/b2)
    set(B2_ARGS install)
    message(VERBOSE "Executing b2 cmd: ${B2_CMD} ${B2_ARGS}")
    execute_process(COMMAND ${B2_CMD} ${B2_ARGS} WORKING_DIRECTORY ${boost_SOURCE_DIR} COMMAND_ECHO STDOUT RESULT_VARIABLE ret)

    # TODO Move to helper function
    if(NOT ret EQUAL 0)
        message( FATAL_ERROR "b2 failed. Bad exit status ${ret}")
    else()
        message(VERBOSE "b2 completed successfully")
    endif()
endif()

# Sources
file(GLOB SRC CONFIGURE_DEPENDS 
    native/src/*.cpp
    native/src/common/*.cpp
    P18/*.cpp
    P20/*.cpp
    )

# includes
file(GLOB INC CONFIGURE_DEPENDS 
    native/include/common/*.h 
    P18/*.h
    P20/*.h
    )

# boost src
file(GLOB BOOST_INC CONFIGURE_DEPENDS
    native/external/boost/include/*.hpp)

assign_source_group(${SRC})
assign_source_group(${INC})
assign_source_group(${BOOST_INC})

add_executable(AOC21
    ${SRC} 
    ${INC}
    ${BOOST_INC})

# include paths
target_include_directories(AOC21 PRIVATE 
    native/include
    P18
    P20 
    native/external/boost/include)

# lib search dir
target_link_directories(AOC21 PRIVATE ${PROJECT_SOURCE_DIR}/native/external/boost/lib)

# link libs 
target_link_libraries(AOC21 boost_program_options)