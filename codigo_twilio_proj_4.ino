#include <Wire.h>

#include <UIPEthernet.h>
#include <RestClient.h>
#define envia_mensagem 1
#define toca_buzzer 2

int buzzer_2 = 6;

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF1, 0x41 };
EthernetClient ethclient;

RestClient client = RestClient("192.168.3.186", 3000, ethclient);

const char* sid = "ACdd043bf052ac6900296b9554ee23e1c5";
const char* token = "47e5667f618521db8cbb00efdef9770b";
const char* to = "5511986293810";
const char* from = "12013992780";
String response;

void setup() {
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.println("Inicio");

  if (Ethernet.begin(mac)) {
    Serial.println("Conectado via DHCP");
    Serial.print("IP recebido:"); Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Falha de rede!");
  }
}

void loop() {}



void enviaSMS(String mensagem) {

  String parametros = "sid=";
  parametros.concat(sid);

  parametros.concat("&token=");
  parametros.concat(token);

  parametros.concat("&to=");
  parametros.concat(to);

  parametros.concat("&from=");
  parametros.concat(from);

  parametros.concat("&body=" + mensagem);

  Serial.println(parametros);

  //Perguntar se isso vai rodar msm se não precisar mandar a mensagem de invasão

  int statusCode = client.post("/sms", parametros.c_str(), &response);
  if (statusCode != 500) {
    tone(buzzer_2, 1500);
    delay(500);
    noTone(buzzer_2);
    delay(500);
    tone(buzzer_2, 1500);
    delay(500);
    noTone(buzzer_2);
    delay(500);
    tone(buzzer_2, 1500);
    delay(500);
    noTone(buzzer_2);
    enviaSMS("Erro de sistema");

    // Ação para contornar o sms
    // Ligar para a policia
    // Avisar o vizinho do lado
    // Tocar buzzer irritante
  }
  Serial.print("Status da resposta: ");
  Serial.println(statusCode);
  Serial.print("Resposta do servidor: ");
  Serial.println(response);
  delay(1000);
}

void receiveEvent(int length) {
  while (Wire.available()) {
    // Ler mensagem de tamanho 1 byte
    byte c = Wire.read();

    // Verificar qual tipo de mensagem é e executar ação correspondente
    switch (c) {
      case envia_mensagem:
      
        enviaSMS("Sua casa foi invadida");
        break;
      default:
        Serial.print(F("Mensagem desconhecida via I2C:"));
        Serial.print(c);
    }
  }
}
