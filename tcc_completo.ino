#include <Servo.h>                                // Inclui a Biblioteca do servo motor
 
Servo Horizontal;                                 // Inicia o Servo da Horizontal
Servo Vertical;                                   // Inicia o Servo da Vertical
 
int ServoHorizontal = 90;                         // Estabelece valor fixo à ServoHorizontal
int ServoVertical = 90;                           // Estabelece valor fixo à ServoVertical
 
int LimiteServoHorizontalMax = 180;               // Estabele os limites de rotação
int LimiteServoHorizontalMin = 65;                // Estabele os limites de rotação
 
int LimiteServoVerticalMax = 120;                 // Estabele os limites de rotação
int LimiteServoVerticalMin = 15;                  // Estabele os limites de rotação
 
float LDRDC = A0;                                 // Inicia LDRDC no pino A0
float LDRDB = A1;                                 // Inicia LDRDB no pino A1
float LDREC = A2;                                 // Inicia LDREC no pino A2
float LDREB = A3;                                 // Inicia LDREB no pino A3


/*------------------------Divisão-de-Código---------------------*/ 

int bomb=8;                                        // Estabelece a bomba d'água como pino 1
int servo=2;                                       // Estabelece o Servo do slider como pino 2
int blue;                                          // Carrega a variavel do bluetooth
int pot_irrad;                                     // Variavel para a potência calculada da irradiância solar
int volt;                                          // Variavel para carregar a voltagem do módulo
int amp;                                           // Variavel para carregar a corrente do módulo
int pot_multi = volt * amp;                        // Calculo para medir a potência de saída da placa solar

Servo Slider;                                      // Inicia o Servo slider

/*------------------------Divisão-de-Código---------------------*/

void setup()  {
  Horizontal.attach(3);                             // Inicia servo Horizontal na porta D3
  Vertical.attach(5);                               // Inicia servo Vertical na porta D5
  pinMode(blue, OUTPUT);                            // Define o bluetooth como pino 13 
  Horizontal.write(180);                            // Inicia servo Horizontal na posição 180
  Vertical.write(45);                               // Inicia servo Vertical na posição 45
  delay(3000);                                      // Aguarda 3 segundos

/*------------------------Divisão-de-Código---------------------*/

  pinMode(servo, OUTPUT);                            // Defini o pino 2 como saída
  Slider.attach(servo);                              // Defini o pino 2 para o slider          
  Slider.write(0);                                   // Inicia o servo do slider na posição 0
  digitalWrite(bomb, LOW);                           // Seta o pino da bomba d'água como estado lógico 0
  digitalWrite(bomb, OUTPUT);                        // Seta o pino da bomba d'água como saída
  pinMode(A0, INPUT);                                // Seta a entrada analógica A0 como entrada de dados (Voltimetro)
  pinMode(A1, INPUT);                                // Seta a entrada analógica A1 como entrada de dados (Amperimetro)
  Serial.begin(9600);                                // Inicia a serial
  blue=Serial.read();                                // Defini a variavel blue como igual a leitura da serial

}
 
void loop() {

  if (blue == "L") {                                 // Condições booleanas referente ao botão do aplicativo
    Horizontal.write(0);                             // Seta o servo horizontal para a posição 0°
    Vertical.write(45);                              // Seta o servo vertical para a posição 45°    
    digitalWrite(bomb, HIGH);                        // Seta o pino da bomba d'água como estado lógico 1
    Slider.write(180);                               // Seta o servo para a posição 360°
    delay(5000);                                     // Delay de 5 segundos para executar a próxima ação
    Slider.write(0);                                 // Seta o servo para a posição 0°
    delay(5000);                                     // Delay de 5 segundos para executar a próxima ação
    digitalWrite(bomb, LOW);                         // Seta o pino da bomba d'água como estado lógico 0             
  }

  if (blue < 100){                                   // Condições booleanas referente ao parametro de notificação do aplicativo
    if (pot_multi*(blue/100) <= pot_irrad){
    Serial.print("?");                               // Saída da mensagem no celular
    }
  }

  if (blue > 100){                                   // Condições booleanas referente ao parametro de limpeza automatica sem acionamento
    blue - 100;
    if (pot_multi*(blue/100) <= pot_irrad){
    Horizontal.write(0);                             // Seta o servo horizontal para a posição 0°
    Vertical.write(45);                              // Seta o servo vertical para a posição 45°  
    digitalWrite(bomb, HIGH);                        // Seta o pino da bomba d'água como estado lógico 1
    Slider.write(360);                               // Seta o servo para a posição 360
    delay(5000);                                     // Delay de 5 segundos para executar a próxima ação
    Slider.write(0);                                 // Seta o servo para a posição 0
    digitalWrite(bomb, LOW);                         // Seta o pino da bomba d'água como estado lógico 0
    }             
  }


/*------------------------Divisão-de-Código---------------------*/

  int LDC = analogRead(LDRDC);                       // Leitura Analógica do LDR Direita Cima
  int LEC = analogRead(LDREC);                       // Leitura Analógica do LDR Esquerda Cima
  int LDB = analogRead(LDRDB);                       // Leitura Analógica do LDR Direita Baixo
  int LEB = analogRead(LDREB);                       // Leitura Analógica do LDR Esquerda Baixo
  
  int tol = 50;                                      // Inicia uma variavel no valor 50 para utilizar como parametros para o algoritimo do movimento da placa solar
 
  int ValorSup = (LDC + LEC) / 2;                    // Média da leitura dos LDR superior
  int ValorInf = (LDB + LEB) / 2;                    // Média da leitura dos LDR inferior
  int ValorDir = (LDC + LDB) / 2;                    // Média da leitura dos LDR da direita
  int ValorEsq = (LEC + LEB) / 2;                    // Média da leitura dos LDR da esquerda
 
  int DifSupInf = ValorSup - ValorInf;               // Diferença entre LED superior e inferior
  int DifDirEsq = ValorDir - ValorEsq;               // Diferença entre LED direita e esquerda
 
/*------------------------Divisão-de-Código---------------------*/
 

  if (-1 * tol > DifSupInf || DifSupInf > tol)  {    // Algoritimo para os movimentos do Servo Vertical
    if (ValorSup > ValorInf)  {
      ServoVertical = ++ServoVertical;
      if (ServoVertical > LimiteServoVerticalMax)  {
        ServoVertical = LimiteServoVerticalMax;
      }
    }
    else if (ValorSup < ValorInf)  {
      ServoVertical = --ServoVertical;
      if (ServoVertical < LimiteServoVerticalMin)  {
        ServoVertical = LimiteServoVerticalMin;
      }
    }
    Vertical.write(ServoVertical);
  }
 

  if (-1 * tol > DifDirEsq || DifDirEsq > tol) {      // Algoritimo para os movimentos do Servo Horizontal
    if (ValorDir > ValorEsq)  {
      ServoHorizontal = --ServoHorizontal;
      if (ServoHorizontal < LimiteServoHorizontalMin)  {
        ServoHorizontal = LimiteServoHorizontalMin;
      }
    }
    else if (ValorDir < ValorEsq)  {
      ServoHorizontal = ++ServoHorizontal;
      if (ServoHorizontal > LimiteServoHorizontalMax)  {
        ServoHorizontal = LimiteServoHorizontalMax;
      }
    }
    else if (ValorDir = ValorEsq)  {
    }
    Horizontal.write(ServoHorizontal);
  }
 
  delay(100);   // Aguarda 0,1 segundo
}