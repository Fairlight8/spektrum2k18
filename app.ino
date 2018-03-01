//PARAMETROS
const float maxstep = 10;
const int pin_strobe;
const int pin_reset;
const int led1 = 3;
const int pin_analog = 22;
//const int led2;
//const int led3;
//const int led4;
//const int led5;
//const int led6;
//const int led7;
const int numero_de_leds = 10;
const int settling_time = 999; //YA LO DISCUTIREMOS
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
void setup()
{
    pinMode(pin_reset, OUTPUT);
    pinMode(pin_strobe, OUTPUT);
    for (j = 0; j < 10; j++)
    {
        pinMode(j + led1, OUTPUT) //todos los leds de salida
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

    for (i = 0; i < 7; i++)
    {
        digitalWrite(pin_strobe, LOW);
        instante_inicial = micros(); //micros(de 4 en 4)
        if(lectura[i]<representar[i]){
            if(representar[i]-lectura[i]>maxstep){
                representar[i]-=maxstep;
            }
            else{
                representar[i]=lectura[i];
            }
        }
        else{
            if(lectura[i]-representar[i]>maxstep){
                representar[i]+=maxstep;
            }
            else{
                representar[i]=lectura[i];
            }
        }
        microsegundos_ultimo_led = 1000 * representar[i] - 1000 * ((int)representar[i]);
        instante_final = instante_inicial + 1000;
        instante_led = instante_inicial + microsegundos_ultimo_led;
        instante_strobe = instante_inicial + settling_time;
        for (j = 0; j <= (int)representar[i]; j++)
        {
            digitalWrite(j + led1, HIGH);
        }
            while (micros() < instante_final)
        {
            if (micros() >= instante_led && !Flag_led)
            {
                Flag_led = true;
                digitalWrite((int)representar[i] + led1, LOW);
            }
            if (micros() <= instante_strobe && !Flag_strobe)
            {
                Flag_strobe = true;
                lectura_analog = analogRead(pin_analog);
                if (i == 6)
                {
                    //------------------------------OJO COÑO
                    digitalWrite(pin_reset, HIGH);//OJO CUIDAO
                    //------------------------------EEEEEEEH HOSTIA
                    lectura[0] = (lectura_analog * numero_de_leds) / 1024;
                }
                else
                {
                    lectura[i + 1] = (lectura_analog * numero_de_leds) / 1024;
                }
                digitalWrite(pin_strobe, HIGH);
            }
        }

        //delay until time;
        //encender_leds
        //}
        //strobe=0
        //led decimal on
        //delay(decimal-Ts)
        //leer R2D2
        //strobe=1
        //delay(Ts)
        //led decimal off
        //delay(1000-decimal)
    }

}
