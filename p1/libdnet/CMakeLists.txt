cmake_minimum_required( VERSION 2.8 )

project( dnet )

find_package( Threads REQUIRED )
find_package( Boost REQUIRED )

message(${Boost_INCLUDE_DIRS})

set( MY_SRC_DIR ${PROJECT_SOURCE_DIR}/src )

#--------------------------------------
# Include and link directories
#--------------------------------------
include_directories( ${MY_SRC_DIR} )
include_directories( ${Boost_INCLUDE_DIRS} )

link_directories( ${Boost_LIBRARY_DIRS} )

#--------------------------------------
# Source files
#--------------------------------------
set(
    SOURCE_FILES
    ${MY_SRC_DIR}/net/Socket.cpp
    ${MY_SRC_DIR}/net/TcpSocket.cpp
    ${MY_SRC_DIR}/net/UdpSocket.cpp
    ${MY_SRC_DIR}/net/TcpServer.cpp
    #${MY_SRC_DIR}/net/Misc.cpp
    #${MY_SRC_DIR}/net/PortPool.cpp

    ${MY_SRC_DIR}/sys/ThreadPool.cpp

    ${MY_SRC_DIR}/utl/Logger.cpp
    ${MY_SRC_DIR}/utl/String.cpp
    #${MY_SRC_DIR}/utl/Timer.cpp
    )

#--------------------------------------
# Build Library
#--------------------------------------
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    ${Boost_THREAD_LIBRARY}
    libboost_date_time.a
    libboost_system.a
    libboost_filesystem.a
    pthread
    )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
