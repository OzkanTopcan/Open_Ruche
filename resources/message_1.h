#ifndef MESSAGE_1_H
#define MESSAGE_1_H

#include "mbed.h"
#include "message.h"

class Message_1 : public Message {
    public:

        //constructeur
        Message_1(float tempDHT, float tempDSB, float Vitesse, float Lumi, float weight, float humDHT, float Direction):Message(){
            this->m_tempDHT = this->parseToInt16_t(tempDHT);
            this->m_tempDSB = this->parseToInt16_t(tempDSB);
            this->m_Vitesse = this->parseToInt16_t(Vitesse);
            this->m_Lumi = this->parseToInt16_t(Lumi);
            this->m_weight = this->parseToInt16_t(weight);
            this->m_humDHT = this->parseToInt8_t(humDHT);
            this->m_Direction = this->parseToInt8_t(Direction);
        };
            
        void send(){  //envoie de donnee a sigfox
            char donnee[] = "AT$SF=";
            this->sigfox.printf(donnee);
            
            this->writeInSendBuffer(this->m_tempDHT);
            
            this->writeInSendBuffer(this->m_tempDSB);
            
            this->writeInSendBuffer(this->m_Vitesse);
            
            this->writeInSendBuffer(this->m_Lumi);
            
            this->writeInSendBuffer(this->m_weight);
            
            this->writeInSendBuffer2(this->m_humDHT);
            
            this->writeInSendBuffer2(this->m_Direction);
            
            this->sigfox.printf("\r\n");
        };
    private:
        int16_t m_tempDHT;
        int8_t m_humDHT;
        int16_t m_tempDSB;
        int8_t m_Direction;
        int16_t m_Vitesse;
        int16_t m_Lumi;
        int16_t m_weight;
        
};


#endif
