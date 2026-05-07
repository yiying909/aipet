# 1 "C:\\Users\\kmana\\AppData\\Local\\Temp\\tmpyk8knvml"
#include <Arduino.h>
# 1 "C:/Users/kmana/Downloads/CS 256/aipet/setup.ino"
#include <Arduino.h>
#include <SSD1306Wire.h>
SSD1306Wire lcd(0x3c, SDA, SCL);

#define BUTTON1 34
#define BUTTON2 0
#define BUTTON3 35


enum State
{
    BOOT,
    IDLE,
    LOADING,
    RESULT1,
    RESULT2,
    RESULT3
};

State state = BOOT;


unsigned long stateStartTime = 0;
unsigned long lastFrameTime = 0;


bool frame = false;
bool render = true;


unsigned long lastBtnTime = 0;
const unsigned long debounceDelay = 250;
void setup();
void blink();
void handleInput();
void update();
void display();
void loop();
#line 34 "C:/Users/kmana/Downloads/CS 256/aipet/setup.ino"
void setup()
{

    Serial.begin(115200);


    lcd.init();
    lcd.flipScreenVertically();


    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);

    stateStartTime = millis();
    render = true;
}

void blink()
{

    lcd.clear();
    lcd.drawString(30, 20, "o o");
    lcd.display();
    delay(300);


    lcd.clear();
    lcd.drawString(30, 20, "^ ^");
    lcd.display();
    delay(150);
}

void handleInput()
{
    unsigned long now = millis();

    if (now - lastBtnTime < debounceDelay)
        return;

    if (digitalRead(BUTTON1) == LOW)
    {
        state = LOADING;
        stateStartTime = now;
        lastBtnTime = now;
        render = true;
    }

    if (digitalRead(BUTTON2) == LOW)
    {
        state = LOADING;
        stateStartTime = now;
        lastBtnTime = now;
        render = true;
    }
}
void update()
{
    unsigned long now = millis();


    if (state == BOOT && now - stateStartTime > 2000)
    {
        state = IDLE;
        stateStartTime = now;
        render = true;
    }


    if (state == IDLE && now - lastFrameTime > 1000)
    {
        frame = !frame;
        lastFrameTime = now;
        render = true;
    }


    if (state == LOADING && now - stateStartTime > 1000)
    {
        state = RESULT1;
        stateStartTime = now;
        render = true;
    }


    if ((state == RESULT1 || state == RESULT2) &&
        now - stateStartTime > 2000)
    {
        state = IDLE;
        stateStartTime = now;
        render = true;
    }
}

void display()
{
    lcd.clear();

    switch (state)
    {

    case BOOT:
        lcd.drawString(20, 20, "Hello");
        break;

    case IDLE:
        if (frame)
            lcd.drawString(30, 20, "o o");
        else
            lcd.drawString(30, 20, "^ ^");
        break;

    case LOADING:
        lcd.drawString(20, 20, "Loading...");
        break;

    case RESULT1:
        lcd.drawString(10, 20, "Result A");
        break;

    case RESULT2:
        lcd.drawString(10, 20, "Result B");
        break;
    }

    lcd.display();
}

void loop()
{
    handleInput();
    update();
    if (render)
    {
        display();
        render = false;
    }
}