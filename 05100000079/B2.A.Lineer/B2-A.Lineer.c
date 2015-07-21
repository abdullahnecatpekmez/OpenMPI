#include <stdio.h>
#include "mpi.h"
//Line topolojisini MPI_Send and MPI_Recv fonksiyonlarını kullanarak  her proses bir sonraki prosese veri gönderiyor
// Veri gönderimi roottan(0) başlıyor.
int main(int argc,char *argv[])
{
    int	MyRank, Numprocs;
    char* messages={"\"Line topolojisi\""};
    int	Source;
    int	Destination;
    int Root = 0;
    MPI_Status status;


    MPI_Init(&argc,&argv);//MPI_Init size bir değer dönmektedir bu “MPI_SUCCESS” olursa kod geri kalan MPI_X fonksiyonlarını kullanabilecektir,
    // eğer bu değer dönmezse MPI_X fonksiyonlarını kullanamazsınız.
    MPI_Comm_size(MPI_COMM_WORLD,&Numprocs);//Bu fonksiyonumuz proses sayısını(Numprocs) bize vermektedir
    MPI_Comm_rank(MPI_COMM_WORLD,&MyRank);//Bu MPI fonksiyonu çağıran prosesi MyRank değişkenine atıyoruz 
   
    if (MyRank == Root) //Rank root mu(0)kotrol ediyor
    {
		Destination     = MyRank + 1;//Hangi prosesse veri gönderilecek belirleniyor(0->1)
		// MPI_COMM_WORLD değişkeni ise çalıştığımız environmenti (çevre) vermekte
			//messages gönderilecek veri adresini gösteriyoruz
			//1 yollanan veri sayisi
			//Veri hangi prosese gönderileceği
			//MPI_INT Datatype parametresi gidecek verinin türünü belirtmektedir (int,float,char,...)
			//Tag parametresi ise 0-32767 arası sayısal bir değer alır ve kullanıcı tarafından yanlış makinelerden mesajlar gelmesini önlemek için 
			//verilebilecek bir tür güvenlik kodudur send ve recive eşit olmalı
			//Status mesajın geldiği kaynağı bize bildirmektedir
		MPI_Send(&messages, 1, MPI_INT, Destination, 0, MPI_COMM_WORLD);//Mesaj Kökten Destinationa gönderiliyor
    }
    else//rank kökten farklı ise
    {     
		if(MyRank<Numprocs-1)//Rank Proses sayısından 1 eksik ise
		{
			Source     = MyRank-1;//Hangi prosesten veri gelmiş belirleniyor.Rankın bir öncesinden

			MPI_Recv(&messages, 1, MPI_INT, Source, 0, MPI_COMM_WORLD, &status);//Source veri hangi prosesten gelmiş
			
			printf("Proces %d received messages %s from process %d\n", MyRank, messages,Source);// sonuçları print ediyoruz 
			Destination     = MyRank+1;//Hangi prosese veri gönderilecek .Rankın bir sonraki prosese.
			MPI_Send(&messages, 1, MPI_INT, Destination, 0, MPI_COMM_WORLD);

		}
		else//En son rankın bir önceki proses veri gönderimi(8 proses için (0-7)  6 prosesten veri alınmış
		{
			Source     = MyRank-1;

			MPI_Recv(&messages, 1, MPI_INT, Source, 0, MPI_COMM_WORLD, &status);

			printf("Proces %d received messages %s from process %d\n", MyRank, messages,Source);// sonuçları print ediyoruz 
		}
    }

    MPI_Finalize();//8 prosess işlemiş oldu.işlem bitti.
	return 0;
}
