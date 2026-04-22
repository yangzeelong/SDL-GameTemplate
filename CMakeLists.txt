cmake_minimum_required(VERSION 3.24)
project(SDL-GameTemplate)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ============================================
# SDL3 Configuration
# ============================================
set(SDL3_DIR "D:/Library/SDL3")
set(SDL3_INCLUDE_DIR "${SDL3_DIR}/include")
set(SDL3_LIB_DIR "${SDL3_DIR}/lib/x64")

# Find SDL3 components
find_package(SDL3 REQUIRED CONFIG PATHS "${SDL3_DIR}" NO_DEFAULT_PATH)
find_package(SDL3_image REQUIRED CONFIG PATHS "${SDL3_DIR}" NO_DEFAULT_PATH)
find_package(SDL3_mixer REQUIRED CONFIG PATHS "${SDL3_DIR}" NO_DEFAULT_PATH)
find_package(SDL3_ttf REQUIRED CONFIG PATHS "${SDL3_DIR}" NO_DEFAULT_PATH)

message(STATUS "SDL3 found: ${SDL3_INCLUDE_DIRS}")
message(STATUS "SDL3_LIBRARIES: ${SDL3_LIBRARIES}")

# ============================================
# Source Files
# ============================================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp" "src/**/*.cpp")

add_executable(SDL-GameTemplate ${SRC_FILES})

# Include directories
target_include_directories(SDL-GameTemplate PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Base
    ${SDL3_INCLUDE_DIR}
    ${SDL3_INCLUDE_DIRS}
)

# Link directories
target_link_directories(SDL-GameTemplate PRIVATE ${SDL3_LIB_DIR})

# Link libraries
target_link_libraries(SDL-GameTemplate PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
)

# ============================================
# Post Build: Copy SDL DLLs
# ============================================
set(DLLS_TO_COPY
    SDL3.dll
    SDL3_image.dll
    SDL3_mixer.dll
    SDL3_ttf.dll
)

foreach(dll ${DLLS_TO_COPY})
    add_custom_command(TARGET SDL-GameTemplate POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_LIB_DIR}/${dll}"
            $<TARGET_FILE_DIR:SDL-GameTemplate>
    )
endforeach()
