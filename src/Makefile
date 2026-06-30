all: a.out

mnist_reader.o: mnist_reader.cc mnist_reader.h
	clang++ -r -o mnist_reader.o mnist_reader.cc

image_processing.o: image_processing.cc image_processing.h
	clang++ -r -o image_processing.o image_processing.cc

a.out: mnist_reader.o image_processing.o visualization.cc visualization.h image_processing.o test.cc
	clang++ mnist_reader.o visualization.cc image_processing.o test.cc -lz
