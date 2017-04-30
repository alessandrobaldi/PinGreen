#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

// Endereço MAC do Arduino (DEFAULT)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Chave do Xively
char xivelyKey[] = "z1m6tlvDZoT2WnoPIsDtDB6qYRZkrYF5C6ShVLPRNVyn06cQ";

// Strings para UPLOAD
char sensorumidade[] = "umidade";
char sensortemperatura[] = "temperatura";
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorumidade, strlen(sensorumidade), DATASTREAM_FLOAT),
  XivelyDatastream(sensortemperatura, strlen(sensortemperatura), DATASTREAM_FLOAT)
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(1052499700, datastreams, 2 /* number of datastreams */);

EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Iniciando múltiplos uploads para o Xively...");
  Serial.println();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Erro ao tentar obter o DHCP...");
    delay(15000);
  }
    pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
    digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  int valordosensortemperatura = analogRead(A1)/2;
  int valordosensorumidade = analogRead(A5);
  datastreams[0].setFloat(valordosensorumidade);
  datastreams[1].setFloat(valordosensortemperatura);
  Serial.print("Lendo valor dos sensores - Umidade:");
  Serial.print(datastreams[0].getFloat());
  Serial.print(" Temperatura: ");
  Serial.println(datastreams[1].getFloat());

  Serial.println("Fazendo upload para o Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  Serial.print("xivelyclient.put retornou ");
  Serial.println(ret);

  Serial.println();
  delay(15000);
}

