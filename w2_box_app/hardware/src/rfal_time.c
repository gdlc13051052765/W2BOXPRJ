#include "rfal_time.h"

 
/*
******************************************************************************
* LOCAL DEFINES
******************************************************************************
*/

/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/

static uint32_t timerStopwatchTick;

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/


/*******************************************************************************/
uint32_t timerCalculateTimer( uint16_t time )
{  
  return (platformGetSysTick() + time);
}


/*******************************************************************************/
uint8_t timerIsExpired( uint32_t timer )
{
  uint32_t uDiff;
  int32_t sDiff;
  
  uDiff = (timer - platformGetSysTick());   /* Calculate the diff between the timers */
  sDiff = uDiff;                            /* Convert the diff to a signed var      */
  
  /* Check if the given timer has expired already */
  if( sDiff < 0 )
  {
    return 1;
  }
  
  return 0;
}


/*******************************************************************************/
void timerDelay( uint16_t tOut )
{
  uint32_t t;
  
  /* Calculate the timer and wait blocking until is running */
  t = timerCalculateTimer( tOut );
  while( timerIsRunning(t) );
}


/*******************************************************************************/
void timerStopwatchStart( void )
{
  timerStopwatchTick = platformGetSysTick();
}


/*******************************************************************************/
uint32_t timerStopwatchMeasure( void )
{
  return (uint32_t)(platformGetSysTick() - timerStopwatchTick);
}


