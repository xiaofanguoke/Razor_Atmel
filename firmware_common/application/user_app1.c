/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserAppSM_Idle(void) 
{ 
  static u32 u32Counter = 0; 
  static bool isOn = 0;
  static u16 ledColor = 0;
  /* Increment u32Counter every 1ms cycle */ 
  u32Counter++; 
  /* Check and roll over */ 
  if(u32Counter == COUNTER_LIMIT_MS) 
  { 
    u32Counter = 0; 
    
    if(isOn)
    {
      //HEARTBEAT_OFF();
      
      LedOff(ledColor);
      
      ledColor++;
      
      if(ledColor == 8)
      {
        ledColor = 0;
      }
    }
    else
    {
      HEARTBEAT_ON();
      
      LedOn(ledColor);
    }
    
    
    isOn = ~isOn;
  } 
} /* end UserAppSM_Idle() */

static void test(void) 
{ 
   static u8 u8ColorIndex = 0; 
   static u16 u16BlinkCount = 0; 
   static u8 u8Counter = 0; 

   u16BlinkCount++; 
   if(u16BlinkCount == 500) 
   { 
     u16BlinkCount = 0; 
     /* Update the counter and roll at 16 */ 
     u8Counter++; 
     if(u8Counter == 2) 
     { 
         u8Counter = 0; 
         /* Manage the back light color */ 
         u8ColorIndex++; 
         if(u8ColorIndex == 7) 
         { 
            u8ColorIndex = 0; 
         } 
         /* Set the backlight color: white (all), purple (blue + red), blue, 
        cyan (blue + green), 
         green, yellow (green + red), red */ 
         switch(u8ColorIndex) 
         { 
         case 0: /* white */ 
         LedOn(LCD_RED); 
         LedOn(LCD_GREEN); 
         LedOn(LCD_BLUE); 
         break; 
         case 1: /* purple */ 
         LedOn(LCD_RED); 
         LedOff(LCD_GREEN); 
         LedOn(LCD_BLUE); 
         break; 
         case 2: /* blue */ 
         LedOff(LCD_RED); 
         LedOff(LCD_GREEN); 
         LedOn(LCD_BLUE); 
         break; 
         case 3: /* cyan */ 
         LedOff(LCD_RED); 
         LedOn(LCD_GREEN); 
         LedOn(LCD_BLUE); 
         break; 
         case 4: /* green */ 
         LedOff(LCD_RED); 
         LedOn(LCD_GREEN); 
         LedOff(LCD_BLUE); 
         break; 
         case 5: /* yellow */ 
         LedOn(LCD_RED); 
         LedOn(LCD_GREEN); 
         LedOff(LCD_BLUE); 
         break; 
         case 6: /* red */ 
         LedOn(LCD_RED); 
         LedOff(LCD_GREEN); 
         LedOff(LCD_BLUE); 
         break; 
         default: /* off */ 
         LedOff(LCD_RED); 
         LedOff(LCD_GREEN); 
         LedOff(LCD_BLUE); 
         break; 
         } /* end switch */ 
      } /* end if(u8Counter == 16) */ 
   } /* end if(u16BlinkCount == 500) */
} /* end demo() */

static void demo2()
{
  static u16 u16BlinkCount = 0;
  static u8 u8Counter = 0;
  static u8 u8ColorIndex = 0;
  u16BlinkCount++;
  if(u16BlinkCount == 500)
  {
    u16BlinkCount = 0;
    /* Update the counter and roll at 16 */
    u8Counter++;
    if(u8Counter == 16)
    {
      u8Counter = 0;
      LedOff((LedNumberType)(3 + (4 * u8ColorIndex)));
      LedOff((LedNumberType)(2 + (4 * u8ColorIndex)));
      LedOff((LedNumberType)(1 + (4 * u8ColorIndex)));
      LedOff((LedNumberType)(0 + (4 * u8ColorIndex)));
      u8ColorIndex++;
      if(u8ColorIndex == 3)
      {
        u8ColorIndex = 0;
      }
    } /* end if(u8Counter == 16) */
    /* Parse the current count to set the LEDs. From leds.h we see the enum
    for red, green and blue
    are seperated by 4 so use this with u8ColorIndex to */
    if(u8Counter & 0x01)
    {
      LedOn(3 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(3 + (4 * u8ColorIndex));
    }
      if(u8Counter & 0x02)
    {
      LedOn(2 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(2 + (4 * u8ColorIndex));
    }
    if(u8Counter & 0x04)
    {
      LedOn(1 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(1 + (4 * u8ColorIndex));
    }
    if(u8Counter & 0x08)
    {
      LedOn(0 + (4 * u8ColorIndex));
    }
    else
    {
      LedOff(0 + (4 * u8ColorIndex));
    }
  } /* end if(u16BlinkCount == 500) */
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
 
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

  //UserAppSM_Idle();
  test();
  //demo();
} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{

} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
