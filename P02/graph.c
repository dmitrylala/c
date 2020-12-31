#include "graph.h"


// Initializing the graph
int
graph_init(Graph *graph)
{

    graph -> num_nodes = 0;
    graph -> matrix = NULL;
    graph -> nodes = NULL;

    return 0;
}

// Initializing the graph from already opened file descriptor
int
graph_initf(const int fd, Graph *graph)
{

    if (fd < 0) {
        return BAD_FD;
    }

    graph_init(graph);


    char *arrow = "->\0", *bckt_open = "{\0", *bckt_close = "}\0";
    char *str, *pos, *left_bckt, *right_bckt;
    int i, j, k, key1, key2, inc_weight, err;

    while ((str = str_in(fd)) != NULL) {
        str = str_clear_spaces(str);
        i = substr_c(str, arrow);
        j = substr_c(str, bckt_open);
        k = substr_c(str, bckt_close);
        if ((i > 1) || (j > 1) || (k > 1)) {
            return WRONG_INPUT;
        } else if (i == 1) {
            pos = strstr(str, arrow);
            left_bckt = strstr(str, bckt_open);
            right_bckt = strstr(str, bckt_close);

            if ((((left_bckt == NULL) && (right_bckt != NULL)) ||
                ((left_bckt != NULL) && (right_bckt == NULL))) ||
                (left_bckt > right_bckt) || (&left_bckt[1] == right_bckt) ||
                (pos > left_bckt) || ((strlen(pos) == 2) || (pos == str)) ||
                (&pos[2] == left_bckt)) {
                return WRONG_INPUT;
            }

            ERR(sscanf(str, "%d", &key1), 1, WRONG_INPUT);
            ERR(sscanf(&pos[2], "%d", &key2), 1, WRONG_INPUT); // 2 = strlen(arrow)

            if (left_bckt != NULL) {
                ERR(sscanf(&left_bckt[1], "%d", &inc_weight), 1, WRONG_INPUT);
            } else {
                inc_weight = 1;
            }

            ERR(err = node_add(graph, key1), 0, err);
            ERR(err = node_add(graph, key2), 0, err);
            int cur_weight;
            ERR(err = get_weight(graph, key1, key2, &cur_weight), 0, err);
            ERR(err = set_weight(graph, key1, key2, cur_weight + inc_weight), 0,
                err);

        } else if (i == 0) {

            if ((j == 1) || (k == 1)) {
                return WRONG_INPUT;
            }

            ERR(sscanf(str, "%d", &key1), 1, WRONG_INPUT);
            ERR(err = node_add(graph, key1), 0, err);
        }

        free(str);
    }

    return 0;
}

/*
    deinitializing the graph
    considering that graph was initialized
*/
int
graph_final(Graph *graph)
{

    matrix_free(&(graph -> matrix), graph -> num_nodes);
    free(graph -> nodes);

    graph -> num_nodes = 0;
    graph -> matrix = NULL;
    graph -> nodes = NULL;

    return 0;
}

/*
    setting the weight value
    considering that graph was initialized
*/
int
set_weight(Graph *graph, const int key1, const int key2, const int weight)
{

    if (weight < 0) {
        return WRONG_WEIGHT;
    }

    int num1, num2, err;
    ERR(err = get_num(graph, key1, &num1), 0, err);
    ERR(err = get_num(graph, key2, &num2), 0, err);

    graph -> matrix[num1][num2] = weight;

    return 0;
}

/*
    adding a new node by key
    considering that graph was initialized
*/
int
node_add(Graph *graph, const int key)
{

    if (key < 0) {
        return WRONG_KEY;
    }

    int num;
    int flag1 = get_num(graph, key, &num);
    int flag2 = get_first_empty(graph, &num);
    if ((flag1 == NO_SUCH_NODE) && (flag2 == NO_SUCH_NODE)) {
        Node new_node;
        new_node.degree = 0;
        new_node.key = key;
        new_node.number = graph -> num_nodes;
        new_node.existent = 1;
        graph -> num_nodes += 1;

        int n = graph -> num_nodes;
        ERR_MEM(graph -> nodes = realloc(graph -> nodes, n *
                    sizeof *(graph -> nodes)), ERR_MEM_ALLOC);

        graph -> nodes[n - 1] = new_node;

        Matrix matrix = graph -> matrix;
        ERR_MEM(matrix = realloc(matrix, n * sizeof *matrix), ERR_MEM_ALLOC);
        ERR_MEM(matrix[n - 1] = malloc(n * sizeof **matrix), ERR_MEM_ALLOC);

        for (int i = 0; i < n - 1; ++i) {
            ERR_MEM(matrix[i] = realloc(matrix[i], n * sizeof **matrix),
                        ERR_MEM_ALLOC);
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if ((j == n - 1) || (i == n - 1)) {
                    matrix[i][j] = 0;
                }
            }
        }
        graph -> matrix = matrix;
    } else if (flag2 == 0) {
        Node new_node;
        new_node.degree = 0;
        new_node.key = key;
        new_node.number = num;
        new_node.existent = 1;

        graph -> nodes[num] = new_node;
    }

    return 0;
}

/*
    "deleting" a node by key
    considering that graph was initialized
*/
int
node_delete(Graph *graph, const int key)
{
    int num, err;
    ERR(err = get_num(graph, key, &num), 0, err);

    int n = graph -> num_nodes;
    graph -> nodes[num].existent = 0;

    // Deleting edges
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if ((i == num) || (j == num)) {
                graph -> matrix[i][j] = 0;
            }
        }
    }

    // Recalculating degrees
    for (int i = 0; i < n; ++i) {
        graph -> nodes[i].degree = arr_calc_sum(graph -> matrix[i], n);
    }

    return 0;
}

/*
    finding a number of node by key
    considering that graph was initialized
*/
int
get_num(const Graph *graph, const int key, int *num)
{

    for (int i = 0; i < graph -> num_nodes; ++i) {
        Node node = graph -> nodes[i];
        if ((node.key == key) && (node.existent == 1)) {
            *num = i;
            return 0;
        }
    }

    return NO_SUCH_NODE;
}

/*
    function returns the number of first node with existent = 0
    if all nodes exist, returns 1
    considering that graph was initialized
*/
int
get_first_empty(const Graph *graph, int *num)
{

    for (int i = 0; i < graph -> num_nodes; ++i) {
        if (graph -> nodes[i].existent == 0) {
            *num = i;
            return 0;
        }
    }

    return NO_SUCH_NODE;
}

/*
    function gets a degree of node by it's key
    considering that graph was initialized
*/
int
get_degree(const Graph *graph, const int key, int *degree)
{
    int num, err;
    ERR(err = get_num(graph, key, &num), 0, err);

    *degree = graph -> nodes[num].degree;

    return 0;
}

/*
    function gets a number of nodes in graph
    considering that graph was initialized
*/
int
get_num_nodes(const Graph *graph)
{
    return graph -> num_nodes;
}

/*
    function gets weight from node with key1 to node with key2
    considering that graph was initialized
*/
int
get_weight(const Graph *graph, const int key1, const int key2, int *weight)
{

    int num1, num2, err;
    ERR(err = get_num(graph, key1, &num1), 0, err);
    ERR(err = get_num(graph, key2, &num2), 0, err);

    *weight = graph -> matrix[num1][num2];

    return 0;
}

/*
    printing the whole information about graph to stdout
    considering that graph was initialized
*/
int
graph_print(const Graph graph)
{

    // printing matrix
    int n = graph.num_nodes;
    Matrix matrix = graph.matrix;
    Node *list = graph.nodes;
    printf("Adjacency matrix:\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if ((list[i].existent == 1) && (list[j].existent == 1)) {
                printf("%3d ", matrix[i][j]);
            }
        }
        if (list[i].existent == 1) {
            printf("\n");
        }
    }

    // printing nodes
    printf("List of nodes:\n");
    int c = 0;
    for (int i = 0; i < n; ++i) {
        if (list[i].existent == 1) {
            ++c;
            printf("number: %3d   key: %3d   degree: %3d\n",
                list[i].number, list[i].key, list[i].degree);
        }
    }

    printf("Number of nodes: %d\n", c);
    printf("\n");

    return 0;
}

/*
    function returns the length of shortest path from key1 to key2 in path var
    considering that graph was initialized
*/
int
shortest_path(const Graph *graph, const int key1, const int key2, int *path)
{

    int num1, num2, err;
    ERR(err = get_num(graph, key1, &num1), 0, err);
    ERR(err = get_num(graph, key2, &num2), 0, err);


    int n = graph -> num_nodes;

    // copying matrix
    Matrix matrix;
    ERR_MEM(matrix = malloc(n * sizeof *matrix), ERR_MEM_ALLOC);
    for (int i = 0; i < n; ++i) {
        ERR_MEM(matrix[i] = malloc(n * sizeof **matrix), ERR_MEM_ALLOC);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = graph -> matrix[i][j];
        }
    }


    // computing the matrix of shortest distances
    int max_sum = 0;
    matrix_shortest_dists_compute(&matrix, n, &max_sum);


    int result;
    if (matrix[num1][num2] == max_sum) {
        result = -1;
    } else {
        result = matrix[num1][num2];
    }

    matrix_free(&matrix, n);

    if (result == -1) {
        return NO_PATH;
    } else {
        *path = result;
        return 0;
    }
}

/*
    function returns the key of median node
    considering that graph was initialized
*/
int
median(const Graph *graph, int *median)
{

    int n = graph -> num_nodes;

    // copying matrix
    Matrix matrix;
    ERR_MEM(matrix = malloc(n * sizeof *matrix), ERR_MEM_ALLOC);
    for (int i = 0; i < n; ++i) {
        ERR_MEM(matrix[i] = malloc(n * sizeof **matrix), ERR_MEM_ALLOC);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = graph -> matrix[i][j];
        }
    }


    // computing the matrix of shortest distances
    int max_sum = 0;
    matrix_shortest_dists_compute(&matrix, n, &max_sum);


    // computing the minimum sum of rows
    int *sum_rows;
    ERR_MEM(sum_rows = malloc(n * sizeof *sum_rows), ERR_MEM_ALLOC);
    for (int i = 0; i < n; ++i) {
        sum_rows[i] = arr_calc_sum(matrix[i], n);
    }

    int min = n * max_sum;
    int min_num = -1;

    for (int i = 0; i < n; ++i) {
        if ((sum_rows[i] <= min) && (graph -> nodes[i].existent == 1)) {
            min = sum_rows[i];
            min_num = i;
        }
    }
    free(sum_rows);

    matrix_free(&matrix, n);

    if (min_num == -1) {
        return NO_MEDIAN;
    } else {
        *median = graph -> nodes[min_num].key;
        return 0;
    }
}
