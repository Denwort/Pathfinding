#include "matriz.h"

bool sonColoresIguales(Color a, Color b)
{
    return a.r==b.r && a.g==b.g && a.b==b.b && a.a==b.a;
}

void imprimirMatriz(int** matriz, int max_recs_x, int max_recs_y)
{
    int nNodos = max_recs_x*max_recs_y;
    for(int origen = 0; origen < nNodos; origen++){
        for(int destino = 0; destino < nNodos; destino++){
            std::cout << matriz[origen][destino] << " ";
        }
        std::cout << std::endl;
    }
    // Plotter: https://graphonline.ru/en/
}

int** matrizDeAdyacenciaParcial(Color** colores, int max_recs_x, int max_recs_y, Color rojo)
{
    int nNodos = max_recs_x*max_recs_y;
    // Inicializacion
    int** matriz = new int* [nNodos];
    for(int i = 0; i < nNodos; i++){
        matriz[i] = new int[nNodos];
        for(int j = 0; j < nNodos; j++){
            matriz[i][j] = 0;

            
        }
    }

    

    // Matriz de adyacencia
    for(int y = 0; y < max_recs_y; y++)
    {
        for(int x = 0; x < max_recs_x; x++)
        {
            // Si es un nodo valido
            if(!sonColoresIguales(colores[x][y],rojo))
            {
                // Conectar al nodo actual con sus alrededores
                int nodoActual = (x)+(y)*max_recs_x; // Formula para hallar el numero de nodo, teniendo su (x,y)
                //std::cout << nodoActual << std::endl;

                // Nodo izquierda
                if(x-1 >= 0 && !sonColoresIguales(colores[x-1][y],rojo)){
                    matriz[nodoActual][(x-1)+(y)*max_recs_x] = 1;
                }
                // Nodo derecha
                if(x+1 < max_recs_x && !sonColoresIguales(colores[x+1][y],rojo)){
                    matriz[nodoActual][(x+1)+(y)*max_recs_x] = 1;
                }
                // Nodo arriba
                if(y-1 >= 0 && !sonColoresIguales(colores[x][y-1],rojo)){
                    matriz[nodoActual][(x)+(y-1)*max_recs_x] = 1;
                }
                // Nodo abajo
                if(y+1 < max_recs_y && !sonColoresIguales(colores[x][y+1],rojo)){
                    matriz[nodoActual][(x)+(y+1)*max_recs_x] = 1;
                }

                // Nodo izquierda arriba 
                if(x-1 >= 0 && y-1 >= 0 &&!sonColoresIguales(colores[x-1][y-1],rojo)){
                    matriz[nodoActual][(x-1)+(y-1)*max_recs_x] = 1; // o 1.4
                }
                // Nodo izquierda abajo 
                if(x-1 >= 0 && y+1 < max_recs_y &&!sonColoresIguales(colores[x-1][y+1],rojo)){
                    matriz[nodoActual][(x-1)+(y+1)*max_recs_x] = 1; // o 1.4
                }
                // Nodo derecha arriba 
                if(x+1 < max_recs_x && y-1 >= 0 &&!sonColoresIguales(colores[x+1][y-1],rojo)){
                    matriz[nodoActual][(x+1)+(y-1)*max_recs_x] = 1; // o 1.4
                }
                // Nodo derecha abajo 
                if(x+1 < max_recs_x && y+1 < max_recs_y &&!sonColoresIguales(colores[x+1][y+1],rojo)){
                    matriz[nodoActual][(x+1)+(y+1)*max_recs_x] = 1; // o 1.4
                }

            }

        }
    }

    return matriz;
}

int** matrizDeAdyacenciaCompleta(Color** colores, int max_recs_x, int max_recs_y)
{
    int nNodos = max_recs_x*max_recs_y;
    // Inicializacion
    int** matriz = new int* [nNodos];
    for(int i = 0; i < nNodos; i++){
        matriz[i] = new int[nNodos];
        for(int j = 0; j < nNodos; j++){
            matriz[i][j] = 0;

            
        }
    }

    // Matriz de adyacencia
    for(int y = 0; y < max_recs_y; y++)
    {
        for(int x = 0; x < max_recs_x; x++)
        {
            // Conectar al nodo actual con sus alrededores
            int nodoActual = (x)+(y)*max_recs_x; // Formula para hallar el numero de nodo, teniendo su (x,y)
            //std::cout << nodoActual << std::endl;

            // Nodo izquierda
            if(x-1 >= 0){
                matriz[nodoActual][(x-1)+(y)*max_recs_x] = 1;
            }
            // Nodo derecha
            if(x+1 < max_recs_x){
                matriz[nodoActual][(x+1)+(y)*max_recs_x] = 1;
            }
            // Nodo arriba
            if(y-1 >= 0){
                matriz[nodoActual][(x)+(y-1)*max_recs_x] = 1;
            }
            // Nodo abajo
            if(y+1 < max_recs_y){
                matriz[nodoActual][(x)+(y+1)*max_recs_x] = 1;
            }

            // Nodo izquierda arriba 
            if(x-1 >= 0 && y-1 >= 0){
                matriz[nodoActual][(x-1)+(y-1)*max_recs_x] = 1;
            }
            // Nodo izquierda abajo 
            if(x-1 >= 0 && y+1 < max_recs_y){
                matriz[nodoActual][(x-1)+(y+1)*max_recs_x] = 1;
            }
            // Nodo derecha arriba 
            if(x+1 < max_recs_x && y-1 >= 0){
                matriz[nodoActual][(x+1)+(y-1)*max_recs_x] = 1;
            }
            // Nodo derecha abajo 
            if(x+1 < max_recs_x && y+1 < max_recs_y){
                matriz[nodoActual][(x+1)+(y+1)*max_recs_x] = 1;
            }


        }
    }

    return matriz;
}


double** matrizDeAdyacenciaPonderada(Color** colores, int max_recs_x, int max_recs_y)
{
    int nNodos = max_recs_x*max_recs_y;
    // Inicializacion
    double** matriz = new double* [nNodos];
    for(int i = 0; i < nNodos; i++){
        matriz[i] = new double[nNodos];
        for(int j = 0; j < nNodos; j++){
            matriz[i][j] = 0;

            
        }
    }

    // Matriz de adyacencia
    for(int y = 0; y < max_recs_y; y++)
    {
        for(int x = 0; x < max_recs_x; x++)
        {
            // Conectar al nodo actual con sus alrededores
            int nodoActual = (x)+(y)*max_recs_x; // Formula para hallar el numero de nodo, teniendo su (x,y)
            //std::cout << nodoActual << std::endl;

            // Nodo izquierda
            if(x-1 >= 0){
                matriz[nodoActual][(x-1)+(y)*max_recs_x] = 100;
            }
            // Nodo derecha
            if(x+1 < max_recs_x){
                matriz[nodoActual][(x+1)+(y)*max_recs_x] = 100;
            }
            // Nodo arriba
            if(y-1 >= 0){
                matriz[nodoActual][(x)+(y-1)*max_recs_x] = 100;
            }
            // Nodo abajo
            if(y+1 < max_recs_y){
                matriz[nodoActual][(x)+(y+1)*max_recs_x] = 100;
            }

            // Nodo izquierda arriba 
            if(x-1 >= 0 && y-1 >= 0){
                matriz[nodoActual][(x-1)+(y-1)*max_recs_x] = 141;
            }
            // Nodo izquierda abajo 
            if(x-1 >= 0 && y+1 < max_recs_y){
                matriz[nodoActual][(x-1)+(y+1)*max_recs_x] = 141;
            }
            // Nodo derecha arriba 
            if(x+1 < max_recs_x && y-1 >= 0){
                matriz[nodoActual][(x+1)+(y-1)*max_recs_x] = 141;
            }
            // Nodo derecha abajo 
            if(x+1 < max_recs_x && y+1 < max_recs_y){
                matriz[nodoActual][(x+1)+(y+1)*max_recs_x] = 141;
            }


        }
    }

    return matriz;
}

