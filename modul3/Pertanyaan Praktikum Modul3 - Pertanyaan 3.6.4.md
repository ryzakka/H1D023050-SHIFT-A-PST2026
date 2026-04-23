# Dokumentasi Tugas Arduino: I2C LCD & Potensiometer

## 1) Cara Kerja Komunikasi I2C antara Arduino dan LCD
Komunikasi I2C (*Inter-Integrated Circuit*) beroperasi menggunakan dua jalur kabel utama: 
* **SDA (Serial Data):** Jalur untuk mengirim dan menerima data.
* **SCL (Serial Clock):** Jalur untuk sinkronisasi waktu pengiriman data.

Pada rangkaian ini, Arduino bertindak sebagai **Master** dan modul I2C yang menempel di belakang LCD bertindak sebagai **Slave**. 
1. Arduino (Master) memulai komunikasi dengan menghasilkan sinyal *clock* (SCL) dan memanggil alamat unik dari modul LCD I2C (dalam kode ini alamatnya adalah `0x27`).
2. Setelah LCD merespon, Arduino mengirimkan data *byte per byte* melalui jalur SDA. Data ini bisa berupa instruksi pengaturan (misalnya "pindah ke baris 2") atau karakter ASCII untuk ditampilkan di layar.
3. Library `Wire.h` menangani sinyal listrik I2C tingkat rendah, sedangkan `LiquidCrystal_I2C.h` menyederhanakannya menjadi perintah tingkat tinggi yang mudah dibaca, seperti `lcd.print()`.

---

## 2) Konfigurasi Pin Potensiometer
Aturan utama dalam pemasangan potensiometer adalah **pin tengah (wiper) wajib dihubungkan ke pin analog Arduino (seperti A0)**. Hal ini karena kaki tengah inilah yang mengeluarkan tegangan yang bervariasi (0V - 5V) saat *knob* diputar. Dua pin sisanya (kiri dan kanan) dihubungkan ke VCC (5V) dan GND.

**Apa yang terjadi jika pin kiri dan kanan (VCC dan GND) tertukar?**
Tidak akan terjadi korsleting atau kerusakan pada Arduino maupun potensiometer. Yang terjadi hanyalah **arah putarannya menjadi terbalik (reversibel)**. 
* Jika awalnya memutar *knob* ke kanan (searah jarum jam) membuat nilai ADC naik (0 ke 1023)...
* Maka saat kabel VCC dan GND ditukar posisinya, memutar *knob* ke kanan akan membuat nilai ADC justru turun (1023 ke 0).

---

## 3) Modifikasi Program (UART & I2C Output)
Berikut adalah modifikasi program agar data ditampilkan ke LCD dan Serial Monitor secara bersamaan, lengkap dengan penjelasan per baris kodenya.

```cpp
#include <Wire.h>               // Memanggil library untuk komunikasi I2C bawaan Arduino
#include <LiquidCrystal_I2C.h>  // Memanggil library khusus untuk mengontrol LCD via modul I2C
#include <Arduino.h>            // Memanggil library utama Arduino (opsional jika di Arduino IDE)

// Menginisialisasi objek LCD dengan alamat 0x27, jumlah kolom 16, dan jumlah baris 2
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0;          // Menetapkan pin A0 sebagai pin pembacaan potensiometer

void setup() {
  Serial.begin(9600);           // Membuka jalur komunikasi serial (UART) ke komputer dengan kecepatan 9600 bps
  
  lcd.init();                   // Melakukan inisialisasi awal pada layar LCD
  lcd.backlight();              // Menyalakan lampu latar (backlight) agar tulisan pada LCD terlihat
}

void loop() {
  int nilai = analogRead(pinPot); // Membaca tegangan analog dari A0 dan mengubahnya jadi angka digital (0 - 1023)
  
  // -- BLOK KALKULASI --
  // Menghitung nilai tegangan aktual: (Nilai ADC / 1023) * 5.0 Volt
  float voltase = nilai * (5.0 / 1023.0); 
  // Memetakan rentang ADC 0-1023 menjadi rentang persentase 0-100%
  int persen = map(nilai, 0, 1023, 0, 100);
  // Memetakan rentang ADC 0-1023 menjadi jumlah blok visual LCD 0-16 blok
  int panjangBar = map(nilai, 0, 1023, 0, 16);
  
  // -- BLOK OUTPUT SERIAL MONITOR (UART) --
  Serial.print("ADC: ");        // Mencetak teks "ADC: " ke Serial Monitor tanpa baris baru
  Serial.print(nilai);          // Mencetak nilai mentah ADC
  Serial.print("\tVolt: ");     // Mencetak spasi tabulasi (\t) dan teks "Volt: "
  Serial.print(voltase);        // Mencetak hasil perhitungan voltase
  Serial.print(" V\tPersen: "); // Mencetak teks dengan tabulasi
  Serial.print(persen);         // Mencetak nilai persentase
  Serial.println("%");          // Mencetak simbol "%" dan membuat baris baru (println)
  
  // -- BLOK OUTPUT LCD (I2C) --
  // Baris 1: ADC dan Persentase
  lcd.setCursor(0, 0);          // Memindahkan kursor LCD ke kolom pertama (0), baris pertama (0)
  lcd.print("ADC: ");           // Menampilkan teks "ADC: " di LCD
  lcd.print(nilai);             // Menampilkan angka ADC
  lcd.print("  ");              // Menambahkan spasi sebagai pemisah visual
  lcd.print(persen);            // Menampilkan angka persentase
  lcd.print("%   ");            // Menambahkan simbol % dan spasi ekstra untuk menimpa sisa karakter lama
  
  // Baris 2: Indikator Bar Level
  lcd.setCursor(0, 1);          // Memindahkan kursor LCD ke kolom pertama (0), baris kedua (1)
  for (int i = 0; i < 16; i++) { // Memulai perulangan dari 0 hingga 15 (total 16 kolom)
    if (i < panjangBar) {       // Jika posisi kolom saat ini (i) lebih kecil dari target panjang bar...
      lcd.print((char)255);     // ...cetak karakter kotak solid (ASCII 255)
    } else {                    // Jika tidak (melebihi batas panjang bar)...
      lcd.print(" ");           // ...cetak spasi kosong untuk menghapus sisa blok saat nilai turun
    }
  }
  
  delay(200);                   // Jeda waktu 200 ms agar pembacaan stabil dan layar tidak berkedip (flicker)
}
