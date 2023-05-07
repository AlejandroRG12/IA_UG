//activa o desactiva esta opcion para impimir los individuos en caso de no hacerlo solo se mostrara el resultado
#define PrintPoblacion 1 // Mostrar = 1, No mostrar = 0

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

typedef struct {
    INDIVIDUO *pob;
    unsigned int PobSize;
    unsigned int NdGens;
    const unsigned int *NdBxGen;
    unsigned int CromSize;
    const float *LimSup;
    const float *LimInf;
}AGS;

AGS InicializarAGS(unsigned int pobSize, unsigned int NumeroDeGenes, const unsigned int *NumeroDeBitPorGen, const float *LimiteSuperior, const float *LimiteInferior);


INDIVIDUO* Inicializar();
void ShowIndividuo(AGS *pAG, unsigned int Id);
void ShowPoblacion(AGS *pAG);
void DecodeEntero(AGS *p);
void ShowEntero(INDIVIDUO *p, unsigned int NdGens_);
/* void DecodeEntero(INDIVIDUO *p);
void ShowDecodeEntero(INDIVIDUO *p);
void DecodeReal(INDIVIDUO *p); */



int main(){
    AGS ag;

    ag = InicializarAGS(
        10,
        NumeroDeGenes, 
        NumeroDeBitPorGen,
        LimiteSuperior,
        LimiteInferior
    );

    DecodeEntero(&ag);
    //mostrar la poblacion (si no entindes esta opcion ve a la linea 1)
    if(PrintPoblacion == 1){
        ShowPoblacion(&ag);
    }

    return 0;
}

AGS InicializarAGS(unsigned int pobSize, unsigned int NumeroDeGenes, const unsigned int *NumeroDeBitPorGen, const float *LimiteSuperior, const float *LimiteInferior){
    AGS ag;
    unsigned int aux = 0, k;
    ag.PobSize= pobSize;
    //Reservar memmoria para una estructura individuo
    ag.pob = (INDIVIDUO *)malloc(pobSize*sizeof(INDIVIDUO));
    if(ag.pob == NULL){
        printf("Error al reservar la memoria para el individuo :c");
        exit(0);
    }

    ag.NdGens = NumeroDeGenes;
    ag.NdBxGen = NumeroDeBitPorGen;
    ag.LimSup = LimiteSuperior;
    ag.LimInf = LimiteInferior;

    //reservar memoria para la decodificacion a entero de la poblacion
    for(k=0; k<pobSize; k++){
        ag.pob[k].ValE =(int *)malloc(sizeof(int)*ag.NdGens);
        if(ag.pob[k].ValE == NULL){
            printf("Error al reservar la memoria para la decodificacion entero :c1 ");
            exit(0);
        }
    }

    for(k=0; k<pobSize; k++){
        ag.pob[k].ValR =(float *)malloc(ag.NdGens*sizeof(float)*ag.NdGens);
        if(ag.pob[k].ValR == NULL){
            printf("Error al reservar la memoria para la decodificacion real :c 2");
            exit(0);
        }
    }

    //calcular el tamaño bits del cromosoma
    for(k = 0; k < ag.NdGens; k++)
        aux += ag.NdBxGen[k];

    ag.CromSize = aux;
    for(k = 0; k < pobSize; k++){
        ag.pob[k].Cromosoma = (Byte *)malloc(ag.CromSize*sizeof(Byte));
        if(ag.pob[k].Cromosoma == NULL){
            printf("Error al reservar la memoria para el Cromosoma :c");
            exit(0);
        }
    }

    for(k=0; k<pobSize; k++)
    for(int j=0; j<ag.CromSize; j++)
        ag.pob[k].Cromosoma[j] = rand()%2;

    return ag;
}

void ShowPoblacion(AGS *pAG){
    unsigned int k;
    for(k=0; k<pAG->PobSize; k++){
        printf("Individuo %d\n", k);
        ShowIndividuo(pAG, k);
        printf("\n");
    }
}

void ShowIndividuo(AGS *pAG, unsigned int Id){

     int k, acumulador, j;
    acumulador = pAG->CromSize - pAG->NdBxGen[pAG->NdGens - 1];
    j = pAG->NdGens - 1;
    printf("\n");
    for(k = pAG->CromSize-1; k >= 0; k--){
        if(k==acumulador - 1){
            printf(":");
            j--;
            acumulador -= pAG->NdBxGen[j];
        }
        printf("%d ",pAG->pob[Id].Cromosoma[k]);
    }
    ShowEntero(pAG->pob, pAG->NdGens);
}

void DecodeEntero(AGS *p){
    int j,i,k,Inicio =0;
    for(j=0;j<p->PobSize;j++){
        for(i=0; i<p->NdGens; i++){
            p->pob[j].ValE[i] = 0;
            for(k = Inicio; k < (p->NdBxGen[i]+ Inicio); k++){
                p->pob[j].ValE[i] += pow(2,k-Inicio) * p->pob[j].Cromosoma[k];
            }
            Inicio += p->NdBxGen[i];
        }
    }
}

void ShowEntero(INDIVIDUO *p, unsigned int NdGens_){
    int k;
    printf(" - ");
    for(k=NdGens_-1; k>=0; k--)
        printf("%d ", p->ValE[k]);
}


/* void DecodeEntero(INDIVIDUO *p){
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
 */