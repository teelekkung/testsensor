// src/aht10.cpp
#include "global.h"
#include "i2cdisplay.h"

void i2cdisplayinit() {
    // setup i2c lcd on wire bus i2clcd
    Serial.println("Init I2CLCD") ;
    lcd.begin(16, 2, i2clcd) ;
    lcd.setBacklight(255) ;
    lcd.clear() ;
    displayCenteredText("Done initialized", 0) ;
    displayCenteredText("Done", 1) ;
    delay(1000) ;
}

void i2cdisplayrun() {
    
}

void displayCenteredText(const String& text, uint8_t row) {
  int lcdWidth = 16; // Width of the LCD
  int textLength = text.length();

#ifdef DEBUG
  // Debug: Print the input text and its length
  Serial.print("Input Text: ");
  Serial.println(text);
  Serial.print("Text Length: ");
  Serial.println(textLength);
#endif

  // Clear the row explicitly
  lcd.setCursor(0, row);
  for (int i = 0; i < lcdWidth; i++) {
    lcd.print(' '); // Write a space to each position
  }

  // Calculate left padding
  int leftPadding = (lcdWidth - textLength) / 2;
  if (leftPadding < 0) leftPadding = 0; // Prevent negative padding

  // Create the padded string manually
  String paddedText = "";
  for (int i = 0; i < leftPadding; i++) {
    paddedText += " ";
  }
  paddedText += text;

  // Ensure the string is not longer than lcdWidth
  paddedText = paddedText.substring(0, lcdWidth);

#ifdef DEBUG
  // Debug: Print the padded text
  Serial.print("Padded Text: ");
  Serial.println(paddedText);
#endif

  // Display the padded text on the LCD
  lcd.setCursor(0, row);
  lcd.print(paddedText);
}