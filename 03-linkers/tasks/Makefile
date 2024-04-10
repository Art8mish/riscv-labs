
CC 		   ?= riscv64-unknown-linux-gnu-gcc
QEMU_USER  ?= qemu-riscv64

CFLAGS 	   ?= 
QEMU_FLAGS = -L /opt/sc-dt/riscv-gcc/sysroot/

TARGET := app

BUILD_DIR := ./build
SRC_DIR	  := ./src
OBJ_DIR   := ./obj

LIB_DYN_NAME := mystr
LIB_DYN_DIR  := ./custom_dynamic_lib/lib


LIB_STATIC_NAME := mystr
LIB_STATIC_DIR  := ./custom_static_lib/lib


SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ_SRC = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ = ${OBJ_SRC} ${OBJ_LIB}


all : $(TARGET)

.PHONY: _make_dirs clean

_make_dirs:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${OBJ_DIR}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

${TARGET}-static: _make_dirs ${OBJ_SRC}
	${CC} ${CFLAGS} ${OBJ_SRC} -L${LIB_STATIC_DIR} -l${LIB_STATIC_NAME} -o ${BUILD_DIR}/${TARGET}

${TARGET}-dyn: _make_dirs ${OBJ_SRC}
	${CC} ${CFLAGS} ${OBJ_SRC} -fPIC -L${LIB_DYN_DIR} -l${LIB_DYN_NAME} -Wl,-rpath,${LIB_DYN_DIR} -o ${BUILD_DIR}/${TARGET}


run-qemu:
	${QEMU_USER} ${QEMU_FLAGS} ${BUILD_DIR}/${TARGET}

run-qemu-gdb:
	${QEMU_USER} ${QEMU_FLAGS} -g 1234 ${BUILD_DIR}/${TARGET}

clean:
	rm ${OBJ_DIR}/*.o ${BUILD_DIR}/${TARGET}