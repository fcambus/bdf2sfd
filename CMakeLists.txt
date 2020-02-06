#
# bdftosfd
# Copyright (c) 2019-2020, Frederic Cambus
# https://github.com/fcambus/bdftosfd
#
# Created:      2019-11-21
# Last Updated: 2020-02-06
#
# bdftosfd is released under the BSD 2-Clause license
# See LICENSE file for details
#

cmake_minimum_required (VERSION 2.6)

project(bdftosfd C)

include(CheckFunctionExists)
include(GNUInstallDirs)

# Check if system has pledge and strtonum
list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_OPENBSD_SOURCE)
check_function_exists(pledge HAVE_PLEDGE)
check_function_exists(strtonum HAVE_STRTONUM)

# Additional include directories for compat functions + dependencies
include_directories("compat")

set(CMAKE_BUILD_TYPE Release)
set(SRC src/bdftosfd.c src/header.c src/parse.c)

if(NOT HAVE_PLEDGE)
  set (SRC ${SRC} compat/pledge.c)
endif()

if(NOT HAVE_STRTONUM)
  set (SRC ${SRC} compat/strtonum.c)
endif()

add_definitions(-Wall -Wextra -std=c99 -pedantic)
add_executable(bdftosfd ${SRC})

install(TARGETS bdftosfd DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES bdftosfd.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/)

enable_testing()
add_test(bdftosfd bdftosfd)
