#include <mbed.h>
#include "drivers/LCD_DISCO_F429ZI.h"
#define BACKGROUND 1
#define FOREGROUND 0
#define GRAPH_PADDING 5

#define I2C_SDA PC_9
#define I2C_SCL PA_8

I2C Wire(I2C_SDA, I2C_SCL);
int writeAddress = 0x18 << 1;
int readAddress = writeAddress | 0x1;

const float p_min = 0;
const float p_max = 300;
const float output_min = 419430.4;
const float output_max = 3774873.6;
const char READ_COMMAND[] = {0xAA, 0x00, 0x00};

LCD_DISCO_F429ZI lcd;

//buffer for holding displayed text strings
char display_buf[2][60];
/*
float dummy_data[] = {-0.32, -0.37, -0.4, -0.28, -0.49, -0.47, -0.48, -0.45, -0.4, 1.01, 1.56, 1.31, 1.18, 1.4, 2.25, 2.47, 1.94, 1.75, 2.54, 3.19, 2.94, 2.8, 3.66, 4.52, 4.2, 4.04, 5.19, 6.81, 6.57, 6.41, 6.54, 10.24, 10.34, 9.82, 9.83, 13.64, 14.17, 13.57, 13.17, 17.49, 22.39, 20.2, 19.74, 22.77, 29.84, 29.0, 27.44, 29.31, 39.41, 40.91, 39.43, 38.5, 50.47, 51.56, 49.8, 48.55, 63.56, 66.47, 63.94, 62.65, 67.45, 82.37, 79.16, 78.39, 86.16, 101.08, 94.86, 92.25, 93.64, 111.96, 112.04, 110.22, 109.36, 127.73, 127.65, 125.8, 124.15, 134.43, 140.52, 138.23, 136.94, 153.21, 154.11, 152.39, 151.32, 152.97, 171.02, 168.5, 167.03, 166.04, 184.15, 186.33, 184.5, 183.27, 182.65, 200.17,
202.26, 200.1, 198.87, 197.99, 197.55, 196.82, 196.32, 195.56, 195.14, 194.69, 194.38, 193.92, 193.74, 193.59, 193.18, 192.94, 192.66, 192.49, 192.23, 191.77, 191.13, 190.63, 190.31, 189.56, 188.72, 186.94, 185.22, 183.53, 181.99, 180.68, 179.52, 178.77, 176.78, 175.5, 174.39, 173.26, 172.06, 170.91, 169.21, 168.14, 167.53, 166.24, 165.23, 164.12, 162.56, 161.41, 160.32, 159.33, 158.25, 157.43, 155.96, 154.87, 153.81, 152.84, 151.83, 150.76, 149.76, 148.36, 147.63, 146.92, 145.76, 144.77, 143.96, 142.63, 141.68, 140.75, 139.85, 138.97, 138.64, 137.05, 136.14, 135.38, 134.51, 133.64, 132.83, 131.56, 130.76, 130.55, 129.67, 128.61, 127.71, 126.93, 125.6, 124.79, 124.0, 123.2, 122.61, 122.41, 120.87, 119.88, 119.25, 118.45, 117.7, 117.05, 115.94, 115.55, 115.55, 114.48, 113.38, 112.73, 111.62, 110.83, 110.18, 109.48, 108.9, 109.18, 108.63, 107.16, 106.36, 105.7, 104.89, 104.19, 103.57, 102.81, 103.28, 102.48, 101.68, 100.78, 100.18, 98.95, 98.49, 97.81, 97.16, 97.04, 97.34, 96.27, 95.4, 94.89, 94.16, 93.35, 92.68, 92.07, 91.21, 91.04, 91.54, 90.95, 90.37, 89.71, 88.8, 88.01, 87.39, 86.73, 86.17, 85.63, 85.42, 85.49, 84.94, 84.33, 83.7, 83.19, 82.1, 81.44, 80.84, 80.26, 79.96, 80.23, 79.86, 78.88, 78.29, 77.7, 76.98, 76.3, 75.71, 74.89, 75.31, 75.06, 74.44, 73.57, 72.94, 71.95, 71.21, 70.65, 70.07, 69.55, 69.79, 69.17, 68.44, 67.77, 67.24, 66.6, 66.02, 65.45, 64.62, 64.47, 64.66, 64.22, 63.52, 62.94, 62.25, 61.73, 61.27, 60.87, 60.41, 59.98, 60.11, 59.67, 59.14, 58.69, 58.29, 57.87, 57.21, 56.84, 56.55, 56.25, 56.43, 56.17, 55.53, 54.86, 54.47, 53.94, 53.48, 53.02, 52.65, 52.54, 52.21, 51.69, 51.22, 50.78, 50.35, 49.6, 49.22, 48.79, 48.5, 48.59, 48.2, 47.47, 47.05, 46.7, 46.29, 45.89, 45.53, 45.14, 44.61, 44.72, 44.58, 44.17, 43.62, 43.27, 42.68, 42.48, 42.25, 42.0, 41.57, 41.59, 40.99, 40.56, 40.21, 39.91, 39.56, 39.25, 38.94, 38.62, 38.65, 38.49, 38.16, 37.62, 37.25, 36.84, 37.15, 36.93, 36.43, 35.96, 35.89, 35.66, 35.27, 35.15, 35.07, 34.67, 34.34, 33.67, 33.36, 33.07, 32.94, 32.87, 32.52, 32.08, 31.85, 31.58, 31.28, 31.12, 30.87, 30.72, 30.23, 30.28, 30.02, 29.71, 29.48, 29.22, 28.87, 28.55, 28.27, 28.07, 27.85, 27.84, 27.45, 27.17, 26.99, 26.83, 26.58, 26.3, 25.99, 25.85, 25.76, 25.69, 25.41, 25.12, 24.93, 24.69, 24.44, 24.22, 24.0, 23.81, 23.85, 23.45, 23.16, 23.03, 22.82, 22.64, 22.42, 22.19, 22.18, 22.04, 21.84, 21.55, 21.34, 21.06, 20.91, 20.77, 20.55, 20.59, 20.42, 20.19, 19.93, 19.78, 19.63, 19.38, 19.25, 19.1, 19.07, 18.81, 18.64, 18.5, 18.34, 18.2, 17.95, 17.89, 17.73, 17.71, 17.57, 17.41, 17.2, 16.95, 16.83, 16.69, 16.59, 16.47, 16.37, 16.34, 16.07, 15.91, 15.81, 15.71, 15.53, 15.4, 15.33, 15.13, 15.18, 15.05, 14.91};
*/
float pressureData[1000];
float slope[1000];
int mapIdx = 0;

float rawPressureTommHg(char pressure_raw_btye1, char pressure_raw_btye2, char pressure_raw_btye3) {
  float pressure_raw = ((uint32_t)pressure_raw_btye1<<16) | ((uint32_t)pressure_raw_btye2<<8) | ((uint32_t)pressure_raw_btye3);
  float pressrue_mmHg = (pressure_raw - output_min) * (p_max - p_min) / (output_max - output_min) + p_min;
  return pressrue_mmHg;
}

float readData() {
  char sensor_status[1] = {0};
  char sensor_data[4] = {0};
  float pressure_mmHg = 0;
  
  Wire.write(writeAddress, (const char*)READ_COMMAND, 3);
  Wire.read(readAddress, sensor_status, 1);
  // wait for data conversion
  wait_us(5000);
  Wire.read(readAddress, sensor_data, 4);
  pressure_mmHg = rawPressureTommHg(sensor_data[1], sensor_data[2], sensor_data[3]);
  return pressure_mmHg;
}

void recordPressure() {
  float prev_pressure = 0.0;
  float maxDeflateRate = 6 * 0.2;
  float minDeflateRate = 3 * 0.2;
  float current_pressure = 0.0;
  int idx = 0;
  // increase to 200 mmHg
  snprintf(display_buf[0],60,"increase to 200 mmHg");
  lcd.DisplayStringAt(0, LINE(7), (uint8_t *)display_buf[0], LEFT_MODE);
  printf("increase to 200 mmHg\n");
  while (1) {
    current_pressure = readData();
    if (prev_pressure > 200) {
      snprintf(display_buf[0],60,"please deflate");
      lcd.DisplayStringAt(0, LINE(7), (uint8_t *)display_buf[0], LEFT_MODE);
    }
    if ((prev_pressure > 200) && (prev_pressure - current_pressure > 0) ) {
      break;
    }

    prev_pressure = current_pressure;
    snprintf(display_buf[0],60,"pressure: %.2f", current_pressure);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)display_buf[0], LEFT_MODE);
    printf("pressure: %.2f\n", current_pressure);

    // sample every 0.2 second
    wait_us(200000);
  }

  lcd.Clear(LCD_COLOR_WHITE);
  snprintf(display_buf[0],60,"please deflate");
  lcd.DisplayStringAt(0, LINE(7), (uint8_t *)display_buf[0], LEFT_MODE);
  printf("deflating\n");
  // start recording when start to deflate, sample every 0.2 second
  
  for (int i = 0; i < 1000; ++i) {
    current_pressure = readData();
    if (current_pressure < 30) {
      break;
    }
    pressureData[i] = current_pressure;
    float deflateAmount = prev_pressure - current_pressure;
    // check if deflation rate is appropriate
    
    if (deflateAmount > maxDeflateRate) {
      snprintf(display_buf[0],60,"deflating too fast!");
      lcd.DisplayStringAt(0, LINE(9), (uint8_t *)display_buf[0], LEFT_MODE);
      printf("deflating too fast!\n");
    } else if ((deflateAmount > 0) && (deflateAmount < minDeflateRate)) {
      snprintf(display_buf[0],60,"deflating too slow!");
      lcd.DisplayStringAt(0, LINE(9), (uint8_t *)display_buf[0], LEFT_MODE);
      printf("deflating too slow!\n");
    } else {
      snprintf(display_buf[0],60,"good deflation rate");
      lcd.DisplayStringAt(0, LINE(9), (uint8_t *)display_buf[0], LEFT_MODE);
      printf("good deflation rate\n");
    }
    snprintf(display_buf[0],60,"pressure: %.2f", current_pressure);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)display_buf[0], LEFT_MODE);
    printf("pressure: %.2f\n", current_pressure);
    // sample every 0.2 second
    wait_us(200000);
    slope[i] = current_pressure - prev_pressure;
    prev_pressure = current_pressure;
  }
}

float getSystolicPressure() {
  int heartBeatCount = 0;
  for (int i = 0; pressureData[i] > 80; ++i) {
    if (slope[i] > 0) {
      heartBeatCount = 0;
      for (int j = i+1; j < i+12; ++j) {
        if (slope[j] > 0) {
          heartBeatCount++;
        }
      }
      if (heartBeatCount > 0) {
        //systolicIdx = i;
        return pressureData[i];
      }
    }
  }
  return 0.0;
}

int getMapIdx() {
  float maxSlope = 0.0;
  int maxSlopeIdx = 0;
  for (int i = 0; pressureData[i] > 80; ++i) {
    if (slope[i] > 0) {
      float totalSlope = 0.0;
      for (int j = i+1; slope[j] >= 0; ++j) {
        totalSlope += slope[i];
      }
      if (slope[i] > maxSlope) {
        maxSlope = totalSlope;
        maxSlopeIdx = i;
      }
    }
  }
  mapIdx = maxSlopeIdx;
  return maxSlopeIdx;
}

float getDistolicPressure(float systolicPressure, float MapPressure) {
  return MapPressure - systolicPressure + MapPressure;
}

int getBpm() {
  int totalHeartBeat = 0;
  for (int i = mapIdx -50; i < mapIdx +50; ++i) {
    if (slope[i] > 0 && slope[i+1] < 0) {
      totalHeartBeat++;
    }
  }

  return totalHeartBeat*3;
}

int main() {

  while(1) {
    // record pressure, save in pressureData in mmHg
    recordPressure();
    // get systolic pressure
    float systolicPressure = getSystolicPressure();
    // get Map pressure
    int mapIdx = getMapIdx();
    // get diastolic pressure
    float diastolicPressure = getDistolicPressure(systolicPressure, pressureData[mapIdx]);
    // get bpm
    int bpm = getBpm();
    // print pressures on screen
    lcd.Clear(LCD_COLOR_WHITE);
    snprintf(display_buf[0],60,"systolic: %.2f", systolicPressure);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)display_buf[0], LEFT_MODE);
    snprintf(display_buf[0],60,"diastolic: %.2f", diastolicPressure);
    lcd.DisplayStringAt(0, LINE(11), (uint8_t *)display_buf[0], LEFT_MODE);
    snprintf(display_buf[0],60,"bpm: %d", bpm);
    lcd.DisplayStringAt(0, LINE(12), (uint8_t *)display_buf[0], LEFT_MODE);

    printf("systolic: %.2f, diastolic: %.2f, map: %.2f, bpm: %d\n", systolicPressure, diastolicPressure, pressureData[mapIdx], bpm);
    printf("====================== end ======================\n");
    wait_us(20000000);
    lcd.Clear(LCD_COLOR_WHITE);
  }
}