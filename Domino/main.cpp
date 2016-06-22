/* 
 * File:   main.cpp
 * Author: mikel
 *
 * Created on 10 de mayo de 2015, 11:40
 */
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <string.h>

using namespace std;

typedef struct{
    int nodo1;
    int nodo2;
}arista;

typedef struct {
   int** aristas;
   vector<int> gradoNodos;
   vector<arista> vAristas;
   int N;
   int A;
}grafo;

//Realiza un recorrido en profundidad desde el nodo parámetro
void visita(grafo *G,vector<bool> *pV, int nodo){
     pV->at(nodo)=true;
     for(int i= 0; i<7; i++){
         if(G->aristas[nodo][i] != 0 and pV->at(i)==false){
             visita(G,pV,i);
         }
     }
}
//Detemina si un grafo es conexo y responde verdadero o falso
bool conexo(grafo *G){
    vector<bool> visitados(7);
    int i = 0; bool found = false;
    while(i < 7 and !found){
        if(G->gradoNodos[i] != 0){
            found = true;
        }
        i++;
    }
    visita(G,&visitados,i);
    for(i=0; i<7; i++){
        if(G->gradoNodos[i] != 0 and visitados[i]==false){
            return false;
        }
    }
    return true;
}
//Cuenta el grado de cada vértice
int contarGradoVertices(grafo *G){
    int i,j, grado, impares;
    impares = 0;
    for(i=0; i<G->N; i++){
        grado = 0;
        for(j=0;j<G->N;j++){
             if(G->aristas[i][j] == 1){
                 grado++;
                 if(i == j)
                     grado++;
             }
        }
        G->gradoNodos[i]=grado;
        if(grado % 2 != 0){
            impares++;
        }
    }
    return impares;
}
void inicializarGrafo(grafo *G){
    int** aristas;
    aristas = new int* [7]; 
    vector<int> nodos(7);
    for (int j = 0; j < 7; j++){   
        nodos[j]=0;
        aristas[j] = new int [7]; 
        for(int h=0; h< 7 ; h++)
            aristas[j][h] = 0;
    }
    G->gradoNodos = nodos;
    G->aristas=aristas;
    G->N = 7;
    G->A = 0;
}
void imprimirMatriz(grafo *G){
    for(int i = 0; i < G->N; i++){
        for(int j = 0; j < G->N; j++){
            cout << G->aristas[i][j] <<  "\t";  
        }   
        cout << endl;
    }
};
grafo leerFichero(string pFname){
    int i=0,A,nodo1,nodo2,j; arista a;
    grafo G;
    inicializarGrafo(&G);
    ifstream fe(pFname.c_str());
    //número de fichas de domino
    fe >> A;
    G.A = A;
    G.vAristas = vector<arista>(A);
    for(i = 0; i < A ; i++){
        fe >> nodo1;
        fe >> nodo2;
        G.aristas[nodo1][nodo2]++;
        a.nodo1=nodo1; a.nodo2=nodo2;
        G.vAristas[i]=a;
        if(nodo1 != nodo2)
            G.aristas[nodo2][nodo1]++;
    }
    fe.close();
    return G;
};
//Elimina una arista del grafo y comprueba si hace que el grafo resultante no sea conexo
bool puente(grafo pG, int nodo1, int nodo2){
    grafo G=pG;
    G.aristas[nodo1][nodo2] = 0;
    G.aristas[nodo2][nodo1] = 0;
    G.gradoNodos[nodo1]--;
    G.gradoNodos[nodo2]--;
    if(conexo(&G)){ return false; }
    else{ return true;}
}
void camino_Euleriano(grafo *G){
    vector<bool> aristasVisitadas(G->A);
    std::fill(aristasVisitadas.begin(),aristasVisitadas.end(),false);
    //Buscamos un nodo desde el que empezar a recorrer el grafo
    int nodoActual=0; bool found=false;
    while(nodoActual < 7 and !found){
        if(G->gradoNodos[nodoActual] != 0){
            found = true;
        } nodoActual++;
    }
    for(int i=0; i<G->A; i++)
        for(int j=0; j<G->A; j++)
            if(!aristasVisitadas[j] and (G->vAristas[j].nodo1 == nodoActual or G->vAristas[j].nodo2 == nodoActual) and !puente(*G,G->vAristas[j].nodo1,G->vAristas[j].nodo2)){
                aristasVisitadas[j]=true;
                //cout << "vuelta"<<i<<j<<endl; 
                G->aristas[G->vAristas[j].nodo1][G->vAristas[j].nodo2]--;
                G->aristas[G->vAristas[j].nodo2][G->vAristas[j].nodo1]--;
                G->gradoNodos[nodoActual]--;
                if(G->vAristas[j].nodo1 == G->vAristas[j].nodo2){
                    G->gradoNodos[nodoActual]--;
                }else{
                    if(G->vAristas[j].nodo1 == nodoActual){
                        nodoActual= G->vAristas[j].nodo2;
                    }else{
                        nodoActual= G->vAristas[j].nodo1;
                    }
                }
                cout<<nodoActual<<" - ";//Escribimos los nodos visitados en la pantalla
            }
    cout << " fin de busqueda"<<endl;  
}
void domino(string fname){
    grafo G;
    G = leerFichero(fname);
    //imprimirMatriz(&G);
    
    int impares = contarGradoVertices(&G);
    if(!conexo(&G)){cout << "El grafo no es conexo no, no existen caminos eulerianos"<< endl; return;}
    else{ cout << "El grafo es conexo."<< endl;}
    if(impares == 2){cout << "El grafo tiene: " << impares << " vértices de grado impar, posee un ciclo euleriano."<< endl; 
        camino_Euleriano(&G);}
    else if( impares == 0 ){cout << "El grafo tiene: " << impares << " vértices de grado impar, posee un camino euleriano."<< endl; 
        camino_Euleriano(&G);}
    else{cout << "El grafo tiene: " << impares  << " vértices de grado impar, no contiene ciclos ni caminos eulerianos."<< endl; return;}
}
int main(int argc, char** argv) {
    domino("JuegoCompleto");
    domino("JuegoIncompleto");
    domino("JuegoInsatisfactible");
    return 0;
}

