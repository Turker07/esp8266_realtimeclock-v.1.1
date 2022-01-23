
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <time.h>


#ifdef CREDENTIALS
const char* ssid = mySSID;
const char* password = myPASSWORD;
#else
const char* ssid = "wifiname";  // wifi ismini buraya giriniz örneğin const char* ssid = "mft";
const char* password = "wifi password"; // wifi şifrenizi buraya giriniz örneğin const char* password = "123456"; 
#endif

const char* NTP_SERVER = "ch.pool.ntp.org";
const char* TZ_INFO    = "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00";  // bulunduğunuz zaman dilimini seçiniz (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;


void setup() {
  Serial.begin(115200);
  Serial.println("gerçek zaman test ");
  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {  // WİFİ.begin da yer alan wifi ssid ve password bağlantısını kontrol eder. bağlı değil ise her 200ms deneme yapar. 
    delay(200);
   
  }
  Serial.println("\n\nWiFi connected\n\n");

configTime(0, 0, NTP_SERVER);
  // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv  Bölgeniz için saat dilimi kodlarına buradan ulaşabilirsiniz.
  setenv("TZ", TZ_INFO, 1);

  
  showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
}


void loop() {
  
  getNTPtime(10);  // oluşturulan fonksiyonda 10 saniyede bir veriler çekliyor burada fonksiyonun içerisini değiştirerek güncellemeleri değişterebilirsiniz. 
  showTime(timeinfo); // zaman bilgisini gösteren fonksiyon 
  delay(1000);
}

bool getNTPtime(int sec) {

  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } 
    while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) 
    return false; 

     
    Serial.print("now ");  Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now)); // time_output içerisinde bulunan verileri istenen %a  %d-%m-%y %T düzenler. 
    Serial.println(time_output);
    Serial.println();
  }
  return true;
}




void showTime(tm localTime) {
  Serial.print(localTime.tm_mday); // gün bilgisi
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1); // ay bilgisi
  Serial.print('/');
  Serial.print(localTime.tm_year - 100); // yıl bilgisi
  Serial.print('-');
  Serial.print(localTime.tm_hour);  // saat bilgisi
  Serial.print(':');
  Serial.print(localTime.tm_min); // dakika bilgisi
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}
