# Dokumentasi & Analisis Program Kontrol LED Arduino
---

## 1. Proses dari Input Keyboard hingga LED Menyala/Mati

Proses ini melibatkan komunikasi serial (UART) antara komputer dan mikrokontroler Arduino:

1. **Input Pengguna:** Pengguna mengetik karakter (misal '1' atau '0') di Serial Monitor (komputer) dan menekan *Enter*.
2. **Pengiriman Data:** Komputer mengubah karakter tersebut menjadi kode ASCII (misal '1' menjadi nilai desimal 49) dan mengirimkannya melalui kabel USB ke chip USB-to-Serial di papan Arduino.
3. **Penyimpanan di Buffer:** Chip pada Arduino menerima sinyal listrik tersebut dan menyimpannya sementara di memori penerimaan yang disebut **Serial Receive Buffer** (kapasitas standar 64 bytes).
4. **Pengecekan oleh Program:** Program Arduino yang sedang berjalan (di dalam fungsi `loop`) mengecek apakah ada data di buffer menggunakan fungsi `Serial.available()`.
5. **Pembacaan Data:** Jika ada, fungsi `Serial.read()` mengambil 1 byte pertama dari buffer dan memindahkannya ke dalam variabel (dalam hal ini variabel `data`).
6. **Eksekusi Logika:** Statement `if-else` mengevaluasi nilai variabel `data`. Jika isinya '1', program mengeksekusi `digitalWrite(PIN_LED, HIGH)`.
7. **Respon Fisik:** Fungsi `digitalWrite()` memerintahkan pin mikrokontroler (pin 12) untuk mengeluarkan tegangan 5V (HIGH) yang membuat arus mengalir sehingga LED menyala. Jika '0', tegangan diturunkan menjadi 0V (LOW) sehingga LED mati.

---

## 2. Fungsi `Serial.available()` dan Dampak Jika Dihilangkan

### Mengapa digunakan `Serial.available()`?
Fungsi ini digunakan untuk **mengecek jumlah antrean data (byte) yang siap dibaca** di dalam *buffer* penerima. Sebelum kita menyuruh Arduino membaca data dengan `Serial.read()`, kita harus memastikan datanya benar-benar ada.

### Apa yang terjadi jika baris tersebut dihilangkan?
Jika `if (Serial.available() > 0)` dihapus, program akan terus-menerus memaksa mengeksekusi `Serial.read()` setiap kali `loop()` berputar, bahkan saat kita tidak mengetik apa-apa. 
* Jika tidak ada data yang masuk, `Serial.read()` akan mengembalikan nilai `-1` (atau karakter kosong/sampah).
* Karena `-1` bukan '1', bukan '0', dan bukan tombol *Enter* (`\n` atau `\r`), maka blok kode `else if` terakhir akan terpenuhi.
* Akibatnya, Serial Monitor kamu akan **dibanjiri (spam) dengan tulisan "Perintah tidak dikenal"** tanpa henti dengan sangat cepat, dan program menjadi kacau.

---

## 3 & 4. Modifikasi Program (Blink) & Pemilihan Delay vs Millis

### Tentukan apakah menggunakan `delay()` atau `millis()`!
Kita **WAJIB menggunakan `millis()`**. 

**Penjelasannya:**
* **`delay()` bersifat *Blocking*:** Fungsi `delay(500)` akan menghentikan **seluruh** aktivitas mikrokontroler selama 500 milidetik. Jika kita menggunakan `delay()` untuk membuat LED berkedip, Arduino tidak akan bisa menjalankan `Serial.read()` dengan responsif. Saat Arduino sedang "berhenti" di tengah fungsi `delay()`, ia tidak akan langsung merespon jika kita menekan angka '1' atau '0' untuk mengganti mode.
* **`millis()` bersifat *Non-Blocking*:** `millis()` adalah *stopwatch* internal Arduino yang terus menghitung waktu sejak Arduino menyala. Dengan `millis()`, kita bisa membuat logika "cek waktu, jika sudah lewat 500ms, ubah status LED". Sambil menunggu 500ms tersebut tercapai, fungsi `loop()` tetap bisa berjalan bebas dengan sangat cepat untuk memantau input keyboard (Serial Monitor), sehingga sistem **jauh lebih responsif**.

### Kode Program Modifikasi (Penjelasan Per Baris)

Berikut adalah modifikasi kodenya beserta penjelasan di setiap barisnya:

```cpp
#include <Arduino.h> // Memanggil library utama Arduino (standar framework C++ untuk Arduino)

const int PIN_LED = 12; // Mendeklarasikan konstanta integer PIN_LED di pin 12
char statusMode = '0'; // Variabel untuk menyimpan mode saat ini. Default: '0' (Mati)
unsigned long waktuSebelumnya = 0; // Menyimpan catatan waktu (dalam milidetik) terakhir kali LED berkedip
const long intervalKedip = 500; // Mengatur jeda waktu kedipan LED sebesar 500 milidetik (setengah detik)
bool statusLED = LOW; // Variabel untuk menyimpan kondisi LED saat ini (Nyala atau Mati) saat mode Blink

void setup() { // Fungsi inisialisasi awal, dieksekusi sekali saat startup
  Serial.begin(9600); // Membuka jalur komunikasi serial dengan komputer pada kecepatan 9600 baud rate
  Serial.println("Ketik '1' ON, '0' OFF, '2' BLINK"); // Menampilkan menu instruksi awal di Serial Monitor
  pinMode(PIN_LED, OUTPUT); // Mengkonfigurasi pin 12 agar bertugas sebagai OUTPUT tegangan
  digitalWrite(PIN_LED, LOW); // Memastikan saat awal menyala, LED dalam kondisi mati (LOW)
} // Akhir dari fungsi setup

void loop() { // Fungsi utama yang akan berjalan berulang kali (looping) tanpa henti
  
  // --- BLOK 1: MEMBACA INPUT SERIAL ---
  if (Serial.available() > 0) { // Mengecek apakah ada karakter yang dikirim dari komputer ke buffer Arduino
    char dataMasuk = Serial.read(); // Jika ada, baca satu karakter tersebut dan simpan di variabel dataMasuk
    
    if (dataMasuk == '1' || dataMasuk == '0' || dataMasuk == '2') { // Jika input valid ('1', '0', atau '2')
      statusMode = dataMasuk; // Perbarui variabel statusMode dengan perintah yang baru
      
      if (statusMode == '1') { // Jika perintahnya adalah '1'
        Serial.println("MODE: LED MENYALA"); // Tampilkan konfirmasi di Serial Monitor
        digitalWrite(PIN_LED, HIGH); // Nyalakan LED secara permanen
      } 
      else if (statusMode == '0') { // Jika perintahnya adalah '0'
        Serial.println("MODE: LED MATI"); // Tampilkan konfirmasi di Serial Monitor
        digitalWrite(PIN_LED, LOW); // Matikan LED secara permanen
      } 
      else if (statusMode == '2') { // Jika perintahnya adalah '2'
        Serial.println("MODE: LED BERKEDIP"); // Tampilkan konfirmasi di Serial Monitor (Logika kedip ada di blok bawah)
      }
    } 
    else if (dataMasuk != '\n' && dataMasuk != '\r') { // Jika input BUKAN 1, 0, 2, dan BUKAN tombol Enter
      Serial.println("Perintah tidak dikenal"); // Berikan peringatan bahwa input salah
    } // Akhir evaluasi input salah
  } // Akhir blok pembacaan serial
  

  // --- BLOK 2: EKSEKUSI MODE BLINKING (NON-BLOCKING) ---
  if (statusMode == '2') { // Mengecek secara terus menerus apakah sistem sedang berada di mode '2'
    unsigned long waktuSekarang = millis(); // Catat waktu saat ini menggunakan fungsi millis()
    
    // Cek apakah selisih waktu sekarang dan waktu sebelumnya sudah melebihi atau sama dengan 500ms
    if (waktuSekarang - waktuSebelumnya >= intervalKedip) { 
      waktuSebelumnya = waktuSekarang; // Reset catatan waktu sebelumnya menjadi waktu yang sekarang
      
      if (statusLED == LOW) { // Membalikkan keadaan: Jika LED sedang mati (LOW)...
        statusLED = HIGH; // ...maka ubah variabel status menjadi nyala (HIGH)
      } else { // Sebaliknya, jika LED sedang nyala...
        statusLED = LOW; // ...maka ubah variabel status menjadi mati (LOW)
      } // Akhir pembalikan logika
      
      digitalWrite(PIN_LED, statusLED); // Terapkan status tegangan yang baru (HIGH/LOW) ke pin fisik LED
    } // Akhir pengecekan waktu interval
  } // Akhir dari blok mode '2'

} // Akhir dari fungsi loop utama
