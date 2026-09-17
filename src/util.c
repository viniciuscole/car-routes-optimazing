#include <stdio.h>
#include <stdlib.h>
#include "../include/util.h"

// Implementations=========================================//
void read_file_header(int *node_amount, int *edge_amount, int *src_node, int *dest_node, double *velocity, FILE *file)
{
    fscanf(file, "%d;%d\n%d;%d\n%lf\n", node_amount, edge_amount, src_node, dest_node, velocity);
}

// Depracated
//======================================================//
// Edge *read_edges(FILE *file, int edge_amount)
// {
//     // Reading velocity
//     double velocity = 0.0;
//     fscanf(file, "%lf\n", &velocity);

//     Edge *edge_vector = malloc(sizeof(Edge) * edge_amount);
//     int src, dest;
//     double distance, weight;

//     for (int x = 0; x < edge_amount; x++)
//     {
//         fscanf(file, "%d;%d;%lf\n", &src, &dest, &distance);
//         weight = calculate_weight(distance, velocity); // weight = time
//         edge_vector[x] = init_edge(src, dest, distance, weight);
//     }
//     return edge_vector;
// }

// Reading edges and creating adjacency vector
//======================================================//
AdjList **read_edges(FILE *file, int node_amount, int edge_amount, double velocity)
{

    AdjList **list_vector = malloc(sizeof(AdjList *) * node_amount);
    for (int x = 0; x < node_amount; x++)
    {
        list_vector[x] = init_list();
    }

    Adj adj;
    int src, dest;
    double distance, weight;

    for (int x = 0; x < edge_amount; x++)
    {
        fscanf(file, "%d;%d;%lf\n", &src, &dest, &distance);
        adj = init_adj(dest, distance, calculate_weight(distance, velocity));
        list_push(list_vector[src - 1], adj);
    }

    return list_vector;
}

//======================================================//
Update *read_updates(FILE *file, int *updates_amount)
{
    // Getting current seek
    long file_seek = ftell(file);

    // Counting number of updates to allocate memory
    int line_count = 0, c, at_line_start = 1;
    while ((c = fgetc(file)) != EOF)
    {
        if (at_line_start && c != '\n') line_count++;
        at_line_start = (c == '\n');
    }

    // Reseting seek after counting
    fseek(file, file_seek, SEEK_SET);

    // Assigning value of updates amount
    *updates_amount = line_count;

    // Reading updates
    Update *updates = malloc(sizeof(Update) * (line_count > 0 ? line_count : 1));
    int src_node, dest_node;
    double instant_time, new_velocity;
    for (int i = 0; i < line_count; i++)
    {
        if (fscanf(file, "%lf;%d;%d;%lf\n", &instant_time, &src_node, &dest_node, &new_velocity) != 4)
        {
            *updates_amount = i;
            break;
        }
        updates[i] = init_update(src_node, dest_node, instant_time, new_velocity);
    }
    return updates;
}

//======================================================//
double calculate_weight(double distance, double velocity)
{
    // Transforming velocity to m/s
    // Returning time in seconds
    velocity = velocity / 3.6;
    return distance / velocity;
}
