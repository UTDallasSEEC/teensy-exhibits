#include <SD.h>
#include <SPI.h>
#include "Exhibit.h"

void setup() {
  SD.begin(BUILTIN_SDCARD);
  logEntry("SETUP", "Exhibit startup");
  Exhibit::rtc_init();
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) parse_command();
}


void parse_command() {
  int cmd = Serial.read();
  switch (cmd) {
    case 'T': Exhibit::rtc_set(Serial.parseInt()); return;
  }
}
