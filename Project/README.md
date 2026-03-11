# Project 4 README

Student 1: Linjie Yang (923779926)

Student 2: Ruoyu Xi (923235075)

## Project Status
The project is completely implemented and functioning as expected. We have successfully integrated the `COpenStreetMap` and `CCSVBusSystem` from Project 3 to build a fully functional routing engine, the `DSV` and `XML` parsers from Project 2, and 'StrtingUtil" from Project 1.

Additionally, we have successfully implemented both Extra Credit opportunities:
1. **Path Description**: Fully implemented step-by-step navigation instructions with bearing calculations and street name lookaheads.
2. **Speed Performance**: Deeply optimized `CDijkstraPathRouter` and `CDijkstraTransportationPlanner` to pass the `speedtest_optimized` baseline without compiler optimizations (`-O0`). We achieved this by using contiguous memory (`std::vector` instead of `shared_ptr` for edges), a Min-Heap (`std::priority_queue`), and $O(\log N)$ binary search (`std::lower_bound`) for node ID lookups during precomputation.

## Known Issues
There are no major known issues at this time.

## Code References
* [Different ways to initialize variables](https://stackoverflow.com/questions/22543875/different-ways-to-initialize-variables)
* [Google Test Primer (for writing unit tests)](https://google.github.io/googletest/primer.html)
* [Expat XML Parser Documentation](https://libexpat.github.io/doc/)
* [std::lower_bound Documentation (for O(log N) fast ID lookups)](https://en.cppreference.com/w/cpp/algorithm/lower_bound)


## Generative AI Use
I used ChatGPT for Prompt 1, and hatGPT for Prompt 2 to assist with code debugging and test case design.

### Prompt 1
How do I iterate over all characters in a string in C++17?

### Response 1
In C++17, you can iterate over all characters in a string using a range-based for loop. Here's an example:
```cpp
#include <iostream>
#include <string>

int main() {
    std::string myString = "Hello, World!";

    // Using range-based for loop to iterate over characters in the string
    for (char c : myString) {
        std::cout << c << " ";
    }

    return 0;
}
```
In this example, the `for (char c : myString)` loop iterates over each character in the string `myString`, and the variable `c` takes on the value of each character in turn. The loop body prints each character followed by a space.

This is a concise and readable way to iterate over the characters in a string in C++17.

### Chagnes 1
I only needed the `for` loop part, and my string was named `bar` so I changed it to just:
```cpp
// AI Generated
for (char c : bar) {
```

### Prompt 2
How do I implement `CDijkstraPathRouter` efficiently to handle negative weights and use a Min-Heap for the shortest path?

### Response 2
To implement Dijkstra's algorithm efficiently, you should use a `std::priority_queue` with `std::greater` to act as a Min-Heap. For negative weights, project specifications usually require rejecting them during the `AddEdge` phase. Here is a skeleton using a Min-Heap:
```cpp
using NodeDist = std::pair<double, TVertexID>;
std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;
// ... (relaxation loop logic)

### Changes 2
We adopted the Min-Heap structure using std::priority_queue and std::greater for our FindShortestPath implementation. We also ensured AddEdge returns false if a weight is less than 0.0.
