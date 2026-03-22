# Single-Source Shortest Paths in C++

This project implements and compares multiple algorithms for solving the **Single-Source Shortest Path (SSSP)** problem in directed graphs.

The focus is on practical performance and extensibility, including several heuristic improvements of the Bellman-Ford algorithm.

## Features

- Implementation of classical shortest path algorithms:
  - Dijkstra (priority queue variant)
  - Bellman-Ford

- Heuristic improvements of Bellman-Ford:
  - Yen's First Heuristic (queue-based optimization)
  - Yen's Second Heuristic (bidirectional edge relaxation)
  - Tarjan’s heuristic (tree disassembly & early negative cycle detection)

- Support for **combining heuristics**
  - e.g. `Tarjan + YenOne`, `YenOne + YenTwo`, etc.

- Modular and extensible architecture
- Performance benchmarking and logging
- Path reconstruction support
- Detection of negative-weight cycles

## Key Ideas

The project is built around a **modular Bellman-Ford framework**, where heuristics are implemented as composable components.

- Core algorithm logic is reused
- Heuristics inject behavior via:
  - iteration steps
  - relax actions
  - vertex filtering

This allows flexible experimentation without duplicating code.

## Architecture

- `Graph`
  - Adjacency list representation using `std::unordered_map`
  - Supports arbitrary vertex IDs

- `BellmanFord` (base class)
  - Core algorithm logic
  - Provides extensibility hooks

- Heuristic classes:
  - `YenOne`
  - `YenTwo`
  - `Tarjan`

- Combined variants:
  - `YenBoth`
  - `TarjanYenOne`
  - `TarjanYenTwo`
  - `TarjanYenBoth`

## Experiments

The algorithms were evaluated on multiple graph types:

- Real-world road networks
- Sparse and dense graphs
- Graphs with negative weights (no negative cycles)
- Graphs with injected negative cycles
- Graphs with different degree distributions

### Key observations:
- `Tarjan + YenOne` consistently provides the best performance among Bellman-Ford variants
- Performance approaches Dijkstra in many cases
- Combining heuristics is not always beneficial
- Graph structure has major impact on performance

## Usage

### Run algorithm
```cpp
algorithm.run(source);
```

### Print shortest path
```cpp
algorithm.printPath(target);
```

### Export distances
```cpp
algorithm.exportDistances("output.txt");
```

## Testing

- All algorithms share a common interface
- Outputs are compared automatically
- Ensures correctness across implementations
- Negative cycle detection is validated across all variants

## Performance Measurement

- Execution time measured using `<chrono>`
- Results stored in output files for further analysis

## Technologies

- C++23
- STL (containers, algorithms, smart pointers)
- Templates, constraints and concepts, and lambda functions

## Motivation

The goal of this project is to explore the **practical performance of shortest path algorithms** and demonstrate how heuristic optimizations can significantly improve real-world efficiency without changing theoretical complexity.

## Author

Adam Pavelka  
Bachelor’s Thesis – Faculty of Informatics, Masaryk University
