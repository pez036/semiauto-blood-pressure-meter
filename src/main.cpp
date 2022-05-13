#include <mbed.h>
#include "drivers/LCD_DISCO_F429ZI.h"
#define BACKGROUND 1
#define FOREGROUND 0
#define GRAPH_PADDING 5

#define I2C_SDA PC_9
#define I2C_SCL PA_8

const uint8_t writeAddress = 0x18 << 1;
const uint8_t readAddress = writeAddress | 0x1;

const float p_min = 0;
const float p_max = 300;
const float output_min = 419430.4;
const float output_max = 3774873.6;


float rawPressureTommHg(uint32_t pressure_raw) {
  /*if (pressure_raw == 0) {
    return (float)rand()/((float)RAND_MAX/100);
  }*/
  float pressrue_mmHg = ((float)pressure_raw - output_min) * (p_max - p_min) / (output_max - output_min) + p_min;
  return pressrue_mmHg;
}

LCD_DISCO_F429ZI lcd;

//buffer for holding displayed text strings
char display_buf[2][60];
uint32_t graph_width=lcd.GetXSize()-2*GRAPH_PADDING;
uint32_t graph_height=graph_width;

//sets the background layer 
//to be visible, transparent, and
//resets its colors to all black
void setup_background_layer(){
  lcd.SelectLayer(BACKGROUND);
  lcd.Clear(LCD_COLOR_BLACK);
  lcd.SetBackColor(LCD_COLOR_BLACK);
  lcd.SetTextColor(LCD_COLOR_GREEN);
  lcd.SetLayerVisible(BACKGROUND,ENABLE);
  lcd.SetTransparency(BACKGROUND,0x7Fu);
}

//resets the foreground layer to
//all black
void setup_foreground_layer(){
    lcd.SelectLayer(FOREGROUND);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_LIGHTGREEN);
}

int main() {
  I2C Wire(I2C_SDA, I2C_SCL);

  char sensor_status[1] = {0};
  char sensor_data[4] = {0};
  uint32_t pressure_raw = 0;
  float pressure_mmHg = 0;
  const char READ_COMMAND[] = {0xAA, 0x00, 0x00};

  //print initial data on screen
  setup_background_layer();
  setup_foreground_layer();
  snprintf(display_buf[0],60,"current pressure: %.2f mmHg",0.0);
  lcd.SelectLayer(FOREGROUND);
  lcd.DisplayStringAt(0, LINE(17), (uint8_t *)display_buf[0], LEFT_MODE);
  lcd.SelectLayer(FOREGROUND); 

  while(1) {
    Wire.write(writeAddress, (const char*)READ_COMMAND, 3);
    Wire.read(readAddress, sensor_status, 1);
    // wait for data conversion
    wait_us(5000);
    Wire.read(readAddress, sensor_data, 4);

    // convert raw pressure to mmHg
    pressure_raw = (sensor_data[1]<<16) | (sensor_data[2]<<8) | (sensor_data[3]);
    pressure_mmHg = rawPressureTommHg(pressure_raw);

    // print pressure on screen
    snprintf(display_buf[0],60,"pressure: %.2f mmHg",pressure_mmHg);
    lcd.SelectLayer(FOREGROUND);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)display_buf[0], LEFT_MODE);
    wait_us(1000000);
  }
}