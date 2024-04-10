
CC 		   ?= riscv64-unknown-linux-gnu-gcc
QEMU_USER  ?= qemu-riscv64
AR		   ?= riscv64-unknown-linux-gnu-ar
CFLAGS 	   ?= 
QEMU_FLAGS = -L /opt/sc-dt/riscv-gcc/sysroot/

TARGET := fact

BUILD_DIR := ./build
SRC_DIR	  := ./src
OBJ_DIR   := ./obj
LIB_DIR   := ./lib

SRC = $(wildcard $(SRC_DIR)/*.c)
LIB = $(wildcard $(LIB_DIR)/*.c)

OBJ_SRC = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ_LIB = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB))

OBJ = ${OBJ_SRC} ${OBJ_LIB}


all : $(TARGET)

.PHONY: _make_dirs clean

_make_dirs:
	@mkdir -p ${BUILD_DIR}
	@mkdir -p ${OBJ_DIR}

run-docker:
	sudo docker start cpp
	sudo docker exec -it cpp sh -c "/bin/bash"
run-env:
	source /opt/sc-dt/env.sh


$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

$(OBJ_DIR)/%.o : $(LIB_DIR)/%.c 
	$(CC) $(CFLAGS) $< -c -o $@


${TARGET}-no-lib : ${OBJ}
	${CC} ${CFLAGS} $^ -o ${BUILD_DIR}/${TARGET}

${TARGET}-static-lib : ${OBJ_LIB} static-libfact ${OBJ_SRC}
	${CC} ${CFLAGS} ${OBJ_SRC} -L${OBJ_DIR} -lfact -o ${BUILD_DIR}/${TARGET}

${TARGET}-dynamic-lib : ${OBJ_LIB} dynamic-libfact ${OBJ_SRC}
	${CC} ${CFLAGS} ${OBJ_SRC} -fPIC -L${OBJ_DIR} -lfact -Wl,-rpath,${OBJ_DIR} -o ${BUILD_DIR}/${TARGET}

bin-with-lib: ${}
	${CC} ${CFLAGS} $^ -o ${BUILD_DIR}/${TARGET}

	
main.o: ${SRC_DIR}/main.c
	${CC} ${CFLAGS} $^ -c -o ${OBJ_DIR}/$@

fact.o: ${LIB_DIR}/fact.c
	${CC} ${CFLAGS} $^ -c -o ${OBJ_DIR}/$@

static-libfact: ${OBJ_DIR}/fact.o
	${AR} rc ${OBJ_DIR}/libfact.a $^

dynamic-libfact: ${OBJ_DIR}/fact.o
	${CC} -shared $^ -o ${OBJ_DIR}/libfact.so

example: ${SRC_DIR}/main.c
	${CC} ${CFLAGS} $^ -o ${BUILD_DIR}/${TARGET}


run-qemu:
	${QEMU_USER} ${QEMU_FLAGS} ${BUILD_DIR}/${TARGET}

run-qemu-gdb: $(TARGET)
	${QEMU_USER} ${QEMU_FLAGS} -g 1234 ${BUILD_DIR}/${TARGET}

clean:
	rm ${OBJ_DIR}/*.o ${OBJ_DIR}/*.a ${OBJ_DIR}/*.so ${BUILD_DIR}/${TARGET}