#include "grafo.h"
#include "graph.h"

int main()
{
   /* Grafo con maps
   map< string , map< string, double> >  m_Grafo;
   m_Grafo["Arequipa"]["Lima"]=100;
   m_Grafo["Arequipa"]["puno"]=1070;

   cout<<m_Grafo["Arequipa"]["puno"]<<endl;
  */
	Grafo hola;
	hola.insert_vertex("aqp");
	hola.insert_vertex("lima");
	hola.insert_Arista("aqp","lima",13.5);
	hola.mostrar();

	return 1;
}
