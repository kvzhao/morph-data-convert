NAME = DataConveter
SRCS = morph_converter.cpp
LIBS = -lglog -lprotobuf -lleveldb -lmatio -lhdf5
CC = g++

all:
	$(CC) -o $(NAME) $(SRCS) $(LIBS)

clean:
	rm $(NAME)
