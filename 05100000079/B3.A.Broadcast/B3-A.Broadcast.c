#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define LENGTH 16   //Dizi uzunluğu
/*Broadcast ile bir diziyi bütün proseslere gönderilip sonra dizi proseslere eşit bölüp her prosesteki işlemleri
 * toplayıp aratplam yaratıp bütün aratoplamları  masterda toplatmak.Dizinin toplamı bulunmuş oldu*/
int main(int argc, char* argv[])
{
	int i;
	int rank, size;   
	const int root = 0;         
	int pSize;
	int x[LENGTH];       
	int toplam=0;
	int araToplam;
	char* myname={"Abdullah"};
	MPI_Init(&argc, &argv);                /* Initialize MPI */
	MPI_Comm_size(MPI_COMM_WORLD, &size);   // Bu fonksiyonumuz proses sayısını bize vermektedir
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);    //Bu MPI fonksiyonu çağıran prosesin sırasını vermektedir
	pSize=LENGTH/size;//Diziyi proseslere paylaştırıyoruz
	if (rank == root)
	{   
		/* Diziye deger atıyoruz. */
		for (i = 0; i<LENGTH; i++)
		{
			x[i] = i;
			printf("%d-",x[i]);//Dizini elemanlarını ekrana basıyoruz
		}
		MPI_Bcast(&x,LENGTH,MPI_INT,root,MPI_COMM_WORLD);//Root(Master)olarak sayılan prosesten arabellekteki (buffer) x dizisini
		 //  “MPI_COMM_WORLD ortamında ki bütün proseslere yollamaktadır.length veri sayisi
		
		printf("\nProcess %d on host %s is distributing array x to all %d processes\n\n", rank, myname, size);
		araToplam=0;
		for(i=pSize*rank;i<pSize*(rank+1);i++)//master prosesin yaptığı işlemin aralığını toplatıyor.
			araToplam+=x[i];
		toplam+=araToplam;	//master prosesin işlemini tek bir yerde topluyor.
		for(i=1;i<size;i++)
		{
			
			MPI_Recv(&araToplam, 1, MPI_INT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);//Masterda bütün proseslerdeki
			toplam+=araToplam;                                                      //yapılan işlemleri topluyor
		}
		/* Dizinin toplamını ekrana basıyor */
		printf("total: %d",toplam);
	}
	else 
	{ //else Master dışındaki proseslerin sonuçlarını toplatıyor
		//x dizisi bütün proseslere gönderiliyor rootan
		MPI_Bcast(&x,LENGTH,MPI_INT,root,MPI_COMM_WORLD);
		araToplam=0;
		for(i=pSize*rank;i<pSize*(rank+1);i++)//rank=1 için i=2;i<2*2 arası 
		{
			araToplam+=x[i];
		}
		printf("process %d subtotal %d\n",rank,araToplam);//Her prosesin hesapdığı değeri yazdırıyor
		MPI_Send(&araToplam, 1, MPI_INT, root, 0, MPI_COMM_WORLD);//her  prosesteki yapılan işi mastera gönderiyor.
	}

	MPI_Finalize();//Bütün işlemlerimizden sonra MPI_Finalize diyerek işlemlerimizi sonlandırıyoruz.
	return 0;
}

