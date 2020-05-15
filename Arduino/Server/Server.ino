#define WIFI_SSID "JihadJoe"
#define WIFI_PSK  "2winvegas"
#define KEY "auth=123"
#define LOCK_ID 1

#include "cert.h"
#include "private_key.h"
#include <Servo.h>
#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <HTTPClient.h>
//#include <PN532_I2C.h>
//#include <PN532.h>
//#include <NfcAdapter.h>
//#include <Wire.h>

using namespace httpsserver;

// PN532 NFC Board
//PN532_I2C pn532_i2c(Wire);
//NfcAdapter nfc = NfcAdapter(pn532_i2c);

// Servo config
static const int servoPin = 13;
static const int servoOpenPos = 6;
static const int servoClosePos = 70;
Servo servy;
static char* lockState = "0"; // Default unlocked


// Create cert from files
SSLCert cert = SSLCert(
  example_crt_DER, example_crt_DER_len,
  example_key_DER, example_key_DER_len
);

// SSL-Server
HTTPSServer secureServer = HTTPSServer(&cert);

// HTTPS Client
HTTPClient httpClient;
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIICOzCCAaQCCQDc1pPiEsF6yTANBgkqhkiG9w0BAQsFADBiMQswCQYDVQQGEwJV\n" \
"UzELMAkGA1UECAwCQ0ExETAPBgNVBAcMCFRlbWVjdWxhMRIwEAYDVQQKDAlCYW5h\n" \
"bmFTZWMxHzAdBgNVBAMMFnNlcnZlci5iYW5hbmFzZWMubG9jYWwwHhcNMjAwNTAy\n" \
"MjAzNzMyWhcNMzAwNDMwMjAzNzMyWjBiMQswCQYDVQQGEwJVUzELMAkGA1UECAwC\n" \
"Q0ExETAPBgNVBAcMCFRlbWVjdWxhMRIwEAYDVQQKDAlCYW5hbmFTZWMxHzAdBgNV\n" \
"BAMMFnNlcnZlci5iYW5hbmFzZWMubG9jYWwwgZ8wDQYJKoZIhvcNAQEBBQADgY0A\n" \
"MIGJAoGBAJS/sKS/JfOk+yQq9i9aFJdI4D6LVidugTybHOiQB3p5+DmZPoK+uBqr\n" \
"pxMmqi1JFUH/8C70QcLMPtpbNB+tjmi/ZI7sFwQVOy4vHhulggghXnBQSAIAj942\n" \
"KEM7yTP/Zp+Psowv/aWX9zfCZIIYF3ubwN2iDvhCNSoFnqGKHr+/AgMBAAEwDQYJ\n" \
"KoZIhvcNAQELBQADgYEATf/1LOPvqSWzihgdhIt47Zb3R/4QN5kkx8TuRFjH4yJm\n" \
"YFy/9ZE/JjduIfn5naAvZu3FYUoK0Fgx7c8DtYRXe6d+soYrEQj6hcpZqTvdbdxK\n" \
"hXqR5jUExgZ+HFG00uVHR8epx3FjxUy+p+ysp0i2HQr/Y0LuHIKhhG7lypHD2CE=\n" \
"-----END CERTIFICATE-----\n";

// Endpoints
void handleLock(HTTPRequest * req, HTTPResponse * res);
void handleUnlock(HTTPRequest * req, HTTPResponse * res);
void handleState(HTTPRequest * req, HTTPResponse * res);
void handle404(HTTPRequest * req, HTTPResponse * res);

void setup() {
  Serial.begin(115200);

  // Start PN532 board
  // nfc.begin();

  // Attach to servo and reset position
  servy.attach(servoPin);
  servy.write(servoOpenPos);

  // Connect to WiFi
  Serial.println("Setting up WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected. IP=");
  Serial.println(WiFi.localIP());

  // Default endpoint
  ResourceNode * node404  = new ResourceNode("", "GET", &handle404);

  // Register endpoints
  ResourceNode * nodeLock = new ResourceNode("/lock", "POST", &handleLock);
  ResourceNode * nodeUnlock = new ResourceNode("/unlock", "POST", &handleUnlock);
  ResourceNode * nodeState = new ResourceNode("/state", "POST", &handleState);

  // Add the nodes for the echo service
  secureServer.registerNode(nodeLock);
  secureServer.registerNode(nodeUnlock);
  secureServer.registerNode(nodeState);

  // Add the 404 not found node to the server.
  // The path is ignored for the default node.
  secureServer.setDefaultNode(node404);

  Serial.println("Starting server...");
  secureServer.start();
  if (secureServer.isRunning()) {
    Serial.println("Server ready.");
  }
}

void loop() {
  // This call will let the server do its work
  secureServer.loop();

  // NFC board died
  // NFC reader scans for tags to unlock the door
  //  if (nfc.tagPresent())
  //  {
  //    NfcTag tag = nfc.read();
  //    sendTag(tag.getUidString());
  //  }
}

void handleLock(HTTPRequest * req, HTTPResponse * res) {
  if (authCheckAndRespond(req, res)) {
    Serial.println("Auth success. Locking");
    servy.write(servoClosePos);
    lockState = "1";
  }
  else
  {
    Serial.println("Auth failed");
  }
}

void handleUnlock(HTTPRequest * req, HTTPResponse * res) {
  if (authCheckAndRespond(req, res)) {
    Serial.println("Auth success. Unlocking");
    servy.write(servoOpenPos);
    lockState = "0";

  }
  else
  {
    Serial.println("Auth failed");
  }
}

void handleState(HTTPRequest * req, HTTPResponse * res) {
  byte buffer[256];
  char message[] = "2"; // Start with fail message

  // Set header
  res->setHeader("Content-Type","text/plain");

  while(!(req->requestComplete())) {
    size_t s = req->readBytes(buffer, 256);

    // Terminate the buffer for comparison
    buffer[s] = '\0';
    Serial.println("Buffer: ");
    Serial.println((char *)buffer);

    // Check against hardcoded key
    if (strcmp(KEY, (char *)buffer) == 0)
    {
      Serial.print("Auth success. Sending state: ");
      Serial.println(lockState);

      strcpy(message, lockState);
    }

    // Send response
    res->write((byte*)message, 1);
  }
}

// Check auth key and responds with authorization sucess
int authCheckAndRespond(HTTPRequest * req, HTTPResponse * res) {
  int success = 0;
  byte buffer[256];
  char message[] = "0";
  
  // Set header
  res->setHeader("Content-Type","text/plain");

  while(!(req->requestComplete())) {
    size_t s = req->readBytes(buffer, 256);

    // Terminate the buffer for comparison
    buffer[s] = '\0';
    Serial.println("Buffer: ");
    Serial.println((char *)buffer);

    // Check against hardcoded key
    if (strcmp(KEY, (char *)buffer) == 0)
    {
      strcpy(message, "1");
      success = 1;
    }

    // Send response
    res->write((byte*)message, 1);
  }

  return success;
}

void handle404(HTTPRequest * req, HTTPResponse * res) {
  // Discard request body, if we received any
  // We do this, as this is the default node and may also server POST/PUT requests
  req->discardRequestBody();

  // Set the response status
  res->setStatusCode(404);
  res->setStatusText("Not Found");

  // Set content type of the response
  res->setHeader("Content-Type", "text/html");

  // Write a tiny HTTP page
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Not Found</title></head>");
  res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
  res->println("</html>");
}


// Send NFC tag to server and unlock if tag is found
void sendTag(String serial) {
    // Format serial
    serial.replace(" ", "");

    String url = "https://server.bananasec.local/nfc_unlock?lock_id=";
    url += LOCK_ID;
    url += "&serial_number=";
    url += serial;

    Serial.print("Requesting: ");
    Serial.println(url);

    httpClient.begin(url, root_ca); //Specify the URL and certificate
    int httpCode = httpClient.GET();
 
    if (httpCode > 0) { //Check for the returning code
        String payload = httpClient.getString();
        if (httpCode == 200 && payload.equals("1")) {
          Serial.println("Tag success: unlocking");

          // Unlock servo
          servy.write(servoOpenPos);
          lockState = "0";
        } else {
          Serial.println("Tag failed");
        }
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    httpClient.end(); //Free the resources
}
