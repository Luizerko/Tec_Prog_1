#include <stdio.h>
#include <unistd.h>
#include "xwc.h"

/* XPM */
static char * nose[] = {
/* nose pixmap
 * width height ncolors chars_per_pixel */
"32 32 4 2 ",
"   c #FFFFFFFFFFFF      s s_#FFFFFFFFFFFF ",
".  c #000   s s_#000 ",
"X  c #FFFFB0B0B0B0      s s_#FFFFB0B0B0B0 ",
"o  c #32329999CCCC      s s_#32329999CCCC ",
/* pixels */
"                                                                ",
"                                                                ",
"                    . . . . .                                   ",
"                  . X X X X X .                                 ",
"                . X X X X X X X .                               ",
"                . X X X X X X X X .                             ",
"                . X X X X X X . . . . . .                       ",
"                . X X X X X .         .   .                     ",
"                . X X X X . . o o o o o . .                     ",
"                . X X X X .       o o o X .                     ",
"                . X X X X .       o o o X X .                   ",
"                . X X X X X .       o X X X X .                 ",
"                . X X X X X X . . . X X X X X X .               ",
"                . X X X X X X X X X X X X X X X X .             ",
"                . X X . . X X X X X X X X X X X X X .           ",
"                . X . X X X X X X X X X X X X X X X .           ",
"                  . X X X X X X X X X X X X X X X X X .         ",
"                  . X X X X X X X X X X X X X X X X X .         ",
"                . X X X X X X X X X X X X X X X X X X .         ",
"                . X X X X X X X X X X X X X X X X X X .         ",
"                . X X X X X X X X X X X X X X X X X X .         ",
"                . X X X X X X X X X X X X X X X X X .           ",
"                . X X X X X X X X X X X X X X X X X .           ",
"                  . X X X . . . X X X X X X X X X .             ",
"                    . . . . X . . . X X X X X . .               ",
"                        . X X . X X . . . . .                   ",
"                        . X X . X X .                           ",
"                        . X X . X X .                           ",
"                        . X X X . . . . .   . .                 ",
"                      . X X X . X X X X X . . X . .             ",
"                    . X X X X X X X X X X X X . X X .           ",
"                    . . . . . . . . . . . . . . . . .           "
} ;

int main(int ac, char **av)
{
  PIC P1, P2, Aux;
#ifdef NOXPM
  puts("Este programa só funciona com a biblioteca Xpm!");
#else
  WINDOW *w1;
  MASK msk;

  w1 = InitGraph(680,300, "Arquivos");
  P1 = ReadPic(w1, "igor_e_fe.xpm", NULL);
  P2 = ReadPic(w1, "Nose.xpm", NULL);


  msk = NewMask(w1, 100, 100);
  Aux = ReadPic(w1, "mascara.xpm", msk);

  puts("Desenhando a figura do arquivo igor_e_fe.xpm.");
  puts("Tecle <enter>"); getchar();
  PutPic(w1, P1, 0,0, 1000, 1000, 0, 0);

  puts("Desenhando o narigudo definido no fonte.");
  puts("Tecle <enter>"); getchar();
  PutPic(w1, P2, 0,0, 32, 32, 100, 0);

  puts("Agora  a figura  do arquivo mascara.xpm.");
  puts("Tecle <enter>"); getchar();
  PutPic(w1, Aux, 0,0, 50, 50, 200, 0);

  SetMask(w1,msk);

  puts("Sobrepondo a última figura com a primeira\n"
       "e usando sua  própria máscara.");
  puts("Tecle <enter>"); getchar();
  PutPic(w1, Aux, 0,0, 32, 32, 0, 0);
  puts("Tecle <enter>"); getchar();

  puts("Gravando o narigudo em Nose.xpm.");
  WritePic(P2,"Nose.xpm", NULL);
  puts("Gravando samp.xpm mascarado em Tutti.xpm.");
  WritePic(P1,"Tutti.xpm", msk);
  CloseGraph();
#endif
  return 0;
}