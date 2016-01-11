CXX=g++

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
TESTS_SRC_DIR=tests
INCLUDE_DIR=include

CFLAGS+=-Wall -Wextra -Weffc++ -g -Og -std=c++11 -I$(INCLUDE_DIR)
TESTS_CFLAGS=$(CFLAGS)
LDFLAGS+=-L/usr/lib -lm -lGL -lglut
TESTS_LDFLAGS=$(LDFLAGS) 

EXEC=plot

all: dirs $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC) : $(OBJ_DIR)/main.o $(OBJ_DIR)/Image.o $(OBJ_DIR)/Ppm.o $(OBJ_DIR)/draw.o $(OBJ_DIR)/matrix33f.o $(OBJ_DIR)/shapes.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CXX) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/Image.o : $(SRC_DIR)/Image.cpp $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CXX) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/Ppm.o : $(SRC_DIR)/Ppm.c $(INCLUDE_DIR)/Ppm.h
	$(CXX) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/draw.o : $(SRC_DIR)/draw.c $(INCLUDE_DIR)/draw.h $(INCLUDE_DIR)/Image.h $(INCLUDE_DIR)/Ppm.h
	$(CXX) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/shapes.o : $(SRC_DIR)/shapes.cpp $(INCLUDE_DIR)/shapes.h $(INCLUDE_DIR)/vector.h $(INCLUDE_DIR)/array.h
	$(CXX) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/matrix33f.o : $(SRC_DIR)/matrix33f.cpp $(INCLUDE_DIR)/vector.h $(INCLUDE_DIR)/array.h
	$(CXX) -c $< -o $@ $(CFLAGS)

tests: $(BIN_DIR)/bresenham

$(BIN_DIR)/bresenham: $(OBJ_DIR)/bresenham.o $(OBJ_DIR)/draw.o $(OBJ_DIR)/Image.o $(OBJ_DIR)/Ppm.o
	$(CXX) $(TESTS_LDFLAGS) $^ -o $@

$(OBJ_DIR)/bresenham.o: $(TESTS_SRC_DIR)/bresenham.c
	$(CXX) $(TESTS_CFLAGS) -c $< -o $@

clean :
	rm $(BIN_DIR)/* $(OBJ_DIR)/*.o

archive:
	tar -acf chavignat_projet_fain.tar.gz src include Makefile tests

dirs: obj bin
	mkdir -p $^

.PHONY: all clean tests archive dirs
