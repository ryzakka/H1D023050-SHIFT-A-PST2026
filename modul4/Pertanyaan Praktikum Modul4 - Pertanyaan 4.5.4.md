# 🤖 Kontrol Motor Servo dengan Potensiometer (Rentang Terbatas 30°-150°)

Proyek ini mendemonstrasikan cara mengendalikan pergerakan motor servo menggunakan sebuah potensiometer. Program ini telah dimodifikasi secara khusus agar putaran servo dibatasi pada rentang sudut tertentu (30° hingga 150°) demi keamanan atau kebutuhan mekanis.

---

## 📝 Jawaban Pertanyaan Praktikum

**1. Apa fungsi perintah `analogRead()` pada rangkaian praktikum ini?**
Fungsi `analogRead()` pada rangkaian ini digunakan untuk membaca nilai tegangan analog yang dikirimkan oleh potensiometer melalui pin **A0**. Mikrokontroler Arduino memiliki *Analog-to-Digital Converter* (ADC) internal beresolusi 10-bit yang akan mengubah variasi tegangan (0V hingga 5V) dari potensiometer menjadi data angka digital dengan rentang nilai **0 hingga 1023**.

**2. Mengapa diperlukan fungsi `map()` dalam program tersebut?**
Fungsi `map()` sangat diperlukan untuk **menyesuaikan skala (scaling)** rentang nilai antara input dan output yang berbeda. 
* **Input:** Nilai dari potensiometer (hasil `analogRead`) memiliki rentang **0 hingga 1023**.
* **Output:** Perintah motor servo (`myservo.write()`) menerima parameter berupa sudut dalam derajat, umumnya rentang **0 hingga 180**. 

Jika nilai 1023 langsung dimasukkan ke motor servo, servo tidak akan bisa merespons karena melebihi batas maksimal sudut putarnya. Oleh karena itu, `map()` digunakan untuk menerjemahkan (memetakan) rentang 0–1023 agar secara proporsional menjadi rentang sudut yang diizinkan (pada kasus ini 30–150).

---

## 💻 Kode Program

Program ini mengatur agar pin A0 sebagai input analog dan pin 9 sebagai output PWM untuk servo.

```cpp
#include <Servo.h> // library untuk servo motor

Servo myservo; // membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0;   // pin analog input
const int servoPin = 9;       // pin digital untuk servo

// ===================== VARIABEL =====================
int pos = 0; // sudut servo 
int val = 0; // nilai ADC 

void setup() {
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  // Membaca nilai ADC potensiometer (0-1023)
  val = analogRead(potensioPin);

  // Mengubah rentang nilai ADC (0-1023) menjadi sudut servo (30-150 derajat)
  pos = map(val, 0, 1023, 30, 150); 

  // Menggerakkan servo ke posisi yang telah dipetakan
  myservo.write(pos);

  // Menampilkan data di Serial Monitor
  Serial.print("ADC Potensio: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  // Stabilisasi gerakan servo
  delay(100);
}
