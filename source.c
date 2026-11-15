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
    printf("\n\t\t[3] JualProperti");
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
