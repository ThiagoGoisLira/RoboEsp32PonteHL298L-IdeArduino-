# 🤖 Carro Robô Wi-Fi (ESP32 + L298N)



Este projeto implementa o controle de um carro robô através de uma interface web minimalista, utilizando um **ESP32** operando como **Access Point (AP)** e uma ponte H **L298N** para o acionamento dos motores DC.

O ESP32 cria uma rede Wi-Fi própria, permitindo que qualquer dispositivo (smartphone, tablet, PC) se conecte e acesse a página de controle.

---

## ✨ Funcionalidades

* **Modo Access Point (AP):** O ESP32 cria sua própria rede Wi-Fi (`CarroRobor`).
* **Controle Web:** Interface HTML/CSS/JavaScript responsiva acessível por IP ( `192.168.4.1`).
* **Controle Intuitivo:** Os comandos de movimento (FRENTE, TRÁS, ESQUERDA, DIREITA) são enviados através de requisições HTTP **GET** ao pressionar/segurar os botões.
* **Parada Automática:** O movimento cessa automaticamente quando o botão é solto (implementado com `mousedown`/`mouseup` e `touchstart`/`touchend` no JavaScript).
* **Controle Motor DC:** Utiliza a ponte H L298N para controlar dois motores independentes.

---

## 🛠️ Hardware Necessário

* **Placa de Desenvolvimento:** ESP32 Dev Module
* **Driver de Motor:** Módulo Ponte H **L298N**
* **Chassi:** Plataforma de Carro Robô (2 ou 4 rodas)
* **Motores:** 2 x Motores DC (com caixa de redução)
* **Fonte de Alimentação:** Bateria para alimentar os motores (12V recomendado) e o ESP32 (ou regulador de tensão adequado).
* **Cabos Jumper**

---

## 📌 Esquema de Conexão (Wiring)

O código utiliza 4 pinos digitais do ESP32 para controlar as 4 entradas lógicas da ponte H L298N.

| Função do Motor | Módulo L298N | Pino ESP32 | Variável no Código |
| :-------------- | :----------: | :--------: | :----------------: |
| Motor A - IN1   | IN1          | **GPIO 4** | `pin_1`            |
| Motor A - IN2   | IN2          | **GPIO 16**| `pin_2`            |
| Motor B - IN3   | IN3          | **GPIO 17**| `pin_3`            |
| Motor B - IN4   | IN4          | **GPIO 5** | `pin_4`            |

**Observação:**
* Conecte as saídas **OUT1/OUT2** do L298N ao **Motor A** (Esquerdo).
* Conecte as saídas **OUT3/OUT4** do L298N ao **Motor B** (Direito).
* Lembre-se de conectar o **GND** (Terra) do ESP32 ao **GND** do L298N para ter uma referência comum.
* Conecte a alimentação dos motores (VCC para L298N) à sua fonte de alimentação externa.

---

## ⚙️ Configuração (Setup)

### 1. Requisitos de Software

* **IDE do Arduino:** Certifique-se de ter o ambiente IDE do Arduino instalado.
* **Suporte ESP32:** Instale o pacote de placas ESP32 no Gerenciador de Placas da IDE do Arduino.
* **Bibliotecas:** As bibliotecas usadas (`WiFi.h` e `WebServer.h`) já vêm incluídas na instalação do pacote ESP32.

### 2. Parâmetros de Rede

Os parâmetros do Access Point estão definidos no início do código. Você pode alterá-los, se necessário.

```cpp
// --- CONFIGURAÇÕES DO ACCESS POINT (AP) --- 
const char* ssid = "NomeWifiAqui";
const char* password = "senhaAqui";
const IPAddress local_IP(192, 168, 4, 1);