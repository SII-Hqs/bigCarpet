// #include <bigcarpet.h>

// float R_temp=0.0;
// float temp=0.0;
// uint16_t send_list[ROW_LENGTH][COL_LENGTH];

// void setup() {
//   Serial.begin(115200);
//   while (!Serial) { delay(10); }
//   // Serial.println("ESP32-S3 ADS7822 ADC Reader");

//   // 初始化 CS 引脚
//   pinMode(ADS_CS_PIN, OUTPUT);
//   digitalWrite(ADS_CS_PIN, HIGH); // CS 默认拉高
//   carpet_init();
// //   carpet_select_col(0);
// //   carpet_select_row(0);
//     carpet_select_dir_carpet(VCC);
//     carpet_select_dir_res(GND);

//   // 初始化 SPI
//   // begin(SCK, MISO, MOSI, CS)
//   // 如果使用默认引脚，只需调用 spi.begin()
//   SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, 4); // -1 表示我们手动管理 CS
// }

// void loop() {
//     uint8_t row = 0;
//     uint8_t col = 0;
//     carpet_select_dir_carpet(VCC);
//     carpet_select_dir_res(GND);
//     delay(1);
//     for(row = 0; row < ROW_LENGTH; row++) {
//         carpet_select_row(row);
//         for(col = 0; col < COL_LENGTH; col++) {
//             carpet_select_col(col);
//             adc_list[row][col] = readADS7822();
//         }
//     }
//     carpet_select_dir_carpet(GND);
//     carpet_select_dir_res(VCC);
//     delay(1);
//     for(col = 0; col < COL_LENGTH; col++) {
//         gnd_list[col] = readADS7822();
//    }
//     for(row = 0; row < ROW_LENGTH; row++) {
//         carpet_select_row(row);
//         for(col = 0; col < COL_LENGTH; col++) {
//             R_temp=(float)adc_list[row][col]/(float)gnd_list[col];
//             temp = R_temp*4000-10.0;
//             send_list[row][col]=(uint16_t)(temp);
//             // Serial.print((uint8_t)temp/1);
//             // Serial.print(adc_list[row][col]);

//             // Serial.print('\t');
//         }
//         // Serial.println('\t');
//     }
//     // Serial.println("----------------------------------------------------");

//     delay(1000); // 每秒读取一次
// }