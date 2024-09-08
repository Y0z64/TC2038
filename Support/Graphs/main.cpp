#include "AGraph.h"
#include <iostream>

int main()
{
  Graph g(5);

  // Adding edges
  g.add_edge(0, 1);
  g.add_edge(0, 4);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 3);
  g.add_edge(3, 4);

  std::cout << "Graph structure:" << std::endl;
  g.display();

  std::cout << "\nRecursive DFS starting from node 0:" << std::endl;
  g.dfs_recursive(0);

  std::cout << "Iterative DFS starting from node 0:" << std::endl;
  g.dfs_iterative(0);

  return 0;
}