all: 3or7

mnist_reader.o: mnist_reader.cc mnist_reader.h
	clang++ -r -o mnist_reader.o mnist_reader.cc

image_processing.o: image_processing.cc image_processing.h
	clang++ -r -o image_processing.o image_processing.cc

visualization.o: visualization.cc visualization.h
	clang++ -r -o visualization.o visualization.cc

3or7: mnist_reader.o image_processing.o visualization.o image_processing.o 3or7.cc
	clang++ -o 3or7 mnist_reader.o visualization.cc image_processing.o 3or7.cc -lz
