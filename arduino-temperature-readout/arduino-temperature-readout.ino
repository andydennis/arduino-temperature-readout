/*
 Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 modified 18 Oct 2012
 by Andy Dennis

 DHT test

 Returns temperature and humidity.

 modified 18 Oct 2012
 by Andy Dennis  

 */

#include "DHT.h"
#include <spi.h>
#include <ethernet.h>
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);
DHT dht(DHTPIN, DHTTYPE);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
void setup()
{
 // start the Ethernet connection and the server:
 Ethernet.begin(mac, ip);
 server.begin(); 
 dht.begin();
}

void loop()
{
 float h = dht.readHumidity();
 float t = dht.readTemperature();

 // listen for incoming clients
 EthernetClient client = server.available();
 if (client) {

 // an http request ends with a blank line
   boolean currentLineIsBlank = true;
   while (client.connected()) {
     if (client.available()) {
       char c = client.read();
      
       // if you've gotten to the end of the line (received a newline
       // character) and the line is blank, the http request has ended,
       // so you can send a reply
       if (c == '\n' && currentLineIsBlank) {
       // send a standard http response header
         client.println("HTTP/1.1 200 OK");
         client.println("Content-Type: text/html");
         client.println();
      
         // check if returns are valid, if they are NaN (not a number) then something went wrong!
         if (isnan(t) || isnan(h)) {
           client.println("Failed to read from DHT");
         } else {
           client.print("Humidity: "); 
           client.print(h);
           client.print(" %\t");
           client.print("Temperature: "); 
           client.print(t);
           client.println(" *C ");
         }
         break;
       }
       if (c == '\n') {
       // you're starting a new line
       currentLineIsBlank = true;
       } else if (c != '\r') {
         // you've gotten a character on the current line
         currentLineIsBlank = false;
       }
     }
   }
 // give the web browser time to receive the data
 delay(1);
 // close the connection:
 client.stop();
 }
}
