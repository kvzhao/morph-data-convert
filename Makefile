NAME = DataConveter
SRCS = morph_converter.cpp
CV_LIBS = -lopencv_core -lopencv_highgui
LIBS = -lglog -lprotobuf -lleveldb -lmatio -lhdf5 $(CV_LIBS)

#LIBS+= -lcaffe
#LISB+= -lcublas -lcudart -lcurand

CUDA_DIR = /usr/local/cuda-5.5
CUDA_INC = $(CUDA_DIR)/include

INCS = -L$(CUDA_INC)
CC = g++
CFLAGS = -std=c++0x -g

all:
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) $(INCS) $(LIBS)

clean:
	rm $(NAME) *.bin
