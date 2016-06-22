/* 
 * File:   main.cpp
 * Author: mikel
 *
 * Created on 25 de marzo de 2015, 15:47
 */

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>

using namespace std;

vector<string> pruebas;

typedef struct {
   int tiempo;
   int beneficio;
   int unidadesMax;
   int unidadesSoluccion;
}producto;

typedef struct {
   int beneficioSuma;
   bool pertenece;
   int numProducto;
}valoresF;

int leerFichero(string fname, vector<producto> *pV);
void crearFicheroPrueba(int numProd, int numMinutos , string fname);
void imprimirTablaProduccion(vector<producto> *pV,int pMinutos);
void ejecutarBloquePruebas();
void crearBloquePruebas();
void imprimirSolucion(vector<producto> *pT);
void calcularBeneficioMaximo(string fname);
/*
 * 
 */
int main() {
    //crearBloquePruebas();
    //ejecutarBloquePruebas();
    cout << "N100M1440-Olatz.txt" << endl; 
    calcularBeneficioMaximo("N100M1440-Olatz.txt");
    cout << endl; 
    cout << "N10M100-Olatz.txt" << endl; 
    calcularBeneficioMaximo("N10M100-Olatz.txt");
    cout << endl;
    cout << "N20M100-Olatz.txt" << endl; 
    calcularBeneficioMaximo("N20M100-Olatz.txt");
    cout << endl;
    
    return 0;
}

void calcularBeneficioMaximo(string fname){
    int Minutos,Ksize = 0, Tj,Bj;
    
    vector<producto> Tabla;
    Minutos = leerFichero(fname,&Tabla);
    
    for(int i = 0; i < Tabla.size(); i++){
        Ksize = Ksize + Tabla[i].unidadesMax;
    }
   
    //Inicializamos la matriz de optimización
    valoresF** K;
    
    K = new valoresF* [Minutos+1]; 
    for (int j = 0; j < Minutos+1; j++)    
        K[j] = new valoresF [Ksize+1]; 
 
    //Inicializamos fila y columna 0
    for (int i = 0; i < Ksize+1; i++){
        K[0][i].beneficioSuma = 0;
        K[0][i].pertenece = false;
        K[0][i].numProducto = 0;
        
    }
    for(int i = 0; i < Minutos+1; i++){
        K[i][0].beneficioSuma = 0;  
        K[i][0].pertenece = false;
        K[i][0].numProducto = 0;
    }
    int i = 0;
    for(int j = 0; j < Tabla.size(); j++){
        Tj = Tabla[j].tiempo;
        Bj = Tabla[j].beneficio;
        for(int k = 0; k < Tabla[j].unidadesMax; k++){
            i++;
            for(int t = 1; t < Minutos+1; t++){
                K[t][i].numProducto = j;
                if( Tj > t){
                    K[t][i].beneficioSuma = K[t][i-1].beneficioSuma;
                    K[t][i].pertenece = false;
                }else{
                    if(K[t][i-1].beneficioSuma < K[t-Tj][i-1].beneficioSuma + Bj){
                        K[t][i].beneficioSuma = K[t-Tj][i-1].beneficioSuma + Bj;
                        K[t][i].pertenece = true;
                    }else{
                        K[t][i].beneficioSuma = K[t][i-1].beneficioSuma;
                        K[t][i].pertenece = false;
                    }
                }
            }
        }
    }
    //Recorremos la matriz incluyendo los elementos que perteneces a la solución 
    i = Minutos;
    int j = Ksize;
    while((i != 0) and (j != 0)){
        if(K[i][j].pertenece){
            Tabla[K[i][j].numProducto].unidadesSoluccion++;
            i = i - Tabla[K[i][j].numProducto].tiempo;
            j--;
        }else{
            j--;
        }
    }   
   imprimirTablaProduccion(&Tabla,Minutos);
   //imprimirSolucion(&Tabla); 
};

void imprimirSolucion(vector<producto> *pT){
    for(int i = 0; i < pT->size(); i++){
         cout << pT->at(i).unidadesSoluccion << endl; 
    }
};
/* 
 * Input: Nombre de fichero y apuntador al vector de productos 
 * Outpu: Rellena array de enteros con los datos del fichero y devuelve en número de minutos de producción
 */
int leerFichero(string fname, vector<producto> *pV){
    int i,lines,Minutos;
    ifstream fe(fname.c_str());
    
    fe >> lines;
    if(not(lines > 100 or lines < 1)){
        vector<producto> V(lines);
        for(i = 0; i < lines ; i++){
            fe >> V.at(i).tiempo;
            fe >> V.at(i).beneficio;
            fe >> V.at(i).unidadesMax;
            V.at(i).unidadesSoluccion = 0;
        }
        *pV = V;
        fe >> Minutos;
        fe.close();
        return Minutos;
    }
    fe.close();
    return -1;
};
/* 
 * Input: int size, tamaño de la lista de elementos, fname nombre para el fichero
 * Outpu: Construye un fichero con un entero positivo en la primera línea indicando el número de líneas
 *  que contiene el fichero
 */
void crearFicheroPrueba(int numProd, int numMinutos , string fname){
    int i,tiempo, benficio, unidadesMax;
    ofstream fs(fname.c_str());
    
    fs << numProd << endl;
    for (i = 0; i < numProd ; i++ ){     
        tiempo = 1 + (rand() % 1440);       // Tiempo de produccion posible [ 1 - 1440]
        benficio = 1 + (rand() % 50);     // Beneficio posible [1 - 50]
        //unidadesMax = 1 + (rand() % 10);  // unidadesMaximas posibles [1 - 100]
        unidadesMax = 100;
        fs << tiempo << " " << benficio << " " << unidadesMax << endl;    
    }
    fs << numMinutos << endl;
    pruebas.push_back(fname);
    fs.close();
};
/* 
 * Input: vector de productos
 * Outpu: imprime por pantalla la tabla de producción
 */
void imprimirTablaProduccion(vector<producto> *pT,int pMinutos){
    
    cout <<"Tabla de Producción"<< endl; 
    cout << endl;
    cout << "Nº\t" <<"Tiempo\t" << "Benef\t"<< "MaxUnid\t"<< "CantidadOPT\t" << endl; 
    for(int i = 0; i < pT->size(); i++){
         cout << i+1 << "\t"<< pT->at(i).tiempo << "\t" << pT->at(i).beneficio << "\t" << pT->at(i).unidadesMax << "\t"<< pT->at(i).unidadesSoluccion << endl; 
    }
    cout << endl;
    cout << "Tiempo para producir (min):  " << pMinutos << endl;
};

void ejecutarBloquePruebas(){

    clock_t tic,toc;
    for(int i =0; i< pruebas.size(); i++){
        tic = clock();
        //cout << endl;
        calcularBeneficioMaximo(pruebas.at(i));
        toc = clock();
        //cout << pruebas.at(i) << " : " << ((double)(toc - tic) / CLOCKS_PER_SEC)<< " segundos" <<endl;
        cout << ((double)(toc - tic) / CLOCKS_PER_SEC) << endl;;
        if(i == 9 or i == 19 or i == 29){
              cout << endl;
        }
        
    }
};

void crearBloquePruebas(){

    crearFicheroPrueba(10, 100, "10prod500min.txt");
    crearFicheroPrueba(20, 100, "20prod500min.txt");
    crearFicheroPrueba(30, 100, "30prod500min.txt");
    crearFicheroPrueba(40, 100, "40prod500min.txt");
    crearFicheroPrueba(50, 100, "50prod500min.txt");
    crearFicheroPrueba(60, 100, "60prod500min.txt");
    crearFicheroPrueba(70, 100, "70prod500min.txt");
    crearFicheroPrueba(80, 100, "80prod500min.txt");
    crearFicheroPrueba(90, 100, "90prod500min.txt");
    crearFicheroPrueba(100, 100, "100prod500min.txt");
   
};
