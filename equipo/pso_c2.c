//pso contrincted
//con valores comendados
//C1 = C2 = 2.05
//X = 0.72984

// 
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
//#include<omp.h>

typedef struct{
	double *Xi; //Posicion
	double *Vi; //Velocidad
	double *Pi; //Mejor posicion
	double Xfit; //Valor de fitnes para la posicion actual
	double Pfit; //Valor de fitnes para la mejor posicion
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
	float X;
	double *Px;
	double *Py;
	int n;
	int m;
}ENJAMBRE;


#define DIM_ 3

//Tamaño del enjambre
const unsigned int NParticulas = 100;
//Numero de variables del problema, 
const unsigned int Dimension = DIM_;
const float limSup[DIM_] = {  1.0,  1.2, 1.0};
const float limInf[DIM_] = { -1.0,  -0.2, 0.0};
const unsigned int IteracionesMaximas = 1000;
const unsigned int NoFunciones = 20;

ENJAMBRE* CrearEnjambre(unsigned int NumPart, unsigned int NumParam);
void InicializarEnjambre(ENJAMBRE* enj, float X, float C1, float C2, int m, unsigned int NumIterMax, const float *LInf,  const float *LSup);
void EliminarEnjambre(ENJAMBRE* enj);
void imprimeParticula(ENJAMBRE* enj, unsigned int Id);
void imprimeEnjambre(ENJAMBRE* enj);
void EvaluarEnjambre(ENJAMBRE* enj);
void EvaluarEnjambreIni(ENJAMBRE* enj);
double FuncionObjetivo(double* Xi, double* Px, double* Py, int m, int n);
void ActualizarVelocidad(ENJAMBRE *enj);
void ActualizarPosicion(ENJAMBRE *enj);
void ActualizarMejoresPosiciones(ENJAMBRE* enj);

void ObtenerPuntos(ENJAMBRE* enj, char* rutaX, char* rutaY);

int bestIt = -1;

int main(){
	//omp_set_nested(0);
    //omp_set_num_threads(24);
	time_t tx;
	srand((unsigned) time(&tx));
	int it = 0;
	ENJAMBRE *Enj;
	Enj =  CrearEnjambre(NParticulas, Dimension*NoFunciones);
	ObtenerPuntos(Enj, "funcion1/xi.txt", "funcion1/yi.txt");
	InicializarEnjambre(Enj, 0.72984, 2.05, 2.05, NoFunciones, IteracionesMaximas, &limInf[0], &limSup[0]);
	//imprimeEnjambre(Enj);
	//printf("\n");
	EvaluarEnjambreIni(Enj);
	
	while(it < Enj->iterMax){
		ActualizarVelocidad(Enj);
		ActualizarPosicion(Enj);
		EvaluarEnjambre(Enj);
		ActualizarMejoresPosiciones(Enj);
		//imprimeEnjambre(Enj);
		//printf("\nIteracion %d\n",it);
		//imprimeParticula(Enj, Enj->IdPg);

		if(bestIt == -1)
			bestIt = it;
		it++;
	}

	imprimeParticula(Enj, Enj->IdPg);
	printf("\nBestIt: %d\n", bestIt);
	EliminarEnjambre(Enj);
	return 0;

}

void ObtenerPuntos(ENJAMBRE* enj, char* rutaX, char* rutaY){
	FILE *FileX;
	FILE *FileY;
    double aux;
	enj->n = 0;

	FileX = fopen(rutaX, "r");
	FileY = fopen(rutaY, "r");

	if(FileX == NULL || FileY == NULL){
		printf("No se pudo abrir el archivo.\n");
        exit(0);
	}

	while (fscanf(FileX, "%lf", &aux) == 1 && fscanf(FileY, "%lf", &aux) == 1) {
        enj->n++;
    }

	enj->Px = (double *)malloc((enj->n) * sizeof(double));
	enj->Py = (double *)malloc((enj->n) * sizeof(double));

	if(enj->Px == NULL){
		printf("Error al reservar memoria Px\n");
		exit(0);
	}
	if(enj->Py == NULL){
		printf("Error al reservar memoria Py\n");
		exit(0);
	}

	fseek(FileX, 0, SEEK_SET);
	fseek(FileY, 0, SEEK_SET);

	for(int i=0; i<enj->n; i++){
		fscanf(FileX, "%lf", &enj->Px[i]);
		fscanf(FileY, "%lf", &enj->Py[i]);
	}
	fclose(FileX);
	fclose(FileY);


}

void ActualizarMejoresPosiciones(ENJAMBRE* enj){
	//#pragma omp parallel for
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
	//#pragma omp parallel for
	for(unsigned int i=0; i<enj->NumPart; i++)
		for(unsigned int j=0; j<enj->NumParam; j++){
			enj->Part[i].Xi[j] += enj->Part[i].Vi[j];
		} 
}

void ActualizarVelocidad(ENJAMBRE *enj){
	double Y1, Y2;
	//#pragma omp parallel for
	for(unsigned int i=0; i<enj->NumPart; i++)
		for(unsigned int j=0; j<enj->NumParam; j++){
			Y1 = (double)rand()/RAND_MAX;
			Y2 = (double)rand()/RAND_MAX;
			enj->Part[i].Vi[j] = enj->X * ( (double)(enj->Part[i].Vi[j]) +
								 		    (enj->C1 * Y1 * (enj->Part[i].Pi[j] - enj->Part[i].Xi[j])) + 
								            (enj->C2 * Y2 *(enj->Part[enj->IdPg].Pi[j] - enj->Part[i].Xi[j])) );
			
		}
}

double FuncionObjetivo(double* Xi, double* Px, double* Py, int m, int n){
    
	double fit=0, rbf=0;
	
	for(int i=0; i<n; i++){
        for(int k=0; k<m; k++){
            rbf += Xi[3*k] * exp(-pow(Px[i]-Xi[(3*k)+1], 2)/pow(Xi[(3*k)+2], 2));
        }
        fit += pow(rbf-Py[i], 2);
		rbf=0;
    }
	return (double)-fit/n;
}

void EvaluarEnjambre(ENJAMBRE* enj){
	//calcular el valor de fitnes de cada particula
	double BestFit = enj->Part[enj->IdPg].Pfit;
	
	//#pragma omp parallel for
	for(unsigned int i=0; i<enj->NumPart; i++){
		enj->Part[i].Xfit = FuncionObjetivo(enj->Part[i].Xi, enj->Px, enj->Py, enj->m, enj->n);

		if(enj->Part[i].Xfit > BestFit){
			BestFit = enj->Part[i].Xfit;
			enj->IdPg = i;
			bestIt = -1;
		}
	}
}

void EvaluarEnjambreIni(ENJAMBRE* enj){
	//calcular el valor de fitnes de cada particula
	double aux;
	double BestFit = FuncionObjetivo(enj->Part[0].Xi, enj->Px, enj->Py, enj->m, enj->n);
	
	//#pragma omp parallel for
	for(unsigned int i=0; i<enj->NumPart; i++){
		aux = FuncionObjetivo(enj->Part[i].Xi, enj->Px, enj->Py, enj->m, enj->n);
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
		printf("%3.6lf, ", enj->Part[Id].Xi[i]);
	}
	printf(" }\n\tVi: { ");
	for(unsigned int i = 0; i < enj->NumParam; i++){
		printf("%3.6lf, ", enj->Part[Id].Vi[i]);
	}
	int separador = 0;
	printf(" }\n\tPi = np.array([[ ");
	for(unsigned int i = 0; i < enj->NumParam; i++){
		printf("%3.17lf", enj->Part[Id].Pi[i]);
		if(separador < 2)
			printf(", ");
		else
			printf("], [");
			
		separador++;
		if(separador==3)
			separador=0;
	}
	printf(" ])\n\tXfit = %lf", enj->Part[Id].Xfit);
	printf("\n\tPfit = %lf", enj->Part[Id].Pfit);
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

void InicializarEnjambre(ENJAMBRE* enj, float X, float C1, float C2, int m, unsigned int NumIterMax, const float *LInf,  const float *LSup){
	double aux;
	enj->C1 = C1;
	enj->C2 = C2;
	enj->iterMax = NumIterMax;
	enj->IdPg = 0;
	enj->limInf = LInf;
	enj->limSup = LSup;
	enj->X = X;
	enj->m = m;

	//Inicializar cada vector de cada particula
	int limId = 0;
	for(unsigned int i=0; i<enj->NumPart; i++){
		for(unsigned int j=0; j<enj->NumParam; j++){
			aux = ((double)rand()/RAND_MAX) * (enj->limSup[limId] - enj->limInf[limId]) + enj->limInf[limId];
			limId++;
			enj->Part[i].Xi[j] = aux;
			enj->Part[i].Vi[j] = 0;
			enj->Part[i].Pi[j] = aux;
			if(limId>=3)
				limId = 0;
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
	free(enj->Px);
	free(enj->Py);
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
		ptr->Part[i].Xi = (double *) malloc(NumParam * sizeof(double));
		ptr->Part[i].Vi = (double *) malloc(NumParam * sizeof(double));
		ptr->Part[i].Pi = (double *) malloc(NumParam * sizeof(double));

		if(ptr->Part[i].Xi == NULL || ptr->Part[i].Xi == NULL || ptr->Part[i].Xi == NULL){
			printf("Error al asignar la memoria para los vectores");
			EliminarEnjambre(ptr);
			exit(0);
		}
	}

	return ptr;
}

