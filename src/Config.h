//
// Created by Giacomo Tanganelli on 04/01/17.
//

#ifndef ESP8266ONEM2M_CONFIG_H
#define ESP8266ONEM2M_CONFIG_H

#include <Arduino.h>
#include <IPAddress.h>
#include <ESPAsyncWebServer.h>
#include <memory>

extern "C" {
#include "user_interface.h"
}

const char HTML_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTML_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTML_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTML_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTML_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTML_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTML_FORM_START[] PROGMEM      = "<form method='get' action='configSave'>";
const char HTML_FORM_CHANGE_START[] PROGMEM      = "<form method='get' action='changeSave'>";
const char HTML_FORM_CHANGE_END[] PROGMEM        = "<button type='submit'>save</button></form>";
const char HTML_FORM_PARAM[] PROGMEM      = "<div class=\"form-group\"><label>{label}</label><input type='{t}' class=\"form-control\" id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}'></div>";
const char HTML_FORM_END[] PROGMEM        = "<button type='submit'>save</button></form>";
const char HTML_FORM_RESET[] PROGMEM      = "<form method='get' action='reset'><br/><button type='submit'>Reset Settings</button></form>";
const char HTML_FORM_CHANGE[] PROGMEM      = "<form method='get' action='change'><br/><button type='submit'>Change Config Credentials</button></form>";
const char HTML_SAVED[] PROGMEM           = "<div>Configuration saved<br />Check your CSE</div>";
const char HTML_SAVED_ERROR[] PROGMEM           = "<div>Error in the configuration provided.<br />Please try again</div>";
const char HTML_SAVED_RESET[] PROGMEM           = "<div>Reset started<br />Connect to the new network.</div>";
const char HTML_END[] PROGMEM             = "</div></body></html>";

#define CONFIG_FILE "/config.json"
#define FAILED_MOUNT_FS 1
#define FILE_NOT_FOUND 2
#define ERROR_OPENING 3
#define PARSING_FAILED 4
#define PARAMETERS_ERROR 5

class Config {
private:
    String _network = "";
    String _network_password = "";
    IPAddress _cse_ip;
    int _cse_port = 0;
    String _cse_name = "";
    String _cse_id = "";
    String _app_name = "EagleDimmerSwitch";
    String _dimmer_name = "Dimmer";
    String _switch_name = "Switch";
    String _user = "admin";
    String _pass = "admin";
    String _portal_user = "admin";
    String _portal_pass = "password";

    int WriteConfig();

    String addParam(String label, String i, String n, String p, String l, String v, String t);

public:

    int ReadConfig();

    int DelConfig();

    const String &get_network() const;

    const String &get_network_password() const;

    void handleConfig(AsyncWebServerRequest *request);

    void handleSave(AsyncWebServerRequest *request);

    void handleReset(AsyncWebServerRequest *request);

    void handleChange(AsyncWebServerRequest *request);

    void handleChangeSave(AsyncWebServerRequest *request);

    const IPAddress &get_cse_ip() const {
        return _cse_ip;
    }

    int get_cse_port() const {
        return _cse_port;
    }

    const String &get_cse_name() const {
        return _cse_name;
    }

    const String &get_cse_id() const {
        return _cse_id;
    }

    const String &get_dimmer_name() const {
        return _dimmer_name;
    }

    const String &get_switch_name() const {
        return _switch_name;
    }

    const String &get_user() const {
        return _user;
    }

    const String &get_pass() const {
        return _pass;
    }

    const String &get_app_name() const {
        return _app_name;
    }

    const String &get_portal_user() const {
        return _portal_user;
    }

    const String &get_portal_pass() const {
        return _portal_pass;
    }

};


#endif //ESP8266ONEM2M_CONFIG_H
