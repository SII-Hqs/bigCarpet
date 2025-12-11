// #include <bigcarpet.h>

// void setup() {
//   Serial.begin(115200);
//   while (!Serial) { delay(10); }
//   Serial.println("ESP32-S3 ADS7822 ADC Reader");

//   // 初始化 CS 引脚
//   pinMode(ADS_CS_PIN, OUTPUT);
//   digitalWrite(ADS_CS_PIN, HIGH); // CS 默认拉高
//   carpet_init();
//   carpet_select_col(0);
//   carpet_select_row(0);

//   // 初始化 SPI
//   // begin(SCK, MISO, MOSI, CS)
//   // 如果使用默认引脚，只需调用 spi.begin()
//   SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, 4); // -1 表示我们手动管理 CS
// }

// void loop() {
//   // 读取原始 ADC 值
//   uint16_t adc_raw = readADS7822();

//   // 将12位原始值转换为电压
//   // 满量程码 (FFFh) 对应 VREF - 1 LSB [cite: 766]
//   // 0-4095 对应 0V 到 V_REF
//   float voltage = (adc_raw / 4095.0) * V_REF_VOLTAGE;

//   // 打印结果
//   Serial.print("Raw ADC Value: ");
//   Serial.print(adc_raw);
//   Serial.print("\t (0-4095)");

//   Serial.print("\t | \tVoltage: ");
//   Serial.print(voltage, 3); // 打印3位小数
//   Serial.println(" V");

//   delay(1); // 每秒读取一次
// }