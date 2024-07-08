set(OpenGL_GL_PREFERENCE "GLVND")

find_package(GLEW REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

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

# Add source to this project's executable.
add_executable (USPKart "graphicsHelper.cpp" 
                        "graphicsHelper.hpp" 
                        "config.cpp" 
                        "config.hpp"
                        "game.cpp" 
                        "game.hpp" )



target_link_libraries(USPKart PRIVATE   GLEW::GLEW
                                        -lSDL2
                                        SDL2
                                        SDL2_image
                                        SDL2_ttf
                                        SDL2_mixer
                                        ${OPENGL_LIBRARIES}
                                        glm::glm
                                        glad::glad 
                                        nlohmann_json::nlohmann_json)

function(copy_directory SRC_DIR DST_DIR)
    # Cria o diretório de destino se ele não existir
    file(MAKE_DIRECTORY ${DST_DIR})

    # Obtém uma lista de todos os arquivos e diretórios no diretório de origem
    file(GLOB_RECURSE ITEMS RELATIVE ${SRC_DIR} ${SRC_DIR}/*)

    foreach(ITEM ${ITEMS})
        # Determina o caminho de origem completo
        set(SRC "${SRC_DIR}/${ITEM}")
        # Determina o caminho de destino completo
        set(DST "${DST_DIR}/${ITEM}")

        # Verifica se o item atual é um diretório
        if(IS_DIRECTORY ${SRC})
            # Cria o diretório de destino
            file(MAKE_DIRECTORY ${DST})
        else()
            # Copia o arquivo para o diretório de destino
            configure_file(${SRC} ${DST} COPYONLY)
        endif()
    endforeach()
endfunction()

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

copy_directory(${SOURCE_DIR} ${DESTINATION_DIR})