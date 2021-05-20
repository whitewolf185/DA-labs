#include <queue>
#include <iostream>
#include <vector>


const long long INF = 9999999999;

struct TEdge {
  long long to;
  long long w;
};

struct TEdgeF {
  long long from, to;
  long long w;
};

using graph = std::vector<std::vector<TEdge>>;

struct TPriority {
  long long dist;
  long long vertex;
};

bool operator<(const TPriority &lhs, const TPriority &rhs) {
  return lhs.dist > rhs.dist;
}

std::vector<long long> Deykstra(const int &n, const graph &gr, const int &start) {
  std::vector<long long> distanses(n, INF);
  std::vector<short> relaxed(n, 0);
  distanses[start] = 0;

  std::priority_queue<TPriority> pq;
  pq.push({0, start});

  while (!pq.empty()) {
    TPriority u = pq.top();
    pq.pop();

    if (relaxed[u.vertex]) {
      continue;
    }
    relaxed[u.vertex] = 1;

    for (const TEdge &e : gr[u.vertex]) {
      if (distanses[e.to] > distanses[u.vertex] + e.w) {
        distanses[e.to] = distanses[u.vertex] + e.w;
        pq.push({distanses[e.to], e.to});
      }
    }
  }

  return distanses;
}


bool FordBellma(const long long &n, const long long &m, const graph &gr,
                std::vector<long long> &distanses) {//std::vector<long long> distanses(n+1,INF);
  std::vector<TEdgeF> grr(n);
  for (int i = 0; i < n; ++i) {
    grr[i].from = 0;
    grr[i].to = i + 1;
    grr[i].w = 0;
  }

  for (int i = 0; i < gr.size(); ++i) {
    for (const TEdge &edge : gr[i]) {
      grr.push_back({i + 1, edge.to + 1, edge.w});
    }
  }

  distanses[0] = 0;
  bool changed = false;

  for (int i = 0; i < grr.size(); ++i) {
    changed = false;
    for (const TEdgeF &e : grr) {
      if (distanses[e.to] > distanses[e.from] + e.w) {
        changed = true;
        distanses[e.to] = distanses[e.from] + e.w;
      }
    }
    if (!changed) {
      break;
    }
  }

  if (changed) {
    return false;
  }

  return true;
}


int main() {
  int n, m;
  std::cin >> n >> m;
  graph gr(n);

  for (int i = 0; i < m; ++i) {
    long long from, to;
    long long w;
    std::cin >> from >> to >> w;
    gr[from - 1].push_back({to - 1, w});
  }

  std::vector<long long> sigma(n + 1, INF);

  if (!FordBellma(n, m, gr, sigma)) {
    std::cout << "Negative cycle" << std::endl;
    return 0;
  }

  for (int i = 0; i < n; ++i) {
    for (TEdge &e : gr[i]) {
      e.w = e.w + sigma[i + 1] - sigma[e.to + 1];
    }
  }

  for (int i = 0; i < n; ++i) {
    std::vector<long long> res = Deykstra(n, gr, i);
    for (int j = 0; j < res.size(); ++j) {
      if (res[j] == INF) {
        std::cout << "inf ";
      }
      else {
        std::cout << res[j] - sigma[i + 1] + sigma[j + 1] << " ";
      }
    }
    std::cout << std::endl;
  }

  return 0;
}
