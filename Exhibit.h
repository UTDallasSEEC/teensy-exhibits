const char* SEEC_LOGFILE = "SEECLOG.CSV";
File logFile;
void logEntry(const char* s, const char *msg = NULL) {
  logFile = SD.open(SEEC_LOGFILE, FILE_WRITE);
  if (!logFile) return;
  logFile.print(Teensy3Clock.get());
  logFile.print(",");
  logFile.print(s);
  logFile.print(",\"");
  if (msg) logFile.print(msg);
  logFile.println("\"");
  logFile.close();
}



#define SEEC_RTC_STATUS (*(uint32_t *)0x4003E018)
class Exhibit {
  public:
  // battery-backed up clock handling on Teensy 3.x
    static const uint32_t SEEC_EPOCH = 1262304000;  // Jan 1, 2010 UTC
    static const uint32_t SEEC_FLAG  = 0x53454543;  // "SEEC"

    static void rtc_init() {
      if (SEEC_RTC_STATUS == SEEC_FLAG) return;  // return if RTC has already been set
      Teensy3Clock.set(SEEC_EPOCH);
      SEEC_RTC_STATUS = SEEC_FLAG;
      logEntry("RTCEPOCH");
    }


    static void rtc_set(unsigned long t) {
      if (t > SEEC_EPOCH) {
        Teensy3Clock.set(t);
        SEEC_RTC_STATUS = SEEC_FLAG;
        logEntry("RTCSET");
      }  
    }
};
