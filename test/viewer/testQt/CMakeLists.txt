# Project
project(TestQt)

# Code
set(TestQt_SOURCES 
    src/main.cpp
    src/MainWindow.cpp
    src/ViewerWidget/ViewerWidget.cpp
    )

set(TestQt_HEADERS
    src/MainWindow.h
    src/ViewerWidget/ViewerWidget.h
    )

set(TestQt_FORMS
    src/MainWindow.ui
    )

add_executable(${PROJECT_NAME}
    ${TestQt_SOURCES}
    ${TestQt_HEADERS}
    ${TestQt_FORMS}
)

# Link libs
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core
    Qt6::Concurrent
    Qt6::OpenGL
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Viewer
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME ${PROJECT_NAME}
                      PROJECT_LABEL "(TEST) ${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      FOLDER "test/viewer")

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)