#include "utilities.h"

#define BAD_FD 1
#define WRONG_INPUT 2
#define WRONG_WEIGHT 3
#define WRONG_KEY 4
#define NO_SUCH_NODE 5
#define NO_PATH 6
#define NO_MEDIAN 7


// Node of graph
typedef struct
{
    int key;
    int number;
    int degree;
    int existent;
} Node;


// Directed graph
typedef struct
{
    int num_nodes;
    Matrix matrix; // adjacency matrix
    Node *nodes;
} Graph;


// init/deinit functions
int
graph_init(Graph *graph);

int
graph_initf(const int fd, Graph *graph);

int
graph_final(Graph *graph);


// functions working with edges
int
set_weight(Graph *graph, const int key1, const int key2, const int weight);


// functions working with nodes
int
node_add(Graph *graph, const int key);

int
node_delete(Graph *graph, const int key);


// functions that get parameters
int
get_num(const Graph *graph, const int key, int *num);

int
get_first_empty(const Graph *graph, int *num);

int
get_degree(const Graph *graph, const int key, int *degree);

int
get_num_nodes(const Graph *graph);

int
get_weight(const Graph *graph, const int key1, const int key2, int *weight);


// graph processing
int
graph_print(const Graph graph);

int
shortest_path(const Graph *graph, const int key1, const int key2, int *path);

int
median(const Graph *graph, int *median);