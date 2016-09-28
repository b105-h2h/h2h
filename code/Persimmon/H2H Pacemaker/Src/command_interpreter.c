/*
 * bt_const_and_struct.h
 *
 *  Created on: 22/6/2015
 *      Author: Tomas Funk
 */

#include "command_interpreter.h"

CommandTable_t      CommandTable[MAX_SUPPORTED_COMMANDS]; /* Variable which is */
                                            /* used to hold the actual Commands*/
                                            /* that are supported by this      */
                                            /* application.                    */

unsigned int        NumberCommands = 0;      /* Variable which is used to hold  */
                                             /* the number of Commands that are */
                                             /* supported by this application.  */
                                             /* Commands are added individually.*/

static CommandFunction_t FindCommand(char *Command)
{
  uint16_t          Index;
  CommandFunction_t ret_val = NULL;

  /* Loop through each element looking for a match */
  for(Index=0; Index < NumberCommands; Index++)
  {
    if((strlen(CommandTable[Index].CommandName) == strlen(Command)) 
       && (strcmp(Command, CommandTable[Index].CommandName) == 0))
    {
      ret_val = CommandTable[Index].CommandFunction;
    }
  }

  return(ret_val);
}
int AddCommand(char *CommandName, CommandFunction_t CommandFunction)
{
  int ret_val;
  
  if( NumberCommands < MAX_SUPPORTED_COMMANDS){
    strcpy(CommandTable[NumberCommands].CommandName, CommandName);
    CommandTable[NumberCommands].CommandFunction = CommandFunction;
    NumberCommands++;
    ret_val = 0;
  }else{
    ret_val = ERROR_MAX_COMMAND;
  }
  return ret_val;
}

int CommandInterpreter(UserCommand_t *TempCommand)
{
  int ret_val;
  CommandFunction_t CommandFunction;
  
  CommandFunction = FindCommand(TempCommand->Command);
  
  if( CommandFunction != NULL)
  {
    ret_val = (*CommandFunction)(&(TempCommand->Parameters));
  }
  else
  {
    ret_val = ERROR_COMMAND_NOT_FOUND;
  }
  return ret_val;
}

/* Clear one specific command */ 
int ClearOneCommand(char *Command)
{
  uint16_t      Index;
  int           ret_val = ERROR_COMMAND_NOT_FOUND;
  
  /* Loop through each element looking for a match */
  for(Index=0; Index < NumberCommands; Index++)
  {
    /* When we find it, we copy the last added command to the position of the */
    /* deleted command, deleting the command and updating the table at the    */
    /* same time ( double efficiency , unic sensation ) */
    if((strlen(CommandTable[Index].CommandName) == strlen(Command)) 
       && (strcmp(Command, CommandTable[Index].CommandName) == 0))
    {
      NumberCommands--;
      if( NumberCommands > 0)
      {
        strcpy(CommandTable[Index].CommandName, CommandTable[NumberCommands].CommandName);
        CommandTable[Index].CommandFunction = CommandTable[NumberCommands].CommandFunction;
      }
      else
      {
        strcpy(CommandTable[Index].CommandName, "");
        CommandTable[Index].CommandFunction = NULL;
      }

      ret_val = 0;
    }
  }

  return(ret_val);
}
/* Clears all the commands of the table */
void ClearCommands(void)
{
  uint16_t          Index;
  
  for(Index=0; Index < NumberCommands; Index++)
  {
    strcpy(CommandTable[NumberCommands].CommandName, "");
    CommandTable[NumberCommands].CommandFunction = NULL;
  }
    NumberCommands = 0;
}