set(HEADER_FILES daemon_server.h tcp_server.h)

set(SOURCE_FILES daemon_server.cc tcp_server.cc)

add_executable(server ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(server PUBLIC Poco::Util Poco::JSON Poco::XML)

configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/config.xml"
        "${CMAKE_CURRENT_BINARY_DIR}/"
        COPYONLY
)