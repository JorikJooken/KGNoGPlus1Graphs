compiler=gcc
flags=-std=gnu11 -march=native
densenauty32=-DWORDSIZE=32 -DMAXN=WORDSIZE nautyW1.a
densenauty64=-DWORDSIZE=64 -DMAXN=WORDSIZE nautyL1.a
densenauty128=-DWORDSIZE=128 -DMAXN=WORDSIZE nautyQ1.a
densenautyArbitrary=-DWORDSIZE=64 nauty.a

#all: debugging5

all: genRGNoGPlus1



genRGNoGPlus1: generateRGGraphsNoGPlus1Cycle.c bitset.h read_graph/readGraph6.c
	$(compiler) -o generateRGGraphsNoGPlus1CycleExecutable generateRGGraphsNoGPlus1Cycle.c read_graph/readGraph6.c $(densenauty128) $(flags) -O4

.PHONY: clean
clean:
	rm -f generateRGGraphsExecutable
