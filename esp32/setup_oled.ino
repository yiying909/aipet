#include <Arduino.h>
#include <SSD1306Wire.h>
SSD1306Wire lcd(0x3c, SDA, SCL);
#include <ArduinoJson.h>

#define BUTTON1 34 // fetchDate, Weather, result1
#define BUTTON2 0  // fetchToDO, result2
#define BUTTON3 35 // voice interaction, result3

// multiple states
enum State
{
    BOOT,
    IDLE,
    LOADING,
    RESULT
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

char resultLine1[32] = "";
char resultLine2[32] = "";
char loadingHint[32] = "Loading...";

void setup()
{
    Serial.begin(115200);
    lcd.init();
    lcd.flipScreenVertically();
    lcd.setFont(ArialMT_Plain_10);

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);

    stateStartTime = millis();
    render = true;
}

void handleSerial()
{
    if (!Serial.available())
        return;
    String raw = Serial.readStringUntil('\n');
    raw.trim();
    if (raw.length() == 0)
        return;

    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, raw);
    if (err)
        return;

    const char *action = doc["action"];
    if (!action)
        return;

    // loading message
    if (strcmp(action, "loading") == 0)
    {
        const char *hint = doc["hint"] | "Loading...";
        strncpy(loadingHint, hint, sizeof(loadingHint) - 1);
        state = LOADING;
        stateStartTime = millis();
        render = true;
        return;
    }

    // result
    const char *l1 = doc["line1"] | "";
    const char *l2 = doc["line2"] | "";
    strncpy(resultLine1, l1, sizeof(resultLine1) - 1);
    strncpy(resultLine2, l2, sizeof(resultLine2) - 1);
    state = RESULT;
    stateStartTime = millis();
    render = true;
}

void handleInput()
{
    unsigned long now = millis();
    if (now - lastBtnTime < debounceDelay)
        return;

    if (digitalRead(BUTTON1) == LOW)
    {
        Serial.println("{\"type\":\"date\"}");
        lastBtnTime = now;
    }
    if (digitalRead(BUTTON2) == LOW)
    {
        Serial.println("{\"type\":\"todo\"}");
        lastBtnTime = now;
    }
    if (digitalRead(BUTTON3) == LOW)
    {
        Serial.println("{\"type\":\"ask\"}");
        lastBtnTime = now;
    }
}

void update()
{
    unsigned long now = millis();

    if (state == BOOT && now - stateStartTime > 2000)
    {
        state = IDLE;
        render = true;
    }
    if (state == IDLE && now - lastFrameTime > 1000)
    {
        frame = !frame;
        lastFrameTime = now;
        render = true;
    }
    // back to idle after 5000ms
    if (state == RESULT && now - stateStartTime > 5000)
    {
        state = IDLE;
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
        lcd.drawString(30, 16, frame ? "o o" : "^ ^");
        lcd.drawString(24, 36, "1:date 2:todo");
        lcd.drawString(24, 50, "3:voice");
        break;

    case LOADING:
        lcd.drawString(10, 20, loadingHint);
        break;

    case RESULT:
        // display result
        lcd.drawString(4, 16, resultLine1);
        lcd.drawString(4, 36, resultLine2);
        break;
    }
    lcd.display();
}

void loop()
{
    handleSerial(); // check result
    handleInput();  // check response
    update();
    if (render)
    {
        display();
        render = false;
    }
}
