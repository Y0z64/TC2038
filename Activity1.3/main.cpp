#include "../Support/Queue/PriorityQueue.h"
#include "../Support/Utilities/measureTime.h"
#include <functional>
#include <iostream>
#include <vector>

/**
 * Implementación de Solución de Laberinto usando Backtracking y Ramificación y
 * Poda Nota: El programa espera que el archivo de texto tenga el formato
 * correcto.
 *
 * Autores:
 * - Yair Salvador Beltran Rios
 * - Daniel Alfredo Barreras Meraz
 * Matrículas:
 * - A01254673
 * - A01254805
 * Fecha: Sep 7
 */

struct Node {
  int x, y, cost;
  Node(int x, int y, int cost) : x(x), y(y), cost(cost) {}
};

/**
 * Función isValid
 * Complejidad Temporal: O(1)
 * Complejidad Espacial: O(1)
 */
bool isValid(int x, int y, int M, int N) {
  return x >= 0 && x < M && y >= 0 && y < N;
}

/**
 * Función solveMazeBacktracking
 * Análisis de Complejidad Temporal:
 * Mejor Caso: O(n), donde n es el número de celdas en el laberinto
 * Caso Promedio: O(2^n), donde n es el número de celdas en el laberinto
 * Peor Caso: O(4^n), donde n es el número de celdas en el laberinto
 *
 * Complejidad Espacial: O(m*n), donde m y n son las dimensiones del laberinto
 */
std::vector<std::vector<bool>>
solveMazeBacktracking(std::vector<std::vector<bool>> &maze, int M, int N) {
  std::vector<std::vector<bool>> solution(M, std::vector<bool>(N, false));
  std::vector<std::pair<int, int>> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

  std::function<bool(int, int)> backtrack = [&](int x, int y) -> bool {
    if (x == M - 1 && y == N - 1) {
      solution[x][y] = true;
      return true;
    }

    if (isValid(x, y, M, N) && maze[x][y]) {
      solution[x][y] = true;
      for (const auto &move : moves) {
        int nextX = x + move.first;
        int nextY = y + move.second;
        if (backtrack(nextX, nextY)) {
          return true;
        }
      }
      solution[x][y] = false;
    }
    return false;
  };

  backtrack(0, 0);
  return solution;
}

/**
 * Función solveMazeBranchAndBound
 * Análisis de Complejidad Temporal:
 * *Usando el theorema maestro
 * Mejor Caso: O(m*n), donde m y n son las dimensiones del laberinto
 * Caso Promedio: O(m*n*log(m*n)), donde m y n son las dimensiones del laberinto
 * Peor Caso: O(m*n*log(m*n)), donde m y n son las dimensiones del laberinto
 *
 * Complejidad Espacial: O(m*n), donde m y n son las dimensiones del laberinto
 */
std::vector<std::vector<bool>>
solveMazeBranchAndBound(const std::vector<std::vector<bool>> &maze, int M,
                        int N) {
  std::vector<std::pair<int, int>> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  std::vector<std::vector<bool>> visited(M, std::vector<bool>(N, false));
  std::vector<std::vector<std::pair<int, int>>> parent(
      M, std::vector<std::pair<int, int>>(N, {-1, -1}));

  auto compare = [](const Node &a, const Node &b) { return a.cost > b.cost; };
  PriorityQueue<Node, decltype(compare)> pq(compare);

  pq.push(Node(0, 0, 0));
  visited[0][0] = true;

  while (!pq.empty()) {
    Node current = pq.top();
    pq.pop();

    if (current.x == M - 1 && current.y == N - 1) {
      // Reconstruct path
      std::vector<std::vector<bool>> solution(M, std::vector<bool>(N, false));
      int x = M - 1, y = N - 1;
      while (x != -1 && y != -1) {
        solution[x][y] = true;
        auto [px, py] = parent[x][y];
        x = px;
        y = py;
      }
      return solution;
    }

    for (const auto &[dx, dy] : moves) {
      int nextX = current.x + dx, nextY = current.y + dy;
      if (isValid(nextX, nextY, M, N) && maze[nextX][nextY] &&
          !visited[nextX][nextY]) {
        visited[nextX][nextY] = true;
        parent[nextX][nextY] = {current.x, current.y};
        int newCost = current.cost + 1 + (M - 1 - nextX) +
                      (N - 1 - nextY); // Manhattan heuristic
        pq.push(Node(nextX, nextY, newCost));
      }
    }
  }

  return std::vector<std::vector<bool>>(M, std::vector<bool>(N, false));
}

int main() {
  int M, N;
  std::cin >> M >> N;
  std::vector<std::vector<bool>> maze(M, std::vector<bool>(N));
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      int val;
      std::cin >> val;
      maze[i][j] = val == 1;
    }
  }

  std::vector<std::vector<bool>> solutionBacktracking;
  double timeBacktracking = ExecutionTimer::measureExecutionTime(
      [&]() { solutionBacktracking = solveMazeBacktracking(maze, M, N); });

  std::vector<std::vector<bool>> solutionBranchAndBound;
  double timeBranchAndBound = ExecutionTimer::measureExecutionTime(
      [&]() { solutionBranchAndBound = solveMazeBranchAndBound(maze, M, N); });

  // Print backtracking solution
  std::cout << "Backtracking:" << std::endl;
  for (const auto &row : solutionBacktracking) {
    for (bool cell : row) {
      std::cout << cell << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "Tiempo: " << timeBacktracking << " ms" << std::endl;
  std::cout << std::endl;

  // Print branch and bound solution
  std::cout << "Ramificacion y poda:" << std::endl;
  for (const auto &row : solutionBranchAndBound) {
    for (bool cell : row) {
      std::cout << cell << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "Tiempo: " << timeBranchAndBound << " ms" << std::endl;
  std::cout << std::endl;

  return 0;
}