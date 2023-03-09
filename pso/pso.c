#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    float *Xi;  //Posicion
    float *Vi;  //Velocidad
    float *Pi;  //Mejor Posicion
    float fit; //Valor de l fitnes
}PARTUCLA;

typedef struct 
{
    PARTUCLA *Part;
    unsigned int size;
    unsigned int IdPg; 

}ENJAMBRE;

const unsigned int NumeroDeParticulas = 10;     //tamaño del enjambre , numero de particulas.
const unsigned int Dimencion = 2;               //Numero de variables del prombelma o dimencionalidad del problema.
const float LimiteSuperior[2] = { 10, 10};
const float LimiteInferior[2] = {-10,-10};
const unsigned int NumeroMaximoDeIteracions = 100;

ENJAMBRE* CrearEnjambre(unsigned int NumPart, unsigned int dim);

int main(){
    ENJAMBRE *Enj;

    Enj = CrearEnjambre(NumeroDeParticulas, Dimencion);
    
    return 0;
}

ENJAMBRE* CrearEnjambre(unsigned int NumPart, unsigned int dim){
    ENJAMBRE *ptr;

    ptr=(ENJAMBRE *)malloc(sizeof(ENJAMBRE));                                   //reservar la memoria para la estructura del enjambre
    if(ptr==NULL){                                                              //comprobrar si se reservo la memoria dinamica
        printf("\n|ERROR| al resevar la memoria para la estructura ENJAMBRE.");
        exit(0);                                                                //en cado de que no este termina el proceso
    } else { printf("\n|CORECTO| memoria asignada para la estructura ENJAMBRE.");}
    ptr->size = NumPart;
    ptr->IdPg = 0;
    ptr->Part = (PARTUCLA *)malloc(NumPart*sizeof(PARTUCLA));                //reservar la meoria para N particulas de dimencion Dim
    if(ptr->Part == NULL){
        printf("\n|ERROR| al resevar la memoria para la estructura PARTICULA dentro de ENJAMBRE.");
        exit(0);
    } else { printf("\n|CORECTO| memoria asignada para la estructura PARTICULA dentro de ENJAMBRE.");}
    return ptr;
}