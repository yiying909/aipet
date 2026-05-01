#include <Arduino.h>
#include <SSD1306Wire.h>
SSD1306Wire lcd(0x3c, SDA, SCL);

#define BUTTON1 34 // fetchDate, Weather, result1
#define BUTTON2 0  // fetchToDO, result2
#define BUTTON3 35 // voice interaction, result3

// multiple states
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

// time, start time, render time
unsigned long stateStartTime = 0;
unsigned long lastFrameTime = 0;

// display
bool frame = false; //
bool render = true;

// debouncing handling
unsigned long lastBtnTime = 0;
const unsigned long debounceDelay = 250;

void setup()
{
    // have serial connection ready
    Serial.begin(115200);

    // initialize lcd display
    lcd.init();
    lcd.flipScreenVertically();

    // initialize pinMode
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);

    stateStartTime = millis();
    render = true;
}

void blink()
{
    // eyes open
    lcd.clear();
    lcd.drawString(30, 20, "o o");
    lcd.display();
    delay(300);

    // smiley
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

    // BOOT → IDLE
    if (state == BOOT && now - stateStartTime > 2000)
    {
        state = IDLE;
        stateStartTime = now;
        render = true;
    }

    // frame change
    if (state == IDLE && now - lastFrameTime > 1000)
    {
        frame = !frame;
        lastFrameTime = now;
        render = true;
    }

    // LOADING → RESULT, to display result
    if (state == LOADING && now - stateStartTime > 1000)
    {
        state = RESULT1;
        stateStartTime = now;
        render = true;
    }

    // RESULT → IDLE
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