/*
  SendAnEmail

  Demonstrates sending an email via a Google Gmail account using the Temboo Arduino Yun SDK.

  This example code is in the public domain.
*/

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
Process picture;

/*** SUBSTITUTE YOUR VALUES BELOW: ***/

// Note that for additional security and reusability, you could
// use #define statements to specify these values in a .h file.

// your Gmail username, formatted as a complete email address, eg "bob.smith@gmail.com"
const String GMAIL_USER_NAME = "arduino.taiwan";

// your Gmail App-Specific Password
const String GMAIL_PASSWORD = "taiwan1234";

// the email address you want to send the email to, eg "jane.doe@temboo.com"
const String TO_EMAIL_ADDRESS = "arduino.taiwan@gmail.com";


boolean success = false; // a flag to indicate whether we've sent the email yet or not

// Filename
String filename;

// Pin
int pir_pin = 8;

// Path
String path = "/mnt/sda1/cap/";



void setup() {
  Serial.begin(9600);

  // for debugging, wait until a serial console is connected
  delay(4000);
  while(!Serial);

  Bridge.begin();
    // Set pin mode
  pinMode(pir_pin,INPUT);
  
}

void loop()
{
    if (digitalRead(pir_pin) == true) {
    
    // Generate filename with timestamp
    filename = "";
    picture.runShellCommand("date +%s");
    while(picture.running());

    while (picture.available()>0) {
      char c = picture.read();
      filename += c;
    } 
    filename.trim();
    filename += ".jpg";
 
    // Take picture
    picture.runShellCommand("fswebcam " + path + filename + " -r 1280x720");
    while(picture.running());
    
    // send message to gmail
            // only try to send the email if we haven't already sent it successfully
            if (!success) {
          
              Serial.println("Running SendAnEmail...");
            
              TembooChoreo SendEmailChoreo;
          
              // invoke the Temboo client
              // NOTE that the client must be reinvoked, and repopulated with
              // appropriate arguments, each time its run() method is called.
              SendEmailChoreo.begin();
              
              // set Temboo account credentials
              SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
              SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
              SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
          
              // identify the Temboo Library choreo to run (Google > Gmail > SendEmail)
              SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
           
          
              // set the required choreo inputs
              // see https://www.temboo.com/library/Library/Google/Gmail/SendEmail/ 
              // for complete details about the inputs for this Choreo
          
              // the first input is your Gmail email address
              SendEmailChoreo.addInput("Username", GMAIL_USER_NAME);
              // next is your Gmail App-Specific password.
              SendEmailChoreo.addInput("Password", GMAIL_PASSWORD);
              // who to send the email to
              SendEmailChoreo.addInput("ToAddress", TO_EMAIL_ADDRESS);
              // then a subject line
              SendEmailChoreo.addInput("Subject", "ALERT: Some Body Coming for Viewing picture:"+filename);
          
               // next comes the message body, the main content of the email   
              SendEmailChoreo.addInput("MessageBody", "Hey! The greenhouse is too cold!");
          
              // tell the Choreo to run and wait for the results. The 
              // return code (returnCode) will tell us whether the Temboo client 
              // was able to send our request to the Temboo servers
              unsigned int returnCode = SendEmailChoreo.run();
          
              // a return code of zero (0) means everything worked
              if (returnCode == 0) {
                  Serial.println("Success! Email sent!");
                  success = true;
              } else {
                // a non-zero return code means there was an error
                // read and print the error message
                while (SendEmailChoreo.available()) {
                  char c = SendEmailChoreo.read();
                  Serial.print(c);
                }
              } 
              SendEmailChoreo.close();
          
              // do nothing for the next 60 seconds
              delay(60000);
            }
  }
  

}
