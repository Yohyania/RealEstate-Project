#include "header.h"

int main(int argc, char *argv[]) {
    ListParent L;
    addressParent salesDicari;
    addressChild propDicari; // properti
    DataParent dataSales;
    DataChild dataProperti, dataPropBaru;
    int menuUtama, pilihanSales, pilihProperti, pilihLaporan;
    string nipBaru, nip;
    string noProp;

    /* Daftar item menu (item TERAKHIR = Exit/Kembali -> dikembalikan sebagai 0) */
    char *itemUtama[]    = {"Sales", "Properti", "Jual Properti", "Laporan Owner", "Exit"};
    char *itemSales[]    = {"Tambah Sales", "Ubah Sales", "Hapus Sales", "Lihat Semua Sales", "Kembali"};
    char *itemProperti[] = {"Tambah Properti", "Ubah Properti", "Hapus Properti", "Kembali"};
    char *itemLaporan[]  = {"Daftar Nama Sales", "Properti Belum Terjual per Jenis",
                            "Jumlah Properti per Jenis", "Kembali"};

    inisialisasi(&L);

    do {
        menuUtama = menuPanah("SISTEM PENGELOLAAN INVENTARIS DAN PENJUALAN PROPERTI", itemUtama, 5);

        switch (menuUtama) {
            case 1: /* MENU SALES (parent) */
                do {
                    pilihanSales = menuPanah("MENU SALES", itemSales, 5);

                    switch (pilihanSales) {
                        case 1: { /* Tambah Sales */
                            bool isValid;

                            printf("\n\t--- TAMBAH SALES ---\n");

                            do {
                                isValid = true;
                                printf("\tNIP           : ");
                                fflush(stdin);
                                gets(dataSales.NIP);
                                if (strcmp(dataSales.NIP, "") == 0) {
                                    err("NIP Tidak Boleh Kosong");
                                    isValid = false;
                                } else if (findParent(L, dataSales.NIP) != NULL) {
                                    err("NIP Sudah Terdaftar");

                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("\tNama          : ");
                                fflush(stdin);
                                gets(dataSales.nama);
                                if (strcmp(dataSales.nama, "") == 0) {
                                    err("Nama Tidak Boleh Kosong");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("\tNomor Telepon : ");
                                fflush(stdin);
                                gets(dataSales.nomorTelpon);
                                if (strcmp(dataSales.nomorTelpon, "") == 0) {
                                    err("Nomor Telepon Tidak Boleh Kosong");
                                    isValid = false;
                                }
                            } while (!isValid);

                            /* Sales baru selalu mulai dari 0, bertambah saat properti terjual */
                            dataSales.totalPenjualan = 0;

                            insertLastParent(&L, dataSales);
                            break;
                        }

                        case 2: { /* Ubah Sales */
                            bool isValid, batal = false;

                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printf("\n\t--- UBAH SALES ---\n");
                            printAllParent(L);
                            printf("\n");

                            do {
                                isValid = true;
                                printf("\tNIP Sales yang diubah (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    batal = true;
                                } else if (findParent(L, nip) == NULL) {
                                    err("Data Sales Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);

                            if (batal) {
                                err("Perubahan Dibatalkan");
                                break;
                            }

                            do {
                                isValid = true;
                                printf("\tNIP Baru (kosongkan jika tetap) : ");
                                fflush(stdin);
                                gets(nipBaru);
                                if (strcmp(nipBaru, "") == 0) {
                                    strcpy(nipBaru, nip);
                                } else if (strcmp(nipBaru, nip) != 0 && findParent(L, nipBaru) != NULL) {
                                    err("NIP Sudah Digunakan Sales Lain");
                                    isValid = false;
                                }
                            } while (!isValid);
                            strcpy(dataSales.NIP, nipBaru);

                            do {
                                isValid = true;
                                printf("\tNama Baru          : ");
                                fflush(stdin);
                                gets(dataSales.nama);
                                if (strcmp(dataSales.nama, "") == 0) {
                                    err("Nama Tidak Boleh Kosong");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("\tNomor Telepon Baru : ");
                                fflush(stdin);
                                gets(dataSales.nomorTelpon);
                                if (strcmp(dataSales.nomorTelpon, "") == 0) {
                                    err("Nomor Telepon Tidak Boleh Kosong");
                                    isValid = false;
                                }
                            } while (!isValid);

                            updateParent(&L, nip, dataSales);
                            break;
                            
                        }

                        case 3: { /* Hapus Sales */
                            bool isValid, batal = false;

                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printf("\n\t--- HAPUS SALES ---\n");
                            printAllParent(L);
                            printf("\n");

                            do {
                                isValid = true;
                                printf("\tNIP Sales yang dihapus (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    batal = true;
                                } else if (findParent(L, nip) == NULL) {
                                    err("Data Sales Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);

                            if (batal) {
                                err("Penghapusan Dibatalkan");
                                break;
                            }

                            if (haveChild(findParent(L, nip))) {
                                if (countParent(L) < 2) {
                                    err("Tidak Ada Sales Pengganti, Hapus Properti Terlebih Dahulu");
                                    break;
                                }

                                do {
                                    isValid = true;
                                    printf("\tNIP Sales pengganti (kosongkan untuk batal) : ");
                                    fflush(stdin);
                                    gets(nipBaru);
                                    if (strcmp(nipBaru, "") == 0) {
                                        batal = true;
                                    } else if (strcmp(nipBaru, nip) == 0) {
                                        err("Sales Pengganti Harus Berbeda");
                                        isValid = false;
                                    } else if (findParent(L, nipBaru) == NULL) {
                                        err("Sales Pengganti Tidak Ditemukan");
                                        isValid = false;
                                    }
                                } while (!isValid);

                                if (batal) {
                                    err("Penghapusan Dibatalkan");
                                    break;
                                }

                                pindahProperti(&L, nip, nipBaru);
                            }

                            deleteAtParent(&L, nip);
                            break;
                        
                        }

                        case 4: // Lihat Semua Sales
                            printf("\n\t--- DAFTAR SALES ---");
                            printAllParent(L);
                            break;

                        case 0: // Kembali ke menu utama
                            break;

                        default:
                            err("Pilihan Tidak Tersedia");
                            break;
                    }
                    if (pilihanSales != 0) getch(); // pause, kecuali saat "Kembali"
                } while (pilihanSales != 0);
                break;

            case 2: // MENU PROPERTI (child)
                do {
                    pilihProperti = menuPanah("MENU PROPERTI", itemProperti, 4);

                    switch (pilihProperti) {
                        case 1: { /* Tambah Properti */
                            bool isValid, batal = false;
                            string hargaStr;

                            printf("\n\t--- TAMBAH PROPERTI ---\n");
                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printAllParent(L);
                            printf("\n");

                            do {
                                isValid = true;
                                printf("Masukkan NIP Sales (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    batal = true;
                                } else {
                                    salesDicari = findParent(L, nip);
                                    if (salesDicari == NULL) {
                                        err("Data Sales Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);
                            if (batal) { err("Penambahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Masukkan nomor properti (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(dataProperti.nomorProperti);
                                if (strcmp(dataProperti.nomorProperti, "") == 0) {
                                    batal = true;
                                } else if (findChild(salesDicari->firstChild, dataProperti.nomorProperti) != NULL) {
                                    err("Nomor Properti Sudah Ada");
                                    isValid = false;
                                }
                            } while (!isValid);
                            if (batal) { err("Penambahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Masukkan jenis properti (rumah/tanah, kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(dataProperti.jenis);
                                if (strcmp(dataProperti.jenis, "") == 0) {
                                    batal = true;
                                } else if (strcmpi(dataProperti.jenis, "rumah") != 0 && strcmpi(dataProperti.jenis, "tanah") != 0) {
                                    err("Jenis Harus Rumah atau Tanah");
                                    isValid = false;
                                }
                            } while (!isValid);
                            if (batal) { err("Penambahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Masukkan alamat properti (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(dataProperti.alamat);
                                if (strcmp(dataProperti.alamat, "") == 0) {
                                    batal = true;
                                }
                            } while (!isValid);
                            if (batal) { err("Penambahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Masukkan harga properti (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(hargaStr);
                                if (strcmp(hargaStr, "") == 0) {
                                    batal = true;
                                } else {
                                    dataProperti.harga = atoi(hargaStr);
                                    if (dataProperti.harga <= 0) {
                                        err("Harga Harus Berupa Angka Lebih Dari 0");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);
                            if (batal) { err("Penambahan Dibatalkan"); break; }

                            insertLastChild(&L, nip, dataProperti);
                            break;
                        }

                        case 2: { /* Ubah Properti */
                            bool isValid, batal = false;
                            string hargaStr;

                            printf("\n\t--- UBAH PROPERTI ---\n");
                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printSalesDanProperti(L);
                            printf("\n");

                            do {
                                isValid = true;
                                printf("Masukkan NIP Sales (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    batal = true;
                                } else {
                                    salesDicari = findParent(L, nip);
                                    if (salesDicari == NULL) {
                                        err("Data Sales Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);
                            if (batal) { err("Perubahan Dibatalkan"); break; }

                            if (!haveChild(salesDicari)) {
                                err("Sales Ini Belum Memiliki Properti");
                                break;
                            }

                            printf("\n\tProperti milik sales ini :");
                            tampilProperti(salesDicari->firstChild);
                            printf("\n\n");

                            do {
                                isValid = true;
                                printf("Masukkan nomor properti (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(noProp);
                                if (strcmp(noProp, "") == 0) {
                                    batal = true;
                                } else if (findChild(salesDicari->firstChild, noProp) == NULL) {
                                    err("Data Properti Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);
                            if (batal) { err("Perubahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Jenis properti baru (rumah/tanah, kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(dataPropBaru.jenis);
                                if (strcmp(dataPropBaru.jenis, "") == 0) {
                                    batal = true;
                                } else if (strcmpi(dataPropBaru.jenis, "rumah") != 0 && strcmpi(dataPropBaru.jenis, "tanah") != 0) {
                                    err("Jenis Harus Rumah atau Tanah");
                                    isValid = false;
                                }
                            } while (!isValid);
                            if (batal) { err("Perubahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Alamat properti baru (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(dataPropBaru.alamat);
                                if (strcmp(dataPropBaru.alamat, "") == 0) {
                                    batal = true;
                                }
                            } while (!isValid);
                            if (batal) { err("Perubahan Dibatalkan"); break; }

                            do {
                                isValid = true;
                                printf("Harga properti baru (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(hargaStr);
                                if (strcmp(hargaStr, "") == 0) {
                                    batal = true;
                                } else {
                                    dataPropBaru.harga = atoi(hargaStr);
                                    if (dataPropBaru.harga <= 0) {
                                        err("Harga Harus Berupa Angka Lebih Dari 0");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);
                            if (batal) { err("Perubahan Dibatalkan"); break; }

                            updateChild(&L, nip, noProp, dataPropBaru);
                            break;
                        }

                        case 3: { /* Hapus Properti */
                            bool isValid, batal = false;

                            printf("\n\t--- HAPUS PROPERTI ---\n");
                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printSalesDanProperti(L);
                            printf("\n");

                            do {
                                isValid = true;
                                printf("Masukkan NIP Sales (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    batal = true;
                                } else {
                                    salesDicari = findParent(L, nip);
                                    if (salesDicari == NULL) {
                                        err("Data Sales Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);
                            if (batal) { err("Penghapusan Dibatalkan"); break; }

                            if (!haveChild(salesDicari)) {
                                err("Sales Ini Belum Memiliki Properti");
                                break;
                            }

                            printf("\n\tProperti milik sales ini :");
                            tampilProperti(salesDicari->firstChild);
                            printf("\n\n");

                            do {
                                isValid = true;
                                printf("Masukkan nomor properti (kosongkan untuk batal) : ");
                                fflush(stdin);
                                gets(noProp);
                                if (strcmp(noProp, "") == 0) {
                                    batal = true;
                                } else if (findChild(salesDicari->firstChild, noProp) == NULL) {
                                    err("Data Properti Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);
                            if (batal) { err("Penghapusan Dibatalkan"); break; }

                            deleteAtChild(&L, nip, noProp);
                            break;
                        }

                        case 0: {
                            break;
                        }

                        default: {
                            err("Pilihan Tidak Tersedia");
                            break;
                        }
                    }
                    if (pilihProperti != 0) getch(); // pause, kecuali saat "Kembali"
                } while (pilihProperti != 0);
                break;

            case 3:{
                bool isValid, batal = false;
				string konfirmasi;
				addressParent pTemp;
				addressChild cTemp;
				
				getchar(); 
				
            	printf("\n\t--- Jual Properti ---\n");
            	if(isEmpty(L)){
            		err("Data sales masih kosong");
            		getch();
            		break;
				}
				
				printSalesDanProperti(L);
				printf("\n");
				
				do {
                    isValid = true;
                    printf("\tMasukkan NIP Sales (kosongkan untuk batal) : ");
                    fflush(stdin);
                    gets(nip);
                    if (strcmp(nip, "") == 0) {
                        batal = true;
                    } else {
                        salesDicari = findParent(L, nip);
                        if (salesDicari == NULL) {
                            err("Data sales tidak ditemukan");
                            isValid = false;
                        }
                    }
                } while (!isValid);
				
				if(batal){
					err("transaksi dibatalakan");
					getch();
					break;
				}
				
				
				if (!haveChild(salesDicari)) {
                    err("Sales ini belum memiliki properti untuk dijual");
                    getch();
                    break;
                }
                
                printf("\n\tProperti milik %s :", salesDicari->dataParent.nama);
                tampilProperti(salesDicari->firstChild);
                printf("\n\n");
                
                do {
                    isValid = true;
                    printf("\tMasukkan nomor properti yang terjual (kosongkan untuk batal) : ");fflush(stdin);gets(noProp);
                    if (strcmp(noProp, "") == 0) {
                        batal = true;
                    } else {
                        propDicari = findChild(salesDicari->firstChild, noProp);
                        if (propDicari == NULL) {
                            err("Data properti tidak ditemukan");
                            isValid = false;
                        }
                    }
                } while (!isValid);

                if (batal) {
                    err("Transaksi dibatalkan");
                    getch();
                    break;
                }
				
				printf("\n\tDetail Transaksi:");
                printf("\n\t[-] Sales   : %s (%s)", salesDicari->dataParent.nama, salesDicari->dataParent.NIP);
                printf("\n\t[-] Properti: [%s] %s - %s", propDicari->dataChild.nomorProperti, propDicari->dataChild.jenis, propDicari->dataChild.alamat);
                printf("\n\t[-] Harga   : Rp %d\n", propDicari->dataChild.harga);
                
                do{
                	isValid = true;
                	printf("\n\tKonfirmasi penjualan properti ini? (y/n): "); fflush(stdin); gets(konfirmasi);
                	if(strcmpi(konfirmasi, "y") != 0 && strcmpi(konfirmasi,"n") != 0){
                		err("Input harus 'y' atau 'n'");
                		isValid = false;
					}
				}while (!isValid);
				
				if (strcmpi(konfirmasi,"n") == 0){
					err("Transaski Penjualan dibatalkan");
				}else{
					jualProperti(&L, nip, noProp);
					printf("\tTotal Penjualan %s sekarang : Rp %d\n", salesDicari->dataParent.nama, salesDicari->dataParent.totalPenjualan);
					
				}
                getch();
                break;
            }

            case 4: // LAPORAN OWNER (belum dibuat)
                do {
                    pilihLaporan = menuPanah("MENU LAPORAN", itemLaporan, 4);

                    switch (pilihLaporan) {
                        case 1: {

                            break;
                        }

                        case 0: {
                            break;
                        }

                        default: {
                            err("Fitur Laporan Belum Tersedia");
                            break;
                        }
                    }
                    if (pilihLaporan != 0) getch(); // pause, kecuali saat "Kembali"
                } while (pilihLaporan != 0);
                break;

            case 0: // Keluar program
                break;

            default:
                err("Menu Tidak Tersedia");
                getch();
                break;
        }
    } while (menuUtama != 0);

    system("cls");
    suc("Terima Kasih Telah Menggunakan Program Ini");

    return 0;
}