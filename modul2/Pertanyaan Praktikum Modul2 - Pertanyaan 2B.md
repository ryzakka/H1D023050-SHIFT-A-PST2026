### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!

<img width="946" height="734" alt="image" src="https://github.com/user-attachments/assets/a67c70fb-e4e5-47f9-9963-2c132c937565" />
<img width="946" height="735" alt="image" src="https://github.com/user-attachments/assets/d6a1f181-cfd8-4b77-a561-ccd4036c8b97" />

**Komponen:**
* 1x Arduino Uno
* 1x 7-Segment Display (Common Cathode)
* 8x Resistor 220Ω (Pembatas arus untuk setiap segmen)
* 1x Pushbutton
* Kabel Jumper secukupnya

**Jalur Koneksi (Wiring):**
* **7-Segment ke Arduino:**
    * Pin `a` ➔ Resistor 220Ω ➔ Arduino Pin 7
    * Pin `b` ➔ Resistor 220Ω ➔ Arduino Pin 6
    * Pin `c` ➔ Resistor 220Ω ➔ Arduino Pin 5
    * Pin `d` ➔ Resistor 220Ω ➔ Arduino Pin 11
    * Pin `e` ➔ Resistor 220Ω ➔ Arduino Pin 10
    * Pin `f` ➔ Resistor 220Ω ➔ Arduino Pin 8
    * Pin `g` ➔ Resistor 220Ω ➔ Arduino Pin 9
    * Pin `dp` ➔ Resistor 220Ω ➔ Arduino Pin 4
    * Pin `Common` (Tengah) ➔ Arduino `GND`
* **Pushbutton ke Arduino:**
    * Kaki Terminal 1 ➔ Arduino Pin 2
    * Kaki Terminal 2 (Diagonal dari Terminal 1) ➔ Arduino `GND`

---

### 2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno? Apa keuntungannya dibandingkan rangkaian biasa?

Mode `INPUT_PULLUP` digunakan untuk mengaktifkan resistor *pull-up* internal (sekitar 20kΩ - 50kΩ) yang sudah tertanam di dalam mikrokontroler Arduino. 

* **Alasan Penggunaan:** Pin input digital sangat sensitif terhadap gangguan elektromagnetik. Jika tombol tidak ditekan, pin yang tidak terhubung ke mana-mana akan berada dalam kondisi "mengambang" (*floating state*), sehingga Arduino bisa membaca sinyal acak antara `HIGH` atau `LOW`. Resistor *pull-up* memastikan pin selalu membaca nilai `HIGH` (5V) secara stabil saat tombol tidak ditekan. Saat tombol ditekan, arus akan mengalir ke `GND`, dan pin akan membaca nilai `LOW` (0V).
* **Keuntungan:** Keuntungan utamanya adalah **menyederhanakan rangkaian perangkat keras**. Kita tidak perlu lagi memasang resistor *pull-up* atau *pull-down* eksternal di *breadboard*, sehingga menghemat penggunaan komponen, mengurangi kerumitan kabel, dan meminimalisir risiko kesalahan pemasangan resistor.

---

### 3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?

Jika hanya satu segmen tertentu (misalnya segmen 'b') yang mati sedangkan yang lain normal, penyebabnya dapat dianalisis dari dua sisi:

**Dari Sisi Hardware:**
1.  **Kabel Jumper Putus/Longgar:** Kabel penghubung dari Arduino ke resistor atau dari resistor ke pin 7-segment tidak menancap dengan baik di *breadboard*.
2.  **Resistor Rusak:** Resistor pembatas arus untuk segmen tersebut putus atau tidak terpasang dengan benar.
3.  **LED Segmen Internal Putus:** LED di dalam komponen 7-segment untuk bagian tersebut sudah terbakar/rusak secara fisik.
4.  **Pin Mikrokontroler Rusak:** Pin digital pada papan Arduino yang bertugas mengontrol segmen tersebut mengalami kerusakan *hardware* (I/O pin mati).

**Dari Sisi Software:**
1.  **Kesalahan Pola Biner (Array):** Nilai pada `digitPattern` untuk segmen tersebut salah ketik menjadi `0` (LOW), padahal seharusnya `1` (HIGH) untuk menyala.
2.  **Kesalahan Pin Mapping:** Nomor pin pada deklarasi array `segmentPins` tertukar atau merujuk ke pin Arduino yang salah/tidak terhubung.
3.  **Lupa Inisialisasi OUTPUT:** Pin yang bersangkutan tidak dideklarasikan di dalam perulangan fungsi `setup()` menggunakan `pinMode(pin, OUTPUT)`.

---

### 4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai penambahan (increment) dan pengurangan (decrement) pada sistem counter dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

# Modifikasi 7-Segment (Counter Up & Down) dengan Dua Pushbutton

<img width="833" height="271" alt="image" src="https://github.com/user-attachments/assets/657ba6e1-14cc-4e81-b91f-3271f8ed896f" />

Program ini mengontrol 7-Segment Display heksadesimal (0-F) menggunakan dua buah tombol. Tombol 1 berfungsi untuk menambah angka (Increment), sedangkan Tombol 2 berfungsi untuk mengurangi angka (Decrement).

**Modifikasi Rangkaian:**
Tambahkan satu pushbutton lagi. Hubungkan kaki terminal 1 ke **Pin 3** Arduino, dan kaki terminal 2 ke **GND**.

## Kode Program dan Penjelasan Baris

```cpp
#include <Arduino.h> // Memasukkan library inti Arduino

// Deklarasi urutan pin Arduino yang terhubung ke segmen a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Deklarasi pin untuk tombol penambahan (Increment)
const int btnIncPin = 2;
// Deklarasi pin untuk tombol pengurangan (Decrement)
const int btnDecPin = 3;

// Variabel untuk menyimpan nilai angka saat ini (dimulai dari 0)
int currentDigit = 0;

// Variabel untuk menyimpan status tombol increment sebelumnya (HIGH = tidak ditekan)
bool lastIncState = HIGH;
// Variabel untuk menyimpan status tombol decrement sebelumnya (HIGH = tidak ditekan)
bool lastDecState = HIGH;

// Array 2D berisi pola biner untuk menyalakan segmen membentuk karakter 0-9 dan A-F
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}, // 9
  {1,1,1,0,1,1,1,0}, // A
  {0,0,1,1,1,1,1,0}, // b
  {1,0,0,1,1,1,0,0}, // C
  {0,1,1,1,1,0,1,0}, // d
  {1,0,0,1,1,1,1,0}, // E
  {1,0,0,0,1,1,1,0}  // F
};

// Fungsi kustom untuk menampilkan angka ke 7-segment berdasarkan nilai 'num'
void displayDigit(int num) {
  // Melakukan looping 8 kali untuk mengatur status tiap pin segmen
  for(int i=0; i<8; i++) {
    // Menyalakan atau mematikan segmen berdasarkan referensi data array
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  // Looping untuk mengonfigurasi ke-8 pin segmen sebagai jalur keluar (OUTPUT)
  for(int i=0; i<8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Mengonfigurasi pin tombol Increment sebagai INPUT dengan resistor internal pull-up
  pinMode(btnIncPin, INPUT_PULLUP);
  // Mengonfigurasi pin tombol Decrement sebagai INPUT dengan resistor internal pull-up
  pinMode(btnDecPin, INPUT_PULLUP);
  
  // Menampilkan angka awal (0) saat Arduino pertama kali dinyalakan
  displayDigit(currentDigit);
}

void loop() {
  // Membaca sinyal saat ini dari tombol Increment
  bool currentIncState = digitalRead(btnIncPin);
  // Membaca sinyal saat ini dari tombol Decrement
  bool currentDecState = digitalRead(btnDecPin);

  // --- LOGIKA TOMBOL INCREMENT (TAMBAH) ---
  // Mengecek apakah tombol tambah baru saja ditekan (transisi HIGH ke LOW)
  if (currentIncState == LOW && lastIncState == HIGH) {
    currentDigit++; // Menambah nilai angka saat ini sebanyak 1
    
    // Jika nilai melebihi batas atas (15 atau F), kembalikan ke 0
    if (currentDigit > 15) {
      currentDigit = 0;
    }
    
    // Tampilkan angka yang baru ke 7-segment
    displayDigit(currentDigit);
    // Beri jeda 50ms untuk debouncing (menghindari deteksi getaran mekanik tombol ganda)
    delay(50);
  }
  // Simpan status tombol tambah saat ini untuk putaran loop selanjutnya
  lastIncState = currentIncState;

  // --- LOGIKA TOMBOL DECREMENT (KURANG) ---
  // Mengecek apakah tombol kurang baru saja ditekan (transisi HIGH ke LOW)
  if (currentDecState == LOW && lastDecState == HIGH) {
    currentDigit--; // Mengurangi nilai angka saat ini sebanyak 1
    
    // Jika nilai turun di bawah 0, ubah nilainya melompat ke 15 (F)
    if (currentDigit < 0) {
      currentDigit = 15;
    }
    
    // Tampilkan angka yang baru ke 7-segment
    displayDigit(currentDigit);
    // Beri jeda 50ms untuk debouncing
    delay(50);
  }
  // Simpan status tombol kurang saat ini untuk putaran loop selanjutnya
  lastDecState = currentDecState;
}
