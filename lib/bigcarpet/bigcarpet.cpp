#include "bigcarpet.h"

SPISettings adsSPISettings(1000000, MSBFIRST, SPI_MODE1);

uint16_t adc_list[ROW_LENGTH][COL_LENGTH];
uint16_t last_list[ROW_LENGTH][COL_LENGTH];
uint16_t gnd_list[COL_LENGTH];

void carpet_init(void) {
    // 设置列引脚为输出
    pinMode(COL_E, OUTPUT);
    pinMode(COL_A0, OUTPUT);
    pinMode(COL_A1, OUTPUT);
    pinMode(COL_A2, OUTPUT);
    pinMode(COL_A3, OUTPUT);

    pinMode(ROW_E, OUTPUT);
    pinMode(ROW_A0, OUTPUT);
    pinMode(ROW_A1, OUTPUT);
    pinMode(ROW_A2, OUTPUT);
    pinMode(ROW_A3, OUTPUT);

    pinMode(SW, OUTPUT);

}

void carpet_select_col(uint8_t num) {
    uint8_t n = num % 16;
    uint8_t e = num/16;
    digitalWrite(COL_E, e); // e ? HIGH : LOW
    digitalWrite(COL_A0, (n & 0x01)); // (n & 1) ? HIGH : LOW
    digitalWrite(COL_A1, (n & 0x02)); // (n & 2) ? HIGH : LOW
    digitalWrite(COL_A2, (n & 0x04)); // (n & 4) ? HIGH : LOW
    digitalWrite(COL_A3, (n & 0x08)); // (n & 8) ? HIGH : LOW
}

void carpet_select_row(uint8_t num) {
    int8_t n = num % 16;
    uint8_t e = num/16;
    digitalWrite(VCC_GND, LOW); // VCC
    digitalWrite(ROW_E, e); // e ? HIGH : LOW
    digitalWrite(ROW_A0, (n & 0x01)); // (n & 1) ? HIGH : LOW
    digitalWrite(ROW_A1, (n & 0x02)); // (n & 2) ? HIGH : LOW
    digitalWrite(ROW_A2, (n & 0x04)); // (n & 4) ? HIGH : LOW
    digitalWrite(ROW_A3, (n & 0x08)); // (n & 8) ? HIGH : LOW
}

void carpet_select_dir_res(uint8_t direction) {
    digitalWrite(SW, direction); // e ? HIGH : LOW
}

void carpet_select_dir_carpet(uint8_t direction) {
    digitalWrite(VCC_GND, direction); // e ? HIGH : LOW
}

uint8_t calculate_Checksum(void) 
{
    uint8_t checksum = 0;
	uint8_t i=0;
	uint8_t j=0;
	for(i=0;i<ROW_LENGTH;i++)
	{
		for(j=0;j<COL_LENGTH;j++)
		{
			checksum +=adc_list[i][j];
		}
	}
    return checksum;
}

uint16_t readADS7822(void) {
  
  // 1. 拉低 CS，启动转换 
  digitalWrite(ADS_CS_PIN, LOW);

  // 2. 开始 SPI 事务
  SPI.beginTransaction(adsSPISettings);
  
  // 3. 执行一个 16 位的 SPI 传输
  // 我们发送 0x0000 (16个时钟脉冲) 并同时接收 16 位数据
  // 接收到的数据格式为：[Null, B11, B10, ..., B0, B1, B2, B3]
  // Bx 是 LSB-first 的重复数据 [cite: 760, 609]
  uint16_t rawData = SPI.transfer16(0x0000);

  // 4. 结束 SPI 事务
  SPI.endTransaction();

  // 5. 拉高 CS，停止转换并使芯片进入关断模式（如果时钟停止）
  digitalWrite(ADS_CS_PIN, HIGH);
  
  // 6. 解析数据
  // 原始数据 (rawData) 格式: N B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0 r1 r2 r3
  // N = Null bit, rX = 重复的 LSB-first 位
  // 我们需要提取 B11 到 B0 (共12位)
  // 为此，我们将数据右移 3 位来移除 r1, r2, r3
  // 然后使用 0x0FFF (0b0000111111111111) 进行“与”运算，以屏蔽掉 Null bit
  uint16_t adcValue = (rawData >> 3) & 0x0FFF;

  return adcValue;
}