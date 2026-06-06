#pragma once

//Sensor Pins
#define encoder_A 22
#define lichtschranke_pin 27
#define echo_pin 32 //blau
#define trigger_pin 33 //gruen
#define endschalter_pin 21 

void sensor_init(void);

void measure_distance(void *pvParameters);

volatile long get_pulses_drill();

volatile long get_pulses_linear();

float get_distance();

void reset_pulses();