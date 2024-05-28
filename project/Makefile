
CC 		 := riscv64-unknown-linux-gnu-gcc
QEMU_USER ?= qemu-riscv64
CFLAGS 	 ?=
RUNFLAGS ?= LD_PRELOAD=$PWD/inject.so
QEMU_FLAGS = -L /opt/sc-dt/riscv-gcc/sysroot/

TARGET := app

BUILD_DIR := ./build
SRC_DIR	  := ./src
OBJ_DIR   := ./obj
LIB_DIR   := ./lib

    SRC = $(wildcard $(SRC_DIR)/*.c)
	LIB = $(wildcard $(LIB_DIR)/*.c)
OBJ_SRC = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ_LIB = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB))

all:  $(TARGET)

.PHONY: _make_dirs clean

_make_dirs:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${OBJ_DIR}

run: $(TARGET)
	$(BUILD_DIR)/$(TARGET)

run-preload: $(TARGET) hook
	LD_PRELOAD=$(PWD)/$(OBJ_DIR)/hook.so $(BUILD_DIR)/$(TARGET)


$(TARGET) : $(OBJ_SRC) 
	$(CC) $(CFLAGS) $(OBJ_SRC) -o $(BUILD_DIR)/$(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)/%.o : $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

hook: $(OBJ_LIB)
	$(CC) -shared $< -o $(OBJ_DIR)/hook.so


run-qemu:
	${QEMU_USER} ${QEMU_FLAGS} ${BUILD_DIR}/${TARGET}

run-qemu-preload:
	${QEMU_USER} LD_PRELOAD=$(PWD)/$(OBJ_DIR)/hook.so $(BUILD_DIR)/$(TARGET) ${QEMU_FLAGS} ${BUILD_DIR}/${TARGET}

run-qemu-gdb: $(TARGET)
	${QEMU_USER} ${QEMU_FLAGS} -g 1234 ${BUILD_DIR}/${TARGET}

clean:
	rm ${OBJ_DIR}/*.o ${OBJ_DIR}/*.a ${OBJ_DIR}/*.so ${BUILD_DIR}/${TARGET}