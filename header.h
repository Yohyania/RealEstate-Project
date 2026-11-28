#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

typedef char string[100];
typedef struct tChild *addressChild;
typedef struct tParent *addressParent;

typedef struct {
    string nomorProperti;
    string jenis; // (tanah / rumah)
    string alamat;
    int harga;
} DataChild;

typedef struct {
    string NIP;
    string nama;
    string nomorTelpon;
    int totalPenjualan;
} DataParent;

typedef struct tChild {
    DataChild dataChild;
    addressChild nextChild;
} NodeChild;

typedef struct tParent {
    DataParent dataParent;
    addressParent nextParent;
    addressChild firstChild;
} NodeParent;

typedef struct {
    addressParent first;
} ListParent;

/* Riwayat penjualan: menyimpan properti yang sudah dijual */
typedef struct {
    string nomorProperti;
    string jenis;
    string alamat;
    int harga;
    string nipSales;
    string namaSales;
} DataTransaksi;

typedef struct tTransaksi *addressTransaksi;

typedef struct tTransaksi {
    DataTransaksi dataTransaksi;
    addressTransaksi nextTransaksi;
} NodeTransaksi;

typedef struct {
    addressTransaksi first;
} ListTransaksi;

//////////////////////Fungsi Tampilan////////////////////////////
void err(char *s);   // Menampilkan pesan error (merah)
void suc(char *s);   // Menampilkan pesan berhasil (hijau)
void showMenu();     // Menampilkan menu utama
void menuSales();    // Menampilkan menu sales
void menuProperti(); // Menampilkan menu properti
void menuLaporan();  // Menampilkan menu laporan owner

//////////////////////Fungsi List////////////////////////////////
void inisialisasi(ListParent *L);             // Menginisialisasikan LIST
void inisialisasiTransaksi(ListTransaksi *T); // Menginisialisasikan riwayat transaksi
bool isEmpty(ListParent L);                   // Mengecek apakah List kosong

//////////////////////Fungsi Parent//////////////////////////////
bool haveChild(addressParent sales);                                           // Mengecek apakah sales memiliki properti
addressParent alokasiParent(DataParent dataBaru);                              // Mengalokasikan data sales (konstruktor)
addressParent findParent(ListParent L, string nipSales);                       // Mencari data sales berdasarkan NIP
int countParent(ListParent L);                                                 // Menghitung jumlah sales
void insertFirstParent(ListParent *L, DataParent dataBaru);                    // Menambah sales di awal list
void insertLastParent(ListParent *L, DataParent dataBaru);                     // Menambah sales di akhir list
void insertAfterParent(ListParent *L, string nipSebelum, DataParent dataBaru); // Menambah sales setelah sales tertentu
void updateParent(ListParent *L, string nipSales, DataParent dataBaru);        // Mengubah data sales
void deleteFirstParent(ListParent *L);                                         // Menghapus sales pertama
void deleteLastParent(ListParent *L);                                          // Menghapus sales terakhir
void deleteAtParent(ListParent *L, string nipSales);                           // Menghapus sales berdasarkan NIP
void printParent(addressParent sales);                                         // Menampilkan satu data sales
void printAllParent(ListParent L);                                             // Menampilkan semua data sales

//////////////////////Fungsi Child///////////////////////////////
addressChild alokasiChild(DataChild dataChild);
addressChild findChild(addressChild C, string nomorProperti);
void insertFirstChild(ListParent *L, string nipSales, DataChild dataBaru);
void insertLastChild(ListParent *L, string nipSales, DataChild dataBaru);
void insertAtChild(ListParent *L, string nipSales, DataChild dataBaru, int input);
void updateChild(ListParent *L, string nipSales, string nomorProperti, DataChild dataBaru);
void deleteFirstChild(ListParent *L, string nipsSales);
void deleteLastChild(ListParent *L, string nipSales);
void deleteAtChild(ListParent *L, string nipSales, string nomorProperti);
void tampilProperti(addressChild C);
void printSalesDanProperti(ListParent L);

////////////////////// Laporan Owner & Transaksi ///////////////////////////////
void insertTransaksi(ListTransaksi *T, DataTransaksi dataBaru); // Menambah riwayat penjualan
void jualProperti(ListParent *L, string nipSales, string nomorProperti, ListTransaksi *T);
void pindahProperti(ListParent *L, string nipAsal, string nipTujuan);
void laporanNamaSales(ListParent L);
void laporanBelumTerjual(ListParent L, string nipSales, string jenis);
void laporanJumlahJenis(ListParent L);
void laporanPropertiTerjual(ListTransaksi T);

// arrow key
int menuPanah(char *judul, char *item[], int jumlah);