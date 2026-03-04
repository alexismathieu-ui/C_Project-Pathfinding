#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct n {
    int id;
    struct n **links;
    int link_count;
} Node;

typedef enum {
    FILE_NOT_FOUND = 1,
    NO_START_NODE,
    NO_END_NODE,
    BAD_FILE_FORMAT
} ErrorCode;

Node *find_node(Node **nodes, int size, int id);

void add_link(Node *a, Node *b);

Node **init_node(const char *filename, int *ncount, int *lcount,
                 int *sid, int *eid);

Node **get_unconnected_nodes(Node **nodes, int size, Node *head);

Node *find_by_id(Node **nodes, int n, int id);

void display_nodes(Node *start);

void bfs_path(Node **nodes, int n, Node *start, Node *end);



