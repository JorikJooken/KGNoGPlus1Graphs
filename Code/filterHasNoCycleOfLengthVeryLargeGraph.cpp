#include <bits/stdc++.h>

// Unsafe because no defined behaviour if character = 0. ctz and clz work with 32 bit numbers.
#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

using namespace std;

const int nb_bits=4096;
#define MAXVERTICES 4096

int n;
vector< vector<int> > graph;

int toAvoidCycleLength;

int getNumberOfVertices(string graphString) 
{
	if(graphString.size() == 0){
        printf("Error: String is empty.\n");
        abort();
    }
    else if((graphString[0] < 63 || graphString[0] > 126) && graphString[0] != '>') {
    	printf("Error: Invalid start of graphstring.\n");
    	abort();
    }

	int index = 0;
	if (graphString[index] == '>') { // Skip >>graph6<< header.
		index += 10;
	}

	if(graphString[index] < 126) { // 0 <= n <= 62
		return (int) graphString[index] - 63;
	}

	else if(graphString[++index] < 126) { 
		int number = 0;
		for(int i = 2; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else if (graphString[++index] < 126) {
		int number = 0;
		for (int i = 5; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else {
		printf("Error: Format only works for graphs up to 68719476735 vertices.\n");
		abort();
	}
}

void loadGraph(string graphString, int numberOfVertices) {
    vector<int> emp;
    graph.assign(n,emp);
	int startIndex = 0;
	if (graphString[startIndex] == '>') { // Skip >>graph6<< header.
		startIndex += 10;
	}
	if (numberOfVertices <= 62) {
		startIndex += 1;
	}
	else if (numberOfVertices <= MAXVERTICES) {
		startIndex += 4;
	}
	else {
		printf("Error: Program can only handle graphs with %d vertices or fewer.\n",MAXVERTICES);
		abort();
	}

	int currentVertex = 1;
	int sum = 0; 
	for (int index = startIndex; index<graphString.size(); index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while(5-i+(index-startIndex)*6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5-i+(index - startIndex)*6 - sum;
            graph[currentVertex].push_back(neighbour);
            graph[neighbour].push_back(currentVertex);
		}
	}
}

vector<int> currentCycle;
vector<bool> vis;

bool has(int current, int start, int nbToAdd)
{
    if(nbToAdd==0)
    {
        for(int neigh : graph[current])
        {
            if(neigh==start) return true;
        }
        return false;
    }
    for(int neigh : graph[current])
    {
        if(vis[neigh]) continue;
        if(neigh<start) continue;
        vis[neigh]=true;
        if(has(neigh,start,nbToAdd-1)) return true;
        vis[neigh]=false;
    }
    return false;
}

bool hasCycle()
{
    for(int i=0; i+toAvoidCycleLength-1<n; i++)
    {
        currentCycle.clear();
        currentCycle.push_back(i);
        vis.assign(n,false);
        vis[i]=true;
        if(has(i,i,toAvoidCycleLength-1)) return true;
    }
    return false;
}

int main(int argc, char ** argv) {

    if(argc!=2)
    {
        fprintf(stderr,"Wrong number of command line arguments!\nExpected toAvoidCycleLength\n");
    }
    toAvoidCycleLength=atoi(argv[1]);
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    string line;
    while(getline(cin,line))
    {
        //line+="\n";
        nb_graphs_read_from_input++;
        n = getNumberOfVertices(line);
        loadGraph(line,n);
        if(hasCycle()) continue;
        printf("%s\n",line.c_str());
    }
    return 0;
}
