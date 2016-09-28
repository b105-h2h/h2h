/*
 * command_interpreter.h
 *
 *  Created on: 22/6/2015
 *      Author: Tomas Funk
 */
#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#include <stdint.h>
#include <string.h>

#define MAX_NUM_OF_PARAMETERS           (25) /* Denotes the max number of     */
                                             /* parameters a command can have.*/
	
#define MAX_SUPPORTED_COMMANDS          (36) /* Denotes the maximum number of */
                                             /* User Commands that are by this*/
                                             /* application.                  */
		
#define ERROR_MAX_COMMAND               (-1) /*Error triggered when no more   */
                                             /* commands can be added         */

#define ERROR_COMMAND_NOT_FOUND         (-2) /*Error triggered when current   */
                                             /* command can't be found        */
   /* The following type definition represents the structure which holds*/
   /* all information about the parameter, in particular the parameter  */
   /* as a string and the parameter as an unsigned int.                 */
typedef struct _tagParameter_t
{
   char     *strParam;
   int32_t  intParam;
} Parameter_t;

   /* The following type definition represents the structure which holds*/
   /* a list of parameters that are to be associated with a command The */
   /* NumberofParameters variable holds the value of the number of      */
   /* parameters in the list.                                           */
typedef struct _tagParameterList_t
{
   int         NumberofParameters;
   Parameter_t Params[MAX_NUM_OF_PARAMETERS];
} ParameterList_t;

   /* The following type definition represents the structure which holds*/
   /* the command and parameters to be executed.                        */
typedef struct _tagUserCommand_t
{
   char            *Command;
   ParameterList_t  Parameters;
} UserCommand_t;

   /* The following type definition represents the generic function     */
   /* pointer to be used by all commands that can be executed by the    */
   /* test program.                                                     */
typedef int (*CommandFunction_t)(ParameterList_t *TempParam);

   /* The following type definition represents the structure which holds*/
   /* information used in the interpretation and execution of Commands. */
typedef struct _tagCommandTable_t
{
   char              *CommandName;
   CommandFunction_t  CommandFunction;
} CommandTable_t;

int CommandInterpreter(UserCommand_t *TempCommand);
int AddCommand(char *CommandName, CommandFunction_t CommandFunction);
int ClearOneCommand(char *Command);
void ClearCommands(void);

#endif /* COMMAND_INTERPRETER_H */