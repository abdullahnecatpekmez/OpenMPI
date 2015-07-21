#include <stdio.h>
#include "mpi.h"
#define MAXPROC 8    /* Max number of procsses */
#define NAMELEN 80   /* Max length of machine name */
#define LENGTH 24    /* Lengt of send buffer is divisible by 2, 4, 6 and 8 */
//Scatter ve Gather dizi bütün proseslere dagıtılıp tekrar Masterda toplatılıyor
int toplam(int* d, int s)
{
	int i;
	int toplam=0;
	for(i=0;i<s;i++)
		toplam+=d[i];
	
	return toplam;
}
int main(int argc, char* argv[])
{
	int i, np, rank;
	const int root = 0;         /* Root process in scatter */
	int anaToplam;
	int araToplam[MAXPROC];
	char myname[NAMELEN];             /* Local host name string */
	int x[LENGTH];        /* Send buffer */
	int y[LENGTH];        /* Receive buffer */

	MPI_Init(&argc, &argv);                /* Initialize MPI */
	MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Proses sayisi*/
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);    /*Hangi proses çalışıyorsa world_rank değişkenine set ediyoruz */
	gethostname(&myname, NAMELEN);    /* Get host name (Ubuntu) */

	if (rank == 0)//Master Proses (0)için işlemler
	{    

		/* x dizisine deger atanıyor 0 .. LENGTH-1 */
		for (i = 0; i<LENGTH; i++)
		{
			x[i] = i;
			printf("%d-",i);
		}
		/* Check that we have an even number of processes and at most MAXPROC */
		
		printf("\nProcess %d on host %s is distributing array x to all %d processes\n\n", rank, myname, np);

		/* Scatter ile Rootan diziyi bölerek(LENGTH / np) y dizisine gönderiyor */
		MPI_Scatter(&x, LENGTH / np, MPI_INT, &y, LENGTH / np, MPI_INT, root, MPI_COMM_WORLD);
		anaToplam=toplam(y,LENGTH / np);//Master prosesteki diziyi toplatıyor
		//Master prosesteki dizinin toplamını Roota  araToplam dizisinde topluyor
		MPI_Gather(&anaToplam, 1, MPI_INT, araToplam, 1, MPI_INT, root, MPI_COMM_WORLD);
		/* Print out own portion of the scattered array */
		anaToplam=toplam(araToplam,np);//Ara toplam dizisinin elemenlarını topluyor
		
		printf("Dizinin\n");
		printf("Toplam değeri: %d",anaToplam);//Masterda bölünen dizini toplamı
		

	}
	else 
	{ /* Master dışındaki prosesler için işlemler*/

		//Scatter ile Masterda dizi bölünerek (LENGTH / np)tüm proseslere dagıtılıyor
		MPI_Scatter(&x, LENGTH / np, MPI_INT, &y, LENGTH / np, MPI_INT, root, MPI_COMM_WORLD);
		anaToplam=toplam(y,LENGTH / np);//Her prosesteki diziyi toplatıyor
		//Scatter ile Masterda proseslere gönderilen her dizinin toplamını (LENGTH / np) araToplam dizisinde masterda topluyor
		MPI_Gather(&anaToplam, 1, MPI_INT, araToplam, 1, MPI_INT, root, MPI_COMM_WORLD);
		/* Print out own portion of the scattered array */
		anaToplam=toplam(araToplam,np);//Her Prsesin araToplam dizisini topluyor değişkene atıyor

	}

	MPI_Finalize();//İşlemi sonlandır
	return 0;
}
