ez_cmake_init()

ez_build_filter_everything()

#ez_requires(EZ_3RDPARTY_OZZ_SUPPORT)

# Get the name of this folder as the project name
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

ez_create_target(LIBRARY ${PROJECT_NAME} NO_EZ_PREFIX)

target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

target_compile_definitions(${PROJECT_NAME} PRIVATE OZZ_BUILD_BASE_LIB)
target_compile_definitions(${PROJECT_NAME} PRIVATE OZZ_BUILD_ANIMATION_LIB)
target_compile_definitions(${PROJECT_NAME} PRIVATE OZZ_BUILD_GEOMETRY_LIB)
target_compile_definitions(${PROJECT_NAME} PRIVATE OZZ_BUILD_ANIMOFFLINE_LIB)
target_compile_definitions(${PROJECT_NAME} PRIVATE OZZ_BUILD_ANIMATIONTOOLS_LIB)

if(EZ_CMAKE_PLATFORM_WINDOWS)
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4661)
  target_compile_definitions(${PROJECT_NAME} PUBLIC OZZ_USE_DYNAMIC_LINKING)
endif()
