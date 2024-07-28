message(STATUS "[qvgeio] QT_VERSION: ${QT_VERSION}, QT_DIR: ${QT_DIR}")

if (${QT_VERSION} VERSION_LESS 5.11.0)
  message(FATAL_ERROR "Requires qt version >= 5.11.0, Your current version is ${QT_VERSION}")
endif()

file(GLOB qvgeio_SRCS
    "*.cpp"
)
file(GLOB qvgeio_HDRS
    "*.h"
)

qt_add_library(qvgeio STATIC)

target_sources(qvgeio
    PRIVATE
        ${qvgeio_SRCS}
)
target_include_directories(qvgeio
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)

target_link_libraries(qvgeio
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
)

set_target_properties(qvgeio
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
