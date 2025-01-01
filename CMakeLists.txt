# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set vcpkg directory
set(VCPKG_DIR "${CMAKE_BINARY_DIR}/vcpkg")

# Function to read vcpkg version from vcpkg-configuration.json
function(read_vcpkg_version)
    file(READ "${CMAKE_SOURCE_DIR}/vcpkg-configuration.json" VCPKG_CONFIG_CONTENT)
    string(REGEX MATCH "\"baseline\": \"([^\"]+)\"" _ ${VCPKG_CONFIG_CONTENT})
    set(VCPKG_VERSION ${CMAKE_MATCH_1} PARENT_SCOPE)
endfunction()

# Read vcpkg version
read_vcpkg_version()

# Download and install vcpkg if not already installed
if(NOT EXISTS ${VCPKG_DIR}/vcpkg)
    message(STATUS "Downloading vcpkg version ${VCPKG_VERSION}...")
    file(DOWNLOAD https://github.com/microsoft/vcpkg/archive/${VCPKG_VERSION}.tar.gz ${CMAKE_BINARY_DIR}/vcpkg.tar.gz)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/vcpkg.tar.gz WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    file(RENAME ${CMAKE_BINARY_DIR}/vcpkg-${VCPKG_VERSION} ${VCPKG_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/vcpkg.tar.gz)
    if(UNIX)
        execute_process(COMMAND ${CMAKE_COMMAND} -E chdir ${VCPKG_DIR} ./bootstrap-vcpkg.sh)
    elseif(WIN32)
        execute_process(COMMAND ${CMAKE_COMMAND} -E chdir ${VCPKG_DIR} .\\bootstrap-vcpkg.bat)
    endif()
endif()

# Set the toolchain file
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")

project("USPKart" VERSION 0.5.4)

# Include sub-projects.
add_subdirectory("src")
