Data Structures
1.	Graph
o	The main data structure representing the metro network.
o	Stores stations, lines, adjacency lists for each station, and utility fields like visited and station_count.
o	Fields:
	V: An array of pointers, where each index represents a station and points to a linked list of adjacent stations.
	lines: An array of Line structs, where each Line represents a different metro line and contains the list of stations on that line.
	visited: An array of integers for marking visited stations during graph traversal.
	station_count: The total number of unique stations in the graph.
	stations: An array of strings storing station names to index and track them in the graph.
2.	Node (NODE)
o	A structure representing each adjacent station connected to a particular station.
o	Fields:
	info: Name of the adjacent station.
	fare: The fare or weight of the edge connecting the station to its adjacent station.
	next: A pointer to the next adjacent station in the linked list.
3.	Line
o	Represents a metro line within the network.
o	Fields:
	line_name: The name of the line (e.g., "Blue Line").
	stations: An array of station names on the line.
	num_stations: The number of stations on that line.
4.	Intersection
o	A structure to represent stations that lie on multiple lines (intersections).
o	Fields:
	station: Name of the intersecting station.
	lines: The number of lines that pass through this station.
Key Functions
1.	initGraph
o	Initializes the graph by setting each station’s adjacency list pointer (V[i]) to NULL and visited[i] to 0. Resets the station count.
2.	createGraph
o	Creates the graph by adding stations, edges (fares between connected stations), and defining metro lines.
o	It reads the station and line data, finds new stations, and calls addEdge to establish connections.
3.	addEdge
o	Adds an edge between two stations, creating an adjacency list for each station.
o	Uses a linked list structure (NODE) to maintain multiple connections for each station.
4.	findStationIndex
o	Searches for a station by name and returns its index within the graph’s stations array.
5.	station_Details
o	Provides details for a specific station, including the lines it is part of and its directly connected adjacent stations.
o	Iterates through graph->lines to find lines that contain the specified station.
6.	findIntersectingStations
o	Identifies stations that are intersections (appear on more than one line).
o	Uses an array of intersection structures to count the occurrences of each station across lines.
7.	findNearestIntersection
o	Finds the closest intersection from a given starting station, using a breadth-first search (BFS).
o	This method uses an array predecessors to track paths, allowing reconstruction of the shortest path.
8.	findTerminals
o	Lists all terminal stations for each line by displaying the first and last stations of each line in graph->lines.
9.	possibleroutes & printAllPathsUtil
o	Finds all possible routes between two stations.
o	Uses depth-first search (DFS) to explore all paths between a source and destination, keeping track of the path and accumulated fare cost.
