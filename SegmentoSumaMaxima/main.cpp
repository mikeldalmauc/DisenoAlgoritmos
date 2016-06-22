/* 
 * File:   main.cpp
 * Author: mdalmau002
 *
 * Created on 8 de marzo de 2015, 12:35
 */

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>

using namespace std;
 
typedef struct {
   int i;
   int j;
   int sum;
} sumAndIndex;

void crearBloquePruebas();
void crearFicheroPrueba(int size ,string name);
int leerFichero(string fname, vector<int> *pV, int *n);

void segSumaMaxH(string fname);
void segSumaMaxF(string fname);

sumAndIndex sumMaxCentro(vector<int> *v,int pi, int pj,int centro);
sumAndIndex segSumaMaxR(vector<int> *v,int pi, int pj);
void segSumaMaxRecursivo(string fname);

void Kadane(string fname);

void ProgramacionDinamica(string fname);

void ejecutarBloqueKadane();
void ejecutarBloqueProgDin();
void ejecutarBloqueR();
void ejecutarBloqueH();
void ejecutarBloqueF();

int main ()
{
   //crearBloquePruebas();
   
   //ejecutarBloqueProgDin();
   //ejecutarBloqueKadane();
   //ejecutarBloqueR();
   //ejecutarBloqueF();
   ejecutarBloqueH();
    
   return 0;
}

void ProgramacionDinamica(string fname){
    vector<int> v;
    int n,max_start,start_tmp,max_end,max;
    leerFichero(fname, &v, &n);
 
    vector<int> s(v.size());
    s[0] = v[0]; max = v[0]; max_start = 0; max_end = 0; start_tmp = 0;

    for(int i=1; i < v.size() ; i++){
        
        if(s[i-1] + v[i] < v[i]){
            s[i] = v[i];
            
            if(v[i] > max){
                  max = v[i];
                  max_start = i;
                  max_end = i;
            }else{
                 start_tmp = i;
            }
            
        }else{
            s[i] = s[i-1] + v[i];
            
            if(s[i] > max){
                max_start = start_tmp;
                max_end = i;
                max = s[i];
            }
        }
    }
    
    cout << max_start << " " << max_end << " " << max << endl; 
};

void Kadane(string fname){
    vector<int> v;
    int max, max_temp, n, start, end, max_start, max_end,val_temp,max_special,pos_special;
    leerFichero(fname, &v, &n);
    max_temp = v.at(0); max = max_temp; start = 0; end = 0; max_start = 0; max_end = 0; max_special = v.at(0); pos_special = 0;
    for (int i = 1; i < n; i++){
        val_temp =  v.at(i);
        if(val_temp > max_special){
            max_special  = val_temp;
            pos_special = i;
        }
        if( val_temp >  (max_temp + val_temp)){
            start = i;
            end = i;
            max_temp =  val_temp;
        }else{
            end = i;
            max_temp = max_temp + val_temp;
        }
        if(max_temp > max){
            max = max_temp;
            max_start = start;
            max_end = end;
        }
    }
    if(max_special < 0){
        cout << pos_special << " " << pos_special << " " << max_special << endl; 
    }else{
        cout << max_start << " " << max_end << " " << max << endl; 
    }
};
/*
 *
 */
void segSumaMaxRecursivo(string fname){
    vector<int> V;
    int n;
    sumAndIndex sol;
 
    leerFichero(fname, &V, &n);
    sol = segSumaMaxR(&V, 0, n-1);
    cout << sol.i << " " << sol.j << " " << sol.sum << endl; 
};
/*
 *
 */
/*
 *
 */
sumAndIndex sumMaxCentro(vector<int> *v,int pi, int pj,int centro){
    int k,partial,maxI,maxD,auxI,auxD;
    sumAndIndex e;
    int i, j;
    i = pi; j = pj;
    
    maxI = v->at(centro); auxI = centro; partial = v->at(centro);
    for(k = centro-1; k > i-1; k--){
        partial = partial + v->at(k);
        if(partial > maxI){
            maxI = partial;
            auxI = k;
        }
    }
    maxD = v->at(centro+1); auxD = centro + 1; partial = v->at(centro+1);
    for(k = centro+2; k < j+1; k++){
        partial = partial + v->at(k);
        if(partial > maxD){
            maxD = partial;
            auxD = k;
        }
    }
    e.i = auxI; e.j = auxD; e.sum = maxI + maxD;
    return e;
};
/*
 *
 */
void segSumaMaxF(string fname){
    vector<int> V;
    int n,i,j,sum,maxSum,start,end;
    
    leerFichero(fname, &V, &n);
    maxSum = V.front(); start = 0; end = 0;
    for(i = 0; i < n ; i++){
        sum = 0;
        for(j = i; j < n ; j++){
            sum = sum + V.at(j);
            if( sum > maxSum){
                maxSum = sum;
                start = i;
                end = j;
            }
        }
    }
    cout << start << " " << end << " " << maxSum << endl;
};
/*
 *
 */
void segSumaMaxH(string fname){
    vector<int> V;
    int n,i,j,k,sum,maxSum,start,end;
   
    leerFichero(fname, &V, &n);
    maxSum = V.front(); start = 0; end = 0;
    for(i = 0; i < n ; i++){
        for(j = i; j < n ; j++){
            sum = 0;
            for(k = i; k < j +1; k++){
                sum = sum + V.at(k);
                if( sum > maxSum){
                    maxSum = sum;
                    start = i;
                    end = j;
                }
            }
        }
    }
    cout << start << " " << end - 1 << " " << maxSum << endl; 
};
/* 
 * Input: Nombre de fichero y apuntador a array de enteros y tamaño del array
 * Outpu: Rellena array de enteros con los datos del fichero
 */
int leerFichero(string fname, vector<int> *pV, int *n){
    int i,lines;
    ifstream fe(fname.c_str());
    
    fe >> lines;
    *n = lines;
    if(not(lines > 1000000 or lines < 1)){
        vector<int> V(lines);
        for(i = 0; i < lines ; i++){
            fe >> V.at(i);
        }
        *pV = V;
        fe.close();
        return 0;
    }
    fe.close();
    return -1;
};
/* 
 * Input: int size, tamaño de la lista de elementos, fname nombre para el fichero
 * Outpu: Construye un fichero con un entero positivo en la primera línea indicando el número de líneas
 *  que contiene el fichero
 */
void crearFicheroPrueba(int size , string fname){
    int i,num;
    ofstream fs(fname.c_str());
    
    fs << size << endl;
    for (i = 0; i < size ; i++ ){     
        num = (rand() % 201) - 100;
        fs << num << endl;
    }
    fs.close();
};
/*
 * Construye un bloque de ficheros de distintos tamaños para realizar las pruebas
 */
void crearBloquePruebas(){
    crearFicheroPrueba(1000,"1Ksec.txt");
    crearFicheroPrueba(2000,"2Ksec.txt");
    crearFicheroPrueba(4000,"4Ksec.txt");
    crearFicheroPrueba(5000,"5Ksec.txt");
    crearFicheroPrueba(8000,"8Ksec.txt");
    crearFicheroPrueba(16000,"16Ksec.txt");
    crearFicheroPrueba(10000,"10Ksec.txt");
    crearFicheroPrueba(32000,"32Ksec.txt");
    crearFicheroPrueba(64000,"64Ksec.txt");
    crearFicheroPrueba(100000,"100Ksec.txt");
    crearFicheroPrueba(200000,"200Ksec.txt");
    crearFicheroPrueba(500000,"500Ksec.txt");
    crearFicheroPrueba(750000,"750Ksec.txt");
    crearFicheroPrueba(1000000,"1000Ksec.txt");
};
/*
 *En los siguientes bloques está definido como se ejecutan las pruebas y con que ficheros
 * 
 */
void ejecutarBloqueKadane(){
     
    clock_t tic,toc;
    cout << "Kadane" << endl;
    cout << endl;
    
    tic = clock();
    Kadane("dSSM3200S92.txt");
    toc = clock();
    cout << "dSSM3200S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    Kadane("dSSM320KS138.txt");
    toc = clock();
    cout << "dSSM320KS138: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    Kadane("dSSM32S46.txt");
    toc = clock();
    cout << "dSSM32S46: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    Kadane("dSSM96S92.txt");
    toc = clock();
    cout << "dSSM96S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
   /*
    tic = clock();
    Kadane("750Ksec.txt");
    toc = clock();
    cout << "Elapsed 750K: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    Kadane("1000Ksec.txt");
    toc = clock();
    cout << "Elapsed 1000K: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    */ 
    cout << endl;
};

void ejecutarBloqueProgDin(){
     
    clock_t tic,toc;
    cout << "ProgDin" << endl;
    cout << endl;
    tic = clock();
    ProgramacionDinamica("dSSM3200S92.txt");
    toc = clock();
    cout << "dSSM3200S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    ProgramacionDinamica("dSSM32S46.txt");
    toc = clock();
    cout << "dSSM32S46: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    ProgramacionDinamica("dSSM96S92.txt");
    toc = clock();
    cout << "dSSM96S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    ProgramacionDinamica("dSSM320KS138.txt");
    toc = clock();
    cout << "dSSM320KS138: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
     cout << endl;
};

void ejecutarBloqueH(){
    
    clock_t tic,toc;
    
    cout << "Cúbico" << endl;
    cout << endl;
    tic = clock();
    segSumaMaxH("dSSM3200S92.txt");
    toc = clock();
    cout << "dSSM3200S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxH("dSSM32S46.txt");
    toc = clock();
    cout << "dSSM32S46: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxH("dSSM96S92.txt");
    toc = clock();
    cout << "dSSM96S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxH("dSSM320KS138.txt");
    toc = clock();
    cout << "dSSM320KS138: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
   /* 
    tic = clock();
    segSumaMaxH("10Ksec.txt");
    toc = clock();
    cout << "Elapsed 10K: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;*/
    cout << endl;
};

void ejecutarBloqueF(){
    clock_t tic,toc;
    cout << "Cuadratico" << endl;
    cout << endl;
    
   tic = clock();
    segSumaMaxF("dSSM3200S92.txt");
    toc = clock();
    cout << "dSSM3200S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxF("dSSM32S46.txt");
    toc = clock();
    cout << "dSSM32S46: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxF("dSSM96S92.txt");
    toc = clock();
    cout << "dSSM96S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxF("dSSM320KS138.txt");
    toc = clock();
    cout << "dSSM320KS138: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    
    /*
    tic = clock();
    segSumaMaxF("200Ksec.txt");
    toc = clock();
    cout << "Elapsed 32K: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;*/
    cout << endl;
};

void ejecutarBloqueR(){
    clock_t tic,toc;
    
    cout << " Recursivo" << endl;
    cout << endl;
      
    tic = clock();
    segSumaMaxRecursivo("dSSM3200S92.txt");
    toc = clock();
    cout << "dSSM3200S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxRecursivo("dSSM32S46.txt");
    toc = clock();
    cout << "dSSM32S46: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxRecursivo("dSSM96S92.txt");
    toc = clock();
    cout << "dSSM96S92: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;
    
    tic = clock();
    segSumaMaxRecursivo("dSSM320KS138.txt");
    toc = clock();
    cout << "dSSM320KS138: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds" << endl;

    cout << endl;
};
