cmake_minimum_required(VERSION 3.10)

project(trax)

set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(trax 
    main.cpp 
    src/yt_dlp.cpp
    src/menu.cpp
    src/MetadataSearcher.cpp
    src/utils.cpp
    src/tageditor.cpp
    src/program.cpp
)

target_include_directories(trax PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/
    ${PROJECT_SOURCE_DIR}/headers/
    ${MUSICBRAINZ_INCLUDE_DIRS}
)

target_link_directories(trax PRIVATE /usr/local/lib)

target_link_libraries(trax PRIVATE
    musicbrainz5
    tag
    coverartcc
)

