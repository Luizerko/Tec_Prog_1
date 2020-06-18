#include <stdio.h>
#include <math.h>
#include "xwc.h"
#include "funcionamento_jogo.h"

int main(int ac, char **av) {

#ifdef NOXPM
    puts("Este programa só funciona com a biblioteca Xpm!");
#else

    WINDOW *w;
    PIC fundo;

    printf("Abrindo a janela\n");
    w = InitGraph(800, 600, "Janela");

    fundo = ReadPic(w, "fundo_do_mar.xpm", NULL);
    getchar();
    printf("Printando a imagem de fundo\n");
    PutPic(w, fundo, 0, 0, 800, 800, 0, 0);
    getchar();

    CloseGraph();

#endif

    return 0;
}


