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
CMAKE_COMMAND = /opt/clion-2019.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lisek/Dokumenty/polibuda/sk2/http-sever

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/http_sever.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/http_sever.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/http_sever.dir/flags.make

CMakeFiles/http_sever.dir/main.cpp.o: CMakeFiles/http_sever.dir/flags.make
CMakeFiles/http_sever.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/http_sever.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http_sever.dir/main.cpp.o -c /home/lisek/Dokumenty/polibuda/sk2/http-sever/main.cpp

CMakeFiles/http_sever.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_sever.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lisek/Dokumenty/polibuda/sk2/http-sever/main.cpp > CMakeFiles/http_sever.dir/main.cpp.i

CMakeFiles/http_sever.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_sever.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lisek/Dokumenty/polibuda/sk2/http-sever/main.cpp -o CMakeFiles/http_sever.dir/main.cpp.s

CMakeFiles/http_sever.dir/Server.cpp.o: CMakeFiles/http_sever.dir/flags.make
CMakeFiles/http_sever.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/http_sever.dir/Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http_sever.dir/Server.cpp.o -c /home/lisek/Dokumenty/polibuda/sk2/http-sever/Server.cpp

CMakeFiles/http_sever.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_sever.dir/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lisek/Dokumenty/polibuda/sk2/http-sever/Server.cpp > CMakeFiles/http_sever.dir/Server.cpp.i

CMakeFiles/http_sever.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_sever.dir/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lisek/Dokumenty/polibuda/sk2/http-sever/Server.cpp -o CMakeFiles/http_sever.dir/Server.cpp.s

CMakeFiles/http_sever.dir/HttpRequest.cpp.o: CMakeFiles/http_sever.dir/flags.make
CMakeFiles/http_sever.dir/HttpRequest.cpp.o: ../HttpRequest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/http_sever.dir/HttpRequest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http_sever.dir/HttpRequest.cpp.o -c /home/lisek/Dokumenty/polibuda/sk2/http-sever/HttpRequest.cpp

CMakeFiles/http_sever.dir/HttpRequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_sever.dir/HttpRequest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lisek/Dokumenty/polibuda/sk2/http-sever/HttpRequest.cpp > CMakeFiles/http_sever.dir/HttpRequest.cpp.i

CMakeFiles/http_sever.dir/HttpRequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_sever.dir/HttpRequest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lisek/Dokumenty/polibuda/sk2/http-sever/HttpRequest.cpp -o CMakeFiles/http_sever.dir/HttpRequest.cpp.s

CMakeFiles/http_sever.dir/HttpResponse.cpp.o: CMakeFiles/http_sever.dir/flags.make
CMakeFiles/http_sever.dir/HttpResponse.cpp.o: ../HttpResponse.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/http_sever.dir/HttpResponse.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http_sever.dir/HttpResponse.cpp.o -c /home/lisek/Dokumenty/polibuda/sk2/http-sever/HttpResponse.cpp

CMakeFiles/http_sever.dir/HttpResponse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_sever.dir/HttpResponse.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lisek/Dokumenty/polibuda/sk2/http-sever/HttpResponse.cpp > CMakeFiles/http_sever.dir/HttpResponse.cpp.i

CMakeFiles/http_sever.dir/HttpResponse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_sever.dir/HttpResponse.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lisek/Dokumenty/polibuda/sk2/http-sever/HttpResponse.cpp -o CMakeFiles/http_sever.dir/HttpResponse.cpp.s

# Object files for target http_sever
http_sever_OBJECTS = \
"CMakeFiles/http_sever.dir/main.cpp.o" \
"CMakeFiles/http_sever.dir/Server.cpp.o" \
"CMakeFiles/http_sever.dir/HttpRequest.cpp.o" \
"CMakeFiles/http_sever.dir/HttpResponse.cpp.o"

# External object files for target http_sever
http_sever_EXTERNAL_OBJECTS =

http_sever: CMakeFiles/http_sever.dir/main.cpp.o
http_sever: CMakeFiles/http_sever.dir/Server.cpp.o
http_sever: CMakeFiles/http_sever.dir/HttpRequest.cpp.o
http_sever: CMakeFiles/http_sever.dir/HttpResponse.cpp.o
http_sever: CMakeFiles/http_sever.dir/build.make
http_sever: ../boost/lib/libboost_filesystem.a
http_sever: ../boost/lib/libboost_system.a
http_sever: ../boost/lib/libboost_program_options.a
http_sever: ../boost/lib/libboost_iostreams.a
http_sever: CMakeFiles/http_sever.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable http_sever"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/http_sever.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/http_sever.dir/build: http_sever

.PHONY : CMakeFiles/http_sever.dir/build

CMakeFiles/http_sever.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/http_sever.dir/cmake_clean.cmake
.PHONY : CMakeFiles/http_sever.dir/clean

CMakeFiles/http_sever.dir/depend:
	cd /home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lisek/Dokumenty/polibuda/sk2/http-sever /home/lisek/Dokumenty/polibuda/sk2/http-sever /home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug /home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug /home/lisek/Dokumenty/polibuda/sk2/http-sever/cmake-build-debug/CMakeFiles/http_sever.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/http_sever.dir/depend

