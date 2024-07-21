const int TRIG = 3;
const int ECHO = 2;
const int BUZZER = 10; // Pino do buzzer
const int LED_RED = 4;   // Pino do LED vermelho
const int LED_GREEN = 5; // Pino do LED verde
const int LED_BLUE = 6;  // Pino do LED azul
const int MOTOR_PWM = 9; // Pino PWM para controle de velocidade do motor
const int MOTOR_DIR = 8; // Pino de direção do motor

const int DISTANCIA_INICIO_BARULHO = 50; // Distância em cm para começar a fazer barulho
const int DISTANCIA_MAXIMA = 51; // Distância máxima para o buzzer fazer barulho
const int DISTANCIA_CRITICA = 5; // Distância crítica em cm
const int FREQUENCIA_MAXIMA = 2000; // Frequência máxima do buzzer em Hz
const int FREQUENCIA_MINIMA = 500;  // Frequência mínima do buzzer em Hz
const int FREQUENCIA_CRITICA = 3000; // Frequência crítica do buzzer em Hz

// Defina o brilho máximo e mínimo
const int BRILHO_MAXIMO = 255;
const int BRILHO_MINIMO = 50; // Ajuste o brilho mínimo conforme necessário

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT); // Configura o pino do buzzer como saída

  // Configura os pinos do LED RGB como saída
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  // Configura o pino do motor como saída
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
}

void loop() {
  int distancia = sensor_morcego(TRIG, ECHO);
  
  // Calcula o brilho com base na distância
  int brilho = map(distancia, 0, DISTANCIA_INICIO_BARULHO, BRILHO_MAXIMO, BRILHO_MINIMO);
  brilho = constrain(brilho, BRILHO_MINIMO, BRILHO_MAXIMO);

  if (distancia <= DISTANCIA_MAXIMA) {
    Serial.print("Com obstaculo: ");
    Serial.print(distancia);
    Serial.println("cm");

    // Controle do buzzer e LEDs com base na distância
    if (distancia <= DISTANCIA_CRITICA) {
      // Distância crítica: buzzer em frequência mais alta e LED vermelho piscante
      tone(BUZZER, FREQUENCIA_CRITICA); // Frequência crítica para alarme
      piscaLED(255, 0, 0); // Vermelho piscante
      // Controle da velocidade do motor com base na distância crítica
      analogWrite(MOTOR_PWM, 0); // Desliga o motor
    } else {
      // Distância dentro da faixa de barulho
      if (distancia <= DISTANCIA_INICIO_BARULHO) {
        int frequencia = map(distancia, 0, DISTANCIA_INICIO_BARULHO, FREQUENCIA_MAXIMA, FREQUENCIA_MINIMA);
        frequencia = constrain(frequencia, FREQUENCIA_MINIMA, FREQUENCIA_MAXIMA);
        tone(BUZZER, frequencia); // Aciona o buzzer com a frequência calculada
      } else {
        noTone(BUZZER); // Desliga o buzzer se a distância for maior que a faixa de barulho
      }

      // Controle do LED RGB com base na distância e brilho
      if (distancia <= 10) {
        setColor(brilho, 0, 0);   // Vermelho com brilho ajustado
      } else if (distancia <= 20) {
        setColor(0, brilho, 0);   // Verde com brilho ajustado
      } else if (distancia <= 30) {
        setColor(0, 0, brilho);   // Azul com brilho ajustado
      } else if (distancia <= 50) {
        setColor(brilho, brilho, 0); // Amarelo com brilho ajustado
      } else {
        setColor(brilho, brilho, brilho); // Branco com brilho ajustado
      }

      // Controle da velocidade do motor com base na distância
      int velocidade = map(distancia, DISTANCIA_CRITICA, DISTANCIA_INICIO_BARULHO, 0, 255);
      velocidade = constrain(velocidade, 0, 255);
      analogWrite(MOTOR_PWM, velocidade); // Ajusta a velocidade do motor
    }
  } else {
    Serial.print("Sem obstaculo: ");
    Serial.print(distancia);
    Serial.println("cm");
    noTone(BUZZER); // Desliga o buzzer se não houver obstáculo

    // Desliga o LED RGB se não houver obstáculo
    setColor(0, 0, 0);
    
    // Desliga o motor
    analogWrite(MOTOR_PWM, 0);
  }
  
  delay(100);
}

int sensor_morcego(int pinotrig, int pinoecho) {
  digitalWrite(pinotrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinotrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig, LOW);

  return pulseIn(pinoecho, HIGH) / 58;
}

// Função para definir a cor do LED RGB
void setColor(int red, int green, int blue) {
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

// Função para piscar o LED RGB
void piscaLED(int red, int green, int blue) {
  for (int i = 0; i < 5; i++) {
    setColor(red, green, blue);
    delay(200);
    setColor(0, 0, 0); // Desliga o LED
    delay(200);
  }
}
