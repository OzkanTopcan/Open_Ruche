/*--------------Librairies--------------*/
#include "mbed.h"
#include "platform/mbed_thread.h"
#include "DHT22.h"
#include "DS1820.h"
#include "Anemometer.h"
#include "HX711.h"
#include "message_1.h"


/*---------Prototypes fonctions---------*/

bool MesureDHT22();
bool MesureDS18B20();
bool MesureLumi();
bool MesureDirection();
bool MesureVitesse();
bool MesurePoids();
void MesureGlobale();
void ClignotterLed();

/*----------Variables globales----------*/

static float tempDHT;
static float humDHT;
static float tempDSB;
static float Direction;
static float Vitesse;
static float Lumi;
static float weight;
static float calibration_factor = 14200;
static int   averageSamples = 10;    

/*------------Flag capteurs-------------*/

static bool flagDHT22;
static bool flagDS18B20;
static bool flagDirection;
static bool flagVitesse;
static bool flagLumi;
static bool flagPoids;

/*------------Configurations------------*/

Serial      device(PA_9, PA_10);
DHT22       sensorDHT22(PA_11);
DS1820      sensorDS1820(PA_8);
AnalogIn    ain(PA_6);
AnalogIn    sensorGirouette(PA_1);
CAnemometer sensorAnemo(PB_0, 1000);
DigitalOut  gpo(PA_12);
AnalogIn    scaleRaw(PA_4);
HX711       scale(PA_4, PA_12);
DigitalOut led(LED1);
 
 
/*------------Code principal------------*/

int main()
{
    ClignotterLed();
    scale.setScale(3000);
    scale.tare();
    scale.setOffset(1);
    while(true) {
        MesureGlobale();
        //Message_1 msg1(tempDHT, tempDSB, 6.4, (Lumi)/10, 54, humDHT, 5.0); // instanciation du corps du message avec les données à envoyer
        Message_1 msg1(tempDHT, tempDSB, Vitesse, (Lumi)/10, weight, humDHT, Direction); // instanciation du corps du message avec les données à envoyer

        //Message_1 msg1(25.2, 25.3, 5.4, (2605)/10, 40.6, 50.2, 5.0); // instanciation du corps du message avec les données à envoyer
        msg1.send();
        ClignotterLed();
        printf("message envoyé \r\n");
        wait(720);
    }
}

/*--------Definitions fonctions---------*/

void ClignotterLed(){
    led = 1;
    int i = 0;
    for ( i  = 0 ; i < 3 ; i++){
     if(led == 1) 
     {
        led =! led ;
        wait(0.2);
    }     
}

void MesureGlobale(){
    printf("\n\r+++++++++++Mesure Globale++++++++++++\n\r");
    
    printf("\n\r----------------DHT22----------------\n\r");
    flagDHT22 = MesureDHT22();
    if (flagDHT22 == false){
        wait(1);
        flagDHT22 = MesureDHT22();
        }
        
    printf("\n\r---------------DS18B20---------------\n\r");
    flagDS18B20 = MesureDS18B20();
    if (flagDS18B20 == false){
        flagDS18B20 = MesureDS18B20();
        }
        
    printf("\n\r--------------Direction--------------\n\r");
    flagDirection = MesureDirection();
    if (flagDirection == false){
        wait(1);
        flagDirection = MesureDirection();
        }
    printf("\n\r---------------Vitesse---------------\n\r");
    flagVitesse = MesureVitesse();
    if (flagVitesse == false){
        flagVitesse = MesureVitesse();
        }
        
    printf("\n\r-------------Luminositée-------------\n\r");
    flagLumi = MesureLumi();
    if (flagLumi == false){
        flagLumi = MesureLumi();
        }
    
    printf("\n\r----------------Poids----------------\n\r");
    flagPoids = MesurePoids();
    }

bool MesureDHT22() {
    bool boule;
    boule = sensorDHT22.sample();
    if (boule) {
        tempDHT = ((float)(sensorDHT22.getTemperature()))/10;
        humDHT = ((float)(sensorDHT22.getHumidity()))/10;
        printf("Température : %.2f \r\nHumiditée : %.2f \r\n" , tempDHT, humDHT);
        return true;
    }
    else {
        printf("DHT22 non détecté \r\n");
        return false;
    }
}

bool MesureDS18B20() {
    int result = 0;
    bool test = sensorDS1820.begin();
    if (1) {
        sensorDS1820.startConversion();   // start temperature conversion from analog to digital
        wait(1);                  // let DS1820 complete the temperature conversion
        result = sensorDS1820.read(tempDSB); // read temperature from DS1820 and perform cyclic redundancy check (CRC)
        switch (result) {
            case 0:                 // no errors -> 'temp' contains the value of measured temperature
                printf("Température = %3.1f%°C \r\n", tempDSB);
                return true;
   
            case 1:                 // no sensor present -> 'temp' is not updated
                printf("no sensor present \n\r");
                return false;
   
            case 2:                 // CRC error -> 'temp' is not updated
                printf("CRC error\r\n");
                return false;
        }
    }
    else {
        printf("DS18B20 non détecté \r\n");
        return false;
    }
    return false;
}

bool MesureLumi() {
    Lumi = (int)(8.606*exp(2.1*(ain.read()*3.3)));
    printf("Luminosité = %3.1f Lux \r\n", Lumi);
    return true;
}


bool MesureDirection(){
        float Girouette = 0;
        Girouette = (float)sensorGirouette.read()*100;
            if ( Girouette >= 85 && Girouette <=86.99 ){
                Direction = 0;
                printf("Le vent souffle au nord \r\n"); 
                return true;
            }
        if ( Girouette >= 72 && Girouette <=73.99 ){
                Direction = 7;
                printf("Le vent souffle au nord-ouest \r\n");
                return true;
            }
        if ( Girouette >= 78 && Girouette <=79.99 ){
                Direction = 1;
                printf("Le vent souffle au nord-est \r\n");
                return true;
            }
        if ( Girouette >= 90 && Girouette <=91.99 ){
                Direction = 4;
                printf("Le vent souffle au sud \r\n");
                return true;
        }
        if ( Girouette >= 83 && Girouette <=84.99 ){
                Direction = 3;
                printf("Le vent souffle au sud-est \r\n");
                return true;
        }  
        if ( Girouette >= 76 && Girouette <=77.99 ){
                Direction = 5;
                printf("Le vent souffle au sud-ouest \r\n");
                return true;
        }
        if ( Girouette >= 88 && Girouette <=89.99 ){
                Direction = 2;
                printf("Le vent souffle à l'est \r\n");
                return true;
        }
        if ( Girouette >= 81 && Girouette <=82.99 ){
                Direction = 6;
                printf("Le vent souffle à l'ouest\r\n");
                return true;
        }
        printf("girouette non détectée \r\n");
        return false;
        }
       
bool MesureVitesse(){
    float somme = 0 ;
    int  j;
    for ( j = 0; j < 100000; j++){
        somme += sensorAnemo.GetCurrentWindSpeed();
        }
    somme /= 100000;
    
    Vitesse = somme;
    if (somme == 0){
        printf("Pas de vent détecté \r\n");
        return false;
        }
    printf("Vitesse du vent = %f m/s \r\n", Vitesse);  
    return true;
}

bool MesurePoids(){
    scale.setScale(calibration_factor); 
    weight = scale.getGram()+43.9;
    printf("Poids = %f kg \n\r",weight); 
    return true;
}
