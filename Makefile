CC=gcc

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
TESTS_SRC_DIR=tests
INCLUDE_DIR=include

CFLAGS+=-Wall -g -I$(INCLUDE_DIR)
TESTS_CFLAGS=$(CFLAGS)
LDFLAGS+=-L/usr/lib -lm -lGL -lglut
TESTS_LDFLAGS=$(LDFLAGS) 

EXEC=plot
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC) : $(OBJ_DIR)/main.o $(OBJ_DIR)/Image.o $(OBJ_DIR)/Ppm.o $(OBJ_DIR)/draw.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/Image.o : $(SRC_DIR)/Image.c $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/Ppm.o : $(SRC_DIR)/Ppm.c $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/draw.o : $(SRC_DIR)/draw.c $(INCLUDE_DIR)/draw.h $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

tests: $(BIN_DIR)/bresenham

$(BIN_DIR)/bresenham: $(OBJ_DIR)/bresenham.o $(OBJ_DIR)/draw.o $(OBJ_DIR)/Image.o $(OBJ_DIR)/Ppm.o
	$(CC) $(TESTS_LDFLAGS) $^ -o $@

$(OBJ_DIR)/bresenham.o: $(TESTS_SRC_DIR)/bresenham.c
	$(CC) $(TESTS_CFLAGS) -c $< -o $@

clean :
	rm $(BIN_DIR)/* $(OBJ_DIR)/*.o

.PHONY: all clean tests
