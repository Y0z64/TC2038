#include <iostream>
#include <vector>
#include <stack>
#include "AGraph.h"

void Graph::dfs_recursive(int start_node)
{
  std::vector<bool> visited(num_nodes, false);
  dfs_recursive_util(start_node, visited);
  std::cout << std::endl;
}

void Graph::dfs_recursive_util(int node, std::vector<bool> &visited)
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

void Graph::dfs_iterative(int start_node)
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