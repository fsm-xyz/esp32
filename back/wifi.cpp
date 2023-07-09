#include <AccelStepper.h>
#include <WiFi.h>

const char *ssid = "智慧大棚";     /*Enter Your SSID*/
const char *password = "zhdp8888"; /*Enter Your Password*/

WiFiServer server(80); /* Instance of WiFiServer with port number 80 */
String request;

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::FULL2WIRE, 18, 19);

void setup()
{
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi ");
  Serial.println(WiFi.SSID());
  delay(1000);
  server.begin(); /* Start the HTTP web Server */
  Serial.print("Connect to IP Address: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  stepper.setMaxSpeed(1000);
  stepper.setSpeed(900);
}

int Motor_Status;
WiFiClient client;

void html()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>");
  client.println("html { font-family: Roboto; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;");
  client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}");
  client.println(".button_ON {background-color: white; color: black; border: 2px solid #4CAF50;}");
  client.println(".button_OFF {background-color: white; color: black; border: 2px solid #f44336;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h2>Stepper Motor Control using ESP32</h2>");
  client.println("<p>Click to Forward and Reverse the Stepper Motor</p>");

  if (Motor_Status == LOW)
  {
    client.print("<p><a href=\"/For\n\"><button class=\"button button_ON\">Forward</button></a></p>");
  }

  else
  {
    client.print("<p><a href=\"/Rev\n\"><button class=\"button button_OFF\">Reverse</button></a></p>");
  }

  client.println("</body>");
  client.println("</html>");
}
void loop()
{
  stepper.runSpeed();

  client = server.available();
  // if (!client)
  // {
  //   return;
  // }

  if (client.connected())
  {
    if (client.available())
    {
      char c = client.read();
      request += c;

      if (c == '\n')
      {
        if (request.indexOf("GET /For") != -1)
        {
          Serial.println("clockwise");
        }
        else if (request.indexOf("GET /Rev") != -1)
        {
          Serial.println("counterclockwise");
          stepper.runSpeed();
        }
        html();
      }
    }
  }

  request = "";
  client.stop();
}
