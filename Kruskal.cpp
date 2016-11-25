/*
Name: Xiangzhen Sun
ID: xs14
Algorithm: Kruskal's algorithm to find MST
Methodology:
1. Sort all the edges in non-decreasing order of their weight.
2. Pick the smallest edge at a time. Check if it forms a cycle with
   the spanning tree formed so far. If cycle is not formed, then include
   this edge. Else, discard it.
3. Repeat step 2 until there are V-1 edges in the spanning tree, which
   denotes the formation of a minimum spanning tree.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// create a structure to represent a undirected, weighted edge in a graph
struct Edge
{
    // the format of an edge is: (source, destination), weight
    int src, dest, weight;
};

// create a structure to represent a connected, undirected, and weighted graph
struct Graph
{
    // V denotes the number of vertices, E denotes the number of edges
    int V, E;

    // the edges of a graph will be presented in an array
    struct Edge* edge;
};

// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
    // allocate memory for the Graph
    struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
    graph->V = V;
    graph->E = E;
    // allocate memory for edge
    graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );

    return graph;
}

// A structure to represent a subset for the union of found disjoint subsets
struct subset
{
    int parent;
    int rank;
};

// the function finds set of an element i
int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// Union operation: find the union of two sets
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high rank tree
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and increment
    // its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Compare two edges according to their weights.
// this will be used in qsort() for sorting an array of edges
int myComp(const void* a, const void* b)
{
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight > b1->weight;
}

// Construct MST using Kruskal's algorithm
void KruskalMST(struct Graph* graph)
{
    int V = graph->V;
    struct Edge result[V];  // stores the resultant MST
    int e = 0;  // An index variable, used for result[]
    int i = 0;
    // Step 1:  Sort all the edges in non-decreasing order of their weight
    // If we are not allowed to change the given graph, we can create a copy of
    // array of edges
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);

    // Allocate memory for creating V ssubsets
    struct subset *subsets =
        (struct subset*) malloc( V * sizeof(struct subset) );

    // Create V subsets with single elements
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Number of edges to be taken is equal to V-1
    while (e < V - 1)
    {
        // Step 2: Pick the smallest edge. And increment the index
        // for next iteration
        struct Edge next_edge = graph->edge[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        // If including this edge does't cause cycle, include it
        // in result and increment the index of result for next edge
        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
        // Else discard the next_edge
    }

    // print the contents of result[] to display the built MST
    int totalCost = 0;
    cout << "A summary of the constructed MST\n";
    for (int j = 0; j < e; ++j)  {
        totalCost = totalCost + result[j].weight;
    }
    cout << totalCost << '\n';
    // sort the resultant MST nodes
    for(int k = 0; k < e; k++)  {
        if( result[k].src > result[k].dest ) {
            auto temp = result[k].dest;
            result[k].dest = result[k].src;
            result[k].src = temp;
        }
    }

    int flag = 1;
    for (int k = 1; (k < e) && flag; k++)  {
        flag = 0;
        for (int j = 0; j < e - 1; j++)  {
            if ((result[j+1].src < result[j].src)||((result[j+1].src == result[j].src) && (result[j+1].dest < result[j].dest)))  {
                auto temp = result[j];
                result[j] = result[j+1];
                result[j+1] = temp;
                flag = 1;
            }
        }
    }

    for (int j = 0; j < e; j++ )  {
        cout << result[j].src + 1 << ' ' << result[j].dest + 1 << '\n';
    }

    return;
}

// Driver program to test above functions
int main()
{

   char filename[256];
   cout << "Please type in the name of the input file, hit ENTER to complete: ";
   int V;
   int E = 0;
   fstream myfile;
   fstream myfile1;
   char input[256];
   cin >> filename;
   myfile.open(filename);
   myfile.getline(input, 256, '\n');
   V = atoi(input);

   while(!myfile.eof())  {
       myfile.getline(input, 256, '\n');
       E++;
   }
   E--;
   myfile.close();

   struct Graph* graph = createGraph(V, E);
   myfile1.open(filename);
   myfile1.getline(input, 256, '\n');

   string str;
   for (int i = 0; i < E; i++)  {
       str.clear();
       getline(myfile1, str, '\n');
       string::size_type sy;
       graph->edge[i].src = stoi(str, &sy) - 1;
       string subs = str.substr(sy);
       string::size_type sz;
       graph->edge[i].dest = stoi(subs, &sz) -1;
       string subs1 = subs.substr(sz);
       graph->edge[i].weight = stoi(subs1, nullptr);
   }

   KruskalMST(graph);
   return 0;
}

