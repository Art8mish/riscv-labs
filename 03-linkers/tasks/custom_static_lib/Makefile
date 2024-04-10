
CC 		?= riscv64-unknown-linux-gnu-gcc
CFLAGS  ?= 
AR		?= riscv64-unknown-linux-gnu-ar
ARFLAGS ?= 

TARGET := libmystr

SRC_DIR = .
OBJ_DIR = ./obj
LIB_DIR = ./lib

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,  $(OBJ_DIR)/%.o, $(SRC))

all : $(TARGET)

.PHONY: clean _make_dirs

_make_dirs:
	@mkdir -p ${LIB_DIR}
	@mkdir -p ${OBJ_DIR}

${TARGET}: _make_dirs $(OBJ)
	${AR} rc ${LIB_DIR}/$@.a $(OBJ)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm ${OBJ_DIR}/*.o ${LIB_DIR}/*.a