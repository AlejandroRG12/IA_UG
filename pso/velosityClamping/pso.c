#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//ESTRUCTURAS DEL PSO
typedef struct { float   *Xi;//Posicion
                 float   *Vi;//Velocidad
                 float   *Pi;//Mejor Posicion Historica
                 float  Xfit; //Valor de Fitnes para la posicion actual
                 float  Pfit; //Valor de Fitnes para la Mejor Posicion
                }PARTICULA;

typedef struct{ PARTICULA    *Part;
                unsigned int NumPart;
                unsigned int NumParam;
                unsigned int IdPg;
                unsigned int NumMaxIter;
                float C1;
                float C2;
                float *LimSup;
                float *LimInf;
                float k; //factor de velocidad
               }ENJAMBRE;

//PARAMETROS DE CONFIGURACION DEL PSO
#define DIM_ 5
const unsigned int NumeroParticulas = 60; //Tamaño del enjambre, numero de particulas del enjambre
const unsigned int Dimension=DIM_; //Numero de variables del problema o dimension del problema.
const float LimiteSuperior[DIM_] = { 20, 20, 20, 20, 20};
const float LimiteInferior[DIM_] = {-20, -20, -20, -20, -20};
const unsigned int NumeroMaximoDeIteraciones = 600;

ENJAMBRE* CrearEnjambre(unsigned int NumPart,unsigned int NumParam);
void InicializarEnjambre(ENJAMBRE *Enj, float k, float C1, float C2, unsigned int NumIterMax,const float *LInf, const float *LSup);
void EliminarEnjambre(ENJAMBRE *Enj);
void ImprimeParticula(ENJAMBRE *Enj, unsigned int Id);
void ImprimeEnjambre(ENJAMBRE *Enj);
void EvualarEnjambre(ENJAMBRE *Enj);
void EvaluacionInicialEnjambre(ENJAMBRE *Enj);
void ActualizarMejoresPosiciones(ENJAMBRE *Enj);
void ActualizarVelocidad(ENJAMBRE *Enj);
void ActualizarPosicion(ENJAMBRE *Enj);
float FuncionObjetivo(float *Xi);

int main(){ 
    ENJAMBRE *Enj;
    unsigned int t=1;
    //Crear un enjambre de NumeroParticulas de Numero de parametros igual a Dimension
    Enj=CrearEnjambre(NumeroParticulas,Dimension);
    printf("\nSe creo el ENJAMBRE");
    InicializarEnjambre(Enj,0.075,2.0,2.0,NumeroMaximoDeIteraciones,LimiteInferior,LimiteSuperior);
    EvaluacionInicialEnjambre(Enj);
    printf("\n\n=============\n Iteracones 0 \n=============");
    ImprimeEnjambre(Enj);
    while (t<Enj->NumMaxIter) <>
    {
        
        printf("\n\n=============\n Iteracones %i \n=============",t);
        ActualizarVelocidad(Enj);
        ActualizarPosicion(Enj);
        EvualarEnjambre(Enj);
        ActualizarMejoresPosiciones(Enj);
        ImprimeEnjambre(Enj);
        t++;
    }

    printf("\n\n|...... BEST PARTICLE ......|");
    ImprimeParticula(Enj, Enj->IdPg);
    printf("\n|...........................|");

    EliminarEnjambre(Enj);
    printf("\nSe elimino el ENJAMBRE");
    return 0;
}

void ActualizarMejoresPosiciones(ENJAMBRE *Enj){
    for(unsigned int i=0; i<Enj->NumPart; i++)
    {
        if(Enj->Part[i].Xfit > Enj->Part[i].Pfit)
        {
            Enj->Part[i].Pfit = Enj->Part[i].Xfit;
            for(unsigned int j=0; j<Enj->NumParam; j++){
                Enj->Part[i].Pi[j] = Enj->Part[i].Xi[j];
            }
        }
    }
}

void EvualarEnjambre(ENJAMBRE *Enj){
    float aux, BestFit;
    //calcular el valor de fitess de cada particula
    BestFit = Enj->Part[Enj->IdPg].Pfit;//receperar el valor del fitess de la mejor partiula de todod el anjambre
    for(int i=0; i<Enj->NumPart; i++)
    {
        Enj->Part[i].Xfit = FuncionObjetivo(Enj->Part[i].Xi);
        //alamacena el mejor indice de la mejor particula de todo el enjambre
        if(Enj->Part[i].Xfit>BestFit)
        {
            BestFit=Enj->Part[i].Xfit;
            Enj->IdPg = i;
        }
    }
}

void ActualizarPosicion(ENJAMBRE *Enj){
    for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
    for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector de la particula i
    { 
        Enj->Part[i].Xi[j] += Enj->Part[i].Vi[j];
    }
}

void ActualizarVelocidad(ENJAMBRE *Enj){
    float Y1, Y2;
    float Vmin, Vmax;
    for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
    for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector de la particula i
    { 
        Vmax = Enj->k * (Enj->LimSup[j]-Enj->LimInf[j]) / 2;
        Vmin = -Vmax; 
        Y1 = (float)rand()/RAND_MAX;
        Y2 = (float)rand()/RAND_MAX;
        //formula de la veloidad
        Enj->Part[i].Vi[j] += (Enj->C1*Y1*(Enj->Part[i].Pi[j] - Enj->Part[i].Xi[j]) + Enj->C2*Y2*(Enj->Part[Enj->IdPg].Pi[j] - Enj->Part[i].Xi[j]));  
        if( Enj->Part[i].Vi[j] > Vmax ){
            Enj->Part[i].Vi[j] = Vmax;
        }
        if( Enj->Part[i].Vi[j] < Vmin ){
            Enj->Part[i].Vi[j] = Vmin;
        }
    }
}

float FuncionObjetivo(float *Xi){
    float fit;
    /*Maximisar la siguiente funcion 
    f(w,x,y,z)=250 -(w+4)^2-(x+1)^2 -(y-3)^2 -(x-5)^2    
    */
    fit = 250 - pow(Xi[0]+7, 2) - pow(Xi[1]+3, 2) - pow(Xi[1]-3, 2) - pow(Xi[1]-5,2)- pow(Xi[1]-8, 2);
    return fit;
}

void EvaluacionInicialEnjambre(ENJAMBRE *Enj){
    float aux, BestFit;
    //calcular el valor de fitess de cada particula
    BestFit = FuncionObjetivo(Enj->Part[0].Xi);
    for(int i=0; i<Enj->NumPart; i++){
        aux = FuncionObjetivo(Enj->Part[i].Xi);
        Enj->Part[i].Xfit=aux;
        Enj->Part[i].Pfit=aux;
        //alamacena el mejor indice de la mejor particula de todo el enjambre
        if(aux>BestFit){
            BestFit=aux;
            Enj->IdPg = i;
        }
    }
}


void ImprimeEnjambre(ENJAMBRE *Enj)
{ for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
      { printf("\n\nParticula[%i]:",i);
        ImprimeParticula(Enj,i);
      }
}
void ImprimeParticula(ENJAMBRE *Enj, unsigned int Id)
    { 
        printf("\n| Xi: ");
        for(unsigned int i=0; i<Enj->NumParam; i++)
            printf("%f, ",Enj->Part[Id].Xi[i]);
        printf("\n| Vi: ");  
        for(unsigned int i=0; i<Enj->NumParam; i++)
            printf("%f, ",Enj->Part[Id].Vi[i]);
        printf("\n| Pi: ");      
        for(unsigned int i=0; i<Enj->NumParam; i++)
            printf("%f, ",Enj->Part[Id].Pi[i]);
        printf("\n| Xfit=%f",Enj->Part[Id].Xfit);
        printf("\n| Pfit=%f",Enj->Part[Id].Pfit);
      }

void InicializarEnjambre(ENJAMBRE *Enj, float k, float C1, float C2, unsigned int NumIterMax,const float *LInf, const float *LSup)
{ 
    float aux,rango;
    Enj->k = k;
    Enj->C1=C1;
    Enj->C2=C2;
    Enj->NumMaxIter=NumIterMax;
    Enj->IdPg=0;
    Enj->LimSup=LSup;
    Enj->LimInf=LInf;
    //Inicializar cada vector de cada particula
    for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
    for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector de la particula i
    { 
        rango=Enj->LimSup[j]-Enj->LimInf[j];
        aux= ((float)rand()/RAND_MAX) * rango + Enj->LimInf[j];
        Enj->Part[i].Xi[j]=aux;
        Enj->Part[i].Vi[j]=0;
        Enj->Part[i].Pi[j]=aux;
    }
}

void EliminarEnjambre(ENJAMBRE* Enj)
{ //Liberar la memoria para de los 3 vectores de cada Particula
  for(unsigned int i=0; i<Enj->NumPart; i++)
     { free(Enj->Part[i].Xi);
       free(Enj->Part[i].Vi);
       free(Enj->Part[i].Pi);
     }
  //Liberar la memoria de las estructuras particula
  free(Enj->Part);
  //Liberar la memoria de la estrcutura del enajmbre
  free(Enj);   
}

ENJAMBRE* CrearEnjambre( unsigned int NumPart,unsigned int NumParam){ 
    ENJAMBRE *ptr;
    //Reservar la memoria para la estructura del enjambre
    ptr=(ENJAMBRE *)malloc(sizeof(ENJAMBRE));
    if(ptr==NULL){ 
        printf("|ERROR| al reservar la memoria para la estructura ENJAMBRE.");
        exit(0);
    }else {
        printf("|CORRECTO| Se reservo la memoria para la estructura ENJAMBRE.");
    }
    ptr->NumPart=NumPart;
    ptr->NumParam=NumParam;

    //Reservar la memoria para N particulas de M parametros
    ptr->Part=(PARTICULA *)malloc(NumPart*sizeof(PARTICULA));
    if(ptr->Part==NULL){
        printf("\n|ERROR| al reservar la memoria para las Particulas.");
        exit(0);
    } else {
        printf("\n|CORRECTO| Se reservo la memoria para las Particulas\n");
    }
    //Reservar memoria para los 3 vectores de cada Particula
    for(unsigned int i=0; i<NumPart; i++){ 
        ptr->Part[i].Xi=(float *)malloc(NumParam*sizeof(float));
        ptr->Part[i].Vi=(float *)malloc(NumParam*sizeof(float));
        ptr->Part[i].Pi=(float *)malloc(NumParam*sizeof(float));
    }  
    return ptr;
}
