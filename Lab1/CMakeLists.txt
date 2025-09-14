# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab1 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)

# Add source files
# file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/Ball.cpp
# 				  ${PROJECT_SOURCE_DIR}/Bat.cpp
# 				  ${PROJECT_SOURCE_DIR}/Pong.cpp
# 				  ${PROJECT_SOURCE_DIR}/test.cpp)

# Add the executable
add_executable(Lab1 
    "code/main.cpp"
    "code/ECEBuzzy"
    "code/Laser"
    "code/ECEEnemy"
 )

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab1 PUBLIC sfml-graphics sfml-system sfml-window)

set_target_properties(
    Lab1 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)

# Copy the graphics folder to the output directory
add_custom_command(
    TARGET Lab1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Lab1/graphics $<TARGET_FILE_DIR:Lab1>/graphics
)

# Copy the fonts folder to the output directory
add_custom_command(
    TARGET Lab1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Lab1/fonts $<TARGET_FILE_DIR:Lab1>/fonts
)