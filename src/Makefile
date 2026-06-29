all: a.out

a.out: mnist_reader.cc mnist_reader.h visualization.cc visualization.h test.cc
	clang++ mnist_reader.cc visualization.cc test.cc -lz
