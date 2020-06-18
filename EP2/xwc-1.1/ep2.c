/*
Tecnicas de Programacao I - MAC0216
Exercício Programa 04
Alunos:
Fernanda Itoda - 10740825
Igor Barberino - 11221689
Luis Vitor Zerkowski - 9837201
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "xwc.h"
#include "funcionamento_jogo.h"

#define epsolon 0.001

int recomeca_jogo(WINDOW *w) {
    while(WGetKey(w) != 36);
    return 1;
}

int main(int ac, char **av) {

#ifdef NOXPM
    puts("Este programa só funciona com a biblioteca Xpm!");
#else
    int comecar = 1;
    int termina_jogo = 0, placar_1 = 0, placar_2 = 0;
    WINDOW *w;

    PIC barra_lateral_d, barra_lateral_e, barra_superior, barra_inferior, abertura_imagem;
    w = InitGraph(800, 600, "Janela");
    abertura_imagem = ReadPic(w, "Abertura.xpm", NULL);
    barra_superior = ReadPic(w, "Barra_superior.xpm", NULL);
    barra_inferior = ReadPic(w, "Barra_superior.xpm", NULL);
    barra_lateral_d = ReadPic(w, "Barra_lateral.xpm", NULL);
    barra_lateral_e = ReadPic(w, "Barra_lateral.xpm", NULL);
    PutPic(w, barra_inferior, 0, 0, 800, 1000, 120, 580);
    PutPic(w, barra_lateral_e, 0, 0, 800, 1000, 0, 0);
    PutPic(w, barra_superior, 0, 0, 800, 1000, 120, 0);
    PutPic(w, abertura_imagem, 0, 0, 800, 1000, 90, 80);
    PutPic(w, barra_lateral_d, 0, 0, 800, 1000, 638, 0);
    InitKBD(w);
    while(WGetKey(w) != 36);

    while(comecar == 1 || recomeca_jogo(w) == 1) {
        PIC fundo_imagem, nave1_desenho, nave2_desenho, projeteis_desenho, game_over;

        Planeta planet;
        Coordenada coordenada_planeta;
        Nave nave1, nave2;
        Projetil* projeteis;
        double tempo_de_execucao;
        int* indice_de_projeteis_destruidos;
        int numero_projetil, num_tiros = 100;
        double tempo_projetil;

        PIC numero;
        for(int i = 3; i > 0; i--) {
            WClear(w);
            //fundo_imagem = ReadPic(w, "fundo_universo.xpm", NULL);
            //PutPic(w, fundo_imagem, 0, 0, 800, 1000, 0, 0);
            //MASK mascara = NewMask(w, 500, 500);
            //SetMask(fundo_imagem, mascara);
            if(i == 3) numero = ReadPic(w, "numero_3.xpm", NULL);
            else if (i == 2) numero = ReadPic(w, "numero_2.xpm", NULL);
            else if (i == 1) numero = ReadPic(w, "numero_1.xpm", NULL);
            PutPic(w, numero, 0, 0, 500, 500, 140, 65);
            clock_t start_time = clock();
            while(clock() < start_time + 1000000);
            UnSetMask(w);
        }

        //Inicia o jogo, pasasndo todas as informacoes contidas no arquivo de entrada sobre o planeta, as naves e os projeteis atraves da chamada da funcao
        //inicia_jogo feita na etapa anterior.
        inicia_jogo(&planet, &nave1, &nave2, &projeteis, &tempo_de_execucao, &indice_de_projeteis_destruidos, &coordenada_planeta, &numero_projetil, &tempo_projetil, num_tiros, &termina_jogo);

        //Laco para rodar o jogo. Por padrão, o jogo roda em 1 segundo. E escolhemos um tempo de iteração de 0.02 segundos. Para o exemplo padrão, portanto,
        //o jogo passa-se em 50 iterações. O laco funciona da seguinte maneira: primeiro lemos a imagem de fundo e, baseado nas posições do planeta - constante -,
        //das naves e dos projeteis, e desenhamos as figuras em cima da imagem de fundo. Escolhemos desenhos de tamanhos razoaveis para jogabilidade, fixando os
        //tamanhos do planeta, das naves e dos projeteis, bem como fixando as cores que representarao cada uma das figuras em jogo. Apos desenhar os objetos na
        //imagem de fundo, printamos a imagem de fundo a janela, imagem essa já com todos os objetos devidamente desenhados em seus lugares, e, em seguida, 
        //rodamos uma iteracao do jogo através da chamada da funcao rodada_jogo da etapa anterior. Essa funcao, por sua vez, calcula todas as forcas resultantes 
        //e acaeleracoes sobre os corpos e, depois disso, atualiza a posicao dos objetos no plano cartesiano. Acabada uma iteracao do laco, o tempo de execucao e 
        //reduzido em 0.02 segundos e, apos um pequeno delay existente para que o jogo possa ser visivel, o programa entra no proximo laco ou termina o jogo.
        //OBS: o jogo funcoiona num sistema de coordenadas dentro dos intervalos [-60000000, 60000000] em x e [-80000000, 80000000] em y.
        int i = 0;
        int j = 4;
        Coordenada* frente_nave1 = malloc(8*sizeof(Coordenada));
        Coordenada* frente_nave2 = malloc(8*sizeof(Coordenada));
        int aux = 0;
        while(tempo_de_execucao > epsolon) {
            InitKBD(w);
            WClear(w);

            fundo_imagem = ReadPic(w, "fundo_universo.xpm", NULL);
            int nave1_desenho_x = (int)(nave1.coordenada_nave.x/(pow(10, 5)));
            int nave1_desenho_y = (int)(nave1.coordenada_nave.y/(pow(10, 5)));
            int nave1_desenho_diam=30;

            frente_nave1[0].x = nave1_desenho_x + 10;
            frente_nave1[0].y = nave1_desenho_y + 20;

            frente_nave1[1].x = nave1_desenho_x + 3;
            frente_nave1[1].y = nave1_desenho_y + 17;

            frente_nave1[2].x = nave1_desenho_x + 0;
            frente_nave1[2].y = nave1_desenho_y + 10;

            frente_nave1[3].x = nave1_desenho_x + 3;
            frente_nave1[3].y = nave1_desenho_y + 3;

            frente_nave1[4].x = nave1_desenho_x + 10;
            frente_nave1[4].y = nave1_desenho_y + 0;

            frente_nave1[5].x = nave1_desenho_x + 17;
            frente_nave1[5].y = nave1_desenho_y + 3;

            frente_nave1[6].x = nave1_desenho_x + 20;
            frente_nave1[6].y = nave1_desenho_y + 10;

            frente_nave1[7].x = nave1_desenho_x + 17;
            frente_nave1[7].y = nave1_desenho_y + 17;

            WFillArc(fundo_imagem, nave1_desenho_x, nave1_desenho_y, 0, 360*64, nave1_desenho_diam, nave1_desenho_diam, WNamedColor("Dark Blue")); //Cículo nave1_desenho
            WFillArc(fundo_imagem, frente_nave1[i].x, frente_nave1[i].y, 0, 360*64, 10, 10, WNamedColor("Dark Red")); //Frente da nave1_desenho
            WFillArc(fundo_imagem, frente_nave1[(i+2)%8].x, frente_nave1[(i+2)%8].y, 0, 360*64, 10, 10, WNamedColor("Dark Orange")); //Cículo esquerda nave1_desenho
            WFillArc(fundo_imagem, frente_nave1[(i+4)%8].x, frente_nave1[(i+4)%8].y, 0, 360*64, 10, 10, WNamedColor("Dark Orange")); //Círculo de cima nave1_desenho
            WFillArc(fundo_imagem, frente_nave1[(i+6)%8].x, frente_nave1[(i+6)%8].y, 0, 360*64, 10, 10, WNamedColor("Dark Orange")); //Círculo direita nave1_desenho

            int nave2_desenho_x = (int)(nave2.coordenada_nave.x/(pow(10, 5)));
            int nave2_desenho_y = (int)(nave2.coordenada_nave.y/(pow(10, 5)));
            int nave2_desenho_diam=30;

            frente_nave2[0].x = nave2_desenho_x + 10;
            frente_nave2[0].y = nave2_desenho_y + 20;

            frente_nave2[1].x = nave2_desenho_x + 3;
            frente_nave2[1].y = nave2_desenho_y + 17;

            frente_nave2[2].x = nave2_desenho_x + 0;
            frente_nave2[2].y = nave2_desenho_y + 10;

            frente_nave2[3].x = nave2_desenho_x + 3;
            frente_nave2[3].y = nave2_desenho_y + 3;

            frente_nave2[4].x = nave2_desenho_x + 10;
            frente_nave2[4].y = nave2_desenho_y + 0;

            frente_nave2[5].x = nave2_desenho_x + 17;
            frente_nave2[5].y = nave2_desenho_y + 3;

            frente_nave2[6].x = nave2_desenho_x + 20;
            frente_nave2[6].y = nave2_desenho_y + 10;

            frente_nave2[7].x = nave2_desenho_x + 17;
            frente_nave2[7].y = nave2_desenho_y + 17;

            WFillArc(fundo_imagem, nave2_desenho_x, nave2_desenho_y, 0, 360*64, nave2_desenho_diam, nave2_desenho_diam, WNamedColor("Dark Magenta")); //Círculo nave2_desenho
            WFillArc(fundo_imagem, frente_nave2[j].x, frente_nave2[j].y, 0, 360*64, 10, 10, WNamedColor("Yellow")); //Frente da nave2_desenho
            WFillArc(fundo_imagem, frente_nave2[(j+2)%8].x, frente_nave2[(j+2)%8].y, 0, 360*64, 10, 10, WNamedColor("Green")); //Círculo da direita nave2_desenho
            WFillArc(fundo_imagem, frente_nave2[(j+4)%8].x, frente_nave2[(j+4)%8].y, 0, 360*64, 10, 10, WNamedColor("Green")); //Círculo de baixo nave2_desenho
            WFillArc(fundo_imagem, frente_nave2[(j+6)%8].x, frente_nave2[(j+6)%8].y, 0, 360*64, 10, 10, WNamedColor("Green")); //Círculo da esquerda nave2_desenho

            WFillArc(fundo_imagem, coordenada_planeta.x/(pow(10, 5)), coordenada_planeta.y/(pow(10, 5)), 0, 360*64, 122, 122, WNamedColor("Orange Red")); //Desenha o planeta no centro

            for(int i = 0; i < numero_projetil + num_tiros; i++) {
                if(indice_de_projeteis_destruidos[i] == -1) {
                    int projetil_desenho_x = (int)(projeteis[i].coordenada_projetil.x/(pow(10, 5)));
                    int projetil_desenho_y = (int)(projeteis[i].coordenada_projetil.y/(pow(10, 5)));
                    if(i >= 4) WFillArc(fundo_imagem, projetil_desenho_x, projetil_desenho_y, 0, 360*64, 16, 16, WNamedColor("Violet"));
                    else WFillArc(fundo_imagem, projetil_desenho_x, projetil_desenho_y, 0, 360*64, 20, 20, WNamedColor("Aqua")); //Desenha o projetil i
                }
            }

            PutPic(w, fundo_imagem, 0, 0, 800, 1000, 0, 0);
            
            rodada_jogo(&planet, &nave1, &nave2, &projeteis, &indice_de_projeteis_destruidos, &coordenada_planeta, &numero_projetil, &tempo_projetil, num_tiros, &termina_jogo);

            if(termina_jogo != 0) {
                if(termina_jogo == 1) placar_1++;
                if(termina_jogo == 2) placar_2++;
                termina_jogo = 0;
                game_over = ReadPic(w, "game_over.xpm", NULL);
                clock_t start_time_2 = clock();
                while(clock() < start_time_2 + 3000000);
                WClear(w);
                PutPic(w, game_over, 0, 0, 1000, 800, 120, 40);
                break;
            }

            if(WCheckKBD(w)){
                aux = WGetKey(w);
                if(aux == 114) i = (i + 1)%8;
                else if(aux == 113) {
                    i = (i - 1)%8;
                    if(i < 0) i = 8 + i;
                }

                if(aux == 40) j = (j + 1)%8;
                else if(aux == 38) {
                    j = (j - 1)%8;
                    if(j < 0) j = 8 + j;
                }

                if(aux == 111) acelera_ae(&nave1, i);
                else if(aux == 25) acelera_ae(&nave2, j);

                if(aux == 116) desacelera_ae(&nave1, i);
                else if(aux == 39) desacelera_ae(&nave2, j);

                if(aux == 44) {
                    inicia_tiro(nave1, &projeteis, &indice_de_projeteis_destruidos, i, &numero_projetil);
                    num_tiros--;
                }
                else if(aux == 55) {
                    inicia_tiro(nave2, &projeteis, &indice_de_projeteis_destruidos, j, &numero_projetil);
                    num_tiros--;
                }
            }

            tempo_de_execucao -= 0.0002;
            clock_t start_time = clock();
            while(clock() < start_time + 20000);

        }
        InitKBD(w);
        comecar = 0;
    }

    CloseGraph();

#endif

    return 0;
}