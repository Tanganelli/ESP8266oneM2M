//
// Created by Giacomo Tanganelli on 05/01/17.
//

#include "oneM2MClient.h"

oneM2MClient::oneM2MClient(String user, String password, IPAddress ip, int port): user(user), password(password),
                                                                               ip(ip), port(port) {
}

int oneM2MClient::createAE(String cseid, String appName) {
#if DEBUG
    Serial.print("createAE: ");
    Serial.print(cseid);
    Serial.print(", ");
    Serial.println(appName);
    Serial.print("ip: ");
    Serial.println(ip.toString().c_str());
    Serial.print("port: ");
    Serial.println(port);
#endif
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    String header = "X-M2M-Origin: " + user + ":" + password;
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=2");
    String body = "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\""+appName+"\" >\n"
            "    <api>Dimmer</api>\n"
            "    <lbl>Type/actuator Category/light Location/home</lbl>\n"
            "    <rr>false</rr>\n"
            "</m2m:ae>";
    String cse = "/~/" + cseid;
    int statusCode = client.post(cse.c_str(), body.c_str());
    return statusCode;
}


int oneM2MClient::createAE(String cseid, String appName, String *pString) {
    String header = "X-M2M-Origin: " + user + ":" + password;
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=2");
    String body = "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\""+appName+"\" >\n"
            "    <api>Dimmer</api>\n"
            "    <lbl>Type/actuator Category/light Location/home</lbl>\n"
            "    <rr>false</rr>\n"
            "</m2m:ae>";
    String cse = "/~/" + cseid;
    int statusCode = client.post(cse.c_str(), body.c_str(), pString);
    return statusCode;
}

int oneM2MClient::createContainer(String cseid, String cseName, String appName, String containerName) {
    String header = "X-M2M-Origin: " + user + ":" + password;
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=3");
    String body = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\""+containerName+"\">\n"
            "</m2m:cnt>";

    String cse = "/~/" + cseid + "/" + cseName + "/" + appName;
    int statusCode = client.post(cse.c_str(), body.c_str());
    return statusCode;
}

int oneM2MClient::createContainer(String cseid, String cseName, String appName, String containerName, String *pString) {
    String header = "X-M2M-Origin: " + user + ":" + password;
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=3");
    String body = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\""+containerName+"\">\n"
            "</m2m:cnt>";

    String cse = "/~/" + cseid + "/" + cseName + "/" + appName;
    int statusCode = client.post(cse.c_str(), body.c_str(), pString);
    return statusCode;
}

int oneM2MClient::createContentInstance(String cseid, String cseName, String appName, String containerName,
                                        String value, String *pString) {
    String header = "X-M2M-Origin: " + user + ":" + password;
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=4");
    String body = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\">\n"
                          "    <cnf>message</cnf>\n"
                          "    <con>\n" + value +
                  "    </con>\n"
                          "</m2m:cin>";

    String cse = "/~/" + cseid + "/" + cseName + "/" + appName + "/" + containerName;
    int statusCode = client.post(cse.c_str(), body.c_str(), pString);
    return statusCode;
}

int oneM2MClient::createSubscription(String cseid, String cseName, String appName, String containerName,
                                     String notificationURI) {
    String header = "X-M2M-Origin: " + user + ":" + password;
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=23");
    String body = "<m2m:sub xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"SUB_MY_SENSOR\">\n"
                          "    <nu>"+notificationURI+"</nu>\n"
                          "    <nct>2</nct>\n"
                          "</m2m:sub>";

    String cse = "/~/" + cseid + "/" + cseName + "/" + appName + "/" + containerName;
    int statusCode = client.post(cse.c_str(), body.c_str());
    return statusCode;
}

int oneM2MClient::createSubscription(String cseid, String cseName, String appName, String containerName,
                                     String notificationURI, String *pString) {
    String header = "X-M2M-Origin: " + user + ":" + password;
    char ip_buf[20];
    String ipS = ip.toString();
    ipS.toCharArray(ip_buf,20);
    RestClient client(ip_buf, port);
    client.setHeader(header.c_str());
    client.setHeader("Accept: application/json");
    client.setContentType("application/xml;ty=23");
    String body = "<m2m:sub xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"SUB_MY_SENSOR\">\n"
                          "    <nu>"+notificationURI+"</nu>\n"
                          "    <nct>2</nct>\n"
                          "</m2m:sub>";

    String cse = "/~/" + cseid + "/" + cseName + "/" + appName + "/" + containerName;
    int statusCode = client.post(cse.c_str(), body.c_str(), pString);
    return statusCode;
}
