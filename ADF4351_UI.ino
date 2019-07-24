#include <Arduino.h>
// #include <HardwareSerial.h>
#include <SPI.h>
#include "src/UIX/UIX.h"
#include "src/ADF4351/ADF4351.h"
#include "esp_log.h"

static const char* TAG = "UI"; // ESP32_log

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

void btnfreqplus(int tag, UIXButton* obj);
void btnfreqminus(int tag, UIXButton* obj);
void voidfunc(int tag, UIXButton* obj);

char freqstr[100]="";
char recvstr[100]="";

double freq = INITFREQ;

bool tickflag = false;

UIXInputBox inptsend(20, 120, 180, 210, COLVSBLUE, COLCYAN, freqstr);
UIXButton btnsend(20, 90, 220, 250, COLVSBLUE, COLCYAN, "Apply", btnsendclick);

UIXButton freqplus(150, 220, 180, 210, COLLIGHTGRAY, COLCYAN, "+", voidfunc);
UIXButton freqminus(150, 220, 220, 250, COLLIGHTGRAY, COLCYAN, "-", voidfunc);

void btnsendclick(int tag, UIXButton* obj){
    // freq = atof(freqstr);
    sscanf(freqstr, "%lf", &freq);
    // Serial.println(freq);
    frequpdate();
    obj->selected = false;
}

void voidfunc(int tag, UIXButton* obj){
    //doing nothing
}

void frequpdate(){
    PLL->setFreq(freq);
    freqstr[0]='\0';    //clear received string
    sprintf(freqstr, "%lf", freq);
    inptsend.invalidate();
}

void setup(){
    hspi = new SPIClass(HSPI);
    PLL = new ADF4351(CS, CE, hspi);
    Serial.begin(115200);
    delay(500);
    hspi->begin();
    PLL->initialize(freq, 100); // initialize the PLL to output 400 Mhz, using an
    sprintf(freqstr, "%lf", freq);

    pinMode(15, OUTPUT);
    uixuserpanelnum = 1;
    uixpanels[0].label="SerialTest";
    uixpanels[0].uixobjects+=inptsend;
    uixpanels[0].uixobjects+=btnsend;
    uixpanels[0].uixobjects+=freqplus;
    uixpanels[0].uixobjects+=freqminus;

    uix.begin();
}

void loop(){
    tickflag = uix.tick();
    if(freqminus.pressed){
        if(tickflag){
            freq = freq - 1.0;
            frequpdate();
        }
    }
    if(freqplus.pressed){
        if(tickflag){
            freq = freq + 1.0;
            frequpdate();
        }
    }

}