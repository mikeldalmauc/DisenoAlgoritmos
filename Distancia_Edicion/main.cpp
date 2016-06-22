/* 
 * File:   main.cpp
 * Author: mikel
 *
 * Created on 23 de marzo de 2015, 15:30
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <string.h>

using namespace std;

void calcularDistancia(char X[], char Y[]);
/*
 * 
 */
int main() {
    char X[100];
    char Y[100];
  
    cout << "Introduce la priemra palabra" << endl;
    scanf ("%79s",X);
    cout << "Introduce la segunda palabra" << endl;
    scanf ("%79s",Y);

    calcularDistancia(X,Y);
    return 0;
}

void calcularDistancia( char X[], char Y[]){
    
    int N = strlen(X) + 1;
    int M = strlen(Y) + 1;
    
    int E[N][M];
    
    E[0][0]=0;
    for(int i = 1; i < N; i++){
        E[i][0] = i;
    }    
    for(int j = 1; j < M ; j++){
        E[0][j] = j;
    }
    
    for(int i = 1; i < N ; i++)
        for(int j = 1; j < M ; j++){
            
            int diag = E[i-1][j-1];
            int up = E[i-1][j];
            int left = E[i][j-1];
            
            if((diag <= up) and (diag <= left)){
                if(X[i-1] !=  Y[j-1]){
                    E[i][j] = diag + 1;
                }else{
                    E[i][j] = diag;
                }       
            }else if(up <= left){
                E[i][j] = up + 1;
            }else{
                E[i][j] = left + 1;
            }
            
        }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
           cout << E[i][j] ;
        }
        cout << endl;
    }
//Apartir de aquí está el código para calcular las operaciones realizadas    
    
    int i=N-1; int j=M-1; string ops=""; 
    do{
        if(i==0){
            ops =" +" + ops;
            j--;
        }else if(j==0){
            ops =" -" + ops; 
            i--;
        }else{
            int diag = E[i-1][j-1];
            int up = E[i-1][j];
            int left = E[i][j-1];
            
            if((diag <= up) and (diag <= left)){       
                if(E[i][j] > diag){
                    ops =" x" + ops;
                }else{
                    ops =" =" + ops;
                    
                }
                i--; j--;
            }else if(up <= left){
                ops =" -" + ops; 
                i--;
            }else{
                ops =" +" + ops;
                j--;
            }
        }
           
    }while((i != 0) or (j != 0));    
  
    string x=" "; string y=" ";
    for(int i = 0; i< strlen(X); i++){
        x=x+X[i]+" ";
    }
    for(int i = 0; i< strlen(Y); i++){
        y=y+Y[i]+" ";
    }
    cout << endl;
    cout << endl;
    cout << "Distancia entre palabras = " << E[N-1][M-1]<< endl ;
    cout << endl;
    cout << "Operaciones para transformar una palabra en otra " << endl ;
    cout << x << endl;
    cout << ops << endl;
    cout << y << endl;
    cout << endl;
    cout << "Matriz E:"<< endl;
    cout << endl;
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
           cout << E[i][j] ;
        }
        cout << endl;
    }
}

//Descomentar en insertar para imprimir matriz E 
/*
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
           cout << E[i][j] ;
        }
        cout << endl;
    }
 * 
 *  string x=" "; string y=" ";
    for(int i = 0; i< strlen(X); i++){
        x=x+X[i]+" ";
    }
    for(int i = 0; i< strlen(Y); i++){
        y=y+Y[i]+" ";
    }
    
 */