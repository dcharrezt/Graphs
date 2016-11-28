#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <stack>
#include <algorithm>
#include <typeinfo>
#include <queue>

using namespace std;

typedef pair< double, int >                              T_Arista;
typedef vector < T_Arista >                              T_Adyasentes;
typedef tuple< string, T_Adyasentes, bool>               T_Vertex;




bool operator==( const T_Vertex & a , const T_Vertex & b) {
	return get<0>(a) == get<0>(b);
}

bool operator>(T_Vertex & a , T_Vertex & b) {
	return get<0>(a) > get<0>(b);
}

bool operator<(T_Vertex & a , T_Vertex & b) {
	return get<0>(a) < get<0>(b);
}

typedef vector<T_Vertex>   			  	T_Grafo;
typedef vector<T_Vertex>::iterator    	T_Iterator_Grafo;


// Representa una estructura de conjuntos disjuntos
struct DisjointSets
{
    int *parent, *rnk;
    int n;

    // Constructor.
    DisjointSets(int n)
    {
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        // Inicialmente todos los vertices tienen diferente conjunto y rank 0
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            // Cada elemento es padre de si mismo
            parent[i] = i;
        }
    }

    // Encuentra el padre de u
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

class Grafo {
private:
	T_Grafo  m_Grafo;
public:
	Grafo(){};

	void insert_vertex(string v);
	void insert_Arista(string v1, string v2, double edge);
	void profundidad();
	void mostrar();

	//ALGORITMOS
	void Dijkstra(string v);
	int Kruskal();
	void Prim();
};


void Grafo::insert_vertex(string v) {
	T_Vertex vertice = tuple<string,T_Adyasentes,bool>(v,vector<T_Arista>(),false);
	m_Grafo.push_back(vertice);
}

void Grafo::insert_Arista(string v1, string v2, double edge) {
	T_Vertex V1 =  tuple<string,T_Adyasentes,bool>(v1,vector<T_Arista>(),false);
	T_Vertex V2 =  tuple<string,T_Adyasentes,bool>(v2,vector<T_Arista>(),false);

	T_Grafo::iterator it1 = find(m_Grafo.begin(),m_Grafo.end(), V1);
	T_Grafo::iterator it2 = find(m_Grafo.begin(),m_Grafo.end(), V2);

	if (it1 != m_Grafo.end() && it2 != m_Grafo.end()) {
		int pos2 = distance(m_Grafo.begin(),it2);
		get<1>(*it1).push_back(T_Arista(edge,pos2));
	}
}

void Grafo::profundidad() {
	cout<<"Profundidad:  "<<endl;
	stack< T_Iterator_Grafo >    pila;
	T_Iterator_Grafo it = m_Grafo.begin();
	pila.push(it);

	while(!pila.empty()) {
		T_Iterator_Grafo tmp = pila.top();
		pila.pop();
		cout<< get<0>(*tmp) <<endl;;
		get<2>(*tmp) = true;
		T_Adyasentes & adyasentes  = get<1>(*tmp);
		int pos;

		for( int i = 0; i<adyasentes.size() ;i++ ) {
			pos = adyasentes[i].second;
			if (!get<2>(m_Grafo[pos])) {
				pila.push(m_Grafo.begin()+pos);
			}
		}
	}
}

void Grafo::mostrar() {
	for (auto i: m_Grafo) {
		cout<<"Vertice:  "<<get<0>(i)<<endl;
		for(auto j:get<1>(i)) {
		  cout<<"\t Peso: "<<j.first<<"  Para: "<<get<0>(m_Grafo[j.second])<<endl;
		}
	}
}


// Recibe desde donde se hallaran las distancias a todos los demas vertices
void Grafo::Dijkstra(string v) {

    priority_queue< T_Vertex, vector < T_Vertex> , greater<T_Vertex> > pq;

    vector<int> dist(m_Grafo.size(), 99999);

	T_Vertex V1 =  tuple<string,T_Adyasentes,bool>(v,vector<T_Arista>(),false);
	T_Grafo::iterator it1 = find(m_Grafo.begin(),m_Grafo.end(), V1);
	int src = distance(m_Grafo.begin(),it1);
	dist[src] = 0;

	for( auto m : m_Grafo) {
		if ( get<0>(m) == v) {
			pq.push(m);
			break;
		}
	}

    while (!pq.empty()) {
		T_Vertex V2 = pq.top();
		T_Iterator_Grafo temp = find(m_Grafo.begin(),m_Grafo.end(), V2);
		int u = distance(m_Grafo.begin(), temp);
        pq.pop();
     	T_Iterator_Grafo i;

        for (i = m_Grafo.begin(); i != m_Grafo.end(); ++i) {
			for( auto ms : get<1>(*i) ) {
				int pos = ms.second;
	            int weight = ms.first;

				if ( dist[pos] > dist[u] + weight) {
					dist[pos] = dist[u] + weight;
					pq.push( m_Grafo[pos] );
				}
			}
        }
    }

	/* Llena el vector con las distancias que tienen el mismo indice de los
		vertices en m_Grafo */

}

int Grafo::Kruskal() {

    int total = 0;
	int tam = m_Grafo.size();

	vector< pair<int, T_Arista>> edges;
	int c = 0;
	for ( auto i : m_Grafo) {
		for( auto j : get<1>(i) ) {
				edges.push_back(make_pair(c,j));
		}
		c++;
	}

    sort(edges.begin(), edges.end());
    DisjointSets ds(tam);

    for ( auto ms : edges ) {
        int u = ms.first;
        int v = ms.second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        if (set_u != set_v)
        {
			// Imprime arista que etara en el arbol de expacion minima
            cout << u << " - " << v << endl;

            // actualizar el total
            total += ms.second.first;

            // une dos conjuntos
            ds.merge(set_u, set_v);
        }
    }

    return total;

}
void Grafo::Prim() {

}






#endif
