//Calculator Code - Ariel Rosen
// May 11, 2026
// Introduction to IoT
// Presented to: Daphne Saju, on May 14, 2026


#include <LiquidCrystal.h>

int Contrast=75; //make an integer to have a contrast for the contrast pin (I had no potentiometer and found a video on line for how to wire the LCD without one)
const int rs=8, en=9, d4=10, d5=11, d6=12, d7=13;  // lcd pinout
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //lcd pins declaration

long Number, Num1, Num2; //define storage system used for the variables in the equation
char action; //define storage system used for the action in the equation
boolean result=false; //define storage system for whether or not the equation is ready to be equated
boolean error=false; //define a storage system for whether or not a division by 0 error is flagged

String input=""; //create a string for the equation, will later on be split into distinct parts

void setup(){

  Serial.begin(9600); //start the 9600 baud serial monitor

  analogWrite(6, Contrast);
  lcd.begin(16, 2); //declare size of lcd
  delay(1000);
  Serial.println();
  Serial.print("Arduino Calc - IoT PBL");
  lcd.print("Arduino Calc");
  lcd.setCursor(0, 1);
  lcd.print("IoT PBL");
  delay(2000);
  lcd.clear();

  lcd.print("Enter equation:");
  Serial.println();
  Serial.println("Enter equation:");
}

void loop(){
  while (Serial.available()>0){ //check if there is anything in the serial input
    char key=Serial.read(); //keep reading the serial monitor input while there are inputs being inputted

    if(key=='\n'){ //if enter is pressed, and a line is returned
      result=true; //make the boolean state be true, thus continuing with other functions
    } 
    else{
      input=input+key; //add the inputted key to the previously made empty string, thus now displaying the key as the string
    }
  }

  if(result==true){ //when enter is pressed, thus making the boolean result true
    detectInput(); //funciton to be created later
    calculateResult(); //function to be created later
    displayResult(); //function to be created later

    delay(500);

    input=""; //once all is displayed, reset the input string to blank
    result=false; //once all is displayed, reset the boolean state to false (thus now waiting for an 'ENTER')

    Serial.println("Enter next equation:"); //ready to start again
  }
}

void detectInput(){
  Number=0; //set the final number to 0, to make the code and the storage repeatable
  Num1=0; //same for number 1
  Num2=0; //and number 2
  action=' '; //make the character being stored for 'action' a blank character, to now store a new action and make the code and storage repeatable

  // find operator and split numbers
  for(int i=0; i<input.length(); i++){ //creates the integer 'i' to scan the input and break it into its different parts

    if(input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/'){ //use an OR comparator to see if any of the integers inside of the string are a mathematical operator

      action=input[i]; //make the char storing the mathematical operator  to whichever operator 'i' found

      Num1=input.substring(0, i).toInt(); //make everything before the operator into a stored long value of Num1
      Num2=input.substring(i + 1).toInt(); //make everything after the operator into a stored long value of Num2

      Serial.print("Equation: "); //serial monitor display of the equation, before the next function solves it
      Serial.print(Num1);
      Serial.print(action);
      Serial.println(Num2);
    }
  }
}

void calculateResult(){
  if(action=='+'){ //logic for when the operator is '+'
    Number=Num1+Num2;
  }

  else if(action=='-'){ //logic for when the operator is'-'
    Number=Num1-Num2;
  }

  else if(action=='*'){ //logic for when the operator is'*'
    Number=Num1*Num2;
  }

  else if(action=='/'){ //logic for when the operator is '/'
    if(Num2==0){ //if you try dividing by 0

      error=true; //make the boolean state for error be true
      result=false; //return nothing
      return; //stop calculation, to avoid a system error in dividing by zero
    }

    Number=Num1/Num2;
  }
}


void displayResult(){

  lcd.clear(); //clear the lcd, to make the code repeatable

  lcd.setCursor(0, 0); //set cursor to the beginning of the lcd, and display the equation
  lcd.print(Num1);
  lcd.print(action);
  lcd.print(Num2);

  if(error==true){ //if a division by 0 is detected
    
    lcd.clear(); //remove anything else, since the error overrrides
      lcd.setCursor(0, 0);
      lcd.print("ERROR: Division");
      lcd.setCursor(0, 1);
      lcd.print("By The Number 0"); //display the error message on the lcd

      Serial.println("ERROR: Division By The Number 0"); //display the error message on the serial monitor
  }

  else{
    
     if(result==true){ //if the result boolean is true (triggered by pressing enter)
      lcd.print("="); //lcd print the answer to the equation

    lcd.setCursor(0, 1); //move down a line to display the answer
    lcd.print(Number); //display the answer

    Serial.print("Result: "); //serial monitor print the answer to the equation
    Serial.println(Number);
    delay(5000);
    lcd.clear();
    lcd.print("Enter Equation:");
    }
  }
}