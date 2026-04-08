CC := gcc

OUT_DIR := out
SRC_DIR := src
OBJ_ROOT := obj
LIB_DIR := lib

RAYLIB_VERSION := $(strip $(shell cat $(LIB_DIR)/raylib.version))
RAYGUI_VERSION := $(strip $(shell cat $(LIB_DIR)/raygui.version))

RAYLIB_SRC := $(LIB_DIR)/src/raylib-$(RAYLIB_VERSION)/src
RAYGUI_SRC := $(LIB_DIR)/src/raygui-$(RAYGUI_VERSION)/src

DEBUG_TARGET := $(OUT_DIR)/debug
RELEASE_TARGET := $(OUT_DIR)/release

DEBUG_OBJ_DIR := $(OBJ_ROOT)/debug
RELEASE_OBJ_DIR := $(OBJ_ROOT)/release

CFLAGS_COMMON := -Wall -Wextra -std=c99 -I$(RAYLIB_SRC) -I$(RAYGUI_SRC)
LDLIBS_COMMON := -lGL -lm -lpthread -ldl -lrt -lX11

DEBUG_CFLAGS := -g -O0
RELEASE_CFLAGS := -O2 -DNDEBUG

DEBUG_RAYLIB_LIB := $(LIB_DIR)/dbg/libraylib.a
RELEASE_RAYLIB_LIB := $(LIB_DIR)/rel/libraylib.a

DEBUG_RAYGUI_LIB := $(LIB_DIR)/dbg/libraygui.a
RELEASE_RAYGUI_LIB := $(LIB_DIR)/rel/libraygui.a

SRCS := $(shell find $(SRC_DIR) -name '*.c')

DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.c,$(DEBUG_OBJ_DIR)/%.o,$(SRCS))
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.c,$(RELEASE_OBJ_DIR)/%.o,$(SRCS))

.PHONY: debug release run-debug run-release clean

debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJS) $(DEBUG_RAYLIB_LIB) $(DEBUG_RAYGUI_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(DEBUG_OBJS) -o $@ $(DEBUG_RAYGUI_LIB) $(DEBUG_RAYLIB_LIB) $(LDLIBS_COMMON)

$(RELEASE_TARGET): $(RELEASE_OBJS) $(RELEASE_RAYLIB_LIB) $(RELEASE_RAYGUI_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(RELEASE_OBJS) -o $@ $(RELEASE_RAYGUI_LIB) $(RELEASE_RAYLIB_LIB) $(LDLIBS_COMMON)

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