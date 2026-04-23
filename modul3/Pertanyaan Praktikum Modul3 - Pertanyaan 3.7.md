# Analisis Komunikasi UART dan I2C pada Sistem Arduino

Berdasarkan percobaan yang telah dilakukan (mengontrol LED via Serial Monitor dan menampilkan data Potensiometer ke LCD), berikut adalah analisis dari protokol UART dan I2C yang digunakan.

---

## 1) Keuntungan dan Kerugian UART dan I2C

### **UART (Universal Asynchronous Receiver-Transmitter)**
Pada percobaan kita, UART digunakan untuk komunikasi antara Arduino dan komputer (Serial Monitor).

* **Keuntungan:**
  * **Sederhana & Mudah:** Hanya membutuhkan 2 jalur kabel utama (TX untuk mengirim, RX untuk menerima), ditambah Ground.
  * **Tidak Butuh Clock (Asinkron):** Tidak perlu kabel *clock* untuk sinkronisasi, sehingga menghemat pin.
  * **Dukungan Luas:** Sangat standar untuk komunikasi antara mikrokontroler dengan PC/Laptop melalui kabel USB.
* **Kerugian:**
  * **Point-to-Point (1-ke-1):** Hanya bisa menghubungkan dua perangkat secara langsung. Tidak bisa dipasang banyak perangkat dalam satu jalur yang sama.
  * **Overhead Data:** Memerlukan bit tambahan (*Start bit* dan *Stop bit*) di setiap pengiriman data.
  * **Kecepatan Harus Sama (Baud Rate):** Kedua perangkat wajib disetel pada kecepatan yang sama (misal 9600 bps). Jika berbeda sedikit saja, data akan terbaca sebagai karakter acak/sampah.

### **I2C (Inter-Integrated Circuit)**
Pada percobaan kita, I2C digunakan untuk komunikasi antara Arduino (Master) dan layar LCD (Slave).

* **Keuntungan:**
  * **Multi-Slave / Multi-Master:** Menggunakan sistem *Bus*. Puluhan hingga ratusan sensor atau modul dapat dihubungkan hanya menggunakan **2 pin yang sama** (SDA dan SCL).
  * **Keamanan Data (ACK/NACK):** Memiliki sistem konfirmasi bawaan. Setiap kali Master mengirim data, Slave akan merespon untuk memberitahu bahwa data berhasil diterima.
  * **Sinkron (Ada Clock):** Jalur SCL (Serial Clock) memastikan kedua perangkat membaca data di waktu yang tepat, sehingga tidak terpengaruh perbedaan kecepatan prosesor.
* **Kerugian:**
  * **Jarak Dekat:** Didesain untuk komunikasi antar komponen di dalam satu papan sirkuit yang sama (biasanya kurang dari 1 meter). Jika kabel terlalu panjang, sinyal akan rusak.
  * **Kecepatan Lebih Lambat:** Dibandingkan protokol sinkron lain seperti SPI, I2C relatif lebih lambat karena adanya *overhead* alamat dan konfirmasi.

---

## 2) Peran Alamat I2C pada LCD (0x27 vs 0x20)

Karena I2C adalah sistem *Bus* di mana banyak perangkat berbagi jalur kabel SDA dan SCL yang sama, Arduino (sebagai Master) membutuhkan cara untuk membedakan perangkat mana yang sedang diajak bicara. Di sinilah **Alamat I2C (*I2C Address*)** berperan, layaknya nomor rumah atau alamat IP.

* **Modul I2C LCD (Chip PCF8574):** Modul hitam yang disolder di belakang LCD menggunakan IC PCF8574. Chip ini memiliki alamat bawaan pabrik.
* **Perbedaan Alamat (0x27 atau 0x20):**
  * Tergantung pabrikan dan varian seri IC-nya (PCF8574T atau PCF8574AT), alamat *default*-nya bisa `0x27`, `0x20`, atau `0x3F`. 
* **Jika Alamat Salah:** Jika kamu menulis `LiquidCrystal_I2C lcd(0x20, 16, 2);` namun modulmu aslinya beralamat `0x27`, maka LCD **tidak akan menampilkan apa-apa**. Arduino mengirim data ke "rumah" nomor `0x20`, tapi layar LCD yang beralamat `0x27` akan mengabaikan paket data tersebut karena merasa itu bukan untuknya.

---

## 3) Alur Kerja & Manajemen Sistem Gabungan UART dan I2C

Ketika kita menggabungkan keduanya (misalnya mengetik perintah di Serial Monitor komputer untuk ditampilkan ke LCD I2C), sistem bekerja dengan alur dan manajemen berikut:

### **Alur Kerja Sistem:**
1. **Input (PC ke Arduino - UART):** Pengguna mengetik data di PC. PC mengirimkannya ke Arduino via USB.
2. **Buffering:** Modul UART di dalam *chip* Arduino menerima sinyal tersebut dan menyimpannya sementara di memori penerimaan (Buffer Serial).
3. **Pemrosesan (CPU):** Fungsi `loop()` mengeksekusi `Serial.available()` dan `Serial.read()`. Mikrokontroler membaca karakter tersebut dan menjalankan logika program (misal mengecek perintah).
4. **Output (Arduino ke LCD - I2C):** Setelah data siap, program memanggil fungsi `lcd.print()`. Arduino menghentikan pekerjaannya sejenak untuk memutar sinyal *clock* (SCL) dan mengirim instruksi serta karakter tersebut ke modul LCD melalui jalur SDA.
5. **Eksekusi Fisik:** LCD menerima instruksi via I2C dan menyalakan pixel di layar.

### **Bagaimana Arduino Mengelola Keduanya Sekaligus?**
Arduino sebenarnya tidak mengeksekusinya di waktu yang benar-benar bersamaan secara paralel (*multi-threading*), melainkan bergantian dengan sangat cepat (*sequential*), dibantu oleh arsitektur perangkat keras:

* **Hardware Independen:** *Chip* ATmega328P pada Arduino Uno memiliki blok perangkat keras (IC) terpisah di dalamnya. Ada blok khusus UART (pin 0 & 1) dan blok khusus I2C/TWI (pin A4 & A5). 
* **Penggunaan Interupsi (Interrupt):** Ketika data dari PC masuk via UART, Arduino tidak perlu menunggunya. Perangkat keras UART secara otomatis menangkap sinyal itu di latar belakang (*background*) melalui sistem *Interrupt* dan meletakkannya di *buffer* memori tanpa mengganggu pekerjaan utama CPU.
* **Eksekusi Cepat:** Karena CPU Arduino berjalan pada kecepatan 16 MHz (16 juta instruksi per detik), ia bisa membaca buffer UART, memprosesnya, lalu mengirim data ke modul I2C LCD dalam hitungan mikrodetik. Mata manusia tidak bisa melihat jeda waktu tersebut, sehingga terlihat seperti Arduino melakukan dua tugas sekaligus secara *real-time*.
