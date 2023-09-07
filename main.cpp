#include "raylib.h"
#include "matriz.h"
#include "BFS.h"
#include "dstar.h"
#include <string>

#define WINDOW_WIDTH            1200
#define WINDOW_HEIGHT           800

#define RECS_WIDTH              50
#define RECS_HEIGHT             50

#define MAX_RECS_X              (int) WINDOW_WIDTH/RECS_WIDTH
#define MAX_RECS_Y              (int) WINDOW_HEIGHT/RECS_HEIGHT

#define CELESTE                 (Color){ 184, 237, 255, 100 }
#define ROJO                    (Color){ 255, 0, 0, 100 }
#define VERDE                   (Color){ 4, 255, 0, 255 }
#define AMARILLO                (Color){ 255, 234, 0, 255 }

#define AZUL                    (Color){ 0, 0, 255, 255 }
#define GRIS                    (Color){ 100, 100, 100, 255 }
#define NEGRO                    (Color){ 0, 0, 0, 255 }


#define COLOR_CONTORNO_EDIT     (Color){ 0, 0, 0, 255 }
#define COLOR_CONTORNO_NOEDIT     (Color){ 255, 255, 255, 255 }


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pathfinding");
    
    //ToggleFullscreen();// Pantalla completa
    SetExitKey(KEY_BACKSPACE); // Cerrar programa con tecla DELETE

    // Array de rectangulos
    Rectangle recs[MAX_RECS_X][MAX_RECS_Y] = {};

    // Colores de los rectangulos
    Color** colores = new Color* [MAX_RECS_X];
    for (int x = 0; x < MAX_RECS_X; x++){
        colores[x] = new Color[MAX_RECS_Y];
        for (int y = 0; y < MAX_RECS_Y; y++){
            colores[x][y] = CELESTE;
        }
    }

    // Variables de la grilla
    bool enEdicion = false;
    bool origenSeleccionado = false;
    bool destinoSeleccionado = false;
    int origen;
    int destino;
    int recX;
    int recY;

    // Variables del bfs
    bool dibujandoBFS = false;
    Queue* bfs = new Queue();
    Queue* caminobfs = new Queue();

    // Variables del dstar
    bool dibujandoDSTAR = false;
    bool dibujandoCAMINODSTAR = false;
    Dstar* d;
    PriorityQueue* dstar = new PriorityQueue();
    PriorityQueue* caminodstar = new PriorityQueue();
    int Xc;

    // Variable para dibujar progresivamente
    int contador = 0;

    // Color del contorno
    Color colorContorno = COLOR_CONTORNO_NOEDIT;

    // Parametros de cada rectangulo
    for (int x = 0; x < MAX_RECS_X; x++){
        for (int y = 0; y < MAX_RECS_Y; y++){
            recs[x][y].x = RECS_WIDTH*x;
            recs[x][y].y = RECS_HEIGHT*y;
            recs[x][y].width = RECS_WIDTH;
            recs[x][y].height = RECS_HEIGHT;
        }
    }

    // Numeros de casillas
    char*** numeros = new char**[MAX_RECS_X];
    for (int x = 0; x < MAX_RECS_X; x++){
        numeros[x] = new char*[MAX_RECS_Y];
        for (int y = 0; y < MAX_RECS_Y; y++){
            int numero = x+y*MAX_RECS_X;
            char* numero_str = new char[5 + sizeof(char)];
            std::sprintf(numero_str,"%d", numero);
            numeros[x][y] = numero_str;
        }
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or DELETE key
    {
        // Update variables
        //----------------------------------------------------------------------------------

        // Intercalar: modo edicion o modo no edicion
        if(IsKeyPressed(KEY_LEFT_CONTROL))
        {
            enEdicion = !enEdicion;
            colorContorno = enEdicion ? COLOR_CONTORNO_EDIT : COLOR_CONTORNO_NOEDIT;
        }
        // Modo edicion
        if(enEdicion)
        {
            // Click izquierdo: Bloquear
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                recX = GetMouseX() / RECS_WIDTH;
                recY = GetMouseY() / RECS_HEIGHT;
                if(recX < MAX_RECS_X && recY < MAX_RECS_Y){ // Click en rectangulo valido
                    if(sonColoresIguales(colores[recX][recY], VERDE)) origenSeleccionado = false;
                    if(sonColoresIguales(colores[recX][recY], AMARILLO)) destinoSeleccionado = false;
                    colores[recX][recY] = dibujandoCAMINODSTAR ? NEGRO : ROJO; // Pintar de Rojo o Negro(si son nuevos obstaculos para el dstar)
                }
            }

            // Click derecho: Desbloquear
            else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                recX = GetMouseX() / RECS_WIDTH;
                recY = GetMouseY() / RECS_HEIGHT;
                if(recX < MAX_RECS_X && recY < MAX_RECS_Y){ // Click en rectangulo valido
                    if(sonColoresIguales(colores[recX][recY], VERDE)) origenSeleccionado = false;
                    if(sonColoresIguales(colores[recX][recY], AMARILLO)) destinoSeleccionado = false;
                    colores[recX][recY] = CELESTE; // Pintar de celeste
                }
            }
        }

        // Modo no edicion
        else
        {
            // Click izquierdo
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // Marcar origen
                if(origenSeleccionado == false)
                {
                    recX = GetMouseX() / RECS_WIDTH;
                    recY = GetMouseY() / RECS_HEIGHT;
                    if(recX < MAX_RECS_X && recY < MAX_RECS_Y){ // Click en rectangulo valido
                        colores[recX][recY] = VERDE; // Pintar de verde
                        origenSeleccionado = true;
                        origen = recX + recY * MAX_RECS_X; // Guardar el origen
                    }
                }
                // Marcar destino
                else if(origenSeleccionado == true && destinoSeleccionado == false)
                {
                    recX = GetMouseX() / RECS_WIDTH;
                    recY = GetMouseY() / RECS_HEIGHT;
                    if(recX < MAX_RECS_X && recY < MAX_RECS_Y){ // Click en rectangulo valido
                        if(sonColoresIguales(colores[recX][recY], VERDE)) origenSeleccionado = false;
                        colores[recX][recY] = AMARILLO; // Pintar de amarillo
                        destinoSeleccionado = true;
                        destino = recX + recY * MAX_RECS_X; // Guardar el destino
                    }            
                }
            }

            // Enter: ALGORITMO 1
            else if(IsKeyPressed(KEY_ENTER) && origenSeleccionado && destinoSeleccionado)
            {
                // Seguir algoritmo 1 y pintar de AZUL
                int aux_x = MAX_RECS_X; int aux_y = MAX_RECS_Y; Color color_aux = ROJO;
                int** matriz = matrizDeAdyacenciaParcial(colores, aux_x, aux_y, color_aux);
                int length = MAX_RECS_X * MAX_RECS_Y;
                
                std::cout << "Origen: " << origen << " - Destino: " << destino << std::endl;
                Graph* grafo = new Graph(matriz, length, origen, destino);

                // Obtener todos los nodos recorridos
                bfs = grafo->bfs();
                bfs->printQueue();

                // Obtener el camino de nodos desde el origen hasta el destino
                caminobfs = grafo->caminobfs(bfs);
                caminobfs->printQueue();

                bfs->pop(); if(grafo->encontrado) bfs->popUltimo(); // No cambie de color al origen ni al destino
                caminobfs->pop(); caminobfs->popUltimo(); // No cambie de color al origen ni al destino

                dibujandoBFS = true; // Comenzar a dibujar

            }

            // Space: ALGORITMO 2
            else if(IsKeyPressed(KEY_SPACE) && origenSeleccionado && destinoSeleccionado)
            {
                // Seguir algoritmo 2 y pintar de AZUL

                // Si es la primera vez que se usa el algoritmo
                if(dibujandoCAMINODSTAR == false)
                {
                    double** matrizPonderada = matrizDeAdyacenciaPonderada(colores, MAX_RECS_X, MAX_RECS_Y);
                    int** matrizCompleta = matrizDeAdyacenciaCompleta(colores, MAX_RECS_X, MAX_RECS_Y);
                    int length = MAX_RECS_X * MAX_RECS_Y;

                    // Crear un dstar de la grilla sin obstaculos
                    d = new Dstar();
                    dstar = d->dstar(matrizCompleta, matrizPonderada, origen, destino, length);
                    
                    // Agregar los obstaculos iniciales
                    std::cout << "Agregando obstaculos" << std::endl;
                    for(int y = 0; y < MAX_RECS_Y; y++)
                        for(int x = 0; x < MAX_RECS_X; x++)
                            if(sonColoresIguales(colores[x][y], ROJO))
                                d->Add_obstacle(x + y*MAX_RECS_X);
                    
                    // Obstener el camino optimo del mapa con obstaculos iniciales
                    Xc = origen;
                    dstar = d->Replan(Xc, destino);
                    dstar->pop(); dstar->popUltimo(); // No cambie de color al origen ni al destino

                    // Copiar el mismo camino para imprimir uno de gris y otro de azul
                    caminodstar = dstar->Copiar();
                    dstar->Imprimir(d->k);

                    dibujandoDSTAR = true; // Empezar a dibujar
                }

                // Si ya se habia ejecutado el algoritmo, pero se ha agregado nuevos obstaculos y se desea replantear el camino
                else
                {
                    // Limpiar los grises anteriores
                    for(int y = 0; y < MAX_RECS_Y; y++)
                        for(int x = 0; x < MAX_RECS_X; x++)
                            if(sonColoresIguales(colores[x][y], GRIS))
                                colores[x][y] = CELESTE;
                    // Agregar los nuevos obstaculos en negro, pero posteriormente se vuelven rojos
                    for(int y = 0; y < MAX_RECS_Y; y++)
                        for(int x = 0; x < MAX_RECS_X; x++)
                            if(sonColoresIguales(colores[x][y], NEGRO))
                            {
                                d->Add_obstacle(x + y*MAX_RECS_X);
                                colores[x][y] = ROJO;
                            }
                    // Volver a calcular el camino optimo
                    dstar = d->Replan(Xc, destino);
                    dstar->pop(); dstar->popUltimo(); // No cambie de color al Xc ni al destino
                    // Copiar para pintar uno de gris y otro de azul
                    caminodstar = dstar->Copiar();
                    dstar->Imprimir(d->k);
                    dibujandoDSTAR = true; // Empezar a dibujar
                }

            }
        }

        // Escape: Resetear
        if(IsKeyPressed(KEY_ESCAPE))
        {
            // Pintar toda la grilla de celeste
            for (int x = 0; x < MAX_RECS_X; x++){
                for (int y = 0; y < MAX_RECS_Y; y++){
                    colores[x][y] = CELESTE;
                }
            }
            // Resetear todas las variables a su estado inicial
            origenSeleccionado = false;
            destinoSeleccionado = false;
            dibujandoBFS = false;
            dibujandoDSTAR = false;
            dibujandoCAMINODSTAR = false;

        }
        
        // Cuando se activa el bfs, dibujar los azules generados por el bfs
        if(dibujandoBFS)
        {
            contador++;   
            if(contador % 1 == 0) // Cada 1fps, dibuja un camino
            {
                // Imprimir bfs
                if(!bfs->isEmpty) 
                {
                    int paso = bfs->pop()->value; // Sacar el nodo a imprimir
                    int aux = MAX_RECS_X; // Por alguna razon, para hallar x,y no deja hacer paso%MAX_RECS_X por que arroja cero. Usando un auxiliar sí arroja el resultado correcto
                    int x = paso % aux; // Pasar de numero de nodo a coordenadas x,y
                    int y = paso / aux;
                    colores[x][y] = GRIS; // Pintar de azul el nodo
                }
                // Imprimir caminobfs
                else if(!caminobfs->isEmpty)
                {
                    int paso = caminobfs->pop()->value; // Sacar el nodo
                    int aux = MAX_RECS_X;
                    int x = paso % aux; // Convertir nodo a x,y
                    int y = paso / aux;
                    colores[x][y] = AZUL; // Pintar de azul
                }
                // Cuando ya termine de imprimir, volver al estado normal
                else
                {
                    contador = 0;
                    dibujandoBFS = false;
                }
            }
        }

        // Cuando se activa el D star, dibujar los grises generados por el dstar
        if(dibujandoDSTAR)
        {
            contador++;   
            if(contador % 1 == 0) // Cada 1fps, dibuja un camino
            {
                // Imprimir bfs
                if(!dstar->isEmpty())
                {
                    int paso = dstar->pop(); // Sacar el nodo a imprimir
                    int aux = MAX_RECS_X;
                    int x = paso % aux; // Pasar de numero de nodo a coordenadas x,y
                    int y = paso / aux;
                    colores[x][y] = GRIS; // Pintar de gris el nodo
                }
                // Cuando ya termine de imprimir, permitir dar pasos en azul
                else
                {
                    contador = 0;
                    dibujandoDSTAR = false;
                    dibujandoCAMINODSTAR = true;
                }
            }
        }

        // Cuando se activa el D star, dibujar los azules generados por el dstar
        if(dibujandoCAMINODSTAR)
        {
            // Imprimir bfs
            if(!caminodstar->isEmpty())
            {
                if(IsKeyPressed(KEY_LEFT_ALT))
                {
                    int paso = caminodstar->pop(); // Sacar el nodo a imprimir
                    int aux = MAX_RECS_X; 
                    int x = paso % aux; // Pasar de numero de nodo a coordenadas x,y
                    int y = paso / aux;
                    colores[x][y] = AZUL; // Pintar de azul el nodo
                    Xc = paso; // Guardar la casilla actual en la que se ha dado el paso
                }
            }
            // Cuando ya termine de imprimir, volver al estado normal
            else
            {
                contador = 0;
                dibujandoCAMINODSTAR = false;
            }
            
        }
        
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        for (int x = 0; x < MAX_RECS_X; x++){
            for (int y = 0; y < MAX_RECS_Y; y++){
                DrawRectangleRec(recs[x][y], colores[x][y]); // Rectangulos
                DrawRectangleLinesEx(recs[x][y], 2.0f, colorContorno);  // Contornos
                DrawText(numeros[x][y], (int)(recs[x][y].x + recs[x][y].width/2), (int)(recs[x][y].y + recs[x][y].height/2), 13, BLACK); // Numeros de casillas
            }
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

