# @Filename: Makefile-Settings.mk
# @Author:   Ben Sokol <Ben>
# @Email:    ben@bensokol.com
# @Created:  February 14th, 2019 [5:21pm]
# @Modified: March 29th, 2019 [10:03pm]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

###############################################################################
# Makefile Settings                                                           #
###############################################################################
EXE = reportGenerator

ifeq ($(shell hostname | head -c5),cycle)
CXXSTD = -std=c++1z
else
CXXSTD = -std=c++17
endif

CC = mpicc
CXX = mpic++
CCSTD = -std=c11

CFLAGS = -O3 -g -DNDEBUG
CXXFLAGS = -O3 -g -DNDEBUG
WARNING_FLAGS = -Wall -Wextra -Wpedantic
WARNING_FLAGS_GCC =
WARNING_FLAGS_CLANG =

MACOS_FLAGS =
LINUX_FLAGS = -pthread

# Disable install/uninstall, not needed.
MAKEFILE_RULE_INSTALL_DISABLE = 1
MAKEFILE_RULE_UNINSTALL_DISABLE = 1

ENABLE_LOGGING = 1

RUN_CMD = mpirun -np 4 ./$(EXE)
COMMAND_LINE_ARGS = Data3000.txt 2007 I

CPPCHECK_DEFINES = -D__cplusplus -DENABLE_LOGGING
CPPCHECK_SUPPRESS = --suppress=unmatchedSuppression --suppress=missingIncludeSystem --suppress=preprocessorErrorDirective:submodules/utilities/UTL_textWrap.cpp:17
CPPCHECK_FLAGS = -q --platform=native --error-exitcode=1 --force --std=c11 --std=c++11
CPPCHECK_ENABLE = --enable=warning --enable=style --enable=missingInclude
