#include <algorithm>
#include <climits>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <vector>

class Edge;
class Graph;

class Node {
  friend Graph;

private:
  size_t _id;
  std::vector<Edge> _edges;

public:
  explicit Node(size_t id);

  void addEdge(Edge &edge);
  void addEdge(Edge &&edge);

  int sum_capacity() const;
};

class Edge {
  friend Node;
  friend Graph;

private:
  Node *_from;
  std::shared_ptr<Node> _to;
  int _capacity;
  int _flow = 0;

public:
  Edge(Node *from, std::shared_ptr<Node> to, int capacity);
};

class Graph {
private:
  std::vector<std::shared_ptr<Node>> _nodes;

public:
  Graph() = default;
  explicit Graph(const std::vector<std::shared_ptr<Node>> &nodes);

  size_t addNode();
  std::shared_ptr<Node> getNode(size_t id);

  int dinic(std::shared_ptr<Node> source, std::shared_ptr<Node> target);

private:
  std::unique_ptr<std::vector<std::vector<Edge *>>> make_level_graph(std::shared_ptr<Node> source,
                                                                     std::shared_ptr<Node> target);
};

struct Position {
  int x;
  int y;
};

struct IceCube {
  Position position;
  int penguins;
  int capacity;
};

class Problem {
private:
  float _jumping_distance;
  std::vector<IceCube> _ice_cubes;
  std::vector<size_t> _results;

public:
  void solve();

  friend std::istream &operator>>(std::istream &is, Problem &problem);
  friend std::ostream &operator<<(std::ostream &os, const Problem &problem);
};

Node::Node(size_t id) : _id{id} {}

void Node::addEdge(Edge &edge) { _edges.emplace_back(edge); }

void Node::addEdge(Edge &&edge) { _edges.emplace_back(std::move(edge)); }

int Node::sum_capacity() const {
  int res = 0;
  for (const auto &edge : _edges) {
    res += edge._capacity;
  }
  return res;
}

Edge::Edge(Node *from, std::shared_ptr<Node> to, int capacity) : _from{from}, _to{to}, _capacity{capacity} {}

Graph::Graph(const std::vector<std::shared_ptr<Node>> &nodes) : _nodes{nodes} {}

size_t Graph::addNode() {
  _nodes.emplace_back(std::make_shared<Node>(_nodes.size()));
  return _nodes.size() - 1;
}

std::shared_ptr<Node> Graph::getNode(size_t id) { return _nodes.at(id); }

std::unique_ptr<std::vector<std::vector<Edge *>>> Graph::make_level_graph(std::shared_ptr<Node> source,
                                                                          std::shared_ptr<Node> target) {
  auto level_graph{std::make_unique<std::vector<std::vector<Edge *>>>()};
  bool reached_target = false;
  std::set<size_t> visited;
  std::set<size_t> visited_frontier;
  std::queue<std::vector<Edge *>> paths;
  for (auto &edge : source->_edges) {
    if (edge._capacity - edge._flow > 0) {
      paths.push({&edge});
      visited.emplace(edge._to->_id);
      if (edge._to->_id == target->_id) {
        reached_target = true;
        level_graph->push_back(paths.back());
      }
    }
  }
  while (!(reached_target || paths.empty())) {
    size_t original_size = paths.size();
    for (size_t i = 0; i < original_size; i++) {
      auto path = paths.front();
      paths.pop();
      for (auto &edge : path.back()->_to->_edges) {
        if (count(visited.begin(), visited.end(), edge._to->_id) == 0 && (edge._capacity - edge._flow) > 0) {
          auto new_path{path};
          new_path.push_back(&edge);
          paths.push(new_path);
          visited_frontier.emplace(edge._to->_id);
          if (edge._to->_id == target->_id) {
            reached_target = true;
            level_graph->push_back(new_path);
          }
        }
      }
    }
    for (auto id : visited_frontier) {
      visited.emplace(id);
    }
    visited_frontier.clear();
  }
  return level_graph;
}

int Graph::dinic(std::shared_ptr<Node> source, std::shared_ptr<Node> target) {
  int max_flow = 0;

  // Reset graph
  for (const auto &node : _nodes) {
    for (auto &edge : node->_edges) {
      edge._flow = 0;
    }
  }

  for (auto level_graph{*make_level_graph(source, target)}; !level_graph.empty();
       level_graph = *make_level_graph(source, target)) {
    for (const auto &path : level_graph) {
      int bottleneck = INT_MAX;
      for (auto &edge : path) {
        bottleneck = std::min(bottleneck, edge->_capacity - edge->_flow);
      }
      for (auto edge : path) {
        edge->_flow += bottleneck;
      }
      max_flow += bottleneck;
    }
  }

  return max_flow;
}

void Problem::solve() {
  Graph graph;

  float jd_squared = _jumping_distance * _jumping_distance;
  for (const auto &ice_cube : _ice_cubes) {
    auto from = graph.getNode(graph.addNode());
    auto to = graph.getNode(graph.addNode());
    from->addEdge({from.get(), to, ice_cube.capacity});
  }
  auto source = graph.getNode(graph.addNode());
  for (size_t i = 0; i < _ice_cubes.size(); i++) {
    auto node = graph.getNode(2 * i + 1);
    source->addEdge({source.get(), graph.getNode(2 * i), _ice_cubes[i].penguins});
    for (size_t j = 0; j < _ice_cubes.size(); j++) {
      if (i == j)
        continue;

      auto d_x = static_cast<float>(_ice_cubes[i].position.x - _ice_cubes[j].position.x);
      auto d_y = static_cast<float>(_ice_cubes[i].position.y - _ice_cubes[j].position.y);
      if (d_x * d_x + d_y * d_y <= jd_squared) {
        node->addEdge({node.get(), graph.getNode(2 * j), INT_MAX});
      }
    }
  }

  for (size_t i = 0; i < _ice_cubes.size(); i++) {
    if (graph.dinic(source, graph.getNode(2 * i)) == source->sum_capacity()) {
      _results.push_back(i);
    }
  }
}

std::istream &operator>>(std::istream &is, Problem &problem) {
  int n;
  is >> n >> problem._jumping_distance;
  problem._ice_cubes.clear();
  problem._results.clear();
  problem._ice_cubes.resize(n);
  for (auto &ice_cube : problem._ice_cubes) {
    is >> ice_cube.position.x >> ice_cube.position.y >> ice_cube.penguins >> ice_cube.capacity;
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const Problem &problem) {
  if (!problem._results.empty()) {
    for (auto result : problem._results) {
      os << result << ' ';
    }
  } else {
    os << -1 << ' ';
  }
  os << '\n';
  return os;
}

int main() {
  int n;
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    Problem problem;
    std::cin >> problem;
    problem.solve();
    std::cout << problem;
  }
  return 0;
}
