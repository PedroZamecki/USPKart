cmake_minimum_required(VERSION 3.8)

# Set the #define USP_KART_VERSION to the project version
add_definitions(-DUSP_KART_VERSION="${PROJECT_VERSION}")

# Remove GLU dependency
add_definitions(-DGLEW_NO_GLU)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(nlohmann_json REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

if (NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif ()

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

file(GLOB_RECURSE SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*.hpp"
)

add_executable(USPKart ${SOURCE_FILES})

target_include_directories(USPKart PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

target_link_libraries(USPKart PRIVATE GLEW::GLEW
        glfw
        OpenAL::OpenAL
        OpenGL::GL
        ${OPENGL_LIBRARIES}
        glm::glm
        soil2
        nlohmann_json::nlohmann_json
        assimp::assimp
        imgui::imgui)

function(copy_directory SRC_DIR DST_DIR)
    file(MAKE_DIRECTORY ${DST_DIR})

    file(GLOB_RECURSE ITEMS RELATIVE ${SRC_DIR} ${SRC_DIR}/*)

    foreach (ITEM ${ITEMS})
        set(SRC "${SRC_DIR}/${ITEM}")
        set(DST "${DST_DIR}/${ITEM}")

        if (IS_DIRECTORY ${SRC})
            file(MAKE_DIRECTORY ${DST})
        else ()
            configure_file(${SRC} ${DST} COPYONLY)
        endif ()
    endforeach ()

    message("-- Copying files from \"${SRC_DIR}\" to \"${DST_DIR}\" done")
endfunction()

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

message(STATUS "Copying assets to build directory")

copy_directory(${SOURCE_DIR} ${DESTINATION_DIR})
