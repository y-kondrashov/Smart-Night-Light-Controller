#include <Arduino.h>
#include <iostream>
#include <iomanip>

#define RELAY 13
#define LDR 2
#define N 4

const u16_t THRESHOLD_DARK = 2000;
u16_t THRESHOLD_LIGHT = 3000;
u32_t adc_period = 50;
u32_t adc_max = 4095;

u16_t readings[N];
u16_t idx = 0;
u32_t sum = 0;

u32_t lastTimeChange = 0;


u16_t movingAvg(u16_t rawData)
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

void filterInit(u16_t initData)
{
  for (u8_t i = 0; i < N; ++i)
  {
    readings[i] = initData;
    sum += initData;
  }
}

void setup()
{
  pinMode(RELAY, OUTPUT);
  pinMode(LDR, INPUT);
  filterInit(analogRead(LDR));
}

void loop()
{
  if (millis() - lastTimeChange > adc_period)
  {
    lastTimeChange = millis();
    u16_t adc_value = analogRead(LDR);
    u16_t filtered_adc_value = movingAvg(adc_value);
    // std::cout << "ADC raw value: " << adc_value << " | U calculated: " << std::fixed << std::setprecision(2) << u_calc << "V | Read millivolts: " << u_measured_mv << std::endl;
    std::cout << "ADC filtered value: " << filtered_adc_value << std::endl;

    if (filtered_adc_value < THRESHOLD_DARK)
    {
      digitalWrite(RELAY, HIGH);
      printf("Light is ON\n");
    }
    else if (filtered_adc_value > THRESHOLD_LIGHT)
    {
      digitalWrite(RELAY, LOW);
      printf("Light is OFF\n");
    }
  }
}