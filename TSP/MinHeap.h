/* 
 * File:   MinHeap.h
 * Author: mikel
 *
 * Created on 19 de mayo de 2015, 19:00
 */

#ifndef MINHEAP_H
#define	MINHEAP_H
#include "vector"

using namespace std;

typedef struct {
   int peso;
   vector<int> nodos;
   int l;
}ensayo;

class MinHeap
{
private:
    vector<ensayo> _vector;
    void BubbleDown(int index);
    void BubbleUp(int index);
    void Heapify();

public:
    MinHeap(ensayo* array, int length);
    MinHeap(const vector<ensayo>& vector);
    MinHeap();

    void Insert(ensayo newValue);
    ensayo GetMin();
    void DeleteMin();
    void DeleteFrom(int n);
    bool isEmpty();
};

#endif	/* MINHEAP_H */

