#include "AGraph.h"

Graph::Graph(int n) : num_nodes(n), adj_list(n) {}

void Graph::add_edge(int u, int v)
{
  adj_list[u].push_back(v);
  adj_list[v].push_back(u);
}

void Graph::display()
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
