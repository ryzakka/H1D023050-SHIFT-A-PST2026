# 📘 Praktikum Sistem Tertanam - Modul 1 Percabangan

## Pertanyaan Praktikum

1. Pada kondisi apa program masuk ke blok `if`?  
2. Pada kondisi apa program masuk ke blok `else`?  
3. Apa fungsi dari perintah `delay(timeDelay)`?  
4. Jika program memiliki alur **mati → lambat → cepat → reset (mati)**, ubah menjadi:
   **lambat → cepat → sedang → mati (tanpa reset langsung)** dan jelaskan setiap baris kode.

---

## ✅ Jawaban

### 1. Kondisi Masuk ke Blok `if`

Program akan masuk ke blok `if` ketika kondisi berikut terpenuhi:

```cpp
if (timeDelay <= 100)
```

**Penjelasan:**
- Program mengecek apakah nilai `timeDelay` kurang dari atau sama dengan 100 ms  
- Jika kondisi tersebut benar, maka blok `if` dijalankan  
- Kondisi ini menunjukkan bahwa LED sudah berada pada kecepatan kedip yang sangat cepat  
- Biasanya terjadi setelah delay dikurangi secara bertahap pada setiap siklus  

---

### 2. Kondisi Masuk ke Blok `else`

Program akan masuk ke blok `else` ketika kondisi `if` **tidak terpenuhi**, yaitu:

```cpp
timeDelay > 100
```

Di dalam blok `else` terdapat perintah:

```cpp
timeDelay -= 100;
```

**Penjelasan:**
- Jika `timeDelay` masih lebih besar dari 100 ms  
- Maka delay akan dikurangi sebesar 100 ms setiap siklus  
- Hal ini menyebabkan LED berkedip semakin cepat secara bertahap  

---

### 3. Fungsi `delay(timeDelay)`

Fungsi:

```cpp
delay(timeDelay);
```

**Kegunaan:**
- Memberikan jeda waktu dalam satuan milidetik (ms)  
- Mengatur durasi LED dalam kondisi menyala dan mati  

**Penjelasan tambahan:**
- `delay(1000)` → jeda 1 detik → LED berkedip lambat  
- `delay(100)` → jeda 0.1 detik → LED berkedip cepat  
- Semakin kecil nilai delay → semakin cepat kedipan LED  
- Semakin besar nilai delay → semakin lambat kedipan LED  

---

## 4. Modifikasi Program  
### Alur Baru: **Lambat → Cepat → Sedang → Mati (Tanpa Reset)**

### 📌 Source Code

```cpp
const int ledPin = 6;     // Menentukan pin LED pada pin digital 6
int timeDelay = 1000;     // Delay awal (LED berkedip lambat)

void setup() { 
  pinMode(ledPin, OUTPUT);   // Mengatur pin LED sebagai output
}

void loop() { 
  // Menyalakan LED
  digitalWrite(ledPin, HIGH);  
  delay(timeDelay);           // Menunggu sesuai nilai delay (LED ON)

  // Mematikan LED
  digitalWrite(ledPin, LOW);  
  delay(timeDelay);           // Menunggu sesuai nilai delay (LED OFF)

  // Percabangan untuk mengatur perubahan kecepatan LED
  if (timeDelay <= 200) {     
    // Jika delay sudah sangat kecil (LED sangat cepat)
    timeDelay = 500;          
    // Ubah menjadi kecepatan sedang

  } else if (timeDelay <= 500) {  
    // Jika delay berada pada kecepatan sedang
    timeDelay = 0;            
    // Ubah menjadi mati (tidak berkedip)

  } else {  
    // Jika LED masih dalam kondisi lambat
    timeDelay -= 200;         
    // Kurangi delay agar LED semakin cepat
  }

  // Kondisi untuk menghentikan program
  if (timeDelay == 0) {
    digitalWrite(ledPin, LOW); // Pastikan LED benar-benar mati
    while(true);               // Menghentikan program secara permanen
  }
}
```

---

### Penjelasan Alur Program

1. **Kondisi Awal**
   - `timeDelay = 1000 ms`
   - LED berkedip lambat  

2. **Percepatan Bertahap**
   - Program masuk ke blok `else`
   - `timeDelay` dikurangi (`-200`)
   - LED menjadi semakin cepat  

3. **Kondisi Cepat**
   - Saat `timeDelay ≤ 200`
   - Program masuk ke blok `if`
   - Delay diubah menjadi `500 ms` (kecepatan sedang)  

4. **Kondisi Sedang ke Mati**
   - Saat `timeDelay ≤ 500`
   - Delay diubah menjadi `0`  
   - LED berhenti berkedip  

5. **Program Berhenti**
   - LED dimatikan  
   - `while(true)` menjaga program tetap berhenti (tidak reset ulang)  

---

## Kesimpulan

- Percabangan `if-else` digunakan untuk mengatur perubahan kecepatan LED  
- Nilai `timeDelay` berperan sebagai pengontrol utama kecepatan  
- Modifikasi program menghasilkan alur yang lebih halus:

  **Lambat → Cepat → Sedang → Mati**

- Program tidak kembali ke awal (tidak reset), melainkan berhenti permanen  
