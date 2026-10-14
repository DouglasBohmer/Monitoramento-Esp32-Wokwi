/*
 * Desafio 16 - Monitoramento de uma sala de servidores
 * ESP32 DevKit V1 + DHT22 + LEDs + Wi-Fi no Wokwi.
 * O monitor serial representa a telemetria local deste desafio.
 */

#include <DHTesp.h>
#include <WiFi.h>
#include <math.h>

// GPIOs: estes numeros devem coincidir com as ligacoes do diagram.json.
const int PINO_DHT = 15;
const int LED_VERDE = 18;
const int LED_AMARELO = 19;
const int LED_VERMELHO = 21;

const unsigned long INTERVALO_LEITURA_MS = 2000;
const char* DISPOSITIVO = "ESP32_SALA_01";
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_SENHA = "";

DHTesp dht;
unsigned long ultimaLeitura = 0;
bool wifiEstavaConectado = false;

// Desliga todos antes de escolher um: impede dois LEDs acesos juntos.
void desligarLeds() {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}

// A conexao ocorre em paralelo; o sensor nao depende do Wi-Fi.
void verificarWifi() {
  bool conectado = WiFi.status() == WL_CONNECTED;
  if (conectado && !wifiEstavaConectado) {
    Serial.println("Wi-Fi conectado a Wokwi-GUEST!");
    Serial.print("Dispositivo: ");
    Serial.println(DISPOSITIVO);
    Serial.print("Endereco IP: ");
    Serial.println(WiFi.localIP());
  } else if (!conectado && wifiEstavaConectado) {
    Serial.println("Wi-Fi desconectado. Monitoramento local continua.");
  }
  wifiEstavaConectado = conectado;
}

// Le o sensor, aplica as faixas do enunciado e imprime a telemetria.
void atualizarMonitoramento() {
  float temperatura = dht.getTemperature();
  const char* status;
  const char* ledAtivo;
  desligarLeds();

  // Uma falha nao pode ser apresentada como temperatura normal ou critica.
  if (isnan(temperatura)) {
    status = "ERRO_SENSOR";
    ledAtivo = "NENHUM";
  } else if (temperatura < 25.0) {
    status = "NORMAL";
    ledAtivo = "VERDE";
    digitalWrite(LED_VERDE, HIGH);
  } else if (temperatura <= 30.0) {
    // O primeiro if ja excluiu os valores abaixo de 25 C.
    status = "ATENCAO";
    ledAtivo = "AMARELO";
    digitalWrite(LED_AMARELO, HIGH);
  } else {
    status = "CRITICO";
    ledAtivo = "VERMELHO";
    digitalWrite(LED_VERMELHO, HIGH);
  }

  Serial.println("\nSISTEMA DE MONITORAMENTO IoT");
  Serial.print("Dispositivo: ");
  Serial.println(DISPOSITIVO);
  Serial.print("Temperatura: ");
  if (isnan(temperatura)) {
    Serial.println("LEITURA INVALIDA - confira o DHT22");
  } else {
    Serial.print(temperatura, 2);
    Serial.println(" C");
  }
  Serial.print("Status: ");
  Serial.println(status);
  Serial.print("LED Ativo: ");
  Serial.println(ledAtivo);
  Serial.print("Wi-Fi: ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "CONECTADO" : "CONECTANDO/SEM CONEXAO");
  Serial.print("Tempo: ");
  Serial.print(millis());
  Serial.println(" ms");
  Serial.println("----------------------------------------");
}

// Executada uma vez ao iniciar ou reiniciar a simulacao.
void setup() {
  Serial.begin(115200);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  desligarLeds();
  dht.setup(PINO_DHT, DHTesp::DHT22);

  Serial.println("SISTEMA DE MONITORAMENTO IoT - INICIANDO");
  Serial.print("Dispositivo: ");
  Serial.println(DISPOSITIVO);
  Serial.println("Conectando ao Wi-Fi Wokwi-GUEST (sem senha)...");
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_SENHA, 6);
  ultimaLeitura = millis();
}

// Repete continuamente. millis() mede o intervalo sem bloquear o Wi-Fi.
void loop() {
  verificarWifi();
  unsigned long agora = millis();
  if (agora - ultimaLeitura >= INTERVALO_LEITURA_MS) {
    ultimaLeitura = agora;
    atualizarMonitoramento();
  }
  delay(10); // Pequena pausa para o simulador; nao define a cadencia do sensor.
}
