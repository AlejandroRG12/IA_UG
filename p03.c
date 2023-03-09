/*
EJEMPLO DEL USO DE ESTRUCTURAS DE DATOS CREADAS DINAMICAMENTE
*/
#include<stdio.h>
#include <stdlib.h>

typedef struct{ 
  float re;
  float im;  
}COMPLEJO;

typedef struct{ 
  COMPLEJO *data;
  unsigned int M;//Numero de filas
  unsigned int N;//Numero de columnas
}MATRIZ_COMPLEJA;

void SumaCompleja(COMPLEJO *z1, COMPLEJO *z2);
void ProductoComplejo(COMPLEJO *result, COMPLEJO *z1, COMPLEJO *z2);
void ImprimeComplejo(COMPLEJO *z);
void CapturaMatrizCompleja(MATRIZ_COMPLEJA *C);
void ImprimeMatrizCompleja(MATRIZ_COMPLEJA *C);
MATRIZ_COMPLEJA* CrearMatrizCompleja(unsigned int Nf,unsigned int Nc);
void LiberarMatrizCompleja(MATRIZ_COMPLEJA *M);

MATRIZ_COMPLEJA* ProductoMatrizCompleja(MATRIZ_COMPLEJA *A, MATRIZ_COMPLEJA *B);

int main(){ 
  MATRIZ_COMPLEJA *A, *B; 
  unsigned int filas;
  unsigned int columnas;
  printf("Numero de filas de la matriz A: ");
  scanf("%u",&filas);
  printf("Numero de columnas de la matriz A: ");
  scanf("%u",&columnas);
  //crear matriz
  A=CrearMatrizCompleja(filas,columnas);
  CapturaMatrizCompleja(A);
  printf("\nLa matriz A:\n");
  ImprimeMatrizCompleja(A);
  LiberarMatrizCompleja(A);

  printf("Numero de filas de la matriz B: ");
  scanf("%u",&filas);
  printf("Numero de columnas de la matriz B: ");
  scanf("%u",&columnas);
  B=CrearMatrizCompleja(filas,columnas);
  CapturaMatrizCompleja(B);
  printf("\nLa matriz B:");
  ImprimeMatrizCompleja(B);
  LiberarMatrizCompleja(B);

  MATRIZ_COMPLEJA *C;
  C = ProductoMatrizCompleja(A,B);
  printf("\nA*B");
  ImprimeMatrizCompleja(C);
  LiberarMatrizCompleja(C);

  return 0;
}

MATRIZ_COMPLEJA* CrearMatrizCompleja(unsigned int Nf,unsigned int Nc){ 
  MATRIZ_COMPLEJA *ptr;
  //Reservar la memoria para la Estructura de la matriz compleja
  ptr=(MATRIZ_COMPLEJA *)malloc(sizeof(MATRIZ_COMPLEJA));
  //Verificar el contenido del apuntador 
  if(ptr==NULL)
     { printf("\nError al reservar la memoria para la estructura de Matriz Compleja.");
       exit(0);
     }
  ptr->M=Nf;
  ptr->N=Nc;
  //Reservar la Memoria para almacenar los numeros complejos
  ptr->data=(COMPLEJO*)malloc(Nf*Nc*sizeof(COMPLEJO));
  if(ptr->data==NULL)
     { printf("\nError al reservar la memoria para los datos de la Matriz Compleja.");
       exit(0);
     }
  return ptr;   
}

void ImprimeMatrizCompleja(MATRIZ_COMPLEJA *C){ 
  int i,j;
  for(i=0; i<C->M; i++){
    for(j=0; j<C->N; j++){ 
        ImprimeComplejo(&C->data[i*C->N+j]);
        printf(" ");
    }
    printf("\n");
  }
}

void CapturaMatrizCompleja(MATRIZ_COMPLEJA *C)
{ 
  int i,j;
  for(i=0; i<C->M; i++)
  for(j=0; j<C->N; j++){ 
    printf("dato[%i][%i].re=",i,j);
    scanf("%f",&C->data[i*C->N+j].re);
    printf("dato[%i][%i].im=",i,j);
    scanf("%f",&C->data[i*C->N+j].im);
  }
}

void ImprimeComplejo(COMPLEJO *z)
{ 
  if(z->im<0)
    printf("%g%gi",z->re,z->im);
  else
    printf("%g+%gi",z->re,z->im);
}

void SumaCompleja(COMPLEJO *z1, COMPLEJO *z2)
{
  z1->re=z1->re+z2->re;
  z1->im=z1->im+z2->im;
}
void ProductoComplejo(COMPLEJO *result, COMPLEJO *z1, COMPLEJO *z2)
{ 
  result-> re=(z1->re*z2->re)-(z1->im*z2->im);
  result-> im=(z1->re*z2->im)+(z1->im*z2->re);
}

void LiberarMatrizCompleja(MATRIZ_COMPLEJA *M){
  free(M->data);
  free(M);
}

MATRIZ_COMPLEJA* ProductoMatrizCompleja(MATRIZ_COMPLEJA *A, MATRIZ_COMPLEJA *B){
  MATRIZ_COMPLEJA *C;

  if(A->N == B->M){
    C = CrearMatrizCompleja(A->M, B->N);
    int multTemp = 0;
    COMPLEJO aux;

    /* //aplicar mroducto de la matriz AxB
    Mi metodo
      for (int i = 0; i < C->M; i++){
        for (int j = 0; j < C->N; j++){
        for(int complejos=0; complejos < 1; complejos++){
          if(complejos=0){
            C[i*C->N+j].data[i*C->N+j].re = &A->data[i*C->N+j].re* &B->data[i*C->N+j].re;
          }else {
            C[i*C->N+j].data[i*C->N+j].im = &A->data[i*C->N+j].im* &B->data[i*C->N+j].im;
          }
        }
      }
    } */
    for (int i = 0; i < C->M; i++){
      for (int j = 0; j < C->N; j++){
        C->data[i*C->N+j].re=0;
        C->data[i*C->N+j].im=0;
        for (int k = 0; k < A->N; k++){
          ProductoComplejo(&aux,&A->data[i*A->N+j], &B->data[i*B->N+j]);
          SumaCompleja(&C->data[i*C->N+j], &aux);
        }
      }
    }
    
  } else {
    C=NULL;
    printf("|ERROR| La matriz no A no tiene la misma dimencion que la B");
  }
  return C;
}