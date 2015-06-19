#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running GetTemperature - Run #" + String(numRuns++));
    
    TembooChoreo GetTemperatureChoreo;

    // Invoke the Temboo client
    GetTemperatureChoreo.begin();

    // Set Temboo account credentials
    GetTemperatureChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetTemperatureChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetTemperatureChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    GetTemperatureChoreo.addInput("Address", "Taiwan,TW");
    
    // Identify the Choreo to run
    GetTemperatureChoreo.setChoreo("/Library/Yahoo/Weather/GetTemperature");
    
    // Run the Choreo; when results are available, print them to serial
    GetTemperatureChoreo.run();
    
    while(GetTemperatureChoreo.available()) {
      char c = GetTemperatureChoreo.read();
      Serial.print(c);
    }
    GetTemperatureChoreo.close();
  }

  Serial.println("Waiting...");
  delay(30000); // wait 30 seconds between GetTemperature calls
}
