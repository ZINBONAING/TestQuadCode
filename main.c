#include <stm32f4xx.h>
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include <stdio.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stm32f4xx_i2c.h>
#include <MPU9150.h>
#include "stm32f4xx_rcc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <math.h>
#include "stm32f4xx_tim.h"
#include <stm32f4xx_it.h>
#define SERIAL_BUFFER_SIZE 512
char serial_buffer[SERIAL_BUFFER_SIZE] ;
int sb_index = 0 ;
int end_index = 0 ;
<<<<<<< HEAD
// main 1E
=======
// main 1D Testing T3
>>>>>>> testing
// Changes


int togglebit=0;
int timercount=0;
int his_timercount=0;
int stoptimer=0;
int serialflag=0;
int movavgcounter=0;
int XErrbuf;
// P=0.8 , I=0.1 , D=0.225
// P=3, I=1, D=0.3
//P=0.17 D=0.16 when upper limit set to 600
//0.34 and 0.28 under control for 2x prop
//small battery 0.23 and 0.22 seems ok. slow thouggh,0.33,0.34
//small batter 0.165 , 0.17 ok. 4 props
//4 x prop small battery
//1.9 and Dgain 1.6 is ok

//Rate PID PG=4.4 , D=5.4 , Kp=0.7
//Rate PID PG=4.2 , D=5.4 , Kp=0.9 ,IGain=0
//Rate PID PG=4.2 , D=7 , Kp=0.9 ,IGain=0
//best value pG=5.2 Dgain=7.2 , Kp=0.8 , Ki=0.005

#define Logbuf 10 //
float PGain=4.7,ErrorX=0,ErrorY=0,setX=0,setY=0,setheight,ErrorH=0,GH=0.0005;
float IGain=0.0,Dgain=7.2,err_diffX=0.0,err_diffY=0.0,int_errX=0.0,int_errY=0.0,PreviousErrX=0.0,PreviousErrY=0.0;
//--------------------------------------------- Rate PID ---------------------------------------------------------
float RateYPG=0.8,RateYDG=0,RateYIG=0.005,SetYRate=5;
float PreviousErrRateY,ErrRateY,DiffErrRateY,IntErrRateY,PtermRateY,DtermRateY,ItermRateY;
float PIDRateY;


float RateXPG=0.8,RateXDG=0,RateXIG=0.005,SetXRate=5;
float PreviousErrRateX,ErrRateX,DiffErrRateX,IntErrRateX,PtermRateX,DtermRateX,ItermRateX;
float PIDRateX;


float p_termx=0.0,i_termx=0.0,d_termx=0.0,p_termy=0.0,i_termy=0.0,d_termy=0.0;
float pidx=0.0,pidy=0.0;
float M1=500,M2=500,M3=500,M4=500;
int16_t AccXA[20],AccYA[20],AccZA[20],GyroXA[20],GyroYA[20],GyroZA[20];
int trottle_manual=0,trottleintrrupt=0,conttrolflag,setmotorspeed=0,state1=0;
float AccAngleX,AccAngleY,RateAyz,RateAxz;
float temperrX,temperrY;
int flightmode=0;
#define I2C_TIMEOUT  (0x5)
#define PI 3.14159265358979
float MXlimit=15000;
#define MNlimit 8100
#define step 100

int IC2Value_radio, DutyCycle_radio,DutyCycle2_radio,Frequency_radio,curr_ch3,prev_ch3;
int IC2Value_radioCh5,DutyCycle_radio5,DutyCycle2_radio5,Frequency_radio5;
int IC2Value_radioCh9,DutyCycle_radio9,DutyCycle2_radio9,Frequency_radio9;
float sensorheight=0;


float PGainH=0.2,IGainH=0,DgainH=0;
float err_diffH,int_errH,p_termh,i_termh,d_termh,pidh,PreviousErrH;
int PreviousFlightMode=0,StableMode=0;

//Radio status state machine
int Radio_status=0;
int Rstate=0;
int ns_radio=0;
float radioin=0;
int tempradio_status=0;

float XErrbuffer[Logbuf];
float M1x[Logbuf];
float M2x[Logbuf];
float M3x[Logbuf];
float M4x[Logbuf];
float Ptermx[Logbuf];
float Dtermx[Logbuf];
float Itermx[Logbuf];
float PIDx[Logbuf];
float Radioinx[Logbuf];
int PID_Start=0;
//end Radio status state machine
void PWMinput_radioCH3(void);
void radio_in(void);
//Version Number : Major.Minor.Complete/Progress
// Revision : ZIO1.M1.2 # added PWM for motor


// this slave address belongs to the STM32F4-Discovery board's
// CS43L22 Audio DAC
// connect PD4 to VDD in order to get the DAC out of reset and test the I2C
// interface
#define SLAVE_ADDRESS 0x68 // the slave address (example)

__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint16_t StartMotor = 0;
__IO uint16_t DutyCycle2 = 0;
__IO uint32_t Frequency = 0;



int16_t AccXvalue=0;
int16_t AccYvalue=0;
int16_t  AccZvalue=0;
int16_t GyroXvalue=0;
int16_t GyroYvalue=0;
int16_t GyroZvalue=0;


float RxEst,RyEst,RzEst,RxEstpast,RyEstpast,RzEstpast,Axr,Ayr,Azr;
float 	RxAccR,RyAccR,RzAccR,RxGyroR,RyGyroR,RzGyroR;




int32_t RatePIDY(int DesiredRate){


	return(PIDRateY);

}












//-----------------------------------------End Rate PID -------------------------------------------------------------

#define SENSITIVITY 4096
int T=0;


#define MAX_STRLEN 5 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string
//------------------Start UART Setup------------------------------
/*
* This funcion initializes the USART1 peripheral
 *
 * Arguments: baudrate --> the baudrate at which the USART is
 * 						   supposed to operate
 */

void fuseGyroAcc(int,int,int,int,int,int);

float squaredz(float x){
  return x*x;
}
void fuseGyroAcc(int RxAcc0,int RyAcc0,int RzAcc0,int RxGyro0,int RyGyro0,int RzGyro0){

	float RaccModulus,RaccNormalize,Axz,Ayz,SignRzGyro;
	float 	RxAcc,RyAcc,RzAcc,RxGyro,RyGyro,RzGyro;

	float   AyzPast,AxzPast,R;
	float Wgyro=10.0;
	int i,j;
	float totalx=0.0,totaly=0.0,totalz=0.0;
	RxAccR=(((float)RxAcc0))/4096;
	RyAccR=(((float)RyAcc0))/4096;
	RzAccR=(((float)RzAcc0))/4096;




	// Avgerage last 20 sample



	//int16_t AccXA[20],AccYA[20],AccZA[20],GyroXA[20],GyroYA[20],GyroZA[20];
	AccXA[movavgcounter]=RxAccR;
	AccYA[movavgcounter]=RyAccR;
	AccZA[movavgcounter]=RzAccR;
	if(movavgcounter>10){
		movavgcounter=0;
	}

	for(i=0;i<11;i++){
		totalx=AccXA[i]+totalx;
		totaly=AccYA[i]+totaly;
		totalz=AccZA[i]+totalz;

	}
//	RxAccR=totalx/10.0;
//	RyAccR=totaly/10.0;
//	RzAccR=totalz/10.0;
	totalx=0;totaly=0;totalz=0;

	movavgcounter=movavgcounter+1;
	//end Avergae last 20 sample

	RxGyroR=((float)RxGyro0)/65.5;
	RyGyroR=((float)RyGyro0)/65.5;
	RzGyroR=((float)RzGyro0)/65.5;

	RaccModulus=sqrt((RxAccR*RxAccR)+(RyAccR*RyAccR)+(RzAccR*RzAccR));
	RxAcc=RxAccR/RaccModulus;
	RyAcc=RyAccR/RaccModulus;
	RzAcc=RzAccR/RaccModulus;
	//AccAngleX,AccAngleY;

	AccAngleX=acos(RxAcc)*180/PI-90;
	AccAngleY=acos(RyAcc)*180/PI-90;

    if(T==0){  //time is 0, first data point , define REst at 0 time.
	   RxEstpast= RxAcc;
	   RyEstpast= RyAcc;
       RzEstpast= RzAcc;

    }

    if( (RzEstpast<0.1) && (RzEstpast>-0.1)    ) {
    	RxGyro= RxAcc;
    	RyGyro= RyAcc;
    	RzGyro= RzAcc;

    }
    else
    {
    	RateAxz=1*RyGyroR*PI/180.0;
    	RateAyz=1*RxGyroR*PI/180.0;


    	AxzPast=atan2(RxEstpast,RzEstpast);
    	Axz=AxzPast+RateAxz*0.01;//new angle based on Gyro data
    	AyzPast=atan2(RyEstpast,RzEstpast);
    	Ayz=AyzPast+RateAyz*0.01;


    	if((cos(Axz)*(PI/180.0))>=0)
    		{
    		SignRzGyro=1;
    		}
    	else
    	{
    		SignRzGyro=-1;
    	}


    	RxGyro=sin(Axz)/sqrt((1+ squaredz(cos(Axz))*squaredz(tan(Ayz))   ));
    	RyGyro=sin(Ayz)/sqrt((1+squaredz(cos(Ayz))*squaredz(tan(Axz))   ));
    	//0.9428 / (1+(-0.77)x4.21

    	//RzGyro=1;
    	RzGyro=SignRzGyro*sqrt(1-(RxGyro*RxGyro)-(RyGyro*RyGyro));



    }
    T=2; //make sure Tis not Zero so it won't initialize again
  // ------------------------------GYRO----------------------------------------------------



RxEst=(RxAcc+RxGyro*Wgyro)/(1+Wgyro);
RyEst=(RyAcc+RyGyro*Wgyro)/(1+Wgyro);
RzEst=(RzAcc+RzGyro*Wgyro)/(1+Wgyro);

R=sqrt((RxEst*RxEst)+(RyEst*RyEst)+(RzEst*RzEst));
RxEst=RxEst/R;
RyEst=RyEst/R;
RzEst=RzEst/R;


RxEstpast= RxEst;
RyEstpast= RyEst;
RzEstpast= RzEst;


Axr=acos(RxEst)*180/PI-90;
Ayr=acos(RyEst)*180/PI-90;
Azr=acos(RzEst)*180/PI-90;

}

char Csign(float Zee){
	char signn;

	if((Zee<0)&(Zee>-1)){
		signn='-';

	}
	else {
		signn=32;
	}
	return signn;
}

int C1(float Zee){
	int int1;
	float int2;

	int1=(int)Zee;

	return int1;
}
int C2(float Zee){
	int int1,ans;
	float int2,int3;
	if(Zee<0){
		Zee=Zee*(-1);

		}
	int1=(int)Zee;
	int2=(Zee-int1)*1000;
	ans=(int)int2;

	return ans;
}

int ki;

	int main(void){

/*

		 for(ki=0;ki<Logbuf;ki++){

					  M1x[ki]=-99;
					  M2x[ki]=-99;
					  M3x[ki]=-99;
					  M4x[ki]=-99;

					 Ptermx[ki]=-99;
					 Dtermx[ki]=-99;
					 Itermx[ki]=-99;
					  PIDx[ki]=-99;
					  Radioinx[ki]=-99;
					  XErrbuffer[ki]=-99;

				 			                }

*/
int sensor_value=0;

I2C1_init(); // initialize I2C peripheral
InitPWM();
InitPWM2();
InitPWM3();
InitPWM4();

PWMinput_sound();
PWMinput_radioCH3();
//PWMinput_radioCH5();
PWMinput_radioCH6();
InitializeTimer2();

//InitializeTimer2();
Delay(50000);
	Delay(50000);
	Delay(50000);Delay(50000);
	Delay(50000);
	Delay(50000);Delay(50000);
	Delay(50000);
	Delay(50000);
init_USART3(9600);
uint8_t received_data2=0xF0;



MPUIni_init16();
sensor_value=0;
sensor_value=MPU9150_read1byte(MPU9150_PWR_MGMT_1);
serial_output("PWRMG1 = %x ",sensor_value);
sensor_value=MPU9150_read1byte(MPU9150_WHO_AM_I);
serial_output("I am MPU = %x ",sensor_value);

init_GPIO();

  /* This flashed the LEDs on the board once
   * Two registers are used to set the pins (pin level is VCC)
   * or to reset the pins (pin level is GND)
   *
   * BSRR stands for bit set/reset register
   * it is seperated into a high and a low word (each of 16 bit size)
   *
   * A logical 1 in BSRRL will set the pin and a logical 1 in BSRRH will
   * reset the pin. A logical 0 in either register has no effect
   */
  GPIOD->BSRRL = 0xF000; // set PD1
  Delay1(1000000L);		 // wait a short period of time
  GPIOD->BSRRH = 0xF000; // reset PD1


  StartMotor=0;





while(1){

	if(conttrolflag==1){

			conttrolflag=0;




	}


if(serialflag==1){


//RxGyroR

  //  serial_output("%c%d.%d,",Csign(RxAccR),C1(RxAccR),C2(RxAccR));
  //  serial_output("%c%d.%d,",Csign(RyAccR),C1(RyAccR),C2(RyAccR));
  //  serial_output("%c%d.%d,",Csign(RzAccR),C1(RzAccR),C2(RzAccR));
   serial_output("%c%d.%d\t",Csign(M1),C1(M1),C2(M1));
   serial_output("%c%d.%d\t",Csign(M2),C1(M2),C2(M2));
   serial_output("%c%d.%d\t",Csign(M3),C1(M3),C2(M3));
   serial_output("%c%d.%d\t",Csign(M4),C1(M4),C2(M4));

 //  serial_output("RateY:%c%d.%d\t",Csign(RyGyroR),C1(RyGyroR),C2(RyGyroR));
 //  serial_output("Pterm:%c%d.%d\t",Csign(PtermRateY),C1(PtermRateY),C2(PtermRateY));
 //  serial_output("ErrRateY:%c%d.%d\t",Csign(ErrRateY),C1(ErrRateY),C2(ErrRateY));
  // serial_output("PID:Y: %d,",PIDRateY);



  // float RateYPG=200,RateYDG=0,RateYIG=50,SetYRate=-5;
  // float PreviousErrRateY,ErrRateY,DiffErrRateY,IntErrRateY,PtermRateY,DtermRateY,ItermRateY;
  /*
   ErrRateY=SetYRate-RyGyroR;
   	PtermRateY=	ErrRateY*RateYPG;
   	DiffErrRateY=ErrRateY-PreviousErrRateY;
   	DtermRateY=DiffErrRateY*RateYDG;
   	IntErrRateY=IntErrRateY+ErrRateY;
   	ItermRateY=RateYIG*IntErrRateY;
   	*/
  //  serial_output("%d,",M1);
  //  serial_output("%d,",M2);
  //  serial_output("%d,",M3);
  //  serial_output("%d,",M4);
 //   serial_output("%d,",sensorheight);
    serial_output("Height %c%d.%d cm,",Csign(sensorheight),C1(sensorheight),C2(sensorheight));
   // serial_output("motor=%d,",StartMotor);
  //  serial_output("FlightMode=%d,",flightmode);
   //serial_output("%c%d.%d,",Csign(ErrorH),C1(ErrorH),C2(ErrorH));
  // temperrX=ErrorX*100;
   // temperrY=ErrorY*100;
   // serial_output("%c%d.%d,",Csign(temperrX),C1(temperrX),C2(temperrX));
   // serial_output("%c%d.%d",Csign(temperrY),C1(temperrY),C2(temperrY));

   // serial_output("%c%d.%d,",Csign(RzAccR),C1(RzAccR),C2(RzAccR));
   // serial_output("%c%d.%d,",Csign(RzAccR),C1(RzAccR),C2(RzAccR));
   // serial_output("%c%d.%d,",Csign(RzAccR),C1(RzAccR),C2(RzAccR));

  // serial_output("P:%c%d.%d\t",Csign(p_termx),C1(p_termx),C2(p_termx));
  // serial_output("I:%c%d.%d\t",Csign(i_termx),C1(i_termx),C2(i_termx));
  // serial_output("D:%c%d.%d\t",Csign(d_termx),C1(d_termx),C2(d_termx));
  // serial_output("PID:%c%d.%d\t",Csign(pidx),C1(pidx),C2(pidx));
 //   ErrorH=setheight

    serial_output("ErrorH:%c%d.%d\t",Csign(ErrorH),C1(ErrorH),C2(ErrorH));
    serial_output("setheight:%c%d.%d cm \t",Csign(setheight),C1(setheight),C2(setheight));


  //  serial_output("Xangle= \t%c%d.%d\t",Csign(Axr),C1(Axr),C2(Axr));
    serial_output("ErrX=\t%c%d.%d\t",Csign(ErrorX),C1(ErrorX),C2(ErrorX));
    serial_output("ErrY=\t%c%d.%d\t",Csign(ErrorY),C1(ErrorY),C2(ErrorY));
 //   serial_output("SM:%d\t",StartMotor);
 //   serial_output("RS:%d\t",Radio_status);

    serial_output("StableMode:%d\t",StableMode);
    serial_output("FlightMode:%d\t",flightmode);

 //   serial_output("Radio=%c%d.%d\t",Csign(DutyCycle2_radio),C1(DutyCycle2_radio),C2(DutyCycle2_radio));

    //DutyCycle2_radio5
 //   serial_output("Radio6=%c%d.%d\t",Csign(DutyCycle2_radio5),C1(DutyCycle2_radio5),C2(DutyCycle2_radio5));
//StableMode
    //


    serial_output("Ayr : %c%d.%d",Csign(Ayr),C1(Ayr),C2(Ayr));
    //serial_output("%5d,",AccYvalue);
   // serial_output("#Est%f<",RyEst);
   // serial_output("%5d,",AccZvalue);
   // serial_output("#Est %f<",RzEst);

 //   serial_output("Timer=%5d ",timercount);
    serial_output("\n");
    serialflag=0;
}

his_timercount=timercount;
}
return 0;
}

/* @brief  This function handles TIM4 global interrupt request.
* @param  None
* @retval None
*/
void TIM4_IRQHandler(void)
{

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	 //togglebit^=1;
	//	  if(togglebit) GPIOD->BSRRL = 0xF000; // set PD1
		  	 // wait a short period of time
	//	  else GPIOD->BSRRH = 0xF000; // reset PD1

	//	TIM_ICInitTypeDef  TIM_ICInitStructure;


/* Clear TIM4 Capture compare interrupt pending bit */
TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

/* Get the Input Capture value */
IC2Value = TIM_GetCapture2(TIM4);

if (IC2Value != 0)
{
  /* Duty cycle computation */
  //DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;
  DutyCycle=IC2Value;
  DutyCycle2=TIM_GetCapture1(TIM4);
  sensorheight=DutyCycle2/96.6;
  /* Frequency computation
     TIM4 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

  Frequency = (RCC_Clocks.HCLK_Frequency)/2 / IC2Value;
}
else
{
  DutyCycle = 0;
  Frequency = 0;
}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void radio_in(){
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	 serial_output("Radio signal=\t%c%d.%d\t",Csign(DutyCycle2_radio),C1(DutyCycle2_radio),C2(DutyCycle2_radio));
        if(DutyCycle2_radio<20000)
        {
        	Rstate=ns_radio;
            switch(Rstate)
            {
				case 0:ns_radio= ns_radio+1;
				break;
				case 1:ns_radio= ns_radio+1;
								break;
				case 2:ns_radio= ns_radio+1;
								break;


				case 3:ns_radio= 0;
				Radio_status=0;

				break;
		    }
        }
	    else
	    {
			Radio_status=1;
			ns_radio=0;






         }


	    if((DutyCycle2_radio>20000) && (DutyCycle2_radio<40000))

	    {


	    	//radioin=((DutyCycle2_radio-32941)/54.6)+490;
	    	radioin=((DutyCycle2_radio-20000)/1.358)+8100;
	    //	radioin=550.00;
	    //	MXlimit=radioin*1.3;
	    //	if(MXlimit>700){
	    	//	MXlimit=700;
	    	//}
	     }

	if(Radio_status==0){
		StartMotor=0;
		M1=0;
		M2=0;
		M3=0;
		M4=0;
		TIM_SetCompare1(TIM1, 8000);
						TIM_SetCompare2(TIM1, 8000);
						TIM_SetCompare3(TIM1, 8000);
						TIM_SetCompare4(TIM1, 8000);
						M1=8000;
						M2=8000;
						M3=8000;
						M4=8000;
		radioin=0;
		DutyCycle2_radio=0;

	}



	 if((timercount>5000) & (Radio_status==1)){
		 StartMotor=1;
	 }

	DutyCycle2_radio=0;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ControlLoop(){
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          //--------Begin PID correction----------

	/*
		togglebit^=1;
		       		  if(togglebit) GPIOD->BSRRL = 0xF000; // set PD1
		       		  	//  wait a short period of time
		       		  else GPIOD->BSRRH = 0xF000; // reset PD1
		       		  */



		 	//Delay(50000);
		  //Delay(150000);
		 	// Delay(150000);
		 	// Delay(150000);
		//serial_output("Reading Sensor\n");
		 //int  result = snprintf(buf, SIZE, "%f", value);



	 GyroXvalue=(MPU9150_readSensor(MPU9150_GYRO_XOUT_L,MPU9150_GYRO_XOUT_H));


					GyroYvalue=(MPU9150_readSensor(MPU9150_GYRO_YOUT_L,MPU9150_GYRO_YOUT_H));
					 GyroZvalue=(MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H));
					     //serial_output("GyroX=%5d ",GyroXvalue);
					     //serial_output("GyroY=%5d ",GyroYvalue);
					     //serial_output("GyroZ=%5d ",GyroZvalue);


					 	AccXvalue=((MPU9150_readSensor(MPU9150_ACCEL_XOUT_L,MPU9150_ACCEL_XOUT_H)));//to convert to 2nd complement -128 to 128 instead of 0-256
					 //	Delay(50000);
					// 	AccXvalue=(signed char)AccXvalue;
					     AccYvalue=((MPU9150_readSensor(MPU9150_ACCEL_YOUT_L,MPU9150_ACCEL_YOUT_H)));//to convert to 2nd complement -128 to 128 instead of 0-256
					 //	Delay(50000);
					 	AccZvalue=((MPU9150_readSensor(MPU9150_ACCEL_ZOUT_L,MPU9150_ACCEL_ZOUT_H)));//to convert to 2



					 	fuseGyroAcc(AccXvalue,AccYvalue,AccZvalue,GyroXvalue,GyroYvalue,GyroZvalue);








		   ErrorX=setX-Axr;
		   ErrorY=setY-Ayr;
		 //  ErrorH=setheight-DutyCycle2;



		   if((Radio_status==1)){


				   PID_Start=PID_Start+1;


							   if(PID_Start>30000){
								   PID_Start=10;

							   }





								err_diffX=(ErrorX-PreviousErrX)/0.07;
								int_errX=int_errX + ErrorX;



								p_termx=PGain*ErrorX;  //2.4
								i_termx=IGain*int_errX;
								d_termx=(Dgain*err_diffX);
								pidx=p_termx+d_termx+i_termx;
								PreviousErrX=ErrorX;

								err_diffY=ErrorY-PreviousErrY;
								int_errY=int_errY + ErrorY;
								p_termy=PGain*ErrorY;
								i_termy=IGain*int_errY;
								d_termy=Dgain*(err_diffY/0.07);
								pidy=p_termy+d_termy+i_termy;
								PreviousErrY=ErrorY;


								if(((ErrorY<2) && (ErrorY>-2)) && ((ErrorX<2) && (ErrorX>-2))){
																	 StableMode=1;

																 }
																 else
																 {
																	 StableMode=0;
																 }


								 if((ErrorY<2) && (ErrorY>-2)) {int_errY=0; }
								 if((ErrorX<2) && (ErrorX>-2)) {int_errX=0; }

								 if(StableMode==1){

									 if((flightmode==1)&(PreviousFlightMode!=flightmode)){   //Altitude Hold Mode just entered
										 setheight=sensorheight;
 									 }
									 PreviousFlightMode=flightmode;

								 }




//----------------- Temp disable to test Rate Gyro------------------------------------------------------------------------

							//	 RatePIDY


								    ErrRateY=pidy-RyGyroR;
								 	PtermRateY=	ErrRateY*RateYPG;
								 	DiffErrRateY=ErrRateY-PreviousErrRateY;
								 	DtermRateY=DiffErrRateY*RateYDG;
								 	IntErrRateY=IntErrRateY+ErrRateY;
								 	ItermRateY=RateYIG*IntErrRateY;
								 	PIDRateY=PtermRateY+DtermRateY+ItermRateY;
						   //End Rate PID Y


								 	//	 RatePIDX


								 	ErrRateX=pidx-RxGyroR;
								 	PtermRateX=	ErrRateX*RateXPG;
								 	DiffErrRateX=ErrRateX-PreviousErrRateX;
								 	DtermRateX=DiffErrRateX*RateXDG;
								 	IntErrRateX=IntErrRateX+ErrRateX;
								 	ItermRateX=RateXIG*IntErrRateX;
								 	PIDRateX=PtermRateX+DtermRateX+ItermRateX;
								    //End Rate PID XY


						//		M2=radioin+PIDRateY;//;//+(GH*ErrorH); -pidx
						//		M3=radioin+PIDRateY;//;//+(GH*ErrorH);+(GH*ErrorH); -pidx


						//		M1=radioin-PIDRateY;//;//+(GH*ErrorH);+(GH*ErrorH); +pidx
						//		M4=radioin-PIDRateY;//;//+(GH*ErrorH);+(GH*ErrorH); +pidx

									 if(flightmode==1){

										ErrorH=setheight-sensorheight;
										err_diffH=(ErrorH-PreviousErrH)/0.07;
										int_errH=int_errH + ErrorH;
								        p_termh=PGainH*ErrorH;  //2.4
										i_termh=IGainH*int_errH;
										d_termh=(DgainH*err_diffH);
										pidh=p_termh+d_termh+i_termh;
										PreviousErrH=ErrorH;

										M1=M1+pidh;
										M2=M2+pidh;
										M3=M3+pidh;
										M4=M4+pidh;


																 }
									 if(flightmode==0){
										   M2=radioin+PIDRateY-PIDRateX;//;//+(GH*ErrorH); -pidx
										   M3=radioin+PIDRateY+PIDRateX;//;//+(GH*ErrorH);+(GH*ErrorH); -pidx


										   M1=radioin-PIDRateY-PIDRateX;//;//+(GH*ErrorH);+(GH*ErrorH); +pidx
										   M4=radioin-PIDRateY+PIDRateX;//;//+(GH*ErrorH);+(GH*ErrorH); +pidx


									 }

//----------------- End Temp disable to test Rate Gyro------------------------------------------------------------------------
			   //--------End PID correction----------

	        }
               //------- if Radio is off ?------------

		    if(M1>(MXlimit)){ M1=MXlimit;}
		    if(M2>(MXlimit)){M2=MXlimit;}
			if(M3>(MXlimit)){M3=MXlimit;}
            if(M4>MXlimit){M4=MXlimit;}
            if(M1<MNlimit){M1=MNlimit;}
			if(M2<MNlimit){M2=MNlimit;}
			if(M3<MNlimit){M3=MNlimit;}
            if(M4<MNlimit){M4=MNlimit;}
            //---Begin only  execute if StartMotor Flag =1;
            if((StartMotor==1)& (Radio_status==1))
            {

				TIM_SetCompare1(TIM1, M1); //M1
			 	TIM_SetCompare2(TIM1, M2); //M2
			 	TIM_SetCompare3(TIM1, M3); //M3
			 	TIM_SetCompare4(TIM1, M4); //M4



            //---End only  execute if StartMotor Flag =1;
	            if(XErrbuf<Logbuf)
					{
						XErrbuffer[XErrbuf]=ErrorX;
						M1x[XErrbuf]=M1;
						M2x[XErrbuf]=M2;
						M3x[XErrbuf]=M3;
						M4x[XErrbuf]=M4;

						 Ptermx[XErrbuf]=p_termx;
										 Dtermx[XErrbuf]=d_termx;
										 Itermx[XErrbuf]=i_termx;
										 PIDx[XErrbuf]=pidx;
										 Radioinx[XErrbuf]=radioin;
					//XErrbuf=XErrbuf+1;
					}

	            else
	            {
	            	int k;
	            	float sampletime=0;
	                StartMotor=0;
	            	TIM_SetCompare1(TIM1, 450);
	            	TIM_SetCompare2(TIM1, 450);
	            	TIM_SetCompare3(TIM1, 450);
	            	TIM_SetCompare4(TIM1, 450);
	            	TIM_Cmd(TIM2, DISABLE);
	            	 for(k=0;k<500;k++){

	            			                sampletime=k;
	            			                serial_output("%d\t",k);

	            			                serial_output("Rin:\t%c%d.%d\t",Csign(Radioinx[k]),C1(Radioinx[k]),C2(Radioinx[k]));
	            			                serial_output("%c%d.%d\t",Csign(M1x[k]),C1(M1x[k]),C2(M1x[k]));
	            				            serial_output("%c%d.%d\t",Csign(M2x[k]),C1(M2x[k]),C2(M2x[k]));
	            				            serial_output("%c%d.%d\t",Csign(M3x[k]),C1(M3x[k]),C2(M3x[k]));
	            				            serial_output("%c%d.%d\t",Csign(M4x[k]),C1(M4x[k]),C2(M4x[k]));

	            				           serial_output("P\t%c%d.%d\t",Csign(Ptermx[k]),C1(Ptermx[k]),C2(Ptermx[k]));
	            				           serial_output("D\t%c%d.%d\t",Csign(Dtermx[k]),C1(Dtermx[k]),C2(Dtermx[k]));
	            				           serial_output("PID:\t\t%c%d.%d\t",Csign(PIDx[k]),C1(PIDx[k]),C2(PIDx[k]));
	            				           serial_output("Set\t\t%c%d.%d\t",Csign(setX),C1(setX),C2(setX));
	            				           serial_output("PG\t\t%c%d.%d\t",Csign(PGain),C1(PGain),C2(PGain));
	            				           serial_output("DG\t\t%c%d.%d\t",Csign(Dgain),C1(Dgain),C2(Dgain));
	            				            serial_output("Err\t%c%d.%d\t",Csign( XErrbuffer[k]),C1(XErrbuffer[k]),C2(XErrbuffer[k]));
	            				            serial_output("\n");

	            			                }

	            }


}

            else

            {

            	TIM_SetCompare1(TIM1, 8000);
            		            	TIM_SetCompare2(TIM1, 8000);
            		            	TIM_SetCompare3(TIM1, 8000);
            		            	TIM_SetCompare4(TIM1, 8000);

            }
}
//--- End Control loop--------------------------------------------------------------------------
void TIM2_IRQHandler()
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        timercount=timercount+1;
        if(timercount>500000){

        	timercount=5000;
        }

        if(timercount%100==0){
        	serialflag=1;
        }
        if(timercount%10==0)
              {
        	radio_in();
              	conttrolflag=1;
              }

        if((timercount%12==0) & (timercount>5000)){
        	GPIOD->BSRRL = 0xF000; // set PD1
        	ControlLoop();

		 	 GPIOD->BSRRH = 0xF000; // reset PD1
        }



        if(timercount==5000){
        	StartMotor=1;
                     }

        if(timercount%10000==0){
             // 	 setY=20;
               }
               if(timercount%20000==0){
             //    setY=-20.0;
                }

               if(timercount%30000==0){
                     //         setY=0.0;
                             }

    }
}

void TIM3_IRQHandler(void)
{

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	 //togglebit^=1;
	//	  if(togglebit) GPIOD->BSRRL = 0xF000; // set PD1
		  	 // wait a short period of time
	//	  else GPIOD->BSRRH = 0xF000; // reset PD1

	//	TIM_ICInitTypeDef  TIM_ICInitStructure;


/* Clear TIM4 Capture compare interrupt pending bit */
TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

/* Get the Input Capture value */
IC2Value_radio = TIM_GetCapture2(TIM3);

if (IC2Value_radio != 0)
{
  /* Duty cycle computation */
  //DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;
  DutyCycle_radio=IC2Value_radio;
  DutyCycle2_radio=TIM_GetCapture1(TIM3);
  /* Frequency computation
     TIM4 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

  Frequency_radio = (RCC_Clocks.HCLK_Frequency)/2 / IC2Value_radio;
}
else
{
	DutyCycle_radio = 0;
	Frequency_radio = 0;
	DutyCycle2_radio=0;
}
//serial_output("PWMwidth=%5d\t",DutyCycle2_radio);



}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void TIM5_IRQHandler(void)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{
//For Ch5 radio control input
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	 //togglebit^=1;
	//	  if(togglebit) GPIOD->BSRRL = 0xF000; // set PD1
		  	 // wait a short period of time
	//	  else GPIOD->BSRRH = 0xF000; // reset PD1

	//	TIM_ICInitTypeDef  TIM_ICInitStructure;


/* Clear TIM4 Capture compare interrupt pending bit */
TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);

/* Get the Input Capture value */
IC2Value_radioCh5 = TIM_GetCapture2(TIM5);

if (IC2Value_radioCh5 != 0)
{
  /* Duty cycle computation */
  //DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;
  DutyCycle_radio5=IC2Value_radioCh5;
  DutyCycle2_radio5=TIM_GetCapture1(TIM5);




  /* Frequency computation
     TIM4 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

  Frequency_radio5 = (RCC_Clocks.HCLK_Frequency)/2 / IC2Value_radioCh5;

  if((DutyCycle2_radio5<3000) & (DutyCycle2_radio5>1700)){
	flightmode=0;
}

if((DutyCycle2_radio5<3200) & (DutyCycle2_radio5>3001)){
	flightmode=1;
}






}
else
{
	DutyCycle_radio5 = 0;
	Frequency_radio5 = 0;
	DutyCycle2_radio5=0;
}
}

