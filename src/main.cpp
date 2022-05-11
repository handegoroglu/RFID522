
#include <SPI.h>
#include <RFID.h>

const byte birinci_led = 7;
const byte ikinci_led = 6;
const byte buzzer = 5;
RFID lrt720(10, 9);
byte kart[10][5] = {};
byte ogretmeModuKarti[5] = {64, 17, 152, 162, 107};

bool ogretmeModuAktif = false;
int okutulanKart = 0;

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    lrt720.init();
    pinMode(birinci_led, OUTPUT);
    pinMode(ikinci_led, OUTPUT);
    pinMode(buzzer, OUTPUT);
}

void loop()
{
    if (lrt720.isCard())
    {
        if (lrt720.readCardSerial())
        {
            Serial.println("Kart Bulundu ID : ");
            Serial.print(lrt720.serNum[0]);
            Serial.print(" , ");
            Serial.print(lrt720.serNum[1]);
            Serial.print(" , ");
            Serial.print(lrt720.serNum[2]);
            Serial.print(" , ");
            Serial.print(lrt720.serNum[3]);
            Serial.print(" , ");
            Serial.print(lrt720.serNum[4]);
            Serial.println(" ");

            bool ogrenmeKartiMi = true;
            for (int i = 0; i < 5; i++)
            {
                if (lrt720.serNum[i] != ogretmeModuKarti[i])
                {

                    ogrenmeKartiMi = false;
                    break;
                }
            }
            if (ogrenmeKartiMi == true)
            {
                Serial.println("Öğretme Modu Kartı Algılandı.");
                digitalWrite(ikinci_led, HIGH);
                digitalWrite(birinci_led, HIGH);
                delay(1000);
                digitalWrite(ikinci_led, LOW);
                digitalWrite(birinci_led, LOW);
                delay(1000);

                if (ogretmeModuAktif == true)
                {
                    ogretmeModuAktif = false;
                    Serial.println("Öğretme Modu Kapali.");
                }       
                else
                {
                    Serial.println("Öğretme Modu Acik.");
                    ogretmeModuAktif = true;
                }

                ogrenmeKartiMi = false;
                 
            }

            else if (ogretmeModuAktif == true)
            {
                bool kartKayitliMi = true;
                for (int i = 0; i < 10; i++)
                {
                    kartKayitliMi = true;
                    for (int x = 0; x < 5; x++)
                    {
                        if (lrt720.serNum[x] != kart[i][x])
                        {
                            kartKayitliMi = false;
                            break;
                        }
                    }
                    if (kartKayitliMi == true)
                    {
                        break;
                    }
                }

                if (kartKayitliMi == false)
                {
                    for (int x = 0; x < 5; x++)
                    {
                        kart[okutulanKart][x] = lrt720.serNum[x];
                    }
                    Serial.println("Kart kaydedildi");
                }
                else
                {
                    Serial.println("Kart zaten kayitli.");
                }

                okutulanKart++;
            }
            else
            {
                bool izin = false;
                for (int i = 0; i < 10; i++)
                {
                    for (int x = 0; x < 5; x++)
                    {
                        if (lrt720.serNum[x] != kart[i][x])
                        {
                            izin = false;
                            break;
                        }
                        else
                        {
                            izin = true;
                        }
                    }
                    if (izin == true)
                    {
                        break;
                    }
                }

                if (izin == true)
                {

                    Serial.println("izin verildi.");
                    digitalWrite(ikinci_led, HIGH);
                    delay(1000);
                    digitalWrite(ikinci_led, LOW);
                    delay(1000);
                }
                else
                {

                    Serial.println("izinsiz giriş.");
                    digitalWrite(birinci_led, HIGH);
                    digitalWrite(buzzer, HIGH);
                    delay(1000);
                    digitalWrite(birinci_led, LOW);
                    digitalWrite(buzzer, LOW);
                    delay(1000);
                }
            }
        }

        while (lrt720.isCard())
        {
            delay(1000);
            lrt720.halt();
        }
    }

    lrt720.halt();
}
