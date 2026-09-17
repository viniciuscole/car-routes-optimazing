#include "../include/trace.h"
#include "../include/route.h"
#include "../include/util.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void on_pop(int node, double time) { printf("pop %d %.3f\n", node, time); }
static void on_relax(int from, int to, double t) { printf("relax %d %d %.3f\n", from, to, t); }
static void on_plan(const int *path, int length, double eta)
{
    printf("plan");
    for (int i = 0; i < length; i++) printf(" %d", path[i]);
    if (eta >= DBL_MAX) printf(" unreachable\n");
    else printf(" %.3f\n", eta);
}
static void on_unreachable(int node, double clock) { printf("unreachable %d %.3f\n", node, clock); }
static void on_replan(int node, double clock) { printf("replan %d %.3f\n", node, clock); }
static void on_move(int from, int to, double t0, double t1) { printf("move %d %d %.3f %.3f\n", from, to, t0, t1); }
static void on_update(int from, int to, double kmh, double clock) { printf("update %d %d %.3f %.3f\n", from, to, kmh, clock); }
static void on_done(double clock, double km) { printf("done %.3f %.3f\n", clock, km); }

static void print_graph_edge(Adj adj, void *ctx)
{
    int from = *(int *)ctx;
    double kmh = get_time(adj) > 0 ? get_distance(adj) / get_time(adj) * 3.6 : 0;
    printf("edge %d %d %.0f %.3f\n", from, get_node_id(adj), get_distance(adj), kmh);
}

int run_trace(const char *input)
{
    FILE *file = fmemopen((void *)input, strlen(input), "r");
    if (!file) return 1;

    int node_amount = 0, edge_amount = 0, updates_amount = 0, src = 0, dest = 0;
    double velocity = 0;
    read_file_header(&node_amount, &edge_amount, &src, &dest, &velocity, file);
    if (node_amount <= 0 || edge_amount < 0 || src < 1 || src > node_amount || dest < 1 || dest > node_amount || velocity <= 0 ||
        node_amount > 1000000 || edge_amount > 1000000) {
        fclose(file);
        return 2;
    }
    AdjList **graph = read_edges(file, node_amount, edge_amount, velocity);
    if (!graph) {
        fclose(file);
        return 3;
    }
    Update *updates = read_updates(file, &updates_amount);
    fclose(file);

    printf("graph %d %d\n", node_amount, edge_amount);
    for (int i = 1; i <= node_amount; i++) list_for_each(graph[i - 1], print_graph_edge, &i);

    trace.pop = on_pop; trace.relax = on_relax; trace.plan = on_plan; trace.unreachable = on_unreachable;
    trace.replan = on_replan; trace.move = on_move; trace.update = on_update; trace.done = on_done;

    int *path = calloc(node_amount + 1, sizeof(int));
    double total_time = 0, total_distance = 0;
    int path_length = 0;
    calculate_path(graph, updates, node_amount, edge_amount, updates_amount, src, dest, path, &total_time, &total_distance, &path_length);
    fflush(stdout);

    free(path);
    free(updates);
    end_list_vector(graph, node_amount);
    return 0;
}
