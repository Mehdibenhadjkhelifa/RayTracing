cmake_minimum_required(VERSION 3.20)

project(RayTracingWorkspace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Vulkan SDK configuration (same as before)
if(WIN32)
    if(NOT DEFINED ENV{VULKAN_SDK})
        message(FATAL_ERROR "VULKAN_SDK environment variable is not set!")
    endif()
    set(VULKAN_SDK $ENV{VULKAN_SDK})
elseif(UNIX)
    if(DEFINED ENV{VULKAN_SDK})
        set(VULKAN_SDK $ENV{VULKAN_SDK})
    else()
        set(VULKAN_SDK /usr)
    endif()
endif()

set(IncludeDir
    ${VULKAN_SDK}/include
    ${CMAKE_SOURCE_DIR}/Walnut/vendor/glm
)

set(LibraryDir
    ${VULKAN_SDK}/lib
)

set(Libraries
    vulkan
)

# Add vendor subdirectories
add_subdirectory(Walnut/vendor/GLFW)
add_subdirectory(Walnut/vendor/imgui)

# Add Walnut subdirectory
add_subdirectory(Walnut/Walnut)

# Add RayTracing subdirectory
add_subdirectory(RayTracing)

