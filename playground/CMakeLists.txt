cmake_minimum_required(VERSION 3.10)
project(app C)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Define source files
file(GLOB_RECURSE SRC_FILES "src/*.c")
file(GLOB_RECURSE INCLUDE_SRC_FILES "../src/*.c")

get_filename_component(full_path_to_main ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c ABSOLUTE)

message(STATUS "full_path_to_main: ${full_path_to_main}")
message("${INCLUDE_SRC_FILES}")
list(REMOVE_ITEM INCLUDE_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../src/main.c")
message("${INCLUDE_SRC_FILES}")

set(GLAD_FILE "../lib/glad/glad.c")

# Include directories
include_directories("../include","src", "../src")

# Add FreeType as a subdirectory if building from source (adjust path as needed)
# add_subdirectory(../packages/freetype)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCLUDE_SRC_FILES} ${GLAD_FILE})

# Link directories
target_link_directories(${PROJECT_NAME} PRIVATE ../lib/GLFW)

# Link libraries
target_link_libraries(${PROJECT_NAME} glfw3_mt msvcrtd libcmtd user32 kernel32 gdi32 shell32)

# Define debug flags
target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
