#ifndef MESSAGE_H
#define MESSAGE_H

#include "mbed.h"

#define PIN_RX      PA_10 
#define PIN_TX      PA_9 
class Message {
    public:
        
        //constructeur
        Message():sigfox(PIN_TX, PIN_RX){};
        
        //Methode
        virtual void send() = 0;
        
     
   
    protected:
        
        void writeInSendBuffer(int16_t var){
            if(var >= 0 && var <256) 
            {   
                if(var >= 0 && var < 16)
                {
                    this->sigfox.printf("000%hx", var);
                }
                else
                {
                this->sigfox.printf("00%hx", var); 
                }
            }
            else this->sigfox.printf("0%hx", var);                     
        };                                                              
    
        void writeInSendBuffer2(int8_t var){
                if(var >= 0 && var < 16)
                {
                    this->sigfox.printf("0%hx", var);
            }
            else this->sigfox.printf("%hx", var);                     
        };      
        
        int16_t parseToInt16_t(int var){ // toutes nos données sont codées sur 16 bits. 2 chiffres sont gardés apres la virgule 
            int16_t temp = var * 10;
            return temp;
        };
        
        int16_t parseToInt16_t(float var){ // surcharge de fonction pour les floatants
            int16_t temp = var * 10;
            return temp;
        };
        
        int8_t parseToInt8_t(int var){ // toutes nos données sont codées sur 16 bits. 2 chiffres sont gardés apres la virgule 
            int8_t temp = var;
            return temp;
        };
        
        int8_t parseToInt8_t(float var){ // surcharge de fonction pour les floatants
            int8_t temp = var;
            return temp;
        };
        Serial sigfox; // declaration de la communication serie avec le module wisol
        
    
};


#endif

