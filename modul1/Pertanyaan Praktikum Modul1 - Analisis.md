# Pertanyaan Analisis

1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan! 
2. Bagaimana pengaruh penggunaan struktur perulangan (seperti for dan while) terhadap jalannya program pada Arduino? 
3. Bagaimana cara kerja percabangan (if-else) dalam menentukan kondisi LED menyala 
atau mati berdasarkan input yang diberikan? 
4. Bagaimana kombinasi antara perulangan dan percabangan dapat digunakan untuk 
membuat sistem Arduino yang responsif terhadap perubahan kondisi lingkungan? 

# Jawaban

### 1. Uraian Hasil Tugas pada Setiap Percobaan

* **Percobaan 1 (Efek LED Bergulir / *Knight Rider*):**
    Program ini menggunakan perulangan `for` untuk menyalakan dan mematikan rentetan LED (Pin 2 hingga 8) secara berurutan. Hasilnya, cahaya LED tampak berjalan dari kiri ke kanan, lalu setelah mencapai ujung kanan, cahaya akan berbalik arah dan berjalan dari kanan ke kiri secara terus-menerus dengan jeda 100ms.
* **Percobaan 2 (Percabangan / LED Berkedip Makin Cepat):**
    Program ini mengontrol satu LED di pin 12. Hasilnya adalah LED berkedip dengan ritme yang semakin lama semakin cepat (durasi nyala-mati berkurang 100ms setiap siklusnya dari awal 1000ms). Ketika kedipan sudah sangat cepat (jeda mencapai 100ms atau kurang), logika `if-else` akan bekerja untuk menghentikan sementara program selama 3 detik, lalu mereset kecepatan kedipan kembali lambat (1000ms).
* **Percobaan 3 (Perulangan 2 Bagian / 3 Kiri & 3 Kanan):**
    Program ini membagi 6 LED (Pin 2 hingga 7) menjadi dua kelompok. Hasilnya, 3 LED di sebelah kiri dan 3 LED di sebelah kanan menyala secara bergantian setiap 500ms.

---

### 2. Pengaruh Penggunaan Struktur Perulangan (`for` dan `while`) terhadap Program
* **Efisiensi Baris Kode:** Daripada menulis perintah `digitalWrite(pin, HIGH)` sebanyak puluhan kali untuk menyalakan rentetan LED, menulisnya satu kali di dalam blok `for` atau `while` membuat kode jauh lebih pendek dan rapi.
* **Menghemat Memori:** Kode yang lebih pendek mengonsumsi lebih sedikit memori pada papan Arduino.
* **Inisialisasi Massal:** Memudahkan proses *setup* (seperti pada Percobaan 1 dan 3); mengatur banyak pin sebagai `OUTPUT` hanya dengan 3 baris kode perulangan.
* **Manipulasi Waktu/Sekuensial:** Memungkinkan membuat pola yang kompleks berdasarkan urutan angka; LED berjalan yang angkanya naik `++` lalu turun `--`.

---

### 3. Cara Kerja Percabangan (`if-else`) Menentukan Kondisi LED
Struktur `if-else` bertindak sebagai "pengambil keputusan" atau logika kondisional di dalam mikrokontroler; dengan mengevaluasi sebuah kondisi apakah bernilai **Benar (*True*)** atau **Salah (*False*)**.

Mengambil contoh dari Percobaan 2:
```cpp
if (timeDelay <= 100) { 
  // Blok eksekusi jika BENAR
  delay(3000); 
  timeDelay = 1000; 
} else { 
  // Blok eksekusi jika SALAH
  timeDelay -= 100; 
}
```
* Setiap kali program melewati kode, Arduino akan "bertanya" pada input/variabel: *"Apakah nilai `timeDelay` saat ini lebih kecil atau sama dengan 100?"*
* **Jika inputnya Benar (True):** Arduino masuk ke dalam blok `if`, memberi jeda 3 detik, dan mereset LED agar kembali lambat.
* **Jika inputnya Salah (False):** Arduino mengabaikan blok `if` dan langsung melompat ke blok `else`, yaitu mengurangi nilai delay sebesar 100ms agar kedipan selanjutnya lebih cepat.

---

### 4. Kombinasi Perulangan dan Percabangan untuk Sistem Responsif
Kombinasi keduanya adalah fondasi utama dari sistem *embedded* modern. Perulangan berfungsi sebagai "mesin" yang terus berjalan, sedangkan percabangan berfungsi sebagai "otak" yang merespons perubahan.

**Penerapan**
* **Sistem Monitoring (*Loop*):** Arduino menggunakan fungsi perulangan (seperti `void loop()` itu sendiri atau perulangan `while` di dalamnya) untuk terus-menerus memindai atau membaca data dari kondisi lingkungan secara real-time (misalnya: membaca suhu dari sensor, atau mengecek apakah tombol ditekan).
* **Pengambilan Keputusan (*If-Else*):** Di dalam perulangan tersebut, disisipkan percabangan `if-else`. Ketika perulangan menangkap adanya perubahan data lingkungan (misal: sensor mendeteksi suhu naik di atas 35 derajat celcius), blok `if` akan aktif untuk melakukan tindakan (misal: menyalakan kipas/LED merah). Jika suhu normal, blok `else` akan mematikan kipas.

Ini dapat membantu sistem untuk tidak hanya menjalankan tugas secara abstrak, tetapi bisa **bereaksi, beradaptasi, dan merespons** lingkungan sekitarnya dengan tepat waktu.
