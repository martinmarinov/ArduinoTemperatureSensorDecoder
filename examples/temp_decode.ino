#include <TempSensorDecode.h>

#define RF_RECEIVER_PIN 2

void setup() {
  Serial.begin(9600);
  TempSensorDecode::setup(RF_RECEIVER_PIN);
}


void loop() {
  if (TempSensorDecode::hasNewData()) {
    Serial.println("\n=== Reading with TempSensorDecode ===");
    Serial.print("Temp: "); Serial.print(TempSensorDecode::getTemperature()); Serial.println(" C");
    Serial.print("Humidity: "); Serial.print(TempSensorDecode::getHumidity()); Serial.println(" %");
    Serial.print("Channel: "); Serial.println(TempSensorDecode::getChannel());
    Serial.print("Battery: "); Serial.println(TempSensorDecode::getBatteryState());
    Serial.print("Button pressed: "); Serial.println(TempSensorDecode::getIsButtonPressed());
  }
}
