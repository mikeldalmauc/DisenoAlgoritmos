/* 
 * File:   main.cpp
 * Author: mikel
 *
 * Created on 7 de mayo de 2015, 17:02
 */
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <string.h>
#include <limits>
#define INFINITO -1
#define DEFAULT -1
#define INGENUO 0
#define ACOTADO 1
#define BESTFIRST 2
#define PRE_CONEXO 1
#define PRE_GRADO 0
#define PRE_GRADO_CONEXO 2
#define PRE_BOND_CHVA 3

#include "MinHeap.h"

using namespace std;

clock_t tic,toc;
 
typedef struct {
   int** pesos;
   vector<int> grado;
   int N;
   int A;
}grafo;

void crearGrafoCompleto(int n, string fname);
int factorial(int n);
bool OreCond(grafo *G);
bool DiracCond(grafo *G);
bool esCompleto(grafo *G);
grafo cerradura(grafo G);
bool contarGradoVertices(grafo *G);
void visita(grafo *G,vector<bool> *pV, int nodo);
bool conexo(grafo *G);
void swap(ensayo *pE, int i, int j);
int leerFichero(string pFname, grafo *pG);
void imprimirMatriz(grafo *G);
ensayo TSP_Backtrack_Acotado(grafo *G,ensayo E,ensayo Eopt,int l);
ensayo TSP_Backtrack_Ingenuo(grafo *G,ensayo E,ensayo Eopt,int l);
ensayo TSP_Backtrack_PrimeroMejor(grafo *G,ensayo Einicial);
void TSP(string pFname, int TIPOALG, int PREPROC);
int main(int argc, char** argv);

int main(int argc, char** argv) {  
    
    vector<string> pruebas;
   
    //pruebas.push_back("N10_A45_IL.txt");     pruebas.push_back( "N15_A105_IL.txt");  pruebas.push_back("N17_A136_IL.txt");    pruebas.push_back("N18_A153_IL.txt");  
   // pruebas.push_back("4N8A");  pruebas.push_back("10N20A");     pruebas.push_back("16N33A");    pruebas.push_back("18N42A");
   //pruebas.push_back("20N52A");     pruebas.push_back("21N61A");   // pruebas.push_back("22N69A");    
 
   // crearGrafoCompleto(9,"9N"); crearGrafoCompleto(10,"10N"); crearGrafoCompleto(11,"11N"); crearGrafoCompleto(12,"12N");    crearGrafoCompleto(13,"13N");   crearGrafoCompleto(15,"15N");
    //crearGrafoCompleto(7,"7N"); crearGrafoCompleto(8,"8N");  crearGrafoCompleto(14,"14N");     crearGrafoCompleto(16,"16N");     
   
    //pruebas.push_back("7N");pruebas.push_back("8N"); pruebas.push_back("9N");pruebas.push_back("10N");pruebas.push_back("11N"); pruebas.push_back("12N"); pruebas.push_back("13N");
    //pruebas.push_back("14N"); pruebas.push_back("15N"); pruebas.push_back("16N"); 
    
   
   pruebas.push_back("TSP2Pruebas.txt");
        
    
    //pruebas.clear();
    //crearGrafoCompleto(17,"17N"); crearGrafoCompleto(18,"18N"); crearGrafoCompleto(19,"19N"); crearGrafoCompleto(20,"20N"); crearGrafoCompleto(21,"21N"); crearGrafoCompleto(22,"22N"); 
    //pruebas.push_back("17N");pruebas.push_back("18N"); pruebas.push_back("19N");pruebas.push_back("20N");pruebas.push_back("21N"); pruebas.push_back("22N");
    
   for(int i=0; i < pruebas.size() ; i++){
        cout<< endl;
        cout<< "Pruebas Backtrack Acotado: "<<endl;
        cout<< pruebas[i] << endl;
        TSP(pruebas[i],ACOTADO,PRE_GRADO_CONEXO);
        cout<< endl;
    }
    
    for(int i=0; i < pruebas.size() ; i++){
        cout<< "Pruebas Backtrack Best First: "<<endl;
        cout<< pruebas[i] << endl;
        TSP(pruebas[i],BESTFIRST ,PRE_GRADO_CONEXO);
        cout<< endl;
    }
    
    for(int i=0; i < pruebas.size() ; i++){
        cout<< "Pruebas Backtrack Best First: "<<endl;
        cout<< pruebas[i] << endl;
        TSP(pruebas[i],INGENUO ,PRE_GRADO_CONEXO);
        cout<< endl;
    }
    
   
    cout<< endl;
    cout<< endl;
   
     
    return 0;
}
void TSP(string pFname, int TIPOALG, int PREPROC){
    grafo G; 
    ensayo E,Eopt; 
    int N =  leerFichero(pFname,&G);
    bool satisf = true;   
    
    //imprimirMatriz(&G);
    
    switch (PREPROC){
        
        case PRE_GRADO:
            satisf = contarGradoVertices(&G);
            break;
            
        case PRE_CONEXO:
            satisf = conexo(&G);
            break;
            
        case PRE_GRADO_CONEXO:
            satisf = contarGradoVertices(&G);
            satisf = conexo(&G);
            break;
            
       case  PRE_BOND_CHVA:
             if (!contarGradoVertices(&G)){satisf = false;}
             else{ 
                 if (!conexo(&G)){satisf = false;} 
                 else{
                     grafo cG =cerradura(G);
                     if(esCompleto(&G)){satisf = true;}
                     if(DiracCond(&G)){satisf = true;}
                     if(OreCond(&G)){satisf = true;}
                 }
             } 
             break;
        default:
            break;
    }
    if(satisf){
    
        E.peso = 0; E.l=0;
        E.nodos = vector<int>(G.N);
        for(int i = 0; i < G.N ; i++)
            E.nodos[i] = i;
        Eopt.peso = INFINITO;
        Eopt.nodos = vector<int>(G.N);
        
        switch (TIPOALG){
            
            case INGENUO:
                tic = clock();
                Eopt = TSP_Backtrack_Ingenuo(&G,E,Eopt,0);
                toc = clock();
                cout << "Ingenuo: " <<"  "<<((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
                break;
                
            case ACOTADO:
                tic = clock();
                Eopt = TSP_Backtrack_Acotado(&G,E,Eopt,0);
                toc = clock();
                cout << "Acotado: " <<"  "<<((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
                break;
                
            case BESTFIRST:
                tic = clock();
                Eopt = TSP_Backtrack_PrimeroMejor(&G,E);
                toc = clock();
                cout << "Mejor el primero: " <<"  "<<((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
                break;
                
            default:
                cout << "Algoritmo seleccionado incorrecto" << endl ; return;
                break;
        }
        if(Eopt.peso != INFINITO){
            for(int j= 0; j< Eopt.nodos.size(); j++)
                cout << Eopt.nodos[j] << " ";
            cout << endl << " Peso del recorrido: " << Eopt.peso  << endl; return;  
        }
    }   
    cout << "Insatisfactible" << endl ; return;
};

ensayo TSP_Backtrack_Ingenuo(grafo *G,ensayo E,ensayo Eopt,int l){
    int n = G->N;
    
    if( n-1 == l){
        if(G->pesos[ E.nodos[n-1] ][ E.nodos[0] ] != INFINITO){
                Eopt.peso = E.peso + G->pesos[ E.nodos[n-1] ][ E.nodos[0] ];
                Eopt.nodos.operator =(E.nodos);     
        }
    }else{
        Eopt.peso=INFINITO;
        int antiguoPeso = E.peso;
        
        for(int i = l+1; i < n; i++)
            if( G->pesos[ E.nodos[l] ][ E.nodos[i] ] != INFINITO){          
                int nuevoPeso = E.peso + G->pesos[ E.nodos[l] ][ E.nodos[i] ];
                E.peso = nuevoPeso;
                swap(&E,l+1,i);    
                ensayo Eaux = TSP_Backtrack_Ingenuo(G,E,Eopt,l+1);
                if(Eopt.peso == INFINITO){
                    if(Eaux.peso != INFINITO){  Eopt = Eaux;}   
                }else{
                    if(Eaux.peso != INFINITO and Eaux.peso < Eopt.peso){  Eopt = Eaux;  }   
                }
                swap(&E,i,l+1);
                E.peso = antiguoPeso;
            }
    }
    return Eopt;
};

ensayo TSP_Backtrack_Acotado(grafo *G,ensayo E,ensayo Eopt,int l){
    int n = G->N;
    
    if( n-1 == l){ 
        if(G->pesos[ E.nodos[n-1] ][ E.nodos[0] ] != INFINITO){ 
            if(Eopt.peso == INFINITO){
                Eopt.peso = E.peso + G->pesos[ E.nodos[n-1]][E.nodos[0] ];
                Eopt.nodos.operator =(E.nodos);  
            }else{
                if(E.peso + G->pesos[ E.nodos[n-1]][ E.nodos[0] ] < Eopt.peso){
                    Eopt.peso = E.peso + G->pesos[ E.nodos[n-1]][ E.nodos[0] ];
                    Eopt.nodos.operator =(E.nodos);  
                }   
            }
           
        }
    }else{
        int antiguoPeso = E.peso;
 
        for(int i = l+1; i < n; i++)
            if(G->pesos[ E.nodos[l] ][ E.nodos[i] ] != INFINITO){
                int nuevoPeso = E.peso + G->pesos[ E.nodos[l] ][ E.nodos[i] ];
                if(Eopt.peso==INFINITO or nuevoPeso < Eopt.peso){
                    E.peso = nuevoPeso;
                    swap(&E,l+1,i);    
                    ensayo Eaux = TSP_Backtrack_Acotado(G,E,Eopt,l+1);
                    if(Eopt.peso == INFINITO){
                        if(Eaux.peso != INFINITO){  Eopt = Eaux;}   
                    }else{
                        if(Eaux.peso != INFINITO and Eaux.peso < Eopt.peso){  Eopt = Eaux;  }   
                    }
                    swap(&E,i,l+1);
                    E.peso = antiguoPeso;
                }
            }
    }
    return Eopt;
};

ensayo TSP_Backtrack_PrimeroMejor(grafo *G,ensayo Einicial){
    ensayo E; int n = G->N; bool fin = false;
    int pesoOPT  = INFINITO;

    ensayo e[] = {Einicial};
    MinHeap monticulo = MinHeap(e,1);
    E  = monticulo.GetMin();

    while(!fin){ 
        monticulo.DeleteMin();
        int l = E.l;
        
        if( l ==  n ) {
            return E;
            
        }else if( l == n-1 ) {
            if(G->pesos[ E.nodos[n-1] ][ E.nodos[0] ] != INFINITO){ 
                    E.peso = E.peso + G->pesos[ E.nodos[n-1] ][ E.nodos[0] ];
                    E.l = n;
                    if(pesoOPT == INFINITO or E.peso < pesoOPT){
                        pesoOPT = E.peso;
                        monticulo.Insert(E);
                        monticulo.DeleteFrom(pesoOPT);
                    }
            }
        }else{
            int antiguoPeso = E.peso; 
            for(int i = l+1; i < n; i++){
                if( G->pesos[ E.nodos[l] ][ E.nodos[i] ] != INFINITO){          
                    int nuevoPeso = antiguoPeso + G->pesos[ E.nodos[l] ][ E.nodos[i] ];
                    if(pesoOPT==INFINITO or nuevoPeso < pesoOPT){
                        E.peso = nuevoPeso;
                        swap(&E,l+1,i);    
                        E.l = l + 1;
                        monticulo.Insert(E);
                        E.l= l - 1; 
                        swap(&E,l+1,i); 
                    }   
                }
            }
        }
        if(monticulo.isEmpty()){ fin = true; }
        else{E = monticulo.GetMin();}
    }
    return E;
};

grafo cerradura(grafo G){
    grafo cG; 

    cG.A = G.A; cG.N = G.N;
    cG.grado=G.grado; 
    int** pesos;
    pesos = new int* [cG.N]; 
    for (int j = 0; j < cG.N; j++){   
        pesos[j] = new int [ cG.N]; 
        for(int h=0; h<  cG.N ; h++)
           pesos[j][h] = G.pesos[j][h];
    }
    cG.pesos=pesos;
    
    int i,j;
    bool finished = false;
    bool seAgregaArista = false;
    while(!finished){
        i = 0;
        seAgregaArista = false;
        while( i < cG.N and !seAgregaArista){
            j = 0;
            while( j < cG.N and !seAgregaArista){
                if(cG.pesos[i][j] == INFINITO and i!=j)
                    if(cG.grado[i] + cG.grado[j] >= cG.N){
                        cG.pesos[i][j] = 1;
                        cG.pesos[j][i] = 1;
                        cG.grado[i]++;
                        cG.grado[j]++;
                        seAgregaArista = true;
                    }
                j++;
            }
            i++;
        }
        if(!seAgregaArista)
            finished = true;
    }
    return cG;
};
void swap(ensayo *pE, int i, int j){
    if(i < 0 or i > pE->nodos.size()){  return; }
    if(j < 0 or j > pE->nodos.size()){  return; }
    
    int swap = pE->nodos[j];
    pE->nodos[j] = pE->nodos[i];
    pE->nodos[i] = swap;
};
bool conexo(grafo *G){
    vector<bool> visitados(G->N);
    visita(G,&visitados,0);
    for(int i=0; i<G->N; i++)
        if(visitados[i]==false){
            return false;
        }
    return true;
};
void visita(grafo *G,vector<bool> *pV, int nodo){
     pV->at(nodo)=true;
     for(int i= 0; i<G->N; i++){
         if(G->pesos[nodo][i] != INFINITO and pV->at(i)==false){
             visita(G,pV,i);
         }
     }
};
bool contarGradoVertices(grafo *G){
    G->grado=vector<int>(G->N);
    for(int i=0; i<G->N; i++){
        int grado = 0;
        for(int j=0;j<G->N;j++)
            if(G->pesos[i][j] != INFINITO)
                 grado++;
        G->grado[i]=grado;
        if(grado < 2){
            return false;
        }
    }
    return true;
};
bool esCompleto(grafo *G){
    for(int i=0; i<G->grado.size();i++)
        if( G->grado[i] != G->N)
            return false;
    return true;
};
bool DiracCond(grafo *G){
    for(int i=0; i<G->grado.size();i++)
        if( G->grado[i] <= G->N/2 )
            return false;
    return true;
};
bool OreCond(grafo *G){
    for(int i=0; i<G->N; i++)
        for(int j=0; j<G->N; j++)
            if(G->pesos[i][j] == INFINITO and i!=j)
                if( G->grado[i] +G->grado[j] < G->N)
                    return false;
    return true;
};
int leerFichero(string pFname, grafo *pG){
    int i=0,origen,destino,peso;
    grafo G;
    const int anInt = sizeof(anInt);
    char cadena[500];
    ifstream fe(pFname.c_str());
    
    for(int h = 0; h < 5; h++){
        fe.getline(cadena, 500);
    }
    fe >> G.N; fe >>  G.A;
    int** pesos;
    pesos = new int* [ G.N]; 
    for (int j = 0; j < G.N; j++){   
        pesos[j] = new int [ G.N]; 
        for(int h=0; h<  G.N ; h++)
           pesos[j][h] = INFINITO;
    }
   
    for(i = 0; i < G.A ; i++){
        fe >> origen;
        fe >> destino;
        fe >> peso;
        pesos[origen][destino] = peso;
        pesos[destino][origen] = peso;
    }
    fe.close();
    G.pesos = pesos;
    *pG = G;
    return G.N;
};
void imprimirMatriz(grafo *G){
    for(int i = 0; i < G->N; i++){
        for(int j = 0; j < G->N; j++)
            cout << G->pesos[i][j] <<  "\t";  
        cout << endl;
    }
};
void crearGrafoCompleto(int n, string fname){
    ofstream fe(fname.c_str());
    int coste; int A= 0;
    for(int k = n ; k > 0; k--){A = A + k;}
    
    fe << "c" << endl;  fe << "c" << endl;  fe << "c" << endl; fe << "c" << endl; fe << "c" << endl;
    
    fe << n << " " << A << endl;
    for(int i = 0; i < n; i++)
        for(int j= i; j < n; j++){
            coste = 1 + (rand() % 30);
            if(j == i){
                fe << i << " " << j << " -1" << endl;
            }else{
                fe << i << " " << j << " " <<coste << endl;
            }
            
        }           
    fe.close();
}


