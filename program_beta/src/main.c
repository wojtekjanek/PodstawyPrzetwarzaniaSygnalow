#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dac.h"
 
int ConvertedValue = 0; //Converted value readed from ADC
 
 
void adc_configure(){
	
 ADC_InitTypeDef fifa; //Structure for adc confguration
 GPIO_InitTypeDef rzeznik; //Structure for analog input pin
	
 //Clock configuration
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
 RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
 
	//Analog pin configuration
 rzeznik.GPIO_Pin = GPIO_Pin_0;//PC0-ADC
 rzeznik.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
 rzeznik.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
 GPIO_Init(GPIOC,&rzeznik);//Affecting the port with the initialization structure configuration
 
	//ADC structure configuration
 ADC_DeInit();
 fifa.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
 fifa.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
 fifa.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
 fifa.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
 fifa.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
 fifa.ADC_NbrOfConversion = 1;//I think this one is clear :p
 fifa.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
 ADC_Init(ADC1,&fifa);//Initialize ADC with the previous configuration
 
 //Enable ADC conversion
 ADC_Cmd(ADC1,ENABLE);
 
 //Select the channel to be read from
 ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_144Cycles);
}


void dac_configure(){
	
	DAC_InitTypeDef janek;		//konfiguracja DAC
	GPIO_InitTypeDef wojtek;		//konfiguracja pinu
	
	//Clock configuration
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
  RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the DAC port!! Do not forget about this one ;)
 
	//Analog pin configuration
  wojtek.GPIO_Pin = GPIO_Pin_4;//PC4 - DAC
  wojtek.GPIO_Mode = GPIO_Mode_AN; //The PC4 pin is configured in analog mode
  wojtek.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
  GPIO_Init(GPIOC,&wojtek);//Affecting the port with the initialization structure configuration
	
	DAC_DeInit();
	janek.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	janek.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	janek.DAC_Trigger = DAC_Trigger_None;
	janek.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_1,&janek);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

int adc_convert(){
 ADC_SoftwareStartConv(ADC1);//Start the conversion
 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
 return ADC_GetConversionValue(ADC1); //Return the converted data
}

int dac_convert(int data){
	DAC_SetChannel1Data(DAC_Align_12b_R, data);	
}

//tablica side short 16bitow
// 128 probek

int counter, DATA_ADC[128],ptr_adc;


int main(void){
	adc_configure();//Start configuration
	dac_configure();
    while(1){//loop while the board is working
    ConvertedValue = adc_convert();//Read the ADC converted value
			
	//test ADC_DAC forsimul.		
		DATA_ADC[ptr_adc++] = ptr_adc;
			if(ptr_adc > 127) ptr_adc =0;
			
		dac_convert(ConvertedValue);
    }
}
