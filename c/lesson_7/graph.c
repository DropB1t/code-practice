#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Graph loadGraph(char* file) {
    Graph g;
    g.N = 0;
    g.A = 0;
    g.nodes = NULL;

    FILE* fl = fopen(file, "r");

    if (fl) {
        fscanf(fl, "%d", &g.N);
        g.nodes = malloc(g.N * sizeof(Node));
        if (g.nodes) {
            for (int i = 0; i < g.N; i++) {
                fscanf(fl, "%d", &g.nodes[i].label);
                g.nodes[i].edges = NULL;
            }
            while (!feof(fl)) {
                int n1, n2, n1_idx, n2_idx;
                float w;
                fscanf(fl, "%d %d %f", &n1, &n2, &w);
                if (!feof(fl)) {
                    for (int i = 0; i < g.N; i++) {
                        if (g.nodes[i].label == n1) {
                            n1_idx = i;
                        }
                        if (g.nodes[i].label == n2) {
                            n2_idx = i;
                        }
                    }
                    addEdge(&g.nodes[n1_idx].edges, n2_idx, w);
                    addEdge(&g.nodes[n2_idx].edges, n1_idx, w);
                    g.A++;
                }
            }
            fclose(fl);
        } else {
            printf("Memory if full\n");
            exit(1);
        }
    } else {
        perror("Apertura file non riuscita.\n");
    }
    return g;
}

void addEdge(Edge** adjList, int n, float w) {
    Edge* newE = (Edge*)malloc(sizeof(Edge));

    if (newE) {
        newE->neigh = n;
        newE->weight = w;
        newE->next = *adjList;
        *adjList = newE;
    } else {
        printf("Memory if full\n");
        exit(1);
    }

    return;
}

int* dfs(Graph g, int start, int* hasCycles, int* cccount) {  //prende grafo e indice del nodo di inizio
    Stack s = NULL;                                           //pila per i nodi da visitare
    int finished = 0;
    int* ris = malloc(g.N * sizeof(int));  //nodi in ordine della visita
    int risCount = 0;

    int visited[g.N];  //array che tiene traccia dei nodi già visitati
    for (int i = 0; i < g.N; i++)
        visited[i] = 0;  //nessun nodo visitato all'inizio
    if (hasCycles) {
        *hasCycles = 0;
    }
    push(&s, (double)start);
    if (cccount)
        *cccount = 1;
    while (!finished) {
        while (s) {  //fin che ho nodi da visitare in questa componente connessa
            int node = (int)pop(&s);
            if (!visited[node]) {  //ho un ciclo
                visited[node] = 1;
                ris[risCount] = node;
                risCount++;
            } else {
                if (hasCycles) {
                    *hasCycles = 1;
                }
            }
            Edge* edges = g.nodes[node].edges;
            while (edges) {
                if (!visited[edges->neigh])
                    push(&s, (double)edges->neigh);
                edges = edges->next;
            }
        }
        //ho finito la componente connessa, devo controllare che non ne hon altre
        int i = 0;
        while (i < g.N && visited[i]) {
            i++;
        }
        if (i == g.N)  //ho visitato tutti i nodi
            finished = 1;
        else {
            push(&s, (double)i);
            if (cccount)
                (*cccount)++;
        }  //continuo con la componente connessa che contine nodo i
    }
    freeS(&s);
    return ris;
}

int hasCycle(Graph g) {
    int cycles, *vis;
    vis = dfs(g, 0, &cycles, NULL);
    free(vis);
    return cycles;
}

int isConnected(Graph g) {
    int ccc, *vis;
    vis = dfs(g, 0, NULL, &ccc);
    free(vis);
    return ccc;
}

int isTree(Graph g) {
    int ccc, cycles, *vis;
    vis = dfs(g, 0, &cycles, &ccc);
    free(vis);
    return !cycles && ccc;
}

Graph kruskal(Graph g) {
    Graph T;
    T.N = g.N;
    T.A = 0;
    T.nodes = (Node*)malloc(sizeof(Node) * T.N);
    if (T.nodes) {
        for (int i = 0; i < T.N; i++) {
            T.nodes[i].label = g.nodes[i].label;
            T.nodes[i].edges = NULL;
        }
        //Creare array di archi, li ordino e runniamo kruskal;
        SimpleEdge edges[g.A];
        int count = 0;
        for (int i = 0; i < g.N; i++) {
            Edge* e = g.nodes[i].edges;
            while (e) {
                if (e->neigh > i) {
                    edges[count].n1 = i;
                    edges[count].n2 = e->neigh;
                    edges[count].weight = e->weight;
                    count++;
                }
                e = e->next;
            }
        }
        qsort(edges, g.A, sizeof(SimpleEdge), compareEdges);
        //Kruskal
        count = 0;
        while (T.A < T.N - 1 && count < g.A) {
            int i = edges[count].n1;
            int j = edges[count].n2;
            addEdge(&T.nodes[i].edges, j, edges[count].weight);
            addEdge(&T.nodes[j].edges, i, edges[count].weight);
            T.A++;
            if (hasCycle(T)) {
                T.A--;
                removeEdge(&T.nodes[i].edges, j);
                removeEdge(&T.nodes[j].edges, i);
            }
            count++;
        }

    } else {
        printf("Errore di memoria.\n");
        exit(1);
    }
    return T;
}

int compareEdges(const void* a, const void* b) {
    SimpleEdge* e1 = (SimpleEdge*)a;
    SimpleEdge* e2 = (SimpleEdge*)b;
    return e1->weight - e2->weight;
}

void removeEdge(Edge** adjList, int n) {
    if (*adjList != NULL && (*adjList)->neigh == n) {
        Edge* temp = *adjList;
        *adjList = temp->next;
        free(temp);
        return;
    }
    if (*adjList != NULL)
        removeEdge(&(*adjList)->next, n);
    return;
}

void printGraph(Graph g) {
    printf("Grafo con %d nodi e %d archi: \n", g.N, g.A);
    for (int i = 0; i < g.N; i++) {
        printf("Nodo %d con vicini: \n", g.nodes[i].label);
        Edge* e = g.nodes[i].edges;
        while (e) {
            printf("%d->%d(%.0f)", g.nodes[i].label, g.nodes[e->neigh].label, e->weight);
            e = e->next;
        }
        printf("\n");
    }
    return;
}

void freeGraph(Graph g) {
    for (int i = 0; i < g.N; i++) {
        freeEdges(&g.nodes[i].edges);
    }
    free(g.nodes);
    return;
}

void freeEdges(Edge** l){
    if(*l != NULL){
        freeEdges(&(*l)->next);
        free(*l);
        *l = NULL;
    }
    return;
}