# Penjelasan Kode — RealEstate XYZ (MultiList)

Dokumen ini menjelaskan isi `header.h`, `source.c`, dan `main.c` **sesuai kondisi kode saat ini**.
Fokus pembahasan: struktur data MultiList, **seluruh fungsi parent (Sales)**, dan
menu interaktif untuk mengelola sales.

> Status: fungsi parent dan menunya sudah lengkap serta lolos uji. Fungsi child
> (Properti), fitur JualProperti, dan Laporan Owner belum dibuat, sehingga
> menu 2–4 di program masih berupa pesan "belum tersedia".

---

## 1. Gambaran Umum

Perusahaan RealEstateXYZ punya dua entitas:

| Entitas | Peran di MultiList | Data yang dicatat |
| --- | --- | --- |
| Sales | **Parent** | NIP, Nama, Nomor Telepon, Total Penjualan |
| Properti | **Child** | Nomor Properti, Jenis (Tanah/Rumah), Alamat, Harga |

Satu sales bisa menangani banyak properti (relasi **one-to-many**), dan setiap sales
adalah node dalam satu linked list. Inilah yang disebut **MultiList**: list parent
(sales) yang setiap node-nya punya list child sendiri (properti).

---

## 2. Struktur File & Cara Kompilasi

```
RealEstate-Project-Fork/
├── header.h    -> tipe data, struct, prototipe fungsi
├── source.c    -> implementasi fungsi tampilan & MultiList
├── main.c      -> menu nested switch-case + input dari user
└── RealEstate.dev -> project file Dev-C++
```

Kompilasi lewat terminal:

```bash
gcc -Wall -Wextra main.c source.c -o RealEstate.exe
```

Jalankan dengan:

```bash
RealEstate.exe
```

Atau buka `RealEstate.dev` di Dev-C++ lalu tekan **F11**.

---

## 3. Struktur Data (`header.h`)

### 3.1 Tipe bantuan

```c
typedef char string[100];
typedef struct tChild *addressChild;
typedef struct tParent *addressParent;
```

- `string` adalah alias `char[100]`, jadi setiap field teks maksimal 99 karakter
  - penutup `'\0'`.
- `addressChild` / `addressParent` adalah **pointer** ke node. Ditulis terpisah
  (forward declaration) supaya `NodeParent` boleh menunjuk `NodeChild` dan
  sebaliknya tanpa urutan deklarasi yang rumit.

### 3.2 Data yang disimpan

```c
typedef struct {
    string nomorProperti;
    string jenis;        // "tanah" atau "rumah"
    string alamat;
    int harga;
} DataChild;             // isi data properti

typedef struct {
    string NIP;
    string nama;
    string nomorTelpon;
    int totalPenjualan;
} DataParent;            // isi data sales
```

Struct `Data*` sengaja dipisah dari `Node*`. Tujuannya: saat menambah data, kita
cukup mengirim satu paket `DataParent` ke fungsi insert, tanpa harus mengurus
pointer. Pointer hanya diurus di dalam fungsi alokasi.

### 3.3 Node dan list

```c
typedef struct tChild {
    DataChild dataChild;
    addressChild nextChild;   // properti berikutnya milik sales yang sama
} NodeChild;

typedef struct tParent {
    DataParent dataParent;
    addressParent nextParent; // sales berikutnya
    addressChild firstChild;  // kepala list properti milik sales ini
} NodeParent;

typedef struct {
    addressParent first;      // kepala list sales
} ListParent;
```

### 3.4 Bentuk di memori

```
ListParent
  └── first
        │
        ▼
   ┌─────────────┐   nextParent   ┌─────────────┐   nextParent
   │ Sales 1002  │ ─────────────> │ Sales 1001  │ ─────────────> NULL
   │ Budi        │                │ Andi        │
   └──────┬──────┘                └──────┬──────┘
          │ firstChild                   │ firstChild
          ▼                              ▼
   ┌─────────────┐   nextChild     ┌─────────────┐
   │ P01 Rumah   │ ─────────────>  │ P05 Tanah   │ ───────────> NULL
   └─────────────┘                 └─────────────┘
```

Dua hal penting:

1. `nextParent` menghubungkan sales ke sales lain (list parent).
2. `firstChild` adalah **akar list terpisah**, bukan bagian dari rantai parent.
   Jadi menghapus sales tidak otomatis menghapus propertinya — itu sebabnya
   `deleteAtParent` menolak jika sales masih punya properti.

---

## 4. Fungsi Tampilan

### 4.1 `err()` dan `suc()`

```c
void err(char *s) {
    printf("\033[1;31m");                 // teks merah terang
    printf("\n\t  [!] %s [!]\n", s);
    printf("\033[0m");                    // reset warna
}
```

`suc()` sama, hanya warnanya hijau (`\033[1;32m`) dan penandanya `[*]`.
Semua pesan berhasil/gagal di program lewat dua fungsi ini supaya formatnya seragam.

`\033[` adalah *ANSI escape sequence*. Di Windows 10/11 (Windows Terminal, cmd baru)
warna tampil normal. Di cmd lama bisa muncul kode aneh — itu batasan terminal,
bukan bug logika.

### 4.2 Fungsi menu

`showMenu()`, `menuSales()`, `menuProperti()`, `menuLaporan()` hanya bertugas
**mencetak** pilihan. Mereka tidak membaca input, sehingga input tetap ditangani
`main.c` (lihat bagian 7). Masing-masing diawali `system("cls")` agar layar bersih
sebelum menu baru tampil (khusus Windows).

---

## 5. Fungsi Dasar List

### 5.1 `inisialisasi(ListParent *L)`

```c
L->first = NULL;
```

Wajib dipanggil sebelum list dipakai. Tanpa ini, `L.first` berisi nilai sampah
dan `isEmpty` bisa salah menilai (menganggap list berisi data padahal tidak).

### 5.2 `isEmpty(ListParent L)`

Mengembalikan `true` jika `L.first == NULL`. Perhatikan parameter dikirim
**by value** (bukan pointer) karena fungsi ini hanya membaca, tidak mengubah list.
Menyalin satu struct berisi satu pointer sangat murah.

---

## 6. Fungsi Parent (Sales)

### 6.1 `haveChild(addressParent sales)`

**Tujuan:** mengecek apakah seorang sales masih menanggung properti.

```c
if (sales == NULL)          return false;   // pointer kosong = tidak punya
if (sales->firstChild != NULL) return true; // ada child pertama = punya properti
return false;
```

Dipakai `deleteAtParent` sebagai penjaga. Karena `firstChild` adalah kepala list
properti, cukup cek apakah pointer itu `NULL`.

### 6.2 `alokasiParent(DataParent dataBaru)`

**Tujuan:** membuat satu node sales baru di heap.

Langkah:

1. `malloc(sizeof(NodeParent))` memesan memori seukuran satu node.
2. Jika hasilnya `NULL` (memori habis), tampilkan error dan kembalikan `NULL`.
3. Salin seluruh `dataBaru` ke `salesBaru->dataParent` (assignment struct, bukan `strcpy`).
4. Set `nextParent = NULL` dan `firstChild = NULL` — **wajib**, karena `malloc`
   tidak membersihkan memori; tanpa inisialisasi, pointer berisi sampah dan
   traversal akan membaca alamat acak.
5. Kembalikan node baru.

Fungsi ini **tidak** menyambungkan node ke list. Penyambungan dilakukan
`insertFirstParent` / `insertLastParent`.

### 6.3 `findParent(ListParent L, string nipSales)`

**Tujuan:** mencari node sales berdasarkan NIP, mengembalikan `addressParent`
atau `NULL` jika tidak ada.

```c
if (isEmpty(L)) return NULL;
if (strcmp(L.first->dataParent.NIP, nipSales) == 0) return L.first;

temp.first = L.first->nextParent;      // "list tanpa node pertama"
return findParent(temp, nipSales);     // ulangi untuk sisa list
```

Ini versi **rekursif**. Setiap pemanggilan memeriksa satu node; kalau bukan
target, pencarian diulang untuk sisa list (`temp.first = L.first->nextParent`).

Konsekuensi yang perlu diketahui:

- List kosong menjadi *base case* (berhenti dan kembalikan `NULL`).
- Kedalaman rekursi = jumlah sales. Aman untuk data tugas, tapi kalau nanti
  jumlah sales sangat banyak, versi `while` lebih hemat stack.
- NIP bersifat **unik**, jadi berhenti di kecocokan pertama sudah benar.

### 6.4 `countParent(ListParent L)`

```c
salesSekarang = L.first;
while (salesSekarang != NULL) {
    jumlahSales++;
    salesSekarang = salesSekarang->nextParent;
}
```

Traversal standar sampai `NULL`, menghitung jumlah node. Akan dipakai laporan
owner (misalnya "total sales: N").

### 6.5 `insertFirstParent(ListParent *L, DataParent dataBaru)`

**Tujuan:** menambah sales di depan list. Urutan data jadi terbalik dari urutan input.

Alur:

1. **Validasi duplikat** — `findParent(*L, dataBaru.NIP)`. Kalau ketemu, tampilkan
   `NIP Sales Sudah Terdaftar` dan batal. Pengecekan ditaruh di sini supaya
   aturan "NIP unik" dijamin di lapisan data, bukan hanya di menu.
2. Alokasi node baru; kalau gagal, berhenti.
3. Sambungkan: `salesBaru->nextParent = L->first`, lalu `L->first = salesBaru`.

Urutan dua baris terakhir tidak boleh dibalik. Kalau `L->first` diubah dulu,
alamat node lama hilang dan list terputus (memory leak).

### 6.6 `insertLastParent(ListParent *L, DataParent dataBaru)`

**Tujuan:** menambah sales di belakang sehingga urutan tampil sama dengan urutan input.

Alur:

1. Validasi duplikat NIP (sama seperti insert first).
2. Alokasi node.
3. Jika list kosong, node baru langsung jadi `L->first`. Jika tidak, traversal
   `salesTerakhir` sampai `nextParent == NULL`, lalu `salesTerakhir->nextParent = salesBaru`.

Operasi ini O(n) karena harus berjalan ke ujung list. Untuk data tugas ini
tidak masalah; kalau perlu O(1), bisa ditambah pointer `last` di `ListParent`.

### 6.7 `insertAfterParent(ListParent *L, string nipSebelum, DataParent dataBaru)`

**Tujuan:** menyisipkan sales baru tepat setelah sales acuan (dicari lewat NIP).

Alur:

1. Kalau list masih kosong, delegasi ke `insertFirstParent`.
2. Cari sales acuan (`nipSebelum`). Tidak ada → `Data Sales Sebelumnya Tidak Ditemukan`.
3. Tolak kalau NIP sales baru sudah terdaftar.
4. Alokasi node, lalu sambungkan:

```c
salesBaru->nextParent = salesSebelum->nextParent;
salesSebelum->nextParent = salesBaru;
```

Baris pertama wajib lebih dulu. Kalau `salesSebelum->nextParent` ditimpa dulu,
alamat sisa list setelahnya hilang dan list terputus.

### 6.8 `updateParent(ListParent *L, string nipSales, DataParent dataBaru)`

**Tujuan:** mengubah data sales yang sudah ada. `nipSales` = NIP lama (kunci
pencarian), `dataBaru` = isi baru.

Alur:

1. Cari node dengan NIP lama. Tidak ada → `Data Sales Tidak Ditemukan`.
2. Jika NIP **diganti**, cek dulu apakah NIP baru sudah dipakai sales lain
   (`salesNipSama`). Kalau dipakai → `NIP Sales Sudah Digunakan`, batal.
   Kalau NIP tidak berubah, pengecekan ini dilewati supaya tidak "menemukan
   dirinya sendiri" dan menolak perubahan nama.
3. Salin `nama` dan `nomorTelpon` dengan `strcpy`.

**Yang sengaja tidak diubah:** `totalPenjualan`. Field ini akumulasi omzet dari
properti yang terjual, bukan data yang diinput manual. Kalau bisa diedit bebas,
laporan penjualan jadi tidak valid.

### 6.9 `deleteFirstParent(ListParent *L)`

**Tujuan:** menghapus sales pertama pada list.

Alur:

1. List kosong → `Data Sales Masih Kosong`.
2. `haveChild(L->first)` true → tolak, properti harus dialihkan dulu.
3. Simpan head ke `salesHapus`, geser `L->first = salesHapus->nextParent`, lalu `free`.

### 6.10 `deleteLastParent(ListParent *L)`

**Tujuan:** menghapus sales terakhir.

Alur:

1. List kosong → `Data Sales Masih Kosong`.
2. Kalau hanya satu elemen (`L->first->nextParent == NULL`), delegasi ke `deleteFirstParent`.
3. Selain itu, traversal sampai node **sebelum** terakhir:
   `while (salesSebelumnya->nextParent->nextParent != NULL)`.
4. Cek `haveChild` pada node terakhir. Kalau masih punya properti, hapus dibatalkan
   dan rantai tidak diubah.
5. Putuskan `salesSebelumnya->nextParent = NULL`, lalu `free(salesHapus)`.

Pengecekan child dilakukan sebelum pemutusan rantai supaya list tidak berubah
saat penghapusan ditolak.

### 6.11 `deleteAtParent(ListParent *L, string nipSales)`

**Tujuan:** menghapus node sales yang **tidak** punya properti.

Linked-list satu arah tidak bisa mundur, jadi penghapusan butuh dua penunjuk:

| Variabel | Peran |
| --- | --- |
| `salesTarget` | node yang sedang diperiksa / akan dihapus |
| `salesSebelumnya` | node tepat sebelum `salesTarget`; `NULL` artinya target di kepala list |

Alur:

1. List kosong → `Data Sales Masih Kosong`.
2. Geser kedua pointer sampai NIP cocok atau `salesTarget == NULL`.
3. Tidak ketemu → `Data Sales Tidak Ditemukan`.
4. `haveChild(salesTarget)` bernilai true → tolak dengan pesan agar properti
   dialihkan dulu. Ini mencegah properti "menggantung" tanpa penanggung jawab.
5. Lepas rantai:
   - target di awal (`salesSebelumnya == NULL`) → `L->first = salesTarget->nextParent`
   - target di tengah/akhir → `salesSebelumnya->nextParent = salesTarget->nextParent`
6. `free(salesTarget)` mengembalikan memori ke sistem.

Tanpa `free`, node tetap menempati memori walau sudah tidak bisa diakses
(memory leak).

### 6.12 `printParent(addressParent sales)`

**Tujuan:** menampilkan satu node sales. Dipanggil `printAllParent` dan menu
`[4] Lihat Semua Sales`.

```c
printf("\n\t === Data Sales ===");
printf("\n\t[-] NIP             : %s", sales->dataParent.NIP);
printf("\n\t[-] Nama            : %s", sales->dataParent.nama);
printf("\n\t[-] Nomor Telepon   : %s", sales->dataParent.nomorTelpon);
printf("\n\t[-] Total Penjualan : Rp %d", sales->dataParent.totalPenjualan);
```

### 6.13 `printAllParent(ListParent L)`

**Tujuan:** traversal seluruh list parent dan mencetak tiap sales.

Alur:

1. List kosong → `Data Sales Masih Kosong` (tidak mencetak apa pun).
2. Loop `salesSekarang` dari `L.first` sampai `NULL`, panggil `printParent`.
3. Cetak jumlah sales lewat `countParent(L)` sebagai penutup.

### 6.14 Ringkasan fungsi parent

| Fungsi | Kompleksitas | Mengubah list | Catatan |
| --- | --- | --- | --- |
| `haveChild` | O(1) | tidak | cek `firstChild` |
| `alokasiParent` | O(1) | tidak | `malloc` + inisialisasi pointer |
| `findParent` | O(n) | tidak | rekursif, kunci NIP |
| `countParent` | O(n) | tidak | hitung node |
| `insertFirstParent` | O(1) | ya | tolak NIP duplikat |
| `insertLastParent` | O(n) | ya | tolak NIP duplikat |
| `insertAfterParent` | O(n) | ya | cari NIP acuan dulu |
| `updateParent` | O(n) | ya | NIP baru divalidasi |
| `deleteFirstParent` | O(1) | ya | hanya jika tanpa child |
| `deleteLastParent` | O(n) | ya | delegasi ke delete first saat satu elemen |
| `deleteAtParent` | O(n) | ya | hanya jika tanpa child |
| `printParent` | O(1) | tidak | satu node |
| `printAllParent` | O(n) | tidak | traversal + jumlah sales |

---

## 7. Program Utama (`main.c`)

`main.c` hanya berisi fungsi `main`. Tidak ada fungsi bantu yang dideklarasikan atau
didefinisikan di file ini: semua fungsi lain prototipenya ada di `header.h` dan
implementasinya di `source.c`.

### 7.1 Struktur menu (nested switch-case)

`main` memakai dua `do-while` bersarang, masing-masing dengan `switch` di dalamnya.
Lapisan luar untuk menu utama, lapisan dalam untuk submenu Sales.

```
do {                                    // menu utama
    showMenu();
    scanf("%d", &menuUtama);

    switch (menuUtama) {
    case 1:                             // MENU SALES
        do {                            // submenu
            menuSales();
            scanf("%d", &pilihanSales);

            switch (pilihanSales) {
            case 1: ... insertLastParent(&L, dataSales); break;
            case 2: ... updateParent(&L, nipLama, dataSales); break;
            case 3: ... deleteAtParent(&L, nipLama); break;
            case 4: printAllParent(L); break;
            case 0: break;              // kembali ke menu utama
            default: err("Pilihan Tidak Tersedia"); break;
            }
            if (pilihanSales != 0)
                getch();
        } while (pilihanSales != 0);
        break;

    case 2: err("Menu Properti Belum Tersedia"); break;
    case 3: err("Fitur Jual Properti Belum Tersedia"); break;
    case 4: err("Menu Laporan Belum Tersedia"); break;
    case 0: break;                      // keluar
    default: err("Menu Tidak Tersedia"); break;
    }
} while (menuUtama != 0);
```

Kenapa `do-while`, bukan `while`? Karena menu harus tampil dan pilihan harus dibaca
minimal satu kali. Kalau memakai `while (menuUtama != 0)` dengan variabel yang belum
diisi, nilainya masih sampah dan program bisa langsung keluar.

Nilai `0` pada submenu hanya mengakhiri loop dalam (`pilihanSales != 0`), lalu
`break` mengembalikan kendali ke menu utama. Nilai `0` pada menu utama mengakhiri
loop luar sehingga program selesai.

### 7.2 Cara membaca input

Pola mengikuti praktikum GD6/GD7: angka dengan `scanf`, teks dengan `fflush(stdin)`
lalu `gets`, dan jeda dengan `getch`. Tidak ada fungsi bantu tambahan di `main.c`.

| Jenis input | Cara | Alasan |
| --- | --- | --- |
| nomor menu | `scanf("%d", &menuUtama)` | langsung membaca angka, tidak perlu konversi manual |
| teks (NIP, nama, telepon) | `fflush(stdin);` lalu `gets(dataSales.NIP);` | spasi ikut terbaca (`Andi Wijaya`); `fflush` membuang newline sisa `scanf` |
| jeda antar aksi | `getch();` dari `conio.h` | menunggu satu tombol tanpa perlu menekan Enter |

`fflush(stdin)` sebenarnya bukan operasi standar C, tetapi berfungsi di Windows
(Dev-C++/MinGW) dan memang dipakai di kode praktikum. Kalau program dipindah ke
Linux, bagian ini harus diganti `while (getchar() != '\n');`.

Jeda diletakkan sekali di akhir setiap iterasi submenu:

```c
if (pilihanSales != 0)
    getch();
```

Karena `getch()` membaca langsung dari keyboard (bukan buffer `stdin`), sisa newline
dari `scanf` tidak membuat jeda terlewati.

### 7.3 Alur submenu Sales

**`[1] Tambah Sales`**

1. Minta NIP. Kalau kosong (`strcmp(..., "") == 0`), tampilkan error dan kembali ke submenu.
2. Minta Nama dan Nomor Telepon.
3. Set `dataSales.totalPenjualan = 0` — sales baru belum menjual apa pun.
4. `insertLastParent(&L, dataSales)`. Dipilih *last* (bukan *first*) supaya urutan
   tampil sama dengan urutan input.
5. Pesan berhasil/gagal keluar dari `insertLastParent`, termasuk penolakan NIP duplikat.

**`[2] Ubah Sales`**

1. Minta NIP lama, lalu `findParent` dicek lebih dulu. Kalau tidak ada, proses
   dihentikan tanpa menyuruh user mengetik data baru.
2. Minta NIP baru. Kosong berarti NIP tetap: `strcpy(nipBaru, nipLama)`.
3. Minta Nama Baru dan Nomor Telepon Baru.
4. `updateParent(&L, nipLama, dataSales)` melakukan validasi NIP dan penyalinan field.

**`[3] Hapus Sales`**

1. Minta NIP.
2. `deleteAtParent(&L, nipLama)` menangani pencarian dan penolakan jika sales masih
   punya properti.

**`[4] Lihat Semua Sales`**

1. Memanggil `printAllParent(L)`.
2. Menampilkan NIP, nama, telepon, total penjualan tiap sales, lalu jumlah sales.
3. Kalau list kosong, muncul `Data Sales Masih Kosong`.

**`[0] Kembali`** keluar dari submenu tanpa mengubah list.

### 7.4 Menu yang masih placeholder

| Menu | Perilaku sekarang |
| --- | --- |
| `[2] Properti` | `err("Menu Properti Belum Tersedia")` |
| `[3] JualProperti` | `err("Fitur Jual Properti Belum Tersedia")` |
| `[4] Laporan Owner` | `err("Menu Laporan Belum Tersedia")` |

Semuanya hanya menampilkan pesan lalu `getch()`, sehingga struktur menu sudah
final dan tinggal diisi ketika fungsi child dibuat.

### 7.5 Hasil uji

Menu diuji dengan build sebelum pola input disesuaikan ke `scanf` + `fflush` +
`gets`. Perilakunya tidak berubah karena yang diganti hanya cara membaca input.
Fungsi parent sendiri diuji lewat driver terpisah:

| Skenario | Hasil |
| --- | --- |
| tambah 1001 Andi, 1002 Budi | `[*] Data Sales Berhasil Dimasukkan [*]` dua kali |
| tambah 1002 lagi | `[!] NIP Sales Sudah Terdaftar [!]` |
| ubah 1001 (NIP dikosongkan, nama "Andi Wijaya") | `[*] Data Sales Berhasil Diubah [*]` |
| hapus 1001 | `[*] Data Sales Berhasil Dihapus [*]` |
| `[4] Lihat Semua Sales` | daftar NIP/nama/telepon + jumlah sales |
| `insertAfterParent` setelah 1001 | urutan menjadi 1002, 1001, 1004, 1003 |
| `deleteFirstParent` / `deleteLastParent` | node ujung terhapus, jumlah berkurang |
| hapus sales yang punya child | ditolak, list tidak berubah |
| hapus saat list kosong | `[!] Data Sales Masih Kosong [!]` |
| `0` lalu `0` | keluar + pesan terima kasih |

---

## 8. Contoh Alur Data

Misal input berurutan: Andi (1001), Budi (1002), Citra (1003).

**Setelah `insertFirstParent` untuk Andi dan Budi, lalu `insertLastParent` untuk Citra:**

```
first -> [1002 Budi] -> [1001 Andi] -> [1003 Citra] -> NULL
```

Budi ada di depan karena `insertFirstParent` menyisipkan di kepala; Citra di
belakang karena `insertLastParent`. Lewat menu `[1] Sales`, program memakai
`insertLastParent`, sehingga urutan tampil sama dengan urutan input.

**`insertLastParent` dengan NIP 1002 lagi:**

```
[!] NIP Sales Sudah Terdaftar [!]
```

Jumlah sales tetap 3.

**`deleteAtParent(L, "1001")` saat Andi masih punya properti:**

```
[!] Sales Masih Memiliki Properti, Alihkan Properti Terlebih Dahulu [!]
```

Setelah properti Andi dialihkan/dikosongkan (nanti lewat fitur child), pemanggilan
yang sama berhasil dan list menjadi:

```
first -> [1002 Budi] -> [1003 Citra] -> NULL
```

---

## 9. Aturan Bisnis yang Sudah Diterapkan

| Aturan dari soal | Implementasi saat ini |
| --- | --- |
| NIP sales unik | validasi di `insertFirstParent`, `insertLastParent`, `insertAfterParent`, `updateParent` |
| Total penjualan adalah akumulasi | tidak bisa diubah manual di `updateParent` |
| Sales resign → properti dialihkan dulu | `deleteAtParent` menolak jika `haveChild` true |
| Sales dihapus setelah properti dialihkan | penghapusan menunggu list child kosong (fitur child menyusul) |
| Sales dikelola lewat menu | submenu `[1] Sales`: tambah, ubah, hapus, dan lihat semua sales |
| Data properti, jual properti, laporan | belum diimplementasikan (menu masih placeholder) |

---

## 10. Konvensi Penamaan

Variabel dibuat deskriptif agar mudah dibaca saat presentasi:

| Nama | Arti |
| --- | --- |
| `salesBaru` | node sales hasil alokasi yang belum tersambung |
| `salesSekarang` | node yang sedang ditunjuk saat traversal |
| `salesTerakhir` | node terakhir pada list (untuk insert di belakang) |
| `salesDiubah` | node sales yang datanya sedang diubah |
| `salesNipSama` | sales lain yang sudah memakai NIP baru (calon bentrok) |
| `salesSebelumnya` | node sebelum target pada proses hapus |
| `salesTarget` | node yang dicari / akan dihapus |
| `salesHapus` | node yang sudah dilepas dari rantai dan akan di-`free` |
| `salesSebelum` | node acuan pada `insertAfterParent` |
| `nipSebelum` | NIP sales acuan pada `insertAfterParent` |
| `jumlahSales` | hasil hitung `countParent` |
| `dataBaru` | paket data yang akan dimasukkan/dipakai |
| `dataSales` | paket data sales yang diisi dari input menu |
| `nipLama` | NIP sales yang dicari, diubah, atau dihapus |
| `nipBaru` | NIP pengganti saat ubah data (kosong = tetap) |
| `salesDicari` | hasil `findParent` untuk cek sebelum mengubah data |
| `menuUtama` | pilihan pada menu utama |
| `pilihanSales` | pilihan pada submenu Sales |

---

## 11. Catatan Teknis

- **`typedef char string[100]`** membuat parameter `string` sebenarnya
  `char[100]`. Kalau fungsi dipanggil dengan string literal
  (`findParent(L, "1003")`), GCC bisa memberi warning `-Wstringop-overflow`.
  Di program asli argumennya variabel `string nipSales`, jadi warning ini tidak muncul.
- **`malloc` tidak membersihkan memori.** Semua pointer di node baru harus
  di-set eksplisit; ini alasan `alokasiParent` mengisi `nextParent` dan `firstChild`.
- **`free` hanya membebaskan satu node.** Menghapus sales tidak otomatis
  membebaskan propertinya — itu sebabnya `deleteAtParent` dijaga `haveChild`.
- **`gets` membaca satu baris penuh tanpa menyimpan newline.** Cocok untuk nama
  yang mengandung spasi. Kekurangannya, panjang input tidak dibatasi, jadi buffer
  tujuan wajib bertipe `string` (100 byte) dan input harus wajar. GCC modern
  memberi warning `Using gets() is always unsafe`; untuk tugas ini warning itu
  bisa diabaikan, tetapi di program nyata sebaiknya pakai `fgets` + buang newline.
- **`fflush(stdin)` dipakai sebelum `gets`** untuk membuang newline sisa `scanf`.
  Ini perilaku non-standar C, tetapi berfungsi di Dev-C++/MinGW dan memang pola
  yang dipakai praktikum. Di Linux, ganti dengan `while (getchar() != '\n');`.
- **`getch()` membaca langsung dari keyboard**, bukan dari buffer `stdin`, sehingga
  sisa newline tidak membuat jeda terlewati. Karena itu `conio.h` ikut di-include
  di `header.h`.
- **Gaya penulisan mengikuti praktikum GD6/GD7:** indentasi 4 spasi, brace K&R
  (`{` di baris yang sama), banner `///////PARENT///////`, dan satu deklarasi per
  baris untuk struct.
- **Warning `argc`/`argv` tidak dipakai** berasal dari template `main` bawaan,
  bukan kesalahan logika. Bisa dihilangkan dengan menulis `int main(void)`.
- `system("cls")` dan pewarnaan ANSI membuat program berjalan baik di Windows.
  Di Linux, `cls` perlu diganti `clear`.

---

## 12. Langkah Berikutnya

1. Fungsi child: `alokasiChild`, `insertFirstChild`, `insertLastChild`,
   `findChild`, `deleteChild`, `countChild`.
2. Validasi nomor properti unik **global** (menelusuri semua sales).
3. Isi submenu `[2] Properti`: tambah, ubah, dan hapus properti milik sales tertentu.
4. `JualProperti` untuk menu `[3]`: hapus child dari list sales, lalu
   `totalPenjualan += harga properti`.
5. `SalesResign`: pindahkan seluruh child ke sales pengganti, lalu hapus parent
   (setelah itu `deleteAtParent` bisa dipakai karena `firstChild` sudah kosong).
6. Tiga laporan owner untuk menu `[4]`: daftar nama sales, properti belum terjual
   per jenis, dan jumlah properti per jenis.
