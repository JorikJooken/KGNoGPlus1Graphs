# On (k,g)-Graphs without (g+1)-Cycles

This repository contains code and data related to the paper "On (k,g)-Graphs without (g+1)-Cycles". All code can be found in the directory "Code", whereas all data can be found in the directory "Data".

A $(k,g,\underline{g+1})$-graphs is a $k$-regular graph with girth $g$ that does not contain any cycles of length $g+1$. We use $n(k,g,\underline{g+1})$ to denote the order of the smallest such graph.

Below, we briefly describe the different programs and data.

## DATA
The files "smallest_x,yNoz.g6" contains a complete list of all (proven) smallest $(x,y,\underline{z})$-graphs that were generated using the algorithm described in the paper. The files "record_x,yNoz.g6" contain the smallest such known graphs, but it is not proven that no smaller such graphs can exist. All graphs are stored in graph6-format.

## CODE

This folder contains multiple files containing code for the exhaustive generation of $(k,g,\underline{g+1})$-graphs. There are also several files containing helper functions (e.g. to support reading graphs in graph6 format, efficiently representing sets as bitsets and so on). We will only briefly discuss the code that is directly related to $(k,g,\underline{g+1})$-graphs graphs.

### generateRGGraphsNoGPlus1Cycle.c

This programs allows one to generate all $k$-regular graphs with girth at least $g$ without $(g+1)$-cycles on $n$ vertices.

These programs can be compiled by executing the makefile:
```bash
make
```

This program expects the parameters $n$, $k$, $g$ as input and will output a list of all $k$-regular graphs with girth at least $g$ without $(g+1)$-cycles on $n$ vertices (one graph in graph6 format per line). For example, executing the following command:

```bash
./generateRGGraphsNoGPlus1CycleExecutable 10 3 3
```
 will output a list of all $3$-regular graphs with girth at least $3$ without $4$-cycles on $10$ vertices:

```bash
n, r and girthRequired: 10 3 3
I{O_w_H@W
I{O_ogI@W
IsP@PGXD_
Computation finished for n, r and girthRequired: 10 3 3
```

Note that not of all these graphs are necessarily $(k,g,\underline{g+1})$ graphs, because the girth could be strictly larger than $g$. For example, in the above output the Petersen graph is such an example. It is easy to manually filter the $(k,g,\underline{g+1})$-graphs, by using nauty for example:


```bash
./generateRGGraphsNoGPlus1CycleExecutable 10 3 3 | ./nauty2_8_8/pickg -g3
```

 will result in the following two graphs (i.e. the Petersen graph is not selected)

```bash
I{O_w_H@W
I{O_ogI@W
```

The program "generateRGGraphsNoGPlus1CycleExecutable" supports graphs until order 128.

### generateCanonicalDoubleCover.cpp

This program can be compiled by executing the following command:
```bash
g++ -g -O3 generateCanonicalDoubleCover.cpp -o generateCanonicalDoubleCoverExecutable
```

This program expects as input a list of graphs in graph6 format (one graph per line). It will output for each graph an adjacency matrix of the canonical double cover of this graph. This adjacency matrix can be easily converted to a graph6-string, again using nauty. For example, executing:

```bash
./generateRGGraphsNoGPlus1CycleExecutable 18 3 5 | ./generateCanonicalDoubleCoverExecutable | ./nauty2_8_8/amtog
```

will produce the following output:
 ```bash
c?????????????????????????B_?CW?AB??_K?ACO?GAG?I@??GGG?AGO??g?O?A?o??A?g??@O_??GOG??@CO????h????gG????J???
```

This is the canonical double cover of the unique $(3,5,\underline{6})$-cage. We can verify that its girth is indeed 8, as expected:


```bash
./generateRGGraphsNoGPlus1CycleExecutable 18 3 5 | ./generateCanonicalDoubleCoverExecutable | ./nauty2_8_8/amtog | ./nauty2_8_8/countg --g
```

yields as output:

```bash
1 graphs converted from stdin to stdout.
          1 graphs : girth=8
1 graphs altogether; cpu=0.00 sec
```

### filterHasNoCycleOfLengthVeryLargeGraph.cpp

This program can be compiled by executing the following command:
```bash
g++ -g -std=c++11 -O3 filterHasNoCycleOfLengthVeryLargeGraph.cpp -o filterHasNoCycleOfLengthVeryLargeGraphExecutable
```

This program allows one to filter out all graphs that do not have any cycles of length $s$. For example, executing:

```bash
./generateRGGraphsNoGPlus1CycleExecutable 18 3 5 | ./filterHasNoCycleOfLengthVeryLargeGraphExecutable 6
```

yields as output the following graph:

```bash
QsP@P?PD?O?O?I@A?G_?D?D??@W
```

This graph does not have any 6-cycles, because it is the unique $(3,5,\underline{6})$-cage.

### genAllLiftsOfK1\_3LoopedLeavesAlsoGirthLowerBound.cpp

This program can be compiled by executing the following command:
```bash
g++ -g -std=c++11 -O3 genAllLiftsOfK1_3LoopedLeavesAlsoGirthLowerBound.cpp -o genAllLiftsOfK1_3LoopedLeavesAlsoGirthLowerBoundExecutable
```

This program allows one to generate all regular lifts of the graph obtained by adding a loop to each leaf of the tree $K_{1,3}$, which have girth at least $g$, where the voltage assignments are considered for all groups from the input. For example, executing:

```bash
./genAllLiftsOfK1_3LoopedLeavesAlsoGirthLowerBoundExecutable 8 < groupsUntilOrder20.txt
```

yields as output the adjacency matrices of all such graphs which have girth at least 8. Here, the file "groupsUntilOrder20.txt" contains all groups until order 20.
