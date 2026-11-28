#include "header.h"

void err(char *s) {
    printf("\033[1;31m");
    printf("\n\t  [!] %s [!]\n", s);
    printf("\033[0m");
}

void suc(char *s) {
    printf("\033[1;32m");
    printf("\n\t  [*] %s [*]\n", s);
    printf("\033[0m");
}

void showMenu() {
    system("cls");
    printf("\033[0;34m");
    printf("\n\t============================================================");
    printf("\033[0m");
    printf("\n\t    SISTEM PENGELOLAAN INVENTARIS DAN PENJUALAN PROPERTI");
    printf("\033[0;34m");
    printf("\n\t============================================================");
    printf("\033[0m");
    printf("\n\t\t[1] Sales");
    printf("\n\t\t[2] Properti");
    printf("\n\t\t[3] Jual Properti");
    printf("\n\t\t[4] Laporan Owner");
    printf("\n\t\t[0] Exit");
    printf("\n\t\t>>> ");
}

void menuSales() {
    system("cls");
    printf("\n\t========================================");
    printf("\n\t              MENU SALES                ");
    printf("\n\t========================================");
    printf("\n\t\t[1] Tambah Sales");
    printf("\n\t\t[2] Ubah Sales");
    printf("\n\t\t[3] Hapus Sales");
    printf("\n\t\t[4] Lihat Semua Sales");
    printf("\n\t\t[0] Kembali");
    printf("\n\t\t>>> ");
}

void menuProperti() {
    system("cls");
    printf("\n\t========================================");
    printf("\n\t              MENU PROPERTI             ");
    printf("\n\t========================================");
    printf("\n\t\t[1] Tambah Properti");
    printf("\n\t\t[2] Ubah Properti");
    printf("\n\t\t[3] Hapus Properti");
    printf("\n\t\t[0] Kembali");
    printf("\n\t\t>>> ");
}

void menuLaporan() {
    system("cls");
    printf("\n\t========================================");
    printf("\n\t              MENU LAPORAN              ");
    printf("\n\t========================================");
    printf("\n\t\t[1] Daftar Nama Sales");
    printf("\n\t\t[2] Properti Belum Terjual per Jenis"); // sales tertentu
    printf("\n\t\t[3] Jumlah Properti per Jenis");
    printf("\n\t\t[4] Properti Terjual");
    printf("\n\t\t[0] Kembali");
    printf("\n\t\t>>> ");
}

///////////////////////////////////LIST////////////////////////////////////////
void inisialisasi(ListParent *L) {
    L->first = NULL;
}

bool isEmpty(ListParent L) {
    if (L.first == NULL)
        return 1;
    else
        return 0;
}

void inisialisasiTransaksi(ListTransaksi *T) {
    T->first = NULL;
}

void insertTransaksi(ListTransaksi *T, DataTransaksi dataBaru) {
    addressTransaksi baru, temp;

    baru = (addressTransaksi)malloc(sizeof(NodeTransaksi));
    if (baru == NULL) {
        err("Alokasi Memori Gagal");
        return;
    }

    baru->dataTransaksi = dataBaru;
    baru->nextTransaksi = NULL;

    if (T->first == NULL) {
        T->first = baru;
    } else {
        temp = T->first;
        while (temp->nextTransaksi != NULL) {
            temp = temp->nextTransaksi;
        }
        temp->nextTransaksi = baru;
    }
}

///////////////////////////////////PARENT//////////////////////////////////////
bool haveChild(addressParent sales) {
    if (sales == NULL)
        return false;
    if (sales->firstChild != NULL)
        return true;
    return false;
}

addressParent alokasiParent(DataParent dataBaru) {
    addressParent salesBaru;
    salesBaru = (addressParent)malloc(sizeof(NodeParent));
    if (salesBaru == NULL) {
        err("Alokasi Memori Gagal");
        return NULL;
    }
    salesBaru->dataParent = dataBaru;
    salesBaru->nextParent = NULL;
    salesBaru->firstChild = NULL;
    return salesBaru;
}

addressParent findParent(ListParent L, string nipSales) {
    ListParent temp;

    if (isEmpty(L))
        return NULL;
    if (strcmp(L.first->dataParent.NIP, nipSales) == 0)
        return L.first;
    temp.first = L.first->nextParent;
    return findParent(temp, nipSales);
}

int countParent(ListParent L) {
    addressParent salesSekarang;
    int jumlahSales = 0;

    salesSekarang = L.first;
    while (salesSekarang != NULL) {
        jumlahSales++;
        salesSekarang = salesSekarang->nextParent;
    }
    return jumlahSales;
}

void insertFirstParent(ListParent *L, DataParent dataBaru) {
    addressParent salesBaru;

    if (findParent(*L, dataBaru.NIP) != NULL) {
        err("NIP Sales Sudah Terdaftar");
        return;
    }

    salesBaru = alokasiParent(dataBaru);
    if (salesBaru == NULL)
        return;

    salesBaru->nextParent = L->first;
    L->first = salesBaru;

    suc("Data Sales Berhasil Dimasukkan");
}

void insertLastParent(ListParent *L, DataParent dataBaru) {
    addressParent salesBaru, salesTerakhir;

    if (findParent(*L, dataBaru.NIP) != NULL) {
        err("NIP Sales Sudah Terdaftar");
        return;
    }

    salesBaru = alokasiParent(dataBaru);
    if (salesBaru == NULL)
        return;

    if (isEmpty(*L)) {
        L->first = salesBaru;
    } else {
        salesTerakhir = L->first;
        while (salesTerakhir->nextParent != NULL) {
            salesTerakhir = salesTerakhir->nextParent;
        }
        salesTerakhir->nextParent = salesBaru;
    }

    suc("Data Sales Berhasil Dimasukkan");
}

void insertAfterParent(ListParent *L, string nipSebelum, DataParent dataBaru) {
    addressParent salesSebelum, salesBaru;

    if (isEmpty(*L)) {
        insertFirstParent(L, dataBaru);
        return;
    }

    salesSebelum = findParent(*L, nipSebelum);
    if (salesSebelum == NULL) {
        err("Data Sales Sebelumnya Tidak Ditemukan");
        return;
    }

    if (findParent(*L, dataBaru.NIP) != NULL) {
        err("NIP Sales Sudah Terdaftar");
        return;
    }

    salesBaru = alokasiParent(dataBaru);
    if (salesBaru == NULL)
        return;

    salesBaru->nextParent = salesSebelum->nextParent;
    salesSebelum->nextParent = salesBaru;

    suc("Data Sales Berhasil Dimasukkan Setelah Sales Tersebut");
}

void updateParent(ListParent *L, string nipSales, DataParent dataBaru) {
    addressParent salesDiubah, salesNipSama;

    salesDiubah = findParent(*L, nipSales);
    if (salesDiubah == NULL) {
        err("Data Sales Tidak Ditemukan");
        return;
    }

    if (strcmp(nipSales, dataBaru.NIP) != 0) {
        salesNipSama = findParent(*L, dataBaru.NIP);
        if (salesNipSama != NULL) {
            err("NIP Sales Sudah Digunakan");
            return;
        }
        strcpy(salesDiubah->dataParent.NIP, dataBaru.NIP);
    }

    strcpy(salesDiubah->dataParent.nama, dataBaru.nama);
    strcpy(salesDiubah->dataParent.nomorTelpon, dataBaru.nomorTelpon);
    // totalPenjualan tidak diubah manual, hanya bertambah saat properti terjual

    suc("Data Sales Berhasil Diubah");
}

void deleteFirstParent(ListParent *L) {
    addressParent salesHapus;

    if (isEmpty(*L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    salesHapus = L->first;
    if (haveChild(salesHapus)) {
        err("Sales Masih Memiliki Properti, Alihkan Properti Terlebih Dahulu");
        return;
    }

    L->first = salesHapus->nextParent;
    free(salesHapus);

    suc("Data Sales Pertama Berhasil Dihapus");
}

void deleteLastParent(ListParent *L) {
    addressParent salesSebelumnya, salesHapus;

    if (isEmpty(*L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    if (L->first->nextParent == NULL) {
        deleteFirstParent(L);
        return;
    }

    salesSebelumnya = L->first;
    while (salesSebelumnya->nextParent->nextParent != NULL) {
        salesSebelumnya = salesSebelumnya->nextParent;
    }

    salesHapus = salesSebelumnya->nextParent;
    if (haveChild(salesHapus)) {
        err("Sales Masih Memiliki Properti, Alihkan Properti Terlebih Dahulu");
        return;
    }

    salesSebelumnya->nextParent = NULL;
    free(salesHapus);

    suc("Data Sales Terakhir Berhasil Dihapus");
}

void deleteAtParent(ListParent *L, string nipSales) {
    addressParent salesSebelumnya, salesTarget;

    if (isEmpty(*L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    salesSebelumnya = NULL;
    salesTarget = L->first;
    while (salesTarget != NULL && strcmp(salesTarget->dataParent.NIP, nipSales) != 0) {
        salesSebelumnya = salesTarget;
        salesTarget = salesTarget->nextParent;
    }

    if (salesTarget == NULL) {
        err("Data Sales Tidak Ditemukan");
        return;
    }

    if (haveChild(salesTarget)) {
        err("Sales Masih Memiliki Properti, Alihkan Properti Terlebih Dahulu");
        return;
    }

    if (salesSebelumnya == NULL)
        L->first = salesTarget->nextParent;
    else
        salesSebelumnya->nextParent = salesTarget->nextParent;

    free(salesTarget);

    suc("Data Sales Berhasil Dihapus");
}

void printParent(addressParent sales) {
    printf("\n\t === Data Sales ===");
    printf("\n\t[-] NIP             : %s", sales->dataParent.NIP);
    printf("\n\t[-] Nama            : %s", sales->dataParent.nama);
    printf("\n\t[-] Nomor Telepon   : %s", sales->dataParent.nomorTelpon);
    printf("\n\t[-] Total Penjualan : Rp %d", sales->dataParent.totalPenjualan);
}

void printAllParent(ListParent L) {
    addressParent salesSekarang;

    if (isEmpty(L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    salesSekarang = L.first;
    while (salesSekarang != NULL) {
        printParent(salesSekarang);
        printf("\n");
        salesSekarang = salesSekarang->nextParent;
    }
    printf("\n\tJumlah Sales : %d", countParent(L));
}

//////////////CHILD/////////////////////////

addressChild alokasiChild(DataChild dataChild) {
    addressChild C = (addressChild)malloc(sizeof(NodeChild));
    C->dataChild = dataChild;
    C->nextChild = NULL;
    return C;
}

addressChild findChild(addressChild C, string nomorProperti) {
    if (C == NULL)
        return NULL;
    if (strcmp(C->dataChild.nomorProperti, nomorProperti) == 0)
        return C;
    return findChild(C->nextChild, nomorProperti);
}

void insertFirstChild(ListParent *L, string nipSales, DataChild dataBaru) {
    addressChild C;
    addressParent P;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t[!] Parent tidak ada [!]");
    } else {
        C = alokasiChild(dataBaru);
        C->nextChild = P->firstChild;
        P->firstChild = C;

        printf("\n\t\t [!] Insert First Child [!]");
    }
}

void insertLastChild(ListParent *L, string nipSales, DataChild dataBaru) {
    addressChild C, temp;
    addressParent P;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t [!] Parent tidak ada [!]");
    } else {
        C = alokasiChild(dataBaru);
        if (P->firstChild == NULL) {
            P->firstChild = C;
        } else {
            temp = P->firstChild;
            while (temp->nextChild != NULL) {
                temp = temp->nextChild;
            }
            temp->nextChild = C;
        }
        printf("\n\t\t [!] Insert Last Child [!]");
    }
}

void insertAtChild(ListParent *L, string nipSales, DataChild dataBaru, int input) {
    addressChild C, temp;
    addressParent P;
    int i = 1;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t [!] Parent tidak ada [!]");
    } else {
        if (input < 1) {
            printf("\n\t\t [!] Posisi tidak valid [!]");
        } else if (input == 1) {
            insertFirstChild(L, nipSales, dataBaru);
        } else {
            temp = P->firstChild;
            while (temp != NULL && i < input - 1) {
                temp = temp->nextChild;
                i++;
            }
            if (temp == NULL) {
                printf("\n\t\t [!] Posisi melebihi jumlah properti [!]");
            } else {
                C = alokasiChild(dataBaru);
                C->nextChild = temp->nextChild;
                temp->nextChild = C;

                printf("\n\t\t [!] Insert At Child [!]");
            }
        }
    }
}

void updateChild(ListParent *L, string nipSales, string nomorProperti, DataChild dataBaru) {
    addressParent P;
    addressChild C;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t [!] Parent tidak ada [!]");
    } else {
        C = findChild(P->firstChild, nomorProperti);
        if (C == NULL) {
            printf("\n\t\t [!] Data Properti Tidak Ditemukan [!]");
        } else {
            strcpy(dataBaru.nomorProperti, C->dataChild.nomorProperti);
            C->dataChild = dataBaru;
            printf("\n\t\t [!] Data Properti Berhasil Diubah [!]");
        }
    }
}

void deleteFirstChild(ListParent *L, string nipSales) {
    addressParent P;
    addressChild hapus;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t [!] Parent tidak ada [!]");
    } else {
        if (P->firstChild == NULL) {
            printf("\n\t\t [!] Data Properti Kosong [!]");
        } else {
            hapus = P->firstChild;
            P->firstChild = hapus->nextChild;
            free(hapus);
            printf("\n\t\t [!] Delete First Child [!]");
        }
    }
}

void deleteLastChild(ListParent *L, string nipSales) {
    addressParent P;
    addressChild C;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t [!] Parent tidak ada [!]");
    } else {
        if (P->firstChild == NULL) {
            printf("\n\t\t [!] Data Properti Kosong [!]");
        } else if (P->firstChild->nextChild == NULL) {
            free(P->firstChild);
            P->firstChild = NULL;
            printf("\n\t\t [!] Delete Last Child [!]");
        } else {
            C = P->firstChild;
            while (C->nextChild->nextChild != NULL) {
                C = C->nextChild;
            }
            free(C->nextChild);
            C->nextChild = NULL;
            printf("\n\t\t [!] Delete Last Child [!]");
        }
    }
}

void deleteAtChild(ListParent *L, string nipSales, string nomorProperti) {
    addressParent P;
    addressChild C, hapus;

    P = findParent((*L), nipSales);
    if (P == NULL) {
        printf("\n\t\t [!] Parent tidak ada [!]");
    } else {
        if (P->firstChild == NULL) {
            printf("\n\t\t [!] Data Properti Kosong [!]");
        } else if (strcmp(P->firstChild->dataChild.nomorProperti, nomorProperti) == 0) {
            deleteFirstChild(L, nipSales);
        } else {
            C = P->firstChild;
            while (C->nextChild != NULL && strcmp(C->nextChild->dataChild.nomorProperti, nomorProperti) != 0) {
                C = C->nextChild;
            }
            if (C->nextChild == NULL) {
                printf("\n\t\t [!] Data Properti Tidak Ditemukan [!]");
            } else {
                hapus = C->nextChild;
                C->nextChild = hapus->nextChild;
                free(hapus);
                printf("\n\t\t [!] Delete At Child [!]");
            }
        }
    }
}

void tampilProperti(addressChild C) {
    if (C != NULL) {
        printf("\n\t\t- [%s] %s | %s | Rp %d", C->dataChild.nomorProperti, C->dataChild.jenis, C->dataChild.alamat, C->dataChild.harga);
        tampilProperti(C->nextChild);
    }
}

void printSalesDanProperti(ListParent L) {
    addressParent P;

    if (isEmpty(L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    P = L.first;
    while (P != NULL) {
        printParent(P);
        if (haveChild(P)) {
            printf("\n\t[-] Daftar Properti :");
            tampilProperti(P->firstChild);
        } else {
            printf("\n\t[-] Daftar Properti : (belum ada properti)");
        }
        printf("\n");
        P = P->nextParent;
    }
}

void jualProperti(ListParent *L, string nipSales, string nomorProperti, ListTransaksi *T) {
    addressParent P;
    addressChild C;
    DataTransaksi transaksi;

    P = findParent(*L, nipSales);
    if (P == NULL) {
        err("Data tidak ditemukan");
        return;
    }

    C = findChild(P->firstChild, nomorProperti);
    if (C == NULL) {
        err("Data Properti tidak ditemukan");
        return;
    }

    /* simpan dulu datanya sebelum properti dihapus dari list sales */
    strcpy(transaksi.nomorProperti, C->dataChild.nomorProperti);
    strcpy(transaksi.jenis, C->dataChild.jenis);
    strcpy(transaksi.alamat, C->dataChild.alamat);
    transaksi.harga = C->dataChild.harga;
    strcpy(transaksi.nipSales, P->dataParent.NIP);
    strcpy(transaksi.namaSales, P->dataParent.nama);

    P->dataParent.totalPenjualan += C->dataChild.harga;
    insertTransaksi(T, transaksi);
    deleteAtChild(L, nipSales, nomorProperti);
    suc("Transaksi Berhasil dan Total Penjualan sales telah diperbarui");
}

void pindahProperti(ListParent *L, string nipAsal, string nipTujuan) {
    addressParent asal, tujuan;
    addressChild C, temp;

    asal = findParent(*L, nipAsal);
    tujuan = findParent(*L, nipTujuan);
    if (asal == NULL || tujuan == NULL) {
        err("Data Sales Tidak Ditemukan");
        return;
    }
    if (!haveChild(asal))
        return;

    /* nomor properti dicek unik per sales, jadi cegah bentrok */
    for (C = asal->firstChild; C != NULL; C = C->nextChild) {
        if (findChild(tujuan->firstChild, C->dataChild.nomorProperti) != NULL) {
            err("Nomor Properti Bentrok dengan Milik Sales Pengganti");
            return;
        }
    }

    if (tujuan->firstChild == NULL) {
        tujuan->firstChild = asal->firstChild;
    } else {
        temp = tujuan->firstChild;
        while (temp->nextChild != NULL)
            temp = temp->nextChild;
        temp->nextChild = asal->firstChild;
    }
    asal->firstChild = NULL;

    suc("Properti Berhasil Dialihkan ke Sales Pengganti");
}

// arrow key
int menuPanah(char *judul, char *item[], int jumlah) {
    int pilihan = 0, i, key;

    while (1) {
        system("cls");

        /* header berwarna biru, sama seperti menu awal */
        printf("\033[0;34m");
        printf("\n\t============================================================");
        printf("\033[0m");
        printf("\n\t    %s", judul);
        printf("\033[0;34m");
        printf("\n\t============================================================");
        printf("\033[0m");

        for (i = 0; i < jumlah; i++) {
            if (i == pilihan)
                printf("\n\t\t\033[1;37m> %s\033[0m", item[i]);
            else
                printf("\n\t\t  %s", item[i]); /* spasi pengganti panah, supaya teks sejajar */
        }
        printf("\n");
        printf("\n\t  [Up/Down] Pilih    [Enter] Konfirmasi    [Esc] Kembali\n");

        key = getch();
        if (key == 0 || key == 224) { /* tombol khusus (panah) */
            key = getch();
            if (key == 72) /* panah atas */
                pilihan = (pilihan - 1 + jumlah) % jumlah;
            else if (key == 80) /* panah bawah */
                pilihan = (pilihan + 1) % jumlah;
        } else if (key == 13) {            /* Enter */
            return (pilihan + 1) % jumlah; /* item terakhir -> 0 */
        } else if (key == 27) {            /* Esc */
            return 0;
        }
    }
}

// ///////////////////Laporan Owner & Transaksi///////////////////////////////

void laporanNamaSales(ListParent L) {
    addressParent P;
    addressChild C;
    int nomor = 1, jumlahProperti;

    if (isEmpty(L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    printf("\n\t--- DAFTAR NAMA SALES ---");
    P = L.first;
    while (P != NULL) {
        jumlahProperti = 0;
        C = P->firstChild;
        while (C != NULL) {
            jumlahProperti++;
            C = C->nextChild;
        }

        printf("\n\n\t[%d] NIP             : %s", nomor, P->dataParent.NIP);
        printf("\n\t    Nama            : %s", P->dataParent.nama);
        printf("\n\t    Nomor Telepon   : %s", P->dataParent.nomorTelpon);
        printf("\n\t    Total Penjualan : Rp %d", P->dataParent.totalPenjualan);
        printf("\n\t    Jumlah Properti : %d", jumlahProperti);

        nomor++;
        P = P->nextParent;
    }
    printf("\n\n\tJumlah Sales : %d\n", countParent(L));
}

void laporanBelumTerjual(ListParent L, string nipSales, string jenis) {
    addressParent P;
    addressChild C;
    int jumlah = 0, totalHarga = 0;

    P = findParent(L, nipSales);
    if (P == NULL) {
        err("Data Sales Tidak Ditemukan");
        return;
    }

    if (!haveChild(P)) {
        err("Sales Ini Belum Memiliki Properti");
        return;
    }

    printf("\n\t--- PROPERTI  (%s) MILIK %s BELUM TERJUAL ---", jenis, P->dataParent.nama);

    C = P->firstChild;
    while (C != NULL) {
        if (strcmpi(C->dataChild.jenis, jenis) == 0) {
            printf("\n\t[-] %s | %s | Rp %d", C->dataChild.nomorProperti, C->dataChild.alamat, C->dataChild.harga);
            jumlah++;
            totalHarga += C->dataChild.harga;
        }
        C = C->nextChild;
    }

    if (jumlah == 0) {
        printf("\n\tTidak Ada Properti %s yang Belum Terjual\n", jenis);
        return;
    }

    printf("\n\n\tJumlah Properti %s : %d", jenis, jumlah);
    printf("\n\tTotal Harga         : Rp %d\n", totalHarga);
}

void laporanJumlahJenis(ListParent L) {
    addressParent P;
    addressChild C;
    int totalTanah = 0, totalRumah = 0;
    int tanah, rumah;

    if (isEmpty(L)) {
        err("Data Sales Masih Kosong");
        return;
    }

    printf("\n\t--- JUMLAH PROPERTI PER JENIS ---");
    P = L.first;
    while (P != NULL) {
        tanah = 0;
        rumah = 0;
        C = P->firstChild;
        while (C != NULL) {
            if (strcmpi(C->dataChild.jenis, "tanah") == 0)
                tanah++;
            else if (strcmpi(C->dataChild.jenis, "rumah") == 0)
                rumah++;
            C = C->nextChild;
        }

        printf("\n\t[-] %s (%s) : Tanah = %d | Rumah = %d | Total = %d", P->dataParent.nama, P->dataParent.NIP, tanah, rumah, tanah + rumah);

        totalTanah += tanah;
        totalRumah += rumah;
        P = P->nextParent;
    }

    printf("\n\n\t=== TOTAL SEMUA SALES ===");
    printf("\n\t[-] Tanah : %d", totalTanah);
    printf("\n\t[-] Rumah : %d", totalRumah);
    printf("\n\t[-] Total : %d\n", totalTanah + totalRumah);
}

void laporanPropertiTerjual(ListTransaksi T) {
    addressTransaksi Tr;
    int jumlah = 0, totalHarga = 0;

    if (T.first == NULL) {
        err("Belum Ada Properti yang Terjual");
        return;
    }

    printf("\n\t--- PROPERTI YANG SUDAH TERJUAL ---");

    Tr = T.first;
    while (Tr != NULL) {
        printf("\n\t[-] [%s] %s | %s | Rp %d", Tr->dataTransaksi.nomorProperti, Tr->dataTransaksi.jenis, Tr->dataTransaksi.alamat,
               Tr->dataTransaksi.harga);
        printf("\n\t    Sales: %s (%s)", Tr->dataTransaksi.namaSales, Tr->dataTransaksi.nipSales);
        jumlah++;
        totalHarga += Tr->dataTransaksi.harga;
        Tr = Tr->nextTransaksi;
    }

    printf("\n\n\tJumlah Properti Terjual : %d", jumlah);
    printf("\n\tTotal Harga Terjual     : Rp %d\n", totalHarga);
}