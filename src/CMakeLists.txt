cmake_minimum_required (VERSION 3.8)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(GLEW REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)

if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/permissive->")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:forScope>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:inline>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:rvalueCast>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:wchar_t>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:throwingNew>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:referenceBinding>")

add_executable (USPKart "resource/resourceManager.cpp"
                        "controls/controlsHandler.cpp"
                        "graphic/graphicsHelper.cpp"
                        "graphic/drawingHelper.cpp"
                        "graphic/primitives.cpp"
                        "game/config.cpp"
                        "game/utils.cpp"
                        "game/game.cpp" )
                        
target_include_directories(USPKart PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR})

target_link_libraries(USPKart PRIVATE   GLEW::GLEW
                                        glfw
                                        OpenAL::OpenAL
                                        soil2
                                        OpenGL::GL
                                        ${OPENGL_LIBRARIES}
                                        glm::glm
                                        glad::glad 
                                        nlohmann_json::nlohmann_json)

function(copy_directory SRC_DIR DST_DIR)
    file(MAKE_DIRECTORY ${DST_DIR})

    file(GLOB_RECURSE ITEMS RELATIVE ${SRC_DIR} ${SRC_DIR}/*)

    foreach(ITEM ${ITEMS})
        set(SRC "${SRC_DIR}/${ITEM}")
        set(DST "${DST_DIR}/${ITEM}")

        if(IS_DIRECTORY ${SRC})
            file(MAKE_DIRECTORY ${DST})
        else()
            configure_file(${SRC} ${DST} COPYONLY)
        endif()
    endforeach()
endfunction()

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

copy_directory(${SOURCE_DIR} ${DESTINATION_DIR})

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

copy_directory(${SOURCE_DIR} ${DESTINATION_DIR})