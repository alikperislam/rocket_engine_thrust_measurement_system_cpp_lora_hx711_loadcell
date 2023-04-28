// LORA kodlar -----------------------
#include "LoRa_E22.h"
#include <SoftwareSerial.h>
// LORA kodlar -----------------------

// ---------------------- laodcell - hx711
#include "HX711.h"
// ---------------------- laodcell - hx711

// LORA kodlar -----------------------
SoftwareSerial mySerial(10, 11); // (nano-rx) (nano-tx)
LoRa_E22 E22(&mySerial);
#define M0 7
#define M1 6
// LORA kodlar -----------------------

// ---------------------- laodcell - hx711
//  pin bapğlantıları:
#define DOUT  4 //data pini d4 e bağlı. 
#define CLK  5 //SCK pini d5 e bağlı. 
HX711 scale;
// ---------------------- laodcell - hx711

// LORA kodlar -----------------------
String deger="";
int sayi=0;
int sayi1=1;
// LORA kodlar -----------------------

// ---------------------- laodcell - hx711
float calibration_factor = -301; // konfigurasyon -301 e göre yapılmaktadır. -> 10-15 gr eksik.
float gram,second_gram;
float newton,second_newton;
float max_gram;
float max_newton;
bool state = false;
// ---------------------- laodcell - hx711

void setup() {
  Serial.begin(9600);
  // LORA kodlar -----------------------
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  E22.begin();
  delay(500);
  // LORA kodlar -----------------------
  
  // ---------------------- laodcell - hx711
  //konfig ayarlamaları
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();
  // ---------------------- laodcell - hx711
}

void loop() {
  //kalibrasyon değerine göre güncelliyor.
  scale.set_scale(calibration_factor); 
  
  // gram ve newton hesaplamasi:
  gram_newton();
 
  // max gram ve max newton hesaplamasi:
  maxGramCalculator(gram,second_gram);
  maxNewtonCalculator(newton,second_newton);

  // motorun acik olup olmama durumu:
  rocketState(second_gram);



  
  deger = String(gram)+"/"+String(newton)+"/"+String(max_gram)+"/"+String(max_newton)+"/"+String(state)+"\r\n";
  ResponseStatus rs = E22.sendFixedMessage( 0,0,23, deger);
  Serial.print(rs.getResponseDescription());
  delay(299);
  /*
   // yer istayonuna gonderilen veriler:
  Serial.print(gram); // ölçülen gramaj değeri. +%21 eklendi
  Serial.print("/");
  Serial.print(newton); // newton turunden olcum degiskeni.
  Serial.print("/");
  Serial.print(max_gram); // algoritmaya gore gram turunden maximum olculen itki degeri.
  Serial.print("/");
  Serial.print(max_newton); // algoritmaya gore newton turunden maximum olculen itki degeri.
  Serial.print("/");
  Serial.println(state);
  //------------------------------------------------------------------
  delay(299);
   */
}

// ---------------------- laodcell - hx711 (fonksiyonlar)

void gram_newton(){
  gram=scale.get_units();
  newton=gram*(0.009807);
  delay(1);
  second_gram=scale.get_units();
  second_newton=second_gram*(0.009807);
}

void maxGramCalculator(float gr,float gr_second){
  if(gr_second > gr && gr_second > max_gram){
    max_gram = gr;
  }
  else{}
}

void maxNewtonCalculator(float N,float N_second){
  if(N_second > N && N_second > max_newton ){
    max_newton = N;
  }
  else{}
}

void rocketState(float gr){
   state = (gr<30) ? false : true;
}

// ---------------------- laodcell - hx711 (fonksiyonlar)
// loadcell hx711 bağlantı:
// kırmızı - E+
// siyah   - E-
// yeşil   - A-
// beyaz   - A+
