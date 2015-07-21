
// Ring topolojisini MPI_Send and MPI_Recv  fonksiyonlarını kullanarak oluşturuyor.
//Roottan başlıyor roota geri dönüyor.Döngüsel gibi oluyor.
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
   //MPI_Init size bir değer dönmektedir bu “MPI_SUCCESS” olursa kod geri kalan MPI_X fonksiyonlarını kullanabilecektir,
    // eğer bu değer dönmezse MPI_X fonksiyonlarını kullanamazsınız.
  MPI_Init(NULL, NULL);//world_rank 0
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);//Hangi proses çalışıyorsa world_rank değişkenine set ediyoruz
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);//process sayisini world_size tutuyoruz

  int token;// veri
  
  if (world_rank != 0) {//world_rank default 0 veya -1 olarak set edilir.default 0 dır.
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,//ekrana basma
           world_rank - 1);
  } else {
    // prosess 0 ise token -1 atıyoruz
    token = -1;
  }
  MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,//Başa dönsün diye proses sayisina göre mod alıyoruz 
           MPI_COMM_WORLD);
 
  if (world_rank == 0) {//(0=>7) Son prosess ilk prosese veri gönderiyor.Dairesel gibi oluyor.
    MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_size - 1);
  }
  MPI_Finalize();//İşlemi sonlandır.
  return 0;
}
