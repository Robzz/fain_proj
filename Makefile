CC=gcc

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
INCLUDE_DIR=include

CFLAGS+=-Wall -I$(INCLUDE_DIR)
LFLAGS+=-L/usr/lib -lm -lGL -lglut

EXEC=plot
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

$(BIN_DIR)/$(EXEC) : $(OBJ_DIR)/main.o $(OBJ_DIR)/Image.o $(OBJ_DIR)/Ppm.o $(OBJ_DIR)/draw.o
	$(CC) $^ -o $@ $(LFLAGS)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/Image.o : $(SRC_DIR)/Image.c $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/Ppm.o : $(SRC_DIR)/Ppm.c $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/draw.o : $(SRC_DIR)/draw.c $(INCLUDE_DIR)/draw.h $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	rm $(BIN_DIR)/$(EXEC) $(OBJ_DIR)/*.o
