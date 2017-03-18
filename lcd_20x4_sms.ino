// vi:ts=4
// ----------------------------------------------------------------------------
// SmsBox on LCD 20x4
// Created by Olek Beluga 2017-03-15
// olek.beluga@gmail.com
//
// Reads strings over Bluetooth connection. The format is <sms><sender>SENDER</sender><msg>MESSAGE</msg></sms>
// SENDER and MESSAGE get parsed out and shown on display: message takes up first three lines, sender is shown on fourth.
// That's it.
//
// TODO:
// 1. Add some AT+ commands to initialize BT with a custom name.
// 2. Support displays of different sizes.
// 3. Remove data redunancy between parse_bt(String data) -- func arg data and global variable bt_data
//
// ----------------------------------------------------------------------------
//

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

// LCD geometry
const int LCD_ROWS = 4;
const int LCD_COLS = 20;

// bluetooth stuff
char junk;
String inputString="";

void setup()
{
   Serial.begin(9600);            // set the baud rate to 9600, same should be of your Serial Monitor
 pinMode(13, OUTPUT);
 
	// initialize LCD with number of columns and rows: 
	if( lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible

//		fatalError(1); // this never returns
	}
//	lcd.backlight()();
	// Print a message to the LCD
	lcd.print("Hello, World!");

 lcd.setCursor(0, 2);
 lcd.print("Test of 20x4");
 lcd.setCursor(0, 3);
 lcd.print(" - by Olek Beluga");
}

// bluetooth
String bt_data = "";

void parse_bt(String data)
{
   // parsing of bluetooth commands
    String open_tag = "<sms>";
    String close_tag = "</sms>";
    int open_tag_pos = bt_data.indexOf(open_tag);
    int close_tag_pos = bt_data.indexOf(close_tag);
    
    if(open_tag_pos != -1 && close_tag_pos != -1)
    {
      parse_bt(bt_data);
      // print message
   Serial.println(data);
      int sms_size = close_tag_pos - open_tag_pos;  
      String sms = data.substring(open_tag_pos + open_tag.length(), sms_size);
Serial.println(sms);
      String open_sender_tag = "<sender>";
      String close_sender_tag = "</sender>";
      String open_msg_tag = "<msg>";
      String close_msg_tag = "</msg>";
      int open_sender_pos = sms.indexOf(open_sender_tag);
      int close_sender_pos = sms.indexOf(close_sender_tag);
      int open_msg_pos = data.indexOf(open_msg_tag);
      int close_msg_pos = data.indexOf(close_msg_tag);
      Serial.println(open_msg_pos);
      Serial.println(close_msg_pos);
      int sender_size = close_sender_pos - open_sender_pos;
      Serial.println(sms);

 int msg_size = close_msg_pos - open_msg_pos;
      String msg = data.substring(open_msg_pos + open_msg_tag.length(), close_msg_pos);
      String sender = sms.substring(open_sender_pos + open_sender_tag.length(), close_sender_pos);

     
      Serial.println(msg);

      render_sms(msg, sender, lcd);   
      bt_data = "";
    }
   
}

void render_sms(String message, String from, hd44780_I2Cexp lcd)
{
  Serial.println(message);
  Serial.println(from);
  // displayifirst 60 chars of a message
  int i = 0; 
  int col = 0;
  int row = 0;
  lcd.clear();
  while(i < 60)
  {
   row = (int) i / 20;
   col = i % 20;
   lcd.setCursor(col, row);
   if(i < message.length())
   {
   lcd.print(message[i]);
   }
   else
   {
    lcd.print(" ");
   }
   
  
   i++;
}

  lcd.setCursor(5, 3);
  lcd.print(from);
}

void loop()
{
static unsigned long lastsecs = -1; // pre-initialize with non zero value
unsigned long secs;

	secs = millis() / 1000;

	// see if 1 second has passed
	// so the display is only updated once per second
	if(secs != lastsecs)
	{
		lastsecs = secs; // keep track of last seconds

		// set the cursor position to column 0, row 1
		// note: row 1 is the second row from top,
		// since row counting begins with 0
	//	lcd.setCursor(0, 1);

		// print uptime on lcd device: (time since last reset)
		//PrintUpTime(lcd, secs);
	}

 //bluetooth stuff

 if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    // Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer
    if(inputString == "a"){         //in case of 'a' turn the LED on
      digitalWrite(13, HIGH);  
    }else if(inputString == "b"){   //incase of 'b' turn the LED off
      digitalWrite(13, LOW);
    }

    bt_data += inputString;
    inputString = "";
    
   parse_bt(bt_data);
 }
}

// PrintUpTime(outdev, secs) - print uptime in HH:MM:SS format
// outdev - the device to send output
//   secs - the total number of seconds uptime
void PrintUpTime(Print &outdev, unsigned long secs)
{
unsigned int hr, mins, sec;

	// convert total seconds to hours, mins, seconds
	mins =  secs / 60;	// how many total minutes
	hr = mins / 60;		// how many total hours
	mins = mins % 60;	// how many minutes within the hour
	sec = secs % 60;	// how many seconds within the minute
		

	// print uptime in HH:MM:SS format
	// Print class does not support fixed width formatting
	// so insert a zero if number smaller than 10
	if(hr < 10)
		outdev.write('0');
	outdev.print((int)hr);
	outdev.write(':');
	if(mins < 10)
		outdev.write('0');
	outdev.print((int)mins);
	outdev.write(':');
	if(sec < 10)
		outdev.write('0');
	outdev.print((int)sec);
}

// fatalError() - loop & blink an error code
void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}
