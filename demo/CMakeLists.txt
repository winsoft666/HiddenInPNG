##################################################################################
#    Hidden anything into PNG file and NOT affect original image.
#    ---------------------------------------------------------------------------
#    Copyright (C) 2022 JiangXueqiao <winsoft666@outlook.com>.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
##################################################################################

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

project(demo CXX)

set (CMAKE_CXX_STANDARD 14)

set(EXE_NAME demo)

add_definitions(-DUNICODE -D_UNICODE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB HIDEEN_IN_PNG_FILES ../hidden_in_png.h ../hidden_in_png.cpp ../lodepng/*.h ../lodepng/*.cpp)
file(GLOB DEMO_SOURCE_FILES ./*.cpp)

source_group("HiddenInPngFiles" FILES ${HIDEEN_IN_PNG_FILES})
source_group("demo" FILES ${DEMO_SOURCE_FILES})

include_directories(../jhc/include)

add_executable(
	${EXE_NAME}
	${HIDEEN_IN_PNG_FILES}
	${DEMO_SOURCE_FILES}
	)
	
# Win32 Console
if (WIN32 OR _WIN32)
	set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
	set_target_properties(${EXE_NAME} PROPERTIES COMPILE_DEFINITIONS "_CONSOLE")
endif()
