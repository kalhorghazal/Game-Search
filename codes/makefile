COMPILER = g++
COMPILER_FLAGS = --std=c++11
all : worker.out presenter.out load_balancer.out

worker.out : worker.o extra_methods.o
	${COMPILER} ${COMPILER_FLAGS} worker.o extra_methods.o -o worker.out
	
worker.o :  worker.cpp worker.h extra_methods.h
	${COMPILER} ${COMPILER_FLAGS} -c worker.cpp -o worker.o

presenter.out : presenter.o extra_methods.o
	${COMPILER} ${COMPILER_FLAGS} presenter.o extra_methods.o -o presenter.out

presenter.o :  presenter.cpp presenter.h extra_methods.h
	${COMPILER} ${COMPILER_FLAGS} -c presenter.cpp -o presenter.o

load_balancer.out : load_balancer.o extra_methods.o
	${COMPILER} ${COMPILER_FLAGS} load_balancer.o extra_methods.o -o load_balancer.out

load_balancer.o :  load_balancer.cpp load_balancer.h extra_methods.h
	${COMPILER} ${COMPILER_FLAGS} -c load_balancer.cpp -o load_balancer.o

extra_methods.o :  extra_methods.cpp extra_methods.h
	${COMPILER} ${COMPILER_FLAGS} -c extra_methods.cpp -o extra_methods.o

rm:
	rm -rf *.out 
