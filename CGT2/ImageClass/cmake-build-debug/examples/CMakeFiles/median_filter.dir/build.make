# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/henry/clion-2019.3.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/henry/clion-2019.3.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/henry/Projects/CGT2/ImageClass

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/henry/Projects/CGT2/ImageClass/cmake-build-debug

# Include any dependencies generated for this target.
include examples/CMakeFiles/median_filter.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/median_filter.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/median_filter.dir/flags.make

examples/CMakeFiles/median_filter.dir/median_filter.cpp.o: examples/CMakeFiles/median_filter.dir/flags.make
examples/CMakeFiles/median_filter.dir/median_filter.cpp.o: ../examples/median_filter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/Projects/CGT2/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/median_filter.dir/median_filter.cpp.o"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/median_filter.dir/median_filter.cpp.o -c /home/henry/Projects/CGT2/ImageClass/examples/median_filter.cpp

examples/CMakeFiles/median_filter.dir/median_filter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/median_filter.dir/median_filter.cpp.i"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/Projects/CGT2/ImageClass/examples/median_filter.cpp > CMakeFiles/median_filter.dir/median_filter.cpp.i

examples/CMakeFiles/median_filter.dir/median_filter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/median_filter.dir/median_filter.cpp.s"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/Projects/CGT2/ImageClass/examples/median_filter.cpp -o CMakeFiles/median_filter.dir/median_filter.cpp.s

# Object files for target median_filter
median_filter_OBJECTS = \
"CMakeFiles/median_filter.dir/median_filter.cpp.o"

# External object files for target median_filter
median_filter_EXTERNAL_OBJECTS =

examples/median_filter: examples/CMakeFiles/median_filter.dir/median_filter.cpp.o
examples/median_filter: examples/CMakeFiles/median_filter.dir/build.make
examples/median_filter: src/libutils.a
examples/median_filter: SOIL/libsoil.a
examples/median_filter: /usr/lib/x86_64-linux-gnu/libGL.so
examples/median_filter: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/median_filter: /usr/lib/x86_64-linux-gnu/libglut.so
examples/median_filter: /usr/lib/x86_64-linux-gnu/libjpeg.so
examples/median_filter: examples/CMakeFiles/median_filter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/Projects/CGT2/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable median_filter"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/median_filter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/median_filter.dir/build: examples/median_filter

.PHONY : examples/CMakeFiles/median_filter.dir/build

examples/CMakeFiles/median_filter.dir/clean:
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && $(CMAKE_COMMAND) -P CMakeFiles/median_filter.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/median_filter.dir/clean

examples/CMakeFiles/median_filter.dir/depend:
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/Projects/CGT2/ImageClass /home/henry/Projects/CGT2/ImageClass/examples /home/henry/Projects/CGT2/ImageClass/cmake-build-debug /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples/CMakeFiles/median_filter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/median_filter.dir/depend

