#ifndef route_h
#define route_h

#include "adjacency.h"
#include "updates.h"

void calculate_path(AdjList **adjacency_vector, Update *velocity_updates, int node_amount, int edge_amount,
                    int updates_amount, int main_src_node, int main_dest_node, int *path, double *total_time,
                    double *total_distance, int *path_length);

#endif
