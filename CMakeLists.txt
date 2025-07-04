cmake_minimum_required(VERSION 3.10)
project(app C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Define source files
file(GLOB_RECURSE SRC_FILES "src/*.c")
set(GLAD_FILE "lib/glad/glad.c")

# Include directories
include_directories("include" "src")

# Add FreeType as a subdirectory if building from source (adjust path as needed)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${GLAD_FILE})

# Link directories
target_link_directories(${PROJECT_NAME} PRIVATE lib/GLFW)

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw3_mt)

# Define debug flags
target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
