# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.26.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.26.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/winner/Documents/Source/noise-cancel/audio_lib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/winner/Documents/Source/noise-cancel/audio_lib

# Include any dependencies generated for this target.
include CMakeFiles/audio_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/audio_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/audio_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/audio_lib.dir/flags.make

CMakeFiles/audio_lib.dir/play.c.o: CMakeFiles/audio_lib.dir/flags.make
CMakeFiles/audio_lib.dir/play.c.o: play.c
CMakeFiles/audio_lib.dir/play.c.o: CMakeFiles/audio_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/winner/Documents/Source/noise-cancel/audio_lib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/audio_lib.dir/play.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/audio_lib.dir/play.c.o -MF CMakeFiles/audio_lib.dir/play.c.o.d -o CMakeFiles/audio_lib.dir/play.c.o -c /Users/winner/Documents/Source/noise-cancel/audio_lib/play.c

CMakeFiles/audio_lib.dir/play.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/audio_lib.dir/play.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/winner/Documents/Source/noise-cancel/audio_lib/play.c > CMakeFiles/audio_lib.dir/play.c.i

CMakeFiles/audio_lib.dir/play.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/audio_lib.dir/play.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/winner/Documents/Source/noise-cancel/audio_lib/play.c -o CMakeFiles/audio_lib.dir/play.c.s

# Object files for target audio_lib
audio_lib_OBJECTS = \
"CMakeFiles/audio_lib.dir/play.c.o"

# External object files for target audio_lib
audio_lib_EXTERNAL_OBJECTS =

libaudio_lib.1.0.0.dylib: CMakeFiles/audio_lib.dir/play.c.o
libaudio_lib.1.0.0.dylib: CMakeFiles/audio_lib.dir/build.make
libaudio_lib.1.0.0.dylib: play.def
libaudio_lib.1.0.0.dylib: CMakeFiles/audio_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/winner/Documents/Source/noise-cancel/audio_lib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libaudio_lib.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/audio_lib.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_library libaudio_lib.1.0.0.dylib libaudio_lib.1.dylib libaudio_lib.dylib

libaudio_lib.1.dylib: libaudio_lib.1.0.0.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate libaudio_lib.1.dylib

libaudio_lib.dylib: libaudio_lib.1.0.0.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate libaudio_lib.dylib

# Rule to build all files generated by this target.
CMakeFiles/audio_lib.dir/build: libaudio_lib.dylib
.PHONY : CMakeFiles/audio_lib.dir/build

CMakeFiles/audio_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/audio_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/audio_lib.dir/clean

CMakeFiles/audio_lib.dir/depend:
	cd /Users/winner/Documents/Source/noise-cancel/audio_lib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/winner/Documents/Source/noise-cancel/audio_lib /Users/winner/Documents/Source/noise-cancel/audio_lib /Users/winner/Documents/Source/noise-cancel/audio_lib /Users/winner/Documents/Source/noise-cancel/audio_lib /Users/winner/Documents/Source/noise-cancel/audio_lib/CMakeFiles/audio_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/audio_lib.dir/depend
