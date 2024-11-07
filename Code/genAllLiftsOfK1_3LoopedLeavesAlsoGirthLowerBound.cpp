#include <bits/stdc++.h>


using namespace std;

vector< set<int> > graph;
vector< vector<int> > adjMatrix;
int n;
int requiredGirth;

void writeAdjMatrix()
{
    cout << "n=" << n << endl;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

int calcShortestPathErasedEdge(int u, int v)
{
    vector<int> dist(n,1e9);
    dist[u]=0;
    queue<int> q;
    q.push(u);
    while(!q.empty())
    {
        int now=q.front();
        q.pop();
        for(int neigh : graph[now])
        {
            if(now==u && neigh==v) continue;
            if(now==v && neigh==u) continue;
            if(dist[neigh]<1e9) continue;
            dist[neigh]=dist[now]+1;
            q.push(neigh);
            if(neigh==v) return dist[neigh];
        }
    }
    return dist[v];
}

int calcGirthUpperBound()
{
    int ret=1e9;
    for(int u=0; u<n; u++)
    {
        for(int v : graph[u])
        {
            if(v<u) continue;
            int dist=calcShortestPathErasedEdge(u,v);
            ret=min(ret,dist+1);
            if(ret<requiredGirth) return ret;
        }
    }
    return ret;
}

// lifts of this graph:
//             0
//             |
//             |
//             3
//            / \ 
//           /   \ 
//          1     2

// where 0, 1 and 2 get loops

// TODO: for (3,11,no(12))-graphs, the groups must be non-abelian! (because otherwise, there is a closed non-reversible walk that allows
// us to make a 12-cycle)
int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        fprintf(stderr,"Wrong number of command line arguments!\nExpected girthRequired\n");
    }
    requiredGirth=atoi(argv[1]);

    ios::sync_with_stdio(false);
    cin.tie(0);
    string line;
    //int ctr=0;
    while(getline(cin,line))
    {
        //ctr++;
	// TODO: careful, I do not read all groups, because of this break!
        //if(ctr>100) break;
        int nbGroupElements=atoi(line.c_str());

        // TODO: remove this break later!
        //if(nbGroupElements>230) break;
        cerr << "nbGroupElements: " << nbGroupElements << endl;

        vector< vector<int> > groupMultiplicationTable(nbGroupElements,vector<int>(nbGroupElements,-1));
        for(int i=0; i<nbGroupElements; i++)
        {
            getline(cin,line);
            stringstream ss;
            ss << line;
            for(int j=0; j<nbGroupElements; j++)
            {
                int tmp;
                ss >> tmp;
                //cerr << tmp << " ";
                groupMultiplicationTable[i][j]=tmp-1;
            }
            //cerr << endl;
        }
	if(nbGroupElements!=19) continue;       

        n=nbGroupElements*4;
        adjMatrix.assign(n, vector<int>(n,0));
        set<int> emp;
        graph.assign(n,emp);

        // tree in the middle
        for(int outer=0; outer<3; outer++)
        {
            for(int el=0; el<nbGroupElements; el++)
            {
                int idx1=outer*nbGroupElements+el;
                int idx2=3*nbGroupElements+el;
                adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=1;
                graph[idx1].insert(idx2);
                graph[idx2].insert(idx1);
            }
        }

        set<int> suitableElements;
        for(int topElGroup=0; topElGroup<nbGroupElements; topElGroup++)
        {
            bool ok=true;

            bool loops=false;
            // top
            for(int el=0; el<nbGroupElements; el++)
            {
                int newEl=groupMultiplicationTable[el][topElGroup];
                int revEl=groupMultiplicationTable[newEl][topElGroup];
                int idx1=0*nbGroupElements+el;
                int idx2=0*nbGroupElements+newEl;
                adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=1;
                graph[idx1].insert(idx2);
                graph[idx2].insert(idx1);
                if(idx1==idx2 || el==revEl) loops=true;
            }
            int currGirth=-1;
            if(!loops) currGirth=calcGirthUpperBound();
            if(currGirth<requiredGirth || loops)
            {
                ok=false;
            }
            if(ok) suitableElements.insert(topElGroup);
            // undo top
            for(int el=0; el<nbGroupElements; el++)
            {
                int newEl=groupMultiplicationTable[el][topElGroup];
                int idx1=0*nbGroupElements+el;
                int idx2=0*nbGroupElements+newEl;
                adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                graph[idx1].erase(idx2);
                graph[idx2].erase(idx1);
            }
        }
        cerr << "nbGroupElements and suitableElements.size(): " << nbGroupElements << " " << suitableElements.size() << endl;
        
        for(int topElGroup : suitableElements)
        {
            bool loopsTop=false;
            // top
            for(int el=0; el<nbGroupElements; el++)
            {
                int newEl=groupMultiplicationTable[el][topElGroup];
                int revEl=groupMultiplicationTable[newEl][topElGroup];
                int idx1=0*nbGroupElements+el;
                int idx2=0*nbGroupElements+newEl;
                adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=1;
                graph[idx1].insert(idx2);
                graph[idx2].insert(idx1);
                if(idx1==idx2 || el==revEl) loopsTop=true;
            }
            int currGirth=calcGirthUpperBound();
            if(currGirth<requiredGirth || loopsTop)
            {
                // undo top
                for(int el=0; el<nbGroupElements; el++)
                {
                    int newEl=groupMultiplicationTable[el][topElGroup];
                    int idx1=0*nbGroupElements+el;
                    int idx2=0*nbGroupElements+newEl;
                    adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                    graph[idx1].erase(idx2);
                    graph[idx2].erase(idx1);
                }
                continue;
            }        

            for(int leftElGroup : suitableElements)
            {
                if(requiredGirth>=11 && groupMultiplicationTable[topElGroup][leftElGroup]==groupMultiplicationTable[leftElGroup][topElGroup]) continue;
                bool loopsLeft=false;
                // left
                for(int el=0; el<nbGroupElements; el++)
                {
                    int newEl=groupMultiplicationTable[el][leftElGroup];
                    int revEl=groupMultiplicationTable[newEl][leftElGroup];
                    int idx1=1*nbGroupElements+el;
                    int idx2=1*nbGroupElements+newEl;
                    adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=1;
                    graph[idx1].insert(idx2);
                    graph[idx2].insert(idx1);
                    if(idx1==idx2 || el==revEl) loopsLeft=true;
                }

                currGirth=calcGirthUpperBound();
                if(currGirth<requiredGirth || loopsLeft)
                {
                    // undo left
                    for(int el=0; el<nbGroupElements; el++)
                    {
                        int newEl=groupMultiplicationTable[el][leftElGroup];
                        int idx1=1*nbGroupElements+el;
                        int idx2=1*nbGroupElements+newEl;
                        adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                        graph[idx1].erase(idx2);
                        graph[idx2].erase(idx1);
                    }
                    continue;
                }

                for(int rightElGroup : suitableElements)
                {
                    if(requiredGirth>=11 && groupMultiplicationTable[topElGroup][rightElGroup]==groupMultiplicationTable[rightElGroup][topElGroup]) continue;
                    if(requiredGirth>=11 && groupMultiplicationTable[rightElGroup][leftElGroup]==groupMultiplicationTable[leftElGroup][rightElGroup]) continue;
                    bool loopsRight=false;
                    // right
                    for(int el=0; el<nbGroupElements; el++)
                    {
                        int newEl=groupMultiplicationTable[el][rightElGroup];
                        int revEl=groupMultiplicationTable[newEl][rightElGroup];
                        int idx1=2*nbGroupElements+el;
                        int idx2=2*nbGroupElements+newEl;
                        adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=1;
                        graph[idx1].insert(idx2);
                        graph[idx2].insert(idx1);
                        if(idx1==idx2 || el==revEl) loopsRight=true;
                    }

                    currGirth=calcGirthUpperBound();
                    if(currGirth<requiredGirth || loopsRight)
                    {
                        // undo right
                        for(int el=0; el<nbGroupElements; el++)
                        {
                            int newEl=groupMultiplicationTable[el][rightElGroup];
                            int idx1=2*nbGroupElements+el;
                            int idx2=2*nbGroupElements+newEl;
                            adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                            graph[idx1].erase(idx2);
                            graph[idx2].erase(idx1);
                        }
                        continue;
                    }

                    writeAdjMatrix();

                    // undo right
                    for(int el=0; el<nbGroupElements; el++)
                    {
                        int newEl=groupMultiplicationTable[el][rightElGroup];
                        int idx1=2*nbGroupElements+el;
                        int idx2=2*nbGroupElements+newEl;
                        adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                        graph[idx1].erase(idx2);
                        graph[idx2].erase(idx1);
                    }
                }

                // undo left
                for(int el=0; el<nbGroupElements; el++)
                {
                    int newEl=groupMultiplicationTable[el][leftElGroup];
                    int idx1=1*nbGroupElements+el;
                    int idx2=1*nbGroupElements+newEl;
                    adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                    graph[idx1].erase(idx2);
                    graph[idx2].erase(idx1);
                }
            }

            // undo top
            for(int el=0; el<nbGroupElements; el++)
            {
                int newEl=groupMultiplicationTable[el][topElGroup];
                int idx1=0*nbGroupElements+el;
                int idx2=0*nbGroupElements+newEl;
                adjMatrix[idx1][idx2]=adjMatrix[idx2][idx1]=0;
                graph[idx1].erase(idx2);
                graph[idx2].erase(idx1);
            }
        }
    }
    return 0;
}
