#ifndef WebInterface_h
#define WebInterface_h
#pragma once

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "Streaming.h"
#include "Platform.h"

#include <stdexcept>

using namespace std;

//	Serial must be initialized externaly
//  Wi-Fi must be initialized externaly

class WebInterface
{
private:
    ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

    ESP8266WebServer server; // A webserver object that listens for HTTP request on port 80

    Platform *platform = NULL;

    void handleRoot()
    { // When URI / is requested, send a web page with a button to toggle the LED
        server.send(200, "text/html", "<form action=\"/give_the_chocolate\" method=\"POST\"><input type=\"submit\" value=\"Give the chokolate\"></form>");
    }

    void handleGive_the_chocolate()
    { // If a POST request is made to URI /give_the-chocolate
        try
        {
            Serial << "Giving the chokolate..." << endl;
            Serial << "Opening the platform..." << endl;
            platform->Open();
            Serial << "Platform opened." << endl;
            Serial << "Closing the platform..." << endl;
            platform->Close();
            Serial << "Platform closed." << endl;
        }
        catch (runtime_error &e)
        {
            Serial << "[Error] " << e.what() << endl;
        }
        server.sendHeader("Location", "/"); // Add a header to respond with a new location for the browser to go to the home page again
        server.send(303);                   // Send it back to the browser with an HTTP status 303 (See Other) to redirect
    }

    void handleNotFound()
    {
        server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
    }

public:
    WebInterface(Platform *platform) : server(80), platform(platform)
    {
        wifiMulti.run();

        if (MDNS.begin("esp8266"))
        { // Start the mDNS responder for esp8266.local
            Serial << "mDNS responder started" << endl;
        }
        else
        {
            Serial << "Error setting up MDNS responder!" << endl;
        }

        server.on("/", HTTP_GET, [this]()
                  { this->handleRoot(); }); // Call the 'handleRoot' function when a client requests URI "/"
        server.on("/give_the_chocolate", HTTP_POST, [this]()
                  { this->handleGive_the_chocolate(); }); // Call the 'handleLED' function when a POST request is made to URI "/give_the-chocolate"
        server.onNotFound([this]()
                          { this->handleNotFound(); }); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

        server.begin(); // Actually start the server
        Serial << "HTTP server started" << endl;
    }

    void update()
    {
        server.handleClient(); // Listen for HTTP requests from clients
    }
};

#endif