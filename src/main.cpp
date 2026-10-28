#include <Arduino.h>
#include <iostream>

#define RELAY 13
#define LDR 2
#define N 3

const uint16_t THRESHOLD_DARK = 2000;
const uint16_t THRESHOLD_LIGHT = 3000;
const uint32_t adc_period = 10;

uint16_t readings[N];
uint16_t idx = 0;
uint32_t sum = 0;

uint32_t lastTimeChange = 0;


uint16_t movingAvg(uint16_t rawData)
{
  sum -= readings[idx];
  sum += rawData;
  readings[idx] = rawData;
  ++idx;

  if (idx >= N)
  {
    idx = 0;
  }

  return sum / N;
}

void filterInit(uint16_t initData)
{
  for (u8_t i = 0; i < N; ++i)
  {
    readings[i] = initData;
    sum += initData;
  }
}

void setup(void)
{
  digitalWrite(RELAY, LOW); // безпечний старт
  pinMode(RELAY, OUTPUT);
  pinMode(LDR, INPUT);
  filterInit(analogRead(LDR));
}

void loop(void)
{
  if (millis() - lastTimeChange > adc_period)
  {
    lastTimeChange = millis();
    uint16_t adc_value = analogRead(LDR);
    uint16_t filtered_adc_value = movingAvg(adc_value);
    // std::cout << "ADC filtered value: " << filtered_adc_value << std::endl; //debug mode

    if (filtered_adc_value < THRESHOLD_DARK){
      digitalWrite(RELAY, HIGH); // printf("Light is ON\n");
    } else if (filtered_adc_value > THRESHOLD_LIGHT){
      digitalWrite(RELAY, LOW); // printf("Light is OFF\n");
    }
  }
}