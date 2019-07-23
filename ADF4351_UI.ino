#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include "src/UIX/UIX.h"
#include "src/ADF4351/ADF4351.h"


#define CLOCKSPEED 1000000
#define CS 26
#define MOSI 13
#define CE 25

#define INITFREQ 250

// HardwareSerial SerialTest(2);

UIX uix;

SPIClass * hspi = NULL;
ADF4351 * PLL = NULL;

void btnsendclick(int tag, UIXButton* obj);
void btnclearrecvclick(int tag, UIXButton* obj);

char sendstr[100]="";
char recvstr[100]="";

double freq = INITFREQ;

bool tickflag = false;

UIXInputBox inptsend(20, 120, 220, 250, COLVSBLUE, COLCYAN, sendstr);
UIXButton btnsend(20, 90, 260, 290, COLVSBLUE, COLCYAN, "Apply", btnsendclick);

UIXButton freqplus(100, 170, 220, 250, COLORANGE, COLCYAN, "+", btnfreqplus);
UIXButton freqminus(100, 170, 270, 300, COLORANGE, COLCYAN, "-", btnfreqminus);

void btnsendclick(int tag, UIXButton* obj){
    freq = atof(sendstr);
    // Serial.println(freq);
    frequpdate();
    obj->selected = false;
}

void btnfreqplus(int tag, UIXButton* obj){
    while(obj->pressed && tickflag == true){
        freq = freq + 1;
        frequpdate();
    }
}

void btnfreqminus(int tag, UIXButton* obj){
    while(obj->pressed && tickflag == true){
        freq = freq - 1;
        frequpdate();
    }
}

void frequpdate(){
    PLL->setFreq(freq);
}

void setup(){
    hspi = new SPIClass(HSPI);
    PLL = new ADF4351(CS, CE, hspi);
    Serial.begin(115200);
    delay(500);
    hspi->begin();
    PLL->initialize(350, 100); // initialize the PLL to output 400 Mhz, using an

    pinMode(15, OUTPUT);
    uixuserpanelnum = 1;
    uixpanels[0].label="SerialTest";
    uixpanels[0].uixobjects+=inptsend;
    uixpanels[0].uixobjects+=btnsend;
    uix.begin();

    // delay(10000);
    // PLL->initialize(250, 100); // initialize the PLL to output 400 Mhz, using an
}


void loop(){
    tickflag = uix.tick();

    // bool received = false;
    // while(SerialTest.available()){
    //     received = true;
    //     int inspos = strlen(recvstr);
    //     if(inspos < 50){
    //         char incoming = SerialTest.read();
    //         recvstr[inspos] = incoming;
    //         Serial.write(incoming);
    //     }
    //     recvstr[inspos+1]='\0';
    // }
    // if(received) txtrecv.invalidate();
}