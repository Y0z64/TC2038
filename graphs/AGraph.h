#include <iostream>
#include <vector>
#include <stack>

class Graph {
private:
  int num_nodes;
  std::vector<std::vector<int>> adj_list;

public:
  Graph(int n);
  void add_edge(int u, int v);
  void display();

  // DFS
  void dfs_recursive(int start_node);
  void dfs_recursive_util(int node, std::vector<bool> &visited);
  void dfs_iterative(int start_node);
};