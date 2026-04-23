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
