//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // WIFI Module LED
  #define     LED1      0          // Connectivity With Client #1
  #define     LED2      1          // Connectivity With Client #2
  #define     BUTTON    3          // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       ssid;              // SERVER WIFI NAME
  char*       password;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     6           // MAXIMUM NUMBER OF CLIENTS

  /*
   * MAXSC => Maximum Server Clients
   * They Are 6 Because
   * 1. Blue Fighter Suit
   * 2. Red Fighter Suit
   * 3. Corner Referee 1
   * 4. Corner Referee 2
   * 5. Corner Referee 3
   * 6. Corner Referee 4
   */
  
  WiFiServer  server(80);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  clients;          // THE SERVER CLIENTS (Devices)
//====================================================================================

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
    pinMode(LED2, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    // Setting Up A Wifi Access Point
    SetWifi("Rover_Ubihard", "");
  }

//====================================================================================
  
  void loop()
  {
    WiFiClient clients = server.available();
    if (clients) {
      if (clients.connected()) {
        String request = Serial.readStringUntil('\n');    // receives the message from the client
        clients.println(request); 
        clients.flush();
      }
    
      clients.stop(); 
    }
  }

//====================================================================================
  
  void SetWifi(char* Name, char* Password)
  {
    // Stop Any Previous WIFI
    WiFi.disconnect();

    // Setting The Wifi Mode
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WIFI Mode : AccessPoint Station");
    
    // Setting The AccessPoint Name & Password
    ssid      = Name;
    password  = Password;
    
    // Starting The Access Point
    WiFi.softAP(ssid, password);
    Serial.println("WIFI < " + String(ssid) + " > ... Started");
    
    // Wait For Few Seconds
    delay(1000);
    
    // Getting Server IP
    IPAddress IP = WiFi.softAPIP();
    
    // Printing The Server IP Address
    Serial.print("AccessPoint IP : ");
    Serial.println(IP);

    // Printing MAC Address
    Serial.print("AccessPoint MC : ");
    Serial.println(String(WiFi.softAPmacAddress()));

    // Starting Server
    server.begin();
    server.setNoDelay(true);
    Serial.println("Server Started");
  }

//====================================================================================

  void AvailableClients()
  {   
    if (server.hasClient())
    {
      // Read LED0 Switch To Low If High.
      if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW);
      
      // Light Up LED1
      digitalWrite(LED1, HIGH);
      
      for(uint8_t i = 0; i < MAXSC; i++)
      {
        //find free/disconnected spot
        if (!clients || !clients.connected())
        {
          // Checks If Previously The Client Is Taken
          if(clients)
          {
            clients.stop();
          }

          // Checks If Clients Connected To The Server
          if(clients = server.available())
          {
            Serial.println("New Client: " + String(i));
          }

          // Continue Scanning
          continue;
        }
      }
      
      //no free/disconnected spot so reject
      WiFiClient clients = server.available();
      clients.stop();
    }
    else
    {
      // This LED Blinks If No Clients Where Available
      digitalWrite(LED0, HIGH);
      delay(250);
      digitalWrite(LED0, LOW);
      delay(250);
    }
  }

//====================================================================================

  void AvailableMessage()
  {
    //check clients for data
    for(uint8_t i = 0; i < MAXSC; i++)
    {
      if (clients && clients.connected() && clients.available())
      {
          while(clients.available())
          {
            String Message = clients.readStringUntil('\r');
            clients.flush();
            Serial.println("Client No " + String(i) + " - " + Message);
          }
      }
    }
  }

//====================================================================================
