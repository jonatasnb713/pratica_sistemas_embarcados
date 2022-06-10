int minutosa  =  0;
int minutosb  =  0;
int segundosa =  0;
int segundosb =  0;
int botao1    = 12;
int botao2    = 13;
bool rodar    =  0;
bool filtro   =  0;
int period    =  0;

unsigned long startTime;  
unsigned long currentTime;

//Matrix com numeros no display de 7 seg
const int numeros_Display[10][8] = {

  {1,1,1,1,1,1,0,0},     // 0
  {0,1,1,0,0,0,0,0},     // 1
  {1,1,0,1,1,0,1,0},     // 2
  {1,1,1,1,0,0,1,0},     // 3
  {0,1,1,0,0,1,1,0},     // 4
  {1,0,1,1,0,1,1,0},     // 5
  {1,0,1,1,1,1,1,0},     // 6
  {1,1,1,0,0,0,0,0},     // 7
  {1,1,1,1,1,1,1,0},     // 8
  {1,1,1,1,0,1,1,0}      // 9        
                                    };

//Função para definir o estado inicial de cada pino, sendo a o pino, b INPUT ou OUTPUT
void estadoInicial(int a, int b){
  if(a >=0 && a <= 7){
    if(b == INPUT){
      DDRD &= ~(1 << a);
    }else if(b == OUTPUT){
     DDRD |= (1 << a);
    }
    }else if(a >= 8 && a <= 13){
      if(b == INPUT){
        DDRB &= ~(1 << a-8);
    }else if(b==OUTPUT){
      DDRB |=  (1 << a-8);      
    }
    }else if(a >= 14 && a <= 19){
      if(b == INPUT){
        DDRC &= ~(1 << a-14);
    }else if(b == OUTPUT){
      DDRC |= (1 << a-14);   
  }}}

//Função para escrever em um pino digital, sendo a o pino, b 0 ou 1
void escritaD(int a, int b){
  if(a >= 0 && a <= 7){
    if(b == 0){
      PORTD &= ~(1 << a);
    }else if(b == 1){
      PORTD |= (1 << a);     
    }
  }else if(a >= 8 && a <= 13){
    if(b == 0){
      PORTB &= ~(1 << a-8);
    }else if(b == 1){
      PORTB |= (1<<a-8);      
    }
  }else if(a >= 14 && a <= 19){
    if(b == 0){
      PORTC &= ~(1 << a-14);
    }else if(b == 1){
      PORTC |=  (1 << a-14);   
    }}}

//Função para ler um pino, sendo a o pino, b o valor que a função retorna, isto é, 0 ou 1
bool leituraD(int a){
  int b = 0;
    if(a >= 0 && a <= 7){
      b = PIND & (1 << a);
    return(b);
      }
      else if(a >= 8 && a <= 13){
      b = PINB & (1 << a-8);
    return(b);
      }
      else if(a >= 14 && a <= 19){
      b = PINC & (1 << a-14);
    return(b);
  }}

int adc = 0; 
int m,c,d,u = 0;

//Função para fazer leitura analógica
int leituraA(){  
  ADCSRA |= (1 << ADSC);
  while((ADCSRA && 0x40) == 0x40);  
  adc = ADCL | (ADCH << 8);
}

//Aqui estão setados os pinos DP dos 4 displays
const int dp[4] = {8,9,10,11};
//Aqui estão setados os pinos que o display esta conectado
const int pinosDisp[8] = {0,1,2,3,4,5,6,7};
//Função para controlar o display, sendo n o numero que se deseja exibir, e d qual display
void controleDisplay(int n, int d){
  for (int i = 0; i <= 7; i++)
  {
    escritaD(pinosDisp[i], numeros_Display[n][i]);    
    if(d == dp[1]){
      escritaD(7, 1);
    }    
  }
  escritaD(d, 1);
  delay(5);
  escritaD(d, 0);
  delay(5);
}

void setup() {
  for(int x = 0; x<=7; x++)
  {
    estadoInicial(pinosDisp[x], OUTPUT);
    if(x < 4)
      estadoInicial(dp[x], OUTPUT);
  }   
  ADMUX = 0x40;
  ADCSRA = 0x87;  
  startTime = millis();  //initial start time 
  }
void loop() {
  
  if((leituraD(botao1) == 1) && (filtro == 0)){
    rodar = !rodar;
    filtro = 1;
  }
  
  if((filtro == 1) && (leituraD(botao1) == 0))
  filtro = 0;

  controleDisplay(minutosa, dp[0]);
  controleDisplay(minutosb, dp[1]);
  controleDisplay(segundosa, dp[2]);
  controleDisplay(segundosb, dp[3]);

  if(leituraD(botao2) == 1){
     minutosa  = 0;
     minutosb  = 0;
     segundosa = 0;
     segundosb = 0; 
   }
   
  while(rodar)
  {
    if((leituraD(botao1) == 1) && (filtro == 0)){
    rodar = !rodar;
    filtro = 1;    
    }
    if((filtro == 1) && (leituraD(botao1) == 0))
    filtro = 0;

    if(leituraD(botao2) == 1){
      minutosa  = 0;
      minutosb  = 0;
      segundosa = 0;
      segundosb = 0; 
    }
              
    if(segundosb > 9){
      segundosb = 0;
      segundosa++;
    }
    if(segundosa >= 6){
      segundosa = 0;
      minutosb++;
    }
    if(minutosb > 9){
      minutosb = 0;
      minutosa++;    
    }
    if(minutosa >= 6){
     minutosa  = 0;
     minutosb  = 0;
     segundosa = 0;
     segundosb = 0;    
    }
    
    currentTime = millis();
    leituraA();
    period = adc;
      
    if (currentTime - startTime >= period)//test whether the period has elapsed
    {
        segundosb++;
        startTime = currentTime;
    }  
  controleDisplay(minutosa, dp[0]);
  controleDisplay(minutosb, dp[1]);
  controleDisplay(segundosa, dp[2]);
  controleDisplay(segundosb, dp[3]);    
  }}
