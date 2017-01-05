//
// Created by Giacomo Tanganelli on 05/01/17.
//

#ifndef ESP8266ONEM2M_ONEM2MCLIENT_H
#define ESP8266ONEM2M_ONEM2MCLIENT_H

#include <Arduino.h>
#include <RestClient.h>

class oneM2MClient {
private:
    String user;
    String password;
    IPAddress ip;
    int port;

public:
    oneM2MClient(String user, String password, IPAddress ip, int port);

    int createAE(String cseid, String appName);

    int createAE(String cseid, String appName, String *pString);

    int createContainer(String cseid, String cseName, String appName, String containerName);

    int createContainer(String cseid, String cseName, String appName, String containerName, String *pString);

    int createContentInstance(String cseid, String cseName, String appName, String containerName, String value, String *pString);

    int createSubscription(String cseid, String cseName, String appName, String containerName, String notificationURI);

    int createSubscription(String cseid, String cseName, String appName, String containerName, String notificationURI, String *pString);
};


#endif //ESP8266ONEM2M_ONEM2MCLIENT_H
