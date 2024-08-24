#include <vector>

int main()
{
    std::vector<std::pair<int, int>> table;

    table.resize(10);
    table.push_back({{1,1}});

    for (auto pair : table[0])
}