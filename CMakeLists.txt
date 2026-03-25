cmake_minimum_required(VERSION 3.10)

project(trax)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_library(MUSICBRAINZ5_LIB NAMES musicbrainz5cc musicbrainz5 REQUIRED)
find_library(TAG_LIB NAMES tag REQUIRED)
find_library(COVERART_LIB NAMES coverartcc coverart PATHS /usr/local/lib REQUIRED)

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
)


target_link_libraries(trax PRIVATE
    ${MUSICBRAINZ5_LIB}
    ${TAG_LIB}
    ${COVERART_LIB}
)

