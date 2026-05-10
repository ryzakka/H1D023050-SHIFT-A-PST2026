# Kontrol Kecerahan LED dengan Potensiometer (Kecerahan Menengah)

Proyek ini mendemonstrasikan cara mengatur tingkat kecerahan sebuah LED menggunakan potensiometer melalui metode PWM (*Pulse Width Modulation*). Program telah dimodifikasi agar LED beroperasi pada batasan kecerahan yang spesifik (tidak bisa mati total dan tidak bisa menyala maksimal).

---

## Jawaban Pertanyaan Praktikum

**1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`!** Fungsi `analogWrite()` pada Arduino bekerja menggunakan teknik **PWM (*Pulse Width Modulation*)**. Meskipun mengandung kata "analog", fungsi ini sebenarnya memancarkan sinyal digital (HIGH/5V dan LOW/0V) yang dihidup-matikan (berkedip) dengan sangat cepat (biasanya sekitar 490 Hz).

Mata manusia tidak bisa melihat kedipan secepat itu, sehingga yang terlihat hanyalah perubahan intensitas cahaya. Dengan mengubah *duty cycle* (persentase waktu sinyal berada dalam keadaan HIGH dibandingkan total waktu satu siklus), tegangan rata-rata yang diterima LED akan berubah:
* Jika nilai PWM tinggi (mendekati 255), sinyal HIGH lebih lama, tegangan rata-rata tinggi, sehingga LED menyala **terang**.
* Jika nilai PWM rendah (mendekati 0), sinyal LOW lebih lama, tegangan rata-rata rendah, sehingga LED menyala **redup**.

**2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?** Hubungannya terletak pada perbedaan resolusi data yang dimiliki oleh komponen internal mikrokontroler Arduino Uno:
* **ADC (*Analog-to-Digital Converter*):** Memiliki resolusi **10-bit**. Ini berarti ADC dapat membaca variasi tegangan analog (0-5V) dan mengubahnya menjadi $2^{10}$ kemungkinan nilai digital, yaitu rentang **0 hingga 1023**.
* **PWM (*Pulse Width Modulation*):** Memiliki resolusi **8-bit**. Ini berarti pin output PWM hanya dapat menerima $2^8$ kemungkinan nilai, yaitu rentang **0 hingga 255**.

Karena perbedaan resolusi ini (skala 4:1), nilai pembacaan sensor yang bernilai 0–1023 tidak bisa langsung dimasukkan ke fungsi `analogWrite()`. Nilai tersebut harus disesuaikan rentangnya (*scaling*) menjadi 0–255 terlebih dahulu menggunakan fungsi `map()` atau dengan cara matematis membaginya dengan 4.

---

## Rangkaian (Wiring)

* **Potensiometer:** Kaki kiri atau kanan ke **5V** dan **GND**, kaki tengah (wiper) ke **Pin A0**.
* **LED:** Kaki anoda (panjang) ke **Pin 9** (menggunakan resistor pengaman), kaki katoda (pendek) ke **GND**.

---

## Kode Program

```cpp
#include <Arduino.h> // library dasar Arduino

// ===================== PIN SETUP =====================
const int potPin = A0;   // pin analog untuk potensiometer
const int ledPin = 9;    // pin PWM untuk LED

// ===================== VARIABEL =====================
int nilaiADC = 0;  // menyimpan nilai mentah potensiometer
int pwm = 0;       // menyimpan nilai PWM yang sudah dikonversi

void setup() {
  // Atur pin LED sebagai output
  pinMode(ledPin, OUTPUT);
  // Aktifkan komunikasi serial 
  Serial.begin(9600);
}

void loop() {
  // Baca nilai analog dari potensiometer (rentang 0–1023)
  nilaiADC = analogRead(potPin);

  // Ubah nilai ADC (0–1023) menjadi rentang kecerahan sedang (50–200)
  pwm = map(nilaiADC, 0, 1023, 50, 200); 

  // Kirim sinyal PWM ke LED
  analogWrite(ledPin, pwm);

  // Tampilkan data ke Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM: ");
  Serial.println(pwm);

  // Delay stabilisasi
  delay(50);
}
