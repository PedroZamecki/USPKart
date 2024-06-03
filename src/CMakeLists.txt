find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(soil2 REQUIRED)
find_package(nlohmann_json REQUIRED)

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
                                        soil2 
                                        glfw 
                                        glm::glm
                                        glad::glad 
                                        nlohmann_json::nlohmann_json)

file(GLOB ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
foreach(ASSET ${ASSETS})
    get_filename_component(ASSET_NAME ${ASSET} NAME)
    configure_file(${ASSET} ${CMAKE_CURRENT_BINARY_DIR}/assets/${ASSET_NAME} COPYONLY)
endforeach()