#include <bigcarpet.h>

float R_temp=0.0;
float temp=0.0;
uint16_t send_list[ROW_LENGTH][COL_LENGTH];

// --- 协议定义 ---
#define FRAME_HEADER_0 0xAA
#define FRAME_HEADER_1 0x55
#define FRAME_TRAILER_0 0xBB
#define FRAME_TRAILER_1 0x66

// --- 辅助函数 ---

/**
 * @brief 计算数据负载的 8 位校验和 (简单求和)
 * @param data 指向数据负载的指针
 * @param len 数据负载的字节长度
 * @return 8 位校验和
 */
uint8_t calculate_checksum(uint8_t* data, uint16_t len) {
  uint8_t sum = 0;
  for (uint16_t i = 0; i < len; i++) {
    sum += data[i]; // 自动模256溢出
  }
  return sum;
}

/**
 * @brief 将数据打包并通过串口发送
 * @param payload 指向数据负载的指针 (例如 (uint8_t*)send_list)
 * @param payload_len 数据负载的字节长度 (例如 sizeof(send_list))
 */
void sendDataPacket(uint8_t* payload, uint16_t payload_len) {
  // 1. 计算校验和
  uint8_t checksum = calculate_checksum(payload, payload_len);

  // 2. 发送帧头 (2 bytes)
  Serial.write(FRAME_HEADER_0);
  Serial.write(FRAME_HEADER_1);

  // 3. 发送数据长度 (2 bytes, 高位在前, High-Byte First)
  Serial.write((payload_len >> 8) & 0xFF); // 高位
  Serial.write(payload_len & 0xFF);         // 低位

  // 4. 发送数据负载 (N bytes)
  // Serial.write() 可以直接发送一个字节数组
  Serial.write(payload, payload_len);

  // 5. 发送校验和 (1 byte)
  Serial.write(checksum);

  // 6. 发送帧尾 (2 bytes)
  Serial.write(FRAME_TRAILER_0);
  Serial.write(FRAME_TRAILER_1);
}

void setup() {
  Serial.begin(921600);
  while (!Serial) { delay(10); }
  // Serial.println("ESP32-S3 ADS7822 ADC Reader");

  // 初始化 CS 引脚
  pinMode(ADS_CS_PIN, OUTPUT);
  digitalWrite(ADS_CS_PIN, HIGH); // CS 默认拉高
  carpet_init();
//   carpet_select_col(0);
//   carpet_select_row(0);
    carpet_select_dir_carpet(VCC);
    carpet_select_dir_res(GND);

  // 初始化 SPI
  // begin(SCK, MISO, MOSI, CS)
  // 如果使用默认引脚，只需调用 spi.begin()
  SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, 4); // -1 表示我们手动管理 CS
}

// 您的原始 loop() 函数
void loop() {
    uint8_t row = 0;
    uint8_t col = 0;
    carpet_select_dir_carpet(VCC);
    carpet_select_dir_res(GND);
    delayMicroseconds(100);
    for(row = 0; row < ROW_LENGTH; row++) {
        carpet_select_row(row);
        delayMicroseconds(30);
        for(col = 0; col < COL_LENGTH; col++) {
            carpet_select_col(col);
            delayMicroseconds(30); // 确保采样稳定
            adc_list[row][col] = readADS7822();
        }
    }
    carpet_select_dir_carpet(GND);
    carpet_select_dir_res(VCC);
    delayMicroseconds(100);
    for(col = 0; col < COL_LENGTH; col++) {
        delayMicroseconds(30); // 确保采样稳定
        gnd_list[col] = readADS7822();
    }
    for(row = 0; row < ROW_LENGTH; row++) {
        for(col = 0; col < COL_LENGTH; col++) {
            R_temp=(float)adc_list[row][col]/(float)gnd_list[col];
            temp = R_temp*10000-10.0;
            send_list[row][col]=(uint16_t)(temp);
            // send_list[row][col] = adc_list[row][col];
        }
    }
    
    // --- 新增的数据发送代码 ---
    
    // 1. 确定数据负载的指针和长度
    // C++ 中二维数组在内存中是连续存储的，可以直接转换
    uint8_t* payload_ptr = (uint8_t*)send_list;
    
    // 2. 使用 sizeof() 自动获取数组的总字节大小
    const uint16_t payload_size = sizeof(send_list); 

    // 3. 发送完整数据包
    sendDataPacket(payload_ptr, payload_size);
    
    // --- 结束 ---

    delay(1); // 每秒读取一次
}