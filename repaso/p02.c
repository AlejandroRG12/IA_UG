/*
EJEMPLO DEL USO DE ARREGLOS MEDIANTE ASIGNACION DINAMICA de MEMORIA
*/
#include<stdio.h>
#include<stdlib.h>

int main()
{ int *Mx; //int Mx[10][10]
  unsigned int filas,i,j;
  unsigned int columnas;
  
  printf("\nNumero de filas del arreglo: ");
  scanf("%u",&filas);
  printf("\nNuermo de columnas del arreglo: ");  
  scanf("%u",&columnas);

  //Resevar la memoria para almacenar la Matriz Mx
  Mx=(int*)malloc(filas*columnas*sizeof(int));
  //Verificar el contenido del apuntador 
  if(Mx==NULL)
     { printf("\nError al reservar la memoria para los datos enteros.");
       exit(0);
     }  
    for(i=0; i<filas; i++)
  	for(j=0; j<columnas; j++)
  	    { printf("M[%i][%i]=",i,j);
  	      //scanf("%i",Mx+i*columnas+j);
          scanf("%i",&Mx[i*columnas+j]);
  	    }

  printf("\n La matriz capturada es (usando arreglo normal): ");
   for(i=0; i<filas; i++)
      { printf("\n");
      	for(j=0; j<columnas; j++)
      		printf("%i ",Mx[i*columnas+j]);
      		//printf("%i ",*(Mx+i*columnas+j));
  	       //printf("[%i][%i]=%i ",i,j,*(Mx+i*columnas+j));
  	    }

   free(Mx);
  return 0;
}
/*
int main()
{ int *v;
  unsigned int N,i;

  printf("\nNumero de datos enteros a almacenar en el arreglo: ");
  scanf("%u",&N);

  //Resevar la memoria para almacenar N numeros enteros
  v=(int*)malloc(N*sizeof(int));
  //Verificar el contenido del apuntador 
  if(v==NULL)
     { printf("\nError al reservar la memoria para los datos enteros.");
       exit(0);
     }
  printf("\n La direccion del bloque de %i bytes es %p",N*sizeof(int),v);   
  for(i=0; i<N; i++)
      {  printf("\nv[%i]=",i);
         scanf("%i",&v[i]);
         //scanf("%i",v+i);
      }
   
   printf("\nLos datos capturados son usando arreglo: ");
   for(i=0; i<N; i++)
   	    printf("\nv[%i]=%i",i,v[i]);
      //printf("\ndireccion:%p v[%i]=%i",v+i,i,*(v+i));

   //Liberar la Memoria Reservada	
   free(v);

  return 0;	
}*/