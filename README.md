# 🗺️ Multi-Modal Transportation Planner

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](#)
[![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra%20%2F%20A*-orange.svg)](#)
[![Course](https://img.shields.io/badge/UC_Davis-ECS_34-navy.svg)](#)

## 👥 Contributors
<img align="right" width="260" src="https://github.com/user-attachments/assets/3430c2cc-d373-4d4e-9e7e-34070234b1f1">
- **Linjie Yang** (UC Davis - Applied Mathematics & Computer Science)
- **Ruoyu Xi** (UC Davis)

## 📌 Project Overview
A robust C++ command-line application designed to calculate optimal travel routes across a city using multi-modal transportation data. The system integrates real-world geographical data from **OpenStreetMap (OSM)** and public transit information from **Bus System** datasets.

## 🧠 Core Functionalities
- **Pathfinding Engine**: Implements efficient algorithms (e.g., Dijkstra) to find the shortest/fastest paths between nodes on a graph.
- **Geographic Data Parsing**: Parses complex XML files (OSM) to reconstruct city road networks.
- **Transit Integration**: Processes CSV-based bus schedules and route data to enable seamless multi-modal planning.
- **Memory Efficiency**: Optimized data structures (unordered_maps, adjacency lists) to handle large-scale spatial data with minimal latency.

## 🏗️ Architecture & File Structure
The project follows a modular design for high maintainability:
- `/src/TransportationPlanner.cpp`: The core logic that coordinates data providers and runs pathfinding algorithms.
- `/src/OpenStreetMap.cpp`: Handles OSM XML parsing and spatial coordinate conversions.
- `/src/BusSystem.cpp`: Manages bus stop indexing and route traversal.
- `/src/XMLReader.cpp` & `/src/CSVReader.cpp`: Specialized parsers for heterogeneous data sources.

## 🛠️ Tech Stack
- **Language**: C++17
- **Tools**: Google Test (GTest) for unit testing, Doxygen for documentation.
- **Data Formats**: XML (OpenStreetMap), CSV (Bus Data).

## 📈 Performance & Complexity
- **Graph Construction**: $O(V + E)$ where $V$ is the number of nodes and $E$ is the number of ways.
- **Query Time**: Optimized to $O(E \log V)$ using priority-queue-based Dijkstra.

## 🚀 How to Run
1. Clone the repository:
   ```bash
   git clone [https://github.com/Felix-Yang-369/Transportation-Planner.git](https://github.com/Felix-Yang-369/Transportation-Planner.git)
