# Alur Kode — RealEstate XYZ (MultiList)

Dokumen ini menjelaskan **alur eksekusi dan alur data** program: mulai dari `main`,
masuk ke menu, pemanggilan fungsi, hingga perubahan tersimpan di list. Fokusnya
adalah hubungan antarfungsi dan urutan langkah, bukan deklarasi variabel.

## Daftar Isi

- [Alur Kode — RealEstate XYZ (MultiList)](#alur-kode--realestate-xyz-multilist)
  - [Daftar Isi](#daftar-isi)
  - [1. Peta File dalam Alur](#1-peta-file-dalam-alur)
  - [2. Data yang Mengalir di List](#2-data-yang-mengalir-di-list)
  - [3. Alur Top-Level `main()`](#3-alur-top-level-main)
  - [4. Alur Submenu Sales](#4-alur-submenu-sales)
    - [4.1 Pembacaan pilihan](#41-pembacaan-pilihan)
    - [4.2 Alur `[1] Tambah Sales`](#42-alur-1-tambah-sales)
    - [4.3 Alur `[2] Ubah Sales`](#43-alur-2-ubah-sales)
    - [4.4 Alur `[3] Hapus Sales`](#44-alur-3-hapus-sales)
    - [4.5 Alur `[4] Lihat Semua Sales`](#45-alur-4-lihat-semua-sales)
  - [5. Peta Alur Fungsi Parent](#5-peta-alur-fungsi-parent)
  - [6. Alur Submenu Properti](#6-alur-submenu-properti)
  - [7. Alur Fungsi Child (Properti)](#7-alur-fungsi-child-properti)
    - [7.1 Pola umum](#71-pola-umum)
    - [7.2 Alur pembuatan dan pencarian node](#72-alur-pembuatan-dan-pencarian-node)
    - [7.3 Alur insert](#73-alur-insert)
    - [7.4 Alur update](#74-alur-update)
    - [7.5 Alur delete](#75-alur-delete)
    - [7.6 Alur tampil](#76-alur-tampil)
    - [7.7 Peta alur fungsi child](#77-peta-alur-fungsi-child)
  - [8. Alur Jual Properti](#8-alur-jual-properti)
  - [9. Alur Laporan Owner](#9-alur-laporan-owner)
  - [10. Alur Input dan Tampilan](#10-alur-input-dan-tampilan)
    - [10.1 Alur input](#101-alur-input)
    - [10.2 Alur pesan](#102-alur-pesan)
  - [11. Contoh Alur Data End-to-End](#11-contoh-alur-data-end-to-end)

---

## 1. Peta File dalam Alur

| File | Peran dalam alur |
| --- | --- |
| `main.c` | Titik masuk. Membaca input user dan memilih fungsi yang dipanggil. |
| `header.h` | Kontrak antarfile: tipe data, struct, dan prototipe fungsi. |
| `source.c` | Implementasi: fungsi tampilan, fungsi parent, fungsi child. |
| `tubes_prd.md` | Spesifikasi alur bisnis yang menjadi dasar program (tidak di-track git). |

Alur antarfile:

```
main.c  --include-->  header.h  <--include--  source.c
   |                     |                       |
   | memanggil fungsi    | prototipe             | implementasi
   +-------------------->+---------------------->+
```

`main.c` tidak mendefinisikan fungsi selain `main`. Setiap pemanggilan fungsi
mengarah ke implementasi di `source.c` melalui prototipe di `header.h`.

---

## 2. Data yang Mengalir di List

Alur data berpusat pada satu variabel `ListParent L` di `main`.

```c
typedef struct {
    string nomorProperti;
    string jenis;
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
```

Peta alur data di memori:

```
L.first
   |
   v
[Sales 1001] --nextParent--> [Sales 1002] --nextParent--> NULL
   |                            |
   | firstChild                 | firstChild
   v                            v
[P01 Rumah] --nextChild--> NULL  [P02 Tanah] --nextChild--> [P03 Rumah] --> NULL
```

Artinya:

- `L.first` adalah pintu masuk ke seluruh sales.
- `nextParent` menghubungkan sales satu ke sales berikutnya.
- `firstChild` adalah pintu masuk ke list properti milik sales tersebut.
- `nextChild` menghubungkan properti satu ke properti berikutnya dalam sales yang sama.

Setiap fungsi menerima `L` (atau `&L`) lalu bergerak mengikuti rantai ini.

---

## 3. Alur Top-Level `main()`

```
main()
 |
 +-- char *itemUtama[]    = {Sales, Properti, Jual Properti, Laporan Owner, Exit}
 |   char *itemSales[]    = {Tambah Sales, Ubah Sales, Hapus Sales, Lihat Semua Sales, Kembali}
 |   char *itemProperti[] = {Tambah Properti, Ubah Properti, Hapus Properti, Kembali}
 |   char *itemLaporan[]  = {Daftar Nama Sales, Properti Belum Terjual per Jenis,
 |                           Jumlah Properti per Jenis, Properti Terjual, Kembali}
 |
 +-- inisialisasi(&L)                     -> L.first = NULL
  +-- inisialisasiTransaksi(&T)            -> T.first = NULL
 |
 +-- LOOP menu utama (do-while)
 |    |
 |    +-- menuPanah("SISTEM ...", itemUtama, 5)
 |    |      -> Up/Down memilih, Enter konfirmasi, Esc kembali
 |    |      -> item terakhir (Exit) dikembalikan sebagai 0
 |    +-- switch (menuUtama)
 |         |
 |         +-- 1  -> LOOP submenu Sales
 |         +-- 2  -> LOOP submenu Properti
 |         +-- 3  -> LOOP transaksi Jual Properti
 |         +-- 4  -> LOOP submenu Laporan Owner
 |         +-- 0  -> keluar dari loop utama
 |         +-- default -> err("Menu Tidak Tersedia") + getch()
 |
 +-- system("cls")
 +-- suc("Terima Kasih Telah Menggunakan Program Ini")
```

`do-while` dipakai supaya menu tampil dan pilihan dibaca minimal sekali.
Keluar dari loop terjadi ketika `menuUtama == 0` (item `Exit`, atau tombol `Esc`).
Navigasi memakai `menuPanah` di `source.c`, bukan `scanf`.

---

## 4. Alur Submenu Sales

### 4.1 Pembacaan pilihan

```
case 1 (menu utama)
 |
 +-- LOOP (do-while)
 |    |
 |    +-- menuPanah("MENU SALES", itemSales, 5)
 |    +-- switch (pilihanSales)
 |    |    |
 |    |    +-- 1 -> Tambah Sales
 |    |    +-- 2 -> Ubah Sales
 |    |    +-- 3 -> Hapus Sales
 |    |    +-- 4 -> Lihat Semua Sales
 |    |    +-- 0 -> keluar submenu (item "Kembali")
 |    |    +-- default -> err("Pilihan Tidak Tersedia")
 |    |
 |    +-- if (pilihanSales != 0) getch()   -> jeda, kecuali saat "Kembali"
 |    +-- ulang selama pilihanSales != 0
 |
 +-- break -> kembali ke menu utama
```

### 4.2 Alur `[1] Tambah Sales`

Tiap field divalidasi oleh loop `do { ... } while (!isValid)` masing-masing.
Kalau satu field salah, hanya field itu yang ditanyakan lagi; field yang sudah
benar tidak perlu diisi ulang dan program tidak kembali ke menu.

```
isValid = true

LOOP NIP
  |
  +-- baca dataSales.NIP dengan prompt "NIP   (kosongkan untuk batal): "
  +-- kosong? -> batal = true -> keluar loop -> err("Penambahan Dibatalkan")
  +-- findParent(L, dataSales.NIP) != NULL? -> err("NIP Sudah Terdaftar") -> isValid = false
  +-- strlen(dataSales.NIP) < 5? -> err("NIP Tidak Boleh Kurang dari 5 Digit") -> isValid = false
  +-- ulang selama !isValid

LOOP Nama
  |
  +-- baca dataSales.nama
  +-- kosong? -> err("Nama Tidak Boleh Kosong") -> isValid = false
  +-- ulang selama !isValid

LOOP Nomor Telepon
  |
  +-- baca dataSales.nomorTelpon
  +-- kosong? -> err("Nomor Telepon Tidak Boleh Kosong") -> isValid = false
  +-- ulang selama !isValid

dataSales.totalPenjualan = 0
insertLastParent(&L, dataSales)
  |
  +-- alokasiParent(dataSales) -> malloc + isi data + nextParent/firstChild = NULL
  +-- L kosong? -> L->first = salesBaru
  |              atau
  |              traversal sampai nextParent == NULL -> sambung di ujung
  +-- suc("Data Sales Berhasil Dimasukkan")
```

Urutan tampilan mengikuti urutan input karena yang dipakai adalah `insertLastParent`.
Pengecekan NIP duplikat juga terjadi di dalam `insertLastParent` sebagai pengaman.

### 4.3 Alur `[2] Ubah Sales`

Sama seperti Tambah Sales, tiap field punya loop validasi sendiri. NIP lama yang
tidak ditemukan juga ditanyakan ulang di tempat, bukan kembali ke menu.

```
isEmpty(L) ?
  |
  +-- ya  -> err("Data Sales Masih Kosong") -> kembali ke switch
  |
  +-- tidak
       |
       +-- LOOP NIP lama
       |     |
       |     +-- baca nip
       |     +-- kosong? -> err("NIP Tidak Boleh Kosong") -> isValid = false
       |     +-- findParent(L, nip) == NULL? -> err("Data Sales Tidak Ditemukan") -> isValid = false
       |     +-- ulang selama !isValid
       |
       +-- LOOP NIP baru
       |     |
       |     +-- baca nipBaru
       |     +-- kosong? -> strcpy(nipBaru, nip)  (NIP tetap)
       |     +-- beda dari nip dan findParent(L, nipBaru) != NULL?
       |     |     -> err("NIP Sudah Digunakan Sales Lain") -> isValid = false
       |     +-- ulang selama !isValid
       |     +-- strcpy(dataSales.NIP, nipBaru)
       |
       +-- LOOP Nama Baru (kosong -> err -> ulang)
       +-- LOOP Nomor Telepon Baru (kosong -> err -> ulang)
       |
       +-- updateParent(&L, nip, dataSales)
             |
             +-- findParent(*L, nip) -> node yang akan diubah
             +-- NIP berubah? -> findParent(*L, NIP baru) -> validasi
             +-- strcpy NIP (jika berubah), nama, nomorTelpon
             +-- suc("Data Sales Berhasil Diubah")
```

`totalPenjualan` tidak melewati jalur input: nilainya berasal dari akumulasi penjualan.

### 4.4 Alur `[3] Hapus Sales`

```
isEmpty(L) ?
  |
  +-- ya  -> err("Data Sales Masih Kosong") -> kembali ke switch
  |
  +-- tidak
       |
       +-- LOOP NIP sales
       |     |
       |     +-- baca nip
       |     +-- kosong? -> err("NIP Tidak Boleh Kosong") -> isValid = false
       |     +-- findParent(L, nip) == NULL? -> err("Data Sales Tidak Ditemukan") -> isValid = false
       |     +-- ulang selama !isValid
       |
       +-- LOOP NIP pengganti
       |     |
       |     +-- baca nipBaru
       |     +-- kosong? -> err("NIP Pengganti Tidak Boleh Kosong") -> isValid = false
       |     +-- sama dengan nip? -> err("Sales Pengganti Harus Berbeda") -> isValid = false
       |     +-- findParent(L, nipBaru) == NULL? -> err("Sales Pengganti Tidak Ditemukan") -> isValid = false
       |     +-- ulang selama !isValid
       |
       +-- deleteAtParent(&L, nip)
             |
             +-- isEmpty(*L) -> err("Data Sales Masih Kosong")
             +-- traversal dengan dua penunjuk:
             |     salesSebelumnya = NULL
             |     salesTarget     = L->first
             |     while (belum cocok && salesTarget != NULL) geser keduanya
             +-- salesTarget == NULL -> err("Data Sales Tidak Ditemukan")
             +-- haveChild(salesTarget) -> err("Sales Masih Memiliki Properti, Alihkan ...")
             +-- lepas rantai:
             |     salesSebelumnya == NULL -> L->first = salesTarget->nextParent
             |     di tengah/akhir    -> salesSebelumnya->nextParent = salesTarget->nextParent
             +-- free(salesTarget)
             +-- suc("Data Sales Berhasil Dihapus")
```

`nipBaru` belum dipakai untuk memindahkan properti; pengalihan properti sesuai
PRD poin 4 masih menjadi pekerjaan berikutnya.

### 4.5 Alur `[4] Lihat Semua Sales`

```
printAllParent(L)
  |
  +-- isEmpty(L) -> err("Data Sales Masih Kosong")
  +-- salesSekarang = L.first
  +-- while (salesSekarang != NULL)
  |     +-- printParent(salesSekarang)
  |           -> NIP, Nama, Nomor Telepon, Total Penjualan
  |     +-- salesSekarang = salesSekarang->nextParent
  +-- printf("Jumlah Sales : %d", countParent(L))
```

`countParent` melakukan traversal terpisah dari `L.first` sampai `NULL` untuk menghitung node.

---

## 5. Peta Alur Fungsi Parent

| Fungsi | Dipanggil dari | Alur singkat |
| --- | --- | --- |
| `inisialisasi` | `main` | `L->first = NULL` sebelum list dipakai. |
| `isEmpty` | semua fungsi insert/delete/print | `L.first == NULL` → `true`. |
| `haveChild` | `deleteFirstParent`, `deleteLastParent`, `deleteAtParent` | Cek `sales->firstChild`. |
| `alokasiParent` | `insertFirstParent`, `insertLastParent`, `insertAfterParent` | `malloc` → salin data → `nextParent`/`firstChild` = `NULL`. |
| `findParent` | insert/update/delete, semua fungsi child | Rekursif: cek head → cocok? → kembalikan node, atau lanjut ke `nextParent`. |
| `countParent` | `printAllParent` | Traversal `L.first` sampai `NULL`, menghitung node. |
| `insertFirstParent` | `insertAfterParent` saat list kosong | `findParent` (duplikat) → alokasi → sambung di depan → `suc`. |
| `insertLastParent` | menu `[1] Tambah Sales` | `findParent` (duplikat) → alokasi → sambung di belakang → `suc`. |
| `insertAfterParent` | (tersedia sebagai fungsi) | List kosong → `insertFirstParent`; sebaliknya cari acuan → cek duplikat → sambung setelah acuan. |
| `updateParent` | menu `[2] Ubah Sales` | `findParent` NIP lama → validasi NIP baru → `strcpy` field → `suc`. |
| `deleteFirstParent` | `deleteLastParent` saat satu elemen | Cek kosong → `haveChild` → geser head → `free` → `suc`. |
| `deleteLastParent` | (tersedia sebagai fungsi) | Cek kosong → satu elemen? delegasi → traversal ke node sebelum terakhir → `haveChild` → putus → `free`. |
| `deleteAtParent` | menu `[3] Hapus Sales` | Traversal dua penunjuk → `haveChild` → lepas rantai → `free` → `suc`. |
| `printParent` | `printAllParent` | Cetak empat field satu sales. |
| `printAllParent` | menu `[4] Lihat Semua Sales` | Traversal → `printParent` tiap node → `countParent`. |

Rantai pemanggilan khas untuk operasi sales:

```
main -> insertLastParent -> findParent
                         -> alokasiParent -> malloc
main -> updateParent     -> findParent -> strcpy
main -> deleteAtParent   -> isEmpty -> haveChild -> free
main -> printAllParent   -> printParent + countParent
```

---

## 6. Alur Submenu Properti

```
case 2 (menu utama)
 |
 +-- LOOP (do-while)
 |    |
 |    +-- menuPanah("MENU PROPERTI", itemProperti, 4)
 |    +-- switch (pilihProperti)
 |    |    +-- 1 -> insertLastChild(&L, nip, dataProperti)
 |    |    +-- 2 -> updateChild(&L, nip, noProp, dataPropBaru)
 |    |    +-- 3 -> deleteAtChild(&L, nip, noProp)
 |    |    +-- 0 -> keluar submenu (item "Kembali")
 |    |    +-- default -> err("Pilihan Tidak Tersedia")
 |    |
 |    +-- if (pilihProperti != 0) getch()   -> jeda, kecuali saat "Kembali"
 |    +-- ulang selama pilihProperti != 0
 |
 +-- break -> kembali ke menu utama
```

Sebelum memilih aksi, user diminta NIP sales; bila sales tidak ditemukan atau
belum punya properti, pesan error ditampilkan dan iterasi dihentikan. Fungsi
child dipanggil langsung dari `main`; detail alurnya dijelaskan di bagian berikutnya.

---

## 7. Alur Fungsi Child (Properti)

### 7.1 Pola umum

Setiap fungsi child diawali dengan mencari sales pemilik properti:

```
fungsi child(ListParent *L, string nipSales, ...)
 |
 +-- P = findParent(*L, nipSales)
 |     |
 |     +-- P == NULL -> pesan "Parent tidak ada" -> selesai
 |     +-- P ketemu  -> lanjut ke P->firstChild
 |
 +-- operasi pada P->firstChild
       |
       +-- insert / update / delete / tampil
```

`P` adalah `addressParent` sales yang bersangkutan; seluruh properti berada di
rantai `P->firstChild -> nextChild -> ...`.

### 7.2 Alur pembuatan dan pencarian node

```
alokasiChild(DataChild data)
  -> malloc(sizeof(NodeChild))
  -> C->dataChild = data
  -> C->nextChild = NULL
  -> kembalikan C ke fungsi pemanggil

findChild(addressChild C, string nomorProperti)
  -> C == NULL ? kembalikan NULL
  -> nomor cocok ? kembalikan C
  -> selainnya: findChild(C->nextChild, nomorProperti)   [rekursif]
```

### 7.3 Alur insert

**`insertFirstChild`**

```
P ditemukan -> C = alokasiChild(dataBaru)
            -> C->nextChild = P->firstChild
            -> P->firstChild = C
            -> pesan "Insert First Child"
```

Properti baru menjadi kepala list child sales tersebut.

**`insertLastChild`**

```
P ditemukan -> C = alokasiChild(dataBaru)
            -> P->firstChild == NULL ? P->firstChild = C
            -> selainnya:
                 temp = P->firstChild
                 while (temp->nextChild != NULL) temp = temp->nextChild
                 temp->nextChild = C
            -> pesan "Insert Last Child"
```

**`insertAtChild`**

```
P ditemukan -> posisi == 1 -> insertFirstChild(L, nipSales, dataBaru)
            -> posisi > 1:
                 temp = P->firstChild
                 i = 1
                 while (temp != NULL && i < posisi - 1) { temp = temp->nextChild; i++ }
                 C = alokasiChild(dataBaru)
                 C->nextChild = temp->nextChild
                 temp->nextChild = C
            -> pesan "Insert At Child"
```

### 7.4 Alur update

```
updateChild(L, nipSales, nomorProperti, dataBaru)
 |
 +-- P = findParent(*L, nipSales)
 +-- C = findChild(P->firstChild, nomorProperti)
 +-- C == NULL -> "Data Properti Tidak Ditemukan"
 +-- C ketemu:
 |     -> nomor properti lama disalin ke dataBaru
 |     -> C->dataChild = dataBaru
 |     -> "Data Properti Berhasil Diubah"
```

Jadi kunci `nomorProperti` dipakai untuk mencari node, lalu isi `DataChild`
disalin sebagai satu paket ke node tersebut.

### 7.5 Alur delete

**`deleteFirstChild`**

```
P ditemukan -> P->firstChild == NULL ? "Data Properti Kosong"
            -> hapus = P->firstChild
            -> P->firstChild = hapus->nextChild
            -> free(hapus)
            -> "Delete First Child"
```

**`deleteLastChild`**

```
P ditemukan -> kosong? -> "Data Properti Kosong"
            -> satu elemen? -> free(P->firstChild); P->firstChild = NULL
            -> lebih dari satu:
                 C = P->firstChild
                 while (C->nextChild->nextChild != NULL) C = C->nextChild
                 free(C->nextChild)
                 C->nextChild = NULL
            -> "Delete Last Child"
```

**`deleteAtChild`**

```
P ditemukan -> P->firstChild == NULL ? "Data Properti Kosong"
            -> head cocok? -> deleteFirstChild(L, nipSales)
            -> selainnya:
                 C = P->firstChild
                 while (C->nextChild != NULL && nomor C->nextChild != target)
                     C = C->nextChild
                 C->nextChild == NULL ? "Data Properti Tidak Ditemukan"
                 hapus = C->nextChild
                 C->nextChild = hapus->nextChild
                 free(hapus)
            -> "Delete At Child"
```

### 7.6 Alur tampil

```
tampilProperti(addressChild C)
  -> C == NULL ? selesai
  -> cetak "[nomor] jenis | alamat | Rp harga"
  -> tampilProperti(C->nextChild)     [rekursif]
```

### 7.7 Peta alur fungsi child

| Fungsi | Alur pemanggilan |
| --- | --- |
| `alokasiChild` | Dipanggil semua fungsi insert; mengembalikan node ke pemanggil. |
| `findChild` | Dipanggil `updateChild`; rekursif di `nextChild`. |
| `insertFirstChild` | `findParent` → `alokasiChild` → sambung di head. |
| `insertLastChild` | `findParent` → `alokasiChild` → traversal → sambung di tail. |
| `insertAtChild` | `findParent` → (`insertFirstChild` atau traversal + `alokasiChild`). |
| `updateChild` | `findParent` → `findChild` → assign `DataChild`. |
| `deleteFirstChild` | `findParent` → geser head → `free`; dipanggil juga `deleteAtChild`. |
| `deleteLastChild` | `findParent` → traversal ke sebelum tail → `free`. |
| `deleteAtChild` | `findParent` → (`deleteFirstChild` atau traversal dua penunjuk) → `free`. |
| `tampilProperti` | Rekursif dari `firstChild` sampai `NULL`. |

---

## 8. Alur Jual Properti

```
case 3 (menu utama)
 |
 +-- if (isEmpty(L)) -> err("Data sales masih kosong") -> getch() -> selesai
 +-- printSalesDanProperti(L)              -> tampilkan data terupdate
 |
 +-- LOOP NIP sales (kosong? -> batal, findParent NULL? -> error)
 +-- LOOP nomor properti (kosong? -> batal, findChild NULL? -> error)
 |     +-- propDicari = findChild(salesDicari->firstChild, noProp)
 |
 +-- tampil detail transaksi (nama sales, properti, harga)
 +-- LOOP konfirmasi: "y" / "n" (selain itu -> error)
 |     +-- "n" -> err("Transaksi Penjualan dibatalkan")
 |     +-- "y" -> jualProperti(&L, nip, noProp, &T)
 |                  |
 |                  +-- P = findParent(*L, nipSales)
 |                  +-- C = findChild(P->firstChild, nomorProperti)
 |                  +-- P->dataParent.totalPenjualan += C->dataChild.harga
 |                  +-- insertTransaksi(T, transaksi) -> simpan data sebelum dihapus
  |                  +-- deleteAtChild(L, nipSales, nomorProperti)
 |                  +-- suc("Transaksi Berhasil dan Total Penjualan sales telah diperbarui")
 +-- getch() -> kembali ke loop menu utama
```

Efek transaksi: node properti dilepas dari list child (jadi tidak lagi masuk
laporan "belum terjual"), dan harganya terakumulasi ke `totalPenjualan` sales
(muncul di laporan daftar nama sales).

---

## 9. Alur Laporan Owner

```
case 4 (menu utama)
 |
 +-- LOOP (do-while)
 |    |
 |    +-- menuPanah("MENU LAPORAN", itemLaporan, 5)
 |    +-- switch (pilihLaporan)
 |    |      |
 |    |      +-- 1 -> laporanNamaSales(L)
 |    |      |          -> if (isEmpty(L)) -> error
 |    |      |          -> P = L.first; while (P != NULL)
 |    |      |               -> NIP, nama, telepon, totalPenjualan
 |    |      |               -> C = P->firstChild; while (C != NULL) -> hitung properti
 |    |      |          -> P = P->nextParent
 |    |      |          -> diakhiri "Jumlah Sales : N"
 |    |      |
 |    |      +-- 2 -> printSalesDanProperti(L) -> data terupdate
 |    |      |          -> LOOP NIP (kosong? -> batal, findParent NULL? -> error)
 |    |      |          -> if (!haveChild(sales)) -> error, tanpa tanya jenis
 |    |      |          -> LOOP jenis ("rumah"/"tanah", strcmpi)
 |    |      |          -> laporanBelumTerjual(L, nip, jenis)
 |    |      |               -> C = P->firstChild; while (C != NULL)
 |    |      |               -> filter strcmpi(jenis), tampil yang cocok
 |    |      |               -> C = C->nextChild, akumulasi jumlah + total harga
 |    |      |
 |    |      +-- 3 -> laporanJumlahJenis(L)
 |    |      |          -> if (isEmpty(L)) -> error
 |    |      |          -> P = L.first; while (P != NULL): rekap Tanah/Rumah per sales
 |    |      |               -> C = P->firstChild; while (C != NULL): hitung per jenis
 |    |      |               -> C = C->nextChild, P = P->nextParent
 |    |      |          -> total semua sales
 |    |      |
 |    |      +-- 4 -> laporanPropertiTerjual(T)
 |    |      |          -> baca riwayat transaksi dari menu 3
 |    |      |          -> if (T.first == NULL) -> error
 |    |      |          -> Tr = T.first; while (Tr != NULL)
 |    |      |               -> nomor, jenis, alamat, harga, sales
 |    |      |               -> Tr = Tr->nextTransaksi, akumulasi jumlah + total harga
 |    |      |
 |    |      +-- 0 -> keluar submenu (item "Kembali")
 |    |      +-- default -> err("Pilihan Tidak Tersedia")
 |    |
 |    +-- if (pilihLaporan != 0) getch()
 |    +-- ulang selama pilihLaporan != 0
 |
 +-- break -> kembali ke menu utama
```

Keempat laporan bersifat read-only: tidak mengubah list. Properti yang sudah
dijual tidak muncul di laporan case 2 karena node-nya dihapus `jualProperti`.

Semua traversal di laporan memakai `while`, bukan `for`: pointer diinisialisasi
sebelum loop (`P = L.first`, `C = P->firstChild`, `Tr = T.first`), lalu
dimajukan di akhir badan loop dengan `P = P->nextParent`, `C = C->nextChild`,
atau `Tr = Tr->nextTransaksi`. Kondisi hentinya tetap `node != NULL`, sehingga
hasilnya identik dengan versi `for`.

Case 2 mencari sales dan jenis lewat input di `main.c`, lalu menyerahkan
pencetakan ke `laporanBelumTerjual`. Case 1, 3, dan 4 langsung memanggil
prosedur laporan masing-masing.

---

## 10. Alur Input dan Tampilan

### 10.1 Alur input

```
navigasi menu -> getch() -> menuPanah()       -> angka pilihan (0 = Exit/Kembali)
                            +-- Up (72) / Down (80) -> geser pilihan
                            +-- Enter (13)          -> konfirmasi
                            +-- Esc (27)            -> kembali (0)
teks          -> fflush(stdin) -> gets(...)    -> field bertipe string
jeda          -> if (pilihan != 0) getch()      -> menunggu satu tombol
```

`menuPanah` di `source.c` menggambar ulang menu setiap tombol ditekan dan
mengembalikan `0` untuk item terakhir (`Exit`/`Kembali`). Karena navigasi tidak
lagi memakai `scanf`, tidak ada newline sisa yang perlu dibersihkan sebelum
`gets`. `fflush(stdin)` tetap dipakai sebelum `gets` sebagai pengaman. `getch()`
membaca langsung dari keyboard, sehingga jeda tidak terlewat.

### 10.2 Alur pesan

```
err("...") -> warna merah -> "\n\t  [!] pesan [!]\n" -> reset warna
suc("...") -> warna hijau -> "\n\t  [*] pesan [*]\n" -> reset warna
```

Fungsi parent memakai `err`/`suc` untuk hasil operasi. Fungsi child mencetak
pesannya langsung dengan `printf` di akhir tiap operasi. Fungsi menu lama
(`showMenu`, `menuSales`, `menuProperti`, `menuLaporan`) masih tersedia di
`source.c` sebagai pencetak daftar, tetapi sudah tidak dipanggil dari `main`;
navigasi sekarang seluruhnya lewat `menuPanah`.

---

## 11. Contoh Alur Data End-to-End

Misal user memilih `[1] Sales`, `[1] Tambah Sales`, lalu mengisi Andi (1001),
Budi (1002), Citra (1003).

| Langkah | Pemanggilan | Kondisi `L` setelah langkah |
| --- | --- | --- |
| 1 | `inisialisasi(&L)` | `L.first = NULL` |
| 2 | `inisialisasiTransaksi(&T)` | `T.first = NULL` |
| 3 | `insertLastParent(&L, Andi)` | `1001` |
| 4 | `insertLastParent(&L, Budi)` | `1001 -> 1002` |
| 5 | `insertLastParent(&L, Citra)` | `1001 -> 1002 -> 1003` |
| 6 | `updateParent(&L, "1001", data)` | data Andi berubah, posisi tetap |
| 7 | `insertLastChild(&L, "1001", P01)` | `1001(firstChild: P01) -> 1002 -> 1003` |
| 8 | `printAllParent(L)` | cetak 3 sales + `Jumlah Sales : 3` |
| 9 | `tampilProperti(P->firstChild)` | cetak `P01` milik Andi |
| 10 | `jualProperti(&L, "1001", "P01", &T)` | `P01` dilepas, `totalPenjualan` Andi bertambah, riwayat `T` bertambah |
| 11 | `laporanBelumTerjual(L, "1001", "rumah")` | `P01` sudah tidak muncul sebagai belum terjual |
| 12 | `laporanPropertiTerjual(T)` | `P01` muncul sebagai properti terjual |
| 13 | `deleteAtParent(&L, "1001")` | `1002 -> 1003`, riwayat `T` tetap ada |

Pada langkah 7, alur masuk ke `findParent` untuk mencari Andi, lalu node properti disambung ke `firstChild` Andi. Pada langkah 10, `findParent` dan `findChild` dipakai sebelum harga diakumulasi, data disalin ke `T`, lalu node properti dilepas. Langkah 12 memperlihatkan bahwa menu 4 membaca riwayat transaksi menu 3, bukan lagi membaca list properti yang masih tersedia.
