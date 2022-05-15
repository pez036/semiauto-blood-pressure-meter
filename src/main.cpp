#include <mbed.h>
#include "drivers/LCD_DISCO_F429ZI.h"
#include <stdio.h>
#include <stdlib.h>

#define BACKGROUND 1
#define FOREGROUND 0
#define GRAPH_PADDING 5

#define I2C_SDA PC_9
#define I2C_SCL PA_8

I2C Wire(I2C_SDA, I2C_SCL);
const uint8_t writeAddress = 0x18 << 1;
const uint8_t readAddress = writeAddress | 0x1;

const float p_min = 0;
const float p_max = 300;
const float output_min = 419430.4;
const float output_max = 3774873.6;

float pressure_playback[1000] = {0};
float gap[1000] = {0};

LCD_DISCO_F429ZI lcd;

//buffer for holding displayed text strings
char display_buf[2][60];

float rawPressureTommHg(char pressure_raw_btye1, char pressure_raw_btye2, char pressure_raw_btye3) {
  float pressure_raw = ((uint32_t)pressure_raw_btye1<<16) | ((uint32_t)pressure_raw_btye2<<8) | ((uint32_t)pressure_raw_btye3);
  float pressrue_mmHg = (pressure_raw - output_min) * (p_max - p_min) / (output_max - output_min) + p_min;
  return pressrue_mmHg;
}

int main() {
  

  char sensor_status[1] = {0};
  char sensor_data[4] = {0};
  float pressure_mmHg = 0; // current pressure
  const char READ_COMMAND[] = {0xAA, 0x00, 0x00};

  float prev_pressure = 0.0;
  float diff = 0.0;
  //float temp = 0.0;
  int increase_flag = 1;

  while(1) {
    Wire.write(writeAddress, (const char*)READ_COMMAND, 3);
    Wire.read(readAddress, sensor_status, 1);
    // wait for data conversion
    wait_us(10000);
    Wire.read(readAddress, sensor_data, 4);

    // convert raw pressure to mmHg
    pressure_mmHg = rawPressureTommHg(sensor_data[1], sensor_data[2], sensor_data[3]);

    // data index
    int i = 0;

    // increase the cuff pressure to 150mmHg
    if(pressure_mmHg < 151 && increase_flag == 1){
      printf("Current pressure is: %.2f\n. Please increase the cuff pressrue to 150mmHg.", pressure_mmHg);
      wait_us(500000);
      continue;
    }
    increase_flag = 0;
    
    prev_pressure = pressure_mmHg;

    // pressure decrease phase
    while(1){
      pressure_playback[i] = pressure_mmHg; //save the pressure data into array
      diff = pressure_mmHg - prev_pressure;
      gap[i] = diff;//save the pressure difference data into array
      printf("Current pressure is: %.2f\n. Please reduce the pressure.", pressure_mmHg);
      if(pressure_mmHg<30){
        break;
      }
      if(diff < 3.5){
        printf("Deflation rate is: %.2f\n. Deflation rate is too slow.", diff);
      }
      else if(diff > 4.5){
        printf("Deflation rate is: %.2f\n. Deflation rate is too fast.", diff);
      }
      else  { 
        printf("Deflation rate is: %.2f\n. Deflation rate is good.", diff);
      }
      prev_pressure = pressure_mmHg;
      i++;
      wait_us(100000);
    }
  // calculate Heart Rate, Systolic, and Diastolic values
    float usefuldata[500];
    int j;
    for(j = 0; j<sizeof(usefuldata); j++){
      usefuldata[j] = pressure_playback[i];
    }

    int window_size = 30;
    float moving_avg[470];
    i = 0;
    while(i<sizeof(usefuldata)-30){
      float this_window_sum = 0;
      for(j = 0;j<30;j++){
        this_window_sum += pressure_playback[i+j];
      }
      moving_avg[i] = this_window_sum/30;
      i++;
    }

    //create oscillations
    float oscillations[470];
    for(i = 0; i < 470; i++){
      oscillations[i] = abs(moving_avg[i]-pressure_playback[i]);
    }

    //find peak
    int max_pulse_ind = 0;
    for(i = 1; i < 470; i++){
      if(oscillations[i] > oscillations[max_pulse_ind]){
        max_pulse_ind = i;
      }
    }
    float max_peak = oscillations[max_pulse_ind];
    //Systolic Blood Pressure is 50% of the maximum peak value to the left of the max pulse
    float systolic = max_peak * 0.5;
    //Diastolic Blood Pressure is 80% of the maximum peak value to the right of the max pulse
    float diastolic = max_peak * 0.8;

    //max pulse
    float max_pulse = pressure_playback[max_pulse_ind];

    //find the systolic blood pressure
    //find the diaastolic blood pressure

    //num_measurements_per_oscillation = sizeof(oscillation)/sizeof(peaks)
    //heart_rate_s = 1/((num_measurements_per_oscillation * time_per_measurement) / 1000) * 60  beats/min

    // print pressure on screen
    // snprintf(display_buf[0],60,"pressure: %.2f mmHg",pressure_mmHg);
    // lcd.DisplayStringAt(0, LINE(10), (uint8_t *)display_buf[0], LEFT_MODE);
    // printf("current pressure: %.2f\n", pressure_mmHg);
    // wait_us(1000000);
  return 0;

  }

}