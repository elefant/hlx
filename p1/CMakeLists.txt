cmake_minimum_required(VERSION 2.8)

project( TestClientServer )

find_package( Boost COMPONENTS thread REQUIRED )

add_subdirectory( libdnet )

include_directories ( libdnet/src )
include_directories ( ${Boost_INCLUDE_DIRS} )

#-----------------------------------------------------
# libdnet
#-----------------------------------------------------
target_link_libraries(dnet)

#------------------------------------------------
# TestTcpServer
#------------------------------------------------
add_executable(TestServer
	src/ServerMain.cpp)

target_link_libraries( TestServer dnet )

set_target_properties( TestServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    COMPILE_FLAGS "-Wall -Wextra -std=c++11"
    )

#------------------------------------------------
# TestTcpClient
#------------------------------------------------
add_executable( TestClient
    src/ClientMain.cpp
    )

target_link_libraries( TestClient dnet )

set_target_properties( TestClient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    COMPILE_FLAGS "-Wall -Wextra -std=c++11"
    )
