#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_transmitter;

const char* receiverID = "RX1:";

void setup() {
  Serial.begin(9600);

  if (!rf_transmitter.init()) {
    Serial.println("Oops! Transmitter failed to initialize. Check the wiring and try again.");
    while (true);
  }

  Serial.println("------------------------------------");
  Serial.println(" Group 4 - Encrypted Messaging");
  Serial.println(" TRANSMITTER");
  Serial.println("------------------------------------");
  Serial.println("Type encoded numbers and press Enter.");
  Serial.println("Each letter = 2 digits, no spaces.");
  Serial.println("a=01 b=02 ... i=09 j=10 ... z=26 space=00");
  Serial.println("Example: hello = 0805121215");
  Serial.println("------------------------------------");
}

void loop() {
  if (Serial.available() > 0) {
    char userMessage[50];
    int len = Serial.readBytesUntil('\n', userMessage, sizeof(userMessage) - 1);
    userMessage[len] = '\0';

    String input = String(userMessage);
    input.trim();

    if (input.length() == 0) return;

    // validate — digits only and even length
    bool valid = true;
    if (input.length() % 2 != 0) {
      valid = false;
    } else {
      for (int i = 0; i < (int)input.length(); i++) {
        if (!isDigit(input[i])) {
          valid = false;
          break;
        }
      }
    }

    if (!valid) {
      Serial.println("[Error] Type even number of digits only.");
      Serial.println("Example: 0805121215");
      return;
    }

    // combine receiver ID + encoded numbers
    char finalMessage[60];
    snprintf(finalMessage, sizeof(finalMessage), "%s%s", receiverID, input.c_str());

    // send over RF
    rf_transmitter.send((uint8_t*)finalMessage, strlen(finalMessage));
    rf_transmitter.waitPacketSent();

    Serial.print("[Sent encoded]: ");
    Serial.println(input);
    Serial.println("------------------------------------");
  }
}
// MARKOS — Group 4
