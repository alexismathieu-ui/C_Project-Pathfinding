#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"

void pathfinding(const char *filename) {
    int ncount, lcount, sid, eid;
    Node **nodes = init_node(filename, &ncount, &lcount, &sid, &eid);
    printf("nodes: %d\n", ncount);
    printf("links: %d\n", lcount);
    printf("start: %d\n", sid);
    printf("end: %d\n", eid);
    Node *start = find_by_id(nodes, ncount, sid);
    Node *end = find_by_id(nodes, ncount, eid);
    printf("unconnected nodes:\n");
    get_unconnected_nodes(nodes, ncount, start);
    printf("pathfinding:\n");
    bfs_path(nodes, ncount, start, end);
}

/* Main */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <graph_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    pathfinding(argv[1]);
    return EXIT_SUCCESS;
}



