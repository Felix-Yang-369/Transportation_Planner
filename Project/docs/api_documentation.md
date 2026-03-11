# Project 4 API Documentation

## 1. CDijkstraPathRouter
* **Description**: A graph-based routing engine that implements Dijkstra's algorithm to find the shortest path between vertices. Optimized with a Min-Heap (`std::priority_queue`) and contiguous memory layout for maximum cache efficiency.
* **Key Methods**:
  * `AddVertex(std::any tag)`: Adds a vertex to the graph and returns its unique ID.
  * `AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir)`: Adds a directed or bidirectional edge. Rejects negative weights.
  * `FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path)`: Computes the shortest path using Dijkstra's algorithm.

## 2. CBusSystemIndexer
* **Description**: Provides an optimized indexing mechanism for the public transit system. Maps raw node IDs to bus stops and routes for $O(1)$ or $O(\log N)$ fast lookups.
* **Key Methods**:
  * `StopByNodeID(TNodeID id)`: Retrieves stop information given a map node ID.
  * `RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute>> &routes)`: Finds all overlapping routes between two connected nodes.

## 3. CDijkstraTransportationPlanner
* **Description**: The core integration system. It builds a graph from OSM data and transit configurations, converting physical coordinates and speed limits into edge weights.
* **Key Methods**:
  * `FindShortestPath(...)`: Calculates the path with the minimum physical distance.
  * `FindFastestPath(...)`: Calculates the multi-modal path (Walk, Bike, Bus) that minimizes travel time.
  * `GetPathDescription(...)`: Generates human-readable, step-by-step navigation instructions including bearings and street names.

## 4. CTransportationPlannerCommandLine
* **Description**: The interactive shell interface. It parses user commands (e.g., `shortest`, `fastest`, `print`, `save`), invokes the planner, and formats the output to the provided data sinks.