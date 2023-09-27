cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(audio_playback_mic VERSION 1.0.0 LANGUAGES C)


# Find PortAudio package
find_package(PortAudio REQUIRED)

add_library(audio_playback_mic SHARED aud.c aud.def)
add_executable(audio_play aud.c)


# Include PortAudio headers
target_include_directories(audio_playback_mic PRIVATE ${PortAudio_INCLUDE_DIRS})

# Link to PortAudio library
target_link_libraries(audio_playback_mic PRIVATE ${PortAudio_LIBRARIES})



set_target_properties(audio_playback_mic PROPERTIES
    PUBLIC_HEADER aud.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "audio_playback"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Hex_Identity_ID_Goes_Here"
)