project(RayTracing)

# Add source files for RayTracing
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

# Include directories
include_directories(${IncludeDirs})
# Link RayTracing executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link dependencies
target_link_libraries(RayTracing PRIVATE WalnutCore)