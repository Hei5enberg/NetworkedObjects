#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

const int switchPos1 = 2;
const int switchPos2 = 4;
const int switchPos3 = 7;
const int ledWhite = 6;
const int ledOrange = 5;

//HARDCODE THESE, USE VERSION 3.0 FOR AUTOMATIC SETUP
int triggerValue1 = 30;
int triggerValue2 = 140;
int triggerValue3 = 150;

void setup() {
  pinMode(switchPos1, INPUT);
  pinMode(switchPos2, INPUT);
  pinMode(switchPos3, INPUT);
  pinMode(ledWhite, OUTPUT);
  pinMode(ledOrange, OUTPUT);

  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("/------------------\\");
  lcd.setCursor(0, 1);
  lcd.print("|  Combined V3.1   |");
  lcd.setCursor(0, 2);
  lcd.print("|  By Gijs Vis     |");
  lcd.setCursor(0, 3);
  lcd.print("\\------------------/");
}

void loop() {
  //LDR readout
  int ldr = analogRead(A0);
  Serial.print("  - LDR(unmapped): ");
  Serial.print(ldr);
  
  int ldrMapped = map(ldr, 0, 1023, 0, 255);
  Serial.print("  - LDR(mapped): ");
  Serial.println(ldrMapped);

  //int triggerValue1 = 5; 
  //int triggerValue2 = 115;
  //int triggerValue3 = 120;

  //Switches on the back
  int switchOn = digitalRead(switchPos1);
  int switchTest = digitalRead(switchPos2);
  int switchDemo = digitalRead(switchPos3);
  //turns off all components
  if(switchTest){
    Serial.println("========== IN SETUP MODE [unavailable] ===");
    lcd.setCursor(0, 0);
    lcd.print("/------------------\\");
    lcd.setCursor(0, 1);
    lcd.print("| Disabled in this |");
    lcd.setCursor(0, 2);
    lcd.print("| Version. Use V3.0|");
    lcd.setCursor(0, 3);
    lcd.print("\\------------------/");
  }
  else if(switchOn){
    //execute the main code
    Serial.println("========== EXECUTING MAIN LOOP ==========");
    mainLoop(ldr, ldrMapped);
  }
  //testing mode to check if everything works
  else if(switchDemo){
    digitalWrite(ledWhite, HIGH);
    digitalWrite(ledOrange, HIGH);
    
    lcd.setCursor(0, 0);
    lcd.print("/------------------\\");
    lcd.setCursor(0, 1);
    lcd.print("|  LDR:            |");
    lcd.setCursor(0, 2);
    lcd.print("|                  |");
    lcd.setCursor(0, 3);
    lcd.print("\\------------------/");

    lcd.setCursor(7, 1);
    lcd.print(ldr);
    delay(500);
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("|  Error: switch   |");
    Serial.println("  ERROR: NO SIGNAL FROM SWITCH");
  }
  delay(1000);
}

void mainLoop(int ldr, int ldrMapped) {
  if(ldr < triggerValue1){
    Serial.println(" ");
    Serial.println("======== Starting Case 3 - PANIC ========");
    
    int lineCount = 0;
    int rowCount = 0;
    int arraySize = 5;
    char *panicArray[]  = {" HELP", "STOP ", " DARK", " REDRUM "};
    lcd.setCursor(0,0);
    while(ldr < triggerValue1){
      if(lineCount > 19){
        rowCount++;
        if(rowCount > 3){
          rowCount = 0; lcd.setCursor(lineCount, 0);
        }
        lineCount = 0; lcd.setCursor(0, rowCount);
      } else { lineCount++; }
      
      int spaceInsert = random(0, 5);
      int ASCIIint = random(32, 127);
      char ASCIIchar = ASCIIint;
      if(spaceInsert == 4){ lcd.print(" ");
      } else { lcd.print(ASCIIchar); }

      int wordInsert = random(0, 30);
      int whichWord = random(0, arraySize);
      if(wordInsert == 15){ lcd.print(panicArray[whichWord]); }

      int ledValue1 = random(0, 2);
      int ledValue2 = random(0, 2);
      digitalWrite(ledWhite, ledValue1);
      digitalWrite(ledOrange, ledValue2);
      delay(100);

      if(digitalRead(switchPos1) == LOW){ Serial.println("  - broke out of while loop"); break; }
      if(analogRead(A0) > triggerValue1){ Serial.println("  - broke out of while loop, cause: ldr change"); break; }
    }
    
    Serial.println("======== Finished Case 3 ================");
    Serial.println(" ");
    
  }

  else if(ldr < triggerValue2){
    Serial.println(" ");
    Serial.println("======== Starting Case 2 - STRESS ======");

    int rowCount = 0;
    while(ldr < triggerValue2){
      for(int i = 0; i <= 255; i++){ analogWrite(ledOrange, i); delay(7); }

      digitalWrite(ledWhite, HIGH);
      delay(500);
      digitalWrite(ledWhite, LOW);

      if(rowCount > 3){ rowCount = 0; lcd.setCursor(0,0);
      } else { lcd.setCursor(0, rowCount); rowCount++; }

      int alternate = 0;
      for(int i = 0; i < 20; i++){
        int character = random(0, 3);
        if(alternate == 0){
          if(character == 0){ lcd.print(">"); delay(20);
          } else if(character == 1){ lcd.print("<"); delay(20); 
          } else { lcd.print(" "); delay(20); }
          alternate = 1;
        }else {
          if(character == 0){ lcd.print("["); delay(20);
          } else if(character == 1){ lcd.print("]"); delay(20); 
          } else { lcd.print(" "); delay(20); }
          alternate = 0;
        }
      }
      for(int i = 255; i >= 0; i--){ analogWrite(ledOrange, i); delay(2); }

      if(digitalRead(switchPos1) == LOW){ Serial.println("  - broke out of while loop, cause; switch change"); break; }
      if(analogRead(A0) > triggerValue2 || analogRead(A0) < triggerValue1){ Serial.println("  - broke out of while loop, cause: ldr change"); break; } 
    }

    Serial.println("======== Finished Case 2 ================");
    Serial.println(" ");
  }

  else if(ldr > triggerValue3){
    Serial.println(" ");
    Serial.println("======== Starting Case 1 - CALM =========");
    if(digitalRead(switchPos1) == LOW){ Serial.println("  - broke out of while loop, cause; switch change"); break; }
    if(analogRead(A0) < triggerValue3){ Serial.println("  - broke out of while loop, cause: ldr change"); break; }

    analogWrite(ledWhite, 200);
    
    bool alternate = LOW;
    lcd.setCursor(0, 0);
    int lineCount = 0;
    int rowCount = 0;
    while(ldr > triggerValue3){
      for(int i = 0; i <= 255; i++){ analogWrite(ledOrange, i); delay(15); }

      if(rowCount > 3){ rowCount = 0; lcd.setCursor(0,0);
      } else { lcd.setCursor(0, rowCount); rowCount++; }
      
      for(int i = 0; i < 10; i++){
        if(alternate){
          lcd.print(">"); delay(100);
          lcd.print(" "); delay(100);
        }
        else {
          lcd.print(" "); delay(100);
          lcd.print(">"); delay(100);
        }
      }

      if(alternate){ alternate = LOW;} else{ alternate = HIGH; }
      
      for(int i = 255; i >= 0; i--){ analogWrite(ledOrange, i); delay(9); }
      
      if(digitalRead(switchPos1) == LOW){ Serial.println("  - broke out of while loop, cause; switch change"); break; }
      if(analogRead(A0) < triggerValue3){ Serial.println("  - broke out of while loop, cause: ldr change"); break; }
      delay(1500);
    }
  
    Serial.println("======== Finished Case 1 ================");
    Serial.println(" ");
  }
  else{ 
    Serial.println("  ERROR: LDR RANGE NO SETUP RIGHT");
    lcd.setCursor(0, 1);
    lcd.print("| Error: ldr range |");
    lcd.setCursor(0, 2);
    lcd.print("|                  |");
  }
}
