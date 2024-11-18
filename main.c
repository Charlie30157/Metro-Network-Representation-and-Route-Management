#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 20 // Maximum number of stations and lines
#define INF 100000 // A large number to represent infinity for distance calculations

// Struct representing an intersection station in the metro network
typedef struct intersection {
    char station[50]; // Name of the station
    int lines; // Number of lines the station is part of (useful to identify intersections)
} intersection;

// Node structure for linked list representation of adjacency lists
typedef struct node {
    char info[50]; // Name of the station connected to
    int fare; // Fare (cost) to travel from one station to another
    struct node *next; // Pointer to the next node in the adjacency list
} NODE;

// Structure for each line in the metro network
typedef struct Line {
    char line_name[50]; // Name of the line
    char stations[MAX][50]; // Array holding names of stations in the line
    int num_stations; // Number of stations in this line
} Line;

// Graph structure representing the metro network
typedef struct Graph {
    NODE* V[MAX]; // Array of adjacency lists, where each list represents a station and its connections
    Line lines[MAX]; // Array of lines in the metro network
    int visited[MAX]; // Array for keeping track of visited stations
    int station_count; // Number of unique stations in the network
    char stations[MAX][50]; // Array holding unique station names
} Graph;

// Function declarations
void initGraph(Graph *graph);
void createGraph(Graph *graph);
void addEdge(Graph *graph, char *src, char *dest, int fare);
int findStationIndex(Graph *graph, char *station_name);
void printGraph(Graph *graph);
void station_Details(Graph *graph, char *station_name);
void findIntersectingStations(Graph *graph);
void findNearestIntersection(Graph *graph);
void findTerminals(Graph *graph);
void possibleroutes(Graph *graph);
void printAllPathsUtil(Graph* graph, int u, int d, bool visited[], int path[], int pathIndex, int pathCost);

// Main function to drive the program
int main() {
    Graph graph;
    initGraph(&graph); // Initialize graph
    createGraph(&graph); // Populate graph with edges and lines

    int choice;
    do {
        printf("\n1. Find Possible Routes\n");
        printf("2. Station Details\n");
        printf("3. Find Intersecting Stations\n");
        printf("4. Find Terminals\n");
        printf("5. Find Nearest Intersection\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                possibleroutes(&graph); // List possible routes between two stations
                break;
            case 2:
                printf("\nEnter station name to get details: ");
                char station_name[50];
                scanf("%s", station_name);
                station_Details(&graph, station_name); // Display details for a specific station
                break;
            case 3:
                findIntersectingStations(&graph); // Display stations where multiple lines intersect
                break;
            case 4:
                findTerminals(&graph); // Display the terminals of each line
                break;
            case 5:
                findNearestIntersection(&graph); // Find nearest intersection station from a starting point
                break;
        }
    } while(choice < 6);

    return 0;
}

// Initialize the graph by setting each station's adjacency list and visited status to default
void initGraph(Graph *graph) {
    for (int i = 0; i < MAX; i++) {
        graph->V[i] = NULL;
        graph->visited[i] = 0;
    }
    graph->station_count = 0;
}

// Function to create the graph by adding edges and lines
void createGraph(Graph *graph) {
    int n, line_count;
    printf("Enter the number of edges in the metro network:\n");
    scanf("%d", &n);
    
    printf("Enter the edge information (source, destination, fare):\n");
    for (int i = 0; i < n; i++) {
        char src[50], dest[50];
        int fare;
        scanf("%s %s %d", src, dest, &fare);

        // Add the stations if they are not already in the graph
        if (findStationIndex(graph, src) == -1) {
            strcpy(graph->stations[graph->station_count++], src);
        }
        if (findStationIndex(graph, dest) == -1) {
            strcpy(graph->stations[graph->station_count++], dest);
        }
        
        // Add the bidirectional edge between the stations
        addEdge(graph, src, dest, fare);
        addEdge(graph, dest, src, fare);
    }

    printf("Enter the number of metro lines:\n");
    scanf("%d", &line_count);
    
    for (int i = 0; i < line_count; i++) {
        printf("Enter the name of the line:\n");
        scanf("%s", graph->lines[i].line_name);
        printf("Enter the number of stations on %s line:\n", graph->lines[i].line_name);
        scanf("%d", &graph->lines[i].num_stations);
        
        printf("Enter the stations on %s line:\n", graph->lines[i].line_name);
        for (int j = 0; j < graph->lines[i].num_stations; j++) {
            scanf("%s", graph->lines[i].stations[j]);
            int index = findStationIndex(graph, graph->lines[i].stations[j]);
            if (index == -1) {
                strcpy(graph->stations[graph->station_count++], graph->lines[i].stations[j]);
            }
        }
    }
}

// Function to add an edge to the adjacency list for a given station
void addEdge(Graph *graph, char *src, char *dest, int fare) {
    int src_idx = findStationIndex(graph, src);
    int dest_idx = findStationIndex(graph, dest);
    
    NODE* newNode = (NODE*)malloc(sizeof(NODE)); // Allocate memory for new node
    strcpy(newNode->info, dest);
    newNode->fare = fare;
    newNode->next = graph->V[src_idx]; // Insert at the head of the list
    graph->V[src_idx] = newNode;
}

// Find the index of a station in the graph's stations array
int findStationIndex(Graph *graph, char *station_name) {
    for (int i = 0; i < graph->station_count; i++) {
        if (strcmp(graph->stations[i], station_name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to display all possible routes between two stations along with fare
void possibleroutes(Graph *graph) {
    char src[50], dest[50];
    printf("Enter the two stations you wish to find the possible routes between: ");
    scanf("%s %s", src, dest);

    int s = findStationIndex(graph, src);
    int d = findStationIndex(graph, dest);

    if (s == -1 || d == -1) {
        printf("One or both stations not found in the network.\n");
        return;
    }

    printf("Following are all different paths from %s to %s:\n", src, dest);
    bool* visited = (bool*)malloc(graph->station_count * sizeof(bool));
    int* path = (int*)malloc(graph->station_count * sizeof(int));
    int pathIndex = 0;

    for (int i = 0; i < graph->station_count; i++) {
        visited[i] = false;
    }

    // Recursive utility to find paths
    printAllPathsUtil(graph, s, d, visited, path, pathIndex, 0);

    free(visited);
    free(path);
}

// Utility function to recursively print all paths from source to destination
void printAllPathsUtil(Graph* graph, int u, int d, bool visited[], int path[], int pathIndex, int pathCost) {
    visited[u] = true;
    path[pathIndex] = u;
    pathIndex++;

    if (u == d) {
        for (int i = 0; i < pathIndex; i++) {
            printf("%s", graph->stations[path[i]]);
            if (i < pathIndex - 1) printf(" -> ");
        }
        printf(" | Total Fare: %d\n", pathCost);
    } else {
        NODE* pCrawl = graph->V[u];
        while (pCrawl != NULL) {
            int v = findStationIndex(graph, pCrawl->info);
            if (!visited[v]) {
                printAllPathsUtil(graph, v, d, visited, path, pathIndex, pathCost + pCrawl->fare);
            }
            pCrawl = pCrawl->next;
        }
    }

    pathIndex--;
    visited[u] = false;
}

// Display station details including lines it is part of and directly connected stations
void station_Details(Graph *graph, char *station_name) {
    int station_idx = findStationIndex(graph, station_name);
    if (station_idx == -1) {
        printf("Station %s not found in the network.\n", station_name);
        return;
    }
    printf("Station Name: %s\n", graph->stations[station_idx]);
    printf("Lines: ");
    int found_lines = 0;
    for (int i = 0; i < MAX && graph->lines[i].num_stations > 0; i++) {
        for (int j = 0; j < graph->lines[i].num_stations; j++) {
            if (strcmp(graph->lines[i].stations[j], station_name) == 0) {
                printf("%s ", graph->lines[i].line_name);
                found_lines = 1;
                break;
            }
        }
    }
    if (!found_lines) printf("None ");
    printf("\nConnections: ");
    NODE *ptr = graph->V[station_idx];
    while (ptr != NULL) {
        printf("%s (Fare: %d) ", ptr->info, ptr->fare);
        ptr = ptr->next;
    }
    printf("\n");
}

// Finds and displays intersecting stations where multiple lines meet
void findIntersectingStations(Graph *graph) {
    printf("Intersecting stations in the metro network:\n");
    intersection intersect_stations[MAX];
    int intersect_count = 0;

    for (int i = 0; i < graph->station_count; i++) {
        int line_count = 0;
        for (int j = 0; j < MAX && graph->lines[j].num_stations > 0; j++) {
            for (int k = 0; k < graph->lines[j].num_stations; k++) {
                if (strcmp(graph->lines[j].stations[k], graph->stations[i]) == 0) {
                    line_count++;
                    break;
                }
            }
        }
        if (line_count > 1) {
            printf("%s (Lines: %d)\n", graph->stations[i], line_count);
            strcpy(intersect_stations[intersect_count].station, graph->stations[i]);
            intersect_stations[intersect_count].lines = line_count;
            intersect_count++;
        }
    }
}

// Finds terminal stations in the network (i.e., end stations of each line)
void findTerminals(Graph *graph) {
    printf("Terminal stations for each line:\n");
    for (int i = 0; i < MAX && graph->lines[i].num_stations > 0; i++) {
        printf("Line %s: %s (Start), %s (End)\n", graph->lines[i].line_name,
               graph->lines[i].stations[0], graph->lines[i].stations[graph->lines[i].num_stations - 1]);
    }
}

// Finds the nearest intersection station given a starting station name
void findNearestIntersection(Graph *graph) {
    char src[50];
    printf("Enter the station to find the nearest intersection station from: ");
    scanf("%s", src);

    int src_idx = findStationIndex(graph, src);
    if (src_idx == -1) {
        printf("Station %s not found in the network.\n", src);
        return;
    }

    bool* visited = (bool*)malloc(graph->station_count * sizeof(bool));
    for (int i = 0; i < graph->station_count; i++) {
        visited[i] = false;
    }

    // Start a BFS from the given station
    struct QueueNode {
        int station_index;
        int distance;
    };
    struct QueueNode queue[MAX];
    int front = 0, rear = 0;

    // Initialize the queue with the starting station
    queue[rear++] = (struct QueueNode){src_idx, 0};
    visited[src_idx] = true;

    while (front < rear) {
        struct QueueNode node = queue[front++];
        int u = node.station_index;

        int line_count = 0;
        for (int j = 0; j < MAX && graph->lines[j].num_stations > 0; j++) {
            for (int k = 0; k < graph->lines[j].num_stations; k++) {
                if (strcmp(graph->lines[j].stations[k], graph->stations[u]) == 0) {
                    line_count++;
                    break;
                }
            }
        }
        if (line_count > 1) {
            printf("Nearest intersection station to %s is %s, at distance %d.\n", src, graph->stations[u], node.distance);
            free(visited);
            return;
        }

        NODE* pCrawl = graph->V[u];
        while (pCrawl != NULL) {
            int v = findStationIndex(graph, pCrawl->info);
            if (!visited[v]) {
                queue[rear++] = (struct QueueNode){v, node.distance + 1};
                visited[v] = true;
            }
            pCrawl = pCrawl->next;
        }
    }

    printf("No intersection station found reachable from %s.\n", src);
    free(visited);
}
