cmake_minimum_required(VERSION 3.20)

project(RayTracingWorkspace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set optimization flags based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Building in RelWithDebInfo mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -DNDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "Building in MinSizeRel mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DNDEBUG")
else()
    message(WARNING "Unknown build type, using default optimization flags")
endif()

#Set common includes windows / linux
set(IncludeDirs 
        ${CMAKE_SOURCE_DIR}/Walnut/vendor/glm
        ${CMAKE_SOURCE_DIR}/Walnut/vendor/GLFW/include
        ${CMAKE_SOURCE_DIR}/Walnut/vendor/imgui
        ${CMAKE_SOURCE_DIR}/Walnut/vendor/stb_image
        ${CMAKE_SOURCE_DIR}/Walnut/Walnut/src
)
#Set common libraries windows / linux
    set(Libraries
        imgui
        glfw
    )
#Setting Variables for WalnutCore
if(WIN32)
#Set Vulkan SDK for windows
    if(NOT DEFINED ENV{VULKAN_SDK})
        message(FATAL_ERROR "VULKAN_SDK environment variable is not set!")
    endif()
    set(VULKAN_SDK $ENV{VULKAN_SDK})
#Set vulkan include for Windows
    set(IncludeDirs ${IncludeDirs}
        ${VULKAN_SDK}\\Include
    )
#Set Library directories for windows
    set(LibraryDirs
    ${VULKAN_SDK}\\Lib
    )
#Set Libraries for Windows
    set(Libraries
        ${Libraries}
        vulkan-1
    )
elseif(UNIX)
#Set Vulkan SDK for linux
    if(DEFINED ENV{VULKAN_SDK})
        set(VULKAN_SDK $ENV{VULKAN_SDK})
    else()
        set(VULKAN_SDK /usr)
    endif()
#Set vulkan include for linux
    set(IncludeDirs
    ${IncludeDirs}
    ${VULKAN_SDK}/include
    )
#Set Library directories for linux
    set(LibraryDirs
    ${VULKAN_SDK}/lib
    )
#Set Libraries for Linux
    set(Libraries
        ${Libraries}
        vulkan
    )
endif()

# Add vendor subdirectories
add_subdirectory(Walnut/vendor/GLFW)
add_subdirectory(Walnut/vendor/imgui)

# Add Walnut subdirectory
add_subdirectory(Walnut/Walnut)

# Add RayTracing subdirectory
add_subdirectory(RayTracing)