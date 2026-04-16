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
