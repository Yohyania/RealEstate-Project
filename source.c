#include "header.h"

void err(char *s){
	printf("\033[1;31m");
	printf("\n\t  [!] %s [!]\n", s);
	printf("\033[0m");
}

void suc(char *s){
	printf("\033[1;32m");
	printf("\n\t  [*] %s [*]\n", s);
	printf("\033[0m");
}

void showMenu(){
	system("cls");
	printf("\033[0;34m");
    printf("\n\t============================================================");
  	printf("\033[0m");
	printf("\n\t     SISTEM PENGELOLAAN INVENTARIS DAN PENJUALAN PROPERTI   ");
	printf("\033[0;34m");
	printf("\n\t============================================================");
	printf("\033[0m");
	printf("\n\t\t[1] Sales");
	printf("\n\t\t[2] Properti");
	printf("\n\t\t[3] JualProperti");
	printf("\n\t\t[4] Laporan Owner");
	printf("\n\t\t[0] Exit");
	printf("\n\t\t>>> ");
}

void menuSales(){
	system("cls");
	printf("\n==========================");
	printf("\n\t     MENU SALES   ");
	printf("\n\t========================");
	printf("\n\t[1] Tambah Sales");
	printf("\n\t[2] Ubah Sales");
	printf("\n\t[3] Hapus Sales");
	printf("\n\t[0] Kembali");
	printf("\n\t>>>");
}

void menuProperti(){
	system("cls");
	printf("\n==========================");
	printf("\n\t     MENU PROPERTI   ");
	printf("\n\t========================");
	printf("\n\t[1] Tambah Properti");
	printf("\n\t[2] Ubah Properti");
	printf("\n\t[3] Hapus Properti");
	printf("\n\t[0] Kembali");
	printf("\n\t>>>");
}

void menuLaporan(){
	system("cls");
	printf("\n==========================");
	printf("\n\t     MENU LAPORAN   ");
	printf("\n\t========================");
	printf("\n\t[1] Daftar Nama Sales");
	printf("\n\t[2] Properti Belum Terjual per Jenis"); // sales tertentu
	printf("\n\t[3] Jumlah Properti per Jenis");
	printf("\n\t[0] Kembali");
	printf("\n\t>>>");
}

///////////////// FUNGSI PARENT /////////////////
void inisialisasi(ListParent *L){
	L->first = NULL;
}

bool isEmpty(ListParent L){
	if(L.first == NULL) return 1;
	else return 0;
}

addressParent alokasiParent(DataParent dataParent){
	addressParent P;
	P = (addressParent)malloc(sizeof(NodeParent));
	P->dataParent = dataParent;
	P->nextParent = NULL;
	P->firstChild = NULL;
	return P;
}

addressParent findParent(ListParent L, string nipSales){
	ListParent temp;
	
	if(isEmpty(L)) return NULL;
	if(strcmp(L.first->dataParent.NIP, nipSales) == 0) return L.first;
	temp.first = L.first->nextParent;
	return findParent(temp, nipSales);
}

void insertFirstParent(ListParent *L, DataParent dataBaru){
	addressParent P;
	P = alokasiParent(dataBaru);
	P->nextParent = L->first;
	L->first = P;
	
	suc("Data Sales Berhasil Dimasukkan");
}
