project(RayTracing)

# Add source files for RayTracing
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

# Include directories
include_directories(
    ../Walnut/vendor/glm
    ../Walnut/vendor/GLFW/include
    ../Walnut/vendor/imgui
    ../Walnut/Walnut/src
)

# Link RayTracing executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link dependencies
target_link_libraries(RayTracing PRIVATE
    WalnutCore
)
# Set optimization flags based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Building in RelWithDebInfo mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -DNDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "Building in MinSizeRel mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DNDEBUG")
else()
    message(WARNING "Unknown build type, using default optimization flags")
endif()
