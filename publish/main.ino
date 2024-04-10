#include <WiFi.h>
#include <PubSubClient.h>

// Definições de rede
const char* ssid = "ssid";
const char* password = "pass";
const char* mqtt_server = "ip do broker";

WiFiClient espClient;
PubSubClient client(espClient);

// Função de callback para mensagens recebidas
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup_wifi() {
  delay(10);
  // Conexão WiFi
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
  // Loop até que estejamos reconectados
  while (!client.connected()) {
    Serial.print("Tentando se conectar ao MQTT Broker...");
    // Tentativa de conexão MQTT
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado");
      // Assine os tópicos que deseja
      client.subscribe("topico/teste");
    } else {
      Serial.print("Falha, state: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      // Aguarda 5 segundos antes de tentar novamente
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Publica uma mensagem a cada 5 segundos
  client.publish("topico/teste", "Hello from ESP32");
  delay(5000);
}
