#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "sssid";
const char* password = "pass";
const char* mqtt_server = "ip do broker";

const int ledPin = 2; // Pino do LED

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Se a mensagem recebida for "ON", liga o LED, caso contrário, desliga o LED
  if (strcmp((char*)payload, "ON") == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado à rede WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando se conectar ao MQTT Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado");
      client.subscribe("topico/led");
    } else {
      Serial.print("Falha, state: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT); // Define o pino do LED como saída
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
