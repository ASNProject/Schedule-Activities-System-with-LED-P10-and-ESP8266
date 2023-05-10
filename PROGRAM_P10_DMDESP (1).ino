#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <DMDESP.h>
#include <fonts/EMKotak5x7.h>
#include <fonts/EMSans8x16.h>
#include <fonts/DejaVuSansBold9.h>
#include <fonts/DejaVuSans9.h>

#include "FirebaseESP8266.h"

#define FontDefault DejaVuSans9
#define FontBesar   DejaVuSansBold9

// DMDESP Setup
#define DISPLAYS_WIDE 2 // Kolom Panel
#define DISPLAYS_HIGH 1 // Baris Panel
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  // Jumlah Panel P10 yang digunakan (KOLOM,BARIS)

// WIFI Setup
#define ssid      "VIVO 1906" // Ubah sesuaikan
#define password  "nggongapasit"        // Ubah sesuaikan

// NTP Client Setup
const long utcOffset = 3600 * 7; // Waktu WIB
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffset);

// Variabel
byte Jam;
byte Men;
byte Det;

char waktu[9];
char data[6];

int geser=145;
int x =0;
unsigned long previousMillis = 0; 

char text0[]="AKADEMI ANGKATAN UDARA";

char text1[]="WAKTU OLAHRAGA PAGI";

char text2[]="WAKTU BERSIH-BERSIH";

char text3[]="WAKTU MAKAN PAGI";

char text4[]="WAKTU APEL PAGI ";

char text5[]="WAKTU KBM DIMULAI ";

char text6[]="WAKTU EKSTRAKURIKULER ";

char text7[]="WAKTU BELAJAR MALAM";

char text8[]="WAKTU APEL MALAM ";


// deklarasi FirebaseData
FirebaseData firebaseData;





//----------------------------------------------------------------------
// SETUP

void setup() {
  Serial.begin(9600);

  // WIFI
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    //Serial.print ( "." );
  }

  // NTP
  timeClient.begin();  

  // DMDESP
  Disp.start(); // Jalankan library DMDESP
  Disp.setBrightness(1); // Tingkat kecerahan
  Disp.setFont(FontDefault); // Huruf default
  
  // Setup Firebase host dan auth token
  Firebase.begin("https://testing-82343-default-rtdb.firebaseio.com/", "yvxqxGcZ7t8TqPZ7uaUdJ71UIqepoVbMREonZ40p");
}



//----------------------------------------------------------------------
// LOOP

void standby(){

  timeClient.update();
  
  Jam = timeClient.getHours();
  Men = timeClient.getMinutes();
  Det = timeClient.getSeconds();
  
   
  sprintf(waktu,"%02d:%02d:%02d", Jam, Men, Det);
  Disp.setFont(FontDefault);
  Disp.drawText(15,0,waktu); 
  
  Disp.setFont(FontBesar);
  Disp.drawText(geser,8,text0);
  
  // Menyimpan jam dalam variable string data
  sprintf(data,"%02d:%02d", Jam, Men);
  Serial.println(data);
  
}

void loop() {

  standby();

  String getText;
  String getSound;
  String strText = "/text";
  strText = data + strText; 
  String strSound = "/sound";
  strSound = data + strSound;
  // Mengambil data text dan disimpan dalam variable 'getText'
  if (Firebase.getString(firebaseData, strText)){
    if (firebaseData.dataType() == "string"){
      getText = firebaseData.stringData();
      // Length (with one extra character for the null terminator)
      int str_len = getText.length() + 1; 
      // Prepare the character array (the buffer) 
      char char_array[str_len];
      // Copy it over 
      getText.toCharArray(char_array, str_len);
    }   
  }

  // Mengambil data sound dan disimpan dalam variable 'getSound'
    if (Firebase.getString(firebaseData, strSound)){
    if (firebaseData.dataType() == "string"){
      getSound = firebaseData.stringData();
      // Length (with one extra character for the null terminator)
      int str_len = getText.length() + 1; 
      // Prepare the character array (the buffer) 
      char char_array[str_len];
      // Copy it over 
      getSound.toCharArray(char_array, str_len);
    }   
  }
  // Menjalankan jadwal kegiatan
  if (Firebase.getString(firebaseData, data)){
    Disp.setFont(FontBesar);
    Disp.drawText(geser,8,text1); // Cek apakah posible diganti dengan 'getText'
    Serial.println(getText);
    Serial.println(getSound);
  }



  
unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 100) {
      previousMillis = currentMillis;
      geser--;
      if(geser<-145){
        geser=145;
        Disp.clear();
      }
  }

  
  Disp.loop();

}
