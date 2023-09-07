#pragma once
#include <iostream>


using namespace std;

class Node
{
    public:
        int value;
        Node* next;
        Node* prev;
        int prev_value; 

        Node(int value);
};

class Queue
{
    public:
        Node* last;
        bool isEmpty;

        Queue();
        ~Queue();
        void push(int value, int prev_value);
        Node* pop();
        Node* popUltimo();
        void printQueue();
};

class Graph
{
    public:
        int** matrix;
        int length;
        int start;
        int finish;
        bool encontrado;

        Graph(int** matrix, int length, int start, int finish);
        Queue* bfs();
        Queue* caminobfs(Queue* nodes); 
};

//int* bfs2(int matrix[][5]);