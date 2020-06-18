#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "funcionamento_jogo.h"

FILE *ponteiro;

#define EPSILON 0.0001

/*typedef struct coordenada {
    double x, y;
} Coordenada;
typedef struct forca_r {
    double forca_x, forca_y;
} Forca_r;
typedef struct velocidade {
    double vel_x;
    double vel_y;
} Velocidade;
typedef struct aceleracao {
    double acel_x, acel_y;
} Aceleracao;
typedef struct planeta {
    double massa, raio;
} Planeta;
typedef struct nave {
    char* nome;
    double massa;
    Coordenada coordenada_nave;
    Velocidade velocidade_nave;
} Nave;
typedef struct projetil {
    double massa;
    Coordenada coordenada_projetil;
    Velocidade velocidade_projetil;
    int tempo;
} Projetil;*/

double distancia(Coordenada ponto_1, Coordenada ponto_2) {
    return sqrt(pow(ponto_2.x - ponto_1.x, 2) + pow(ponto_2.y - ponto_1.y, 2));
}

//Força resultante sobre corpo_1
Forca_r forca_resultante(Coordenada corpo_1, Coordenada corpo_2, double massa_1, double massa_2) {
    double distance = distancia(corpo_1, corpo_2);
    Forca_r forca;
    forca.forca_x = (6.67*pow(10, -11)*massa_1*massa_2)*(corpo_2.x - corpo_1.x)/distance;
    forca.forca_x = forca.forca_x/distance;
    forca.forca_x = forca.forca_x/distance;
    forca.forca_y = (6.67*pow(10, -11)*massa_1*massa_2)*(corpo_2.y - corpo_1.y)/distance;
    forca.forca_y = forca.forca_y/distance;
    forca.forca_y = forca.forca_y/distance;
    return forca;

}

Aceleracao aceleracao(Forca_r forca, double massa) {
    Aceleracao aceleracao_r;
    aceleracao_r.acel_x = (forca.forca_x)/(massa);
    aceleracao_r.acel_y = (forca.forca_y)/(massa);;
    return aceleracao_r;
}

//calcula a nova coordenada de uma nave depois de passado um intervalo igual a "tempo".
void atualiza_posicao (Coordenada *ponto, Velocidade *vel, Aceleracao acel, int tempo) {
    long long int horizontal, vertical;
    tempo = 1; //modificar isso. apenas temporario.
    double tempo_aux = 0.2;
    horizontal = 80000000; // tamanho para cima e para baixo com referencial no 0,0
    vertical = 60000000;

    ponto->x += vel->vel_x*tempo + (acel.acel_x*pow(tempo_aux,2))/2;
    ponto->y += vel->vel_y*tempo + (acel.acel_y*pow(tempo_aux,2))/2;
    vel->vel_x += acel.acel_x * tempo_aux;
    vel->vel_y += acel.acel_y * tempo_aux;

    //implementa o campo toroidal, modificando o valor da coordenada caso o objeto se posicione fora do campo.
    if (ponto->x >= horizontal) 
     ponto->x = ponto->x - 80000000;
    else if(ponto->x <= 0)
     ponto->x = ponto->x + 80000000;

    if (ponto->y >= vertical) 
     ponto->y = ponto->y - 60000000;
    else if(ponto->y <= 0)
     ponto->y = ponto->y + 60000000;

}

//checa se dois objetos estão no mesmo espaço, configurando uma batida.
int checa_batida(Coordenada ponto_obj1, Coordenada ponto_obj2, double raio_obj1, double raio_obj2) {
    if (distancia(ponto_obj1, ponto_obj2) < (raio_obj1 + raio_obj2)) {
        printf("houve uma batida! O jogo acabou");
        return 1;
        //char c = getchar();
        //exit(0);
    }
    return 0;
}

int checa_batida_projeteis(Coordenada ponto_obj1, Coordenada ponto_obj2, double raio_obj1, double raio_obj2) {
    if (distancia(ponto_obj1, ponto_obj2) < (raio_obj1 + raio_obj2)) {
        //printf("\nProjeteis bateram!\n");
        return 1;
    }
    return 0;
}

void acelera_ae (Nave* nave, int frente) {
    if(frente == 0) {
        if(nave->velocidade_nave.vel_y <= 2e6) nave->velocidade_nave.vel_y += 2.5e5;
    }
    else if(frente == 1) {
        if(nave->velocidade_nave.vel_x >= -2e6) nave->velocidade_nave.vel_x -= 178571;
        if(nave->velocidade_nave.vel_y <= 2e6) nave->velocidade_nave.vel_y += 178571;
    }
    else if(frente == 2) {
        if(nave->velocidade_nave.vel_x >= -2e6) nave->velocidade_nave.vel_x -= 2.5e5;
    }
    else if(frente == 3) {
        if(nave->velocidade_nave.vel_x >= -2e6) nave->velocidade_nave.vel_x -= 178571;
        if(nave->velocidade_nave.vel_y >= -2e6) nave->velocidade_nave.vel_y -= 178571;
    }
    else if(frente == 4) {
        if(nave->velocidade_nave.vel_y >= -2e6) nave->velocidade_nave.vel_y -= 2.5e5;
    }
    else if(frente == 5) {
        if(nave->velocidade_nave.vel_x <= 2e6) nave->velocidade_nave.vel_x += 178571;
        if(nave->velocidade_nave.vel_y >= -2e6) nave->velocidade_nave.vel_y -= 178571;
    }
    else if(frente == 6) {
        if(nave->velocidade_nave.vel_x <= 2e6) nave->velocidade_nave.vel_x += 2.5e5;
    }
    else if(frente == 7) {
        if(nave->velocidade_nave.vel_x <= 2e6) nave->velocidade_nave.vel_x += 178571;
        if(nave->velocidade_nave.vel_y <= 2e6) nave->velocidade_nave.vel_y += 178571;
    }
}

void desacelera_ae(Nave* nave, int frente) {
    if(frente == 0) {
        if(nave->velocidade_nave.vel_y >= -2e6) nave->velocidade_nave.vel_y -= 2.5e5;
    }
    else if(frente == 1) {
        if(nave->velocidade_nave.vel_x <= 2e6) nave->velocidade_nave.vel_x += 178571;
        if(nave->velocidade_nave.vel_y >= -2e6) nave->velocidade_nave.vel_y -= 178571;
    }
    else if(frente == 2) {
        if(nave->velocidade_nave.vel_x <= 2e6) nave->velocidade_nave.vel_x += 2.5e5;
    }
    else if(frente == 3) {
        if(nave->velocidade_nave.vel_x <= 2e6) nave->velocidade_nave.vel_x += 178571;
        if(nave->velocidade_nave.vel_y <= 2e6) nave->velocidade_nave.vel_y += 178571;
    }
    else if(frente == 4) {
        if(nave->velocidade_nave.vel_y <= 2e6) nave->velocidade_nave.vel_y += 2.5e5;
    }
    else if(frente == 5) {
        if(nave->velocidade_nave.vel_x >= -2e6) nave->velocidade_nave.vel_x -= 178571;
        if(nave->velocidade_nave.vel_y <= 2e6) nave->velocidade_nave.vel_y += 178571;
    }
    else if(frente == 6) {
        if(nave->velocidade_nave.vel_x >= -2e6) nave->velocidade_nave.vel_x -= 2.5e5;
    }
    else if(frente == 7) {
        if(nave->velocidade_nave.vel_x >= -2e6) nave->velocidade_nave.vel_x -= 178571;
        if(nave->velocidade_nave.vel_y >= -2e6) nave->velocidade_nave.vel_y -= 178571;
    }
}

void inicia_tiro (Nave nave, Projetil** projeteis, int** indice_projeteis_destruidos, int frente, int* numero_projetil) {
    if(*numero_projetil < 104) {
        (*indice_projeteis_destruidos)[*numero_projetil] = -1;
        (*projeteis)[*numero_projetil].coordenada_projetil = nave.coordenada_nave;
        if(frente == 0) {
            (*projeteis)[*numero_projetil].coordenada_projetil.y += 2e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = 2.5e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = 0;
        }
        else if(frente == 1) {
            (*projeteis)[*numero_projetil].coordenada_projetil.y += 1.43e6;
            (*projeteis)[*numero_projetil].coordenada_projetil.x -= 1.43e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = 1.78e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = -1.78e6;
        }
        else if(frente == 2) {
            (*projeteis)[*numero_projetil].coordenada_projetil.x -= 2e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = 0;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = -2.5e6;
        }
        else if(frente == 3) {
            (*projeteis)[*numero_projetil].coordenada_projetil.y -= 1.43e6;
            (*projeteis)[*numero_projetil].coordenada_projetil.x -= 1.43e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = -1.78e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = -1.78e6;
        }
        else if(frente == 4) {
            (*projeteis)[*numero_projetil].coordenada_projetil.y -= 2e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = -2.5e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = 0;
        }
        else if(frente == 5) {
            (*projeteis)[*numero_projetil].coordenada_projetil.y -= 1.43e6;
            (*projeteis)[*numero_projetil].coordenada_projetil.x += 1.43e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = -1.78e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = 1.78e6;
        }
        else if(frente == 6) {
            (*projeteis)[*numero_projetil].coordenada_projetil.x += 2e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = 0;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = 2.5e6;
        }
        else if(frente == 7) {
            (*projeteis)[*numero_projetil].coordenada_projetil.y += 1.43e6;
            (*projeteis)[*numero_projetil].coordenada_projetil.x += 1.43e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_y = 1.78e6;
            (*projeteis)[*numero_projetil].velocidade_projetil.vel_x = 1.78e6;
        }
        *numero_projetil = *numero_projetil + 1;
    }
}

/*void inicia_tiro (Nave* nave, Projetil** projeteis, int** indice_projeteis_destruidos, int rotacao_nave, int* numero_projetil) {
    double val = 3.14159265 / 180;
    Velocidade* auxVel; 
    Velocidade velocidade_inicial = nave->velocidade_nave;
    velocidade_inicial.vel_x += 1e6;
    velocidade_inicial.vel_y += 1e6;

    for (int i = *numero_projetil; i < (*numero_projetil)+1; i++) {
        (*indice_projeteis_destruidos)[i] = -1;
        *auxVel = (*projeteis)[i].velocidade_projetil;
        if (rotacao_nave == 0){
            (*projeteis)[i].coordenada_projetil.y = nave->coordenada_nave.y - 2e6;
            auxVel->vel_y = -velocidade_inicial.vel_y;

        }
        if (rotacao_nave == 4){
            (*projeteis)[i].coordenada_projetil.y = nave->coordenada_nave.y + 2e6;
            auxVel->vel_y = velocidade_inicial.vel_y;
        }
        if (rotacao_nave == 2){
            (*projeteis)[i].coordenada_projetil.x = nave->coordenada_nave.x - 2e6;
            auxVel->vel_x = -velocidade_inicial.vel_x;
        }
        if (rotacao_nave == 6){
            (*projeteis)[i].coordenada_projetil.x = nave->coordenada_nave.x + 2e6;
            auxVel->vel_x = velocidade_inicial.vel_x;
        }

        if (rotacao_nave == 1 || rotacao_nave == 7){
            (*projeteis)[i].coordenada_projetil.y = nave->coordenada_nave.y - sin(45*val) * 2e6;
            auxVel->vel_y = - sin(45*val) * velocidade_inicial.vel_y;
        }
        if (rotacao_nave == 3 || rotacao_nave == 5){
            (*projeteis)[i].coordenada_projetil.y = nave->coordenada_nave.y + sin(45*val) * 2e6;
            auxVel->vel_y = sin(45*val) * velocidade_inicial.vel_y;
        }
        if (rotacao_nave == 1 || rotacao_nave == 3){
            (*projeteis)[i].coordenada_projetil.x = nave->coordenada_nave.x - sin(45*val) * 2e6;
            auxVel->vel_x = - sin(45*val) * velocidade_inicial.vel_x;
        }
        if (rotacao_nave == 5 || rotacao_nave == 7){
            (*projeteis)[i].coordenada_projetil.x = nave->coordenada_nave.x + sin(45*val) * 2e6;
            auxVel->vel_x = sin(45*val) * velocidade_inicial.vel_x;
        }

    }
}
*/

void normaliza_vetor(double* x, double* y) {
    double norma = sqrt(pow(*x, 2) + pow(*y, 2));
    *x = (*x)/norma;
    *y = (*y)/norma;
}



/*int main() {
    Planeta* planet;
    Nave *nave1, *nave2;
    Projetil projeteis;
    int* indice_projeteis_destruidos;
    Coordenada coordenada_planeta;
    int numero_projetil, tempo_projetil, i;
    double tempo_execucao;
    inicia_jogo(planet, nave1, nave2, projeteis, tempo_execucao, indice_projeteis_destruidos,
                &coordenada_planeta, &numero_projetil, &tempo_projetil);
    
    for (i = 0;i < tempo_execucao*10; i++) {
        rodada_jogo(planet, nave1, nave2, projeteis, indice_projeteis_destruidos, &coordenada_planeta,
                &numero_projetil, &tempo_projetil);
    printf("\ntemos a seguinte configuração no mapa no momento tempo = %d:\n", j+1);
    printf("           cordenada                                 velocidade\n\n");
    printf("planeta     (%.4lf,%.4lf)                    (%.4lf,%.4lf)\n", coordenada_planeta.x, coordenada_planeta.y, 0.0, 0.0);
    printf("nave 1      (%.4lf,%.4lf)         (%lf,%.4lf)\n", nave1.coordenada_nave.x, nave1.coordenada_nave.y, nave1.velocidade_nave.vel_x, 
                                                                        nave1.velocidade_nave.vel_y);
    printf("nave 2      (%.4lf,%.4lf)         (%.4lf,%.4lf)\n", nave2.coordenada_nave.x, nave2.coordenada_nave.y, nave2.velocidade_nave.vel_x, 
                                                                        nave2.velocidade_nave.vel_y);
    for (int i = 0; i < numero_projetil; i++) {
        if(indice_projeteis_destruidos[i] == -1) {
            printf("projetil %d     (%.4lf,%.4lf)         (%.4lf,%.4lf)\n", i, projeteis[i].coordenada_projetil.x, projeteis[i].coordenada_projetil.y, 
                                                                    projeteis[i].velocidade_projetil.vel_x, projeteis[i].velocidade_projetil.vel_y);
        }
        
        /********
         * 
         * 
         * parte grafica aqui
         * 
         * 
        *********/
        
/*    }
    
    
    return 0;
}*/

void inicia_jogo (Planeta* planet, Nave* nave1, Nave* nave2, Projetil** projeteis, double* tempo_execucao, 
                int** indice_projeteis_destruidos, Coordenada* coordenada_planeta, int* numero_projetil, double* tempo_projetil, int num_tiros, int* termina_jogo) {
    ponteiro = fopen("entrada.txt", "r");

    coordenada_planeta->x = 3.5e7;
    coordenada_planeta->y = 2.5e7;
    nave1->nome = malloc(100*sizeof(char));
    nave2->nome = malloc(100*sizeof(char));
    fscanf(ponteiro, "%lf %lf %lf", &planet->raio, &planet->massa, tempo_execucao);
    fscanf(ponteiro, "%s %lf %lf %lf %lf %lf", nave1->nome, &nave1->massa, &nave1->coordenada_nave.x, &nave1->coordenada_nave.y, 
                                                &nave1->velocidade_nave.vel_x, &nave1->velocidade_nave.vel_y);
    if(nave1->coordenada_nave.x < 0) nave1->coordenada_nave.x = nave1->coordenada_nave.x*(-1);
    if(nave1->coordenada_nave.y < 0) nave1->coordenada_nave.y = nave1->coordenada_nave.y*(-1);

    fscanf(ponteiro, "%s %lf %lf %lf %lf %lf", nave2->nome, &nave2->massa, &nave2->coordenada_nave.x, &nave2->coordenada_nave.y, 
                                                &nave2->velocidade_nave.vel_x, &nave2->velocidade_nave.vel_y);
    if(nave2->coordenada_nave.x < 0) nave2->coordenada_nave.x = nave2->coordenada_nave.x*(-1);
    if(nave2->coordenada_nave.y < 0) nave2->coordenada_nave.y = nave2->coordenada_nave.y*(-1);

    fscanf(ponteiro, "%d %lf", numero_projetil, tempo_projetil);
    *projeteis = malloc(((*numero_projetil) + num_tiros)*sizeof(Projetil));
    *indice_projeteis_destruidos = malloc(((*numero_projetil) + num_tiros)*sizeof(int));

    for(int i = 0; i < *numero_projetil; i++) {
        fscanf(ponteiro, "%lf %lf %lf %lf %lf", &(*projeteis)[i].massa, &(*projeteis)[i].coordenada_projetil.x, &(*projeteis)[i].coordenada_projetil.y, 
                                            &(*projeteis)[i].velocidade_projetil.vel_x, &(*projeteis)[i].velocidade_projetil.vel_y);
        if((*projeteis)[i].coordenada_projetil.x < 0) (*projeteis)[i].coordenada_projetil.x = (*projeteis)[i].coordenada_projetil.x*(-1);
        if((*projeteis)[i].coordenada_projetil.y < 0) (*projeteis)[i].coordenada_projetil.y = (*projeteis)[i].coordenada_projetil.y*(-1);

    }

    for(int i = 0; i < *numero_projetil; i++) {
        (*indice_projeteis_destruidos)[i] = -1;
    }
    for (int i = *numero_projetil; i < (*numero_projetil)+num_tiros; i++) {
        (*projeteis)[i].massa = 2000000;
        (*indice_projeteis_destruidos)[i] = 1;

    }



    /*
    for (int i = 0; i < *numero_projetil; i++) {
        if((*indice_projeteis_destruidos)[i] == -1) {
            printf("projetil %d     (%.4lf,%.4lf)         (%.4lf,%.4lf)\n", i, (*projeteis)[i].coordenada_projetil.x, (*projeteis)[i].coordenada_projetil.y, 
                                                                    (*projeteis)[i].velocidade_projetil.vel_x, (*projeteis)[i].velocidade_projetil.vel_y);
        }
    }
    */
    
    //Atualizando os valores das potencias em 10^6

    fclose(ponteiro);


    //mudança de valordes para que se ajuste melhor à tela.
    nave1->coordenada_nave.x += 1.5e6;
    nave1->coordenada_nave.y += 1.5e6;

    nave2->coordenada_nave.x += 1.5e6;
    nave2->coordenada_nave.y += 1.5e6;

    coordenada_planeta->x += 5e6;
    coordenada_planeta->y += 5e6;

    for(int i = 0; i < *numero_projetil; i++) {
        if((*indice_projeteis_destruidos)[i] == -1) {
            (*projeteis)[i].coordenada_projetil.x += 1e6;
            (*projeteis)[i].coordenada_projetil.y += 1e6;
        }
    }

    if(checa_batida(nave1->coordenada_nave,nave2->coordenada_nave, 1.5e6, 1.5e6)) *termina_jogo = 3;
    if(checa_batida(nave1->coordenada_nave, *coordenada_planeta, planet->raio, 1.5e6)) *termina_jogo = 2;
    if(checa_batida(nave2->coordenada_nave, *coordenada_planeta, planet->raio, 1.5e6)) *termina_jogo = 1;
    for(int i = 0; i < *numero_projetil; i++) {
        if(checa_batida(nave1->coordenada_nave, (*projeteis)[i].coordenada_projetil, 1.5e6, 1e6)) *termina_jogo = 2;
        if(checa_batida(nave2->coordenada_nave, (*projeteis)[i].coordenada_projetil, 1.5e6, 1e6)) *termina_jogo = 1;
    }
    for(int i = 0; i < *numero_projetil - 1; i++) {
        if((*indice_projeteis_destruidos)[i] == -1) {
            for(int j = i + 1; j < *numero_projetil; j++) {
                if((*indice_projeteis_destruidos)[j] == -1) {
                    if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, (*projeteis)[j].coordenada_projetil, 1e6, 1e6)) {
                        (*indice_projeteis_destruidos)[i] = 1;
                        (*indice_projeteis_destruidos)[j] = 1;
                    }
                }
            }
        }
    }
    for(int i = 0; i < *numero_projetil; i++) {
        if((*indice_projeteis_destruidos)[i] == -1) {
            if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, *coordenada_planeta, 1e6, planet->raio))
                (*indice_projeteis_destruidos)[i] = 1;
        }
    }

    nave1->coordenada_nave.x -= 1.5e6;
    nave1->coordenada_nave.y -= 1.5e6;

    nave2->coordenada_nave.x -= 1.5e6;
    nave2->coordenada_nave.y -= 1.5e6;

    coordenada_planeta->x -= 5e6;
    coordenada_planeta->y -= 5e6;

    for(int i = 0; i < *numero_projetil; i++) {
        if((*indice_projeteis_destruidos)[i] == -1) {
            (*projeteis)[i].coordenada_projetil.x -= 1e6;
            (*projeteis)[i].coordenada_projetil.y -= 1e6;
        }
    }

}

void rodada_jogo (Planeta* planet, Nave* nave1, Nave* nave2, Projetil** projeteis, int** indice_projeteis_destruidos, 
    Coordenada* coordenada_planeta, int* numero_projetil, double* tempo_projetil, int num_tiros, int* termina_jogo) {
    Aceleracao aceleracao_nave_01, aceleracao_nave_02;
    Forca_r forca_nave_01, forca_nave_02;
    Aceleracao aceleracao_projeteis[*numero_projetil];
    Forca_r forca_projeteis[*numero_projetil];


        forca_nave_01 = forca_resultante(nave1->coordenada_nave, nave2->coordenada_nave, nave1->massa,
                                                nave2->massa);

        forca_nave_02.forca_x = (-forca_nave_01.forca_x);
        forca_nave_02.forca_y = (-forca_nave_01.forca_y);


        forca_nave_01.forca_x += forca_resultante(nave1->coordenada_nave, *coordenada_planeta, nave1->massa,
                                                                        planet->massa).forca_x;
        forca_nave_01.forca_y += forca_resultante(nave1->coordenada_nave, *coordenada_planeta, nave1->massa,
                                                                        planet->massa).forca_y;

        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                forca_nave_01.forca_x += forca_resultante(nave1->coordenada_nave, (*projeteis)[i].coordenada_projetil, nave1->massa, (*projeteis)[i].massa).forca_x;
                forca_nave_01.forca_y += forca_resultante(nave1->coordenada_nave, (*projeteis)[i].coordenada_projetil, nave1->massa, (*projeteis)[i].massa).forca_y;
            }
        }

        aceleracao_nave_01 = aceleracao(forca_nave_01, nave1->massa);

        forca_nave_02.forca_x += forca_resultante(nave2->coordenada_nave, *coordenada_planeta, nave2->massa,
                                                                        planet->massa).forca_x;
        forca_nave_02.forca_y += forca_resultante(nave2->coordenada_nave, *coordenada_planeta, nave2->massa,
                                                                        planet->massa).forca_y;

        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                forca_nave_02.forca_x += forca_resultante(nave2->coordenada_nave, (*projeteis)[i].coordenada_projetil, nave2->massa, (*projeteis)[i].massa).forca_x;
                forca_nave_02.forca_y += forca_resultante(nave2->coordenada_nave, (*projeteis)[i].coordenada_projetil, nave2->massa, (*projeteis)[i].massa).forca_y;
            }
        }

        aceleracao_nave_02 = aceleracao(forca_nave_02, nave2->massa);

        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                //forca do planeta no projetil.
                forca_projeteis[i] = forca_resultante((*projeteis)[i].coordenada_projetil, *coordenada_planeta, (*projeteis)[i].massa, 
                                                                            planet->massa);
                //forca da nave 1 no projetil.
                forca_projeteis[i].forca_x += forca_resultante((*projeteis)[i].coordenada_projetil, nave1->coordenada_nave, (*projeteis)[i].massa,
                                                                              nave1->massa).forca_x;
                forca_projeteis[i].forca_y += forca_resultante((*projeteis)[i].coordenada_projetil, nave1->coordenada_nave, (*projeteis)[i].massa,
                                                                              nave1->massa).forca_y;
                //forca da nave 2 no projetil.
                forca_projeteis[i].forca_x += forca_resultante((*projeteis)[i].coordenada_projetil, nave2->coordenada_nave, (*projeteis)[i].massa,
                                                                              nave2->massa).forca_x;
                forca_projeteis[i].forca_y += forca_resultante((*projeteis)[i].coordenada_projetil, nave2->coordenada_nave, (*projeteis)[i].massa,
                                                                              nave2->massa).forca_y;
                //forca dos projeteis no projetil
                for(int j = i+1; j < ((*numero_projetil) + num_tiros); j++) {
                    if((*indice_projeteis_destruidos)[j] == -1) {
                        forca_projeteis[i].forca_x += forca_resultante((*projeteis)[i].coordenada_projetil, (*projeteis)[j].coordenada_projetil, 
                                                                        (*projeteis)[i].massa, (*projeteis)[j].massa).forca_x;
                        forca_projeteis[i].forca_y += forca_resultante((*projeteis)[i].coordenada_projetil, (*projeteis)[j].coordenada_projetil, 
                                                                        (*projeteis)[i].massa, (*projeteis)[j].massa).forca_y;
                    }
                }

                aceleracao_projeteis[i] = aceleracao(forca_projeteis[i], (*projeteis)[i].massa);

                atualiza_posicao(&(*projeteis)[i].coordenada_projetil, &(*projeteis)[i].velocidade_projetil, aceleracao_projeteis[i], 1);
            }
        }
        atualiza_posicao(&nave1->coordenada_nave, &nave1->velocidade_nave, aceleracao_nave_01, 1);
        atualiza_posicao(&nave2->coordenada_nave, &nave2->velocidade_nave, aceleracao_nave_02, 1);

        nave1->coordenada_nave.x += 1.5e6;
        nave1->coordenada_nave.y += 1.5e6;

        nave2->coordenada_nave.x += 1.5e6;
        nave2->coordenada_nave.y += 1.5e6;

        coordenada_planeta->x += 5e6;
        coordenada_planeta->y += 5e6;

        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                if(i >= 4) {
                    (*projeteis)[i].coordenada_projetil.x += 8e5;
                    (*projeteis)[i].coordenada_projetil.y += 8e5;
                }
                else {
                    (*projeteis)[i].coordenada_projetil.x += 1e6;
                    (*projeteis)[i].coordenada_projetil.y += 1e6;
                }
            }
        }

        if(checa_batida(nave1->coordenada_nave, nave2->coordenada_nave, 1.5e6, 1.5e6)) *termina_jogo = 3;
        if(checa_batida(nave1->coordenada_nave, *coordenada_planeta, planet->raio, 1.5e6)) *termina_jogo = 2;
        if(checa_batida(nave2->coordenada_nave, *coordenada_planeta, planet->raio, 1.5e6)) *termina_jogo = 1;
        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                if(i >= 4) {
                    if(checa_batida(nave1->coordenada_nave, (*projeteis)[i].coordenada_projetil, 1.5e6, 8e5)) *termina_jogo = 2;
                    if(checa_batida(nave2->coordenada_nave, (*projeteis)[i].coordenada_projetil, 1.5e6, 8e5)) *termina_jogo = 1;
                }
                else {
                    if(checa_batida(nave1->coordenada_nave, (*projeteis)[i].coordenada_projetil, 1.5e6, 1e6)) *termina_jogo = 2;
                    if(checa_batida(nave2->coordenada_nave, (*projeteis)[i].coordenada_projetil, 1.5e6, 1e6)) *termina_jogo = 1;
                }
            }
        }
        //checa batida entre dois projeteis que ainda não colidiram.
        for(int i = 0; i < ((*numero_projetil) + num_tiros-1); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                for(int j = i + 1; j < ((*numero_projetil) + num_tiros); j++) {
                    if((*indice_projeteis_destruidos)[j] == -1) {
                        if(i >= 4) {
                            if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, (*projeteis)[j].coordenada_projetil, 8e5, 8e5)) {
                                (*indice_projeteis_destruidos)[i] = 1;
                                (*indice_projeteis_destruidos)[j] = 1;
                            }
                        }
                        else if(j >= 4) {
                            if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, (*projeteis)[j].coordenada_projetil, 1e6, 8e5)) {
                                (*indice_projeteis_destruidos)[i] = 1;
                                (*indice_projeteis_destruidos)[j] = 1;
                            }
                        }
                        else {
                            if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, (*projeteis)[j].coordenada_projetil, 1e6, 1e6)) {
                                (*indice_projeteis_destruidos)[i] = 1;
                                (*indice_projeteis_destruidos)[j] = 1;
                            }
                        }
                    }
                }
            }
        }
        //checa batida entre os projeteis e o planeta
        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                if(i >= 4) {
                    if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, *coordenada_planeta, 8e5, planet->raio))
                        (*indice_projeteis_destruidos)[i] = 1;    
                }
                else {
                    if(checa_batida_projeteis((*projeteis)[i].coordenada_projetil, *coordenada_planeta, 1e6, planet->raio))
                        (*indice_projeteis_destruidos)[i] = 1;
                }
            }
        }

        nave1->coordenada_nave.x -= 1.5e6;
        nave1->coordenada_nave.y -= 1.5e6;

        nave2->coordenada_nave.x -= 1.5e6;
        nave2->coordenada_nave.y -= 1.5e6;

        coordenada_planeta->x -= 5e6;
        coordenada_planeta->y -= 5e6;

        for(int i = 0; i < ((*numero_projetil) + num_tiros); i++) {
            if((*indice_projeteis_destruidos)[i] == -1) {
                if(i >= 4) {
                    (*projeteis)[i].coordenada_projetil.x -= 8e5;
                    (*projeteis)[i].coordenada_projetil.y -= 8e5;    
                }
                else {
                    (*projeteis)[i].coordenada_projetil.x -= 1e6;
                    (*projeteis)[i].coordenada_projetil.y -= 1e6;
                }
            }
        }

        *tempo_projetil -= 0.0002;
        if ((*numero_projetil > 0) && (*tempo_projetil < EPSILON) && (*tempo_projetil > -0.0002 + EPSILON)) {
            //*numero_projetil = 0;
            //free(*projeteis);
            //free(*indice_projeteis_destruidos);
            for (int i = 0; i < *numero_projetil; i++) {
                (*indice_projeteis_destruidos)[i] = 1;
            }
            printf("\nProjeteis sumiram? Aonde será que foram parar? \n");
        }
}
