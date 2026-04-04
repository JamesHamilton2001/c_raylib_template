CC := gcc
TARGET_BASE := c_raylib_template

SRC_DIR := src
OBJ_ROOT := obj
LIB_DIR := lib
RAYLIB_DIR := $(LIB_DIR)/raylib
RAYLIB_SRC := $(RAYLIB_DIR)/src

CONFIG ?= release

CFLAGS_COMMON := -Wall -Wextra -std=c99 -I$(RAYLIB_SRC)
LDLIBS_COMMON := -lGL -lm -lpthread -ldl -lrt -lX11

ifeq ($(CONFIG),debug)
    OBJ_DIR := $(OBJ_ROOT)/debug
    TARGET_NAME := $(TARGET_BASE)_debug
    CFLAGS_CONFIG := -g -O0
    RAYLIB_LIB := $(RAYLIB_DIR)/debug/libraylib.a
else ifeq ($(CONFIG),release)
    OBJ_DIR := $(OBJ_ROOT)/release
    TARGET_NAME := $(TARGET_BASE)_release
    CFLAGS_CONFIG := -O2 -DNDEBUG
    RAYLIB_LIB := $(RAYLIB_DIR)/release/libraylib.a
else
    $(error CONFIG must be 'debug' or 'release')
endif

CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_CONFIG)

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all debug release run clean rebuild raylib_install raylib_uninstall

all: $(TARGET_NAME)

debug:
	$(MAKE) CONFIG=debug

release:
	$(MAKE) CONFIG=release

$(TARGET_NAME): $(OBJS) $(RAYLIB_LIB)
	$(CC) $(OBJS) -o $@ $(RAYLIB_LIB) $(LDLIBS_COMMON)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET_NAME)
	./$(TARGET_NAME)

rebuild: clean all

clean:
	rm -rf $(OBJ_ROOT) $(TARGET_BASE)_debug $(TARGET_BASE)_release

raylib_install:
	./setup/install_local_raylib.sh

raylib_uninstall:
	./setup/uninstall_local_raylib.sh