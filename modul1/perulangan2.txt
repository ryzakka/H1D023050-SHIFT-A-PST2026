int timer = 500; // delay. Diubah ke 500 agar kedipan bergantian lebih jelas terlihat.
                 // Semakin tinggi angkanya, semakin lambat waktunya. 

void setup() { 
  // gunakan loop for untuk menginisialisasi pin 2 sampai 7 sebagai output: 
  for (int ledPin = 2; ledPin <= 7; ledPin++) { 
    pinMode(ledPin, OUTPUT); 
  } 
} 

void loop() { 
  // --- KONDISI 1: 3 LED Kiri Menyala, 3 LED Kanan Mati ---
  // Menyalakan LED pin 2, 3, 4
  for (int i = 2; i <= 4; i++) {
    digitalWrite(i, HIGH);
  }
  // Mematikan LED pin 5, 6, 7
  for (int i = 5; i <= 7; i++) {
    digitalWrite(i, LOW);
  }
  
  delay(timer); // tahan kondisi ini selama waktu timer

  // --- KONDISI 2: 3 LED Kiri Mati, 3 LED Kanan Menyala ---
  // Mematikan LED pin 2, 3, 4
  for (int i = 2; i <= 4; i++) {
    digitalWrite(i, LOW);
  }
  // Menyalakan LED pin 5, 6, 7
  for (int i = 5; i <= 7; i++) {
    digitalWrite(i, HIGH);
  }
  
  delay(timer); // tahan kondisi ini selama waktu timer
}
