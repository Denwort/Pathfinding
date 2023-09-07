#include "BFS.h"

Node::Node(int value)
{
    this->value = value;
    this->next = nullptr;
    this->prev = nullptr;
    this->prev_value = -1; // David
}

Queue::Queue()
{
    this->last = nullptr;
    this->isEmpty = true;
}

void Queue::push(int value, int prev_value)
{
    Node* newNode = new Node(value);
    newNode->prev_value = prev_value;

    newNode->next = last;
    last = newNode;
    isEmpty = false;

}

Node* Queue::pop()
{
    // Caso: 0 nodos
    if (this->isEmpty) return nullptr;

    Node* pivot = this->last;

    // Caso: solo 1 nodo
    if (pivot->next == nullptr)
    {
        last = nullptr;
        isEmpty = true;
        return pivot;
    }

    // Caso: mas de 1 nodo
    while(pivot->next->next != nullptr)
    {
        pivot = pivot->next;
    }
    Node* ret = pivot->next;
    pivot->next = nullptr;
    return ret;
}

Node* Queue::popUltimo()
{   
    if(last == nullptr) return nullptr;
    Node* aux = last;
    last = last->next;
    aux->next = nullptr;
    return aux;
}

void Queue::printQueue()
{
    if (this->isEmpty) return;
    else
    {
        Node* pivot = this->last;

        while (pivot != nullptr)
        {
            cout << pivot->value << "(" << pivot->prev_value << ") ";
            pivot = pivot->next;
        }

        cout << endl;
    }
}

Graph::Graph(int** matrix, int length, int start, int finish)
{
    this->matrix = matrix;
    this->length = length;
    this->start = start;
    this->finish = finish;
    encontrado = false;
}

Queue* Graph::bfs()
{
    bool visited[this->length] = {0};
    Queue* cola = new Queue();
    Queue* nodes = new Queue();

    visited[this->start] = true;
    cola->push(this->start, -1);
    nodes->push(this->start, -1);

    while(!cola->isEmpty)
    {
        int node = cola->pop()->value;
        
        for (int i = 0; i < length; i++)
        {
            if (!visited[i] && this->matrix[node][i] == 1)
            {
                visited[i] = true;
                cola->push(i, node);
                nodes->push(i, node);
                if (i == this->finish) {encontrado = true; return nodes;}
            }
        }
    }

    return nodes;
}

// David
Queue* Graph::caminobfs(Queue* nodes)
{
    if(!encontrado) {
        std::cout<< "Camino no posible";
        return new Queue();
    }

    Queue* res = new Queue();
    Node* pivot = nodes->last;
    int parent = pivot->prev_value;
    res->push(pivot->value, -1);

    while(pivot->value != this->start)
    {
        pivot = pivot->next;

        if(pivot->value == parent)
        {
            res->push(pivot->value,-1);
            parent = pivot->prev_value;
        }
    }

    return res;

}

