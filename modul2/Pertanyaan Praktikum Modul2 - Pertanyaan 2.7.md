### 1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!

<img width="929" height="689" alt="image" src="https://github.com/user-attachments/assets/c3369b7a-35b0-4001-99fd-faf3d0695604" />


Berdasarkan praktikum yang telah dilakukan, berikut adalah uraian hasil dari setiap percobaan:

* **Percobaan 1 (Menampilkan Angka dan Huruf Heksadesimal secara Otomatis):** Rangkaian Seven Segment berhasil dirakit dan diprogram untuk menampilkan urutan angka 0 hingga 9, dilanjutkan dengan huruf A hingga F secara bergantian setiap 1 detik. Pada percobaan ini, dilakukan penyesuaian pemetaan pin (pin mapping) fisik dengan array program, serta mengimplementasikan logika *Common Cathode* di mana segmen LED menyala saat menerima sinyal logika `HIGH` (1).
* **Percobaan 2 (Penghitung Naik/Counter Up dengan Pushbutton):** Sistem dimodifikasi dari mode otomatis menjadi manual dengan menambahkan komponen *pushbutton*. Mikrokontroler berhasil membaca input digital tombol menggunakan fitur *internal pull-up* (`INPUT_PULLUP`). Tampilan Seven Segment hanya berpindah ke karakter berikutnya setiap kali tombol ditekan. Masalah pembacaan ganda mekanik berhasil diatasi dengan penerapan logika *State Change Detection* dan jeda *debouncing*.
* **Percobaan 3 (Penghitung Mundur/Counter Down dari F ke 0):** Logika pemrograman berhasil diubah arahnya. Nilai awal diatur menjadi 15 (merepresentasikan karakter F), dan setiap penekanan tombol akan mengurangi nilai pada variabel. Terdapat pembatas logika (`if`) yang mengembalikan nilai ke 15 apabila hitungan sudah berada di bawah 0.

---

### 2. Bagaimana prinsip kerja dari Seven Segment Display dalam menampilkan angka dan karakter?



Seven Segment Display pada dasarnya bekerja dengan mengontrol 8 buah LED dasar (segmen a, b, c, d, e, f, g, dan dp/titik desimal) yang disusun membentuk angka 8. Prinsip kerjanya adalah sebagai berikut:

1. **Kombinasi Segmen:** Untuk membentuk suatu angka atau huruf, mikrokontroler menyalakan kombinasi LED tertentu dan mematikan sisanya. Misalnya, untuk menampilkan angka "1", hanya LED pada segmen **b** dan **c** yang dinyalakan.
2. **Logika Tegangan (Common Cathode):** Pada konfigurasi *Common Cathode* (Katoda Bersama), seluruh kutub negatif (ground) dari kedelapan LED disatukan ke pin *Common* dan dihubungkan ke `GND` Arduino. Untuk menyalakan sebuah segmen, pin Arduino harus mengirimkan tegangan positif atau sinyal logika `HIGH` (5V) ke pin segmen yang dituju. Sinyal logika `LOW` (0V) digunakan untuk mematikan segmen.
3. **Penyimpanan Pola:** Pola biner (1 untuk nyala, 0 untuk mati) dari setiap karakter disimpan di dalam memori program berupa array 2D. Fungsi pada program kemudian membaca pola ini dan mengeksekusinya menggunakan `digitalWrite()` untuk mengirimkan tegangan yang sesuai ke masing-masing pin.

---

### 3. Jelaskan bagaimana sistem counter bekerja pada program tersebut!

Sistem *counter* (penghitung) bekerja dengan melacak perubahan status pada komponen *pushbutton* dan memanipulasi nilai pada sebuah variabel. Berikut alur kerjanya:

1. **Variabel Penyimpan Nilai:** Program mendeklarasikan sebuah variabel bertipe integer (misalnya `currentDigit`) yang berfungsi untuk menyimpan angka hitungan saat ini.
2. **Deteksi Transisi (State Change):** Di dalam fungsi `loop()`, mikrokontroler secara konstan membaca status tegangan tombol. Program didesain agar tidak merespons seberapa lama tombol ditahan, melainkan hanya merespons *tepat pada momen* tombol ditekan (mendeteksi perubahan status sinyal dari `HIGH` menjadi `LOW`).
3. **Operasi Aritmatika:** Jika transisi penekanan tersebut terdeteksi, program akan melakukan operasi aritmatika pada variabel `currentDigit`. Untuk *counter up*, nilainya ditambah satu (`currentDigit++`), sedangkan untuk *counter down*, nilainya dikurangi satu (`currentDigit--`).
4. **Pembatasan Siklus (Threshold):** Setelah operasi matematika, program menggunakan struktur kontrol `if` untuk memeriksa apakah nilai sudah melewati batas. Jika *counter down* jatuh ke nilai kurang dari 0, nilainya direset paksa ke 15 agar siklus hitungan heksadesimal terus berlanjut tanpa eror memori.
5. **Eksekusi Tampilan:** Nilai `currentDigit` yang baru kemudian digunakan sebagai indeks referensi untuk mencari pola nyala LED di dalam array. Terakhir, program memberikan jeda sesaat (`delay(50)`) sebagai penangkal getaran saklar mekanik (*debouncing*) agar penekanan tombol dihitung secara akurat.
