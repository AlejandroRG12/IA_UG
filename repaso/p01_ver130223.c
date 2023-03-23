#include<stdio.h>

typedef struct{ float re;
                float im;  
              }COMPLEJO;

typedef struct{ COMPLEJO data[10][10];
                unsigned int M;//Numero de filas
                unsigned int N;//Numero de columnas
                }MATRIZ_COMPLEJA;

COMPLEJO SumaCompleja(COMPLEJO *z1, COMPLEJO *z2);
COMPLEJO ProductoComplejo(COMPLEJO *z1, COMPLEJO *z2);
void ImprimeComplejo(COMPLEJO *z);
void CapturaMatrizCompleja(MATRIZ_COMPLEJA *C);
void ImprimeMatrizCompleja(MATRIZ_COMPLEJA *C);

int main()
{ MATRIZ_COMPLEJA A; 
  printf("Numero de filas de la matriz A: ");
  scanf("%u",&A.M);
  printf("Numero de columnas de la matriz A: ");
  scanf("%u",&A.N);
  CapturaMatrizCompleja(&A);
  printf("\nLa matriz A:\n");
  //ImprimeMatrizCompleja(&A);
 return 0;
}
void CapturaMatrizCompleja(MATRIZ_COMPLEJA *C)
{ int i,j;
  for(i=0; i<C->M; i++)
  for(j=0; j<C->N; j++)
     { printf("[%i][%i].re=",i,j);
       scanf("%f",&C->data[i][j].re);
       printf("[%i][%i].im=",i,j);
       scanf("%f",&C->data[i][j].im);
     }
}
/*
int main()
{ COMPLEJO W1,W2,Z;
  W1.re=5;
  W1.im=-3;
  W2.re=1;
  W2.im=2;
  printf("\nW1=");
  ImprimeComplejo(&W1);
  printf("\nW2=");
  ImprimeComplejo(&W2);
  Z=SumaCompleja(&W1,&W2);
  printf("\nW1+W2=");
  ImprimeComplejo(&Z);
  Z=ProductoComplejo(&W1,&W2);
  printf("\nW1*W2=");
  ImprimeComplejo(&Z);
 return 0;
}*/

void ImprimeComplejo(COMPLEJO *z)
{ if(z->im<0)
	printf("%g%gi",z->re,z->im);
  else
    printf("%g+%gi",z->re,z->im);
}

COMPLEJO SumaCompleja(COMPLEJO *z1, COMPLEJO *z2)
{ COMPLEJO w;
  w.re=z1->re+z2->re;
  w.im=z1->im+z2->im;
  return w;
}
COMPLEJO ProductoComplejo(COMPLEJO *z1, COMPLEJO *z2)
{ COMPLEJO w;
  w.re=(z1->re*z2->re)-(z1->im*z2->im);
  w.im=(z1->re*z2->im)+(z1->im*z2->re);
  return w;
}











