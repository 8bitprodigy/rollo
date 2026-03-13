CC      = gcc
EMCC    = emcc
AR      = ar
ARFLAGS = rcs

# Directories
SRC_DIR     = src
INC_DIR     = include
BUILD_DIR   = build
DEPS_DIR    = deps

# BTCHWRK
BTCHWRK_DIR    = $(DEPS_DIR)/BTCHWRK
BTCHWRK_LIB    = $(BTCHWRK_DIR)/build/libBTCHWRK.a
BTCHWRK_LIB_W  = $(BTCHWRK_DIR)/build-wasm/libBTCHWRK.a
BTCHWRK_INC    = $(BTCHWRK_DIR)/include

# Source subdirectories
SPINDLE_SRC = $(SRC_DIR)/spindle
COMMON_SRC  = $(SRC_DIR)/common

# Include flags
INCLUDES   = -I$(INC_DIR) -I$(BTCHWRK_INC)
INCLUDES_W = -I$(INC_DIR) -I$(BTCHWRK_INC)

# Compiler flags
CFLAGS_COMMON  = -std=c17 -Wall -Wextra -Wpedantic $(INCLUDES)
CFLAGS_DEBUG   = $(CFLAGS_COMMON) -g -O0 -DDEBUG
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2 -DNDEBUG
CFLAGS_WASM    = -std=c17 -Wall -Wextra -Wpedantic $(INCLUDES_W) \
                 -O2 -DNDEBUG -DSPINDLE_WASM \
                 -DSPINDLE_NO_TLS \
                 -DSPINDLE_NO_CONSOLE \
                 -DSPINDLE_NO_EDITOR \
                 -s WASM=1 -fPIC

# Output libraries
SPINDLE_LIB_DEBUG   = $(BUILD_DIR)/debug/libspindle.a
SPINDLE_LIB_RELEASE = $(BUILD_DIR)/release/libspindle.a
SPINDLE_LIB_WASM    = $(BUILD_DIR)/wasm/libspindle.a
COMMON_LIB_DEBUG    = $(BUILD_DIR)/debug/libcommon.a
COMMON_LIB_RELEASE  = $(BUILD_DIR)/release/libcommon.a
COMMON_LIB_WASM     = $(BUILD_DIR)/wasm/libcommon.a

# Source files
SPINDLE_SRCS = $(wildcard $(SPINDLE_SRC)/*.c)
COMMON_SRCS  = $(wildcard $(COMMON_SRC)/*.c)

# Object files
SPINDLE_OBJS_DEBUG   = $(patsubst $(SPINDLE_SRC)/%.c, $(BUILD_DIR)/debug/spindle/%.o,   $(SPINDLE_SRCS))
SPINDLE_OBJS_RELEASE = $(patsubst $(SPINDLE_SRC)/%.c, $(BUILD_DIR)/release/spindle/%.o, $(SPINDLE_SRCS))
SPINDLE_OBJS_WASM    = $(patsubst $(SPINDLE_SRC)/%.c, $(BUILD_DIR)/wasm/spindle/%.o,    $(SPINDLE_SRCS))
COMMON_OBJS_DEBUG    = $(patsubst $(COMMON_SRC)/%.c,  $(BUILD_DIR)/debug/common/%.o,    $(COMMON_SRCS))
COMMON_OBJS_RELEASE  = $(patsubst $(COMMON_SRC)/%.c,  $(BUILD_DIR)/release/common/%.o,  $(COMMON_SRCS))
COMMON_OBJS_WASM     = $(patsubst $(COMMON_SRC)/%.c,  $(BUILD_DIR)/wasm/common/%.o,     $(COMMON_SRCS))

# ─── Default target ───────────────────────────────────────────────────────────

.DEFAULT_GOAL := debug

# ─── BTCHWRK (native) ────────────────────────────────────────────────────────

$(BTCHWRK_LIB):
	git submodule update --init --recursive -- $(BTCHWRK_DIR)
	$(MAKE) -C $(BTCHWRK_DIR)

# ─── BTCHWRK (wasm) ──────────────────────────────────────────────────────────

$(BTCHWRK_LIB_W):
	git submodule update --init --recursive -- $(BTCHWRK_DIR)
	$(MAKE) -C $(BTCHWRK_DIR) wasm

deps: $(BTCHWRK_LIB)
deps-wasm: $(BTCHWRK_LIB_W)

# ─── Debug ────────────────────────────────────────────────────────────────────

debug: $(SPINDLE_LIB_DEBUG) $(COMMON_LIB_DEBUG)

$(BUILD_DIR)/debug/spindle/%.o: $(SPINDLE_SRC)/%.c | $(BTCHWRK_LIB)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(BUILD_DIR)/debug/common/%.o: $(COMMON_SRC)/%.c | $(BTCHWRK_LIB)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(SPINDLE_LIB_DEBUG): $(SPINDLE_OBJS_DEBUG)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(COMMON_LIB_DEBUG): $(COMMON_OBJS_DEBUG)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

# ─── Release ──────────────────────────────────────────────────────────────────

release: $(SPINDLE_LIB_RELEASE) $(COMMON_LIB_RELEASE)

$(BUILD_DIR)/release/spindle/%.o: $(SPINDLE_SRC)/%.c | $(BTCHWRK_LIB)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(BUILD_DIR)/release/common/%.o: $(COMMON_SRC)/%.c | $(BTCHWRK_LIB)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(SPINDLE_LIB_RELEASE): $(SPINDLE_OBJS_RELEASE)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(COMMON_LIB_RELEASE): $(COMMON_OBJS_RELEASE)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

# ─── Emscripten / WASM ────────────────────────────────────────────────────────
# Requires emscripten SDK active in environment (source emsdk_env.sh)
# TLS, console, and editor excluded via preprocessor flags.
# Networking handled by the browser via fetch/WebSocket.

wasm: $(SPINDLE_LIB_WASM) $(COMMON_LIB_WASM)

$(BUILD_DIR)/wasm/spindle/%.o: $(SPINDLE_SRC)/%.c | $(BTCHWRK_LIB_W)
	mkdir -p $(dir $@)
	$(EMCC) $(CFLAGS_WASM) -c $< -o $@

$(BUILD_DIR)/wasm/common/%.o: $(COMMON_SRC)/%.c | $(BTCHWRK_LIB_W)
	mkdir -p $(dir $@)
	$(EMCC) $(CFLAGS_WASM) -c $< -o $@

$(SPINDLE_LIB_WASM): $(SPINDLE_OBJS_WASM)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(COMMON_LIB_WASM): $(COMMON_OBJS_WASM)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

# ─── Clean ────────────────────────────────────────────────────────────────────

clean:
	rm -rf $(BUILD_DIR)

clean-deps:
	rm -rf $(BTCHWRK_DIR)/build $(BTCHWRK_DIR)/build-wasm

clean-all: clean clean-deps

# ─── Phony ────────────────────────────────────────────────────────────────────

.PHONY: debug release wasm deps deps-wasm clean clean-deps clean-all
