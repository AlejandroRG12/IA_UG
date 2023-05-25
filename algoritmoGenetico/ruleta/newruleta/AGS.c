/*
Algoritmo Genetico Simple
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define NumGenes_ 2
const unsigned int PoblacionSize = 10;
const int NumGenes = NumGenes_;
const int NumBitPorGen[NumGenes_] = {8, 8};
const float limsSup[NumGenes_] = { 10, 10};
const float limsInf[NumGenes_] = {-10, -10};
const float ProbCruza_ = 0.8;
const float ProbMuta_ = 0.001;
const unsigned int NumMaxGeneraciones = 3;

typedef unsigned char BYTE;
typedef enum{MAX, MIN}TIPO_OPT;
typedef struct{
	BYTE *Cromosoma;
	int *ValE;
	float *ValR;
	float VObj;
	float Fit;
}INDIVIDUO;

typedef struct{
	INDIVIDUO* Pob;
	INDIVIDUO* NewPob;
	unsigned int PobSize;
	unsigned int NdGens;
	unsigned int CromSize;
	const unsigned int *NdBxGens;
	const float *limSup;
	const float *limInf;
	unsigned int IdBest;
	unsigned int IdWorst;
	unsigned int* Seleccion;
	float TotalFitnes;
	float PromFit;
	float ProbCruza;
	float ProbMuta;

}AGS;

AGS InicializarAGS(unsigned int PobSize, unsigned int NoGens, const unsigned int* BpG, const float *Lim_inf, const float *Lim_sup, const float ProbCruza, const float ProbMuta, unsigned int NumMaxGeneraciones);
void EliminarAGS(AGS ag);
void DecodeEntero(AGS* p);
void DecodeReal(AGS* p);
void ShowPob(AGS *pAG);
void ShowIndividuo(AGS *pAG, unsigned int id);
void ShowEntero(INDIVIDUO* p, unsigned int NdGens);
void ShowReal(INDIVIDUO* p, unsigned int NdGens);

void EvaluarPoblacion(AGS * pAG);
float FuncionObjetivo(float *Vr);
void Obj2Fit(AGS* pAG, TIPO_OPT tipo);
void SeleccionRuleta(AGS* pAG);
void Cruza1P(AGS *pAG);
void MutacionBit(AGS *pAG);

void NextGenerations(AGS *pAG);

int main(){
	time_t tx;
	srand((unsigned) time(&tx));
	AGS ga = InicializarAGS(PoblacionSize, NumGenes, NumBitPorGen, limsInf, limsSup, ProbCruza_, ProbMuta_, NumMaxGeneraciones);
	DecodeReal(&ga);
	DecodeEntero(&ga);
	EvaluarPoblacion(&ga);
	Obj2Fit(&ga, MAX);
	

	ShowPob(&ga);
	SeleccionRuleta(&ga);
	Cruza1P(&ga);
	MutacionBit(&ga);
	NextGenerations(&ga);
	EvaluarPoblacion(&ga);
	EliminarAGS(ga);
	printf("\n");

	return 0;
}

void NextGenerations(AGS *pAG){
	INDIVIDUO *aux;
	aux = pAG->Pob;
	pAG->Pob = pAG->NewPob;
	pAG->NewPob = aux;
}

void MutacionBit(AGS *pAG){
	for(int i=0; i<pAG->PobSize; i++){
		for(int j=0; j<pAG->CromSize; j++){
			if((float)rand()/RAND_MAX < pAG->ProbMuta){
				/*printf("Muto individuo %i en bit %i\n", i, j);
				for(int k=0; k<pAG->CromSize; k++){
					printf("%i", pAG->NewPob[i].Cromosoma[k]);
				}
				printf("\n");*/
				pAG->NewPob[i].Cromosoma[j] = !pAG->NewPob[i].Cromosoma[j];
				/*for(int k=0; k<pAG->CromSize; k++){
					printf("%i", pAG->NewPob[i].Cromosoma[k]);
				}
				printf("\n");*/
			}
		}
	}
}

void Cruza1P(AGS *pAG){
	int PuntoDeCruza, aux;
	for(int i=0; i<pAG->PobSize; i+=2){
		if(pAG->ProbCruza > (float)rand()/RAND_MAX){
			PuntoDeCruza = (rand()%pAG->CromSize);
			for(int k=0; k<PuntoDeCruza; k++){
				pAG->NewPob[i].Cromosoma[k] = pAG->Pob[pAG->Seleccion[i+1]].Cromosoma[k];
				pAG->NewPob[i+1].Cromosoma[k] = pAG->Pob[pAG->Seleccion[i]].Cromosoma[k];
			}
			for(int k=PuntoDeCruza; k<pAG->CromSize; k++){
				pAG->NewPob[i].Cromosoma[k] = pAG->Pob[pAG->Seleccion[i]].Cromosoma[k];
				pAG->NewPob[i+1].Cromosoma[k] = pAG->Pob[pAG->Seleccion[i+1]].Cromosoma[k];
			}
		}
		else{
			//printf("\nNo se cruzo");
			for(int k=0; k<pAG->CromSize; k++){
				pAG->NewPob[i].Cromosoma[k] = pAG->Pob[pAG->Seleccion[i]].Cromosoma[k];
				pAG->NewPob[i+1].Cromosoma[k] = pAG->Pob[pAG->Seleccion[i+1]].Cromosoma[k];
			}
		}
		/*
		printf("\nPunto de cruza: %i", PuntoDeCruza);
		printf("\nPadre(i=%i, %i): ", i, pAG->Seleccion[i]);
		for(int k=0; k<pAG->CromSize;k++){
			printf("%i", pAG->Pob[pAG->Seleccion[i]].Cromosoma[k]);
		}
		printf("\nPadre(i=%i, %i): ", i+1, pAG->Seleccion[i+1]);
		for(int k=0; k<pAG->CromSize;k++){
			printf("%i", pAG->Pob[pAG->Seleccion[i+1]].Cromosoma[k]);
		}
		printf("\nHijo(%i): ",i);
		for(int k=0; k<pAG->CromSize;k++){
			printf("%i", pAG->NewPob[i].Cromosoma[k]);
		}
		printf("\nHijo(%i): ",i+1);
		for(int k=0; k<pAG->CromSize;k++){
			printf("%i", pAG->NewPob[i+1].Cromosoma[k]);
		}
		printf("\n");
		*/
	}
}

void SeleccionRuleta(AGS* pAG){
	float Ruleta[pAG->PobSize];
	float pelota, suma=0;
	for(int i=0; i<pAG->PobSize; i++){
		Ruleta[i] = pAG->Pob[i].Fit/pAG->TotalFitnes;
		//printf("Ruleta[%d]: %3.2f%\n", i, Ruleta[i]*100);
	}
	for(int k=0; k<pAG->PobSize; k++){
		pelota = (float)rand()/RAND_MAX;
		//printf("pelota: %3.2f%\n", 100*pelota);
		for(int i=0; i<pAG->PobSize; i++){
			suma+=Ruleta[i];
			if(suma>=pelota){
				pAG->Seleccion[k] = i;
				suma = 0;
				break;
			}
		}
		//printf("Individuo Seleccionado: %d\n", pAG->Seleccion[k]);
	}
}

void Obj2Fit(AGS *pAG, TIPO_OPT tipo){
	float min = pAG->Pob[pAG->IdWorst].VObj;
	float max = pAG->Pob[pAG->IdBest].VObj - min;
	pAG->TotalFitnes = 0;
	if(tipo == MAX){
		for(int i=0; i<pAG->PobSize; i++){
			pAG->Pob[i].Fit = 100*((pAG->Pob[i].VObj - min)/max);
			pAG->TotalFitnes += pAG->Pob[i].Fit;
			if(pAG->Pob[i].Fit >= pAG->Pob[pAG->IdBest].Fit)
				pAG->IdBest = i;
			if(pAG->Pob[i].Fit <= pAG->Pob[pAG->IdWorst].Fit)
				pAG->IdWorst = i;
		}
	} else if(tipo == MIN){
		for(int i=0; i<pAG->PobSize; i++){
			pAG->Pob[i].Fit = -100*((pAG->Pob[i].VObj - min)/max)+100;
			pAG->TotalFitnes += pAG->Pob[i].Fit;
			if(pAG->Pob[i].Fit >= pAG->Pob[pAG->IdBest].Fit)
				pAG->IdBest = i;
			if(pAG->Pob[i].Fit <= pAG->Pob[pAG->IdWorst].Fit)
				pAG->IdWorst = i;
		}
	}
	pAG->PromFit = pAG->TotalFitnes/pAG->PobSize;
}

void EvaluarPoblacion(AGS * pAG){
	pAG->IdBest=0;
	pAG->IdWorst=0;
	pAG->Pob[0].VObj=FuncionObjetivo(&pAG->Pob[0].ValR[0]);
	for(int k=0; k<pAG->PobSize; k++){
		pAG->Pob[k].VObj = FuncionObjetivo(&pAG->Pob[k].ValR[0]);
		if(pAG->Pob[k].VObj>pAG->Pob[pAG->IdBest].VObj)
			pAG->IdBest=k;
		if(pAG->Pob[k].VObj<pAG->Pob[pAG->IdWorst].VObj)
			pAG->IdWorst=k;
	}
}

float FuncionObjetivo(float *Vr){
	float obj;
	obj = 50 - pow(Vr[0]+5,2)-pow(Vr[1]+5,2);
	return obj;
}

void ShowReal(INDIVIDUO* p, unsigned int NdGens){
    printf(" - ");
    for ( int k = NdGens-1; k >=0 ; k--)
    {
        printf("%f,", p->ValR[k]);
    }   
}

void DecodeReal(AGS *p){
    float rango;
    int ini = 0;
    // i es el indice para recorrer todos los genes de un individuo
        // k es el indice ára recorrer todos los bits de un gen
	for(int i=0; i<p->PobSize; i++){
		ini = 0;
		for(int j = 0; j<p->NdGens; j++){
			p->Pob[i].ValE[j] = 0;
			for (int k = ini; k < (p->NdBxGens[j]+ini); k++){
				p->Pob[i].ValE[j] += pow(2, (k-ini)) * p->Pob[i].Cromosoma[k];
			}
			ini += p->NdBxGens[j];
		}
		for (int j = 0; j < p->NdGens; j++){
			rango=p->limSup[j] - p->limInf[j];
			p->Pob[i].ValR[j]=p->Pob[i].ValE[j]/(pow(2,p->NdBxGens[j])-1) * rango+p->limInf[j];
		}
	}
    
}

void DecodeEntero(AGS *p){
	int ini;
    for (int i = 0; i < p->PobSize; i++){
		ini = 0;
		for(int j = 0; j<p->NdGens; j++){
			p->Pob[i].ValE[j] = 0;
			for (int k = ini; k < (p->NdBxGens[j]+ini); k++){
				p->Pob[i].ValE[j] += pow(2, (k-ini)) * p->Pob[i].Cromosoma[k];
			}
			ini += p->NdBxGens[j];
		}
    }
}

void ShowEntero(INDIVIDUO* p, unsigned int NdGens){
    printf(" - ");
    for ( int k = NdGens-1; k >=0 ; k--){
        printf("%i,", p->ValE[k]);
    }
}

void ShowPob(AGS *pAG){
	printf("\n");
	for(int i = 0; i<pAG->PobSize; i++){
		printf("   [%d]->",i);
		ShowIndividuo(pAG, i);
	}
	printf("   TotalFitnes: %f\n", pAG->TotalFitnes);
	printf("   PromFit: %f\n", pAG->PromFit);
	printf("   IdBest: %d || IdWorst: %d\n\n", pAG->IdBest, pAG->IdWorst);
}

void ShowIndividuo(AGS *pAG, unsigned int id){
	printf("[");
	int gen = pAG->NdGens-1, genspan=0;
	for(int i = 0; i<pAG->NdGens-1; i++)
		genspan += pAG->NdBxGens[i];
	genspan--;
	for(int i=pAG->CromSize-1; i>=0; i--){
		if(i==genspan){
			printf(":");
			if(gen<pAG->NdGens){
				gen--;
				genspan -= pAG->NdBxGens[gen];
			}
		}
		printf("%i", pAG->Pob[id].Cromosoma[i]);
	}
	printf("]  ");
	ShowEntero(&pAG->Pob[id], pAG->NdGens);	
	ShowReal(&pAG->Pob[id], pAG->NdGens);
	printf(" VObj: %f", pAG->Pob[id].VObj);
	printf(" Fit: %f", pAG->Pob[id].Fit);
	printf("\n");

}

AGS InicializarAGS(unsigned int PobSize, unsigned int NoGens, const unsigned int* BpG, const float *Lim_inf, const float *Lim_sup, const float ProbCruza, const float ProbMuta, unsigned int NumMaxGeneraciones){
	AGS ag;
	unsigned int aux=0;
	ag.PobSize = PobSize;
	//reservar memoria para la poblacion de individuos
	ag.Pob = (INDIVIDUO*)malloc(ag.PobSize * sizeof(INDIVIDUO));
	if(ag.Pob == NULL){
		printf("Error al reservar memoria para los individuos\n");
		exit(0);
	}
	ag.NewPob = (INDIVIDUO*)malloc(ag.PobSize * sizeof(INDIVIDUO));
	if(ag.Pob == NULL){
		printf("Error al reservar memoria para los nuevos individuos\n");
		exit(0);
	}
	ag.NdGens = NoGens;
	ag.NdBxGens = BpG;
	ag.limInf = Lim_inf;
	ag.limSup = Lim_sup;
	ag.ProbCruza = ProbCruza;
	ag.ProbMuta = ProbMuta;
	//recorriendo la poblacion
	for(int i=0; i<ag.PobSize; i++){
		//Reservar memoria para la decodificacion a entero de cada individuo
		ag.Pob[i].ValE = (int*)malloc(ag.NdGens * sizeof(int));
		if(ag.Pob[i].ValE == NULL){
			printf("Error al reservar memoria para ValE del individuo [%d]\n",i);
			exit(0);
		}
		//Reservar memoria para la decodificacion a real de cada individuo
		ag.Pob[i].ValR = (float*)malloc(ag.NdGens * sizeof(float));
		if(ag.Pob[i].ValR == NULL){
			printf("Error al reservar memoria para ValR del individuo [%d]\n",i);
			exit(0);
		}
		//Reservar memoria para la decodificacion a entero de cada nuevo individuo
		ag.NewPob[i].ValE = (int*)malloc(ag.NdGens * sizeof(int));
		if(ag.NewPob[i].ValE == NULL){
			printf("Error al reservar memoria para ValE del nuevo individuo [%d]\n",i);
			exit(0);
		}
		//Reservar memoria para la decodificacion a real de cada nuevo individuo
		ag.NewPob[i].ValR = (float*)malloc(ag.NdGens * sizeof(float));
		if(ag.NewPob[i].ValR == NULL){
			printf("Error al reservar memoria para ValR del nuevo individuo [%d]\n",i);
			exit(0);
		}
	}
	//Calcular el tamaño en bits del cromosoma
	for(int i=0; i<ag.NdGens; i++){
		aux += ag.NdBxGens[i];
	}
	ag.CromSize = aux;
	//reservar memoria para el cromosoma de cada individuo
	for(int i=0; i<PobSize; i++){
		ag.Pob[i].Cromosoma = (BYTE*)malloc(ag.CromSize * sizeof(BYTE));
		if (ag.Pob[i].Cromosoma == NULL){
			printf("Error al reservar memoria para el cromosoma del individuo [%d]\n",i);
			exit(0);
		}
		//inicializar el cromosoma de cada inviduo
		for(int k=0; k<ag.CromSize; k++){
			ag.Pob[i].Cromosoma[k] = rand()%2;
		}

		//reservar memorial para el cromosoma de cada nuevo individuo
		ag.NewPob[i].Cromosoma = (BYTE*)malloc(ag.CromSize * sizeof(BYTE));
		if (ag.NewPob[i].Cromosoma == NULL){
			printf("Error al reservar memoria para el cromosoma del nuevo individuo [%d]\n",i);
			exit(0);
		}
	}
	for(int i = 0; i<ag.PobSize; i++)
		ag.Pob[i].Fit = 0;
	
	//reservar memoria para el listado de individuos seleccionados
	ag.Seleccion = (unsigned int *)malloc(ag.PobSize*sizeof(unsigned int));
	if(ag.Seleccion==NULL){
		printf("Error al reservar memoria para la seleccion\n");
		exit(0);
	}
	
	return ag;
}

void EliminarAGS(AGS ag){
	for(int i=0; i<ag.PobSize; i++){
		free(ag.Pob[i].Cromosoma);
		free(ag.Pob[i].ValE);
		free(ag.Pob[i].ValR);
		free(ag.NewPob[i].Cromosoma);
		free(ag.NewPob[i].ValE);
		free(ag.NewPob[i].ValR);
	}
	free(ag.Seleccion);
	free(ag.Pob);
	free(ag.NewPob);
}