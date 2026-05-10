# Pertanyaan 4.7 - Integrasi ADC dan PWM

## 1. Proses Konversi Sinyal Analog Menjadi Digital pada Arduino

Proses konversi ini dilakukan oleh perangkat keras internal mikrokontroler yang disebut **ADC (*Analog-to-Digital Converter*)**. Pada Arduino Uno, ADC menggunakan metode yang disebut *Successive Approximation*. Prosesnya terbagi menjadi tiga tahapan utama:

1. **Sampling & Hold (Pencuplikan dan Penahanan):** Saat fungsi `analogRead()` dipanggil, Arduino akan "mencuplik" (mengambil sampel) tegangan yang masuk ke pin analog (misal pin A0 dari potensiometer). Tegangan ini kemudian ditahan sejenak menggunakan kapasitor internal agar nilainya stabil dan tidak berubah selama proses konversi berlangsung.
2. **Kuantisasi:** Tegangan analog (yang sifatnya kontinu dari 0V hingga 5V) akan dicocokkan atau dipetakan ke dalam tingkatan (level) diskrit. Karena Arduino Uno memiliki ADC 10-bit, tegangan tersebut dibagi ke dalam 1024 level yang berbeda (berasal dari 2 pangkat 10).
3. **Encoding:** Tingkatan tegangan tersebut kemudian diterjemahkan ke dalam angka digital (integer) dengan rentang 0 hingga 1023. Sebagai contoh, 0V dibaca sebagai angka digital 0, tegangan 2.5V dibaca sekitar 512, dan 5V dibaca sebagai 1023.

---

## 2. Faktor yang Mempengaruhi Keakuratan Pembacaan ADC

Berdasarkan rangkaian praktikum, ada beberapa faktor utama yang bisa membuat pembacaan ADC menjadi tidak akurat (misalnya nilai yang melompat-lompat di Serial Monitor):

* **Ketidakstabilan Tegangan Referensi (VCC):** ADC Arduino membandingkan input dari pin analog dengan tegangan sumber (5V). Jika suplai daya dari USB laptop atau adaptor tidak stabil atau mengalami "drop", acuan 5V tersebut akan berubah, sehingga hasil pembacaan ikut kacau.
* **Gangguan *Noise* Kelistrikan:** Penggunaan *breadboard* dan kabel jumper panjang sangat rentan menangkap interferensi elektromagnetik dari lingkungan sekitar (seperti antena). *Noise* ini bercampur dengan tegangan asli dari potensiometer.
* **Kualitas Komponen Input:** Potensiometer yang murah, kotor, atau sudah aus (karbonnya tergores) akan menghasilkan tegangan yang bergerigi atau tidak mulus saat diputar.
* **Frekuensi Pembacaan:** Mikrokontroler membutuhkan sepersekian milidetik agar kapasitor ADC terisi tegangan dengan benar. Jika program membaca `analogRead()` terlalu cepat tanpa *delay*, tegangan sisa dari pembacaan sebelumnya bisa memengaruhi pembacaan saat ini.

---

## 3. Kendala Integrasi ADC dan PWM dalam Satu Sistem

Jika menggabungkan input ADC (potensiometer) dan output PWM (seperti LED atau Motor Servo) dalam satu rangkaian yang saling terhubung, beberapa kendala yang sering terjadi adalah:

* **Masalah Skala / Resolusi (*Scaling Issue*):** Data ADC memiliki format 10-bit (0–1023), sedangkan PWM memiliki format 8-bit (0–255) atau dikonversi ke derajat (0–180 untuk servo). Jika kita tidak menggunakan fungsi konversi matematis seperti `map()` dengan benar, sistem tidak akan berjalan. Kesalahan pemetaan bisa membuat perubahan kecil pada potensiometer menyebabkan lonjakan gerakan/kecerahan yang kasar pada output.
* ***Noise* Silang (Fluktuasi Arus):** Ini adalah kendala fisik yang paling sering terjadi. Saat komponen output PWM bekerja (terutama beban induktif seperti Motor Servo), komponen tersebut akan menyedot arus listrik yang besar dan putus-putus. Hal ini menyebabkan tegangan 5V pada *breadboard* ikut berayun atau "drop" sesaat. Karena potensiometer juga menggunakan arus 5V yang sama, tegangan input A0 akan ikut bergetar. Efek dominonya: nilai ADC melompat-lompat, yang kemudian membuat lampu LED berkedip tak wajar (*flicker*) atau motor servo bergetar (*jittering* / *tremor*) meskipun potensiometer tidak diputar.
* **Konflik Penggunaan Timer Mikrokontroler:** Pembangkitan sinyal PWM (`analogWrite()`) dan fitur library eksternal (seperti `Servo.h`) sama-sama mengandalkan perangkat keras internal yang disebut Timer. Sebagai contoh nyata, saat memasukkan library `Servo.h` ke dalam program, Timer 1 akan diambil alih oleh servo. Akibatnya, fungsi `analogWrite()` standar untuk mengontrol LED pada Pin 9 dan Pin 10 di Arduino Uno akan mati atau tidak berfungsi. Programmer harus waspada untuk memindahkan LED ke pin PWM lain (seperti pin 3, 5, 6, atau 11) agar tidak terjadi *error* bentrok.
