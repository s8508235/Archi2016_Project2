SRC = ./*.c
OBJ = ./*.o
GCC = gcc
pipeline: allc
	$(GCC) -o $@ $(OBJ)
allc:$(SRC)
	$(GCC) -c $(SRC)
clean:
	rm $(OBJ) pipeline 
