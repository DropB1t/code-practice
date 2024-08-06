typedef struct e {
    int neigh;
    float weight;
    struct e* next;
} Edge;

typedef struct {
    int label;
    Edge* edges;
} Node;

typedef struct {
    int N;
    int A;
    Node* nodes;
} Graph;

typedef struct{
    int n1;
    int n2;
    float weight;
} SimpleEdge;

Graph loadGraph(char*);
void addEdge(Edge**, int, float);
int* dfs(Graph, int, int*, int*);

int hasCycle(Graph);
int isConnected(Graph);
int isTree(Graph);

Graph kruskal(Graph);
int compareEdges(const void*, const void*);
void removeEdge(Edge**,int);

void printGraph(Graph);

void freeGraph(Graph);
void freeEdges(Edge**);