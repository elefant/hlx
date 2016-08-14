cmake_minimum_required( VERSION 2.8 )

project( dnet )

set( MY_SRC_DIR ${PROJECT_SOURCE_DIR}/src )

#--------------------------------------
# Include and link directories
#--------------------------------------
include_directories( ${MY_SRC_DIR} )

#--------------------------------------
# Source files
#--------------------------------------
set(
    SOURCE_FILES
    ${MY_SRC_DIR}/net/Socket.cpp
    ${MY_SRC_DIR}/net/TcpSocket.cpp
    ${MY_SRC_DIR}/net/UdpSocket.cpp
    ${MY_SRC_DIR}/net/TcpServer.cpp
    ${MY_SRC_DIR}/net/Misc.cpp
    ${MY_SRC_DIR}/net/PortPool.cpp

    ${MY_SRC_DIR}/sys/ThreadPool.cpp
    ${MY_SRC_DIR}/sys/Shell.cpp
    #${MY_SRC_DIR}/sys/Application.cpp

    ${MY_SRC_DIR}/utl/Logger.cpp
    ${MY_SRC_DIR}/utl/String.cpp
    ${MY_SRC_DIR}/utl/Timer.cpp
    ${MY_SRC_DIR}/utl/Misc.cpp
    )

#--------------------------------------
# Build Library
#--------------------------------------
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    libboost_thread.a
    libboost_date_time.a
    libboost_filesystem.a
    libboost_system.a
    pthread
    )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
