#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Para uso de função 'memset()'

#include "matriz.h"

/**
 * Função que gera valores para para ser usado em uma matriz
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
 *  @return valor gerado para a posição i,j
 */
static inline real_t generateRandomA(unsigned int i, unsigned int j)
{
  static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
  return ((i == j) ? (real_t)(BASE << 1) : 1.0) * (real_t)random() * invRandMax;
}

/**
 * Função que gera valores aleatórios para ser usado em um vetor
 * @return valor gerado
 *
 */
static inline real_t generateRandomB()
{
  static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
  return (real_t)(BASE << 2) * (real_t)random() * invRandMax;
}

/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 *
 *  @param m     número de linhas da matriz
 *  @param n     número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMatRow(int m, int n, int zerar)
{
  MatRow matriz = (real_t *)malloc(m * n * sizeof(real_t));

  if (matriz)
  {
    if (zerar)
      memset(matriz, 0, m * n * sizeof(real_t));
    else
      for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
          matriz[i * n + j] = generateRandomA(i, j);
  }

  return (matriz);
}

/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 *
 *  @param n  número de elementos do vetor
 *  @param zerar se 0, vetor  tem valores aleatórios, caso contrário,
 *               vetor tem valores todos nulos
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor(int n, int zerar)
{
  Vetor vetor = (real_t *)malloc(n * sizeof(real_t));

  if (vetor)
  {
    if (zerar)
      memset(vetor, 0, n * sizeof(real_t));
    else
      for (int i = 0; i < n; ++i)
        vetor[i] = generateRandomB();
  }

  return (vetor);
}

/**
 *  \brief: libera vetor
 *
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor(void *vet)
{
  free(vet);
}

/**
 *  Funcao multMatVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatVet(MatRow mat, Vetor v, int m, int n, Vetor res)
{

  /* Efetua a multiplicação */
  if (res)
  {
    for (int i = 0; i < m; ++i)
      for (int j = 0; j < n; ++j)
        res[i] += mat[n * i + j] * v[j];
  }
}

/**
 *  Funcao multMatMat: Efetua multiplicacao de duas matrizes 'n x n'
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void multMatMat(MatRow A, MatRow B, int n, MatRow C)
{

  /* Efetua a multiplicação */
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      for (int k = 0; k < n; ++k)
        C[i * n + j] += A[i * n + k] * B[k * n + j];
}

/**
 *  Funcao prnMat:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMat(MatRow mat, int m, int n)
{
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
      printf(DBL_FIELD, mat[n * i + j]);
    printf("\n");
  }
  printf(SEP_RES);
}

void multMatVetOtimizada(MatRow mat, Vetor v, int m, int n, Vetor res)
{

  for (int i = 0; i < m - m % UF_FACTOR; i += UF_FACTOR)
  {
    for (int j = 0; j < n; ++j)
      res[i] = res[i] + mat[i * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 1] = res[i + 1] + mat[(i + 1) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 2] = res[i + 2] + mat[(i + 2) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 3] = res[i + 3] + mat[(i + 3) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 4] = res[i + 4] + mat[(i + 4) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 5] = res[i + 5] + mat[(i + 5) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 6] = res[i + 6] + mat[(i + 6) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 7] = res[i + 7] + mat[(i + 7) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 8] = res[i + 8] + mat[(i + 8) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 9] = res[i + 9] + mat[(i + 9) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 10] = res[i + 10] + mat[(i + 10) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 11] = res[i + 11] + mat[(i + 11) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 12] = res[i + 12] + mat[(i + 12) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 13] = res[i + 13] + mat[(i + 13) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 14] = res[i + 14] + mat[(i + 14) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 15] = res[i + 15] + mat[(i + 15) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 16] = res[i + 16] + mat[(i + 16) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 17] = res[i + 17] + mat[(i + 17) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 18] = res[i + 18] + mat[(i + 18) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 19] = res[i + 19] + mat[(i + 19) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 20] = res[i + 20] + mat[(i + 20) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 21] = res[i + 21] + mat[(i + 21) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 22] = res[i + 22] + mat[(i + 22) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 23] = res[i + 23] + mat[(i + 23) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 24] = res[i + 24] + mat[(i + 24) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 25] = res[i + 25] + mat[(i + 25) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 26] = res[i + 26] + mat[(i + 26) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 27] = res[i + 27] + mat[(i + 27) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 28] = res[i + 28] + mat[(i + 28) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 29] = res[i + 29] + mat[(i + 29) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 30] = res[i + 30] + mat[(i + 30) * n + j] * v[j];
    for (int j = 0; i < n; ++j)
      res[i + 31] = res[i + 31] + mat[(i + 31) * n + j] * v[j];
  }

  for (int i = n - n % UF_FACTOR; i < n; ++i)
    for (int j = 0; j < n; ++j)
      res[i] = res[i] + mat[i * n + j] * v[j];
}

void multMatMatOtimizada(MatRow A, MatRow B, int n, MatRow C)
{

  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n - n % BK; j += BK)
    {
      C[i * n + j] = C[i * n + (j + 1)] = C[i * n + (j + 2)] = C[i * n + (j + 3)] = C[i * n + (j + 4)] = C[i * n + (j + 5)] = C[i * n + (j + 6)] = C[i * n + (j + 7)] = C[i * n + (j + 8)] = C[i * n + (j + 9)] = C[i * n + (j + 10)] = C[i * n + (j + 11)] = C[i * n + (j + 12)] = C[i * n + (j + 13)] = C[i * n + (j + 14)] = C[i * n + (j + 15)] = 0;
      for (int k = 0; k < n; ++k)
      {
        C[i * n + j] += A[i * n + k] * B[k * n + j];
        C[i * n + (j + 1)] += A[i * n + k] * B[k * n + (j + 1)];
        C[i * n + (j + 2)] += A[i * n + k] * B[k * n + (j + 2)];
        C[i * n + (j + 3)] += A[i * n + k] * B[k * n + (j + 3)];
        C[i * n + (j + 4)] += A[i * n + k] * B[k * n + (j + 4)];
        C[i * n + (j + 5)] += A[i * n + k] * B[k * n + (j + 5)];
        C[i * n + (j + 6)] += A[i * n + k] * B[k * n + (j + 6)];
        C[i * n + (j + 7)] += A[i * n + k] * B[k * n + (j + 7)];
        C[i * n + (j + 8)] += A[i * n + k] * B[k * n + (j + 8)];
        C[i * n + (j + 9)] += A[i * n + k] * B[k * n + (j + 9)];
        C[i * n + (j + 10)] += A[i * n + k] * B[k * n + (j + 10)];
        C[i * n + (j + 11)] += A[i * n + k] * B[k * n + (j + 11)];
        C[i * n + (j + 12)] += A[i * n + k] * B[k * n + (j + 12)];
        C[i * n + (j + 13)] += A[i * n + k] * B[k * n + (j + 13)];
        C[i * n + (j + 14)] += A[i * n + k] * B[k * n + (j + 14)];
        C[i * n + (j + 15)] += A[i * n + k] * B[k * n + (j + 15)];
      }
    }

    for (int j = n - n % BK; j < n; ++j)
    {
      C[i * n + j] = 0;
      for (int k = 0; k < n; ++k)
        C[i * n + j] += A[i * n + k] * B[k * n + j];
    }
  }
}

/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */

void prnVetor(Vetor vet, int n)
{
  for (int i = 0; i < n; ++i)
    printf(DBL_FIELD, vet[i]);
  printf(SEP_RES);
}
