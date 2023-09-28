cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(audio_playback_mic VERSION 1.0.0 LANGUAGES C)


add_library(audio_lib SHARED play.c play.def)
add_executable(audio_lib_exec play.c)



set_target_properties(audio_lib PROPERTIES
    PUBLIC_HEADER play.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "audio_lib"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Hex_Identity_ID_Goes_Here"
)