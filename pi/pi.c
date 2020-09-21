#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <pthread.h>


struct taylor_args 
{ 
    int ilk; 
    double payda; 
    int itr; 
};

void *taylor(void *arg);
double sum=0;
pthread_mutex_t locker; 

double realpi = 3.141592653589793238462643383279;
int main(int argc, char **argv)
{	
	
	double pi; 
	int n=1; 
	int i;

	clock_t t;
	t = clock();

	int numthr = atoi(argv[1]);
	int itr=atoi(argv[2]);

	pthread_t tHandles[numthr];

	struct taylor_args *args;
    	pthread_mutex_init(&locker, NULL);
	
	for(i=0;i<numthr;i++)
	{
		args = malloc(sizeof(struct taylor_args)); //her thread icin yeni struct

		(*args).itr = itr/numthr;
		n=1+(i*(*args).itr); //taylor index i 1 den baslayıp artıyor

		(*args).ilk=(n+1)%2;
		(*args).payda=2*n-1;	
	
		pthread_create(&tHandles[i],NULL,taylor,(void *)args);		
	}
	for ( i = 0; i < numthr; i++ ) 
	{
		pthread_join(tHandles[i], NULL); // bütün threadleri bekle
	}
	pi=sum*4;

	printf("Result = %1.30f \n",pi);

	pthread_mutex_destroy(&locker);	

	printf("Pi = %1.30f \n",realpi);

	printf("Approximation Error = %1.30f \n",realpi-pi);

	t=clock() - t ;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Duration of computation = %1.30f sec\n",time_taken);
}

void *taylor(void *arg)
{
	struct taylor_args *args = (struct taylor_args*) arg; 

	double locsum = 0; int i; double nom=1;
	for(i=(*args).ilk;i<(*args).itr;i++)
	{

		locsum+=pow((-1),i)*(nom/(*args).payda); //taylorları bul sum'a ekle
		(*args).payda+=2;

	}
	pthread_mutex_lock(&locker); 
	sum+=locsum;		       // local degeri globale ekle

	pthread_mutex_unlock(&locker);
	free(arg);                     
}
