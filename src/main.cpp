#include <mbed.h>
#include "drivers/LCD_DISCO_F429ZI.h"
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
  float pressure_mmHg = 0;
  const char READ_COMMAND[] = {0xAA, 0x00, 0x00};

  while(1) {
    Wire.write(writeAddress, (const char*)READ_COMMAND, 3);
    Wire.read(readAddress, sensor_status, 1);
    // wait for data conversion
    wait_us(10000);
    Wire.read(readAddress, sensor_data, 4);

    // convert raw pressure to mmHg
    pressure_mmHg = rawPressureTommHg(sensor_data[1], sensor_data[2], sensor_data[3]);

    // print pressure on screen
    snprintf(display_buf[0],60,"pressure: %.2f mmHg",pressure_mmHg);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)display_buf[0], LEFT_MODE);
    printf("current pressure: %.2f\n", pressure_mmHg);
    wait_us(1000000);
  }
}