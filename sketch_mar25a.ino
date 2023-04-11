// Importamos las librerias a usar en el sketch
#include <LiquidCrystal_I2C.h>  // Libreria para la pantalla LCD
#include <Wire.h>               // Libreria para la pantalla LCD
#include <DHT.h>                // Libreria para el sensor de temnperatura y humedad DHT11
#include <WiFi.h>               // Libreria para conectarce a la  red WIFI
#include <HTTPClient.h>         // Libreria para las peticiones a las API's

// WiFiClient client;
HTTPClient https;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Configuracion para el LCD
#define DHTPIN 14
#define LED 15
int ESTADO = 0;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHT11);

float humedad = 0.0;
float temperatura = 0.0;
float ultimaHumedad = 0.0;
float ultimaTemperatura = 0.0;

const int IdDispositivo = 1;

// Credenciales wifi
const char *ssid = "INFINITUM115"; // Nombre de la red
const char *password = "Gaby707381"; //Contraseña de la red



const String urlBase = "https://monitors.hopto.org:3000/api/monitors";


const char *root_ca =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n"
  "iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
  "cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
  "BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n"
  "MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n"
  "BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n"
  "aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n"
  "dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n"
  "AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n"
  "3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n"
  "tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n"
  "Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n"
  "VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n"
  "79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n"
  "c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n"
  "Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n"
  "c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n"
  "UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n"
  "Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n"
  "BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n"
  "A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n"
  "Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n"
  "VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n"
  "ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n"
  "8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n"
  "iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n"
  "Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n"
  "XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n"
  "qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n"
  "VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n"
  "L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n"
  "jjxDah2nGN59PRbxYvnKkKj9n\n"
  "-----END CERTIFICATE-----\n";

void setup() {
  Serial.begin(115200);
  dht.begin();  // Arancamos el DTH11
  
  Serial.println("ESP32 FUNCIONA CORRECTAMENTE");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println(F("DHTxx Test"));

  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println(F("Falla en la Lectura del Sensor"));
    return;
  }

  // Escribimos el Mensaje en el LCD.

  Serial.println();
  Serial.println("Conectando a la red");
  Serial.println("CARGANDO DATOS");
  Serial.println("POR FAVOR ESPERE");
  dht.begin();


  /* Conectando al WiFi*/
  WiFi.begin(ssid, password);

  /* Cargamos la conexcion*/

  //Serial.println("CONECTADO A LA RED");
  int contConnection = 0;
  while (WiFi.status() != WL_CONNECTED) {

    if (contConnection == 30) {
      break;
    }

    Serial.println("CONECTANDO A RED");

    if (contConnection % 2 == 0) {  // La condición que hemos impuesto
      digitalWrite(LED, HIGH);
      delay(1000);
    } else {
      digitalWrite(LED, LOW);
      delay(1000);
    }
    contConnection = contConnection + 1;
  }

  // Imprimimos en la LCD que estamos conectandonos a internet
  //Serial.println("\nConexcion exitosa");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("CONEXION EXITOSA.");
   // updateStatusDispositivo(3);
    delay(2000);
  } else {
    Serial.println("\nCONEXCION RECHAZADA");
    delay(1500);

    Serial.println("REINICIANDO");
    Serial.println("ESP32");
    delay(1000);
    ESP.restart();
  }

  digitalWrite(LED, HIGH);
}


String status = "";
void loop() {

  checkTempHumed();
  delay(3000);
}

void checkTempHumed() {

  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();

  if ((ultimaTemperatura != temperatura) || (ultimaHumedad != humedad)) {

    ultimaTemperatura = temperatura;
    ultimaHumedad = humedad;
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" C");
    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");
    int IdTipoRegistro = 1;
    guardarRegistroTyH(temperatura, humedad, IdTipoRegistro);
  }

  //digitalWrite(LED, HIGH);
  delay(1000);
  //digitalWrite(LED, LOW);
}

void guardarRegistroTyH(float temperatura, float humedad, int IdTipoRegistro) {

  String url = urlBase + "/TyH/nuevoRegistro";

  Serial.println(url);
  // https.begin(url, root_ca);  //Specify the URL and certificate
  https.begin(url);
  https.addHeader("Content-Type", "application/json");
  String data =
    "{\"data\":{\"temperatura\" : " + String(temperatura) + ",\"humedad\":" + String(humedad) + " ,\"IdTipoRegistro\" : " + String(IdTipoRegistro) + ", \"IdDispositivo\" : " + String(IdDispositivo) + " }}";
  Serial.println(data);
  int httpCode = https.POST(data);
  
  Serial.println("httpCode"+ String(httpCode));

  if (httpCode > 0) {
    String payload = https.getString();

  } else {
    Serial.println("Error en la peticion");
  }
  delay(1000);
}

String checkStatusdispositivo() {
  String url = urlBase + "/dispositivo/dispositivo_check_status/" + IdDispositivo;
  https.begin(url, root_ca);  //Specify the URL and certificate
  // https.begin(url);  //Specify the URL and certificate
  // https.addHeader("Content-Type", "application/json");

  int httpCode = https.GET();

  String payload = "-2";

  // Verificamos que no haya erron en la peticion de la api
  if (httpCode > 0) {
    payload = https.getString();
  } else {
    payload = "-2";
  }

  return payload;
}


void updateStatusDispositivo(int IdStatus) {

  String url = urlBase + "/dispositivo/dispositvo_cambiar_status";

  Serial.println(url);
  https.begin(url, root_ca);  //Specify the URL and certificate
  // https.begin(url);
  https.addHeader("Content-Type", "application/json");
  String data =
    "{\"dispositivo\":{\"IdDispositivo\" : " + String(IdDispositivo) + ",\"IdStatus\":" + String(IdStatus) + " }}";
  Serial.println(data);
  int httpCode = https.PUT(data);


  if (httpCode > 0) {
    String payload = https.getString();

  } else {
    Serial.println("Error en la peticion");
  }
  delay(1000);
}

void printLCD(String text_1, String text_2) {
  Serial.println(text_1 + temperatura);
  Serial.println(text_2);
}

void reiniciandoDispositivo() {
  Serial.println("REINICIANDO ESP32");
  Serial.println("POR PROTOCOLO");

  delay(1000);
  ESP.restart();
}

void guardandoLecturaActual() {
  ultimaTemperatura = temperatura;
  ultimaHumedad = humedad;
  Serial.print("Temp:");
  Serial.print(temperatura);
  Serial.print(" C");
  Serial.print("\t");

  Serial.print("Hume:");
  Serial.print(humedad);
  Serial.println(" %");

  int IdTipoRegistro = 2;
  guardarRegistroTyH(temperatura, humedad, IdTipoRegistro);
  //updateStatusDispositivo(9);
  delay(5000);
  // updateStatusDispositivo(3);
}