//The Code Project Open License (CPOL) 1.02
//Enlace a la licencia http://www.codeproject.com/info/cpol10.aspx
//Enlace al código http://www.codeproject.com/Tips/816934/Min-Binary-Heap-Implementation-in-Cplusplus
//Autor: Anton Kaminsky
//Modificado por Mikel Dalmau para la asignatura Diseño de Algoritmos para funcionar con otra estructura de datos
//añadidos métodos:
// IsEmpty
// DeleteFrom

//#include "stdafx.h"
#include <stdio.h>

#include "MinHeap.h"

MinHeap::MinHeap(ensayo* array, int length) : _vector(length)
{
    for(int i = 0; i < length; ++i)
    {
        _vector[i] = array[i];
    }

    Heapify();
}

MinHeap::MinHeap(const vector<ensayo>& vector) : _vector(vector)
{
    Heapify();
}

MinHeap::MinHeap()
{   
}

void MinHeap::Heapify()
{
    int length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(i);
    }
}

void MinHeap::BubbleDown(int index)
{
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int minIndex = index;

    if(_vector[index].peso  > _vector[leftChildIndex].peso )
    {
        minIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[minIndex].peso  > _vector[rightChildIndex].peso ))
    {
        minIndex = rightChildIndex;
    }

    if(minIndex != index)
    {
        //need to swap
        ensayo temp = _vector[index];
        _vector[index] = _vector[minIndex];
        _vector[minIndex] = temp;
        BubbleDown(minIndex);
    }
}

void MinHeap::BubbleUp(int index)
{
    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_vector[parentIndex].peso > _vector[index].peso)
    {
        ensayo temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        BubbleUp(parentIndex);
    }
}

void MinHeap::Insert(ensayo newValue)
{
       _vector.push_back(newValue);
       BubbleUp(_vector.size()-1);
}

ensayo MinHeap::GetMin()
{
    return _vector[0];
}
    
void MinHeap::DeleteMin()
{
    int length = _vector.size();

    if(length == 0)
    {
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    BubbleDown(0);
}

void MinHeap::DeleteFrom(int n)
{
    int i = 0;
    while(i <  _vector.size()){
        if( _vector[i].peso > n ){
            _vector[i]= _vector[_vector.size()-1];
            _vector.pop_back();
            BubbleDown(i);
        }
        i++;
    }
}

bool MinHeap::isEmpty()
{
   return _vector.empty();
   
}