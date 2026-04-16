// Pin mapping segment (sesuai instruksi terakhirmu)
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Pin untuk Pushbutton
const int buttonPin = 2;

// Variabel untuk menyimpan status angka dan tombol
int currentDigit = 0;
bool lastButtonState = HIGH;

byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}, // 9
  {1,1,1,0,1,1,1,0}, // A
  {0,0,1,1,1,1,1,0}, // b
  {1,0,0,1,1,1,0,0}, // C
  {0,1,1,1,1,0,1,0}, // d
  {1,0,0,1,1,1,1,0}, // E
  {1,0,0,0,1,1,1,0}  // F
};

void displayDigit(int num) {
  for(int i=0; i<8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  // Setup pin segment sebagai output
  for(int i=0; i<8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Setup pushbutton sebagai input dengan pull-up internal
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Tampilkan angka 0 saat awal dinyalakan
  displayDigit(currentDigit);
}

void loop() {
  // Membaca status tombol (LOW berarti ditekan karena menggunakan pull-up)
  bool currentButtonState = digitalRead(buttonPin);

  // Jika tombol baru saja ditekan (dari tidak ditekan ke ditekan)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Tambah angka
    currentDigit++;
    
    // Jika sudah lebih dari F (15), balik lagi ke 0
    if (currentDigit > 15) {
      currentDigit = 0;
    }
    
    // Update tampilan display
    displayDigit(currentDigit);
    
    // Debouncing (mencegah pembacaan ganda karena getaran mekanik tombol)
    delay(50);
  }

  // Simpan status tombol terakhir untuk pengecekan di loop berikutnya
  lastButtonState = currentButtonState;
}
