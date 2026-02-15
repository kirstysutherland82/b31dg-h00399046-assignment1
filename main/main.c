#include <stdio.h>
#include "header.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"

bool outputEnable = true; //data on/off
bool outputSelect = false; //alternative behaviour

//button control variables
bool statePB1 = false; //tracks last stable state
bool statePB2 = false; //tracks last stable state
bool prevPB1 = false; //last raw button value
bool prevPB2 = false; //last raw button value
uint32_t lastDebouncePB1 =0;
uint32_t lastDebouncePB2 =0;


//create functions
void buttonCheck();
void outputData();

//equivalent of setup() and loop()
void app_main(void)
{
    //equiv of setup()
    gpio_set_direction(DATA, GPIO_MODE_OUTPUT);
    gpio_set_direction(SYNC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PB1, GPIO_MODE_INPUT);
    gpio_set_direction(PB2, GPIO_MODE_INPUT);
    gpio_set_level(DATA, 0);
    gpio_set_level(SYNC, 0);

    //equiv of loop()
    while(1){
        buttonCheck();

        if(outputEnable){
            outputData();
        }
        else{
            gpio_set_level(DATA, 0);
            gpio_set_level(SYNC, 0);
        }

    }
}

void buttonCheck(){
    bool pb1 = gpio_get_level(PB1);
    bool pb2 = gpio_get_level(PB2);
    uint32_t now = esp_timer_get_time(); // time in micros

      //detect pb1 rising edge
  if(pb1 != prevPB1){
    lastDebouncePB1 = now;
    prevPB1 = pb1;}


  if((now-lastDebouncePB1)>20000){ //20 millis (20000 micros) delay
    if(pb1 != statePB1 ){
      statePB1 = pb1;
      if(statePB1 == true){
        outputEnable = !outputEnable; //toggel outputEnable
      }
    }
  }

    //detect pb2 rising edge
  if(pb2 != prevPB2){
    lastDebouncePB2 = now;
    prevPB2 = pb2;}


  if((now-lastDebouncePB2)>20000){//20 millis (20000 micros) delay
    if(pb2 != statePB2){
      statePB2 = pb2;
      if(statePB2 == true){
        outputSelect = !outputSelect; //toggle outputEnable
      }
    }
  }

}

void outputData(){
    //sync high for 50us
    gpio_set_level(SYNC, 1);
     esp_rom_delay_us(TSYNC);
    gpio_set_level(SYNC, 0);

    //original data pulse pattern, carried out c times
    if(!outputSelect){
        for (int n = 1; n<=C; n++){
            buttonCheck();
            if (!outputEnable) return;
            //initialise T_on
            uint32_t on = A + ((n-1)*50*SCALE); //scale=1000 when debug toggled
    
            //high impulse for calculated time T_on
            gpio_set_level(DATA, 1);
            esp_rom_delay_us(on);
            gpio_set_level(DATA, 0);

            //wait for b us unless last impulse per cycle
            if(n<C){
                esp_rom_delay_us(B);
            }
        }
    }
  //alternative behaviour - reversed
    else{
        for (int n = C; n>=1; n--){
            buttonCheck();
            if (!outputEnable) return;
            //initialise T_on
            uint32_t on = A + ((n-1)*50*SCALE); //scale=1000 when debug toggled
  
            //high impulse for calculated time T_on
            gpio_set_level(DATA, 1);
            esp_rom_delay_us(on);
            gpio_set_level(DATA, 0);

            //wait for b us unless last impulse per cycle
            if(n>1){
                esp_rom_delay_us(B);
            }
        }
    }
//wait for d after final pulse
esp_rom_delay_us(D);
}