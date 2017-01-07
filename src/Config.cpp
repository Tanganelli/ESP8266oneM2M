//
// Created by Giacomo Tanganelli on 04/01/17.
//
#include <FS.h> //must be the first include
#include "Config.h"
#include <ArduinoJson.h>

int Config::ReadConfig() {
#if DEBUG
    Serial.println("mounting FS...");
#endif
    if (SPIFFS.begin()) {
#if DEBUG
        Serial.println("mounted file system");
#endif
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
#if DEBUG
            Serial.println("reading config file");
#endif
            File configFile = SPIFFS.open(CONFIG_FILE, "r");
            if (configFile) {
#if DEBUG
                Serial.println("opened config file");
#endif
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject &json = jsonBuffer.parseObject(buf.get());
                if (json.success()) {
                    if(json.containsKey("portal_user"))
                        _portal_user = json["portal_user"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("portal_pass"))
                        _portal_pass = json["portal_pass"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("network"))
                        _network = json["network"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("network_password"))
                        _network_password = json["network_password"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("ip"))
                        _cse_ip.fromString(json["ip"].as<String>());
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("port"))
                        _cse_port = json["port"].as<int>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("id"))
                        _cse_id = json["id"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("name"))
                        _cse_name = json["name"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("app"))
                        _app_name = json["app"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("dimmer"))
                        _dimmer_name = json["dimmer"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("switch"))
                        _switch_name = json["switch"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("user"))
                        _user = json["user"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                    if(json.containsKey("pass"))
                        _pass = json["pass"].as<String>();
                    else
                        return PARAMETERS_ERROR;
                } else {
                    return PARSING_FAILED;
                }
            } else {
                return ERROR_OPENING;
            }
        } else {
            return FILE_NOT_FOUND;
        }
    } else {
        return FAILED_MOUNT_FS;
    }
    return 0;
}

const String& Config::get_network() const {
    return _network;
}

const String& Config::get_network_password() const {
    return _network_password;
}

void Config::handleChange(AsyncWebServerRequest *request) {
    //TODO check confirm password
    String page = FPSTR(HTML_HEAD);
    page.replace("{v}", "Config Eagle Dimmer Switch");
    page += FPSTR(HTML_SCRIPT);
    page += FPSTR(HTML_STYLE);
    page += FPSTR(HTML_HEAD_END);
    page += FPSTR(HTML_FORM_CHANGE_START);
    page += addParam("USER:", "user", "user", "user", "200", _portal_user, "text",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += addParam("PASSWORD:", "password", "password", "password", "200", _portal_pass, "password",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += addParam("CONFIRM PASSWORD:", "confirm_password", "confirm_password", "confirm_password", "200", "", "password",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += FPSTR(HTML_FORM_CHANGE_END);
    // page += FPSTR(HTML_SCAN_LINK);

    page += FPSTR(HTML_END);
    request->send(200, "text/html", page);

}
void Config::handleConfig(AsyncWebServerRequest *request) {
    String page = FPSTR(HTML_HEAD);
    page.replace("{v}", "Config Eagle Dimmer Switch");
    page += FPSTR(HTML_SCRIPT);
    page += FPSTR(HTML_STYLE);
    page += FPSTR(HTML_HEAD_END);

    /*WiFi.mode(WIFI_AP_STA);
    delay(100);
    int n = WiFi.scanNetworks();
    if (n == 0) {
        page += F("No networks found. Refresh to scan again.");
    } else {
        //sort networks
        int indices[n];
        for (int i = 0; i < n; i++) {
            indices[i] = i;
        }
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
                    std::swap(indices[i], indices[j]);
                }
            }
        }
        // remove duplicates
        String cssid;
        for (int i = 0; i < n; i++) {
            if (indices[i] == -1) continue;
            cssid = WiFi.SSID(indices[i]);
            for (int j = i + 1; j < n; j++) {
                if (cssid == WiFi.SSID(indices[j])) {
                    indices[j] = -1; // set dup aps to index -1
                }
            }
        }
        //display networks in page
        for (int i = 0; i < n; i++) {
            if (indices[i] == -1) continue; // skip dups

            int quality = WiFi.RSSI(indices[i]);
            String item = FPSTR(HTML_ITEM);
            String rssiQ;
            rssiQ += quality;
            item.replace("{v}", WiFi.SSID(indices[i]));
            item.replace("{r}", rssiQ);
            if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
                item.replace("{i}", "l");
            } else {
                item.replace("{i}", "");
            }
            page += item;
        }
        page += "<br/>";
    }*/

    //TODO validate input
    page += FPSTR(HTML_FORM_CHANGE);
    page += FPSTR(HTML_FORM_START);
    page += addParam("NETWORK:", "network", "network", "network", "200", _network, "text", "Network SSID", "");
    page += addParam("NETWORK PASSWORD:", "network_password", "network_password", "network_password", "200", _network_password, "password", "Network Password", "");
    page += addParam("CSE IP:", "ip", "ip", "cseIP", "15", _cse_ip.toString(), "text", "IP is not valid", "pattern='^(25[0-5]|2[0-4][0-9]|[1][0-9]?[0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$'");
    page += addParam("CSE PORT:", "port", "port", "csePort", "5", String(_cse_port), "text", "Port must be a number", "pattern='[0-9]*'");
    page += addParam("CSE ID:", "id", "id", "cseId", "32", _cse_id, "text", "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += addParam("CSE NAME:", "name", "name", "cseName", "32", _cse_name, "text",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += addParam("CSE USER:", "user", "user", "user", "32", _user, "text",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += addParam("CSE PASSWORD:", "password", "password", "pass", "32", _pass, "password",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
    page += addParam("APPLICATION NAME:", "app", "app", "appName", "32", _app_name, "text",  "Only characters, numbers, _ and - are allowed", "pattern='[A-Za-z0-9_-]*'");
   /* page += addParam("DIMMER CONTAINER NAME:", "dimmer", "dimmer", "dimmerName", "32", _dimmer_name, "text");
    page += addParam("SWITCH CONTAINER NAME:", "switch", "switch", "switchName", "32", _switch_name, "text");*/

    page += FPSTR(HTML_FORM_END);
   // page += FPSTR(HTML_SCAN_LINK);

    page += FPSTR(HTML_FORM_RESET);
    page += FPSTR(HTML_END);
    request->send(200, "text/html", page);
}

String Config::addParam(String label, String i, String n, String p, String l, String v, String t, String title, String pattern) {
    String pitem = FPSTR(HTML_FORM_PARAM);
    pitem.replace("{label}", label);
    pitem.replace("{i}", i);
    pitem.replace("{n}", n);
    pitem.replace("{p}", p);
    pitem.replace("{l}", l);
    pitem.replace("{v}", v);
    pitem.replace("{t}", t);
    pitem.replace("{title}", title);
    pitem.replace("{pattern}", pattern);
    return pitem;
}

void Config::handleChangeSave(AsyncWebServerRequest *request) {
    if(request->hasParam("user")){
        AsyncWebParameter* p = request->getParam("user");
        _portal_user = p->value();
    }
    if(request->hasParam("password")){
        AsyncWebParameter* p = request->getParam("password");
        _portal_pass = p->value();
    }
    int check = WriteConfig();
    String page = FPSTR(HTML_HEAD);
    page.replace("{v}", "Config ESP");
    //page += FPSTR(HTML_SCRIPT);
    page += FPSTR(HTML_STYLE);
    page += FPSTR(HTML_HEAD_END);
    if(check == 0)
        page += FPSTR(HTML_SAVED);
    else
        page += FPSTR(HTML_SAVED_ERROR);
    page += FPSTR(HTML_END);
    request->send(200, "text/html", page);
}
void Config::handleSave(AsyncWebServerRequest *request) {
    if(request->hasParam("network")){
        AsyncWebParameter* p = request->getParam("network");
        _network = p->value();
    }
    if(request->hasParam("network_password")){
        AsyncWebParameter* p = request->getParam("network_password");
        _network_password = p->value();
    }
    if(request->hasParam("ip")){
        AsyncWebParameter* p = request->getParam("ip");
        _cse_ip.fromString(p->value());
    }
    if(request->hasParam("port")){
        AsyncWebParameter* p = request->getParam("port");
        _cse_port = p->value().toInt();
    }

    if(request->hasParam("id")){
        AsyncWebParameter* p = request->getParam("id");
        _cse_id = p->value();
    }

    if(request->hasParam("name")){
        AsyncWebParameter* p = request->getParam("name");
        _cse_name = p->value();
    }

    if(request->hasParam("user")){
        AsyncWebParameter* p = request->getParam("user");
        _user = p->value();
    }

    if(request->hasParam("password")){
        AsyncWebParameter* p = request->getParam("password");
        _pass = p->value();
    }

    /*if(request->hasParam("dimmer")){
        AsyncWebParameter* p = request->getParam("dimmer");
        _dimmer_name = p->value();
    }

    if(request->hasParam("switch")){
        AsyncWebParameter* p = request->getParam("switch");
        _switch_name = p->value();
    }*/

    if(request->hasParam("app")){
        AsyncWebParameter* p = request->getParam("app");
        _app_name = p->value();
    }

    int check = WriteConfig();
    String page = FPSTR(HTML_HEAD);
    page.replace("{v}", "Config ESP");
    //page += FPSTR(HTML_SCRIPT);
    page += FPSTR(HTML_STYLE);
    page += FPSTR(HTML_HEAD_END);
    if(check == 0)
        page += FPSTR(HTML_SAVED);
    else
        page += FPSTR(HTML_SAVED_ERROR);
    page += FPSTR(HTML_END);
    request->send(200, "text/html", page);
}

void Config::handleReset(AsyncWebServerRequest *request) {
    String page = FPSTR(HTML_HEAD);
    page.replace("{v}", "Config ESP");
    page += FPSTR(HTML_SCRIPT);
    page += FPSTR(HTML_STYLE);
    page += FPSTR(HTML_HEAD_END);
    page += FPSTR(HTML_SAVED_RESET);
    page += FPSTR(HTML_END);
    request->send(200, "text/html", page);
    DelConfig();
}

int Config::WriteConfig() {
    if (SPIFFS.begin()) {
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.createObject();
        json["ip"] = _cse_ip.toString();
        json["port"] = String(_cse_port);
        json["id"] = _cse_id;
        json["name"] = _cse_name;
        json["user"] = _user;
        json["pass"] = _pass;
        json["dimmer"] = _dimmer_name;
        json["switch"] = _switch_name;
        json["app"] = _app_name;
        json["network"] = _network;
        json["network_password"] = _network_password;
        json["portal_user"] = _portal_user;
        json["portal_pass"] = _portal_pass;

        File configFile = SPIFFS.open(CONFIG_FILE, "w");
        if (!configFile) {
            return ERROR_OPENING;
        }
        json.printTo(configFile);
        configFile.close();
    } else
        return FAILED_MOUNT_FS;
    return 0;
}

int Config::DelConfig() {
    if (SPIFFS.begin()) {
        SPIFFS.remove(CONFIG_FILE);
    } else
        return FAILED_MOUNT_FS;
    return 0;
}
