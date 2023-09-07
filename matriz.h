#pragma once
#include "include/raylib.h"
#include <iostream>


bool sonColoresIguales(Color a, Color b);

void imprimirMatriz(int** matriz, int max_recs_x, int max_recs_y);

int** matrizDeAdyacenciaParcial(Color** colores, int max_recs_x, int max_recs_y, Color rojo);

int** matrizDeAdyacenciaCompleta(Color** colores, int max_recs_x, int max_recs_y);

double** matrizDeAdyacenciaPonderada(Color** colores, int max_recs_x, int max_recs_y);