#include "gheader.h"
#include "gvalue.h"
#include "i2cdisplay.h"

LiquidCrystal_PCF8574 lcd(i2clcdaddr);
// extern sensors_event_t humidity, temp ;

void i2cdisplayinit() {
    #ifdef debug
        Serial.println("Starting I2C display Instant") ;
    #endif
    lcd.begin(16, 2, i2clcd) ;
    lcd.setBacklight(255) ;
    lcd.clear() ;
    #ifdef debug
        Serial.println("Successfull detect I2C display") ;
    #endif
    displayCenteredText("I2C Is Alive", 0) ;
    displayCenteredText("By 66200408", 1) ;
    delay(1000) ;
    lcd.clear() ;
}

void i2cdisplayrun() {
    lcd.setCursor(0,0) ;
    lcd.print(temp.temperature) ;
}

void displayCenteredText(const String& text, uint8_t row) {
    int lcdWidth = 16; // Width of the LCD
    int textLength = text.length();
    #ifdef debugtext
        Serial.print("Input Text: ");
        Serial.print(text);
        Serial.print(" Text Length: ");
        Serial.println(textLength);
    #endif
    lcd.setCursor(0, row);
    for (int i = 0; i < lcdWidth; i++) {
        lcd.print(' ');
    }
    int leftPadding = (lcdWidth - textLength) / 2;
    if (leftPadding < 0) leftPadding = 0; // Prevent negative padding
    String paddedText = "";
    for (int i = 0; i < leftPadding; i++) {
        paddedText += " ";
    }
    paddedText += text;
    paddedText = paddedText.substring(0, lcdWidth);
    #ifdef debugtext
        Serial.print("Padded Text: ");
        Serial.println(paddedText);
    #endif
    lcd.setCursor(0, row);
    lcd.print(paddedText);
}