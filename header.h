#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef char string[100];
typedef struct tChild* addressChild;
typedef struct tParent* addressParent;

typedef struct{
	string nomorProperti;
	string jenis; // (tanah / rumah)
	string alamat;
	int harga;
}DataChild;

typedef struct{
	string NIP;
	string nama;
	string nomorTelpon;
	int totalPenjualan;
}DataParent;

typedef struct tChild{
	DataChild dataChild;
	addressChild nextChild;
}NodeChild;

typedef struct tParent{
	DataParent dataParent;
	addressParent nextParent;
	addressChild firstChild;
}NodeParent;

typedef struct{
	addressParent first;
}ListParent;

void err(char *s);
void suc(char *s);
void showMenu(); // menu utama
void menuSales();
void menuProperti();
void menuLaporan(); // laporan owner

void inisialisasi(ListParent *L);
bool isEmpty(ListParent L);

// FUNGSI PARENT
bool haveChild(addressParent L); // cek punya properti
addressParent alokasiParent(DataParent dataParent); // alokasi data sales
addressParent findParent(ListParent L, string nipSales);
void insertFirstParent(ListParent *L, DataParent dataParent);

// FUNGSI CHILD
