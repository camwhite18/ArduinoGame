#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#include <TimerOne.h>

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
uint8_t i=0;
String M[4] = {"LEFT","RIGHT","UP","DOWN"};
int lengthS = 4;
int sizeM = 2;
String top_scroller = "LEFT - Change no. of characters shown // RIGHT - Change no. of different characters ";
String bot_scroller = "SELECT - Start new game                                                             ";
unsigned int tpos = 0, bpos = 0;
unsigned long now;
boolean pressed = false;
boolean pressedS = false;
boolean pressedM = false;
boolean pressedG = false;
int count = 0;
boolean choice = false;
boolean storyMode = false;
boolean practice = false;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  now = millis();
  randomSeed(analogRead(A0));
}

void menuSystem() {
  //Creates the top menu
  if (!choice) {
    lcd.setCursor(0,0);
    lcd.print("UP: Story Mode");
    lcd.setCursor(0,1);
    lcd.print("DOWN: Practice");
  }
  while (!choice) {
    uint8_t buttons = lcd.readButtons();
    if (buttons & BUTTON_UP) {
      storyMode = true;
      choice = true;
    } else if (buttons & BUTTON_DOWN) {
      practice = true;
      choice = true;
    }
  }
}

void storyMenu() {
  //Creates the story mode menu
  lcd.setCursor(0,0);
  lcd.print("SELECT:  Start");
  lcd.setCursor(0,1);
  //Adds in an arrow pointing left
  lcd.write(B01111111);
  lcd.print(": Leaderboard");
}

void practiceMenu() {
  //Creates the practice mode menu
  lcd.setCursor(0, 0);
  //Makes the menu scroll across
  lcd.print(top_scroller.substring(tpos, tpos + 16));
  lcd.setCursor(0, 1);
  lcd.print(bot_scroller.substring(bpos, bpos + 16));
  if (millis() - now > 250) {
    now = millis();
    if (++tpos > top_scroller.length()) {
      tpos = 0;
    }
    if (++bpos > top_scroller.length()) {
      bpos = 0;
    }
  }
}

void changeLengthS() {
  //Outputs the current no. of characters that will be shown
  lcd.setCursor(0,0);
  lcd.print("No. characters");
  lcd.setCursor(0,1);
  lcd.print("shown: ");
  lcd.print(lengthS);
}

void changeSizeM() {
  //Outptus the current no. of different characters
  lcd.setCursor(0,0);
  lcd.print("No. different");
  lcd.setCursor(0,1);
  lcd.print("characters: ");
  lcd.print(sizeM);
}

//The following functions thumbsUp() and thumbsDown are taken from https://forum.arduino.cc/index.php?topic=54411.0, except the lcd.setCursor lines, which I changed to move the image to a different location
void thumbsUp() {
 byte thumb1[8] = {B00100,B00011,B00100,B00011,B00100,B00011,B00010,B00001};
 byte thumb2[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00011};
 byte thumb3[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00001,B11110};
 byte thumb4[8] = {B00000,B01100,B10010,B10010,B10001,B01000,B11110,B00000};
 byte thumb5[8] = {B00010,B00010,B00010,B00010,B00010,B01110,B10000,B00000};
 byte thumb6[8] = {B00000,B00000,B00000,B00000,B00000,B10000,B01000,B00110};
 lcd.createChar(0, thumb1);
 lcd.createChar(1, thumb2);
 lcd.createChar(2, thumb3);
 lcd.createChar(3, thumb4);
 lcd.createChar(4, thumb5);
 lcd.createChar(5, thumb6);
 lcd.setCursor(1,1);
 lcd.write(0);
 lcd.setCursor(1,0);
 lcd.write(1);
 lcd.setCursor(2,1);
 lcd.write(2);
 lcd.setCursor(2,0);
 lcd.write(3);
 lcd.setCursor(3,1);
 lcd.write(4);
 lcd.setCursor(3,0);
 lcd.write(5);
}

void thumbsDown() {
 byte thumb1[8] = {B00001,B00010,B00011,B00100,B00011,B00100,B00011,B00100};
 byte thumb2[8] = {B00011,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
 byte thumb3[8] = {B11110,B00001,B00000,B00000,B00000,B00000,B00000,B00000};
 byte thumb4[8] = {B00000,B11110,B01000,B10001,B10010,B10010,B01100,B00000};
 byte thumb5[8] = {B00000,B10000,B01110,B00010,B00010,B00010,B00010,B00010};
 byte thumb6[8] = {B00110,B01000,B10000,B00000,B00000,B00000,B00000,B00000};
 lcd.createChar(0, thumb1);
 lcd.createChar(1, thumb2);
 lcd.createChar(2, thumb3);
 lcd.createChar(3, thumb4);
 lcd.createChar(4, thumb5);
 lcd.createChar(5, thumb6);
 lcd.setCursor(1,0);
 lcd.write(0);
 lcd.setCursor(1,1);
 lcd.write(1);
 lcd.setCursor(2,0);
 lcd.write(2);
 lcd.setCursor(2,1);
 lcd.write(3);
 lcd.setCursor(3,0);
 lcd.write(4);
 lcd.setCursor(3,1);
 lcd.write(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  //uint8_t buttons = lcd.readButtons();
  int userPress[lengthS];
  int num[lengthS];
  menuSystem();
  while (practice) {
    //Runs if the user selects practice mode
    uint8_t buttons = lcd.readButtons();
    if (!pressed) {
      practiceMenu();
    }
    if (buttons & BUTTON_LEFT) {
      lcd.clear();
      changeLengthS();
      pressed = true;
      pressedS = true;
    } else if (buttons & BUTTON_RIGHT) {
      lcd.clear();
      changeSizeM();
      pressed = true;
      pressedM = true;
    } else if (buttons & BUTTON_SELECT) {
      lcd.clear();
      pressed = true;
      pressedG = true;
      count = 0;
      int i, n = lengthS;
      for (i=0; i<n; i++) {
        num[i] = (random(0,sizeM));
      }
      for (i=0; i<n; i++) {
        lcd.print(M[num[i]]);
        delay(1000);
        lcd.clear();
        delay(300);
      }
     }
    while (pressedS) {
      uint8_t buttons = lcd.readButtons();
      if (buttons & BUTTON_UP) {
        if (lengthS <10) {
          lengthS += 1;
          //Increases no. of characters shown
          lcd.setCursor(7,1);
          lcd.print("          ");
          lcd.setCursor(7,1);
          lcd.print(lengthS);
        } else {
          //Stops it going over 10
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Max level");
          lcd.setCursor(0,1);
          lcd.print("reached: 10");
          delay(1000);
          lcd.clear();
          changeLengthS();
        }
        delay(200);
      } else if (buttons & BUTTON_DOWN) {
        if (lengthS >4) {
          lengthS -= 1;
          //Decreases no. of characters shown
          lcd.setCursor(7,1);
          lcd.print("          ");
          lcd.setCursor(7,1);
          lcd.print(lengthS);
        } else {
          //Stops it going below 4
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Min level");
          lcd.setCursor(0,1);
          lcd.print("reached: 4");
          delay(1000);
          lcd.clear();
          changeLengthS();
        }
        delay(200);
      } else if (buttons & BUTTON_SELECT) {
        pressedS = false;
        pressed = false;
        delay(200);
        //Confirms choice
      }
    }
    while (pressedM) {
      uint8_t buttons = lcd.readButtons();
      if (buttons & BUTTON_UP) {
        if (sizeM <4) {
          sizeM += 1;
          //Increases number of different characters
          lcd.setCursor(12,1);
          lcd.print("          ");
          lcd.setCursor(12,1);
          lcd.print(sizeM);
        } else {
          //Stops it going over 4
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Max number");
          lcd.setCursor(0,1);
          lcd.print("reached: 4");
          delay(1000);
          lcd.clear();
          changeSizeM();
        }
        delay(200);
      } else if (buttons & BUTTON_DOWN) {
        if (sizeM >2) {
          sizeM -= 1;
          //Decreases number of different characters
          lcd.setCursor(12,1);
          lcd.print("          ");
          lcd.setCursor(12,1);
          lcd.print(sizeM);
        } else {
          //Stops it going below 2
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Min number");
          lcd.setCursor(0,1);
          lcd.print("reached: 2");
          delay(1000);
          lcd.clear();
          changeSizeM();
        }
        delay(200);
      } else if (buttons & BUTTON_SELECT) {
        pressedM = false;
        pressed = false;
        delay(200);
        //Confirms choice
      }
    }
    while (pressedG) {
      uint8_t buttons = lcd.readButtons();
      if (count < lengthS) {
        //Stops once user has pressed the correct amount of times
        if (buttons & BUTTON_LEFT) {
          userPress[count] = 0;
          //Adds their choice to a list
          count += 1;
          delay(200);
          lcd.clear();
          lcd.print("Last Choice:");
          lcd.setCursor(0,1);
          lcd.print("LEFT");
        } else if (buttons & BUTTON_RIGHT) {
          userPress[count] = 1;
          count += 1;
          delay(200);
          lcd.clear();
          lcd.print("Last Choice:");
          lcd.setCursor(0,1);
          lcd.print("RIGHT");
        } else if (buttons & BUTTON_UP) {
          userPress[count] = 2;
          count += 1;
          delay(200);
          lcd.clear();
          lcd.print("Last Choice:");
          lcd.setCursor(0,1);
          lcd.print("UP");
        } else if (buttons & BUTTON_DOWN) {
          userPress[count] = 3;
          count += 1;
          delay(200);
          lcd.clear();
          lcd.print("Last Choice:");
          lcd.setCursor(0,1);
          lcd.print("DOWN");
        }
      } else {
        lcd.clear();
        delay(200);
        boolean correct = true;
        for(i=0;i<lengthS;i++) {
          //Checks to see if each button press is correct
          if (userPress[i] != num[i]) {
            correct = false;
          }
        }
        if (correct) {
          lcd.setBacklight(2);
          thumbsUp();
          //Changes the backlight to green and shows a thumbs up
        } else {
          lcd.setBacklight(1);
          thumbsDown();
          //Changes the backlight to red and shows a thumbs down
        }
        delay(2500);
        lcd.setBacklight(7);
        pressed = false;
        pressedG = false;
      }
    }
  } 
  while (storyMode) {
    //Runs if the user selects story mode
    uint8_t buttons = lcd.readButtons();
    storyMenu();
    int level = 1;
    //Sets the initial level
    boolean correct = true;
    if (buttons & BUTTON_SELECT) {
      lcd.clear();
      while (correct) {
        int num[lengthS];
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Level ");
        lcd.setCursor(6,0);
        lcd.print(level);
        
        delay(3000);
        lcd.clear();
        uint8_t buttons = lcd.readButtons();
        count = 0;
        int i, n = lengthS;
        for (i=0; i<n; i++) {
          num[i] = (random(0,sizeM));
        }
        for (i=0; i<n; i++) {
          lcd.print(M[num[i]]);
          delay(1000);
          lcd.clear();
          delay(300);
        }
        while (count < lengthS) {
          uint8_t buttons = lcd.readButtons();
          if (buttons & BUTTON_LEFT) {
            userPress[count] = 0;
            count += 1;
            delay(200);
            lcd.clear();
            lcd.print("Last Choice:");
            lcd.setCursor(0,1);
            lcd.print("LEFT");
          } else if (buttons & BUTTON_RIGHT) {
            userPress[count] = 1;
            count += 1;
            delay(200);
            lcd.clear();
            lcd.print("Last Choice:");
            lcd.setCursor(0,1);
            lcd.print("RIGHT");
          } else if (buttons & BUTTON_UP) {
            userPress[count] = 2;
            count += 1;
            delay(200);
            lcd.clear();
            lcd.print("Last Choice:");
            lcd.setCursor(0,1);
            lcd.print("UP");
          } else if (buttons & BUTTON_DOWN) {
            userPress[count] = 3;
            count += 1;
            delay(200);
            lcd.clear();
            lcd.print("Last Choice:");
            lcd.setCursor(0,1);
            lcd.print("DOWN");
          }
      }
      lcd.clear();
      delay(200);
      for(i=0;i<lengthS;i++) {
        if (userPress[i] != num[i]) {
          correct = false;
        }
      }
      if (correct) {
        lcd.setBacklight(2);
        thumbsUp();
        level += 1;
        if (lengthS==7 & sizeM<4) {
          lengthS=4;
          sizeM +=1;
          //Changes the difficulty for the next level
        } else if (lengthS==10 & sizeM==4) {
          //Runs if the user reaches level 15 which is the end of the story mode
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Story Mode ");
          lcd.setCursor(0,1);
          lcd.print("Completed");
          lcd.setCursor(0,0);
          correct = false;
          int i;
          for (i=0; i<=512; i++) {
            if (EEPROM.read(i) == 0) {
              EEPROM.write(i, level-1);
              //Writes the score to the EEPROM
              break;
            }
          }
        } else {
          lengthS += 1;
          //Changes the difficulty for the next level
        }
        delay(2500);
      } else {
        //Runs if the user gets the sequence incorrect
        lcd.clear();
        lcd.setBacklight(1);
        lcd.print("Incorrect - try");
        lcd.setCursor(0,1);
        lcd.print("again?");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("UP: Yes");
        lcd.setCursor(0,1);
        lcd.print("DOWN: No");
        boolean userChoice = false;
        //Gives them the option to try again
        while (!userChoice) {
          uint8_t buttons = lcd.readButtons();
          if (buttons & BUTTON_UP) {
            userChoice = true;
            correct = true;
            lcd.clear();
            //Lets them try agin if they press up
          } else if (buttons & BUTTON_DOWN) {
            correct = false;
            userChoice = true;
            lcd.clear();
            //Ends the game if they press down
            for (i=0; i<=512; i++) {
              if (EEPROM.read(i) == 0) {
                if (level > 1) {
                  EEPROM.write(i, level-1);
                  //Writes score to EEPROM
                }
              break;
              }
            }
          }
        }
      }
      
      lcd.setBacklight(7);
    }
  } else if (buttons & BUTTON_LEFT) {
    boolean x = true;
    int a = 1, value;
    //Gets the number of scores in the EEPROM
    while (x) {
      value = EEPROM.read(a);
      a += 1;
      
      if (a == 512 || value == 0) {
        x = false;
      }
    }
    int tempArray[a-1], b, i;
    for (i=0; i<a; i++) {
      value = EEPROM.read(i);
      tempArray[i] = value;
    }
    //Adds all the values in the EEPROM to a list
    for (i = 0; i < a - 1; i++) {
      for (b = 0; b < a - i - 1; b++) {
        if (tempArray[b] < tempArray[b+1]) {
          int swap       = tempArray[b];
          tempArray[b]   = tempArray[b+1];
          tempArray[b+1] = swap;
          //Uses a bubble sort to sort them in order
        }
      }
    }
    for (i=0; i<a-1; i++) {
      Serial.print(i+1);
      Serial.print("\t");
      Serial.print(tempArray[i]);
      Serial.println();
    }
    //Prints the leaderboard to the serial monitor
  }
  }  
}
