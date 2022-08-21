	# /////////////////////////////////////////////////////////////////////////////////////////////
# // Copyright 2017 Intel Corporation
# //
# // Licensed under the Apache License, Version 2.0 (the "License");
# // you may not use this file except in compliance with the License.
# // You may obtain a copy of the License at
# //
# // http://www.apache.org/licenses/LICENSE-2.0
# //
# // Unless required by applicable law or agreed to in writing, software
# // distributed under the License is distributed on an "AS IS" BASIS,
# // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# // See the License for the specific language governing permissions and
# // limitations under the License.
# /////////////////////////////////////////////////////////////////////////////////////////////

cmake_minimum_required (VERSION 3.24.0)
project( "VulkanExamples" )

set_property( GLOBAL PROPERTY USE_FOLDERS ON ) #target property to organize targets into folders

if( CMAKE_BUILD_TYPE STREQUAL "" )
	set( CMAKE_BUILD_TYPE "Debug" )
endif()

if( NOT USE_PLATFORM MATCHES "VK_USE_PLATFORM_.*" )
	if( WIN32 )
		set( USE_PLATFORM "VK_USE_PLATFORM_WIN32_KHR" )
	endif()
endif()


add_definitions( -DVK_NO_PROTOTYPES )
add_definitions( -D${USE_PLATFORM} )
include_directories( "External" )
include_directories( "External/vulkan" )
include_directories( "Common" )

file( GLOB BASIC_SHARED_HEADER_FILES
		"Common/OS.h"
		"Common/VulkanFunctions.h"
		"Common/VulkanDefines.h" )

file( GLOB BASIC_SHARED_SOURCE_FILES
		"Common/OS.cpp"
		"Common/VulkanFunctions.cpp"
		"Common/VulkanDefines.cpp"  )

set( ALL_BASIC_SHARED_FILES
		${BASIC_SHARED_HEADER_FILES}
		${BASIC_SHARED_SOURCE_FILES})

file( GLOB ADVANCED_SHARED_HEADER_FILES
		"Common/Tools.h"
		"Common/VulkanCommon.h" )

file( GLOB ADVANCED_SHARED_SOURCE_FILES
		"Common/Tools.cpp"
		"Common/VulkanCommon.cpp" )

set( ALL_BASIC_AND_ADVANCED_SHARED_FILES
		${ALL_BASIC_SHARED_FILES}
		${ADVANCED_SHARED_HEADER_FILES}
		${ADVANCED_SHARED_SOURCE_FILES} )

source_group( "Header Files\\Common" FILES ${BASIC_SHARED_HEADER_FILES} ${ADVANCED_SHARED_HEADER_FILES} )
source_group( "Source Files\\Common" FILES ${BASIC_SHARED_SOURCE_FILES} ${ADVANCED_SHARED_SOURCE_FILES} )

add_executable( "Example_01" ${ALL_BASIC_SHARED_FILES}
	Examples/01/Example01.h
	Examples/01/main.cpp
	Examples/01/Example01.cpp )
	
add_executable( "Example_02" ${ALL_BASIC_SHARED_FILES}
	Examples/02/Example02.h
	Examples/02/main.cpp
	Examples/02/Example02.cpp )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Example_02)