#include <iostream>
#include <vector>
#include <stack>

class Graph
{
private:
  int num_nodes;
  std::vector<std::vector<int>> adj_list;

public:
  Graph(int n) : num_nodes(n), adj_list(n) {}

  void add_edge(int u, int v)
  {
    adj_list[u].push_back(v);
    adj_list[v].push_back(u); // This makes the graph undirected
  }

  void display()
  {
    for (int i = 0; i < num_nodes; i++)
    {
      std::cout << i << ": ";
      for (int neighbor : adj_list[i])
      {
        std::cout << neighbor << " ";
      }
      std::cout << std::endl;
    }
  }

  void dfs_recursive(int start_node)
  {
    std::vector<bool> visited(num_nodes, false);
    dfs_recursive_util(start_node, visited);
    std::cout << std::endl;
  }

  void dfs_recursive_util(int node, std::vector<bool> &visited)
  {
    visited[node] = true;
    std::cout << node << " ";

    for (int neighbor : adj_list[node])
    {
      if (!visited[neighbor])
      {
        dfs_recursive_util(neighbor, visited);
      }
    }
  }

  void dfs_iterative(int start_node)
  {
    std::vector<bool> visited(num_nodes, false);
    std::stack<int> stack;
    stack.push(start_node);

    while (!stack.empty())
    {
      int node = stack.top();
      stack.pop();

      if (!visited[node])
      {
        std::cout << node << " ";
        visited[node] = true;

        for (int i = adj_list[node].size() - 1; i >= 0; i--)
        {
          int neighbor = adj_list[node][i];
          if (!visited[neighbor])
          {
            stack.push(neighbor);
          }
        }
      }
    }
    std::cout << std::endl;
  }
};

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