#include <Bridge.h>
#include <Temboo.h>
#include <Process.h>

// Contains Temboo account information
#include "TembooAccount.h" 

// Variables

unsigned long time;

// Process to get the measurement time
Process date;

// Your Google Docs data
const String GOOGLE_USERNAME = "arduino.taiwan";
const String GOOGLE_PASSWORD = "taiwan1234";
const String SPREADSHEET_TITLE = "yun";

// Include required libraries
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

// Debug mode ?
boolean debug_mode = true;

void setup() {
   // Initialize BH1750 sensor
    lightMeter.begin();
  // Start Serial
      Serial.begin(9600);
      /*
  if (debug_mode == true){
    Serial.begin(9600);
    delay(4000);
    while(!Serial);
  }
  */
 

  
  // Start bridge
  Bridge.begin();
  
  // Start date process
  time = millis();
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
  }
    Serial.println("Setup complete. Waiting for sensor input...\n");

  if (debug_mode == true){
    Serial.println("Setup complete. Waiting for sensor input...\n");
  }
}

void loop() {

  // Measure the light Lux
   uint16_t lightLevel = lightMeter.readLightLevel();
      Serial.print("now Light Level is :") ;
      Serial.print(lightLevel) ;
      Serial.print("\n");
 
  if (debug_mode == true){
    Serial.println("\nCalling the /Library/Google/Spreadsheets/AppendRow Choreo...");
  }
  
  // Append data to Google Docs sheet
  runAppendRow(lightLevel);
        
  // Repeat every 10 minutes
  delay(600000);
}

// Function to add data to Google Docs
void runAppendRow(int lightLevel) {
  TembooChoreo AppendRowChoreo;

  // Invoke the Temboo client
  AppendRowChoreo.begin();

  // Set Temboo account credentials
  AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
  AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);
  
  // Identify the Choreo to run
  AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");

  // your Google username (usually your email address)
  AppendRowChoreo.addInput("Username", GOOGLE_USERNAME);

  // your Google account password
  AppendRowChoreo.addInput("Password", GOOGLE_PASSWORD);
  
  // the title of the spreadsheet you want to append to
  AppendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);
  
  // Restart the date process:
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%D-%T");
    date.run();
   }
  
  // Get date
  String timeString = date.readString(); 
  
  // Format data
  String data = "";
  data = data + timeString + "," + String(lightLevel);

  // Set Choreo inputs
  AppendRowChoreo.addInput("RowData", data);

  // Run the Choreo
  unsigned int returnCode = AppendRowChoreo.run();

  // A return code of zero means everything worked
  if (returnCode == 0) {
    if (debug_mode == true){
      Serial.println("Completed execution of the /Library/Google/Spreadsheets/AppendRow Choreo.\n");
    }
  } else {
    // A non-zero return code means there was an error
    // Read and print the error message
    while (AppendRowChoreo.available()) {
      char c = AppendRowChoreo.read();
      if (debug_mode == true){ Serial.print(c); }
    }
    if (debug_mode == true){ Serial.println(); }
  }
  AppendRowChoreo.close();
}
