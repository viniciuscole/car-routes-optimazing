#include "../include/edge.h"
#include "../include/util.h"
#include "../include/algorithm.h"
#include "../include/updates.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/route.h"

char* format_time(double total_time);
void  write_output(FILE *out_file, double total_time, double total_distance, int* path, int path_length);


int main(int argc, char *argv[])
{
  int node_amount = 0, edge_amount = 0, updates_amount = 0;
  int main_src_node = 0, main_dest_node = 0;
  double intial_velocity;
  int path_length = 0;

  char *input_file = strdup(argv[1]);
  char *output_file = strdup(argv[2]);

  FILE *file = fopen(input_file, "r");

  read_file_header(&node_amount, &edge_amount, &main_src_node, &main_dest_node, &intial_velocity, file);
  AdjList **adjacency_vector = read_edges(file, node_amount, edge_amount, intial_velocity);
  Update *velocity_updates = read_updates(file, &updates_amount);

  // print_list_vector(adjacency_vector, node_amount);
  int *path = calloc((node_amount + 1), sizeof(int));

  double total_time = 0;
  double total_distance = 0;
  
  calculate_path(adjacency_vector, velocity_updates, node_amount, edge_amount, updates_amount, main_src_node, main_dest_node, path, &total_time, 
                &total_distance, &path_length);


  FILE *out_file = fopen(output_file, "w");
  write_output(out_file, total_time, total_distance, path, path_length);

  fclose(file);
  fclose(out_file);
  free(input_file);
  free(output_file);
  free(path);
  free(velocity_updates);
  end_list_vector(adjacency_vector, node_amount);
  return 0;
}

void  write_output(FILE *out_file, double total_time, double total_distance, int* path, int path_length)
{
  char* string_total_time = format_time(total_time);
  total_distance = total_distance / 1000;
  int i = 0;
  for (i = 0; i < path_length; i++)
  {
    fprintf(out_file, "%d;", path[i]);
  }
  fprintf(out_file, "%d\n%f\n%s", path[i], total_distance, string_total_time);
  free(string_total_time);
}


char* format_time(double total_time)
{
  total_time = floor(total_time * 1000.0 + 0.5) / 1000.0;
  int whole = (int)floor(total_time);
  int hours = whole / 3600;
  int minutes = (whole % 3600) / 60;
  double seconds = total_time - hours * 3600 - minutes * 60;
  char string_total_time[100];
  sprintf(string_total_time, "%02d:%02d:%f", hours, minutes, seconds);
  char* formated_time = strdup(string_total_time);
  return formated_time;
}