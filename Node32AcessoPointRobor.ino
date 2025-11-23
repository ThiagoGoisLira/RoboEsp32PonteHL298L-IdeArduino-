#include <WiFi.h> 
#include <WebServer.h> 

// --- CONFIGURAÇÕES DO ACCESS POINT (AP) --- 
const char* ssid = "NomeWifiAqui"; 
const char* password = "senhaAqui"; 
const IPAddress local_IP(192, 168, 4, 1); 

// --- PINOS DO MOTOR (Usando const int para otimização) --- 
const int pin_1 = 4;  // Motor A - Entrada 1 (Esquerdo) 
const int pin_2 = 16; // Motor A - Entrada 2 (Esquerdo) 
const int pin_3 = 17; // Motor B - Entrada 1 (Direito) 
const int pin_4 = 5;  // Motor B - Entrada 2 (Direito) 

// --- VARIÁVEIS DE ESTADO --- 
WebServer server(80); 

// --- FUNÇÕES AUXILIARES DE MOTOR --- 

void moveMotorA(int in1, int in2) {
    digitalWrite(pin_1, in1);
    digitalWrite(pin_2, in2);
}

void moveMotorB(int in3, int in4) {
    digitalWrite(pin_3, in3);
    digitalWrite(pin_4, in4);
}

void stopMotors() { 
    moveMotorA(LOW, LOW);
    moveMotorB(LOW, LOW);
} 

// --- FUNÇÃO PRINCIPAL DE CONTROLE --- 

void handleComando(const char* comando) { 
  Serial.print("COMANDO RECEBIDO: "); 
  Serial.println(comando); 
  
  // LÓGICA DE CONTROLE DOS PINOS DO MOTOR (Corrigida e Refatorada)
  if (strcmp(comando, "FRENTE") == 0) { 
    moveMotorA(LOW, HIGH); // pin_1=LOW, pin_2=HIGH
    moveMotorB(HIGH, LOW); // pin_3=HIGH, pin_4=LOW
    
  } else if (strcmp(comando, "TRAS") == 0) { 
    moveMotorA(HIGH, LOW); // pin_1=HIGH, pin_2=LOW
    moveMotorB(LOW, HIGH); // pin_3=LOW, pin_4=HIGH
    
  } else if (strcmp(comando, "ESQUERDA") == 0) { 
    moveMotorA(LOW, HIGH);
    moveMotorB(LOW, HIGH);
    
  } else if (strcmp(comando, "DIREITA") == 0) { 
    moveMotorA(HIGH, LOW);
    moveMotorB(HIGH, LOW);
    
  } else if (strcmp(comando, "PARAR") == 0) { 
    stopMotors(); 
  } 
  
  server.send(200, "text/plain", "OK"); 
} 

void handleFrente()    { handleComando("FRENTE"); } 
void handleTras()      { handleComando("TRAS"); } 
void handleDireita()   { handleComando("DIREITA"); } 
void handleEsquerda()  { handleComando("ESQUERDA"); } 
void handleParar()     { handleComando("PARAR"); } 

// --- FUNÇÃO PARA EXIBIR A PÁGINA DE CONTROLE (HTML/CSS/JS OTIMIZADO) --- 

void handleRoot() { 
  server.sendHeader("Location", "/controle"); 
  server.send(303);  
} 

void handleControle() { 
  // O HTML foi modificado para melhor UX e feedback visual
  String html = R"rawliteral( 
<!DOCTYPE html> 
<html> 
<head> 
  <title>Controle de Robo</title> 
  <meta name="viewport" content="width=device-width, initial-scale=1"> 
  <style> 
    body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; background-color: #f4f4f9; } 
    h1 { color: #333; } 
    h2 { color: #007bff; margin-bottom: 5px; }
    
    .controls { 
      display: grid; 
      grid-template-columns: repeat(3, 1fr); 
      grid-template-rows: repeat(3, 1fr);
      grid-template-areas: 
        ". frente ." 
        "esquerda . direita" 
        ". tras .";
      max-width: 450px; 
      margin: 20px auto; 
      gap: 10px; 
    } 
    
    #btn_frente { grid-area: frente; } 
    #btn_esquerda { grid-area: esquerda; } 
    #btn_direita { grid-area: direita; } 
    #btn_tras { grid-area: tras; } 

    button { 
      padding: 20px 10px; 
      font-size: 18px; 
      width: 100%; 
      border: none; 
      border-radius: 8px; 
      cursor: pointer; 
      color: white; 
      transition: background-color 0.3s, transform 0.1s; 
      background-color: #007bff; 
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    } 
    /* Feedback Visual: Botão Ativo */
    button.active {
        background-color: #28a745; /* Verde quando pressionado */
        transform: scale(0.98); 
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
    }
    #btn_tras { background-color: #28a745; } 
    #btn_esquerda { background-color: #ffc107; color: #333;} 
    #btn_direita { background-color: #dc3545; } 
    
    #status { margin-top: 20px; font-weight: bold; } 
  </style> 
</head> 
<body> 
  <h1>Controle de Robo</h1> 
  <h2>Rede: GL Robor (192.168.4.1)</h2>
  <p>Pressione e **SEGURE** para mover. **SOLTE** para parar.</p> 
  
  <div class="controls"> 
    <button id="btn_frente" data-cmd="/frente">FRENTE</button> 
    <button id="btn_esquerda" data-cmd="/esquerda">ESQUERDA</button> 
    <button id="btn_direita" data-cmd="/direita">DIREITA</button> 
    <button id="btn_tras" data-cmd="/tras">TRAS</button> 
  </div> 
  <div id="status">Status: Pronto.</div> 

  <script> 
    const controlButtons = document.querySelectorAll('.controls button'); 
    let isMoving = false; 

    function sendCommand(path) { 
      fetch(path) 
        .catch(error => console.error('Erro de rede:', error)); 
    } 
    
    function startMove(event) { 
      if (isMoving) return; 
      isMoving = true; 
      const commandPath = event.currentTarget.dataset.cmd; 
      sendCommand(commandPath); 
      
      // Adiciona feedback visual
      event.currentTarget.classList.add('active'); 
      event.preventDefault(); 
    } 
    
    function stopMove() { 
      if (!isMoving) return; 
      isMoving = false; 
      sendCommand("/parar"); 
      
      // Remove feedback visual de TODOS os botões
      controlButtons.forEach(btn => btn.classList.remove('active'));
    } 

    controlButtons.forEach(button => { 
      button.addEventListener('mousedown', startMove); 
      button.addEventListener('mouseup', stopMove); 
      button.addEventListener('touchstart', startMove); 
      button.addEventListener('touchend', stopMove); 
      // Adicionar listener para o caso do mouse sair do botão enquanto pressionado
      button.addEventListener('mouseleave', (event) => {
          if(event.buttons === 1) stopMove(); // Se o botão esquerdo do mouse estiver pressionado
      });
    }); 
  </script> 
</body> 
</html> 
)rawliteral"; 
  
  server.send(200, "text/html", html); 
} 

// --- CONFIGURAÇÃO (SETUP) E LOOP --- 

void setup() { 
  Serial.begin(115200); 
  
  // CONFIGURAÇÃO DOS PINOS DO MOTOR E PARADA INICIAL 
  pinMode(pin_1, OUTPUT); 
  pinMode(pin_2, OUTPUT); 
  pinMode(pin_3, OUTPUT); 
  pinMode(pin_4, OUTPUT); 
  stopMotors(); 

  // >>> MODO ACCESS POINT (AP) <<<
  WiFi.softAPConfig(local_IP, local_IP, IPAddress(255, 255, 255, 0)); 
  bool success = WiFi.softAP(ssid, password);
  
  if (success) {
    Serial.println("\nAccess Point criado com sucesso!");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Acesse o controle em: http://"); 
    Serial.println(WiFi.softAPIP()); 
  } else {
    Serial.println("\nFalha ao iniciar o Access Point!");
  }

  // CONFIGURAÇÃO DE ROTAS 
  server.on("/", handleRoot);            
  server.on("/controle", handleControle); 
  server.on("/frente", handleFrente); 
  server.on("/tras", handleTras); 
  server.on("/direita", handleDireita); 
  server.on("/esquerda", handleEsquerda); 
  server.on("/parar", handleParar); 
  
  server.begin(); 
  Serial.println("Servidor HTTP iniciado."); 
} 

void loop() { 
  server.handleClient(); 
}
