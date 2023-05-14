#include<stdio.h>
#include <math.h>
#include<stdlib.h>
#include<time.h>

typedef struct{
	float *Xi; //Posicion
	float *Vi; //Velocidad
	float *Pi; //Mejor posicion
	float Xfit; //Valor de fitnes para la posicion actual
	float Pfit; //Valor de fitnes para la mejor posicion
}PARTICULA;

typedef struct{
	PARTICULA *Part;
	unsigned int NumPart;
	unsigned int NumParam;
	unsigned int IdPg;
	unsigned int iterMax;
	float C1;
	float C2;
	const float *limSup;
	const float *limInf;
	float X;//factor de velocidad
}ENJAMBRE;


#define DIM_ 2

//Tamaño del enjambre
const unsigned int NParticulas = 30;
//Numero de variables del problema, 
const unsigned int Dimension = DIM_;
const float limSup[DIM_] = { 5.12,  5.12};
const float limInf[DIM_] = {-5.12, -5.12};
const unsigned int IteracionesMaximas = 200;

ENJAMBRE* CrearEnjambre(unsigned int NumPart, unsigned int NumParam);
void InicializarEnjambre(ENJAMBRE* enj, float X, float C1, float C2, unsigned int NumIterMax, const float *LInf, const float *LSup);
void EliminarEnjambre(ENJAMBRE* enj);
void imprimeParticula(ENJAMBRE* enj, unsigned int Id);
void imprimeEnjambre(ENJAMBRE* enj);
void EvaluarEnjambre(ENJAMBRE* enj);
void EvaluarEnjambreIni(ENJAMBRE* enj);
float FuncionObjetivo(float* Xi , unsigned int Np);
void ActualizarVelocidad(ENJAMBRE *enj);
void ActualizarPosicion(ENJAMBRE *enj);
void ActualizarMejoresPosiciones(ENJAMBRE* enj);

int bestIt = -1;

int main(){
	time_t tx;
	srand((unsigned) time(&tx));
	int it = 0;
	ENJAMBRE *Enj;
	Enj =  CrearEnjambre(NParticulas, Dimension);
	InicializarEnjambre(Enj, 0.72984, 2.5, 2.5, IteracionesMaximas, limInf, limSup);
	//imprimeEnjambre(Enj);
	//printf("\n");
	EvaluarEnjambreIni(Enj);
	
	while(it < Enj->iterMax){
		ActualizarVelocidad(Enj);
		ActualizarPosicion(Enj);
		EvaluarEnjambre(Enj);
		ActualizarMejoresPosiciones(Enj);
		//imprimeEnjambre(Enj);
		if(bestIt == -1)
			bestIt = it;
		it++;
	}

	imprimeParticula(Enj, Enj->IdPg);

	printf("\nBestIt: %d\n", bestIt);
	EliminarEnjambre(Enj);
	return 0;
}

void ActualizarMejoresPosiciones(ENJAMBRE* enj){
	for(unsigned int i=0; i < enj->NumPart; i++){
		if(enj->Part[i].Xfit > enj->Part[i].Pfit){
			enj->Part[i].Pfit = enj->Part[i].Xfit;
			for(unsigned int j=0; j<enj->NumParam; j++){
				enj->Part[i].Pi[j] = enj->Part[i].Xi[j];
			}
		}
	}
}

void ActualizarPosicion(ENJAMBRE *enj){
	for(unsigned int i=0; i<enj->NumPart; i++)
		for(unsigned int j=0; j<enj->NumParam; j++){
			enj->Part[i].Xi[j] += enj->Part[i].Vi[j];
		} 
}

void ActualizarVelocidad(ENJAMBRE *enj){
	float Y1, Y2;
	for(unsigned int i=0; i<enj->NumPart; i++)
		for(unsigned int j=0; j<enj->NumParam; j++){
			Y1 = (float)rand()/RAND_MAX;
			Y2 = (float)rand()/RAND_MAX;
			enj->Part[i].Vi[j] = enj->X * ((enj->Part[i].Vi[j]) +
								 (enj->C1 * Y1 * (enj->Part[i].Pi[j] - enj->Part[i].Xi[j])) + 
								 (enj->C2 * Y2 *(enj->Part[enj->IdPg].Pi[j] - enj->Part[i].Xi[j])));
			
		}
}

float FuncionObjetivo(float* Xi , unsigned int Np){
	float fit, aux;
	//Maximizar la siguiente funcion
	//f(w,x,y,z) = 250 - (w + 4)^2 - (x+1)^2 - (y-3)^2 - (z-5)^2
    aux = 0;
    for(int k=0; k < Np; k++){
        aux = pow(Xi[k],2) - 10 * cos(6.2831853072*Xi[k]) + 10;
    }
    fit = 100 - aux;
	return fit;
}

void EvaluarEnjambre(ENJAMBRE* enj){
	//calcular el valor de fitnes de cada particula
	float BestFit = enj->Part[enj->IdPg].Pfit;
	for(unsigned int i=0; i<enj->NumPart; i++){
		enj->Part[i].Xfit = FuncionObjetivo(enj->Part[i].Xi, enj->NumParam);

		if(enj->Part[i].Xfit > BestFit){
			BestFit = enj->Part[i].Xfit;
			enj->IdPg = i;
			bestIt = -1;
		}
	}
}

void EvaluarEnjambreIni(ENJAMBRE* enj){
	//calcular el valor de fitnes de cada particula
	float aux;
	float BestFit = FuncionObjetivo(enj->Part[0].Xi, enj->NumParam);
	for(unsigned int i=0; i<enj->NumPart; i++){
		aux = FuncionObjetivo(enj->Part[i].Xi, enj->NumParam);
		enj->Part[i].Xfit = aux;
		enj->Part[i].Pfit = aux;


		if(aux > BestFit){
			BestFit = aux;
			enj->IdPg = i;
		}
	}
}

void imprimeParticula(ENJAMBRE* enj, unsigned int Id){
	printf("\n\tXi: { ");
	for(unsigned int i = 0; i < enj->NumParam; i++){
		printf("%f", enj->Part[Id].Xi[i]);
		if(i < enj->NumPart-1)
			printf(", " );
	}
	printf(" }\n\tVi: { ");
	for(unsigned int i = 0; i < enj->NumParam; i++){
		printf("%f", enj->Part[Id].Vi[i]);
		if(i < enj->NumPart-1)
			printf(", " );
	}
	printf(" }\n\tPi: { ");
	for(unsigned int i = 0; i < enj->NumParam; i++){
		printf("%f", enj->Part[Id].Pi[i]);
		if(i < enj->NumPart-1)
			printf(", " );
	}
	printf(" }\n\tXfit = %f", enj->Part[Id].Xfit);
	printf("\n\tPfit = %f", enj->Part[Id].Pfit);
}

void imprimeEnjambre(ENJAMBRE* enj){
	for(unsigned int i = 0; i < enj->NumPart; i++){
		printf("\nParticula[%d]:", i);
		imprimeParticula(enj, i);
	}
	printf("\nBest Particle: \n");
	printf(">Particula[%d]:", enj->IdPg);
	imprimeParticula(enj, enj->IdPg);
}

void InicializarEnjambre(ENJAMBRE* enj, float X, float C1, float C2, unsigned int NumIterMax, const float *LInf, const float *LSup){
	float aux;
	enj->C1 = C1;
	enj->C2 = C2;
	enj->iterMax = NumIterMax;
	enj->IdPg = 0;
	enj->limInf = LInf;
	enj->limSup = LSup;
	enj->X = X;

	//Inicializar cada vector de cada particula
	for(unsigned int i=0; i<enj->NumPart; i++){
		for(unsigned int j=0; j<enj->NumParam; j++){
			aux = ((float)rand()/RAND_MAX) * (enj->limSup[j] - enj->limInf[j]) + enj->limInf[j];
			enj->Part[i].Xi[j] = aux;
			enj->Part[i].Vi[j] = 0;
			enj->Part[i].Pi[j] = aux;
		}
	}
}

void EliminarEnjambre(ENJAMBRE* enj){
	if(enj == NULL){
		return;
	}
	if(enj->Part == NULL){
		free(enj);
		return;
	}
	for(unsigned int i = 0; i < enj->NumPart; i++){
		if(enj->Part[i].Xi != NULL){
			free(enj->Part[i].Xi);
		}
		if(enj->Part[i].Vi != NULL){
			free(enj->Part[i].Vi);
		}
		if(enj->Part[i].Pi != NULL){
			free(enj->Part[i].Pi);
		}
	}
	free(enj->Part);
	free(enj);
}

ENJAMBRE* CrearEnjambre(unsigned int NumPart, unsigned int NumParam){
	ENJAMBRE *ptr;
	//reservar la memoria para la estructura del enjambre
	ptr = (ENJAMBRE *)malloc( sizeof(ENJAMBRE));
	if(ptr == NULL){
		printf("Error al asignar la memoria para la estructura ENJAMBRE.\n");
		exit(0);
	}
	ptr->NumPart = NumPart;
	ptr->NumParam = NumParam;
	//reservar la memoria para N particulas de M parametros
	ptr->Part = (PARTICULA *)malloc(NumPart*sizeof(PARTICULA));
	if(ptr->Part == NULL){
		printf("Error al asignar la memoria para las particulas");
		EliminarEnjambre(ptr);
		exit(0);
	}
	//reservar memoria para los 3 vectores de cada particula
	for(unsigned int i = 0; i < NumPart; i++){
		ptr->Part[i].Xi = (float *) malloc(NumParam * sizeof(float));
		ptr->Part[i].Vi = (float *) malloc(NumParam * sizeof(float));
		ptr->Part[i].Pi = (float *) malloc(NumParam * sizeof(float));

		if(ptr->Part[i].Xi == NULL || ptr->Part[i].Xi == NULL || ptr->Part[i].Xi == NULL){
			printf("Error al asignar la memoria para los vectores");
			EliminarEnjambre(ptr);
			exit(0);
		}
	}
	return ptr;
}