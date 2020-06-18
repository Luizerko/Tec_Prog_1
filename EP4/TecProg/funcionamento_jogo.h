#ifndef _FUNCIONAMENTO_JOGO_H_
#define _FUNCIONAMENTO_JOGO_H_


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

double distancia(Coordenada ponto_1, Coordenada ponto_2);

Forca_r forca_resultante(Coordenada corpo_1, Coordenada corpo_2, double massa_1, double massa_2);

Aceleracao aceleracao(Forca_r forca, double massa);

void atualiza_posicao (Coordenada *ponto, Velocidade *vel, Aceleracao acel, int tempo);

int checa_batida(Coordenada ponto_obj1, Coordenada ponto_obj2, double raio_obj1, double raio_obj2);

int checa_batida_projeteis(Coordenada ponto_obj1, Coordenada ponto_obj2, double raio_obj1, double raio_obj2);

void acelera_ae (Nave* nave, int frente);

void desacelera_ae(Nave* nave, int frente);

void inicia_tiro (Nave nave, Projetil** projeteis, int** indice_projeteis_destruidos, int frente, int* numero_projetil);

void normaliza_vetor(double* x, double* y);

void inicia_jogo (Planeta* planet, Nave* nave1, Nave* nave2, Projetil** projeteis, double* tempo_execucao, 
                int** indice_projeteis_destruidos, Coordenada* coordenada_planeta, int* numero_projetil, double* tempo_projetil, int num_tiros, int* termina_jogo);

void rodada_jogo (Planeta* planet, Nave* nave1, Nave* nave2, Projetil** projeteis,
 int** indice_projeteis_destruidos, Coordenada* coordenada_planeta, int* numero_projetil, double* tempo_projetil, int num_tiros, int* termina_jogo);


#endif