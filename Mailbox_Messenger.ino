// Mailbox Messenger
// For roommates who share a mailbox. Sends text to appropriate roommate when there is mail.

// Enter your own values here:
#define TEMBOO_ACCOUNT "xxxxxxxx"                                  // your Temboo account name 
#define TEMBOO_APP_KEY_NAME "xxxxxxx"                              // your Temboo app key name
#define TEMBOO_APP_KEY  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"         // your Temboo app key
#define TWILIO_ACCOUNT_SID "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"    // the Account SID from your Twilio account
#define TWILIO_AUTH_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"       // the Auth Token from your Twilio account
#define TWILIO_NUMBER "+1 ###-###-####"                            // your Twilio phone number, e.g., "+1 555-222-1212"
#define ROOMMATE1_NUM "+1 ###-###-####"                            // the number to which the SMS should be sent, e.g., "+1 555-222-1212"
#define ROOMMATE2_NUM "+1 ###-###-####"                            // the number to which the SMS should be sent, e.g., "+1 555-222-1212"
#define ROOMMATE1 HIGH                                             // flag is down
#define ROOMMATE2 LOW                                              // flag is up
#define OPEN HIGH                                                  // door is open
#define CLOSED LOW                                                 // door is closed

// Include statements:
#include <Bridge.h>
#include <Temboo.h>

const int flagPin = 2;              // pin number of switch sensor
const int doorPin = 4;              // pin number of mail door sensor
const int ledPin = 13;              // pin number of the LED

boolean flagState = ROOMMATE1;      // state of the flag
boolean doorState = CLOSED;         // state of the door sensor

void setup() {
  pinMode(flagPin, INPUT);          // set flagPin to INPUT
  pinMode(doorPin, INPUT);          // set doorPin to INPUT
  pinMode(ledPin, OUTPUT);          // set ledPin to OUTPUT

  Bridge.begin();
}

// Send Text to appropriate roommate
void sendText()
{
  // Send an SMS via a Twilio account and Temboo
      
  // we need a Process object to send a Choreo request to Temboo
  TembooChoreo SendSMSChoreo;
  
  // invoke the Temboo client
  // NOTE that the client must be reinvoked and repopulated with
  // appropriate arguments each time its run() method is called.
  SendSMSChoreo.begin();
      
  // set Temboo account credentials
  SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
  
  // identify the Temboo Library choreo to run (Twilio > SMSMessages > SendSMS)
  SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
  
  // set the required choreo inputs
  // see https://www.temboo.com/library/Library/Twilio/SMSMessages/SendSMS/ 
  // for complete details about the inputs for this Choreo
  
  // the first input is a your AccountSID
  SendSMSChoreo.addInput("AccountSID", TWILIO_ACCOUNT_SID);
      
  // next is your Auth Token
  SendSMSChoreo.addInput("AuthToken", TWILIO_AUTH_TOKEN);
   
  // next is your Twilio phone number
  SendSMSChoreo.addInput("From", TWILIO_NUMBER);
      
  // next, what number to send the SMS to
  if(flagState == ROOMMATE1){
    SendSMSChoreo.addInput("To", ROOMMATE1_NUM);
  } else{
    SendSMSChoreo.addInput("To", ROOMMATE2_NUM);
  }
  
  // finally, the text of the message to send
  SendSMSChoreo.addInput("Body", "You got mail!");
      
  // tell the Process to run and wait for the results. The 
  // return code (returnCode) will tell us whether the Temboo client 
  // was able to send our request to the Temboo servers
  unsigned int returnCode = SendSMSChoreo.run();
  
  SendSMSChoreo.close();
}

void blinkLED(){
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
}

void loop() {

  // Wait for door open
  while(doorState == CLOSED){
    doorState = digitalRead(doorPin);  
  }

  // Wait for door close
  while(doorState == OPEN){
    doorState = digitalRead(doorPin);
    // Roommate inserts mail, sets flag
  ;}
  
  // Read roommate flag
  flagState = digitalRead(flagPin);

  // Send text to appropriate roommate
  digitalWrite(ledPin, HIGH);
  sendText();
  digitalWrite(ledPin, LOW);

  // Wait for door open
  while(doorState == CLOSED){
    doorState = digitalRead(doorPin);
    blinkLED();
  }

  // Wait for door close
  while(doorState == OPEN){
    doorState = digitalRead(doorPin);
    // Other roommate retrieves mail
  ;}
  
}
