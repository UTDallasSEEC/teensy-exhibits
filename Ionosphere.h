
class Ionosphere {
  public:
  static const int TIMER_ACTIVE = 180;
  static const int SIGLVL = 45;
  
  static const int P_EXBGND = 0;
  static const int P_EXBPWR = 1;
  static const int P_SW0SIG = 2;
  static const int P_SW0GND = 3;
  static const int P_LED = 13;
  static const int P_TWR0GND = 22;
  static const int P_TWR0LED = 21;
  static const int P_TWR0SIG = A6;
  static const int P_TWR1GND = 19;
  static const int P_TWR1LED = 18;
  static const int P_TWR1SIG = A3;
  static const int P_TWR2GND = 16;
  static const int P_TWR2LED = 15;
  static const int P_TWR2SIG = A0;

  static unsigned long time_off;

  static EdgeLogger sw0Rise;
  static EdgeLogger activeRise;
  static EdgeLogger activeFall;
  static EdgeLogger led0Rise;
  static EdgeLogger led1Rise;
  static EdgeLogger led2Rise;

  static void setup();
  static void loop();
};

unsigned long Ionosphere::time_off = 0;
EdgeLogger Ionosphere::sw0Rise;
EdgeLogger Ionosphere::activeRise;
EdgeLogger Ionosphere::activeFall;
EdgeLogger Ionosphere::led0Rise;
EdgeLogger Ionosphere::led1Rise;
EdgeLogger Ionosphere::led2Rise;

void Ionosphere::setup() {
  logEntry("IONSETUP", "Ionosphere 2019-07-11");
  pinMode(P_SW0GND, OUTPUT);
  pinMode(P_SW0SIG, INPUT_PULLUP);
  pinMode(P_EXBGND, OUTPUT);
  pinMode(P_EXBPWR, OUTPUT);
  pinMode(P_LED, OUTPUT);
  pinMode(P_TWR0GND, OUTPUT);
  pinMode(P_TWR0LED, OUTPUT);
  pinMode(P_TWR0SIG, INPUT_PULLUP);
  pinMode(P_TWR1GND, OUTPUT);
  pinMode(P_TWR1LED, OUTPUT);
  pinMode(P_TWR1SIG, INPUT_PULLUP);
  pinMode(P_TWR2GND, OUTPUT);
  pinMode(P_TWR2LED, OUTPUT);
  pinMode(P_TWR2SIG, INPUT_PULLUP);
}


void Ionosphere::loop() {
  unsigned long nowsec = Teensy3Clock.get();
  digitalWrite(P_SW0GND, LOW);
  int sw0 = (digitalRead(P_SW0SIG) == LOW);
  if (sw0Rise.logRise(sw0, 3000, "SW0RISE")) {
    time_off = nowsec + TIMER_ACTIVE;
  }
   
  int active = (nowsec < time_off);
  activeRise.logRise(active, 0, "EXBRISE");
  activeFall.logFall(active, 0, "EXBFALL");

  int led0 = LOW;
  int led1 = LOW;
  int led2 = LOW;
  int exhibit = LOW;

  if (active) {
    exhibit = HIGH;
    int res0 = analogRead(P_TWR0SIG);
    int res1 = analogRead(P_TWR1SIG);
    int res2 = analogRead(P_TWR2SIG);
    if (res0 < SIGLVL) led0 = HIGH;
    if (res1 < SIGLVL) led1 = HIGH;
    if (res2 < SIGLVL) led2 = HIGH;
  }

  digitalWrite(P_EXBGND, LOW);
  digitalWrite(P_EXBPWR, exhibit);
  digitalWrite(P_LED, exhibit);

  digitalWrite(P_TWR0GND, LOW);
  digitalWrite(P_TWR0LED, led0);
  digitalWrite(P_TWR1GND, LOW);
  digitalWrite(P_TWR1LED, led1);
  digitalWrite(P_TWR2GND, LOW);
  digitalWrite(P_TWR2LED, led2);
  led0Rise.logRise(led0, 3000, "LED0RISE");
  led1Rise.logRise(led1, 3000, "LED1RISE");
  led2Rise.logRise(led2, 3000, "LED2RISE");
}
