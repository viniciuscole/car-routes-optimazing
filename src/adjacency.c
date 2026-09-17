#include "../include/adjacency.h"
#include "../include/PQ.h"
#include <stdio.h>
#include <stdlib.h>

Adj init_adj(int node_id, double distance, double time)
{
    Adj adj;
    adj.node_id = node_id;
    adj.distance = distance;
    adj.time = time;
    return adj;
}

void print_adj(Adj adj)
{
    printf("%d, %lf, %lf\n", adj.node_id, adj.distance, adj.time);
}

//===============//
typedef struct cell Cell;
struct cell
{
    Adj adj;
    Cell *next;
};

struct adj_list
{
    Cell *first;
    Cell *last;
};

//========================//
AdjList *init_list()
{
    AdjList *ad_list = malloc(sizeof(AdjList));
    ad_list->first = NULL;
    ad_list->last = NULL;
    return ad_list;
}

double get_time(Adj adj)
{
    return adj.time; 
}

double get_distance(Adj adj)
{
    return adj.distance; 
}

int get_node_id(Adj adj)
{
    return adj.node_id;
}

Adj set_time(Adj adj, double new_velocity)
{
    new_velocity = new_velocity / 3.6;
    adj.time = adj.distance / new_velocity;
    return adj;
}

// You can  add parameters to this function too
void list_traversal(AdjList *list, ListCallback callback, int v_id, int *visited, double *dist, double *time, PQ pq, int *path)
{

    if (!list)
        perror("Could not cross unexisting list!\n");

    Cell *current = list->first;
    while (current)
    {
        // Define what callback gonna do
        callback(current->adj, v_id, visited, dist, time, pq, path);
        current = current->next;
    }
}

void list_for_each(AdjList *list, void (*fn)(Adj adj, void *ctx), void *ctx)
{
    for (Cell *cell = list->first; cell != NULL; cell = cell->next)
        fn(cell->adj, ctx);
}

void list_push(AdjList *list, Adj adj)
{

    if (!list)
        perror("Could not push item into unexisting list!\n");

    Cell *new_cell = malloc(sizeof(Cell));
    new_cell->next = NULL;
    new_cell->adj = adj;

    if (!list->first)
    {
        list->first = new_cell;
        list->last = new_cell;
        return;
    }
    list->last->next = new_cell;
    list->last = new_cell;
}

void end_list(AdjList *list)
{
    if (list)
    {
        Cell *current = list->first, *aux = NULL;
        while (current)
        {
            aux = current->next;
            free(current);
            current = aux;
        }
        free(list);
    }
}

void end_list_vector(AdjList **vector, int size)
{
    if (vector)
    {
        for (int x = 0; x < size; x++)
        {
            end_list(vector[x]);
        }
        free(vector);
    }
}

void print_list(AdjList *list)
{
    Cell *cell = list->first;

    while (cell != NULL)
    {
        print_adj(cell->adj);
        cell = cell->next;
    }
}
void print_list_vector(AdjList **vector, int size)
{
    if (vector)
    {
        for (int x = 0; x < size; x++)
        {
            printf("node %d:\n", x + 1);
            print_list(vector[x]);
            printf("END\n\n");
        }
    }
}

Adj find_adj_list(AdjList *list, int node_id)
{
    Cell *cell = list->first;
    while(node_id != get_node_id(cell->adj)){
        cell = cell->next;
    }
    return cell->adj;
}

void update_adj(AdjList *list, int node_id, double new_velocity)
{
    Cell *cell = list->first;
    while(node_id != get_node_id(cell->adj)){
        cell = cell->next;
    }
    cell->adj = set_time(cell->adj, new_velocity);
}