cmake_minimum_required(VERSION 3.27)
project(Voxels VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Voxels main.cpp)
target_include_directories(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Voxels PRIVATE GL)

target_sources(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/WindowBuilder.cpp)
target_sources(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/WindowManager.cpp)
target_sources(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/OpenGl/Renderer.cpp)
target_sources(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/OpenGl/WindowVisitor.cpp)

if(CMAKE_HOST_LINUX)
    find_package(X11 REQUIRED)
    target_link_libraries(Voxels PRIVATE ${X11_LIBRARIES})
    target_sources(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Linux/X11/Window.cpp)
    target_sources(Voxels PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Linux/X11/EventTranslator.cpp)
endif()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
