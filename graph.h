#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

template <class D, class W>
class Edge {
public:
	D vertex1;
	D vertex2;
	W weight;
	Edge(D v1, D v2, W w):vertex1(v1), vertex2(v2), weight(w) {}
};

template <class D, class W>
class Graph {
public:
	vector<D> vertice;
	vector<Edge<D,W>> edges;

	vector< pair<D, Edge<D,W>> > adjacent(D u) {
		vector< pair<D, Edge<D,W> >> res;
		for (Edge<D,W> e : edges) {
			if (e.vertex1 == u) {
				res.push_back( make_pair(e.vertex2, e));
			} else if (e.vertex2 == u) {
				res.push_back( make_pair(e.vertex1, e));
			}
		}
		return res;
	}

	void add_vertex(D v);
	void add_edge(D v1, D v2, W w);

	void kruskal();
	void prim(D root);
	void dijsktra(D root);
};

unordered_map<char, char> PARENT;
unordered_map<char, int> RANK;

char find(char vertex) {
	if (PARENT[vertex] == vertex)
		return PARENT[vertex];
	else
		return find(PARENT[vertex]);
}

template <class D, class W>
void Graph<D, W>::add_vertex(D v) {
	vertice.push_back(v);
}

template <class D, class W>
void Graph<D, W>::add_edge(D org, D dest, W weight) {
	edges.push_back(Edge<D, W>( org, dest, weight));
}

template <class D, class W>
void Graph<D,W>::kruskal() {
	vector<Edge<D, W>> res;

	for (auto c : vertice) {
		PARENT[c] = c;
		RANK[c] = 0;
	}

	sort(edges.begin(), edges.end(), [](Edge<D, W> x, Edge<D, W> y)
					{return x.weight < y.weight;});   // O(E*log(E))

	for (Edge<D, W> e : edges) {         // O(E)
		char root1 = find(e.vertex1);  // O(E) worst case
		char root2 = find(e.vertex2);
		if (root1 != root2) {
			res.push_back(e);
			if (RANK[root1] > RANK[root2]) {
				PARENT[root2] = root1;
				RANK[root1]++;
			} else {
				PARENT[root1] = root2;
				RANK[root2]++;
			}
		}
	}

	for (Edge<D, W> e : res) {
		cout << e.vertex1 << " -- " << e.vertex2 << "  " << e.weight << endl;
	}
}

template <class D, class W>
void Graph<D,W>::prim(D root) {
	unordered_map<D, D> res;
	unordered_map<D, D> PARENT;
	unordered_map<D, W> KEY;

	for (auto c : vertice) {
		PARENT[c] = '\0';
		KEY[c] = INT_MAX;
	}
	KEY[root] = 0;
	vector<D> Q = vertice;

	while (!Q.empty()) {    // O(V)
		D u = *std::min_element(Q.begin(), Q.end(), [&](D x, D y)
										{return KEY[x] < KEY[y];});  // O(v)
		vector<char>::iterator itr = remove(Q.begin(), Q.end(), u);  // O(V)
		Q.erase(itr, Q.end());  // erase() following remove() idiom
		if (PARENT[u] != '\0') {
			res[u] = PARENT[u];   // This is one edge of MST
		}
		vector< pair<D, Edge<D,W>> > adj = adjacent(u);   // O(E)
		for (pair<D, Edge<D,W>> v : adj) {
			if (find(Q.begin(), Q.end(), v.first) != Q.end()) {   // O(V)
				if (v.second.weight < KEY[v.first]) {
					PARENT[v.first] = u;
					KEY[v.first] = v.second.weight;
				}
			}
		}
	}

	for (auto e : res) {
		cout << e.first << " -- " << e.second << endl;
	}
}

template <class D, class W>
void Graph<D, W>::dijsktra(D src) {

}


#endif
