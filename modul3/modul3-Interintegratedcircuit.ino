// Memasukkan library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// Membuat objek 'lcd' dan mengatur alamat I2C (0x27), serta ukuran layar (16 kolom, 2 baris)
// Catatan: Ganti alamat ke 0x20 atau 0x3F jika layar tidak merespon/menampilkan karakter
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Mendeklarasikan konstanta pinPot di pin analog A0 untuk membaca tegangan potensiometer
const int pinPot = A0;

void setup() {
  // Membuka jalur komunikasi serial ke PC dengan kecepatan 9600 bps untuk Serial Monitor
  Serial.begin(9600);
  
  // Menjalankan inisialisasi awal agar layar LCD menyala dan siap menerima instruksi
  lcd.init();
  // Mengaktifkan lampu latar (backlight) pada layar LCD agar teks dapat terlihat
  lcd.backlight();
}

void loop() {
  // Membaca tegangan analog dari potensiometer di pin A0 
  // dan mengubahnya menjadi nilai digital murni (rentang 0 hingga 1023)
  int nilai = analogRead(pinPot);
  
  // -- Mapping ke bar (0-16) --
  // Mengkonversi rentang nilai 0-1023 menjadi rentang 0-16. 
  // Angka maksimal 16 dipilih karena sesuai dengan batas maksimal kolom karakter pada layar LCD
  int panjangBar = map(nilai, 0, 1023, 0, 16);
  
  // -- Tampilkan ke Serial --
  // Mengirimkan teks "Nilai ADC : " diikuti hasil pembacaan sensor ke Serial Monitor komputer
  Serial.print("Nilai ADC : ");
  Serial.println(nilai);
  
  // -- Baris 1: nilai ADC --
  // Memindahkan kursor LCD ke kolom 0 (paling kiri) dan baris 0 (baris pertama/atas)
  lcd.setCursor(0, 0);
  // Mencetak teks statis "ADC: " ke layar LCD
  lcd.print("ADC: ");
  // Mencetak nilai aktual dari potensiometer yang baru saja dibaca
  lcd.print(nilai);
  // Mencetak spasi ekstra di belakang nilai untuk menghapus/menimpa sisa digit sebelumnya 
  // (Fungsinya: misal nilai turun dari angka 1000 ke 99, spasi ini akan menghapus angka '0' yang tertinggal di layar)
  lcd.print("   "); // clear sisa
  
  // -- Baris 2: bar --
  // Memindahkan kursor LCD ke kolom 0 (paling kiri) dan baris 1 (baris kedua/bawah)
  lcd.setCursor(0, 1);
  
  // Melakukan perulangan (looping) sebanyak 16 kali untuk mengecek dan mengisi 16 petak di LCD
  for (int i = 0; i < 16; i++) {
    // Jika urutan petak saat ini (i) masih di bawah nilai kalkulasi 'panjangBar'...
    if (i < panjangBar) {
      // ...maka cetak karakter ASCII nomor 255 (karakter yang bentuknya blok kotak hitam penuh)
      lcd.print((char)255);
    } else {
      // ...jika petak sudah melewati batas 'panjangBar', cetak spasi kosong.
      // (Fungsinya untuk menghapus blok hitam saat knob potensiometer diputar turun)
      lcd.print(" ");
    }
  }
  
  // Memberikan jeda waktu selama 200 milidetik sebelum mengulangi loop.
  // Ini penting untuk mencegah layar LCD berkedip (flicker) akibat pembaruan layar yang terlalu cepat.
  delay(200);
}
