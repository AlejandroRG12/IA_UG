//Algoritmo Genetico Simple
#include<stdio.h>
#include<stdlib.h>

#define NumeroDeGenes_ 3
const unsigned int NumeroDeGenes = NumeroDeGenes_;
const unsigned int NumeroDeBitPorGen[NumeroDeGenes_] = {8, 8, 8};
const float LimiteSuperior[NumeroDeGenes_] = {1, 1, 1};
const float LimiteInferior[NumeroDeGenes_] = {-1, -1, -1};

typedef unsigned char Byte;

typedef struct{
        Byte *Cromosoma;
        unsigned int NdGens;
        const unsigned int *NdBxGen;
        unsigned int CromSize;
        int *ValE;
        float *ValR;
        const float *LimSup;
        const float *LimInf;
}INDIVIDUO;

INDIVIDUO* Inicializar();
void DecodeEntero(INDIVIDUO *p);
void MuestraIndividuo(INDIVIDUO *p);

int main(){
    INDIVIDUO *p;

    p = Inicializar();
    MuestraIndividuo(p);

    return 0;
}

void DecodeEntero(INDIVIDUO *p){

}

void MuestraIndividuo(INDIVIDUO *p){
    unsigned int k;

    printf("\n");
    for(k = 0; k < p->CromSize; k++)
        printf("%d", p->Cromosoma[k]);
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
