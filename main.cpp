#include <tdzdd/DdSpec.hpp>
#include <tdzdd/DdStructure.hpp>
#include <tdzdd/DdEval.hpp>
#include <tdzdd/spec/PathZdd.hpp>
#include <tdzdd/util/Graph.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <string>


// Graph class
class Graph {
private:
  int const s;
  int const t;
  int const n;
  int const m;
  std::vector<std::pair<int, int> > const edge_list;
  std::vector<std::vector<int> > adj;
  tdzdd::Graph tdg;

public:
  Graph(int s, int t, int n, int m, std::vector<std::pair<int, int> > edge_list)
    :s(s), t(t), n(n), m(m), edge_list(edge_list){
    
      adj.assign(n + 1, std::vector<int>());
    
      for (auto const& e : edge_list) {
      adj[e.first].push_back(e.second);
      adj[e.second].push_back(e.first);
      tdg.addEdge(std::to_string(e.first), std::to_string(e.second));
    }
    
    tdg.setColor(std::to_string(s), 1);
    tdg.setColor(std::to_string(t), 1);
    tdg.update();
  }

  std::pair<int, int> getEdge (int e) const{
    return std::make_pair(edge_list[e].first, edge_list[e].second);
  }

  int getStart() const {
    return s;
  }

  int getTerminal() const {
    return t;
  }

  int numVertices() const {
    return n;
  }

  int numEdges() const {
    return m;
  }

  std::vector<std::vector<int> > const& adjacency() const {
    return adj;
  }
  
  tdzdd::Graph const& tdzddGraph() const {
    return tdg;
  }

  void printEdges() const {
    //std::cout << "#edges: " << m << std::endl;
    for(auto it = edge_list.begin() ; it != edge_list.end() ; ++it){
      std::cout << (*it).first << ", " << (*it).second << std::endl;
    }
  }

  void print() const{
    std::cout << "#Vertices: " << numVertices() << std::endl;
    std::cout << "#Edges: " << numEdges() << std::endl;
    std::cout << "Start, Terminal = " << s << ", " << t << std::endl;
    std::cout << "Edge List: " << std::endl;
    printEdges();
  }
};



// TdZdd
class PathZDD {
  tdzdd::DdStructure<2> dd;
  tdzdd::Graph const& tdg;

public:
  explicit PathZDD(Graph const& G)
    : dd(tdzdd::PathZdd(G.tdzddGraph(), true)), tdg(G.tdzddGraph()) {
    dd.zddReduce();
  }

  std::string countPaths() const {
    return dd.zddCardinality();
  }

  void listAllPaths(int s, int t) const {
    int const s_id = tdg.getVertex(std::to_string(s));
    int const t_id = tdg.getVertex(std::to_string(t));
    int const edge_count = tdg.edgeSize();
    int const vertex_count = tdg.vertexSize();

    for (auto it = dd.begin(); it != dd.end(); ++it) {
      std::vector<std::vector<int> > adj(vertex_count + 1);

      for (int level : *it) {
        int const edge_index = edge_count - level;
        tdzdd::Graph::EdgeInfo const& e = tdg.edgeInfo(edge_index);
        adj[e.v1].push_back(e.v2);
        adj[e.v2].push_back(e.v1);
      }

      std::vector<int> path;
      int prev = 0;
      int cur = s_id;
      path.push_back(cur);
      while (cur != t_id) {
        int next = 0;
        for (int v : adj[cur]) {
          if (v != prev) {
            next = v;
            break;
          }
        }
        if (next == 0) break;
        prev = cur;
        cur = next;
        path.push_back(cur);
      }

      for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << '-';
        std::cout << tdg.vertexName(path[i]);
      }
      std::cout << '\n';
    }
  }
};


// Read input file
Graph readGraph(int argc, char **argv){
  if(argc < 2){
    std::cerr << "Please input a graph file" << std::endl;
    exit(-1);
  }
  std::ifstream ifs(argv[1]);
  if (!ifs){
    std::cerr << "Error: cannot open file " << argv[1] << std::endl;
    exit(-1);
  }

  int s, t;
  if(ifs)  ifs >> s >> t;

  std::vector<std::pair<int, int> > edge_list;
  int u, v;
  int n = 0;
  while(ifs >> u >> v){
    edge_list.push_back(std::make_pair(u, v));
    if (n < u) n = u;
    if (n < v) n = v;
  }
  Graph G(s, t, n, edge_list.size(), edge_list);

  return G;
}

int main(int argc, char **argv) {
  bool list_all = false;
  if (argc >= 3) {
    std::string opt = argv[2];
    list_all = (opt == "--all" || opt == "-all");
  }

  Graph G = readGraph(argc, argv);
  PathZDD zdd(G);
  if (list_all) {
    zdd.listAllPaths(G.getStart(), G.getTerminal());
    return 0;
  }

  std::cout << zdd.countPaths() << std::endl;
  return 0;
} 
