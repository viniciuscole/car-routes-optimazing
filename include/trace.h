#ifndef trace_h
#define trace_h

typedef struct {
  void (*pop)(int node, double time);
  void (*relax)(int from, int to, double new_time);
  void (*plan)(const int *path, int length, double eta);
  void (*unreachable)(int node, double clock);
  void (*replan)(int node, double clock);
  void (*move)(int from, int to, double t0, double t1);
  void (*update)(int from, int to, double kmh, double clock);
  void (*done)(double clock, double km);
} TraceHooks;

extern TraceHooks trace;

int run_trace(const char *input);

#endif
