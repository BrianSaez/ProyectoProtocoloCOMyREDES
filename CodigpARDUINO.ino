#include <SoftwareSerial.h>
#define RX_PIN 4 //a 33 esp
#define TX_PIN 2 //a 32 esp
SoftwareSerial SerialESP(RX_PIN,TX_PIN); // Rx,Tx
const int LED4 = 7;
const int LED5 = 8;
const char* direccionesUNO[] = {"0110","0111"};
void setup() {
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  Serial.begin(9600); // Velocidad de comunicación del puerto serial
  SerialESP.begin(9600);
}

void loop() {
  if (SerialESP.available()) {
    String paquete = SerialESP.readString();
    paquete.trim(); // Eliminamos espacios o caracteres adicionales al inicio y al final

    int largo = paquete.substring(0, 2).toInt();
    String encabezado = paquete.substring(0,10);
    String largoStr = String(largo);
    encabezado.setCharAt(0, largoStr[0]);
    encabezado.setCharAt(1, largoStr[1]);
    String dirDes = paquete.substring(6, 10);
    String dirOri = paquete.substring(2,6);
    //String orden = paquete.substring(10, 11);
    Serial.println("Un paquete nuevo a llegado");
    Serial.print("Encabezado: ");
    Serial.println(encabezado);
    Serial.print("Largo: ");
    Serial.println(largo);
    Serial.print("Dirección de origen: ");
    Serial.println(dirOri);
    Serial.print("Dirección de destino: ");
    Serial.println(dirDes);

    bool DirEnDominio = false;
    for(int i=0; i< sizeof(direccionesUNO) / sizeof(direccionesUNO[0]); i++)
    {
      if(dirDes == direccionesUNO[i])
      {
        DirEnDominio = true;
        Serial.println("La dirección de destino está en el dominio");
        String orden = paquete.substring(10,largo);
        Serial.print("La orden es: ");
        Serial.println(orden);
        if(dirDes == "0110" && orden == "1")
        {
          Serial.println("Encendiendo LED4");
          digitalWrite(LED4,HIGH);
        }
        if(dirDes == "0110" && orden == "0")
        {
          Serial.println("Apagando LED4");
          digitalWrite(LED4,LOW);
        }
        if(dirDes == "0111" && orden == "1")
        {
          Serial.println("Encendiendo LED5");
          digitalWrite(LED5,HIGH);
        }
        if(dirDes == "0111" && orden == "0")
        {
          Serial.println("Apagando LED5");
          digitalWrite(LED5,LOW);
        }
      }
    }
    if(dirDes == "0010")
    {
      Serial.print("A llegado un mensaje: ");
      String mensaje = paquete.substring(10,largo);
      Serial.println(mensaje);
    }
    if(!DirEnDominio && dirDes != "0010")
    {
      Serial.println("Dirección no encontrada, Retornando mensaje");
      String datazo = "1";
      SerialESP.println(datazo);
    }
  }
}
