if(USE_QT6)
  find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
else()
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets PrintSupport Svg Xml)
message(STATUS "[qvge] QT_VERSION: ${QT_VERSION}, QT_DIR: ${QT_DIR}")

if (${QT_VERSION} VERSION_LESS 5.11.0)
  message(FATAL_ERROR "Requires qt version >= 5.11.0, Your current version is ${QT_VERSION}")
endif()

set(CMAKE_AUTOMOC ON)

file(GLOB qvgelib_SRCS
    "*.cpp"
)
file(GLOB qvgelib_HDRS
    "*.h"
)

qt_add_library(qvgelib STATIC)

target_sources(qvgelib
    PRIVATE
        ${qvgelib_SRCS}
)
target_include_directories(qvgelib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)

target_link_libraries(qvgelib
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::Xml
)

set_target_properties(qvgelib
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

qt_finalize_target(qvgelib)
