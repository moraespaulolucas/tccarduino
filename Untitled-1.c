#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

MFRC522::MIFARE_Key key;

MFRC522::StatusCode auth_status;
MFRC522::StatusCode read_status;

byte buffer[32];
byte bufferSize = 32;
byte dataSize = 8;
byte blockAddr = 1;

long baud = 9600;
int cardDelay = 1000;

void setup()
{
  Serial.begin(baud);
  SPI.begin();
  mfrc522.PCD_Init();

  delay(500);

  Serial.println("TAG CARD");
}

void loop()
{
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.print("CARD TAGGED");

  auth_status = mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A,
      blockAddr,
      &key,
      &(mfrc522.uid));
  if (auth_status != MFRC522::STATUS_OK)
  {
    Serial.println("AUTHENTICATION FAILED");
    Serial.println(mfrc522.GetStatusCodeName(auth_status));
    return;
  }

  read_status = mfrc522.MIFARE_Read(
      blockAddr,
      buffer,
      &bufferSize);
  if (read_status != MFRC522::STATUS_OK)
  {
    Serial.println("READING FAILED");
    Serial.println(mfrc522.GetStatusCodeName(read_status));
    return;
  }

  if (!buffer[0])
  {
    Serial.println();
    Serial.println("SEM REGISTRO");
  }
  else
  {
    for (byte i = 0; i < dataSize; i++)
    {
      Serial.write(buffer[i]);
    }
    Serial.println();
  }

  delay(cardDelay);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}