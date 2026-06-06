#ifndef DRILL_CONSTANTS_H
#define DRILL_CONSTANTS_H

//CAN
#define TX_PIN 5
#define RX_PIN 4

#define TO_DRILL 0x200
#define FROM_DRILL 0x201

//Drill
#define pwmPinDrill 19 
#define dirPinDrill 18
#define lichtschranke_pin 27

const int PPR_Drill = 10;

//Linear Unit
#define pwmPinLinear 26
#define dirPinLinear 25  
#define encA 22

const int PPR_Lin = 700;
const int pwmMin = 0;
const int pwmMax = 255;

const int drill_to_linear = 0.02; //Tune

//Ultraschall
#define echo_pin 33
#define trigger_pin 32

#define end_pin 21 

#endif