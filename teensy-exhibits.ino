#include <SD.h>
#include <SPI.h>
#include "Exhibit.h"
#include "Ionosphere.h"

void setup() {
  Serial.begin(115200);
  SD.begin(BUILTIN_SDCARD);
  delay(100);  
  logEntry("SETUP", "Exhibit startup");
  Exhibit::rtc_init();
  Ionosphere::setup();
}

void loop() {
  if (Serial.available()) parse_command();
  Ionosphere::loop();
}


void parse_command() {
  int cmd = Serial.read();
  switch (cmd) {
    case 'T': Exhibit::rtc_set(Serial.parseInt()); return;
  }
}
