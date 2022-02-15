#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>

const int DATA_PIN = 19;
const int CLOCK_PIN = 5;
const int LATCH_PIN = 18;
byte SHIFT_REGISTER[7];

const int LEVEL[6] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB};
const int LEVEL2[5] = {0x3F, 0x9F, 0xCF, 0xE7, 0xF3};
const int ROW2[5] = {0xC0, 0x60, 0x30, 0x18, 0x0C};

const char* ssid = "Ubee2339";
const char* password = "2FA8B52339";

const char *mqtt_server = "hairdresser.cloudmqtt.com";
const int mqtt_port = 15476;
const char *mqtt_user = "qudxvewe";
const char *mqtt_pass = "W2V2LvCJ715s";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void shift_out() {                 // esto actualizará el código hexadecimal a los registros de desplazamiento
  digitalWrite(LATCH_PIN, 0);
  for (int i = 0; i < 7; i++) {
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, SHIFT_REGISTER[i]);
  }
  digitalWrite(LATCH_PIN, 1);
}

void clear_out() {                // esto borrará todos los leds
  for (int i = 0; i < 6; i++) {
    SHIFT_REGISTER[i] = 0x00;              // uso códigos hexadecimales en lugar de binarios porque creo que es más corto

  }
  SHIFT_REGISTER[6] = 0xFF;
  shift_out();                  // esto cambiará el comando 'clear_out'.

}

void GrowingCube() {

  SHIFT_REGISTER[6] = 0x3F;                                 //2x2
  SHIFT_REGISTER[0] = SHIFT_REGISTER[1] = 0x0C;
  shift_out();
  delay(300);

  SHIFT_REGISTER[6] = 0x1F;                               //3x3
  SHIFT_REGISTER[0] = SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = 0x1C;
  shift_out();
  delay(300);

  SHIFT_REGISTER[6] = 0x0F;                               //4x4
  SHIFT_REGISTER[0] = SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = 0x3C;
  shift_out();
  delay(300);

  SHIFT_REGISTER[6] = 0x07;                               //5x5
  SHIFT_REGISTER[0] = SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = SHIFT_REGISTER[4] = 0x7C;
  shift_out();
  delay(300);

  SHIFT_REGISTER[6] = 0x03;                               //6x6
  SHIFT_REGISTER[0] = SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = SHIFT_REGISTER[4] = SHIFT_REGISTER[5] = 0xFC;
  shift_out();
  delay(300);

  clear_out();
  delayMicroseconds(2000);

  SHIFT_REGISTER[6] = 0x80;                               //5x5
  SHIFT_REGISTER[5] = SHIFT_REGISTER[4] = SHIFT_REGISTER[3] = SHIFT_REGISTER[2] = SHIFT_REGISTER[1] = 0xF8;
  shift_out();
  delay(300);
  clear_out();
  delayMicroseconds(2000);

  SHIFT_REGISTER[6] = 0xC0;                               //4x4
  SHIFT_REGISTER[5] = SHIFT_REGISTER[4] = SHIFT_REGISTER[3] = SHIFT_REGISTER[2] = 0xF0;
  shift_out();
  delay(300);
  clear_out();
  delayMicroseconds(2000);

  SHIFT_REGISTER[6] = 0xE0;                               //3x3
  SHIFT_REGISTER[5] = SHIFT_REGISTER[4] = SHIFT_REGISTER[3] = 0xE0;
  shift_out();
  delay(300);
  clear_out();
  delayMicroseconds(2000);

  SHIFT_REGISTER[6] = 0xF0;                                 //2x2
  SHIFT_REGISTER[5] = SHIFT_REGISTER[4] = 0xC0;
  shift_out();
  delay(300);
  clear_out();
  delayMicroseconds(1000);
}

void frame6x6() {

  for (int k = 0; k < 200; k ++) {
    SHIFT_REGISTER[6] = 0x7B;
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0xFF;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = SHIFT_REGISTER[4] = 0x84;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

    SHIFT_REGISTER[6] = 0x84;
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0x84;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

  }
}

void frame4x4() {

  for (int k = 0; k < 200; k ++) {
    SHIFT_REGISTER[6] = 0xB7;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[4] = 0x78;
    SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = 0x48;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

    SHIFT_REGISTER[6] = 0xCF;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[4] = 0x48;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);


  }
}

void FrameInFrame() {
  for (int k = 0; k < 200; k ++) {
    SHIFT_REGISTER[6] = 0x7B;                                 //top and button
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0xFF;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = SHIFT_REGISTER[4] = 0x84;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

    SHIFT_REGISTER[6] = 0xB7;
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0x84;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[4] = 0x78;
    SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = 0x48;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

    SHIFT_REGISTER[6] = 0xCF;
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0x84;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[4] = 0x48;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

  }
}

void custom() {
  for (int k = 0; k < 200; k ++) {
    SHIFT_REGISTER[6] = 0x7B;                                 //top and button
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0xFF;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = SHIFT_REGISTER[4] = 0x84;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

    SHIFT_REGISTER[6] = 0xB7;
    SHIFT_REGISTER[0] = SHIFT_REGISTER[5] = 0x00;
    SHIFT_REGISTER[1] = SHIFT_REGISTER[4] = 0x78;
    SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = 0x48;
    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

    SHIFT_REGISTER[6] = 0xCF;
    SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = 0x30;

    shift_out();
    delayMicroseconds(1000);
    clear_out();
    delayMicroseconds(1000);

  }
}

void centerCube() {
  SHIFT_REGISTER[6] = 0xCF;
  SHIFT_REGISTER[2] = SHIFT_REGISTER[3] = 0x30;
  shift_out();
  delay(500);
}

void randomCUBE() {         //2x2 cube
  for (int k = 0; k < 20; k ++) {
    int SRrandom = random(0, 5);

    SHIFT_REGISTER[6] = LEVEL2[random(0, 5)];
    SHIFT_REGISTER[SRrandom] = SHIFT_REGISTER[SRrandom + 1] = ROW2[random(0, 5)];
    shift_out();
    delay(300);
    clear_out();
  }
}

void randomLED() {        //only 1 LED will light
  for (int k = 0; k < 20; k ++) {
    SHIFT_REGISTER[6] = LEVEL[random(0, 6)];      //randomly choose a level
    bitSet(SHIFT_REGISTER[random(0, 6)], random(2, 8));   // in that level randomly choose a row and in that row a led that will light, the others will off
    shift_out();
    delay(300);
    clear_out();
  }
}

void full() {                   // if you use this program the whole cube will light
  for (int i = 0; i < 6; i ++) {  //this line will switch all the rows high
    SHIFT_REGISTER[i] = 0xFC;
  }
  SHIFT_REGISTER[6] = 0x00;                   // and this line will switch all the levels low
  shift_out();
}

void flash(){
  for (int i = 0; i < 10; i++){
    full();
    delay(300);
    clear_out();
    delay(300);
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Mensaje recibido bajo el tópico -> ");
  Serial.print(topic);
  Serial.print("\n");

  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }

  if ((char)payload[0] == '0'){
    digitalWrite(2, LOW);
    Serial.println("\n Led Apagado");
  } else if ((char)payload[0] == '1'){
    digitalWrite(2, HIGH);
    Serial.println("\n Led Encendido");
  } else if ((char)payload[0] == 'q'){
    full();
    delay(4000);
    Serial.println("\n Cubo encendido por completo");
  } else if ((char)payload[0] == 'w'){
    for(int p=0; p<2; p++){
      randomCUBE();
    }
    Serial.println("\n Cubos aleatorios encendiendose");
  } else if ((char)payload[0] == 'e'){
    for(int k=0; k<2; k++){
      flash();
    }
    Serial.println("\n Cubo flasheando");
  } else if ((char)payload[0] == 'r'){
    for(int u=0; u<2; u++){
      randomLED();
    }
    Serial.println("\n Leds aleatorios encendidos");
  } else if ((char)payload[0] == 't'){
    for(int v=0; v<4; v++){
      GrowingCube();
    }
    Serial.println("\n Cubo creciente encendiendose");
  } else if ((char)payload[0] == 'y'){
    for(int x=0; x<7; x++){
      frame6x6();
    }
    Serial.println("\n Orillas 6x6 encendidas");
  } else if ((char)payload[0] == 'u'){
    for(int r=0; r<10; r++){
      centerCube();
      frame4x4();
    }
    Serial.println("\n Experimental encendido");
  } else if ((char)payload[0] == 'i'){
    for(int h=0; h<6; h++){
      FrameInFrame();
    }
    Serial.println("\n Cuadro dentro de un Cuadro encendido");
  }

  Serial.println();
}

void setup_wifi(){
  Serial.println();

  Serial.println("Conectado a  -> ");
  Serial.println(ssid);

  //Me conecto a la red WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("Conexión Exitosa!");

  Serial.println("Mi ip es -> ");
  Serial.println(WiFi.localIP());
}

void setup() {
  // put your setup code here, to run once:
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  delay(10);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect(){
  while (!client.connected()) {
    Serial.println("Intentando Conexión MQTT");

    String clientId = "CrisZobe";
    clientId = clientId + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(),mqtt_user, mqtt_pass)){
      Serial.println("Conexión a MQTT exitosa!!!");
      client.publish("salidaKebyn", "primer mensaje");
      client.subscribe("entradaLedKebyn");
      client.subscribe("/Kebyn/Patrones");
    } else {
      Serial.println("Falló la conexión ");
      Serial.print(client.state());
      Serial.print(" Se intentará de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if(client.connected()==false){
    reconnect();
  }

  client.loop();

  /*if (millis() - lastMsg > 2000){
    lastMsg = millis();
    value++;
    String mes ="Criszobe manda un valor -> " + String(value);
    mes.toCharArray(msg, 50);
    client.publish("salidaKebyn", msg);
    Serial.println("Mensaje enviado -> " + String(value));
  }*/

  delay(2000);
}
