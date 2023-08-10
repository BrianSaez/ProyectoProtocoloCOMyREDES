#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#define SerialUNO Serial2
#define RX_PIN 32 //a 2 arduino
#define TX_PIN 33 //a 4 arduino

BluetoothSerial SerialBT;

const int LED1 = 25;
const int LED2 = 26;
const int LED3 = 27;
const char* direccionesESP[] = {"0011","0100","0101"};
void setup()
{
  Serial.begin(115200);       //Serial Monitor Serial
  SerialBT.begin("ESP32-BT"); //Serial BT + Nombre BT
  SerialUNO.begin(9600,SERIAL_8N1,RX_PIN,TX_PIN); // Serial Arduino UNO
  //Configuracion para los leds
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT);

}

void loop()
{
  if (SerialBT.available())
  {
    String paquete = SerialBT.readString();
    int largo = paquete.length();
    String encabezado = paquete.substring(0,10);
    String largoStr = String(largo);
    encabezado.setCharAt(0, largoStr[0]);
    encabezado.setCharAt(1, largoStr[1]);
    //String largoSt = encabezado.substring(0,2);
    String dirDes = paquete.substring(6,10);
    String dirOri = paquete.substring(2,6);
    //String orden = paquete.substring(10,11);
    Serial.println("Un paquete nuevo a llegado");
    Serial.print("Encabezado: ");
    Serial.println(encabezado);
    Serial.print("Largo: ");
    Serial.println(largo);
    Serial.print("Dirección de origen: ");
    Serial.println(dirOri);
    Serial.print("Dirección de destino: ");
    Serial.println(dirDes);
    //Solo mostrar orden o mensaje si la dirección pertenece al esp-32
    bool DirEnDominio = false;
    for(int i=0; i< sizeof(direccionesESP) / sizeof(direccionesESP[0]); i++)
    {
      if (dirDes == direccionesESP[i])
      {
        DirEnDominio = true;
        Serial.println("La dirección de destino está en el dominio");
        String orden = paquete.substring(10,largo);
        Serial.print("La orden es: ");
        Serial.println(orden);
        if (dirDes == "0011" && orden == "1")
        {
          Serial.println("Encendiendo LED1");
          digitalWrite(LED1,HIGH);
        }
        if (dirDes == "0011" && orden == "0")
        {
          Serial.println("Apagando LED1");
          digitalWrite(LED1,LOW);
        }
        if (dirDes == "0100" && orden == "1") 
        {
          Serial.println("Encendiendo LED2");
          digitalWrite(LED2, HIGH);
        }
        if (dirDes == "0100" && orden == "0") 
        {
          Serial.println("Apagando LED2");
          digitalWrite(LED2, LOW);
        }
        if (dirDes == "0101" && orden == "1") 
        {
          Serial.println("Encendiendo LED3");
          digitalWrite(LED3, HIGH);
        }
        if (dirDes == "0101" && orden == "0") 
        {
          Serial.println("Apagando LED3");
          digitalWrite(LED3, LOW);
        }
      }
      
    }
    if (dirDes == "0001")
    {
      Serial.print("A llegado un mensaje: ");
      String mensaje = paquete.substring(10,largo);
      Serial.println(mensaje);
    }
    if (dirOri == "0000" && !DirEnDominio && dirDes != "0001")
    {
      String paqueteUNO = String(largo) + "0001" + dirDes + paquete.substring(10, largo);
      Serial.println("Dirección no encontrada, derivando paquete a ARDUINO");
      SerialUNO.println(paqueteUNO);
    }
    
  }
  if (SerialUNO.available()) 
  {
    String datazo = SerialUNO.readString();
    if(datazo = "1")
    {
      Serial.println("ERR0R, la dirección no se encuentra en el dominio de la red");
    }
  }
}
