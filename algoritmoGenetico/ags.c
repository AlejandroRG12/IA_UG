//Algoritmo Genetico Simple
#include<stdio.h>
#include<stdlib.h>
#include <math.h>

#define NumeroDeGenes_ 3
const unsigned int NumeroDeGenes = NumeroDeGenes_;
const unsigned int NumeroDeBitPorGen[NumeroDeGenes_] = {8, 8, 8};
const float LimiteSuperior[NumeroDeGenes_] = {1, 1, 1};
const float LimiteInferior[NumeroDeGenes_] = {-1, -1, -1};

typedef unsigned char Byte;

typedef struct{
        Byte                *Cromosoma;
        unsigned int        NdGens;
        const unsigned int  *NdBxGen;
        unsigned int        CromSize;
        int                 *ValE;
        float               *ValR;
        const float         *LimSup;
        const float         *LimInf;
}INDIVIDUO;

INDIVIDUO* Inicializar();
void DecodeEntero(INDIVIDUO *p);
void ShowIndividuo(INDIVIDUO *p);
void ShowDecodeEntero(INDIVIDUO *p);
void DecodeReal(INDIVIDUO *p);

int main(){
    INDIVIDUO *p;

    p = Inicializar();
    //DecodeEntero(p);
    DecodeReal(p);
    ShowIndividuo(p);
    ShowDecodeEntero(p);

    return 0;
}

void DecodeEntero(INDIVIDUO *p){
    int inicio = 0;
    for(int i=0; i<p->NdGens; i++){
        p->ValE[i]=0;
        for(int k = inicio; k < (p->NdBxGen[i]+ inicio); k++){
            p->ValE[i] = pow(2,k-inicio) * p->Cromosoma[k];
        }
        inicio += p->NdBxGen[i];
    }
        
}

void DecodeReal(INDIVIDUO *p){
    float rango;
    int i,k,Inicio =0;

    for(i=0; i<p->NdGens; i++){
        p->ValE[i] = 0;
         for(int k = Inicio; k < (p->NdBxGen[i]+ Inicio); k++){
            p->ValE[i] = pow(2,k-Inicio) * p->Cromosoma[k];
        }
        Inicio += p->NdBxGen[i];
    }
    for(i=0; i<p->NdGens; i++){
        rango = p->LimSup[i] - p->LimInf[i];
        p->ValR[i] = (p->ValE[i] / (pow(2, p->NdBxGen[i]) - 1)) * rango + p->LimInf[i];
    }
}

void ShowDecodeEntero(INDIVIDUO *p){
    int k;
    printf(" - ");
    for(k=p->NdGens-1; k>=0; k--){
        printf("%i, ", p->ValE[k]);
    }
}

void ShowIndividuo(INDIVIDUO *p){
     int k, acumulador, j;
    acumulador = p->CromSize - p->NdBxGen[p->NdGens - 1];
    j = p->NdGens - 1;
    printf("\n");
    for(k = p->CromSize-1; k >= 0; k--){
        if(k==acumulador - 1){
            printf(":");
            j--;
            acumulador -= p->NdBxGen[j];
        }
        printf("%d ",p->Cromosoma[k]);
    }
}

INDIVIDUO* Inicializar(){
    INDIVIDUO *pI;
    unsigned int aux = 0, k;

    //Reservar memmoria para una estructura individuo
    pI = (INDIVIDUO *)malloc(sizeof(INDIVIDUO));
    if(pI == NULL){
        printf("Error al reservar la memoria para el individuo :c");
        exit(0);
    }

    pI->NdGens = NumeroDeGenes;
    pI->NdBxGen = NumeroDeBitPorGen;

    pI->LimSup = LimiteSuperior;
    pI->LimInf = LimiteInferior;


    pI->ValE = (int *)malloc(sizeof(int)*pI->NdGens);
    if(pI->ValE == NULL){
        printf("Error al reservar la memoria para la decodificacion entero :c");
        exit(0);
    }

    pI->ValR = (float *)malloc(sizeof(float)*pI->NdGens);
    if(pI->ValR == NULL){
        printf("Error al reservar la memoria para la decodificacion real :c");
        exit(0);
    }

    for(k = 0; k < pI->NdGens; k++)
        aux += pI->NdBxGen[k];

    pI->CromSize = aux;
    pI->Cromosoma = (Byte *)malloc(pI->CromSize*sizeof(Byte));
    if(pI->Cromosoma == NULL){
        printf("Error al reservar la memoria para el Cromosoma :c");
        exit(0);
    }

    for(k = 0; k <pI->CromSize; k++)
        pI->Cromosoma[k] = rand()%2;

    return pI;
}
