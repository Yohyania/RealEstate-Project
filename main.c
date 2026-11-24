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

    inisialisasi(&L);

    do {
        showMenu();
        scanf("%d", &menuUtama);

        switch (menuUtama) {
            case 1: /* MENU SALES (parent) */
                do {
                    menuSales();
                    scanf("%d", &pilihanSales);
                    getchar();
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
                            bool isValid;

                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printf("\n\t--- UBAH SALES ---\n");

                            do {
                                isValid = true;
                                printf("\tNIP Sales yang diubah : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    err("NIP Tidak Boleh Kosong");
                                    isValid = false;
                                } else if (findParent(L, nip) == NULL) {
                                    err("Data Sales Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);

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
                            bool isValid;

                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            printf("\n\t--- HAPUS SALES ---\n");

                            do {
                                isValid = true;
                                printf("\tNIP Sales yang dihapus : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    err("NIP Tidak Boleh Kosong");
                                    isValid = false;
                                } else if (findParent(L, nip) == NULL) {
                                    err("Data Sales Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("\tNIP Sales pengganti : ");
                                fflush(stdin);
                                gets(nipBaru);
                                if (strcmpi(nipBaru, "") == 0) {
                                    err("NIP Pengganti Tidak Boleh Kosong");
                                    isValid = false;
                                } else if (strcmpi(nipBaru, nip) == 0) {
                                    err("Sales Pengganti Harus Berbeda");
                                    isValid = false;
                                } else if (findParent(L, nipBaru) == NULL) {
                                    err("Sales Pengganti Tidak Ditemukan");
                                    isValid = false;
                                }
                            } while (!isValid);

                            /* Pemindahan properti ke sales pengganti belum tersedia */
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
                    getch();
                } while (pilihanSales != 0);
                break;

            case 2: // MENU PROPERTI (child, belum dibuat)
                do {
                    menuProperti();
                    scanf("%d", &pilihProperti);
                    getchar();
                    switch (pilihProperti) {
                        case 1: { /* Tambah Properti */
                            bool isValid;

                            printf("\n\t--- TAMBAH PROPERTI ---\n");
                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            do {
                                isValid = true;
                                printf("Masukkan NIP Sales  : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    err("NIP Tidak Boleh Kosong");
                                    isValid = false;
                                } else {
                                    salesDicari = findParent(L, nip);
                                    if (salesDicari == NULL) {
                                        err("Data Sales Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan nomor properti : ");
                                fflush(stdin);
                                gets(dataProperti.nomorProperti);
                                if (strcmp(dataProperti.nomorProperti, "") == 0) {
                                    err("Nomor properti tidak boleh kosong");
                                    isValid = false;
                                } else {
                                    propDicari = findChild(salesDicari->firstChild, dataProperti.nomorProperti);
                                    if (propDicari != NULL) {
                                        err("Nomor Properti Sudah Ada");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan jenis properti (rumah/tanah)   : ");
                                fflush(stdin);
                                gets(dataProperti.jenis);
                                if (strcmpi(dataProperti.jenis, "rumah") != 0 && strcmpi(dataProperti.jenis, "tanah") != 0) {
                                    err("Jenis Harus Rumah atau Tanah");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan alamat properti     : ");
                                fflush(stdin);
                                gets(dataProperti.alamat);
                                if (strcmp(dataProperti.alamat, "") == 0) {
                                    err("Alamat Tidak Boleh Kosong");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan harga properti : ");
                                scanf("%d", &dataProperti.harga);
                                if (dataProperti.harga <= 0) {
                                    err("Harga Harus Lebih Dari 0");
                                    isValid = false;
                                }
                            } while (!isValid);

                            insertLastChild(&L, nip, dataProperti);
                            break;
                        }

                        case 2: { /* Ubah Properti */
                            bool isValid;

                            printf("\n\t--- UBAH PROPERTI ---\n");
                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            do {
                                isValid = true;
                                printf("Masukkan NIP Sales  : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    err("NIP Tidak Boleh Kosong");
                                    isValid = false;
                                } else {
                                    salesDicari = findParent(L, nip);
                                    if (salesDicari == NULL) {
                                        err("Data Sales Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan nomor properti : ");
                                fflush(stdin);
                                gets(noProp);
                                if (strcmp(noProp, "") == 0) {
                                    err("Nomor Properti Tidak Boleh Kosong");
                                    isValid = false;
                                } else {
                                    propDicari = findChild(salesDicari->firstChild, noProp);
                                    if (propDicari == NULL) {
                                        err("Data Properti Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan jenis properti (tanah/rumah)   : ");
                                fflush(stdin);
                                gets(dataPropBaru.jenis);
                                if (strcmpi(dataPropBaru.jenis, "rumah") != 0 && strcmpi(dataPropBaru.jenis, "tanah") != 0) {
                                    err("Jenis Harus Rumah atau Tanah");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Alamat properti baru : ");
                                fflush(stdin);
                                gets(dataPropBaru.alamat);
                                if (strcmp(dataPropBaru.alamat, "") == 0) {
                                    err("Alamat Tidak Boleh Kosong");
                                    isValid = false;
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Harga properti baru : ");
                                scanf("%d", &dataPropBaru.harga);
                                if (dataPropBaru.harga <= 0) {
                                    err("Harga Harus Lebih Dari 0");
                                    isValid = false;
                                }
                            } while (!isValid);

                            updateChild(&L, nip, noProp, dataPropBaru);
                            break;
                        }

                        case 3: { /* Hapus Properti */
                            bool isValid;

                            printf("\n\t--- HAPUS PROPERTI ---\n");
                            if (isEmpty(L)) {
                                err("Data Sales Masih Kosong");
                                break;
                            }

                            do {
                                isValid = true;
                                printf("Masukkan NIP Sales  : ");
                                fflush(stdin);
                                gets(nip);
                                if (strcmp(nip, "") == 0) {
                                    err("NIP Tidak Boleh Kosong");
                                    isValid = false;
                                } else {
                                    salesDicari = findParent(L, nip);
                                    if (salesDicari == NULL) {
                                        err("Data Sales Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);

                            do {
                                isValid = true;
                                printf("Masukkan nomor properti : ");
                                fflush(stdin);
                                gets(noProp);
                                if (strcmp(noProp, "") == 0) {
                                    err("Nomor Properti Tidak Boleh Kosong");
                                    isValid = false;
                                } else {
                                    propDicari = findChild(salesDicari->firstChild, noProp);
                                    if (propDicari == NULL) {
                                        err("Data Properti Tidak Ditemukan");
                                        isValid = false;
                                    }
                                }
                            } while (!isValid);

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
                    getch();
                } while (pilihProperti != 0);
                break;

            case 3: // JUAL PROPERTI (belum dibuat)
                err("Fitur Jual Properti Belum Tersedia");
                getch();
                break;

            case 4: // LAPORAN OWNER (belum dibuat)
                do {
                    menuLaporan();
                    scanf("%d", &pilihLaporan);
                    switch (pilihLaporan) {
                        case 1: {

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
