#ifndef __BIGCARPET_H
#define __BIGCARPET_H

#include <Arduino.h>
#include <SPI.h>

#define COL_E 40
#define COL_A0 39
#define COL_A1 38
#define COL_A2 37
#define COL_A3 36

#define ROW_E 19
#define ROW_A0 20
#define ROW_A1 3
#define ROW_A2 46
#define ROW_A3 9

#define ADC_CS 10
#define ADC_DOUT 11
#define ADC_DCLK 12

#define SW 47
#define VCC_GND 5
#define VCC 0
#define GND 1

#define ROW_LENGTH 32
#define COL_LENGTH 32

const int ADS_CS_PIN = 10;    // 连接到 ADS7822 CS/SHDN (Pin 5)
const int SPI_SCK_PIN = 12;   // 连接到 ADS7822 DCLOCK (Pin 7)
const int SPI_MISO_PIN = 11;  // 连接到 ADS7822 D_OUT (Pin 6)
const int SPI_MOSI_PIN = 13;  // 未使用 (ADS7822 没有数据输入)


const float V_REF_VOLTAGE = 3.3;

extern uint8_t carpet_ID;
extern uint8_t flip_mode;
extern uint8_t multiple_rate;

extern uint16_t adc_list[ROW_LENGTH][COL_LENGTH];
extern uint16_t last_list[ROW_LENGTH][COL_LENGTH];
extern uint16_t gnd_list[COL_LENGTH];


void carpet_init(void);

void carpet_select_col(uint8_t num);
void carpet_select_row(uint8_t num);
void carpet_select_dir_res(uint8_t direction);
void carpet_select_dir_carpet(uint8_t direction);

uint8_t calculate_Checksum(void); 
uint16_t readADS7822(void);
void Bluetooth_SendPack(uint8_t Check_Sum);


#endif