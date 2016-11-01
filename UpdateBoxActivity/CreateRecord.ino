#include <GSM.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

GSMClient client;
GPRS gprs;
GSM gsm; 

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  // connection state
  boolean connected = false;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while(!connected)
  {
    Serial.println("Connecting");
    if ((gsm.begin(GSM_PIN_NUMBER)==GSM_READY) && (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY)) {
      connected = true;
    }
  }

  Serial.println("Setup complete.\n");
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running CreateRecord - Run #" + String(numRuns++));

    TembooChoreo CreateRecordChoreo(client);

    // Invoke the Temboo client
    CreateRecordChoreo.begin();

    // Set Temboo account credentials
    CreateRecordChoreo.setAccountName(TEMBOO_ACCOUNT);
    CreateRecordChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    CreateRecordChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set profile to use for execution
    CreateRecordChoreo.setProfile("med");

    // Identify the Choreo to run
    CreateRecordChoreo.setChoreo("/Library/Salesforce/Objects/CreateRecord");

    // Run the Choreo; when results are available, print them to serial
    CreateRecordChoreo.run();

    while(CreateRecordChoreo.available()) {
      char c = CreateRecordChoreo.read();
      Serial.print(c);
    }
    CreateRecordChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between CreateRecord calls
}