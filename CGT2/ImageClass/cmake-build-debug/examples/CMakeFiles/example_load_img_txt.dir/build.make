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
include examples/CMakeFiles/example_load_img_txt.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/example_load_img_txt.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/example_load_img_txt.dir/flags.make

examples/CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.o: examples/CMakeFiles/example_load_img_txt.dir/flags.make
examples/CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.o: ../examples/example_load_img_txt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/Projects/CGT2/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.o"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.o -c /home/henry/Projects/CGT2/ImageClass/examples/example_load_img_txt.cpp

examples/CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.i"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/Projects/CGT2/ImageClass/examples/example_load_img_txt.cpp > CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.i

examples/CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.s"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/Projects/CGT2/ImageClass/examples/example_load_img_txt.cpp -o CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.s

# Object files for target example_load_img_txt
example_load_img_txt_OBJECTS = \
"CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.o"

# External object files for target example_load_img_txt
example_load_img_txt_EXTERNAL_OBJECTS =

examples/example_load_img_txt: examples/CMakeFiles/example_load_img_txt.dir/example_load_img_txt.cpp.o
examples/example_load_img_txt: examples/CMakeFiles/example_load_img_txt.dir/build.make
examples/example_load_img_txt: src/libutils.a
examples/example_load_img_txt: SOIL/libsoil.a
examples/example_load_img_txt: /usr/lib/x86_64-linux-gnu/libGL.so
examples/example_load_img_txt: /usr/lib/x86_64-linux-gnu/libGLU.so
examples/example_load_img_txt: /usr/lib/x86_64-linux-gnu/libglut.so
examples/example_load_img_txt: /usr/lib/x86_64-linux-gnu/libjpeg.so
examples/example_load_img_txt: examples/CMakeFiles/example_load_img_txt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/Projects/CGT2/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example_load_img_txt"
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example_load_img_txt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/example_load_img_txt.dir/build: examples/example_load_img_txt

.PHONY : examples/CMakeFiles/example_load_img_txt.dir/build

examples/CMakeFiles/example_load_img_txt.dir/clean:
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples && $(CMAKE_COMMAND) -P CMakeFiles/example_load_img_txt.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/example_load_img_txt.dir/clean

examples/CMakeFiles/example_load_img_txt.dir/depend:
	cd /home/henry/Projects/CGT2/ImageClass/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/Projects/CGT2/ImageClass /home/henry/Projects/CGT2/ImageClass/examples /home/henry/Projects/CGT2/ImageClass/cmake-build-debug /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples /home/henry/Projects/CGT2/ImageClass/cmake-build-debug/examples/CMakeFiles/example_load_img_txt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/example_load_img_txt.dir/depend

