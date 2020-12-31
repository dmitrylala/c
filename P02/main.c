/*
    Let's solve the following problem: output the keys of the graph vertices
    in ascending order of the sum of the shortest distances to all other vertices
*/

#include <fcntl.h>

#include "graph.h"



int main(void) {

    // Initializating graph with initial value from file
    const char *file = "graph.txt";
    int fd = open(file, O_RDONLY);

    Graph graph;
    int err;
    err = graph_initf(fd, &graph);
    close(fd);
    SAFE(err);

    graph_print(graph);

    // calculating medians
    int n = get_num_nodes(&graph);
    int key;
    printf("Medians: ");
    for (int i = 0; i < n; ++i) {
        SAFE(median(&graph, &key));
        SAFE(node_delete(&graph, key));
        printf("%d ", key);
    }
    printf("\n");


    // finalize
    graph_final(&graph);

    return 0;
}
