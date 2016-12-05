#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <queue>

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
	vector<D> vertices;
	vector<Edge<D,W>> edges;

	unordered_map<D, D> PARENT;
	unordered_map<D, W> RANK;

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

	D find_parent(D v);
	void print_graph();
	void add_vertex(D v);
	void add_edge(D v1, D v2, W w);

	vector<Edge<D, W>> kruskal();
	vector<Edge<D, W>> prim(D root);
	void dijkstra(D root);
};

template <class D, class W>
D Graph<D, W>::find_parent(D vertex) {
	if (PARENT[vertex] == vertex)
		return PARENT[vertex];
	else
		return find_parent(PARENT[vertex]);
}

template <class D, class W>
void Graph<D, W>::print_graph() {
	for (auto i: vertices) {
		cout<<"vertex:  "<<i<<endl;
		vector< pair<D, Edge<D,W>> > adj = adjacent(i);
		for(auto j: adj) {
			cout<<"\t Weight: "<<j.second.weight<<"  To: "<< j.first<<endl;
		}
	}
}

template <class D, class W>
void Graph<D, W>::add_vertex(D v) {
	vertices.push_back(v);
}

template <class D, class W>
void Graph<D, W>::add_edge(D org, D dest, W weight) {
	edges.push_back(Edge<D, W>( org, dest, weight));
}

template <class D, class W>
vector<Edge<D, W>> Graph<D,W>::kruskal() {
	vector<Edge<D, W>> res;

	for (auto c : vertices) {
		PARENT[c] = c;
		RANK[c] = 0;
	}
	auto c_edges = edges;
	sort(c_edges.begin(), c_edges.end(), [](Edge<D, W> x, Edge<D, W> y)
					{return x.weight < y.weight;});   // O(E*log(E))

	for (Edge<D, W> e : c_edges) {         // O(E)
		char root1 = find_parent(e.vertex1);  // O(E) worst case
		char root2 = find_parent(e.vertex2);
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

	return res;
}

template <class D, class W>
vector<Edge<D, W>> Graph<D,W>::prim(D root) {
	unordered_map<D, D> res;
	unordered_map<D, D> PARENT;
	unordered_map<D, W> KEY;
	vector<Edge<D, W>> ms;

	for (auto c : vertices) {
		PARENT[c] = '\0';
		KEY[c] = INT_MAX;
	}
	KEY[root] = 0;
	vector<D> Q = vertices;

	while (!Q.empty()) {    // O(V)
		D u = *std::min_element(Q.begin(), Q.end(), [&](D x, D y)
										{return KEY[x] < KEY[y];});  // O(v)
		vector<char>::iterator itr = remove(Q.begin(), Q.end(), u);  // O(V)
		Q.erase(itr, Q.end());
		if (PARENT[u] != '\0') {
			res[u] = PARENT[u];   // This is one edge of MST
			ms.push_back(Edge<D, W>(res[u], PARENT[u], KEY[u]));
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

	return ms;
}

template <class D, class W>
void Graph<D, W>::dijkstra(D src) {

	priority_queue<pair<W,D>, vector<pair<W,D>>, greater<pair<W,D>>> pq;
	vector<int> dist(vertices.size(), INT_MAX); 		// distances as infinite

	pq.push(make_pair(0, src));

	auto it = find(vertices.begin(),vertices.end(),src);
  	int pos = distance(vertices.begin(), it);
	dist[pos] = 0;

	while (!pq.empty()) {
		auto u = pq.top().second;
		pq.pop();

		it = find(vertices.begin(),vertices.end(),u);
	  	pos = distance(vertices.begin(), it);

		vector< pair<D, Edge<D,W>> > adj = adjacent(u);
		for(auto j: adj) {
			auto m = j.first;
            auto s = j.second.weight;

			auto it2 = find(vertices.begin(),vertices.end(),m);
		  	int pos2 = distance(vertices.begin(), it2);

			if (dist[pos2] > dist[pos] + s) {
                dist[pos2] = dist[pos] + s;
				cout << "Pushing " << m << " -> w:  " << dist[pos2] << endl;
                pq.push(make_pair(dist[pos2], m));
            }
		}
	}

	cout << "Distances from source" << endl;
	for (int i = 0; i < vertices.size(); ++i)
		cout << vertices[i] << " -> " << dist[i] << endl;
}


#endif
