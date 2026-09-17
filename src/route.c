#include "../include/route.h"
#include "../include/algorithm.h"
#include "../include/trace.h"
#include <stdlib.h>

TraceHooks trace = {0};

void calculate_path(AdjList **adjacency_vector, Update* velocity_updates, int node_amount, int edge_amount, int updates_amount, int main_src_node,
                   int main_dest_node, int *path, double *total_time, double *total_distance, int *path_length)
{
  path[0] = main_src_node;
  double distanceTravelled = 0;
  double timeTravelled = 0;

  // Initializing temporary path (may change according to updates)
  int *temporary_path = calloc((node_amount + 1), sizeof(int));
  int temp_path_length = 0;

  int index_uptades = 0;
  // While there are uptades
  while(index_uptades < updates_amount)
  {
    if (trace.replan) trace.replan(path[path_length[0]], total_time[0]);
    dijkstra(adjacency_vector, node_amount, path[path_length[0]], main_dest_node, temporary_path, &distanceTravelled, &timeTravelled, edge_amount, &temp_path_length);

    if (temp_path_length == 1 && path[path_length[0]] != main_dest_node)
    {
      if (trace.unreachable) trace.unreachable(path[path_length[0]], total_time[0]);
      free(temporary_path);
      return;
    }

    double next_uptade_time = get_instant_time(velocity_updates[index_uptades]);

    int index_temporary_path = 1;
    while(next_uptade_time > total_time[0])
    {
      // Verify if the current node is the destination node;
      if(temporary_path[index_temporary_path - 1] == main_dest_node)
      {
        if (trace.done) trace.done(total_time[0], total_distance[0] / 1000.0);
        free(temporary_path);
        return;
      }
      // Get the distance and the time between the current node and the next one
      double t0 = total_time[0];
      total_time[0] += get_time(find_adj_list(adjacency_vector[temporary_path[index_temporary_path - 1]-1], temporary_path[index_temporary_path]));
      total_distance[0] += get_distance(find_adj_list(adjacency_vector[temporary_path[index_temporary_path - 1]-1], temporary_path[index_temporary_path]));
      if (trace.move) trace.move(temporary_path[index_temporary_path - 1], temporary_path[index_temporary_path], t0, total_time[0]);

      // Add node to the final path
      path[++path_length[0]] = temporary_path[index_temporary_path++];
    }
    while(total_time[0] >= next_uptade_time)
    {
      update_adj(adjacency_vector[get_src_node(velocity_updates[index_uptades])-1], get_src_node(velocity_updates[index_uptades]),
                 get_dest_node(velocity_updates[index_uptades]), get_new_velocity(velocity_updates[index_uptades]));
      if (trace.update) trace.update(get_src_node(velocity_updates[index_uptades]), get_dest_node(velocity_updates[index_uptades]), get_new_velocity(velocity_updates[index_uptades]), total_time[0]);

      index_uptades++;
      if(index_uptades == updates_amount) break;
      next_uptade_time = get_instant_time(velocity_updates[index_uptades]);
    }
  }

  // No updates left
  if (trace.replan) trace.replan(path[path_length[0]], total_time[0]);
  dijkstra(adjacency_vector, node_amount, path[path_length[0]], main_dest_node, temporary_path, &distanceTravelled, &timeTravelled, edge_amount, &temp_path_length);

  if (temp_path_length == 1 && path[path_length[0]] != main_dest_node)
  {
    if (trace.unreachable) trace.unreachable(path[path_length[0]], total_time[0]);
    free(temporary_path);
    return;
  }

  int index_temporary_path = 1;
  while(path[path_length[0]] != main_dest_node)
  {
    // Get the distance and the time between the current node and the next one
    double t0 = total_time[0];
    total_time[0] += get_time(find_adj_list(adjacency_vector[temporary_path[index_temporary_path - 1]-1], temporary_path[index_temporary_path]));
    total_distance[0] += get_distance(find_adj_list(adjacency_vector[temporary_path[index_temporary_path - 1]-1], temporary_path[index_temporary_path]));
    if (trace.move) trace.move(temporary_path[index_temporary_path - 1], temporary_path[index_temporary_path], t0, total_time[0]);

    // Add node to the final path
    path[++path_length[0]] = temporary_path[index_temporary_path++];
  }

  if (trace.done) trace.done(total_time[0], total_distance[0] / 1000.0);
  free(temporary_path);
}
