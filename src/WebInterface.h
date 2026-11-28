#ifndef WebInterface_h
#define WebInterface_h
#pragma once

#include "Platform.h"
#include "Logger/Logger.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <stdexcept>

using namespace std;

//	Logger must be initialized externaly
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

    void handleGiveTheChocolate()
    { // If a POST request is made to URI /give_the-chocolate

        LOG(LogLevel::INFO) << "Giving the chokolate...";
        server.sendHeader("Location", "/open_the_door"); // Add a header to respond with a new location for the browser
        server.send(303);                                // Send it back to the browser with an HTTP status 303 (See Other) to redirect
    }

    void handleOpenTheDoor()
    { // If a POST request is made to URI /open_the_door
        try
        {
            LOG(LogLevel::INFO) << "Opening the platform...";
            platform->open();
            LOG(LogLevel::INFO) << "Platform opened.";
            server.sendHeader("Location", "/close_the_door"); // Add a header to respond with a new location for the browser
            server.send(303);                                 // Send it back to the browser with an HTTP status 303 (See Other) to redirect
        }
        catch (runtime_error &e)
        {
            LOG(LogLevel::ERROR) << e.what();
            server.send(200, "text/html", "<h1>Error</h1><p>" + String(e.what()) + "</p>");
        }
    }

    void handleCloseTheDoor()
    { // If a POST request is made to URI /close_the_door
        try
        {
            LOG(LogLevel::INFO) << "Closing the platform...";
            platform->close();
            LOG(LogLevel::INFO) << "Platform closed.";
            server.send(200, "text/html", "<h1>Success</h1><p>Chokolate delivered.</p>");
        }
        catch (runtime_error &e)
        {
            LOG(LogLevel::ERROR) << e.what();
            server.send(200, "text/html", "<h1>Error</h1><p>" + String(e.what()) + "</p>");
        }
    }

    void handleNotFound()
    {
        server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
    }

public:
    WebInterface(Platform *platform) : server(80), platform(platform)
    {
        if (wifiMulti.run() != WL_CONNECTED)
        { // Wait for Wi-Fi to connect
            LOG(LogLevel::WARN) << "Web interface is not available!";
            return;
        }

        if (MDNS.begin("esp8266"))
        { // Start the mDNS responder for esp8266.local
            LOG(LogLevel::INFO) << "mDNS responder started";
        }
        else
        {
            LOG(LogLevel::INFO) << "Error setting up MDNS responder!";
        }

        server.on("/", HTTP_GET, [this]()
                  { this->handleRoot(); }); // Call the 'handleRoot' function when a client requests URI "/"
        server.on("/give_the_chocolate", HTTP_POST, [this]()
                  { this->handleGiveTheChocolate(); }); // Call the 'handleGiveTheChocolate' function when a POST request is made to URI "/give_the-chocolate"
        server.on("/open_the_door", HTTP_GET, [this]()
                  { this->handleOpenTheDoor(); }); // Call the 'handleOpenTheDoor' function when a POST request is made to URI "/open_the_door"
        server.on("/close_the_door", HTTP_GET, [this]()
                  { this->handleCloseTheDoor(); }); // Call the 'handleCloseTheDoor' function when a POST request is made to URI "/close_the_door"
        server.onNotFound([this]()
                          { this->handleNotFound(); }); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

        server.begin(); // Actually start the server
        LOG(LogLevel::INFO) << "HTTP server started";
    }

    void update()
    {
        server.handleClient(); // Listen for HTTP requests from clients
    }
};

#endif