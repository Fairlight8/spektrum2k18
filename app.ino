//PARAMETROS
const float maxstep = 0.1;
const int pin_strobe=26;
const int pin_reset=27;//------2 pines
const int pin_barra2=25;
const int pin_barra1=24;
const int pin_barra0=23;//-----3 pines
const int led1 = 3;//------10 pines
const int pin_analog = A7;//1 pin
const int microsegundos_barra=1000;
const int barra_2[7]{0, 0, 0, 0, 1, 1, 1};
const int barra_1[7]{0, 0, 1, 1, 0, 0, 1};
const int barra_0[7]{0, 1, 0, 1, 0, 1, 0};
//const int led2;//--total 16 pines
//const int led3;000 001 010 011 100 101 110 111
//const int led4;1   2    3   4  5   6    7
//const int led5;
//const int led6;
//const int led7;
const int numero_de_leds = 10;
const int settling_time = 500; //YA LO DISCUTIREMOS
//VARIABLES
boolean Flag_led = false;
boolean Flag_strobe = false;
unsigned long instante_inicial, instante_final, instante_led, instante_strobe;

int microsegundos_ultimo_led;
int lectura_analog;
float lectura[7] = {0, 0, 0, 0, 0, 0, 0};
float representar[7] = {0, 0, 0, 0, 0, 0, 0};
int i, j;
//--------------------------------------
int contadordebug=0;
void setup()
{
    pinMode(pin_reset, OUTPUT);
    pinMode(pin_strobe, OUTPUT);
    for (j = 0; j < 10; j++)
    {
        pinMode(j + led1, OUTPUT); //todos los leds de salida
    }
    //leemos la primera barra
    digitalWrite(pin_reset, HIGH);
    digitalWrite(pin_strobe, HIGH);
    delayMicroseconds(3);
    digitalWrite(pin_reset, LOW);
    delayMicroseconds(75);
    digitalWrite(pin_strobe, LOW);
    delayMicroseconds(settling_time);
    lectura_analog = analogRead(pin_analog);
    lectura[0] = representar[0] = (lectura_analog * numero_de_leds) / 1024;
    digitalWrite(pin_strobe, HIGH);
    delayMicroseconds(75);

}

void loop()
{   
  

    for (i = 0; i < 7; i++){
        
        //--------------------------------Instante Inicial
        //habilitar la barra correspondiente
        digitalWrite(pin_barra2, barra_2[i]);
        digitalWrite(pin_barra1, barra_1[i]);
        digitalWrite(pin_barra0, barra_0[i]);
        digitalWrite(pin_strobe, LOW);
        instante_inicial = micros(); //micros(de 4 en 4)
        Flag_led = false;
        Flag_strobe = false;
        if(lectura[i]<representar[i]){
            if((representar[i]-lectura[i])>maxstep){
                representar[i]-=maxstep;
            }
            else{
                representar[i]=lectura[i];
            }
        }
        else{
            if((lectura[i]-representar[i])>maxstep){
                representar[i]+=maxstep;
            }
            else{
                representar[i]=lectura[i];
            }
        }
        microsegundos_ultimo_led = microsegundos_barra * representar[i] - microsegundos_barra * ((int)representar[i]);
        instante_final = instante_inicial + microsegundos_barra;
        instante_led = instante_inicial + microsegundos_ultimo_led;
        instante_strobe = instante_inicial + settling_time;//-------y lectura
        for (j = 0; j <= (int)representar[i]; j++)
        {
            digitalWrite(j + led1, HIGH);
        }
        //-----------------------------------Bucle 1milisegundo
            while (micros() < instante_final)
        {
            if (micros() >= instante_led && !Flag_led)
            {
                Flag_led = true;
                digitalWrite((int)representar[i] + led1, LOW);
            }
            if ((micros() >= instante_strobe )&& !Flag_strobe)
            {
                Flag_strobe = true;
                lectura_analog = analogRead(pin_analog);
                
                if (i == 6)
                {
                    digitalWrite(pin_reset, HIGH);//---------OJO CUIDAO
                    unsigned long instante_reset = instante_strobe+ 1;
                    lectura[0] = (lectura_analog * numero_de_leds) / 1024;
                    while(micros()<=instante_reset){
                        
                    }
                    digitalWrite(pin_reset, LOW);
                }
                else
                {
                    lectura[i + 1] = (lectura_analog * numero_de_leds) / 1024;
                }
                digitalWrite(pin_strobe, HIGH);
            }
        }
        for (j = 0; j <= numero_de_leds; j++)
        {
            digitalWrite(j + led1, LOW);
        }
        //deshabilitar la barra correspondiente
    }
  
}
