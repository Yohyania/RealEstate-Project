# Catatan Git — RealEstate Project

Dokumen ini merangkum seluruh langkah git yang sudah dijalankan maupun yang
masih direncanakan selama sesi kerja ini. Tujuannya supaya bisa mengulang
atau membatalkan langkah tanpa harus mengingat dari ingatan.

Dibuat: 2026-11-22

---

## 1. Peta Remote dan Branch

| Nama | Jenis | Target | Keterangan |
| --- | --- | --- | --- |
| `origin` | remote | github.com/Yosef-kelfin/RealEstate-Project-Fork | Fork milik sendiri. Satu-satunya target push. |
| `upstream` | remote | github.com/Yohyania/RealEstate-Project | Repo asli / teman. Hanya dibaca (fetch), tidak pernah di-push. |
| `main` | branch lokal | commit `f165300` | Branch utama. Sudah berisi kode terbaru repo asli. |
| `kelfins-edit` | branch lokal | commit `0a4028f` | Branch kerja berisi semua perubahan kita. Sudah di-push ke `origin`. |

Aturan penting dari `AGENT.md`: push hanya ke `origin`. Dilarang push ke
`upstream`, dilarang `push --force`, `reset --hard`, atau hapus branch
tanpa konfirmasi eksplisit.

---

## 2. Kronologi Perintah yang SUDAH dijalankan

### 2.1 Inspeksi awal

```bash
git status --short
git log --oneline -15
git branch --show-current
git remote -v
git diff --stat
git diff .gitignore
```

Tujuan: mengetahui kondisi awal sebelum mengubah apa pun.
Hasil: branch aktif `main`, remote `origin` dan `upstream` sudah terdaftar,
ada perubahan belum di-commit di `.gitignore`, `header.h`, `main.c`, `source.c`.

### 2.2 Kembalikan kode ke versi fork

```bash
git fetch origin
git log --oneline -5 origin/main
git rev-parse main origin/main
git status -sb
git checkout origin/main -- main.c source.c header.h
git status -sb
git diff --stat
git diff --stat origin/main -- main.c source.c header.h
```

Tujuan: membuang perubahan lokal lama dan memakai kode dari fork sesuai
permintaan. `fetch` dilakukan dulu untuk memastikan tidak ada commit baru.
Hasil: `origin/main` masih di `5e3807a`, sama dengan `main` lokal.
Ketiga file kode identik dengan fork. `.gitignore` dan `saran.md` sengaja
dibiarkan.

### 2.3 Perubahan kode dan dokumentasi (bukan perintah git)

- `main.c`: hapus blok `if (salesDicari->nextParent == NULL)` yang memakai
  pointer belum diinisialisasi; cukup `insertLastParent(&L, dataSales)`.
- `main.c`: validasi NIP/nama/telepon diganti dari `break` menjadi loop
  `do { ... } while (!valid)` per field, supaya field yang salah saja
  ditanyakan ulang.
- `header.h`, `source.c`: perubahan format/penyesuaian dari sesi sebelumnya.
- `README.md`: update alur 4.2 dan 4.3, tambah Daftar Isi.

### 2.4 Simpan perubahan ke branch baru

```bash
git status -sb
git diff --stat
git checkout -b kelfins-edit
git add .gitignore README.md header.h main.c source.c
git status --short
git commit -m "wip: validasi menu Sales dan pembaruan dokumentasi alur"
git status -sb
git log --oneline -3
git branch -v
```

Tujuan: memisahkan kode kita dari `main` supaya bisa dibalik kapan saja.
Penting: perintah ini dijalankan **sebelum** pindah branch, karena perubahan
yang belum di-commit akan ikut berpindah ke branch mana pun.
Hasil: commit `0a4028f` di `kelfins-edit`; `main` saat itu tetap di
`5e3807a`; working tree bersih. `AGENT.md` tidak ikut karena masuk
`.gitignore`.

### 2.5 Push ke fork

```bash
git status -sb
git branch --show-current
git ls-remote --heads origin kelfins-edit
git push -u origin kelfins-edit
git status -sb
git ls-remote --heads origin kelfins-edit
```

Tujuan: menyimpan branch kerja ke GitHub fork.
Hasil: `refs/heads/kelfins-edit` dibuat di `origin` pada commit `0a4028f`.
Tracking terpasang: `## kelfins-edit...origin/kelfins-edit`.
Sebelum push, halaman compare GitHub muncul *"There isn't anything to
compare"* karena branch head belum ada di server. Setelah push, hilang.

### 2.6 Cek posisi upstream untuk PR

```bash
git log --oneline -6 upstream/main
git merge-base kelfins-edit upstream/main
git log --oneline upstream/main..kelfins-edit
```

Tujuan: memastikan PR nanti hanya berisi commit kita.
Hasil: `upstream/main` sudah mengandung `5e3807a` lewat merge PR #5.
Merge-base = `5e3807a`, sehingga selisihnya hanya `0a4028f`.
Catatan: ref `upstream/main` saat itu masih `f165300` di lokal.

### 2.7 Tarik kode terbaru repo asli ke `main`

```bash
git checkout main
git fetch upstream
git merge upstream/main
```

Output:

```
Updating 5e3807a..f165300
Fast-forward
```

Artinya `main` lokal maju lurus dari `5e3807a` ke `f165300` tanpa commit
gabungan dan tanpa konflik, karena `5e3807a` masih leluhur langsung dari
`f165300`. File-file di working tree ikut terbarui sesuai repo asli.
Branch `kelfins-edit` tidak berubah.

---

## 3. Kondisi Saat Ini (Snapshot)

- Branch aktif: `main`
- `main` lokal: `f165300` (hasil fast-forward dari `upstream/main`)
- `kelfins-edit`: tetap di `0a4028f`, tracking ke `origin/kelfins-edit`
- `upstream/main` (ref lokal): `f165300`
- `origin/main` (fork): masih `5e3807a` belum di-push
- PR ke repo asli: **belum dibuat**
- `GIT_NOTES.md`: file baru, belum di-commit
- `AGENT.md`: ada di folder, tapi di-ignore git

---

## 4. SOP: Ambil Kode Terbaru Repo Asli ke `main` Lokal

```bash
git checkout main
git fetch upstream
git log --oneline main..upstream/main
git merge upstream/main
```

atau ringkas:

```bash
git checkout main
git pull upstream main
```

Penjelasan:

1. `checkout main` wajib dilakukan dulu. Kalau masih di `kelfins-edit`,
   perubahan upstream akan masuk ke branch kerja kita.
2. `fetch upstream` hanya mengunduh commit ke `upstream/main`, belum
   mengubah file.
3. `git log main..upstream/main` untuk melihat apa yang akan masuk.
4. `merge` menggabungkannya. Kalau posisinya lurus seperti kemarin, hasilnya
   *fast-forward* tanpa konflik.

Setelah itu cek:

```bash
git status
git log --oneline -3
```

---

## 5. SOP: Sinkronkan `kelfins-edit` dengan `main`

Kalau `main` sudah di-update dan branch kerja ikut perlu perubahan itu:

```bash
git checkout kelfins-edit
git merge main
git push origin kelfins-edit
```

Gunakan `merge`, bukan `rebase`, karena branch sudah di-push. `rebase`
akan menulis ulang commit dan butuh `push --force` yang dilarang.

---

## 6. SOP: Buat Pull Request ke Repo Asli

URL compare:

```
https://github.com/Yohyania/RealEstate-Project/compare/main...Yosef-kelfin:RealEstate-Project-Fork:kelfins-edit
```

Pastikan:

- **base repository**: `Yohyania/RealEstate-Project`, branch `main`
- **compare/head**: `Yosef-kelfin/RealEstate-Project-Fork`, branch
  `kelfins-edit` (ada huruf **s**)

Lalu klik **Create pull request**, isi judul dan penjelasan:
> Perbaikan validasi menu Sales dan pembaruan dokumentasi alur.
>
> - Menghapus pointer `salesDicari` yang belum diinisialisasi.
> - Validasi per field tanpa kembali ke menu.
> - Update alur README + Daftar Isi.

Setelah PR dibuat, bagikan URL `.../pull/<nomor>` ke pemilik repo asli.
URL compare hanya untuk melihat diff, bukan untuk merge.

---

## 7. SOP: Setelah PR di-merge

Fork tidak otomatis ikut ter-update. Lakukan:

```bash
git checkout main
git fetch upstream
git merge upstream/main
git push origin main
```

Branch `kelfins-edit` boleh dibiarkan sebagai arsip.

---

## 8. SOP: Kalau Tidak Disetujui

- Tetap di `main`, jangan merge branch kerja.
- `kelfins-edit` dan PR-nya bisa dibiarkan atau PR ditutup dari GitHub.
- Hapus branch lokal hanya kalau sudah yakin:

  ```bash
  git branch -D kelfins-edit
  ```

  Ini destruktif dan butuh konfirmasi.

---

## 9. Perintah yang Dilarang Tanpa Konfirmasi

```bash
git push upstream main     # push ke repo asli
git push --force           # menimpa riwayat remote
git reset --hard           # membuang perubahan kerja
git branch -D <branch>     # hapus branch
```

---

## 10. Troubleshooting

**"There isn't anything to compare"**
Biasanya branch head belum di-push, atau salah nama branch. Cek:

```bash
git branch --show-current
git ls-remote --heads origin kelfins-edit
```

**Konflik saat `merge upstream/main`**
Buka file yang konflik, pilih/perbaiki barisnya, lalu:

```bash
git add <file>
git commit
git push origin kelfins-edit
```

**Lupa branch aktif**

```bash
git status -sb
git branch --show-current
```

---

## 11. Cheat Sheet

| Kebutuhan | Perintah |
| --- | --- |
| Lihat status | `git status -sb` |
| Lihat perubahan | `git diff --stat` |
| Pindah branch | `git checkout <nama>` |
| Buat branch | `git checkout -b <nama>` |
| Simpan perubahan | `git add <file>` + `git commit -m "..."` |
| Push ke fork | `git push -u origin <branch>` |
| Ambil update upstream | `git fetch upstream` + `git merge upstream/main` |
| Lihat selisih commit | `git log --oneline main..upstream/main` |
| Lihat remote | `git remote -v` |
