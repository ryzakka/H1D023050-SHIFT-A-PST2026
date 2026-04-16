### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!

<img width="946" height="734" alt="image" src="https://github.com/user-attachments/assets/55cbe591-113d-4ec8-9286-3bb8aabd94ba" />
<br>
<img width="946" height="735" alt="image" src="https://github.com/user-attachments/assets/6fc8bcb1-1453-473e-be40-ec6706ae84b4" />


---

### 2. Apa yang terjadi jika nilai num lebih dari 15?

Jika fungsi `displayDigit(int num)` menerima nilai `num` lebih dari 15, program akan mengalami *Index Out of Bounds* (membaca memori di luar batas array). Array `digitPattern` hanya memiliki 16 baris elemen (indeks 0 hingga 15). Membaca indeks ke-16 atau lebih akan membuat Arduino membaca data dari alamat memori yang tidak semestinya. Akibatnya, 7-segment akan menampilkan pola acak (*glitch*), dan dalam beberapa kasus dapat menyebabkan mikrokontroler *reset* atau *crash*.

---

### 3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan alasanya!

Program dan rangkaian ini menggunakan **Common Cathode** (Katoda Bersama). Alasannya:
1. **Dari Segi Rangkaian:** Pin *common* (kaki tengah pada 7-segment) dihubungkan ke jalur **GND** (Ground/Negatif).
2. **Dari Segi Kode:** Array `digitPattern` menggunakan logika positif (`1` atau `HIGH`) untuk menyalakan segmen LED. Arus listrik mengalir dari pin Arduino yang bertegangan tinggi menuju pin *common* yang bertegangan rendah. Jika menggunakan *Common Anode*, pin *common* harus dihubungkan ke 5V dan program membutuhkan logika `0` (`LOW`) untuk menyalakan LED.

---

### 4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

# Modifikasi 7-Segment (F ke 0) dengan Pushbutton

Program ini memodifikasi tampilan 7-segment agar menghitung mundur dari F (15) hingga 0 secara berurutan setiap kali pushbutton ditekan. Rangkaian ini menggunakan jenis 7-segment Common Cathode.

## Kode Program dan Penjelasan Baris

```cpp
#include <Arduino.h> // Memasukkan library standar Arduino

// Mendeklarasikan urutan pin Arduino yang terhubung ke segmen a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Mendeklarasikan pin Arduino (Pin 2) yang terhubung ke salah satu kaki pushbutton
const int buttonPin = 2;

// Menginisialisasi variabel untuk menyimpan angka saat ini, dimulai dari 15 (F)
int currentDigit = 15;

// Menginisialisasi variabel untuk menyimpan status tombol sebelumnya, diatur HIGH (tidak ditekan)
bool lastButtonState = HIGH;

// Mendeklarasikan array 2D untuk menyimpan pola nyala/mati (1/0) segmen untuk angka 0-9 dan A-F
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // Indeks 0: Pola angka 0
  {0,1,1,0,0,0,0,0}, // Indeks 1: Pola angka 1
  {1,1,0,1,1,0,1,0}, // Indeks 2: Pola angka 2
  {1,1,1,1,0,0,1,0}, // Indeks 3: Pola angka 3
  {0,1,1,0,0,1,1,0}, // Indeks 4: Pola angka 4
  {1,0,1,1,0,1,1,0}, // Indeks 5: Pola angka 5
  {1,0,1,1,1,1,1,0}, // Indeks 6: Pola angka 6
  {1,1,1,0,0,0,0,0}, // Indeks 7: Pola angka 7
  {1,1,1,1,1,1,1,0}, // Indeks 8: Pola angka 8
  {1,1,1,1,0,1,1,0}, // Indeks 9: Pola angka 9
  {1,1,1,0,1,1,1,0}, // Indeks 10: Pola huruf A
  {0,0,1,1,1,1,1,0}, // Indeks 11: Pola huruf b
  {1,0,0,1,1,1,0,0}, // Indeks 12: Pola huruf C
  {0,1,1,1,1,0,1,0}, // Indeks 13: Pola huruf d
  {1,0,0,1,1,1,1,0}, // Indeks 14: Pola huruf E
  {1,0,0,0,1,1,1,0}  // Indeks 15: Pola huruf F
};

// Fungsi untuk menyalakan 7-segment sesuai dengan angka/huruf yang diminta (parameter num)
void displayDigit(int num) {
  // Melakukan perulangan sebanyak 8 kali untuk mengontrol ke-8 segmen (a sampai dp)
  for(int i=0; i<8; i++) {
    // Mengirimkan sinyal HIGH (1) atau LOW (0) ke pin segmen sesuai pola di array
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  // Melakukan perulangan untuk mengatur mode pin pada setiap segmen
  for(int i=0; i<8; i++) {
    // Mengatur pin segmen sebagai OUTPUT agar bisa mengalirkan tegangan
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Mengatur pin tombol sebagai INPUT dengan mengaktifkan resistor PULLUP internal
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Memanggil fungsi displayDigit untuk menampilkan angka pertama (F) saat program baru berjalan
  displayDigit(currentDigit);
}

void loop() {
  // Membaca status tombol saat ini (LOW jika sedang ditekan, HIGH jika dilepas)
  bool currentButtonState = digitalRead(buttonPin);

  // Logika State Change: Mengecek apakah tombol baru saja ditekan (transisi dari HIGH ke LOW)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    
    // Mengurangi nilai currentDigit sebanyak 1 (proses hitung mundur)
    currentDigit--;
    
    // Mengecek apakah nilai sudah berada di bawah 0
    if (currentDigit < 0) {
      // Jika ya, kembalikan nilai ke 15 (F) agar perhitungan berulang
      currentDigit = 15;
    }
    
    // Memperbarui tampilan 7-segment sesuai dengan nilai angka yang baru
    displayDigit(currentDigit);
    
    // Menunda eksekusi selama 50 milidetik sebagai debouncing untuk mencegah pembacaan getaran ganda
    delay(50);
  }

  // Menyimpan status tombol saat ini ke variabel lastButtonState untuk perbandingan putaran berikutnya
  lastButtonState = currentButtonState;
}
```
