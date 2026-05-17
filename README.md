# FINAL PRACTICE: SMART CAMPUS MOBILITY ANALYZER

## Team Members
* SEBASTIAN SANCHEZ GOMEZ
* DIEGO ANDRES CABALLERO FERNANDEZ
* IKKER GIL JORDAN

## Explanation of the Solution
Our solution simulates a mobility and transportation network for university students in Medellín. The program acts as a query system where users can load transportation data, view available locations, check direct connections, and calculate the fastest route between any two points in the city using algorithms.

## Structure Selected
We selected a **Weighted Undirected Graph** combined with **Hash Indexing**.
* The core structure is an Adjacency List built using an `std::unordered_map<string, vector<Route>>`.
* The `unordered_map` acts as the Hash function, providing an $O(1)$ average time complexity for indexing and searching locations.
* Each node (key) points to a vector of `Route` objects (edges), which hold the destination, transportation method, distance, and time.

## Hashing Strategy
The hashing strategy relies entirely on C++'s standard library hashing (`std::unordered_map` and `std::unordered_set`). By hashing the `string` names of the locations (e.g., "EAFIT", "Poblado"), the system instantly accesses the memory address containing the adjacency list for that specific location, bypassing the need to iterate through an array of nodes.

## Traversal Explanation
For the route exploration and traversal requirement, we implemented **Dijkstra's Algorithm** (Shortest-path style analysis). 
When a user wants to go from point A to point B, the algorithm traverses the graph using a Priority Queue (`std::priority_queue`). It explores the neighboring nodes prioritizing the routes that consume the least amount of "Estimated Minutes" (our edge weight). This guarantees that the user always gets the fastest possible path to their destination.

## Execution Instructions
1. Open [OnlineGDB](https://onlinegdb.com).
2. Set the language to `C++`.
3. Paste the provided code in `main.cpp`.
4. Create a new file called `dataset.txt` and paste the provided CSV data.
5. Click **Run**.
6. Use Option `1` first to load the graph into memory.
7. Use Option `6` to generate the DOT code, copy the console output, and paste it into [Graphviz Online](https://dreampuf.github.io/GraphvizOnline/) to visualize the network.
