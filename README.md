# CCRB Expense Tracker

Aplikasi pencatat keuangan harian berbasis **C++ (console)** yang mencatat pemasukan dan pengeluaran per dompet (tunai, e-wallet, dan rekening bank), menyimpannya ke file, dan terhubung dengan **bot Telegram** untuk mengirim laporan harian serta mencatat transaksi langsung dari chat.

Proyek ini dibuat untuk tugas kelompok mata pelajaran Informatika.

---

## Latar belakang

Banyak orang memakai beberapa e-wallet dan rekening sekaligus (GoPay, ShopeePay, DANA, OVO, BCA, dan lain-lain). Akibatnya, pengeluaran harian tersebar di banyak aplikasi dan sulit dipantau. Program ini mengumpulkan semua catatan itu di satu tempat, menghitung saldo tiap dompet, dan mengirim ringkasan pengeluaran ke Telegram.

## Fitur

| Menu | Fitur |
|---|---|
| 1 | Tambah pemasukan |
| 2 | Tambah pengeluaran (ditolak jika saldo dompet tidak cukup) |
| 3 | Lihat riwayat transaksi |
| 4 | Lihat saldo per dompet dan total |
| 5 | Kirim laporan pengeluaran hari ini ke Telegram |
| 6 | Mode bot: catat transaksi lewat chat Telegram |
| 7 | Hapus transaksi |
| 8 | Ubah transaksi |
| 0 | Keluar |

**Dompet yang didukung:** Tunai, GoPay, ShopeePay, BCA, DANA, OVO, LinkAja, BRI, BNI, Mandiri.

Data tersimpan otomatis ke `data.txt`, jadi tidak hilang saat program ditutup.

## Perintah di Telegram (menu 6)

Selama program berjalan di menu 6, kirim pesan ke bot dengan format berikut:

| Pesan | Hasil |
|---|---|
| `keluar 15000 makan siang gopay` | Mencatat pengeluaran Rp15.000 dari GoPay |
| `masuk 500000 uang saku bca` | Mencatat pemasukan Rp500.000 ke BCA |
| `keluar 8000 parkir` | Tanpa nama dompet, dicatat sebagai Tunai |
| `laporan` | Bot membalas laporan pengeluaran hari ini |
| `saldo` | Bot membalas saldo tiap dompet |
| `stop` | Keluar dari mode bot dan kembali ke menu |

Urutan pesan: **jenis, jumlah, keterangan, nama dompet**. Jumlah ditulis angka saja (`15000`, bukan `15.000` atau `15rb`). Huruf besar dan kecil tidak berpengaruh.

## Batasan

Supaya jelas apa yang bisa dan tidak bisa dilakukan program ini:

- Program **tidak terhubung langsung** ke GoPay, ShopeePay, BCA, atau layanan lain. Semua transaksi dicatat oleh pengguna, lewat menu atau lewat Telegram. Nama dompet hanya label kategori.
- Bot Telegram hanya merespons **selama program berjalan di menu 6**. Pesan yang dikirim saat program mati akan diproses saat menu 6 dibuka lagi.
- Laporan ke Telegram dikirim saat menu 5 dipilih, tidak otomatis setiap hari.
- Bot hanya menerima pesan dari chat ID yang tertulis di kode; pesan dari orang lain diabaikan.

## Struktur repositori

```
.
├── expense_tracker.cpp      Kode program
├── flowchart/               Flowchart (PNG) dan pseudocode
│   ├── 1_program_utama.png
│   ├── 2_tambah_transaksi.png
│   ├── 3_lihat_riwayat.png
│   ├── 4_saldo_per_dompet.png
│   ├── 5_kirim_telegram.png
│   ├── 6_mode_bot.png
│   ├── 7_proses_pesan.png
│   └── pseudocode.md
├── .gitignore
└── README.md
```

File berikut dibuat otomatis saat program berjalan dan **tidak** diunggah ke GitHub: `data.txt`, `pesan.txt`, `hasil_kirim.txt`, `update.txt`, `update_terakhir.txt`.

## Cara menjalankan

### Kebutuhan

- Compiler C++ (misalnya **Code::Blocks** versi `mingw-setup`, Dev-C++, atau g++)
- `curl` (sudah tersedia di Windows 10 dan 11)
- Koneksi internet, hanya untuk fitur Telegram (menu 5 dan 6)

### Dengan Code::Blocks

1. Buka `expense_tracker.cpp`.
2. Isi token bot dan chat ID (lihat bagian berikutnya).
3. Tekan **F9** (Build and Run).

### Dengan g++

```bash
g++ expense_tracker.cpp -o expense_tracker
```

```bash
./expense_tracker
```

Di Windows, jalankan `expense_tracker.exe`.

## Menyiapkan bot Telegram

1. Di Telegram, chat **@BotFather**, kirim `/newbot`, lalu ikuti petunjuknya sampai mendapat **token**.
2. Buka bot yang baru dibuat dan tekan **Start**.
3. Cari **chat ID** akun kamu: chat **@userinfobot**, tekan Start, lalu salin angka di baris `Id`.
4. Isi keduanya di bagian atas `expense_tracker.cpp`:

```cpp
const string TOKEN_BOT = "ISI_TOKEN_BOT_DI_SINI";
const string CHAT_ID   = "ISI_CHAT_ID_DI_SINI";
```

> **Penting:** token bot berfungsi seperti password. Jangan pernah mengunggah token asli ke GitHub, menampilkannya di video, atau membagikannya di chat. Sebelum commit, kembalikan kedua baris di atas ke placeholder. Kalau token sempat tersebar, kirim `/revoke` ke @BotFather untuk membuat token baru.

## Format penyimpanan data

Setiap transaksi disimpan sebagai satu baris di `data.txt`, dipisahkan tanda `|`:

```
tanggal|jenis|jumlah|nomor dompet|keterangan
2026-11-16|keluar|15000|1|makan siang
```

Nomor dompet mengikuti urutan daftar dompet di kode (0 = Tunai, 1 = GoPay, dan seterusnya). Karena itu, dompet baru harus selalu ditambahkan **di akhir** daftar `NAMA_DOMPET`, supaya data lama tidak berubah dompet.

## Konsep C++ yang dipakai

- Variabel, konstanta, dan tipe data (`int`, `long long`, `string`, `bool`)
- Percabangan (`if`, `switch`) dan perulangan (`for`, `while`, `do-while`)
- Function
- Array dan `struct`
- File handling (`ifstream`, `ofstream`)
- `stringstream` untuk memecah kalimat menjadi kata
- `system()` untuk menjalankan `curl` ke API Telegram

## Mengatasi masalah

| Masalah | Penyebab dan solusi |
|---|---|
| `g++ is not recognized` | Compiler belum terpasang. Pasang Code::Blocks versi `mingw-setup` |
| `cannot open output file ... Permission denied` | Program lama masih terbuka. Tutup jendelanya atau akhiri lewat Task Manager, lalu compile ulang |
| `blocked by your organization's Device Guard policy` atau `Process returned 32763` | Windows 11 **Smart App Control** memblokir program yang baru di-compile. Cek di Windows Security, bagian App & browser control |
| `Unauthorized` (401) | Token salah atau sudah di-revoke. Salin ulang token dari @BotFather |
| `Not Found` (404) | Format token di kode salah, misalnya ada spasi atau terpotong |
| `chat not found` (400) | Chat ID salah, atau belum menekan Start di bot |
| Bot diam di menu 6 | Chat ID di kode tidak sama dengan akun pengirim pesan |
| Pengeluaran ditolak "saldo tidak cukup" | Isi pemasukan ke dompet itu terlebih dahulu |

## Tim

| Peran | Nama |
|---|---|
| Analis Masalah | Bernardi |
| Telegram Bot Maker & Dokumenter |Christian|
| C++ Programmer |Cleon |
| Desainer Projek & Document |Renard|
