# smsbox
Firmware for AVR-based external Android SMS display

SmsBox. 

It is an external notice screen for your smartphone, meant to be placed on the dashboard of your vehicle. It notifies you when you get a new SMS and immediately displays it on the 20x4 LCD. You don’t have to interact with your phone and get distracted which should improve driving safety.

You place it on the dashboard, pair Android with it (using a custom app I made), and then it notifies you whenever there is a new text and lets you immediately read it on the LCD without having to dig through your phone.

The text itself is displayed on first three lines of the LCD screen, sender number on fourth line.

Technical overview: I conceptualized and prototyped a custom AVR-based device in my electronics lab, wrote some firmware for it in C mostly to talk to HC-05 and LCD, and made a companion application for Android in Java to communicate over Bluetooth using a basic XML-based protocol. The device goes in the car on the dashboard, and connects to an Android smartphone using Bluetooth. Then you launch the SmsBoxAndroid app on Android, it hooks up to SmsReceived event and whenever you get a new text it sends it over to Arduino.  Hardware modules:

The pseudo-XML format is: < sms >< msg >The message itself that's displayed in first three lines.</ msg >< sender >phone_number</ sender ></ sms > (remove spaces in tags, something with Github formatting hides them if I just put straight tags)

The hardware:

1) Arduino Uno R3 or just an ATMEGA chip with firmware flashed.

2) HC-05 Bluetooth module -- it can act as a master/slave, which is useful if we want it to initialize communication sometimes in the future.

3) 20x4 I2C LCD: can display 4 lines, 20 characters per line. 
   3x20 for SMS, 1x20 for sender number/name. Do you really need more than first 60 characters of an SMS to see if the SMS is worth
   further effort?
   
4) Something to hold the cables routed to HC-05 and the LCD. I used a perfboard soldered to the bottom of my Arduino.

5) A power supply, options: 

   a) either a 9V battery + battery-holder. 
   
   b) long, 6 foot USB A-to-B cable to power up this thing from the car lighter jack.
   
  I would go with option b) because I noticed a 9V battery provides about 4-5 hours of uptime so it's quite expensive unless they're rechargeable or something I guess.

The parts above go into one enclosure with a window for LCD and some holes for power cables if necessary.
...

5) Bluetooth-enabled Android phone with the companion app installed (coming soon).

The pins used are: A4, A5 for the LCD I2C interface and RX, TX for HC-05. The rest is 5V/GND power stuff. This leaves a lot of pins for any further modifications.. pretty much all your digital pins and some analog ones but you have the Bluetooth communication/LCD display base.
