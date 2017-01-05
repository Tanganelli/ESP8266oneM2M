//
// Created by Giacomo Tanganelli on 04/01/17.
//

#include <FS.h> //must be the first include
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "Config.h"
#include "oneM2MClient.h"

#define NOTIFICATION_PORT 80

Config config;
std::unique_ptr<AsyncWebServer> WebServer;
String body;
String Dimmer_rn;
String Switch_rn;

int DimmerValue = -1;
int SwitchState = -1;

void cleanSerial(){
    while(Serial.available() > 0) {
        Serial.read();
    }
}

int parsePayloadDimmer(String payload) {
    String pattern = "<int name=\"data\" val=\"";
    int start = payload.indexOf(pattern);
    int end = payload.indexOf("\"/>", (unsigned int) start);
    String val = payload.substring(start+pattern.length(), (unsigned int) end);
#if DEBUG
    Serial.print("newValString:");
    Serial.println(val);
    Serial.print("newValInt:");
    Serial.println(val.toInt());
#endif
    return val.toInt();
}

int parsePayloadSwitch(String payload) {
    String pattern = "<int name=\"data\" val=\"";
    int start = payload.indexOf(pattern);
    int end = payload.indexOf("\"/>", (unsigned int) start);
    String val = payload.substring(start+pattern.length(), (unsigned int) end);
    if(val.compareTo("off")==0)
        return 0;
    return 1;
}

int handleNotificationDimmer(String payload){
#if DEBUG
    Serial.print("Dimmer\n");
#endif
    DynamicJsonBuffer  jsonBufferPayload;
    JsonObject& json = jsonBufferPayload.parseObject(payload);
    if (!json.success()) return 1;
    if (!json.containsKey("m2m:sgn")) return 1;
    JsonObject& jsonsgn = json["m2m:sgn"];
    if (!jsonsgn.containsKey("nev")) return 1;
    JsonObject& jsonnev = jsonsgn["nev"];
    if (!jsonnev.containsKey("rep")) return 1;
    JsonObject& jsonrep = jsonsgn["rep"];
    if (!jsonrep.containsKey("rn")) return 1;
    if (!jsonrep.containsKey("con")) return 1;
    String rn = json["m2m:sgn"]["nev"]["rep"]["rn"].asString();

    if(rn.compareTo(Dimmer_rn)!=0){
        int newVal = parsePayloadDimmer(json["m2m:sgn"]["nev"]["rep"]["con"].asString());
        Dimmer_rn = rn;
#if DEBUG
        Serial.print(json["m2m:sgn"]["nev"]["rep"]["con"].asString());
#endif
        cleanSerial();
        Serial.print("update+dimmer\n");
        Serial.print(newVal);
        Serial.flush();
    }
    return 0;
}

int handleNotificationSwitch(String payload) {
#if DEBUG
    Serial.print("Switch\n");
#endif
    DynamicJsonBuffer  jsonBufferPayload;
    JsonObject& json = jsonBufferPayload.parseObject(payload);
    if (!json.success()) return 1;
    if (!json.containsKey("m2m:sgn")) return 1;
    JsonObject& jsonsgn = json["m2m:sgn"];
    if (!jsonsgn.containsKey("nev")) return 1;
    JsonObject& jsonnev = jsonsgn["nev"];
    if (!jsonnev.containsKey("rep")) return 1;
    JsonObject& jsonrep = jsonsgn["rep"];
    if (!jsonrep.containsKey("rn")) return 1;
    if (!jsonrep.containsKey("con")) return 1;
    String rn = json["m2m:sgn"]["nev"]["rep"]["rn"].asString();
    if(rn.compareTo(Switch_rn)!=0){
        Switch_rn = rn;
        int newVal = parsePayloadSwitch(json["m2m:sgn"]["nev"]["rep"]["con"].asString());
#if DEBUG
        Serial.print(json["m2m:sgn"]["nev"]["rep"]["con"].asString());
#endif
        cleanSerial();
        Serial.print("update+switch\n");
        Serial.print(newVal);
        Serial.flush();
    }
    return 0;
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
#if DEBUG
    Serial.println("onBody");
#endif
    if(request->method() == HTTP_POST){
        if(request->url().compareTo("/notificationSwitch")==0 || request->url().compareTo("/notificationDimmer")==0){
            if(!index){ // first chunk
                body = String((const char *) data);
#if DEBUG
                Serial.printf("BodyStart: %u\n", total);
                Serial.printf("%s", (const char*)data);
#endif
            }
            else {
                body = body + String((const char *) data);
#if DEBUG
                Serial.printf("%s", (const char*)data);
#endif
            }
            if(index + len == total){ //last chunk
                String complete = body;
#if DEBUG
                Serial.printf("BodyEnd: %u\n", total);
                Serial.println(complete);
#endif
                int ret;
                if(request->url().compareTo("/notificationSwitch")==0)
                    ret = handleNotificationSwitch(complete);
                else
                    ret = handleNotificationDimmer(complete);
                if(ret == 0)
                    request->send(201);
                else
                    request->send(503);
            }
        }
    }
}

void handleConfig(AsyncWebServerRequest *request){
    config.handleConfig(request);
}

void handleSave(AsyncWebServerRequest *request){
    config.handleSave(request);
}

void handleReset(AsyncWebServerRequest *request){
    config.handleReset(request);
}

void setup(){
    Serial.begin(115200);
    Serial.println("SETUP!");
    int ret = config.ReadConfig();
    if(ret == PARSING_FAILED){
        Serial.println("Failed to load json config");
    } else if (ret == ERROR_OPENING) {
        Serial.println("Failed to open file");
    } else if (ret == FILE_NOT_FOUND) {
        Serial.println("File does not exist");
    } else if (ret == FAILED_MOUNT_FS) {
        Serial.println("Failed to mount FS");
    } else if (ret == PARAMETERS_ERROR) {
        Serial.println("Failed to parse parameters");
    }
    if(ret != 0){
        // Start AP
        WiFi.mode(WIFI_AP_STA);
        String network = "Eagle" + String(ESP.getChipId());
        WiFi.softAPConfig(IPAddress(192,168,0,1), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
        WiFi.softAP(network.c_str());
        delay(500); // Without delay I've seen the IP address blank
        Serial.print("AP MODE, address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        // Connect to Network
        WiFi.mode(WIFI_STA);
        if (WiFi.status() != WL_CONNECTED) {
            WiFi.begin(config.get_network().c_str(), config.get_network_password().c_str());
            WiFi.waitForConnectResult();
            Serial.print("STA MODE, address: ");
            Serial.println(WiFi.localIP());
        }
    }
    WebServer.reset(new AsyncWebServer(NOTIFICATION_PORT));
    WebServer->on("/", handleConfig);
    WebServer->on("/configSave", handleSave);
    WebServer->on("/reset", handleReset);
    WebServer->onRequestBody(onBody);

    WebServer->begin();

    String notificationDimmerUri("http://" + WiFi.localIP().toString() + ":" + NOTIFICATION_PORT + "/notificationDimmer");
    String notificationSwitchUri("http://" + WiFi.localIP().toString() + ":" + NOTIFICATION_PORT + "/notificationSwitch");

    oneM2MClient client(config.get_user(), config.get_pass(), config.get_cse_ip(), config.get_cse_port());
    int statusCode = client.createAE(config.get_cse_id(), "EAGLE SWITCH DIMMER");

    statusCode = client.createContainer(config.get_cse_id(), config.get_cse_name(), "EAGLE SWITCH DIMMER", config.get_dimmer_name());

    statusCode = client.createContainer(config.get_cse_id(), config.get_cse_name(), "EAGLE SWITCH DIMMER", config.get_switch_name());

    statusCode = client.createSubscription(config.get_cse_id(), config.get_cse_name(), "EAGLE SWITCH DIMMER",
                                           config.get_dimmer_name(), notificationDimmerUri);

    statusCode = client.createSubscription(config.get_cse_id(), config.get_cse_name(), "EAGLE SWITCH DIMMER",
                                           config.get_switch_name(), notificationSwitchUri);
}

void updateDimmer(){
    cleanSerial();
    Serial.print("dimmer\n"); //Ask for dimmer value
    int temp = Serial.parseInt();
    if(temp != DimmerValue) {
        DimmerValue = temp;
#if DEBUG
        Serial.print(value, DEC);
#endif
        String newVal = String(DimmerValue);


        String valueDimmer("&lt;obj&gt;\n"
                                   "&lt;str name=&quot;appId&quot; val=&quot;Dimmer&quot;/&gt;\n"
                                   "        &lt;str name=&quot;category&quot; val=&quot;ligth &quot;/&gt;\n"
                                   "        &lt;int name=&quot;data&quot; val=&quot;" + newVal + "&quot;/&gt;\n"
                                   "        &lt;int name=&quot;unit&quot; val=&quot;percentage&quot;/&gt;\n"
                                   "      &lt;/obj&gt;\n");
        String contentInstanceDimmerResponse;
        oneM2MClient client(config.get_user(), config.get_pass(), config.get_cse_ip(), config.get_cse_port());
        client.createContentInstance(config.get_cse_id(), config.get_cse_name(), "EAGLE SWITCH DIMMER",
                                     config.get_dimmer_name(), valueDimmer,
                              &contentInstanceDimmerResponse);
        DynamicJsonBuffer jsonBufferCinDimmer;
        JsonObject &json = jsonBufferCinDimmer.parseObject(contentInstanceDimmerResponse);
        Dimmer_rn = json["m2m:cin"]["rn"].asString();
#if DEBUG
        Serial.println("DIMMER UPDATE");
#endif
    }
}

void updateSwitch(){
    String newVal = "off";
    cleanSerial();
    Serial.print("switch\n");  //Ask for switch statusDimmer
    int temp = Serial.parseInt();
    if(temp != SwitchState) {
        SwitchState = temp;
#if DEBUG
        Serial.print(lastButtonState, DEC);
#endif
        if (SwitchState == 1)
            newVal = "on";
        String valueSwitch("&lt;obj&gt;\n"
                                   "&lt;str name=&quot;appId&quot; val=&quot;Switch&quot;/&gt;\n"
                                   "        &lt;str name=&quot;category&quot; val=&quot;switch &quot;/&gt;\n"
                                   "        &lt;int name=&quot;data&quot; val=&quot;" + newVal + "&quot;/&gt;\n"
                                   "      &lt;/obj&gt;\n");
        String contentInstanceSwitchResponse;
        oneM2MClient client(config.get_user(), config.get_pass(), config.get_cse_ip(), config.get_cse_port());
        client.createContentInstance(config.get_cse_id(), config.get_cse_name(), "EAGLE SWITCH DIMMER",
                                     config.get_switch_name(), valueSwitch,
                                     &contentInstanceSwitchResponse);
        DynamicJsonBuffer jsonBufferCinSwitch;
        JsonObject &json = jsonBufferCinSwitch.parseObject(contentInstanceSwitchResponse);
        Switch_rn = json["m2m:cin"]["rn"].asString();
#if DEBUG
        Serial.println("SWITCH UPDATE");
#endif
    }
}

void loop(){
#if DEBUG
    Serial.println("LOOP");
#endif
    updateDimmer();
    updateSwitch();
    delay(250);
}