#pragma once
#include <iostream>
#include<cmath>

class Punto
{
public:
    int id;
    Punto* siguiente;
    Punto(int id);
};

class PriorityQueue
{
public:
    Punto* top;
    PriorityQueue();
    void push(int n, double* k);
    void push(int n); // Para tratar a la PriorityQueue como una Queue normal
    int pop();
    int popUltimo();
    bool isEmpty();
    void Imprimir(double* k);
    PriorityQueue* Copiar();
};

class Dstar
{
public:

    // Constantes
    int NNODOS; // Cantidad de nodos. Es obtenido de la grilla.
    double INF = INFINITY; // Infinito en forma de int
    int DEFAULT_H = -1; // Valor por defecto del herustic de los nodos New
    int DEFAULT_K = -1; // Valor por defecto del key de los nodos New
    int DEFAULT_B = -1; // Valor por defecto del backpointer de los nodos New

    // Variables de nodo
    int* b; // backpointer. puntero al siguiente nodo que sigue hacia el mejor camino.
    char* t; // 'N' = New, 'O' = Open, 'C' = Closed
    double* h; // heuristic. costo hasta el destino
    double* k; // key. valor minimo desde que X fue puesto en la cola. se usa para priorizar la cola.

    // Variables entre nodos
    int** L; // Mapa. Grilla de que nodo con que nodo estan conectados. Incluyen obstaculos.
    double** c; // cost. costo desde un }nodo X hacia Y. 100 para vertical/horizontal. 141 para diagonal. INFINITO para obstaculos
    // no r(X,Y)

    // Variables de posicion
    int G; // Goal : Destino
    int S; // Source : Origen
    int Xc; // Nodo en el que actualmente está el robot.

    PriorityQueue* O; // Cola de prioridad ordenada con k.

    Dstar(){};
    double min(double a, double b);
    void ImprimirInfoNodos();

    PriorityQueue* dstar(int** matriz, double** costos, int origen, int destino, int nnodos);
    PriorityQueue* Init_plan(PriorityQueue* O, int** L, int Xc, int G);

    double Process_state(PriorityQueue* O, int** L);
    void Insert(PriorityQueue* O, int X, double hnew);    

    PriorityQueue* Get_backpointer_list(int** L, int Xc, int G);

    void Add_obstacle(int X);
    void Modify_cost(PriorityQueue* O, int X, int Y, double cval);
    PriorityQueue* Replan(int Xc, int G);
};


