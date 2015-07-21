#include "mpi.h"
#include <stdio.h>
#include <math.h>
#define MAXSIZE 100

int main(int argc, char **argv)
{
	int rank, numprocs;
	int data[MAXSIZE], i, x, low, high, myresult=0, result;
	

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(0 == rank) {//Root 0 ise  diziye deger atanıyor
	
		for(i=0; i<MAXSIZE; i++) {
			data[i]=i+1;
		}
	}

	/* Datayı roottan proseslere gönderiliyor*/
	MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);
	
	/* add portion of data */
	x = MAXSIZE/numprocs;	/* Dizi proceslere bölüştürülüyor*/
	low = rank * x;//Proceslerin dizide alt ve üst sınırları bulunuyor
	high = low + x;
	for(i=low; i<high; i++) {//Her processin kendi işlemlerini toplatıp değişkene atıyor
		myresult += data[i];
	}
	printf("I got %d from %d\n", myresult, rank);

	/* N tane makinede oluşmuş sonuçları derleyerek ana proseste result  saklamaktadır. */
	/* N tane makinede oluşmuş sonuçları derleyerek ana proseste result  saklamaktadır. */
	MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(0 == rank) {
		printf("The sum is %d.\n", result);
	}

	MPI_Finalize();
	return 0;
}
	
