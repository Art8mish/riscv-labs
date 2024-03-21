CC ?= riscv64-unknown-linux-gnu-gcc
QEMU_USER ?= qemu-riscv64

CFLAGS ?= 
QEMU_FLAGS = -L /opt/sc-dt/riscv-gcc/sysroot/

BUILD_DIR := build
.DEFAULT_GOAL := hello

.PHONY: _build_dir hello clean

_build_dir:
	@mkdir -p ${BUILD_DIR}

run-docker:
	sudo docker start cpp
	sudo docker exec -it cpp sh -c "/bin/bash"

run-env: /opt/sc-dt/env.sh
	source /opt/sc-dt/env.sh

hello: hello.c _build_dir
	${CC} ${CFLAGS} hello.c -o ${BUILD_DIR}/hello.elf

build: hello

show: hello
	file ${BUILD_DIR}/hello.elf

run: hello
	${BUILD_DIR}/hello.elf

run-qemu: hello
	${QEMU_USER} ${QEMU_FLAGS} ${BUILD_DIR}/hello.elf

run-qemu-gdb: hello
	${QEMU_USER} ${QEMU_FLAGS} -g 1234 ${BUILD_DIR}/hello.elf

clean:
	rm -r ${BUILD_DIR}
