const char* SEEC_LOGFILE = "SEECLOG.CSV";
File logFile;
void logEntry(const char* s, const char *msg = NULL) {
  String logText = String(Teensy3Clock.get()) + "," + String(s) + ",\"";
  if (msg) logText += String(msg);
  logText += "\"";

  logFile = SD.open(SEEC_LOGFILE, FILE_WRITE);
  if (logFile) { 
    logFile.println(logText);
    logFile.close(); 
  }
  Serial.println(logText);
  Serial.flush();
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


class EdgeLogger {
  public:
    unsigned long startLock = 0;
    int lastTF = 0;

    int isLocked(unsigned long lockout) { return (millis() - startLock < lockout); }

    int logRise(int tf, unsigned long lockout, 
                 const char* s, const char* msg = NULL) {
      if (isLocked(lockout)) return 0;
      int rise = (tf && !lastTF);
      if (rise) {
         logEntry(s, msg);
         startLock = millis();
      }
      lastTF = tf;
      return rise;
    }

    int logFall(int tf, unsigned long lockout, 
                 const char* s, const char* msg = NULL) {
      if (isLocked(lockout)) return 0;
      int fall = (!tf && lastTF);
      if (fall) {
         logEntry(s, msg);
         startLock = millis();
      }
      lastTF = tf;
      return fall;
    }

};
