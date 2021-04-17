
#include <Keypad.h>


#include <Arduino.h>

#include <LiquidCrystal.h>

#define RS 6
#define E 7
#define L4 45
#define L5 44
#define L6 43
#define L7 42
#define ROWS 4
#define COLUMNS 4
#define BUZZER 49
#define BEEP_SHORT_DELAY 50
#define BEEP_LONG_DELAY 500
#define MEMORY_REGISTER_LENGTH 60

LiquidCrystal lcd(RS, E, L4, L5, L6, L7);

char hexaKeys[ROWS][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25};
byte columnPins[COLUMNS] = {26, 27, 28, 29};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, columnPins, ROWS, COLUMNS);

bool isBeep = false;
bool stop = false;
struct Time
{
  uint8_t hour, minute, second;
};
Time time = {0, 0, 0};

Time memoryRegister[MEMORY_REGISTER_LENGTH];

uint8_t memoryRegisterSize = 0;

ISR(TIMER4_COMPA_vect)
{
  if (!stop)
  {
    if (++time.second == 60)
    {
      time.second = 0;
      if (++time.minute == 60)
      {
        time.minute = 0;
        if (++time.hour == 24)
          time.hour = 0;
      }
    }

    if (time.second == 0)
    {
      isBeep = true;
    }
  }
}

inline void showTime(const Time &time);
inline void onPressA();
inline void onPressB(uint8_t &currentSavedTimeIndex);
inline void onPressC();
inline void onPressD();
bool apply();
inline void shortBeep();
inline void doubleShortBeep();
inline void longBeep();
inline bool checkIndex(const uint8_t &index);
inline void resetMemoryRegister();
inline void saveToMemoryRegister();

void setup()
{
  noInterrupts();

  pinMode(BUZZER, OUTPUT);
  lcd.begin(16, 2);

  TCCR4A = WGM40;
  TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40);
  TIMSK4 = (1 << OCIE4A);
  OCR4A = 0x3D08;

  interrupts();
}

void loop()
{
  char selectedKey = keypad.getKey();
  delay(100);
  switch (selectedKey)
  {
    case 'A':
      shortBeep();
      onPressA();
      break;

    case 'C':
      shortBeep();
      onPressC();
      break;

    case 'D':
      shortBeep();
      onPressD();
      break;

    case '*':
      shortBeep();
      resetMemoryRegister();
      break;

    case '#':
      shortBeep();
      saveToMemoryRegister();
      break;

    default:
      showTime(time);
      delay(100);
      break;
  }

  if (isBeep)
  {
    longBeep();
    isBeep = false;
  }
}

inline void resetMemoryRegister()
{
  memoryRegisterSize = 0;
  doubleShortBeep();
}

inline void saveToMemoryRegister()
{
  if ((memoryRegisterSize + 1) <= MEMORY_REGISTER_LENGTH)
  {
    memoryRegister[++memoryRegisterSize - 1] = time;
    doubleShortBeep();
  }
  else
  {
    longBeep();
  }
}

inline void onPressA()
{
  lcd.clear();
  if (memoryRegisterSize > 0)
  {
    uint8_t input[2];
    uint8_t inputSize = 0;
    uint8_t currentSavedTimeIndex = 0;
    lcd.setCursor(0, 0);
    lcd.print("0");
    lcd.print(currentSavedTimeIndex + 1);
    lcd.setCursor(3, 0);
    showTime(memoryRegister[currentSavedTimeIndex]);
    lcd.setCursor(13, 0);
    lcd.print("M");
    if (memoryRegisterSize < 10)
    {
      lcd.print("0");
    }
    lcd.print(memoryRegisterSize);
    lcd.setCursor(5, 1);
    lcd.print("<-A Y-# B->");
    lcd.setCursor(0, 1);

    char currentKey;
    while (true)
    {
      if (currentKey = keypad.getKey())
      {
        if (currentKey == 'A')
        {
          shortBeep();
          lcd.clear();
          break;
        }
        else if (currentKey == 'B')
        {
          shortBeep();
          onPressB(currentSavedTimeIndex);
        }
        else if (currentKey == '#')
        {
          shortBeep();
          if (inputSize == 0)
          {
            lcd.setCursor(0, 1);
            lcd.print("BAN");
            longBeep();
            lcd.setCursor(0, 1);
            lcd.print("   ");
          }
          else if (checkIndex(inputSize == 2 ? (input[0] * 10 + input[1] - 1) : input[0] - 1))
          {
            currentSavedTimeIndex = inputSize == 2 ? (input[0] * 10 + input[1] - 1) : input[0] - 1;
            lcd.setCursor(0, 0);
            if (currentSavedTimeIndex < 10)
            {
              lcd.print("0");
            }
            lcd.print(currentSavedTimeIndex + 1);
            lcd.setCursor(3, 0);
            showTime(memoryRegister[currentSavedTimeIndex]);
            inputSize = 0;

            lcd.setCursor(0, 1);
            lcd.print("DONE");
            doubleShortBeep();
            delay(200);
            lcd.setCursor(0, 1);
            lcd.print("    ");
          }
          else
          {
            lcd.setCursor(0, 1);
            lcd.print("BAN");
            longBeep();
            lcd.setCursor(0, 1);
            lcd.print("   ");
            inputSize = 0;
          }
        }
        else if (int(currentKey) <= int('9') && int(currentKey) >= int('0'))
        {
          shortBeep();
          if (inputSize < 2)
          {
            lcd.setCursor(inputSize, 1);
            lcd.print(currentKey);
            inputSize++;
            input[inputSize - 1] = currentKey - '0';
          }
          else
          {
            lcd.setCursor(0, 1);
            lcd.print("BAN");
            longBeep();
            lcd.setCursor(0, 1);
            lcd.print("   ");
            inputSize = 0;
          }
        }
      }
    }
  }
  else
  {
    lcd.print("Register's empty");

    lcd.setCursor(10, 1);
    lcd.print("Back-*");

    while (true)
    {
      if (keypad.getKey() == '*')
      {
        break;
      }
    }
    lcd.clear();
  }
}

inline void onPressB(uint8_t &currentSavedTimeIndex)
{
  if (checkIndex(currentSavedTimeIndex + 1))
  {
    currentSavedTimeIndex++;
  }
  lcd.setCursor(0, 0);
  if (currentSavedTimeIndex < 10)
  {
    lcd.print("0");
  }
  lcd.print(currentSavedTimeIndex + 1);
  showTime(memoryRegister[currentSavedTimeIndex]);
}

inline void onPressC()
{
  time = {0, 0, 0};
  stop = true;
}

inline void onPressD()
{
  stop = !stop;
}

inline bool checkIndex(const uint8_t &index)
{
  return index >= 0 && index < memoryRegisterSize;
}

bool apply()
{
  lcd.setCursor(11, 1);
  lcd.print("Yes-#");

  while (true)
  {
    if (keypad.getKey() == '#')
    {
      return true;
    }
    delay(50);
  }
}

inline void shortBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void doubleShortBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
  delay(BEEP_SHORT_DELAY / 2);
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void longBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_LONG_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void showTime(const Time &time)
{
  lcd.setCursor(3, 0);
  if (time.hour < 10)
    lcd.print("0");
  lcd.print(time.hour);
  lcd.print(":");
  if (time.minute < 10)
    lcd.print("0");
  lcd.print(time.minute);
  lcd.print(":");
  if (time.second < 10)
    lcd.print("0");
  lcd.print(time.second);
}
