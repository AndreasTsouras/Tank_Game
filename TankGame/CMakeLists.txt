cmake_minimum_required(VERSION 3.27)
project(TankGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

# Source files
set(SOURCE_FILES
        main.cpp
        EBO.cpp
        VBO.cpp
        VAO.cpp
        header_files/Shader.cpp
        header_files/Vector.cpp
        header_files/Mesh.cpp
        header_files/Matrix.cpp
        header_files/SphericalCameraManipulator.cpp
        header_files/Texture.cpp
        glad/src/glad.c
)

# Header files
set(HEADER_FILES
        header_files/Shader.h
        header_files/Vector.h
        header_files/Mesh.h
        header_files/Matrix.h
        header_files/SphericalCameraManipulator.h
        header_files/Texture.h
        header_files/stb_truetype.h
        VBO.h
        VBO.cpp
        VAO.h
        VAO.cpp
        EBO.h
        EBO.cpp
        glad/glad.h
)

add_executable(TankGame ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(TankGame PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# Absolute source dir for runtime asset lookups.
target_compile_definitions(TankGame PRIVATE
        TANKGAME_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

# Link against the required libraries
target_link_libraries(TankGame PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT)
