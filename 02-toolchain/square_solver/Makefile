CC ?= riscv64-unknown-linux-gnu-gcc
QEMU_USER ?= qemu-riscv64
TARGET ?= solver

CFLAGS ?=

QEMU_FLAGS = -L /opt/sc-dt/riscv-gcc/sysroot

BUILD_DIR := ./build
SRC_DIR   := ./source
OBJ_DIR   := ./obj

.DEFAULT_GOAL := build

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,  $(OBJ_DIR)/%.o, $(SRC))


.PHONY: _make_dirs clean

_make_dirs:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${OBJ_DIR}

run-docker:
	sudo docker start cpp
	sudo docker exec -it cpp sh -c "/bin/bash"
run-env:
	source /opt/sc-dt/env.sh

build : _make_dirs $(SRC)
	$(CC) $(CFLAGS) $(SRC) -lm -o ${BUILD_DIR}/${TARGET}.elf

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	${BUILD_DIR}/${TARGET}.elf

run-qemu: build
	${QEMU_USER} ${QEMU_FLAGS} ${BUILD_DIR}/solver.elf

run-qemu-gdb: build
	${QEMU_USER} ${QEMU_FLAGS} -g 1234 ${BUILD_DIR}/solver.elf

clean:
	rm -r ${BUILD_DIR}/*.elf ${OBJ_DIR}/*.o
