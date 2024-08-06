#include "graph.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Graph g = loadGraph("g1.txt");

    printGraph(g);

	int* vis=dfs(g,0,NULL,NULL);

	printf("Visita: ");
	for(int i=0;i<g.N;i++){
		printf("%d ", g.nodes[vis[i]].label);
	}
	printf ("\n");
    free(vis);
    printf("Is connected %d, is tree %d, has cycles %d .\n",isConnected(g),isTree(g),hasCycle(g));

    Graph t = kruskal(g);
    printGraph(t);

    freeGraph(g);
    freeGraph(t);
    return 0;
}