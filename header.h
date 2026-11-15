#ifndef HEADER_H
#define HEADER_H

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

//////////////////////Fungsi Tampilan////////////////////////////
void err(char *s);   // Menampilkan pesan error (merah)
void suc(char *s);   // Menampilkan pesan berhasil (hijau)
void showMenu();     // Menampilkan menu utama
void menuSales();    // Menampilkan menu sales
void menuProperti(); // Menampilkan menu properti
void menuLaporan();  // Menampilkan menu laporan owner

//////////////////////Fungsi List////////////////////////////////
void inisialisasi(ListParent *L); // Menginisialisasikan LIST
bool isEmpty(ListParent L);       // Mengecek apakah List kosong

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
bool haveChild(addressParent P);                                               // untuk menegcek apakah sales mempunyai properti
addressChild alokasiChild (DataChild dataChild);                                // untuk mengalokasikan data properti
addressChild findChild(addressChild C, string nomorProperti);                    // untuk mencari data properti
void insertFirstChild(addressChild *C, DataChild dataBaru);                       // untuk menambah awal properti
void insertLastChild(addressChild *C, DataChild databaru);                        // untuk menambah akhir properti
void insertAtChild(addressChild *C, DataChild dataBaru, int input);               // untuk menambah properti setelah properti tertentu
void updateChild(addressChild *C, string nomorProperti ,DataChild dataBaru);      // untuk update data properti
void deleteFirtsChild(addressChild *C);                                           // untuk hapus properti pertama 
void deleteLastChild(addressChild *C);                                            // untuk hapus properti akhir
void deleteAtChild(addressChild *C, string nomorProperti);                        //untuk hapus properti berdasarkan nomor properti
void tampilProperti(addressChild C);                                              // untuk menampilkan semua properti

#endif
