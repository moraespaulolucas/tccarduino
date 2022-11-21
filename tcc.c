#include <ESP8266WebServer.h> // libs do modulo arduino
#include <ESP8266HTTPClient.h>

#include <SPI.h>     // lib para comunicacao serial
#include <MFRC522.h> // lib do modulo rfid

#define SS_PIN D2 // pinos de conexao do modulo rfid
#define RST_PIN D1

MFRC522 mfrc522(SS_PIN, RST_PIN); // instancia modulo rfid

byte byteArrayId[4];       // array de bytes para guardar id -> mfrc522.uid.size == 4
char charArrayId[32] = ""; // array de chars para guardar id
String stringId;           // variavel final com id em string

const char *wifi_ssid = "vem arduino";  // rede wifi
const char *wifi_password = "12345678"; // senha

// METODO DE INICIALIZACAO
void setup()
{
  Serial.begin(115200); // inicia comunicacao serial
  while (!Serial)
    ;
  SPI.begin();

  mfrc522.PCD_Init(); // liga modulo rfid

  delay(500); // delay de inicio

  WiFi.begin(wifi_ssid, wifi_password); // conecta wifi
  Serial.println("Connecting to WiFi network...");

  while (WiFi.status() != WL_CONNECTED) // enquanto nao conecta
  {
    Serial.println(".");
    delay(1000);
  }

  Serial.println("WiFi network connected successfully!"); // conexao bem sucedida
  Serial.print("Network: ");
  Serial.println(wifi_ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// METODO PRINCIPAL
void loop()
{
  if (WiFi.status() == WL_CONNECTED) // executa se estiver conectado
  {
    getId();
  }
}

// METODO PARA PEGAR ID
int getId()
{
  if (!mfrc522.PICC_IsNewCardPresent()) // nova leitura de id
  {
    return 0; // interrompe metodo
  }
  if (!mfrc522.PICC_ReadCardSerial()) // nova leitura de id
  {
    return 0; // interrompe metodo
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) // percorre id
  {
    byteArrayId[i] = mfrc522.uid.uidByte[i]; // salva cada byte em byteArrayId
  }

  toString(byteArrayId, mfrc522.uid.size, charArrayId); // converte para char[] e salva em charArrayId
  stringId = charArrayId;                               // salva como string em stringId
  Serial.println(stringId);

  delay(1500); // delay
  return 1;    // retorna sucesso
}

// METODO PARA CONVERTER BYTE EM CHAR
void toString(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
  }
  buffer[len * 2] = '\0';
}
