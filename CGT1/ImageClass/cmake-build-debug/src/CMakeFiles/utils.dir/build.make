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
CMAKE_SOURCE_DIR = /home/henry/Projects/T1CG/ImageClass

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/henry/Projects/T1CG/ImageClass/cmake-build-debug

# Include any dependencies generated for this target.
include src/CMakeFiles/utils.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/utils.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/utils.dir/flags.make

src/CMakeFiles/utils.dir/ImageClass.cpp.o: src/CMakeFiles/utils.dir/flags.make
src/CMakeFiles/utils.dir/ImageClass.cpp.o: ../src/ImageClass.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/Projects/T1CG/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/utils.dir/ImageClass.cpp.o"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utils.dir/ImageClass.cpp.o -c /home/henry/Projects/T1CG/ImageClass/src/ImageClass.cpp

src/CMakeFiles/utils.dir/ImageClass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utils.dir/ImageClass.cpp.i"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/Projects/T1CG/ImageClass/src/ImageClass.cpp > CMakeFiles/utils.dir/ImageClass.cpp.i

src/CMakeFiles/utils.dir/ImageClass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utils.dir/ImageClass.cpp.s"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/Projects/T1CG/ImageClass/src/ImageClass.cpp -o CMakeFiles/utils.dir/ImageClass.cpp.s

src/CMakeFiles/utils.dir/Temporizador.cpp.o: src/CMakeFiles/utils.dir/flags.make
src/CMakeFiles/utils.dir/Temporizador.cpp.o: ../src/Temporizador.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/Projects/T1CG/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/utils.dir/Temporizador.cpp.o"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utils.dir/Temporizador.cpp.o -c /home/henry/Projects/T1CG/ImageClass/src/Temporizador.cpp

src/CMakeFiles/utils.dir/Temporizador.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utils.dir/Temporizador.cpp.i"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/Projects/T1CG/ImageClass/src/Temporizador.cpp > CMakeFiles/utils.dir/Temporizador.cpp.i

src/CMakeFiles/utils.dir/Temporizador.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utils.dir/Temporizador.cpp.s"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/Projects/T1CG/ImageClass/src/Temporizador.cpp -o CMakeFiles/utils.dir/Temporizador.cpp.s

src/CMakeFiles/utils.dir/Texture.cpp.o: src/CMakeFiles/utils.dir/flags.make
src/CMakeFiles/utils.dir/Texture.cpp.o: ../src/Texture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/Projects/T1CG/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/utils.dir/Texture.cpp.o"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utils.dir/Texture.cpp.o -c /home/henry/Projects/T1CG/ImageClass/src/Texture.cpp

src/CMakeFiles/utils.dir/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utils.dir/Texture.cpp.i"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/Projects/T1CG/ImageClass/src/Texture.cpp > CMakeFiles/utils.dir/Texture.cpp.i

src/CMakeFiles/utils.dir/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utils.dir/Texture.cpp.s"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/Projects/T1CG/ImageClass/src/Texture.cpp -o CMakeFiles/utils.dir/Texture.cpp.s

src/CMakeFiles/utils.dir/TextureClass.cpp.o: src/CMakeFiles/utils.dir/flags.make
src/CMakeFiles/utils.dir/TextureClass.cpp.o: ../src/TextureClass.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/henry/Projects/T1CG/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/utils.dir/TextureClass.cpp.o"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utils.dir/TextureClass.cpp.o -c /home/henry/Projects/T1CG/ImageClass/src/TextureClass.cpp

src/CMakeFiles/utils.dir/TextureClass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utils.dir/TextureClass.cpp.i"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/henry/Projects/T1CG/ImageClass/src/TextureClass.cpp > CMakeFiles/utils.dir/TextureClass.cpp.i

src/CMakeFiles/utils.dir/TextureClass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utils.dir/TextureClass.cpp.s"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/henry/Projects/T1CG/ImageClass/src/TextureClass.cpp -o CMakeFiles/utils.dir/TextureClass.cpp.s

# Object files for target utils
utils_OBJECTS = \
"CMakeFiles/utils.dir/ImageClass.cpp.o" \
"CMakeFiles/utils.dir/Temporizador.cpp.o" \
"CMakeFiles/utils.dir/Texture.cpp.o" \
"CMakeFiles/utils.dir/TextureClass.cpp.o"

# External object files for target utils
utils_EXTERNAL_OBJECTS =

src/libutils.a: src/CMakeFiles/utils.dir/ImageClass.cpp.o
src/libutils.a: src/CMakeFiles/utils.dir/Temporizador.cpp.o
src/libutils.a: src/CMakeFiles/utils.dir/Texture.cpp.o
src/libutils.a: src/CMakeFiles/utils.dir/TextureClass.cpp.o
src/libutils.a: src/CMakeFiles/utils.dir/build.make
src/libutils.a: src/CMakeFiles/utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/henry/Projects/T1CG/ImageClass/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libutils.a"
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean_target.cmake
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/utils.dir/build: src/libutils.a

.PHONY : src/CMakeFiles/utils.dir/build

src/CMakeFiles/utils.dir/clean:
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/utils.dir/clean

src/CMakeFiles/utils.dir/depend:
	cd /home/henry/Projects/T1CG/ImageClass/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/henry/Projects/T1CG/ImageClass /home/henry/Projects/T1CG/ImageClass/src /home/henry/Projects/T1CG/ImageClass/cmake-build-debug /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src /home/henry/Projects/T1CG/ImageClass/cmake-build-debug/src/CMakeFiles/utils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/utils.dir/depend

