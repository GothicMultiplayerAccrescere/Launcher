#
# This file was taken from RakNet 4.082.
# Please see licenses/RakNet license.txt for the underlying license and related copyright.
#
#
# Modified work: Copyright (c) 2017, SLikeSoft UG (haftungsbeschränkt)
#
# This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
# license found in the license.txt file in the root directory of this source tree.
#

cmake_minimum_required(VERSION 2.6)

set( RAKNET_INTERNAL_INCLUDE_DIRS 
	${RakNet_SOURCE_DIR}/Source/include 
	${RakNet_SOURCE_DIR}/DependentExtensions/openssl-1.0.0d/include 
	${RakNet_SOURCE_DIR}/DependentExtensions
)

if( RAKNET_ENABLE_STATIC )
	add_subdirectory(LibStatic)
endif()


#if( RAKNET_ENABLE_DLL )
#	add_subdirectory(DLL)
#endif()
