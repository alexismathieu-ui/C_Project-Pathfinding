#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"

/* Cherche un noeud par id dans un tableau de noeuds */
Node *find_node(Node **nodes, int size, int id) {
    for (int i = 0; i < size; i++)
        if (nodes[i]->id == id) return nodes[i];
    return NULL;
}

/* Ajoute une liaison entre deux noeuds */
void add_link(Node *a, Node *b) {
    a->links = realloc(a->links, (a->link_count + 1) * sizeof(Node *));
    a->links[a->link_count++] = b;
}

/* Initialise les noeuds et liens depuis le fichier */
/* Helpers to keep init_node short */
static FILE *open_graph(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { fprintf(stderr, "Error: file not found\n"); exit(FILE_NOT_FOUND); }
    return f;
}

static void add_node(Node ***nodes, int *size, int id,
                     bool in_start, bool in_end, int *sid, int *eid) {
    *nodes = realloc(*nodes, (*size + 1) * sizeof(Node *));
    (*nodes)[*size] = calloc(1, sizeof(Node));
    (*nodes)[*size]->id = id;
    if (in_start && *sid == -1) *sid = id;
    if (in_end && *eid == -1) *eid = id;
    (*size)++;
}

static void process_link_line(Node **nodes, int size, const char *line, int *links) {
    int a, b;
    if (sscanf(line, "%d-%d", &a, &b) == 2) {
        Node *na = find_node(nodes, size, a);
        Node *nb = find_node(nodes, size, b);
        if (na && nb) { add_link(na, nb); add_link(nb, na); (*links)++; }
    }
}

/* Initialise les noeuds et liens depuis le fichier (<= 20 lignes) */
Node **init_node(const char *filename, int *ncount, int *lcount,
                 int *sid, int *eid) {
    FILE *f = open_graph(filename);
    char line[128]; Node **nodes = NULL; int size = 0, links = 0;
    *sid = -1; *eid = -1; bool links_sec = false, in_start = false, in_end = false;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') {
            if (strncmp(line, "#start", 6) == 0) { in_start = true; in_end = false; continue; }
            if (strncmp(line, "#end", 4) == 0)   { in_end = true; in_start = false; continue; }
            if (strncmp(line, "#links", 6) == 0) { links_sec = true; in_start = false; in_end = false; continue; }
            continue;
        }
        if (links_sec && strchr(line, '-')) { process_link_line(nodes, size, line, &links); continue; }
        int id = atoi(line); if (id == 0) continue;
        add_node(&nodes, &size, id, in_start, in_end, sid, eid);
    }

    fclose(f);
    if (size == 0) { fprintf(stderr, "Error: bad format\n"); exit(BAD_FILE_FORMAT); }
    if (*sid == -1) { fprintf(stderr, "Error: no start node\n"); exit(NO_START_NODE); }
    if (*eid == -1) { fprintf(stderr, "Error: no end node\n"); exit(NO_END_NODE); }
    *ncount = size; *lcount = links; return nodes;
}

/* Affiche les noeuds accessibles depuis start (BFS) */
void display_nodes(Node *start) {
    bool visited[1000] = { false };
    Node *queue[1000]; int f = 0, r = 0;
    queue[r++] = start; visited[start->id] = true;

    while (f < r) {
        Node *cur = queue[f++]; printf("%d ", cur->id);
        for (int i = 0; i < cur->link_count; i++) {
            Node *nxt = cur->links[i];
            if (!visited[nxt->id]) {
                visited[nxt->id] = true; queue[r++] = nxt;
            }
        }
    }
    printf("\n");
}

/* Retourne la liste des noeuds non connectés */
Node **get_unconnected_nodes(Node **nodes, int size, Node *head) {
    bool conn[1000] = { false }; Node *q[1000]; int f = 0, r = 0;
    q[r++] = head; conn[head->id] = true;
    while (f < r) {
        Node *cur = q[f++];
        for (int i = 0; i < cur->link_count; i++) {
            Node *nxt = cur->links[i];
            if (!conn[nxt->id]) { conn[nxt->id] = true; q[r++] = nxt; }
        }
    }
    Node **unc = NULL; int count = 0;
    for (int i = 0; i < size; i++) {
        if (!conn[nodes[i]->id]) {
            unc = realloc(unc, (count + 1) * sizeof(Node *));
            unc[count++] = nodes[i];
        }
    }
    if (count == 0) printf("none\n");
    else { for (int i = 0; i < count; i++) printf("%d ", unc[i]->id);
           printf("\n"); }
    return unc;
}

/* Recherche un noeud par son id */
Node *find_by_id(Node **nodes, int n, int id) {
    for (int i = 0; i < n; i++)
        if (nodes[i]->id == id) return nodes[i];
    return NULL;
}

/* Trouve et affiche le plus court chemin entre start et end */
void bfs_path(Node **nodes, int n, Node *start, Node *end) {
    (void)nodes; (void)n;
    Node *q[1000]; int prev[1000]; bool vis[1000] = { false };
    for (int i = 0; i < 1000; i++) prev[i] = -1;
    int f = 0, r = 0; q[r++] = start; vis[start->id] = true;

    while (f < r) {
        Node *cur = q[f++]; if (cur == end) break;
        for (int i = 0; i < cur->link_count; i++) {
            Node *nxt = cur->links[i];
            if (!vis[nxt->id]) {
                vis[nxt->id] = true; prev[nxt->id] = cur->id; q[r++] = nxt;
            }
        }
    }
    if (!vis[end->id]) { printf("no path found\n"); return; }

    int path[1000], len = 0;
    for (int at = end->id; at != -1; at = prev[at]) path[len++] = at;
    for (int i = len - 1; i >= 0; i--) printf("%d ", path[i]);
    printf("\n");
}


