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
project(AutopatcherClientRestarter)
include_directories(${RAKNETHEADERFILES} ./) 
add_executable(AutopatcherClientRestarter "main.cpp")
target_link_libraries(AutopatcherClientRestarter ${RAKNET_COMMON_LIBS})
VSUBFOLDER(AutopatcherClientRestarter "Samples/AutoPatcher/Client")





