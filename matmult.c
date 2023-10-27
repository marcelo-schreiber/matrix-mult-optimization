#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */
#include <time.h>
#include <likwid.h>

#include "utils.h"
#include "matriz.h"

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
  exit(1);
}



/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main (int argc, char *argv[]) 
{
  
  int n=DEF_SIZE;
  
  MatRow mRow_1, mRow_2, mRow_3, mRow_4, resMat, resMat2;
  Vetor vet, vet2, res, res2;
  
  /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  if (argc < 2)
    usage(argv[0]);

  n = atoi(argv[1]);
  
  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */
 
  srandom(20232);
      
  res = geraVetor (n, 0); // (real_t *) malloc (n*sizeof(real_t));
  res2 = geraVetor (n, 0);
  resMat = geraMatRow(n, n, 1);
  resMat2 = geraMatRow(n, n, 1);
    
  mRow_1 = geraMatRow (n, n, 0);
  mRow_2 = geraMatRow (n, n, 0);
  mRow_3 = geraMatRow (n, n, 0);
  mRow_4 = geraMatRow (n, n, 0);

  vet = geraVetor (n, 0);
  vet2 = geraVetor (n, 0);

  if (!res || !res2 || !resMat || !resMat2 || !mRow_1 || !mRow_2 || !mRow_3 || !mRow_4 || !vet || !vet2) {
    fprintf(stderr, "Falha em alocação de memória !!\n");
    liberaVetor ((void*) mRow_1);
    liberaVetor ((void*) mRow_2);
    liberaVetor ((void*) mRow_3);
    liberaVetor ((void*) mRow_4);
    liberaVetor ((void*) resMat);
    liberaVetor ((void*) resMat2);
    liberaVetor ((void*) vet);
    liberaVetor ((void*) vet2);
    liberaVetor ((void*) res);
    liberaVetor ((void*) res2);
    exit(2);
  }

  LIKWID_MARKER_INIT;
  
  double t_gen = timestamp();

  LIKWID_MARKER_START("mult-matvet-sem-otimizacao");
  multMatVet(mRow_3, vet2, n, n, res2);
  LIKWID_MARKER_STOP("mult-matvet-sem-otimizacao");
  
  t_gen = timestamp() - t_gen;
  
  printf("tempo matvet sem otimizacao: %f\n", t_gen);  

  t_gen = timestamp();
  
  LIKWID_MARKER_START("mult-matmat-sem-otimizacao");
  multMatMat(mRow_3, mRow_4, n, resMat2);
  LIKWID_MARKER_STOP("mult-matmat-sem-otimizacao");
  
  t_gen = timestamp() - t_gen;

  printf("tempo matmat sem otimizacao: %f\n", t_gen);
  
  t_gen = timestamp();

  LIKWID_MARKER_START("mult-matvet-com-otimizacao");
  multMatVetOtimizada (mRow_1, vet, n, n, res);
  LIKWID_MARKER_STOP("mult-matvet-com-otimizacao");

  t_gen = timestamp() - t_gen;

  printf("tempo matvet com otimizacao: %f\n", t_gen);
  
  t_gen = timestamp();

  LIKWID_MARKER_START("mult-matmat-com-otimizacao");  
  multMatMatOtimizada (mRow_1, mRow_2, n, resMat);
  LIKWID_MARKER_STOP("mult-matmat-com-otimizacao");

  t_gen = timestamp() - t_gen;

  printf("tempo matmat com otimizacao: %f\n", t_gen);

  LIKWID_MARKER_CLOSE;
    
  liberaVetor ((void*) mRow_1);
  liberaVetor ((void*) mRow_2);
  liberaVetor ((void*) mRow_3);
  liberaVetor ((void*) mRow_4);
  liberaVetor ((void*) resMat);
  liberaVetor ((void*) resMat2);
  liberaVetor ((void*) vet);
  liberaVetor ((void*) vet2);
  liberaVetor ((void*) res);
  liberaVetor ((void*) res2);

  return 0;
}

