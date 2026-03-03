cmake_minimum_required(VERSION 3.27)
project(TankGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)

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

# Link against the required libraries
target_link_libraries(TankGame PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT)

set(ASSET_SHADER_FILES
        shader.vert
        shader.frag
        cubemap.vert
        cubemap.frag
)

set(ASSET_TEXTURE_FILES
        space.jpg
        space2.jpg
        space3.jpg
        space4.jpg
        space5.jpg
        sky.jpg
        panorama.jpg
        volcano.jpeg
)

set(ASSET_DIR_FOLDERS
        models
        maps
        skybox
)

# Keep local build outputs runnable by mirroring assets beside the executable.
add_custom_command(TARGET TankGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:TankGame>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASSET_SHADER_FILES}
        $<TARGET_FILE_DIR:TankGame>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASSET_TEXTURE_FILES}
        $<TARGET_FILE_DIR:TankGame>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        $<TARGET_FILE_DIR:TankGame>/assets/models
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/maps
        $<TARGET_FILE_DIR:TankGame>/assets/maps
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/skybox
        $<TARGET_FILE_DIR:TankGame>/assets/skybox
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS TankGame RUNTIME DESTINATION .)

install(FILES
        ${ASSET_SHADER_FILES}
        DESTINATION assets
)

install(DIRECTORY
        ${ASSET_DIR_FOLDERS}
        DESTINATION assets
)

install(FILES
        ${ASSET_TEXTURE_FILES}
        DESTINATION assets
)

if (WIN32)
    install(FILES $<TARGET_RUNTIME_DLLS:TankGame> DESTINATION .)
endif()
