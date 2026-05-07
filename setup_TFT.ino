// #include <Arduino.h>
// #include <Adafruit_GFX.h>    // graphic library
// #include <Adafruit_ST7735.h> // TFT display
// #include <SPI.h>             // serial communication
// #include <ArduinoJson.h>     // JSON paser

// // ── TFT cabling (ESP32) ──
// #define TFT_CS 5  // cs: control screen
// #define TFT_RST 4 // reset
// #define TFT_DC 2  // data command

// // initialize TFT display
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// #define BUTTON1 34 // date / weather
// #define BUTTON2 0  // todo
// #define BUTTON3 35 // ask chat

// // multiple states of ai pet
// enum State
// {
//     BOOT,
//     IDLE,
//     LOADING,
//     RESULT
// };

// State state = BOOT;

// unsigned long stateStartTime = 0;
// unsigned long lastFrameTime = 0;
// bool frame = false; // animation update
// bool render = true; // new display

// unsigned long lastBtnTime = 0;
// const unsigned long debounce = 250;

// char resultLine1[32] = "";
// char resultLine2[32] = "";
// char loadingHint[32] = "Loading...";
// // response string storage

// void setup()
// {
//     Serial.begin(115200);

//     tft.initR(INITR_BLACKTAB);    // init screen
//     tft.setRotation(1);           // landscape
//     tft.fillScreen(ST7735_BLACK); // color
//     tft.setTextSize(2);
//     tft.setTextWrap(false); // disable auto wrap around

//     // set up button
//     pinMode(BUTTON1, INPUT_PULLUP);
//     pinMode(BUTTON2, INPUT_PULLUP);
//     pinMode(BUTTON3, INPUT_PULLUP);

//     stateStartTime = millis();
//     render = true;
// }

// // ── handle response from js main ──
// void handleSerial()
// {
//     if (!Serial.available())
//         return;

//     // if has serial message or not
//     String raw = Serial.readStringUntil('\n');
//     raw.trim();
//     if (raw.length() == 0)
//         return;

//     StaticJsonDocument<256> doc;
//     // deserialize JSON into doc
//     // if fail return
//     if (deserializeJson(doc, raw) != DeserializationError::Ok)
//         return;

//     // only take action key, value
//     const char *action = doc["action"];
//     if (!action)
//         return;

//     // if loading,
//     if (strcmp(action, "loading") == 0)
//     {
//         // copy string to loadingHint
//         strncpy(loadingHint, doc["hint"] | "Loading...", sizeof(loadingHint) - 1);
//         state = LOADING;
//         stateStartTime = millis(); // record loading time
//         render = true;             // to update display
//         return;
//     }

//     // once has response
//     // store oneline, twoline as result
//     strncpy(resultLine1, doc["line1"] | "", sizeof(resultLine1) - 1);
//     strncpy(resultLine2, doc["line2"] | "", sizeof(resultLine2) - 1);
//     state = RESULT;
//     stateStartTime = millis();
//     render = true; // display result
// }

// // ── button event triggers ──
// void handleInput()
// {
//     // debounce to avoid noise
//     unsigned long now = millis();
//     if (now - lastBtnTime < debounce)
//         return;

//     // button 1 -> date weather
//     if (digitalRead(BUTTON1) == LOW)
//     {
//         Serial.println("{\"type\":\"date\"}");
//         lastBtnTime = now;
//     }

//     // button 2 -> todo
//     if (digitalRead(BUTTON2) == LOW)
//     {
//         Serial.println("{\"type\":\"todo\"}");
//         lastBtnTime = now;
//     }

//     // button 3 -> chat
//     if (digitalRead(BUTTON3) == LOW)
//     {
//         Serial.println("{\"type\":\"ask\"}");
//         lastBtnTime = now;
//     }
// }

// // ── update state ──
// void update()
// {
//     unsigned long now = millis();

//     // has turned on for 2000ms, into IDLE mode, render
//     if (state == BOOT && now - stateStartTime > 2000)
//     {
//         state = IDLE;
//         render = true;
//     }

//     // IDLE mode, update frame every 1000ms, render
//     if (state == IDLE && now - lastFrameTime > 1000)
//     {
//         frame = !frame;
//         lastFrameTime = now;
//         render = true;
//     }

//     // once has response, goes back IDEL after 6000ms
//     if (state == RESULT && now - stateStartTime > 6000)
//     {
//         state = IDLE;
//         render = true;
//     }
// }

// // ── display the result/loading hint/frame ──
// // landscape 160x128, setTextSize(2): each char 12x16px, each line 13 char
// void display()
// {
//     tft.fillScreen(ST7735_BLACK); // clear screen
//     switch (state)
//     {
//     case BOOT:
//         // boot up, say hello
//         tft.setCursor(40, 50);
//         tft.setTextColor(ST7735_CYAN);
//         tft.print("Hello!");
//         break;

//     case IDLE:
//         // simple animation
//         tft.setCursor(50, 20);
//         tft.setTextColor(ST7735_WHITE);
//         tft.print(frame ? "oo" : "^^");
//         tft.setTextSize(1);
//         tft.setTextColor(ST7735_YELLOW);
//         tft.setCursor(8, 70);
//         // button description
//         tft.print("1:date  2:todo  3:ask");
//         tft.setTextSize(2);
//         break;

//     case LOADING:
//         // display loading message
//         tft.setCursor(8, 55);
//         tft.setTextColor(ST7735_CYAN);
//         tft.print(loadingHint);
//         break;

//     case RESULT:
//         // display two line of results!
//         tft.setCursor(4, 30);
//         tft.setTextColor(ST7735_WHITE);
//         tft.print(resultLine1);
//         tft.setCursor(4, 60);
//         tft.print(resultLine2);
//         break;
//     }
// }

// void loop()
// {
//     handleSerial();
//     handleInput();
//     update();
//     if (render)
//     {
//         display();
//         render = false;
//     }
// }
