NAME = DataConveter
SRCS = morph_converter.cpp
LIBS = -lglog -lprotobuf -lleveldb -lmatio -lhdf5 

#LIBS+= -lcaffe
#LISB+= -lcublas -lcudart -lcurand

CUDA_DIR = /usr/local/cuda-5.5
CUDA_INC = $(CUDA_DIR)/include

INCS = -L$(CUDA_INC)
CC = g++

all:
	$(CC) -o $(NAME) $(SRCS) $(INCS) $(LIBS)

clean:
	rm $(NAME) *.bin
