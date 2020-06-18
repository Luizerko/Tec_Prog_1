#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *ponteiro;

#define EPSILON 0.0001

typedef struct coordenada {
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
} Projetil;

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
    horizontal = 1000000000; // tamanho para cima e para baixo com referencial no 0,0
    vertical = 1000000000;

    ponto->x += vel->vel_x*tempo + (acel.acel_x*pow(tempo,2))/2;
    ponto->y += vel->vel_y*tempo + (acel.acel_y*pow(tempo,2))/2;
    vel->vel_x += acel.acel_x * tempo;
    vel->vel_y += acel.acel_y * tempo;

    //implementa o campo toroidal, modificando o valor da coordenada caso o objeto se posicione fora do campo.
    if (ponto->x > abs(horizontal))
     if (ponto->x > 0) 
      ponto->x = (ponto->x-1000000000) -horizontal;
     else
      ponto->x = horizontal + (ponto->x+1000000000);

    if (ponto->y > abs(vertical))
     if (ponto->y > 0) 
      ponto->y = (ponto->x-1000000000) - vertical;
     else
      ponto->y = vertical + ponto->x+1000000000;
     

}

//checa se dois objetos estão no mesmo espaço, configurando uma batida.
void checa_batida(Coordenada ponto_obj1, Coordenada ponto_obj2, int raio_obj1, int raio_obj2){
    if (distancia(ponto_obj1, ponto_obj2) < (raio_obj1 + raio_obj2)) {
        printf("houve uma batida! O jogo acabou");
    exit(0);
    }
}

int checa_batida_projeteis(Coordenada ponto_obj1, Coordenada ponto_obj2, int raio_obj1, int raio_obj2) {
    if (distancia(ponto_obj1, ponto_obj2) < (raio_obj1 + raio_obj2)) {
        printf("\nProjeteis bateram!\n");
        return 1;
    }
    return 0;
}

int main() {
    ponteiro = fopen("entrada.txt", "r");
    Planeta planet;
    Nave nave1, nave2;
    //nave1 = malloc(sizeof(Nave)); nave2 = malloc(sizeof(Nave));
    nave1.nome = malloc(100*sizeof(char)); nave2.nome = malloc(100*sizeof(char));
    Projetil* projeteis;
    long long int* indice_projeteis_destruidos;
    Aceleracao aceleracao_nave_01, aceleracao_nave_02;
    Forca_r forca_nave_01, forca_nave_02;
    Aceleracao* aceleracao_projeteis;
    Forca_r* forca_projeteis;
    Coordenada coordenada_planeta;
    coordenada_planeta.x = 0;
    coordenada_planeta.y = 0;
    double tempo_de_execucao, tempo_projetil;
    int numero_projetil;

    fscanf(ponteiro, "%lf %lf %lf", &planet.massa, &planet.raio, &tempo_de_execucao);
    fscanf(ponteiro, "%s %lf %lf %lf %lf %lf", nave1.nome, &nave1.massa, &nave1.coordenada_nave.x, &nave1.coordenada_nave.y, 
                                                &nave1.velocidade_nave.vel_x, &nave1.velocidade_nave.vel_y);
    fscanf(ponteiro, "%s %lf %lf %lf %lf %lf", nave2.nome, &nave2.massa, &nave2.coordenada_nave.x, &nave2.coordenada_nave.y, 
                                                &nave2.velocidade_nave.vel_x, &nave2.velocidade_nave.vel_y);
    fscanf(ponteiro, "%d %lf", &numero_projetil, &tempo_projetil);

    projeteis = malloc(numero_projetil*sizeof(Projetil));
    indice_projeteis_destruidos = malloc(numero_projetil*sizeof(long long int));
    aceleracao_projeteis = malloc(numero_projetil*sizeof(Aceleracao));
    forca_projeteis = malloc(numero_projetil*sizeof(Forca_r));

    for(int i = 0; i < numero_projetil; i++) {
        fscanf(ponteiro, "%lf %lf %lf %lf %lf", &projeteis[i].massa, &projeteis[i].coordenada_projetil.x, &projeteis[i].coordenada_projetil.y, 
                                            &projeteis[i].velocidade_projetil.vel_x, &projeteis[i].velocidade_projetil.vel_y);
    }

    //Atualizando os valores das potencias em 10^6

    fclose(ponteiro);

    for(int i = 0; i < numero_projetil; i++) {
        indice_projeteis_destruidos[i] = -1;
    }
    checa_batida(nave1.coordenada_nave,nave2.coordenada_nave, 1, 1);
    checa_batida(nave1.coordenada_nave, coordenada_planeta, planet.raio, 1);
    checa_batida(nave2.coordenada_nave, coordenada_planeta, planet.raio, 1);
    for(int i = 0; i < numero_projetil; i++) {
        checa_batida(nave1.coordenada_nave, projeteis[i].coordenada_projetil, 1, 1);
        checa_batida(nave2.coordenada_nave, projeteis[i].coordenada_projetil, 1, 1);
    }
    for(int i = 0; i < numero_projetil - 1; i++) {
        if(indice_projeteis_destruidos[i] == -1) {
            for(int j = i + 1; j < numero_projetil; j++) {
                    if(checa_batida_projeteis(projeteis[i].coordenada_projetil, projeteis[j].coordenada_projetil, 1, 1)) {
                        indice_projeteis_destruidos[i] = 1;
                        indice_projeteis_destruidos[j] = 1;
                }
            }
        }
    }
    printf("\ntemos a seguinte configuração no mapa no momento tempo = %d:\n", 0);
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

    }
    for(int j = 0; j < tempo_de_execucao*10 ; j++) { 



        forca_nave_01 = forca_resultante(nave1.coordenada_nave, nave2.coordenada_nave, nave1.massa,
                                                nave2.massa);

        forca_nave_02.forca_x = (-forca_nave_01.forca_x);
        forca_nave_02.forca_y = (-forca_nave_01.forca_y);


        forca_nave_01.forca_x += forca_resultante(nave1.coordenada_nave, coordenada_planeta, nave1.massa,
                                                                        planet.massa).forca_x;
        forca_nave_01.forca_y += forca_resultante(nave1.coordenada_nave, coordenada_planeta, nave1.massa,
                                                                        planet.massa).forca_y;

        for(int i = 0; i < numero_projetil; i++) {
            forca_nave_01.forca_x += forca_resultante(nave1.coordenada_nave, projeteis[i].coordenada_projetil, nave1.massa, projeteis[i].massa).forca_x;
            forca_nave_01.forca_y += forca_resultante(nave1.coordenada_nave, projeteis[i].coordenada_projetil, nave1.massa, projeteis[i].massa).forca_y;
        }

        aceleracao_nave_01 = aceleracao(forca_nave_01, nave1.massa);

        forca_nave_02.forca_x += forca_resultante(nave2.coordenada_nave, coordenada_planeta, nave2.massa,
                                                                        planet.massa).forca_x;
        forca_nave_02.forca_y += forca_resultante(nave2.coordenada_nave, coordenada_planeta, nave2.massa,
                                                                        planet.massa).forca_y;

        for(int i = 0; i < numero_projetil; i++) {
            forca_nave_02.forca_x += forca_resultante(nave2.coordenada_nave, projeteis[i].coordenada_projetil, nave2.massa, projeteis[i].massa).forca_x;
            forca_nave_02.forca_y += forca_resultante(nave2.coordenada_nave, projeteis[i].coordenada_projetil, nave2.massa, projeteis[i].massa).forca_y;
        }

        aceleracao_nave_02 = aceleracao(forca_nave_02, nave2.massa);

        for(int i = 0; i < numero_projetil; i++) {
            //forca do planeta no projetil.
            forca_projeteis[i] = forca_resultante(projeteis[i].coordenada_projetil, coordenada_planeta, projeteis[i].massa, 
                                                                        planet.massa);
            //forca da nave 1 no projetil.
            forca_projeteis[i].forca_x += forca_resultante(projeteis[i].coordenada_projetil, nave1.coordenada_nave, projeteis[i].massa,
                                                                          nave1.massa).forca_x;
            forca_projeteis[i].forca_y += forca_resultante(projeteis[i].coordenada_projetil, nave1.coordenada_nave, projeteis[i].massa,
                                                                          nave1.massa).forca_y;
            //forca da nave 2 no projetil.
            forca_projeteis[i].forca_x += forca_resultante(projeteis[i].coordenada_projetil, nave2.coordenada_nave, projeteis[i].massa,
                                                                          nave2.massa).forca_x;
            forca_projeteis[i].forca_y += forca_resultante(projeteis[i].coordenada_projetil, nave2.coordenada_nave, projeteis[i].massa,
                                                                          nave2.massa).forca_y;
            
            aceleracao_projeteis[i] = aceleracao(forca_projeteis[i], projeteis[i].massa);

            atualiza_posicao(&projeteis[i].coordenada_projetil, &projeteis[i].velocidade_projetil, aceleracao_projeteis[i], 1);
            
        }
        atualiza_posicao(&nave1.coordenada_nave, &nave1.velocidade_nave, aceleracao_nave_01, 1);
        atualiza_posicao(&nave2.coordenada_nave, &nave2.velocidade_nave, aceleracao_nave_02, 1);

        checa_batida(nave1.coordenada_nave,nave2.coordenada_nave, 1, 1);
        checa_batida(nave1.coordenada_nave, coordenada_planeta, planet.raio, 1);
        checa_batida(nave2.coordenada_nave, coordenada_planeta, planet.raio, 1);
        for(int i = 0; i < numero_projetil; i++) {
            checa_batida(nave1.coordenada_nave, projeteis[i].coordenada_projetil, 1, 1);
            checa_batida(nave2.coordenada_nave, projeteis[i].coordenada_projetil, 1, 1);
        }
        for(int i = 0; i < numero_projetil - 1; i++) {
            if(indice_projeteis_destruidos[i] == -1) {
                for(int j = i + 1; j < numero_projetil; j++) {
                    if(indice_projeteis_destruidos[j] == -1) {
                        if(checa_batida_projeteis(projeteis[i].coordenada_projetil, projeteis[j].coordenada_projetil, 1, 1)) {
                            indice_projeteis_destruidos[i] = 1;
                            indice_projeteis_destruidos[j] = 1;
                        }
                    }
                }
            }
        }

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

        }
        tempo_projetil -= 0.1;
        if (numero_projetil > 0 && tempo_projetil < EPSILON) {
            numero_projetil = 0;
            free(projeteis);
            printf("\nProjeteis sumiram? Aonde será que foram parar?\n");
        }
    }

    return 0;
}