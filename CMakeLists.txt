cmake_minimum_required(VERSION 3.10)
project(eudaimonia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_definitions(/MP)
endif()


include_directories(
    contrib/msvc15-windows-x86/include
    contrib/common
)

link_directories(
    ${CMAKE_SOURCE_DIR}/contrib/msvc15-windows-x86/lib
)



add_definitions(-DSDL_MAIN_HANDLED)

add_executable(eudaimonia
	src/zero/Core.h
	src/zero/Entity.h
	src/zero/Component.h
	src/zero/System.h
	src/zero/help/NonCopyable.h


	src/Transform.h
	src/Transform.cpp

	src/TransformSystem.h
	src/TransformSystem.cpp

	src/RenderingSystem.h
	src/RenderingSystem.cpp

	src/InputSystem.h
	src/InputSystem.cpp

	src/Resource.h
	src/ResourceSystem.h
	src/ModelRenderer.h
	src/ModelRenderer.cpp

	src/Mesh.h
	src/Mesh.cpp
	src/Model.h
	src/Model.cpp




	src/main.cpp
	src/Camera.h
	src/Shader.h
	src/Shader.cpp
	src/Texture.h
	src/Texture.cpp
	src/Time.h
	src/Time.cpp

	src/FrameBuffer.h
	src/FrameBuffer.cpp
	src/Skybox.h
	src/Skybox.cpp


)

target_link_libraries(eudaimonia
    SDL2 glew32 opengl32 assimp-vc141-mtd
)

add_custom_command(TARGET eudaimonia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc15-windows-x86/bin/SDL2.dll"
      "$<TARGET_FILE_DIR:eudaimonia>"
)
add_custom_command(TARGET eudaimonia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc15-windows-x86/bin/assimp-vc141-mtd.dll"
      "$<TARGET_FILE_DIR:eudaimonia>"
)

add_custom_command(TARGET eudaimonia POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc15-windows-x86/bin/glew32.dll"
      "$<TARGET_FILE_DIR:eudaimonia>"
)


