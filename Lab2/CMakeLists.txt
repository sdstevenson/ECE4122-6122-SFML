# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab2 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)

# Add the executable
add_executable(Lab2 
   "code/main.cpp"
   "code/threadPool.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)
link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Find OpenMP and link its imported target
#find_package(OpenMP REQUIRED)

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab2 PUBLIC 
    sfml-graphics 
    sfml-system 
    sfml-window
#    OpenMp::OpenMP_CXX
)

set_target_properties(
    Lab2 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)
