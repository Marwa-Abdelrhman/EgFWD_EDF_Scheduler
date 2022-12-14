/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "semphr.h"
/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )
	


/*Task Periods*/
#define Button_1_Monitor_Period  50
#define Button_2_Monitor_Period  50
#define Periodic_Transmitter_period 100
#define Uart_Receiver_period 20
#define Load_1_Simulation_period 10
#define Load_2_Simulation_period 100

/*Task Handlers*/
TaskHandle_t Button_1_Monitor_handler = NULL;
TaskHandle_t Button_2_Monitor_handler = NULL;
TaskHandle_t Periodic_Transmitter_handler = NULL;
TaskHandle_t Uart_Receiver_handler = NULL;
TaskHandle_t Load_1_Simulation_handler=NULL;
TaskHandle_t Load_2_Simulation_handler=NULL;

/*Variables for Task Analysis*/
const signed char Tx_Buf[15] = "Hello_EgFWD";

signed char Rx_Buf[15];
char runTimeStatsBuff[200];
int T1_TxStamp = 0, T1_TyStamp = 0, T1_totalTime=0;
int T2_TxStamp = 0, T2_TyStamp = 0, T2_totalTime=0;
int system_time = 0;
int cpu_load = 0;
volatile int misses=0;
/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
/* Task to be created. */

pinState_t button_state;


void Button_1_Monitor( void * pvParameters )
{
  TickType_t xLastWakeTime= xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *) 3);
    for( ;; )
    {
        /* Task code goes here. */
			
			vTaskGetRunTimeStats( runTimeStatsBuff );
	    vTaskDelayUntil(&xLastWakeTime,50);
	
			
    }
}
void Button_2_Monitor( void * pvParameters )
{
    
 TickType_t xLastWakeTime= xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *) 4);
    for( ;; )
    {
        /* Task code goes here. */
			vTaskGetRunTimeStats( runTimeStatsBuff );
		vTaskDelayUntil(&xLastWakeTime,50);
		}
			
			
	

	
			
}
void Periodic_Transmitter ( void * pvParameters )
{
	TickType_t xLastWakeTime= xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *) 5);
    for( ;; )
    {
        /* Task code goes here. */
			vTaskGetRunTimeStats( runTimeStatsBuff );
			vSerialPutString(Tx_Buf, 15);
			GPIO_write(PORT_0, PIN6, PIN_IS_LOW);
			vTaskDelayUntil( &xLastWakeTime, 100 );
			GPIO_write(PORT_0, PIN6, PIN_IS_HIGH);
		}
			
			

}
void Uart_Receiver ( void * pvParameters )
{
	 TickType_t xLastWakeTime= xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *) 6);
    for( ;; )
    {
      vTaskGetRunTimeStats( runTimeStatsBuff );
			
			xSerialGetChar(Rx_Buf);

			GPIO_write(PORT_0, PIN7, PIN_IS_LOW);
			vTaskDelayUntil( &xLastWakeTime, 20 );
			GPIO_write(PORT_0, PIN7, PIN_IS_HIGH);
		}
			
			
}
void Load_1_Simulation (void * pvParameters)
{
	int i=0;
	TickType_t xLastWakeTime= xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *) 1);//1 is voltage scale pxTaskTag=2, Null is current task
    for( ;; )
    {
        /* Task code goes here. */
			/*execution time 5 ms*/
			for(i=0;i<2000;i++)
			{
				i=i;
				
				GPIO_write(PORT_0, PIN3, PIN_IS_HIGH);
				GPIO_write(PORT_0, PIN2, PIN_IS_LOW);
				GPIO_write(PORT_0, PIN4, PIN_IS_LOW);
				
			}
			vTaskGetRunTimeStats(runTimeStatsBuff);//save run time stats in buf
			
			vTaskDelayUntil(&xLastWakeTime,10);
			//xSerialPutChar('\n');
			//vSerialPutString((const signed char *)runTimeStatsBuff,200);
				
	
		}

}
void Load_2_Simulation (void * pvParameters)
{
	int i=0;
	TickType_t xLastWakeTime= xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *) 1);//1 is voltage scale pxTaskTag=2, Null is current task
    for( ;; )
    {
        /* Task code goes here. */
			/*execution time 12 ms*/
			for(i=0;i<4000;i++)
			{
				i=i;
				
				GPIO_write(PORT_0, PIN4, PIN_IS_HIGH);
				GPIO_write(PORT_0, PIN2, PIN_IS_LOW);
				GPIO_write(PORT_0, PIN3, PIN_IS_LOW);
		
			}
			
			vTaskGetRunTimeStats(runTimeStatsBuff);//save run time stats in buf
			
	    vTaskDelayUntil(&xLastWakeTime,100);
			
				
		}

}
	
	
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */
xTaskPeriodicCreate(
                    Button_1_Monitor,       /* Function that implements the task. */
                    "Button_1_Monitor",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Button_1_Monitor_handler, Button_1_Monitor_Period);      /* Used to pass out the created task's handle. */
										
										   /* Create Tasks here */
xTaskPeriodicCreate(
                    Button_2_Monitor,       /* Function that implements the task. */
                    "Button_2_Monitor",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Button_2_Monitor_handler,Button_2_Monitor_Period );      /* Used to pass out the created task's handle. */
xTaskPeriodicCreate(
                    Periodic_Transmitter,       /* Function that implements the task. */
                    "Periodic_Transmitter",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Periodic_Transmitter_handler,Periodic_Transmitter_period );      /* Used to pass out the created task's handle. */									
xTaskPeriodicCreate(
                    Uart_Receiver,       /* Function that implements the task. */
                    "Uart_Receiver",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Uart_Receiver_handler,Uart_Receiver_period );      /* Used to pass out the created task's handle. */
										
xTaskPeriodicCreate(
                    Load_1_Simulation,       /* Function that implements the task. */
                    "Load_1_Simulation",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Load_1_Simulation_handler,Load_1_Simulation_period );      /* Used to pass out the created task's handle. */
xTaskPeriodicCreate(
                    Load_2_Simulation,       /* Function that implements the task. */
                    "Load_2_Simulation",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Load_2_Simulation_handler,Load_2_Simulation_period );      /* Used to pass out the created task's handle. */

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/
void vApplicationTickHook( void )
{
	GPIO_write(PORT_0, PIN5, PIN_IS_HIGH);
	
	GPIO_write(PORT_0, PIN5, PIN_IS_LOW);
	

}
void vApplicationIdleHook( void )
{
	
	GPIO_write(PORT_0, PIN2, PIN_IS_HIGH);
	GPIO_write(PORT_0, PIN3, PIN_IS_LOW);
	GPIO_write(PORT_0, PIN4, PIN_IS_LOW);
}
/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


