# CC := gcc

# OUT_DIR := out
# SRC_DIR := src
# OBJ_ROOT := obj
# LIB_DIR := lib
# RAYLIB_SRC := $(LIB_DIR)/raylib-5.5/src

# TARGET_DIR := out

# CONFIG ?= release

# CFLAGS_COMMON := -Wall -Wextra -std=c99 -I$(RAYLIB_SRC)
# LDLIBS_COMMON := -lGL -lm -lpthread -ldl -lrt -lX11

# ifeq ($(CONFIG),debug)
#     OBJ_DIR := $(OBJ_ROOT)/debug
#     TARGET_NAME := $(TARGET_DIR)/debug
#     CFLAGS_CONFIG := -g -O0
#     RAYLIB_LIB := $(LIB_DIR)/dbg/libraylib.a
# else ifeq ($(CONFIG),release)
#     OBJ_DIR := $(OBJ_ROOT)/release
#     TARGET_NAME := $(TARGET_DIR)/release
#     CFLAGS_CONFIG := -O2 -DNDEBUG
#     RAYLIB_LIB := $(LIB_DIR)/rel/libraylib.a
# else
#     $(error CONFIG must be 'debug' or 'release')
# endif

# CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_CONFIG)

# SRCS := $(shell find $(SRC_DIR) -name '*.c')
# OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# .PHONY: debug release clean

# debug:
# 	$(MAKE) CONFIG=debug

# release:
# 	$(MAKE) CONFIG=release

# $(TARGET_NAME): $(OBJS) $(RAYLIB_LIB)
# 	$(CC) $(OBJS) -o $@ $(RAYLIB_LIB) $(LDLIBS_COMMON)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

# run: $(TARGET_NAME)
# 	./$(TARGET_NAME)

# clean:
# 	rm -rf $(OBJ_ROOT) $(OUT_DIR)
CC := gcc

OUT_DIR := out
SRC_DIR := src
OBJ_ROOT := obj
LIB_DIR := lib
RAYLIB_SRC := $(LIB_DIR)/raylib-5.5/src

DEBUG_TARGET := $(OUT_DIR)/debug
RELEASE_TARGET := $(OUT_DIR)/release

DEBUG_OBJ_DIR := $(OBJ_ROOT)/debug
RELEASE_OBJ_DIR := $(OBJ_ROOT)/release

CFLAGS_COMMON := -Wall -Wextra -std=c99 -I$(RAYLIB_SRC)
LDLIBS_COMMON := -lGL -lm -lpthread -ldl -lrt -lX11

DEBUG_CFLAGS := -g -O0
RELEASE_CFLAGS := -O2 -DNDEBUG

DEBUG_RAYLIB_LIB := $(LIB_DIR)/dbg/libraylib.a
RELEASE_RAYLIB_LIB := $(LIB_DIR)/rel/libraylib.a

SRCS := $(shell find $(SRC_DIR) -name '*.c')

DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.c,$(DEBUG_OBJ_DIR)/%.o,$(SRCS))
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.c,$(RELEASE_OBJ_DIR)/%.o,$(SRCS))

.PHONY: debug release run-debug run-release clean

debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJS) $(DEBUG_RAYLIB_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(DEBUG_OBJS) -o $@ $(DEBUG_RAYLIB_LIB) $(LDLIBS_COMMON)

$(RELEASE_TARGET): $(RELEASE_OBJS) $(RELEASE_RAYLIB_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(RELEASE_OBJS) -o $@ $(RELEASE_RAYLIB_LIB) $(LDLIBS_COMMON)

$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_COMMON) $(DEBUG_CFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_COMMON) $(RELEASE_CFLAGS) -c $< -o $@

run-debug: $(DEBUG_TARGET)
	./$(DEBUG_TARGET)

run-release: $(RELEASE_TARGET)
	./$(RELEASE_TARGET)

clean:
	rm -rf $(OBJ_ROOT) $(OUT_DIR)