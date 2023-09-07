#include "dstar.h"

Punto::Punto(int id)
{
    this->id = id;
    this->siguiente = nullptr;
}

PriorityQueue::PriorityQueue()
{
    top = nullptr;
}
void PriorityQueue::push(int n, double* k)
{
    Punto* nuevo = new Punto(n);
    // Caso 1: cola vacia
    if(top == nullptr)
    {
        top = nuevo;
    }
    // Caso 2: agregar al final de la cola
    else if( k[nuevo->id] >= k[top->id] )
    {
        nuevo->siguiente = top;
        top = nuevo;
    }
    // Caso 3: agregar en cualquier lado, incluso al inicio de la cola
    else
    {
        Punto* anterior = nullptr;
        Punto* pivot = top;
        while( pivot != nullptr && k[nuevo->id] < k[pivot->id] )
        {
            anterior = pivot;
            pivot = pivot->siguiente;
        }
        
        anterior->siguiente = nuevo;
        nuevo->siguiente = pivot;
    }
}
int PriorityQueue::pop()
{
    // Caso 1: cola vacia
    if(top == nullptr) return -1;
    // Caso 2: solo 1 elemento en la cola
    if(top->siguiente == nullptr)
    {
        int retorno = top->id;
        top = nullptr;
        return retorno;
    }
    // Caso 3: 2 o más elementos en la cola
    Punto* pivot = top;
    while(pivot->siguiente->siguiente != nullptr)
    {
        pivot = pivot->siguiente;
    }
    int retorno = pivot->siguiente->id;
    pivot->siguiente = nullptr;
    return retorno;
}
int PriorityQueue::popUltimo()
{   
    if(top == nullptr) return -1;
    Punto* aux = top;
    top = top->siguiente;
    aux->siguiente = nullptr;
    return aux->id;
}
void PriorityQueue::push(int n)
{
    Punto* nuevo = new Punto(n);
    nuevo->siguiente = top;
    top = nuevo;
}
bool PriorityQueue::isEmpty()
{
    return top == nullptr;
}
void PriorityQueue::Imprimir(double* k)
{
    Punto* pivot = top;
    while(pivot != nullptr)
    {
        std::cout << pivot->id << "(" << k[pivot->id] << ")" << " ";
        pivot = pivot->siguiente;
    }
    std::cout << std::endl;
}
PriorityQueue* PriorityQueue::Copiar()
{
    PriorityQueue* retorno = new PriorityQueue();
    PriorityQueue* aux = new PriorityQueue();
    while(!this->isEmpty())
    {
        aux->push(this->pop());
    }
    while(!aux->isEmpty())
    {
        int n = aux->pop();
        this->push(n);
        retorno->push(n);
    }
    return retorno;
}

double Dstar::min(double a, double b)
{
    if(a < b) return a;
    else return b;
}
void Dstar::ImprimirInfoNodos()
{
    std::cout << std::endl;
    for(int i = 0; i < NNODOS; i++)
    {
        std::cout << i << " ";
        std::cout << "t=" << t[i] << " ";
        std::cout << "h=" << h[i] << " ";
        std::cout << "k=" << k[i] << " ";
        std::cout << "b=" << b[i] << " ";
        std::cout << std::endl;
    }
}

PriorityQueue* Dstar::dstar(int** matriz, double** costos, int origen, int destino, int nnodos)
{

    S = origen;
    G = destino;
    NNODOS = nnodos;

    O = new PriorityQueue();
    b = new int[NNODOS]; for(int i = 0; i < NNODOS; i++) b[i] = DEFAULT_B; // Inician con DEFAULT_B
    c = costos; // Inician con costos 100 y 141 base. sin obstaculos, según las costos pasada
    // no r(X,Y)
    t = new char[NNODOS]; for(int i = 0; i < NNODOS; i++) t[i] = 'N'; // Inician con OPEN
    h = new double[NNODOS]; for(int i = 0; i < NNODOS; i++) h[i] = DEFAULT_H; // Inician con DEFAULT_H
    k = new double[NNODOS]; for(int i = 0; i < NNODOS; i++) k[i] = DEFAULT_K; // Inician con DEFAULT_K
    L = matriz; // Inician con 0 y 1 para saber si son vecinos.
    // O : cola
    // L : mapa
    // Xc : origen
    // G : destino



    // Algoritmo sedun pagina 31
    h[G] = 0;
    Insert(O, G, h[G]);
    Xc = S;
    PriorityQueue* P = Init_plan(O, L, Xc, G);
    return P; 
    // En principio que no sea dinamico, sino que solo devuelva el camino
    // Codigo para que sea dinamico
}

PriorityQueue* Dstar::Init_plan(PriorityQueue* O, int** L, int Xc, int G)
{
    int kmin;
    do{
        kmin = Process_state(O, L);
    } while((kmin != -1) && (t[Xc] != 'C')); // MODIFICADO: Ahora evalua todo. Para que evalue optimamente agregar: && (t[Xc] != 'C')
    PriorityQueue* P = Get_backpointer_list(L, Xc, G); // retorna la lista del camino solucion, pero eso lo implementaremos al final.
    return P;
}


double Dstar::Process_state(PriorityQueue* O, int** L)
{

    int X = O->pop();
    t[X] = 'C';

    if(X == -1) return -1; // Si ya no hay mas elementos en la cola.

    double kold = k[X]; 
    // Raise
    if(kold < h[X])
    {
        // std::cout << "Raise: " << X << std::endl;
        for(int Y = 0; Y < NNODOS; Y++) if(L[X][Y] == 1) // Para cada vecino Y de X
        {
            if(t[Y] != 'N' && h[Y] <= kold && h[X] > h[Y] + c[X][Y])
            {
                // std::cout << X << "->" << Y << std::endl;
                b[X] = Y;
                h[X] = h[Y] + c[Y][X];
            }
        }
        
    }

    // Lower
    if(kold == h[X])
    {
        // std::cout << "Lower: " << X << std::endl;
        for(int Y = 0; Y < NNODOS; Y++) if(L[X][Y] == 1) // Para cada vecino Y de X
        {
            if(
                (t[Y] == 'N') ||
                (b[Y] == X && h[Y] != h[X] + c[X][Y]) ||
                (b[Y] != X && h[Y] > h[X] + c[X][Y])
            )
            {
                // std::cout << Y << "->" << X << std::endl;
                b[Y] = X;
                Insert(O, Y, h[X] + c[X][Y]);
            }
        }
        
    }
    else
    {
        // std::cout << "Otro: " << X << std::endl;
        for(int Y = 0; Y < NNODOS; Y++) if(L[X][Y] == 1) // Para cada vecino Y de X
        {
            if( (t[Y] == 'N') || (b[Y] == X && h[Y] != h[X] + c[X][Y]) )
            {
                // std::cout << Y << "->" << X << std::endl;
                b[Y] = X;
                Insert(O, Y, h[X] + c[X][Y]);
            }
            else if (b[Y] != X && h[Y] > h[X] + c[X][Y])
            {
                Insert(O, X, h[X]);
            }
            else if( b[Y] != X && h[X] > h[Y] + c[X][Y] && t[Y] == 'C' && h[Y] > kold)
            {
                Insert(O, Y, h[Y]);
            }
        }
    }

    return k[X];
} 

void Dstar::Insert(PriorityQueue* O, int X, double hnew)
{
    if(t[X] == 'N'){
        k[X] = hnew;
    }
    else if(t[X] == 'O'){
        k[X] = min(k[X], hnew);
    }
    else if(t[X] == 'C'){
        k[X] = min(h[X], hnew);
    }
    h[X] = hnew;
    t[X] = 'O';
    O->push(X, k);
}


PriorityQueue* Dstar::Get_backpointer_list(int** L, int Xc, int G)
{
    std::cout << "DSTAR TERMINADO. se intenta obtener la lista camino" << std::endl;

    if(b[Xc] == DEFAULT_B || h[Xc] == INFINITY) 
    {
        std::cout << "No camino posible"; // Si Xc no tiene backpointer o si su costo es infinito(al recalcular, esta rodeado de obstaculos con costo infinito), no tiene camino posible.
        return new PriorityQueue(); // Devolver cola vacia
    }

    // Seguir los backpointers hasta el final
    int pivot = Xc;
    PriorityQueue* P = new PriorityQueue();
    while(pivot != G)
    {
        std::cout << pivot << "(" << k[pivot] << ")";
        P->push(pivot);
        std::cout <<  " -> " ;
        pivot = b[pivot];
        std::cout << pivot << "(" << k[pivot] << ")" << std::endl;
    }
    P->push(G);
    return P;
}


void Dstar::Add_obstacle(int X) // Prepare_Repairs
{
    std::cout<< "Obstaculo: " << X << " agrega a: " ;
    for(int Y = 0; Y < NNODOS; Y++) if(L[X][Y] == 1) // Para cada vecino Y de X
    {
        Modify_cost(O, X, Y, INF);
        Modify_cost(O, Y, X, INF);
        h[X] = INF;
    }
    std::cout << std::endl;
}

void Dstar::Modify_cost(PriorityQueue* O, int X, int Y, double cval)
{
    c[X][Y] = cval;
    if(t[X] == 'C')
    {
        std::cout << X << "  ";
        Insert(O, X, h[X]);
    }
    // return ¿Get_Kmin(O)?
}

PriorityQueue* Dstar::Replan(int Xc, int G)
{    
    double kmin;
    do
    {
        kmin = Process_state(O, L);
    } while ( (kmin != -1) ); // MODIFICADO: Ahora evalua todo. Para que evalue optimamente agregar: && kmin < h[Xc]

    PriorityQueue* P = Get_backpointer_list(L, Xc, G);
    return P;
}
