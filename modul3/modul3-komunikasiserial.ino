#include <Arduino.h>

// ==========================================
// 1. BAGIAN DEKLARASI GLOBAL
// ==========================================
// Bagian ini berada di luar fungsi apa pun. 
// Digunakan untuk mendefinisikan pengaturan dasar atau variabel 
// yang akan digunakan di seluruh program. Di sini, kita menetapkan 
// bahwa pin nomor 12 pada papan Arduino diberi nama "PIN_LED".
const int PIN_LED = 12;


// ==========================================
// 2. FUNGSI setup()
// ==========================================
// Fungsi ini adalah tahap inisialisasi awal. 
// Arduino HANYA menjalankan fungsi ini SATU KALI saat pertama kali
// dinyalakan atau setelah tombol reset ditekan.
// Fungsinya di sini adalah untuk menyiapkan jalur komunikasi data 
// ke komputer, mencetak pesan sambutan, dan mengatur pin 12 (PIN_LED) 
// agar bertugas sebagai OUTPUT (pemberi sinyal listrik ke lampu LED).
void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1' untuk menyalakan LED, '0' untuk mematikan LED");
  pinMode(PIN_LED, OUTPUT);
}


// ==========================================
// 3. FUNGSI loop()
// ==========================================
// Fungsi ini adalah jantung dari program Arduino. 
// Setelah setup() selesai, fungsi loop() akan dijalankan TERUS-MENERUS
// tanpa henti selama Arduino mendapatkan daya listrik.
// Tugas utamanya di sini adalah memantau perintah dari pengguna 
// lalu menyalakan atau mematikan LED berdasarkan perintah tersebut.
void loop() {
  
  // -- BLOK PENGECEKAN INPUT --
  // Program secara aktif mengecek apakah pengguna mengetik sesuatu 
  // di Serial Monitor komputer. Jika ada data masuk, program akan 
  // membaca karakter tersebut dan menyimpannya di variabel 'data'.
  if (Serial.available() > 0) {       
    char data = Serial.read();        

    // -- BLOK PENGAMBILAN KEPUTUSAN (IF-ELSE) --
    // Program mengevaluasi isi dari variabel 'data' yang baru saja dibaca:
    // - Jika isinya angka '1', program menyalakan LED.
    // - Jika isinya angka '0', program mematikan LED.
    // - Jika isinya BUKAN '1', BUKAN '0', dan BUKAN tombol Enter, 
    //   program akan menganggap input salah dan mencetak pesan "Perintah tidak dikenal".
    if (data == '1') {
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ON");
    }
    else if (data == '0') {
      digitalWrite(PIN_LED, LOW);
      Serial.println("LED OFF");
    }
    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }
}
