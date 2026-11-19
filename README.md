# Alur Kode — RealEstate XYZ (MultiList)

Dokumen ini menjelaskan **alur eksekusi dan alur data** program: mulai dari `main`,
masuk ke menu, pemanggilan fungsi, hingga perubahan tersimpan di list. Fokusnya
adalah hubungan antarfungsi dan urutan langkah, bukan deklarasi variabel.

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
 +-- inisialisasi(&L)                     -> L.first = NULL
 |
 +-- LOOP menu utama (do-while)
 |    |
 |    +-- showMenu()                      -> mencetak pilihan
 |    +-- scanf("%d", &menuUtama)         -> membaca pilihan
 |    +-- switch (menuUtama)
 |         |
 |         +-- 1  -> LOOP submenu Sales
 |         +-- 2  -> LOOP submenu Properti
 |         +-- 3  -> err("Fitur Jual Properti Belum Tersedia") + getch()
 |         +-- 4  -> LOOP submenu Laporan
 |         +-- 0  -> keluar dari loop utama
 |         +-- default -> err("Menu Tidak Tersedia") + getch()
 |
 +-- system("cls")
 +-- suc("Terima Kasih Telah Menggunakan Program Ini")
```

`do-while` dipakai supaya menu tampil dan pilihan dibaca minimal sekali.
Keluar dari loop terjadi ketika `menuUtama == 0`.

---

## 4. Alur Submenu Sales

### 4.1 Pembacaan pilihan

```
case 1 (menu utama)
 |
 +-- LOOP (do-while)
 |    |
 |    +-- menuSales()                  -> mencetak 5 pilihan
 |    +-- scanf("%d", &pilihanSales)
 |    +-- switch (pilihanSales)
 |    |    |
 |    |    +-- 1 -> Tambah Sales
 |    |    +-- 2 -> Ubah Sales
 |    |    +-- 3 -> Hapus Sales
 |    |    +-- 4 -> Lihat Semua Sales
 |    |    +-- 0 -> keluar submenu
 |    |    +-- default -> err("Pilihan Tidak Tersedia")
 |    |
 |    +-- getch()
 |    +-- ulang selama pilihanSales != 0
 |
 +-- break -> kembali ke menu utama
```

### 4.2 Alur `[1] Tambah Sales`

```
baca NIP
  |
  +-- NIP kosong? -> err("NIP Tidak Boleh Kosong") -> kembali ke switch
  |
  +-- baca Nama
  +-- baca Nomor Telepon
  +-- dataSales.totalPenjualan = 0
  +-- insertLastParent(&L, dataSales)
        |
        +-- findParent(*L, NIP) -> cek apakah NIP sudah ada
        +-- alokasiParent(dataSales) -> malloc + isi data + nextParent/firstChild = NULL
        +-- L kosong? -> L->first = salesBaru
        |              atau
        |              traversal sampai nextParent == NULL -> sambung di ujung
        +-- suc("Data Sales Berhasil Dimasukkan")
```

Urutan tampilan mengikuti urutan input karena yang dipakai adalah `insertLastParent`.

### 4.3 Alur `[2] Ubah Sales`

```
isEmpty(L) ?
  |
  +-- ya  -> err("Data Sales Masih Kosong") -> kembali ke switch
  |
  +-- tidak
       |
       +-- baca nipLama
       +-- findParent(L, nipLama)
       |     |
       |     +-- NULL -> err("Data Sales Tidak Ditemukan") -> kembali ke switch
       |     +-- ketemu -> lanjut
       |
       +-- baca nipBaru
       |     +-- kosong -> strcpy(nipBaru, nipLama)  (NIP tetap)
       +-- dataSales.NIP = nipBaru
       +-- baca Nama Baru
       +-- baca Nomor Telepon Baru
       +-- updateParent(&L, nipLama, dataSales)
             |
             +-- findParent(*L, nipLama) -> node yang akan diubah
             +-- NIP berubah? -> findParent(*L, NIP baru) -> validasi
             +-- strcpy NIP (jika berubah), nama, nomorTelpon
             +-- suc("Data Sales Berhasil Diubah")
```

`totalPenjualan` tidak melewati jalur input: nilainya berasal dari akumulasi penjualan.

### 4.4 Alur `[3] Hapus Sales`

```
baca nipLama
baca nipPengganti            -> disiapkan untuk alur pengalihan properti
  |
  +-- deleteAtParent(&L, nipLama)
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
 |    +-- menuProperti()            -> mencetak: Tambah, Ubah, Hapus, Kembali
 |    +-- bacaan pilihan properti
 |    +-- switch (pilihan properti)
 |    |    +-- 1 / 2 / 3 -> kembali ke akhir iterasi loop
 |    |    +-- 0         -> keluar submenu
 |    |    +-- default   -> err("Pilihan Tidak Tersedia")
 |    |
 |    +-- getch()
 |    +-- ulang selama pilihan != 0
 |
 +-- break -> kembali ke menu utama
```

Alur di submenu ini berhenti di `switch`: belum ada pemanggilan fungsi child dari
`main`. Fungsi-fungsi child sendiri sudah tersedia di `source.c` dan alurnya
dijelaskan di bagian berikutnya.

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
 +-- err("Fitur Jual Properti Belum Tersedia")
 +-- getch()
 +-- kembali ke loop menu utama
```

Belum ada pemanggilan fungsi child pada jalur ini.

---

## 9. Alur Laporan Owner

```
case 4 (menu utama)
 |
 +-- LOOP (do-while)
 |    |
 |    +-- menuLaporan()
 |    |      -> [1] Daftar Nama Sales
 |    |      -> [2] Properti Belum Terjual per Jenis
 |    |      -> [3] Jumlah Properti per Jenis
 |    |      -> [0] Kembali
 |    |
 |    +-- bacaan pilihan laporan
 |    +-- switch (pilihan)
 |    |      -> 1 / 2 / 3 / 0 : kembali ke akhir iterasi
 |    |      -> default : err("Pilihan Tidak Tersedia")
 |    |
 |    +-- ulang selama pilihan != 0
 |
 +-- break -> kembali ke menu utama
```

Jalur ini belum memanggil fungsi pembaca data; alur berhenti di `switch`.

---

## 10. Alur Input dan Tampilan

### 10.1 Alur input

```
angka menu   -> scanf("%d", ...)              -> variabel pilihan
teks         -> fflush(stdin) -> gets(...)    -> field bertipe string
jeda         -> getch()                       -> menunggu satu tombol
```

`fflush(stdin)` mengosongkan newline yang tertinggal setelah `scanf`, sehingga
`gets` membaca baris teks berikutnya. `getch()` membaca langsung dari keyboard,
bukan dari buffer `stdin`, sehingga jeda tidak terlewat karena newline sisa.

### 10.2 Alur pesan

```
err("...") -> warna merah -> "\n\t  [!] pesan [!]\n" -> reset warna
suc("...") -> warna hijau -> "\n\t  [*] pesan [*]\n" -> reset warna
```

Fungsi parent memakai `err`/`suc` untuk hasil operasi. Fungsi child mencetak
pesannya langsung dengan `printf` di akhir tiap operasi. Fungsi menu
(`showMenu`, `menuSales`, `menuProperti`, `menuLaporan`) hanya mencetak daftar
pilihan; pembacaan pilihan tetap dilakukan di `main`.

---

## 11. Contoh Alur Data End-to-End

Misal user memilih `[1] Sales`, `[1] Tambah Sales`, lalu mengisi Andi (1001),
Budi (1002), Citra (1003).

| Langkah | Pemanggilan | Kondisi `L` setelah langkah |
| --- | --- | --- |
| 1 | `inisialisasi(&L)` | `L.first = NULL` |
| 2 | `insertLastParent(&L, Andi)` | `1001` |
| 3 | `insertLastParent(&L, Budi)` | `1001 -> 1002` |
| 4 | `insertLastParent(&L, Citra)` | `1001 -> 1002 -> 1003` |
| 5 | `updateParent(&L, "1001", data)` | data Andi berubah, posisi tetap |
| 6 | `insertLastChild(&L, "1001", P01)` | `1001(firstChild: P01) -> 1002 -> 1003` |
| 7 | `printAllParent(L)` | cetak 3 sales + `Jumlah Sales : 3` |
| 8 | `tampilProperti(P->firstChild)` | cetak `P01` milik Andi |
| 9 | `deleteAtChild(&L, "1001", "P01")` | child Andi kosong |
| 10 | `deleteAtParent(&L, "1001")` | `1002 -> 1003` |

Pada langkah 6, alur masuk ke `findParent` untuk mencari Andi, lalu node
properti disambung ke `firstChild` Andi. Pada langkah 9, `findParent` dipakai
lagi sebelum node properti dilepas dari rantai child.
