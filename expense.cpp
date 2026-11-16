#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const string TOKEN_BOT = "9280844600:AAFP_yPhg8NgIr38SQGtyyx1QAuUj9Rz3Q8";
const string CHAT_ID   = "5665833261";

const string FILE_DATA = "data.txt";
const string FILE_UPDATE = "update_terakhir.txt";
const int MAKS = 1000;
const int JUMLAH_DOMPET = 10;
const string NAMA_DOMPET[JUMLAH_DOMPET] = {"Tunai", "GoPay", "ShopeePay", "BCA",
                                           "DANA", "OVO", "LinkAja", "BRI", "BNI", "Mandiri"};

struct Transaksi {
    string tanggal;
    string jenis;
    long long jumlah;
    int dompet;
    string keterangan;
};

Transaksi daftar[MAKS];
int jumlahData = 0;
string tanggalHariIni() {
    time_t sekarang = time(0);
    char teks[11];
    strftime(teks, sizeof(teks), "%Y-%m-%d", localtime(&sekarang));
    return teks;
}
long long bacaAngka(string pertanyaan, long long minimal, long long maksimal) {
    long long angka;
    while (true) {
        cout << pertanyaan;
        if (cin >> angka && angka >= minimal && angka <= maksimal) {
            cin.ignore(1000, '\n');
            return angka;
        }
        cout << "Input tidak valid, coba lagi.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}
string hurufKecil(string teks) {
    for (int i = 0; i < (int)teks.length(); i++)
        teks[i] = tolower(teks[i]);
    return teks;
}
string bersihkan(string teks) {
    for (int i = 0; i < (int)teks.length(); i++)
        if (teks[i] == '|') teks[i] = '/';
    return teks;
}
int cariDompet(string nama) {
    for (int i = 0; i < JUMLAH_DOMPET; i++)
        if (hurufKecil(NAMA_DOMPET[i]) == hurufKecil(nama)) return i;
    return -1;
}
void simpanData() {
    ofstream file(FILE_DATA);
    for (int i = 0; i < jumlahData; i++) {
        file << daftar[i].tanggal << "|" << daftar[i].jenis << "|"
             << daftar[i].jumlah << "|" << daftar[i].dompet << "|"
             << daftar[i].keterangan << "\n";
    }
}

void bacaData() {
    ifstream file(FILE_DATA);
    string jumlahTeks, dompetTeks;
    jumlahData = 0;
    while (jumlahData < MAKS && getline(file, daftar[jumlahData].tanggal, '|')) {
        getline(file, daftar[jumlahData].jenis, '|');
        getline(file, jumlahTeks, '|');
        getline(file, dompetTeks, '|');
        getline(file, daftar[jumlahData].keterangan);
        daftar[jumlahData].jumlah = atoll(jumlahTeks.c_str());
        daftar[jumlahData].dompet = atoi(dompetTeks.c_str());
        jumlahData++;
    }
}
long long efek(Transaksi t) {
    if (t.jenis == "masuk") return t.jumlah;
    return -t.jumlah;
}

long long hitungSaldo(int d) {
    long long saldo = 0;
    for (int i = 0; i < jumlahData; i++)
        if (daftar[i].dompet == d) saldo += efek(daftar[i]);
    return saldo;
}
void tambahTransaksi(string jenis) {
    if (jumlahData >= MAKS) {
        cout << "Data sudah penuh.\n";
        return;
    }
    Transaksi t;
    t.tanggal = tanggalHariIni();
    t.jenis = jenis;
    t.jumlah = bacaAngka("Jumlah (Rp): ", 1, 1000000000000LL);

    cout << "Pilih dompet:\n";
    for (int i = 0; i < JUMLAH_DOMPET; i++)
        cout << "  " << i + 1 << ". " << NAMA_DOMPET[i] << "\n";
    t.dompet = bacaAngka("Dompet: ", 1, JUMLAH_DOMPET) - 1;

    if (jenis == "keluar" && hitungSaldo(t.dompet) < t.jumlah) {
        cout << "Saldo " << NAMA_DOMPET[t.dompet] << " tidak cukup. Saldo sekarang: Rp"
             << hitungSaldo(t.dompet) << "\n";
        return;
    }

    cout << "Keterangan: ";
    getline(cin, t.keterangan);
    t.keterangan = bersihkan(t.keterangan);

    daftar[jumlahData] = t;
    jumlahData++;
    simpanData();
    cout << "Transaksi tersimpan.\n";
}

void lihatRiwayat() {
    if (jumlahData == 0) {
        cout << "Belum ada transaksi.\n";
        return;
    }
    cout << "\nNo | Tanggal    | Jenis  | Jumlah | Dompet | Keterangan\n";
    for (int i = 0; i < jumlahData; i++) {
        cout << i + 1 << " | " << daftar[i].tanggal << " | " << daftar[i].jenis
             << " | Rp" << daftar[i].jumlah << " | " << NAMA_DOMPET[daftar[i].dompet]
             << " | " << daftar[i].keterangan << "\n";
    }
}

void hapusTransaksi() {
    lihatRiwayat();
    if (jumlahData == 0) return;

    int no = bacaAngka("Nomor transaksi yang dihapus (0 = batal): ", 0, jumlahData);
    if (no == 0) {
        cout << "Dibatalkan.\n";
        return;
    }
    int idx = no - 1;
    Transaksi t = daftar[idx];

    if (hitungSaldo(t.dompet) - efek(t) < 0) {
        cout << "Tidak bisa dihapus: saldo " << NAMA_DOMPET[t.dompet] << " akan menjadi minus.\n";
        return;
    }

    cout << "Transaksi: " << t.jenis << " Rp" << t.jumlah << " " << t.keterangan
         << " (" << NAMA_DOMPET[t.dompet] << ")\n";
    if (bacaAngka("Yakin dihapus? (1 = ya, 0 = tidak): ", 0, 1) == 0) {
        cout << "Dibatalkan.\n";
        return;
    }

    for (int i = idx; i < jumlahData - 1; i++)
        daftar[i] = daftar[i + 1];
    jumlahData--;
    simpanData();
    cout << "Transaksi dihapus.\n";
}

void ubahTransaksi() {
    lihatRiwayat();
    if (jumlahData == 0) return;

    int no = bacaAngka("Nomor transaksi yang diubah (0 = batal): ", 0, jumlahData);
    if (no == 0) {
        cout << "Dibatalkan.\n";
        return;
    }
    int idx = no - 1;
    Transaksi baru = daftar[idx];

    cout << "Jumlah lama: Rp" << baru.jumlah << "\n";
    baru.jumlah = bacaAngka("Jumlah baru (Rp): ", 1, 1000000000000LL);

    cout << "Keterangan lama: " << baru.keterangan << "\n";
    cout << "Keterangan baru (kosongkan kalau tidak diubah): ";
    string ket;
    getline(cin, ket);
    if (ket != "") baru.keterangan = bersihkan(ket);

    if (hitungSaldo(baru.dompet) - efek(daftar[idx]) + efek(baru) < 0) {
        cout << "Tidak bisa diubah: saldo " << NAMA_DOMPET[baru.dompet] << " akan menjadi minus.\n";
        return;
    }

    daftar[idx] = baru;
    simpanData();
    cout << "Transaksi diubah.\n";
}
string buatTeksSaldo() {
    string teks = "Saldo per dompet\n";
    long long total = 0;
    for (int d = 0; d < JUMLAH_DOMPET; d++) {
        long long saldo = 0;
        for (int i = 0; i < jumlahData; i++) {
            if (daftar[i].dompet == d) {
                if (daftar[i].jenis == "masuk") saldo += daftar[i].jumlah;
                else saldo -= daftar[i].jumlah;
            }
        }
        teks += NAMA_DOMPET[d] + ": Rp" + to_string(saldo) + "\n";
        total += saldo;
    }
    teks += "Total: Rp" + to_string(total) + "\n";
    return teks;
}

void saldoPerDompet() {
    cout << "\n" << buatTeksSaldo();
}

string buatLaporanHarian() {
    string hariIni = tanggalHariIni();
    string laporan = "Laporan pengeluaran " + hariIni + "\n";
    long long total = 0;
    for (int i = 0; i < jumlahData; i++) {
        if (daftar[i].tanggal == hariIni && daftar[i].jenis == "keluar") {
            laporan += "- Rp" + to_string(daftar[i].jumlah) + " " + daftar[i].keterangan
                     + " (" + NAMA_DOMPET[daftar[i].dompet] + ")\n";
            total += daftar[i].jumlah;
        }
    }
    laporan += "Total: Rp" + to_string(total) + "\n";
    return laporan;
}

bool kirimPesan(string teks) {
    ofstream file("pesan.txt");
    file << teks;
    file.close();

    string perintah = "curl -s -o hasil_kirim.txt"
                      " --data-urlencode \"chat_id=" + CHAT_ID + "\""
                      " --data-urlencode \"text@pesan.txt\""
                      " \"https://api.telegram.org/bot" + TOKEN_BOT + "/sendMessage\"";
    return system(perintah.c_str()) == 0;
}

void kirimKeTelegram() {
    string laporan = buatLaporanHarian();
    cout << "\n" << laporan;

    if (!kirimPesan(laporan)) {
        cout << "Gagal menjalankan curl. Periksa koneksi internet.\n";
        return;
    }

    ifstream file("hasil_kirim.txt");
    string jawaban = "", baris;
    while (getline(file, baris)) jawaban += baris;
    file.close();

    if (jawaban.find("\"ok\":true") != string::npos) {
        cout << "Laporan berhasil dikirim. Cek Telegram kamu.\n";
    } else {
        cout << "Telegram menolak pesan. Jawaban Telegram:\n";
        if (jawaban == "") cout << "(kosong, tidak ada jawaban dari Telegram)\n";
        else cout << jawaban << "\n";
    }
}

string prosesPesan(string pesan) {
    string kata[50];
    int jumlahKata = 0;
    stringstream pemecah(pesan);
    while (jumlahKata < 50 && pemecah >> kata[jumlahKata]) jumlahKata++;

    if (jumlahKata == 0) return "Pesan kosong.";
    string perintah = hurufKecil(kata[0]);

    if (perintah == "laporan") return buatLaporanHarian();
    if (perintah == "saldo") return buatTeksSaldo();

    if (perintah == "masuk" || perintah == "keluar") {
        if (jumlahKata < 2)
            return "Jumlahnya belum ada. Contoh: keluar 15000 makan gopay";

        // Jumlah harus angka semua dan lebih dari 0
        string angka = kata[1];
        bool valid = angka.length() > 0 && angka.length() <= 12;
        for (int i = 0; i < (int)angka.length(); i++)
            if (angka[i] < '0' || angka[i] > '9') valid = false;
        if (!valid || atoll(angka.c_str()) <= 0)
            return "Jumlah harus berupa angka. Contoh: keluar 15000 makan gopay";

        if (jumlahData >= MAKS) return "Data sudah penuh.";

        Transaksi t;
        t.tanggal = tanggalHariIni();
        t.jenis = perintah;
        t.jumlah = atoll(angka.c_str());
        int akhir = jumlahKata;
        t.dompet = 0;
        if (jumlahKata > 2 && cariDompet(kata[jumlahKata - 1]) != -1) {
            t.dompet = cariDompet(kata[jumlahKata - 1]);
            akhir = jumlahKata - 1;
        }

        t.keterangan = "";
        for (int i = 2; i < akhir; i++) {
            if (i > 2) t.keterangan += " ";
            t.keterangan += kata[i];
        }
        if (t.keterangan == "") t.keterangan = "-";
        t.keterangan = bersihkan(t.keterangan);

        if (t.jenis == "keluar" && hitungSaldo(t.dompet) < t.jumlah)
            return "Saldo " + NAMA_DOMPET[t.dompet] + " tidak cukup. Saldo sekarang: Rp"
                 + to_string(hitungSaldo(t.dompet));

        daftar[jumlahData] = t;
        jumlahData++;
        simpanData();
        return "Tercatat: " + t.jenis + " Rp" + to_string(t.jumlah) + " " + t.keterangan
             + " (" + NAMA_DOMPET[t.dompet] + ")";
    }

    return "Perintah tidak dikenal. Yang bisa dipakai:\n"
           "keluar 15000 makan gopay\n"
           "masuk 500000 uang saku bca\n"
           "laporan\n"
           "saldo\n"
           "stop";
}
string ambilTeks(string data, int posisi) {
    string hasil = "";
    while (posisi < (int)data.length() && data[posisi] != '"') {
        if (data[posisi] == '\\' && posisi + 1 < (int)data.length()) {
            posisi++;
            if (data[posisi] == 'n') hasil += ' ';   // baris baru dijadikan spasi
            else hasil += data[posisi];
        } else {
            hasil += data[posisi];
        }
        posisi++;
    }
    return hasil;
}
void modeBot() {
    long long terakhir = 0;
    ifstream baca(FILE_UPDATE);
    baca >> terakhir;
    baca.close();

    cout << "\nMode bot aktif. Kirim pesan ke bot di Telegram, contoh:\n"
         << "  keluar 15000 makan gopay\n"
         << "Kirim \"stop\" di Telegram untuk kembali ke menu.\n";

    bool jalan = true;
    while (jalan) {
        string perintah = "curl -s -o update.txt \"https://api.telegram.org/bot" + TOKEN_BOT
                        + "/getUpdates?timeout=20&offset=" + to_string(terakhir + 1) + "\"";
        if (system(perintah.c_str()) != 0) {
            cout << "Gagal menghubungi Telegram. Periksa koneksi internet.\n";
            return;
        }

        ifstream file("update.txt");
        string data = "", baris;
        while (getline(file, baris)) data += baris;
        file.close();

        if (data.find("\"ok\":true") == string::npos) {
            cout << "Telegram menolak permintaan. Periksa TOKEN_BOT.\n";
            return;
        }
        const string KUNCI_ID = "\"update_id\":";
        const string KUNCI_CHAT = "\"chat\":{\"id\":";
        const string KUNCI_TEKS = "\"text\":\"";
        size_t posisi = data.find(KUNCI_ID);
        while (posisi != string::npos) {
            size_t berikut = data.find(KUNCI_ID, posisi + 1);
            string bagian = data.substr(posisi, berikut == string::npos ? string::npos : berikut - posisi);

            terakhir = atoll(bagian.substr(KUNCI_ID.length()).c_str());
            ofstream tulis(FILE_UPDATE);
            tulis << terakhir;
            tulis.close();
            string pengirim = "";
            size_t pc = bagian.find(KUNCI_CHAT);
            if (pc != string::npos) {
                pc += KUNCI_CHAT.length();
                while (pc < bagian.length() && bagian[pc] != ',' && bagian[pc] != '}')
                    pengirim += bagian[pc++];
            }

            size_t pt = bagian.find(KUNCI_TEKS);
            if (pengirim == CHAT_ID && pt != string::npos) {
                string pesan = ambilTeks(bagian, pt + KUNCI_TEKS.length());
                cout << "Pesan masuk: " << pesan << "\n";

                if (hurufKecil(pesan) == "stop") {
                    kirimPesan("Mode bot dihentikan.");
                    jalan = false;
                } else {
                    string balasan = prosesPesan(pesan);
                    cout << "Balasan    : " << balasan << "\n";
                    kirimPesan(balasan);
                }
            }
            posisi = berikut;
        }
    }
    cout << "Mode bot selesai.\n";
}
int main() {
    bacaData();
    int pilihan;
    do {
        cout << "\n===== PELACAK PENGELUARAN =====\n"
             << "1. Tambah pemasukan\n"
             << "2. Tambah pengeluaran\n"
             << "3. Lihat riwayat\n"
             << "4. Saldo per dompet\n"
             << "5. Kirim laporan hari ini ke Telegram\n"
             << "6. Mode bot (catat transaksi lewat Telegram)\n"
             << "7. Hapus transaksi\n"
             << "8. Ubah transaksi\n"
             << "0. Keluar\n";
        pilihan = bacaAngka("Pilih menu: ", 0, 8);

        switch (pilihan) {
            case 1: tambahTransaksi("masuk"); break;
            case 2: tambahTransaksi("keluar"); break;
            case 3: lihatRiwayat(); break;
            case 4: saldoPerDompet(); break;
            case 5: kirimKeTelegram(); break;
            case 6: modeBot(); break;
            case 7: hapusTransaksi(); break;
            case 8: ubahTransaksi(); break;
        }
    } while (pilihan != 0);

    cout << "Sampai jumpa!\n";
    return 0;
}
