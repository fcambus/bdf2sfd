#
# bdf2sfd 1.1.6
# Copyright (c) 2019-2021, Frederic Cambus
# https://github.com/fcambus/bdf2sfd
#
# Created:      2019-11-21
# Last Updated: 2021-04-04
#
# bdf2sfd is released under the BSD 2-Clause license.
# See LICENSE file for details.
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 2.8.12)

project(bdf2sfd C)

include(CheckFunctionExists)
include(GNUInstallDirs)

# Conditional build options
set(ENABLE_SECCOMP 0 CACHE BOOL "Enable building with seccomp")

# Check if system has pledge and strtonum
list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_OPENBSD_SOURCE)
check_function_exists(pledge HAVE_PLEDGE)
check_function_exists(strtonum HAVE_STRTONUM)

if(ENABLE_SECCOMP)
  # Check if system has seccomp
  message(STATUS "Looking for seccomp")
  find_path(SECCOMP NAMES "linux/seccomp.h")
  if(SECCOMP)
    message(STATUS "Looking for seccomp - found")
    add_definitions(-DHAVE_SECCOMP)
  else()
    message(STATUS "Looking for seccomp - not found")
  endif()
endif(ENABLE_SECCOMP)

# Additional include directories for compat functions + dependencies
include_directories("compat")

set(SRC src/bdf2sfd.c src/header.c src/parse.c src/polygon.c)

if(NOT HAVE_PLEDGE)
  set(SRC ${SRC} compat/pledge.c)
endif()

if(NOT HAVE_STRTONUM)
  set(SRC ${SRC} compat/strtonum.c)
endif()

add_definitions(-D_GNU_SOURCE -Wall -Wextra -std=c99 -pedantic)
add_executable(bdf2sfd ${SRC})

install(TARGETS bdf2sfd DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES bdf2sfd.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/)

enable_testing()
add_test(bdf2sfd bdf2sfd)
add_test(8x16 bdf2sfd ${PROJECT_SOURCE_DIR}/tests/spleen-8x16.bdf)
add_test(12x24 bdf2sfd ${PROJECT_SOURCE_DIR}/tests/spleen-12x24.bdf)
add_test(16x32 bdf2sfd ${PROJECT_SOURCE_DIR}/tests/spleen-16x32.bdf)
add_test(32x64 bdf2sfd ${PROJECT_SOURCE_DIR}/tests/spleen-32x64.bdf)
