#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_receiver;

const char* receiverID = "RX1:";

// ─────────────────────────────────────────────
// Decode number string to plain text
// Example: "0805121215" → "hello"
// reads every 2 digits → converts to letter
// 00=space 01=a 02=b ... 09=i 10=j ... 26=z
// ─────────────────────────────────────────────
String decodeMessage(String encoded) {
  String decoded = "";

  for (int i = 0; i + 1 < (int)encoded.length(); i += 2) {
    char chunk[3] = { encoded[i], encoded[i + 1], '\0' };
    int num = atoi(chunk);

    if (num == 0) {
      decoded += ' ';
    } else if (num >= 1 && num <= 26) {
      decoded += (char)('a' + num - 1);
    } else {
      decoded += '?';
    }
  }
  return decoded;
}

void setup() {
  Serial.begin(9600);

  if (!rf_receiver.init()) {
    Serial.println("Receiver Initialization Failed!");
    while (true);
  }

  Serial.println("------------------------------------");
  Serial.println(" Group 4 - Encrypted Messaging");
  Serial.println(" RECEIVER");
  Serial.println("------------------------------------");
  Serial.println("Receiver Ready! Waiting for messages...");
  Serial.println("------------------------------------");
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rf_receiver.recv(buf, &buflen)) {
    buf[buflen] = '\0';
    String message = String((char*)buf);

    if (message.startsWith(receiverID)) {
      // strip the receiver ID to get just the encoded numbers
      String encoded = message.substring(strlen(receiverID));
      encoded.trim();

      // decode numbers to plain text
      String decoded = decodeMessage(encoded);

      Serial.println("------------------------------------");
      Serial.print("[Encoded received]: ");
      Serial.println(encoded);
      Serial.print("[Decoded message]:  ");
      Serial.println(decoded);
      Serial.println("------------------------------------");
    } else {
      Serial.println("Message received but intended for a different receiver.");
    }
  }
}
// MARKOS — Group 4
