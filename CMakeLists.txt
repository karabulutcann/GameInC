cmake_minimum_required(VERSION 3.10)
project(TestApp C)

# Set C standard
set(CMAKE_C_STANDARD 11)

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
target_link_libraries(${PROJECT_NAME} glfw3_mt msvcrtd libcmtd user32 kernel32 gdi32 shell32)

# Define debug flags
target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
