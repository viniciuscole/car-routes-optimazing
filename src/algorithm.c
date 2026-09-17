#include "../include/algorithm.h"
#include "../include/edge.h"
#include "../include/adjacency.h"
#include "../include/PQ.h"
#include "../include/util.h"
#include "../include/trace.h"
#include <stdlib.h>
#include <stdio.h>

#define INFINITY __DBL_MAX__

// Prototypes =============================================//
void dijkstra(AdjList **graph, int node_amount, int main_src_node, int main_dest_node, int *path, double *distanceTraveled, double *timeTraveled, int edge_amount, int *path_length);
void updateDistanceCallback(Adj adj, int v_id, int *visited, double *dist, double *time, PQ pq, int *path);
void printPath(int *path, int node);
void savePath(int *path, int node, int *pathArray, int *pathLength);

// Implementations=========================================//
void dijkstra(AdjList **graph, int node_amount, int main_src_node, int main_dest_node, int *path, double *distanceTraveled, double *timeTraveled, int edge_amount, int *path_length)
{
    double time[node_amount + 1];
    double dist[node_amount + 1];
    int visited[node_amount + 1];
    path_length[0] = 0;

    for (int i = 0; i < node_amount + 1; i++)
        path[i] = 0;

    // Initializing dist and visited

    for (int i = 1; i <= node_amount; i++)
    {
        dist[i] = INFINITY;
        path[i] = 0;
        visited[i] = 0;
        time[i] = INFINITY;
    }

    // Initializing priority queue
    PQ pq = (PQ)malloc(sizeof(struct priority_queue));
    PQ_init(pq, edge_amount);
    // Setting main source node distance to 0

    dist[main_src_node] = 0;
    time[main_src_node] = 0;
    PQ_insert(pq, init_adj(main_src_node, 0, 0));

    // Dijkstra's algorithm

    while (pq->N != 0)
    {
        Adj v = PQ_delmin(pq);
        int v_id = id(v);
        if (visited[v_id]) continue;
        visited[v_id] = 1;
        if (trace.pop) trace.pop(v_id, time[v_id]);

        AdjList *list = graph[v_id - 1];

        // Updating distance and time
        // Travelling through adjacency list
        list_traversal(list, updateDistanceCallback, v_id, visited, dist, time, pq, path);
    }

    // Saving results

    distanceTraveled[0] = dist[main_dest_node];
    timeTraveled[0] = time[main_dest_node];

    // Saving path
    int pathArray[node_amount];
    savePath(path, main_dest_node, pathArray, path_length);
    for (int i = 0; i < *path_length; i++)
    {
        path[i] = pathArray[i];
    }
    // path can be acessed in main function

    if (trace.plan) trace.plan(path, *path_length, time[main_dest_node]);

    PQ_finish(pq);
}

void savePath(int *path, int node, int *pathArray, int *path_length)
{
    if (path[node] != 0)
    {
        savePath(path, path[node], pathArray, path_length);
    }

    pathArray[*path_length] = node;
    (*path_length)++;
}

void updateDistanceCallback(Adj adj, int v_id, int *visited, double *dist, double *time, PQ pq, int *path)
{
    int w_id = id(adj);
    if (visited[w_id] == 0)
    {
        if (time[w_id] > time[v_id] + value(adj))
        {

            // Updating distance and time
            time[w_id] = time[v_id] + value(adj);
            dist[w_id] = dist[v_id] + dist(adj);

            path[w_id] = v_id;
            if (trace.relax) trace.relax(v_id, w_id, time[w_id]);
            PQ_insert(pq, init_adj(w_id, dist[w_id], time[w_id]));
        }
    }
}

// auxiliar function
void printPath(int *path, int node)
{
    if (path[node] == 0)
    {
        printf("%d", node);
    }
    else
    {
        printPath(path, path[node]);
        printf(" -> %d", node);
    }
}
