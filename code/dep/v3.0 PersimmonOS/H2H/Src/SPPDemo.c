/*****< sppdemo.c >************************************************************/
/*      Copyright 2011 - 2014 Stonestreet One.                                */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*      Copyright 2015 Texas Instruments Incorporated.                        */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*  SPPDEMO - Simple embedded application using SPP Profile.                  */
/*                                                                            */
/*  Author:  Tim Cook                                                         */
/*                                                                            */
/*** MODIFICATION HISTORY *****************************************************/
/*                                                                            */
/*   mm/dd/yy  F. Lastname    Description of Modification                     */
/*   --------  -----------    ------------------------------------------------*/
/*   01/24/11  T. Cook        Initial creation.                               */
/*   11/24/14  R. Malovany    Update.                                         */
/*   03/03/15  D. Horowitz    Adding Demo Application version.                */
/******************************************************************************/
//#include "Main.h"                /* Application Interface Abstraction.        */
#include "SS1BTPS.h"             /* Main SS1 Bluetooth Stack Header.          */
#include "SPPDemo.h"             /* Application Header.                       */
#include "BTPSKRNL.h"            /* BTPS Kernel Header.                       */
#include "BTVSAPI.h"             /* Vendror Specific header.                  */

#include "stm32l1xx_hal.h"



   /* Internal function prototypes.                                     */
//static void UserInterface_Client(void);
//static void UserInterface_Server(void);
//static void UserInterface_Selection(void);
//static Boolean_t CommandLineInterpreter(char *Command);
//static unsigned long StringToUnsignedInteger(char *StringInteger);
//static char *StringParser(char *String);
//static int CommandParser(UserCommand_t *TempCommand, char *Input);
//static int CommandInterpreter(UserCommand_t *TempCommand);
//static int AddCommand(char *CommandName, CommandFunction_t CommandFunction);
//static CommandFunction_t FindCommand(char *Command);
//static void ClearCommands(void);

//static void BD_ADDRToStr(BD_ADDR_t Board_Address, char *BoardStr);
//static void DisplayIOCapabilities(void);
//static void DisplayClassOfDevice(Class_of_Device_t Class_of_Device);
//static void DisplayPrompt(void);
//static void DisplayUsage(char *UsageString);
//static void DisplayFunctionError(char *Function,int Status);
//static void DisplayFunctionSuccess(char *Function);
//static void DisplayFWVersion (void);

//static int OpenStack(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization);
//static int CloseStack(void);
//
//static int SetDisc(void);
//static int SetConnect(void);
//static int SetPairable(void);
//static int DeleteLinkKey(BD_ADDR_t BD_ADDR);
//
////static int DisplayHelp(ParameterList_t *TempParam);
////static int QueryMemory(ParameterList_t *TempParam);
//static int Inquiry(ParameterList_t *TempParam);
////static int DisplayInquiryList(ParameterList_t *TempParam);
//static int SetDiscoverabilityMode(ParameterList_t *TempParam);
//static int SetConnectabilityMode(ParameterList_t *TempParam);
//static int SetPairabilityMode(ParameterList_t *TempParam);
//static int ChangeSimplePairingParameters(ParameterList_t *TempParam);
//static int Pair(ParameterList_t *TempParam);
//static int EndPairing(ParameterList_t *TempParam);
//static int PINCodeResponse(ParameterList_t *TempParam);
//static int PassKeyResponse(ParameterList_t *TempParam);
//static int UserConfirmationResponse(ParameterList_t *TempParam);
//static int GetLocalAddress(ParameterList_t *TempParam);
//static int SetLocalName(ParameterList_t *TempParam);
//static int GetLocalName(ParameterList_t *TempParam);
//static int SetClassOfDevice(ParameterList_t *TempParam);
//static int GetClassOfDevice(ParameterList_t *TempParam);
//static int GetRemoteName(ParameterList_t *TempParam);
//static int SniffMode(ParameterList_t *TempParam);
//static int ExitSniffMode(ParameterList_t *TempParam);
//
//static int OpenServer(ParameterList_t *TempParam);
//static int CloseServer(ParameterList_t *TempParam);
//static int OpenRemoteServer(ParameterList_t *TempParam);
//static int CloseRemoteServer(ParameterList_t *TempParam);
//static int Read(ParameterList_t *TempParam);
//static int Write(ParameterList_t *TempParam);
//static int GetConfigParams(ParameterList_t *TempParam);
//static int SetConfigParams(ParameterList_t *TempParam);
//static int GetQueueParams(ParameterList_t *TempParam);
//static int SetQueueParams(ParameterList_t *TempParam);
//static int Loopback(ParameterList_t *TempParam);
//static int DisplayRawModeData(ParameterList_t *TempParam);
//static int AutomaticReadMode(ParameterList_t *TempParam);
//static int SetBaudRate(ParameterList_t *TempParam);
//static int SendData(ParameterList_t *TempParam);
//
//static int ServerMode(ParameterList_t *TempParam);
//static int ClientMode(ParameterList_t *TempParam);


//   /* This function is responsible for taking the input from the user   */
//   /* and dispatching the appropriate Command Function.  First, this    */
//   /* function retrieves a String of user input, parses the user input  */
//   /* into Command and Parameters, and finally executes the Command or  */
//   /* Displays an Error Message if the input is not a valid Command.    */
//static void UserInterface_Client(void)
//{
//      /* Next display the available commands.                           */
//      DisplayHelp(NULL);
//
//      /* Clear the installed command.                                   */
//      ClearCommands();
//
//      AddCommand("INQUIRY", Inquiry);
//      AddCommand("DISPLAYINQUIRYLIST", DisplayInquiryList);
//      AddCommand("PAIR", Pair);
//      AddCommand("ENDPAIRING", EndPairing);
//      AddCommand("PINCODERESPONSE", PINCodeResponse);
//      AddCommand("PASSKEYRESPONSE", PassKeyResponse);
//      AddCommand("USERCONFIRMATIONRESPONSE", UserConfirmationResponse);
//      AddCommand("SETDISCOVERABILITYMODE", SetDiscoverabilityMode);
//      AddCommand("SETCONNECTABILITYMODE", SetConnectabilityMode);
//      AddCommand("SETPAIRABILITYMODE", SetPairabilityMode);
//      AddCommand("CHANGESIMPLEPAIRINGPARAMETERS", ChangeSimplePairingParameters);
//      AddCommand("GETLOCALADDRESS", GetLocalAddress);
//      AddCommand("SETLOCALNAME", SetLocalName);
//      AddCommand("GETLOCALNAME", GetLocalName);
//      AddCommand("SETCLASSOFDEVICE", SetClassOfDevice);
//      AddCommand("GETCLASSOFDEVICE", GetClassOfDevice);
//      AddCommand("GETREMOTENAME", GetRemoteName);
//      AddCommand("SNIFFMODE", SniffMode);
//      AddCommand("EXITSNIFFMODE", ExitSniffMode);
//      AddCommand("OPEN", OpenRemoteServer);
//      AddCommand("CLOSE", CloseRemoteServer);
//      AddCommand("READ", Read);
//      AddCommand("WRITE", Write);
//      AddCommand("GETCONFIGPARAMS", GetConfigParams);
//      AddCommand("SETCONFIGPARAMS", SetConfigParams);
//      AddCommand("GETQUEUEPARAMS", GetQueueParams);
//      AddCommand("SETQUEUEPARAMS", SetQueueParams);
//      AddCommand("LOOPBACK", Loopback);
//      AddCommand("DISPLAYRAWMODEDATA", DisplayRawModeData);
//      AddCommand("AUTOMATICREADMODE", AutomaticReadMode);
//      AddCommand("SETBAUDRATE", SetBaudRate);
//      AddCommand("SEND", SendData);
//      AddCommand("HELP", DisplayHelp);
//      AddCommand("QUERYMEMORY", QueryMemory);
//}
//
//   /* This function is responsible for taking the input from the user   */
//   /* and dispatching the appropriate Command Function.  First, this    */
//   /* function retrieves a String of user input, parses the user input  */
//   /* into Command and Parameters, and finally executes the Command or  */
//   /* Displays an Error Message if the input is not a valid Command.    */
//static void UserInterface_Server(void)
//{ 
//      /* Next display the available commands.                           */
//      DisplayHelp(NULL);
//
//      /* Clear the installed command.                                   */
//      ClearCommands();
//
//      /* Install the commands revelant for this UI.                     */
//      AddCommand("INQUIRY", Inquiry);
//      AddCommand("DISPLAYINQUIRYLIST", DisplayInquiryList);
//      AddCommand("PAIR", Pair);
//      AddCommand("ENDPAIRING", EndPairing);
//      AddCommand("PINCODERESPONSE", PINCodeResponse);
//      AddCommand("PASSKEYRESPONSE", PassKeyResponse);
//      AddCommand("USERCONFIRMATIONRESPONSE", UserConfirmationResponse);
//      AddCommand("SETDISCOVERABILITYMODE", SetDiscoverabilityMode);
//      AddCommand("SETCONNECTABILITYMODE", SetConnectabilityMode);
//      AddCommand("SETPAIRABILITYMODE", SetPairabilityMode);
//      AddCommand("CHANGESIMPLEPAIRINGPARAMETERS", ChangeSimplePairingParameters);
//      AddCommand("GETLOCALADDRESS", GetLocalAddress);
//      AddCommand("SETLOCALNAME", SetLocalName);
//      AddCommand("GETLOCALNAME", GetLocalName);
//      AddCommand("SETCLASSOFDEVICE", SetClassOfDevice);
//      AddCommand("GETCLASSOFDEVICE", GetClassOfDevice);
//      AddCommand("GETREMOTENAME", GetRemoteName);
//      AddCommand("SNIFFMODE", SniffMode);
//      AddCommand("EXITSNIFFMODE", ExitSniffMode);
//      AddCommand("OPEN", OpenServer);
//      AddCommand("CLOSE", CloseServer);
//      AddCommand("READ", Read);
//      AddCommand("WRITE", Write);
//      AddCommand("GETCONFIGPARAMS", GetConfigParams);
//      AddCommand("SETCONFIGPARAMS", SetConfigParams);
//      AddCommand("GETQUEUEPARAMS", GetQueueParams);
//      AddCommand("SETQUEUEPARAMS", SetQueueParams);
//      AddCommand("LOOPBACK", Loopback);
//      AddCommand("DISPLAYRAWMODEDATA", DisplayRawModeData);
//      AddCommand("AUTOMATICREADMODE", AutomaticReadMode);
//      AddCommand("SETBAUDRATE", SetBaudRate);
//      AddCommand("SEND", SendData);
//      AddCommand("HELP", DisplayHelp);    
//      AddCommand("QUERYMEMORY", QueryMemory);
//}
//
//   /* The following function is responsible for choosing the user       */
//   /* interface to present to the user.                                 */
//static void UserInterface_Selection(void)
//{
//   /* Next display the available commands.                              */
//   DisplayHelp(NULL);
//
//   ClearCommands();
//
//   AddCommand("SERVER", ServerMode);
//   AddCommand("CLIENT", ClientMode);
//   AddCommand("HELP", DisplayHelp);
//}
//
//   /* The following function is responsible for parsing user input      */
//   /* and call appropriate command function.                            */
//static Boolean_t CommandLineInterpreter(char *Command)
//{
//   int           Result = !EXIT_CODE;
//   Boolean_t     ret_val = FALSE;
//   UserCommand_t TempCommand;
//
//   /* The string input by the user contains a value, now run the string */
//   /* through the Command Parser.                                       */
//       if(CommandParser(&TempCommand, Command) >= 0)
//         {
//            Display(("\r\n"));
//
//            /* The Command was successfully parsed run the Command.           */
//            Result = CommandInterpreter(&TempCommand);
//            switch(Result)
//            {
//               case INVALID_COMMAND_ERROR:
//                  Display(("Invalid Command: %s.\r\n",TempCommand.Command));
//                  break;
//               case FUNCTION_ERROR:
//                  Display(("Function Error.\r\n"));
//                  break;
//               case EXIT_CODE:
//            if(ServerPortID)
//               CloseServer(NULL);
//            else
//            {
//               if(SerialPortID)
//                  CloseRemoteServer(NULL);
//            }
//
//            /* Restart the User Interface Selection.                    */
//            UI_Mode = UI_MODE_SELECT;
//            /* Set up the Selection Interface.                          */
//            UserInterface_Selection();
//                  break;
//            }
//
//          /* Display a prompt.                                              */
//          DisplayPrompt();
//
//          ret_val = TRUE;
//         }
//       else
//        {
//          /* Display a prompt.                                              */
//          DisplayPrompt();
//
//          Display(("\r\nInvalid Command.\r\n"));
//        }
//    
//   return(ret_val);
//}
//
//   /* The following function is responsible for converting number       */
//   /* strings to there unsigned integer equivalent.  This function can  */
//   /* handle leading and tailing white space, however it does not handle*/
//   /* signed or comma delimited values.  This function takes as its     */
//   /* input the string which is to be converted.  The function returns  */
//   /* zero if an error occurs otherwise it returns the value parsed from*/
//   /* the string passed as the input parameter.                         */
//static unsigned long StringToUnsignedInteger(char *StringInteger)
//{
//   int          IsHex;
//   unsigned int Index;
//   unsigned long ret_val = 0;
//
//   /* Before proceeding make sure that the parameter that was passed as */
//   /* an input appears to be at least semi-valid.                       */
//   if((StringInteger) && (BTPS_StringLength(StringInteger)))
//   {
//      /* Initialize the variable.                                       */
//      Index = 0;
//
//      /* Next check to see if this is a hexadecimal number.             */
//      if(BTPS_StringLength(StringInteger) > 2)
//      {
//         if((StringInteger[0] == '0') && ((StringInteger[1] == 'x') || (StringInteger[1] == 'X')))
//         {
//            IsHex = 1;
//
//            /* Increment the String passed the Hexadecimal prefix.      */
//            StringInteger += 2;
//         }
//         else
//            IsHex = 0;
//      }
//      else
//         IsHex = 0;
//
//      /* Process the value differently depending on whether or not a    */
//      /* Hexadecimal Number has been specified.                         */
//      if(!IsHex)
//      {
//         /* Decimal Number has been specified.                          */
//         while(1)
//         {
//            /* First check to make sure that this is a valid decimal    */
//            /* digit.                                                   */
//            if((StringInteger[Index] >= '0') && (StringInteger[Index] <= '9'))
//            {
//               /* This is a valid digit, add it to the value being      */
//               /* built.                                                */
//               ret_val += (StringInteger[Index] & 0xF);
//
//               /* Determine if the next digit is valid.                 */
//               if(((Index + 1) < BTPS_StringLength(StringInteger)) && (StringInteger[Index+1] >= '0') && (StringInteger[Index+1] <= '9'))
//               {
//                  /* The next digit is valid so multiply the current    */
//                  /* return value by 10.                                */
//                  ret_val *= 10;
//               }
//               else
//               {
//                  /* The next value is invalid so break out of the loop.*/
//                  break;
//               }
//            }
//
//            Index++;
//         }
//      }
//      else
//      {
//         /* Hexadecimal Number has been specified.                      */
//         while(1)
//         {
//            /* First check to make sure that this is a valid Hexadecimal*/
//            /* digit.                                                   */
//            if(((StringInteger[Index] >= '0') && (StringInteger[Index] <= '9')) || ((StringInteger[Index] >= 'a') && (StringInteger[Index] <= 'f')) || ((StringInteger[Index] >= 'A') && (StringInteger[Index] <= 'F')))
//            {
//               /* This is a valid digit, add it to the value being      */
//               /* built.                                                */
//               if((StringInteger[Index] >= '0') && (StringInteger[Index] <= '9'))
//                  ret_val += (StringInteger[Index] & 0xF);
//               else
//               {
//                  if((StringInteger[Index] >= 'a') && (StringInteger[Index] <= 'f'))
//                     ret_val += (StringInteger[Index] - 'a' + 10);
//                  else
//                     ret_val += (StringInteger[Index] - 'A' + 10);
//               }
//
//               /* Determine if the next digit is valid.                 */
//               if(((Index + 1) < BTPS_StringLength(StringInteger)) && (((StringInteger[Index+1] >= '0') && (StringInteger[Index+1] <= '9')) || ((StringInteger[Index+1] >= 'a') && (StringInteger[Index+1] <= 'f')) || ((StringInteger[Index+1] >= 'A') && (StringInteger[Index+1] <= 'F'))))
//               {
//                  /* The next digit is valid so multiply the current    */
//                  /* return value by 16.                                */
//                  ret_val *= 16;
//               }
//               else
//               {
//                  /* The next value is invalid so break out of the loop.*/
//                  break;
//               }
//            }
//
//            Index++;
//         }
//      }
//   }
//
//   return(ret_val);
//}
//
//   /* The following function is responsible for parsing strings into    */
//   /* components.  The first parameter of this function is a pointer to */
//   /* the String to be parsed.  This function will return the start of  */
//   /* the string upon success and a NULL pointer on all errors.         */
//static char *StringParser(char *String)
//{
//   int   Index;
//   char *ret_val = NULL;
//
//   /* Before proceeding make sure that the string passed in appears to  */
//   /* be at least semi-valid.                                           */
//   if((String) && (BTPS_StringLength(String)))
//   {
//      /* The string appears to be at least semi-valid.  Search for the  */
//      /* first space character and replace it with a NULL terminating   */
//      /* character.                                                     */
//      for(Index=0, ret_val=String;Index < BTPS_StringLength(String);Index++)
//      {
//         /* Is this the space character.                                */
//         if((String[Index] == ' ') || (String[Index] == '\r') || (String[Index] == '\n'))
//         {
//            /* This is the space character, replace it with a NULL      */
//            /* terminating character and set the return value to the    */
//            /* beginning character of the string.                       */
//            String[Index] = '\0';
//            break;
//         }
//      }
//   }
//
//   return(ret_val);
//}
//
//   /* This function is responsible for taking command strings and       */
//   /* parsing them into a command, param1, and param2.  After parsing   */
//   /* this string the data is stored into a UserCommand_t structure to  */
//   /* be used by the interpreter.  The first parameter of this function */
//   /* is the structure used to pass the parsed command string out of the*/
//   /* function.  The second parameter of this function is the string    */
//   /* that is parsed into the UserCommand structure.  Successful        */
//   /* execution of this function is denoted by a return value of zero.  */
//   /* Negative return values denote an error in the parsing of the      */
//   /* string parameter.                                                 */
//static int CommandParser(UserCommand_t *TempCommand, char *Input)
//{
//   int            ret_val;
//   int            StringLength;
//   char          *LastParameter;
//   unsigned int   Count         = 0;
//
//   /* Before proceeding make sure that the passed parameters appear to  */
//   /* be at least semi-valid.                                           */
//   if((TempCommand) && (Input) && (BTPS_StringLength(Input)))
//   {
//      /* First get the initial string length.                           */
//      StringLength = BTPS_StringLength(Input);
//
//      /* Retrieve the first token in the string, this should be the     */
//      /* command.                                                       */
//      TempCommand->Command = StringParser(Input);
//
//      /* Flag that there are NO Parameters for this Command Parse.      */
//      TempCommand->Parameters.NumberofParameters = 0;
//
//       /* Check to see if there is a Command                            */
//      if(TempCommand->Command)
//      {
//         /* Initialize the return value to zero to indicate success on  */
//         /* commands with no parameters.                                */
//         ret_val    = 0;
//
//         /* Adjust the UserInput pointer and StringLength to remove the */
//         /* Command from the data passed in before parsing the          */
//         /* parameters.                                                 */
//         Input        += BTPS_StringLength(TempCommand->Command) + 1;
//         StringLength  = BTPS_StringLength(Input);
//
//         /* There was an available command, now parse out the parameters*/
//         while((StringLength > 0) && ((LastParameter = StringParser(Input)) != NULL))
//         {
//            /* There is an available parameter, now check to see if     */
//            /* there is room in the UserCommand to store the parameter  */
//            if(Count < (sizeof(TempCommand->Parameters.Params)/sizeof(Parameter_t)))
//            {
//               /* Save the parameter as a string.                       */
//               TempCommand->Parameters.Params[Count].strParam = LastParameter;
//
//               /* Save the parameter as an unsigned int intParam will   */
//               /* have a value of zero if an error has occurred.        */
//               TempCommand->Parameters.Params[Count].intParam = StringToUnsignedInteger(LastParameter);
//
//               Count++;
//               Input        += BTPS_StringLength(LastParameter) + 1;
//               StringLength -= BTPS_StringLength(LastParameter) + 1;
//
//               ret_val = 0;
//            }
//            else
//            {
//               /* Be sure we exit out of the Loop.                      */
//               StringLength = 0;
//
//               ret_val      = TO_MANY_PARAMS;
//            }
//         }
//
//         /* Set the number of parameters in the User Command to the     */
//         /* number of found parameters                                  */
//         TempCommand->Parameters.NumberofParameters = Count;
//      }
//      else
//      {
//         /* No command was specified                                    */
//         ret_val = NO_COMMAND_ERROR;
//      }
//   }
//   else
//   {
//      /* One or more of the passed parameters appear to be invalid.     */
//      ret_val = INVALID_PARAMETERS_ERROR;
//   }
//
//   return(ret_val);
//}
//
//   /* This function is responsible for determining the command in which */
//   /* the user entered and running the appropriate function associated  */
//   /* with that command.  The first parameter of this function is a     */
//   /* structure containing information about the command to be issued.  */
//   /* This information includes the command name and multiple parameters*/
//   /* which maybe be passed to the function to be executed.  Successful */
//   /* execution of this function is denoted by a return value of zero.  */
//   /* A negative return value implies that command was not found and is */
//   /* invalid.                                                          */
//static int CommandInterpreter(UserCommand_t *TempCommand)
//{
//   int               i;
//   int               ret_val;
//   CommandFunction_t CommandFunction;
//
//   /* If the command is not found in the table return with an invalid   */
//   /* command error                                                     */
//   ret_val = INVALID_COMMAND_ERROR;
//
//   /* Let's make sure that the data passed to us appears semi-valid.    */
//   if((TempCommand) && (TempCommand->Command))
//   {
//      /* Now, let's make the Command string all upper case so that we   */
//      /* compare against it.                                            */
//      for(i=0;i<BTPS_StringLength(TempCommand->Command);i++)
//      {
//         if((TempCommand->Command[i] >= 'a') && (TempCommand->Command[i] <= 'z'))
//            TempCommand->Command[i] -= ('a' - 'A');
//      }
//
//      /* Check to see if the command which was entered was exit.        */
//      if(BTPS_MemCompare(TempCommand->Command, "QUIT", BTPS_StringLength("QUIT")) != 0)
//      {
//         /* The command entered is not exit so search for command in    */
//         /* table.                                                      */
//         if((CommandFunction = FindCommand(TempCommand->Command)) != NULL)
//         {
//            /* The command was found in the table so call the command.  */
//            if((ret_val = ((*CommandFunction)(&TempCommand->Parameters))) == 0)
//            {
//               /* Return success to the caller.                         */
//               ret_val = 0;
//            }
//            else
//            {
//               if((ret_val != EXIT_CODE) && (ret_val != EXIT_MODE))
//                  ret_val = FUNCTION_ERROR;
//            }
//         }
//      }
//      else
//      {
//         /* The command entered is exit, set return value to EXIT_CODE  */
//         /* and return.                                                 */
//         ret_val = EXIT_CODE;
//      }
//   }
//   else
//      ret_val = INVALID_PARAMETERS_ERROR;
//
//   return(ret_val);
//}
//
//   /* The following function is provided to allow a means to            */
//   /* programatically add Commands the Global (to this module) Command  */
//   /* Table.  The Command Table is simply a mapping of Command Name     */
//   /* (NULL terminated ASCII string) to a command function.  This       */
//   /* function returns zero if successful, or a non-zero value if the   */
//   /* command could not be added to the list.                           */
//static int AddCommand(char *CommandName, CommandFunction_t CommandFunction)
//{
//   int ret_val = 0;
//
//   /* First, make sure that the parameters passed to us appear to be    */
//   /* semi-valid.                                                       */
//   if((CommandName) && (CommandFunction))
//   {
//      /* Next, make sure that we still have room in the Command Table   */
//      /* to add commands.                                               */
//      if(NumberCommands < MAX_SUPPORTED_COMMANDS)
//      {
//         /* Simply add the command data to the command table and        */
//         /* increment the number of supported commands.                 */
//         CommandTable[NumberCommands].CommandName       = CommandName;
//         CommandTable[NumberCommands++].CommandFunction = CommandFunction;
//
//         /* Return success to the caller.                               */
//         ret_val                                        = 0;
//      }
//      else
//         ret_val = 1;
//   }
//   else
//      ret_val = 1;
//
//   return(ret_val);
//}
//
//   /* The following function searches the Command Table for the         */
//   /* specified Command.  If the Command is found, this function returns*/
//   /* a NON-NULL Command Function Pointer.  If the command is not found */
//   /* this function returns NULL.                                       */
//static CommandFunction_t FindCommand(char *Command)
//{
//   unsigned int      Index;
//   CommandFunction_t ret_val;
//
//   /* First, make sure that the command specified is semi-valid.        */
//   if(Command)
//   {
//      /* Now loop through each element in the table to see if there is  */
//      /* a match.                                                       */
//      for(Index=0, ret_val = NULL; ((Index<NumberCommands) && (!ret_val)); Index++)
//      {
//         if((BTPS_StringLength(CommandTable[Index].CommandName) == BTPS_StringLength(Command)) && (BTPS_MemCompare(Command, CommandTable[Index].CommandName, BTPS_StringLength(CommandTable[Index].CommandName)) == 0))
//            ret_val = CommandTable[Index].CommandFunction;
//      }
//   }
//   else
//      ret_val = NULL;
//
//   return(ret_val);
//}
//
//   /* The following function is provided to allow a means to clear out  */
//   /* all available commands from the command table.                    */
//static void ClearCommands(void)
//{
//   /* Simply flag that there are no commands present in the table.      */
//   NumberCommands = 0;
//}


//   /* Displays the current I/O Capabilities.                            */
//static void DisplayIOCapabilities(void)
//{
//   Display(("I/O Capabilities: %s, MITM: %s.\r\n", IOCapabilitiesStrings[(unsigned int)IOCapability], MITMProtection?"TRUE":"FALSE"));
//}

//   /* Utility function to display a Class of Device Structure.          */
//static void DisplayClassOfDevice(Class_of_Device_t Class_of_Device)
//{
//   Display(("Class of Device: 0x%02X%02X%02X.\r\n", Class_of_Device.Class_of_Device0, Class_of_Device.Class_of_Device1, Class_of_Device.Class_of_Device2));
//}

//   /* Displays the correct prompt depending on the Server/Client Mode.  */
//static void DisplayPrompt(void)
//{
//   if(UI_Mode == UI_MODE_IS_CLIENT)
//      Display(("\r\nClient> \b"));
//   else
//   {
//      if(UI_Mode == UI_MODE_IS_SERVER)
//         Display(("\r\nServer> \b"));
//      else
//         Display(("\r\nChoose Mode> \b"));
//   }
//}

//   /* Displays a usage string..                                         */
//static void DisplayUsage(char *UsageString)
//{
//   Display(("\nUsage: %s.\r\n", UsageString));
//}

//   /* Displays a function error message.                                */
//static void DisplayFunctionError(char *Function, int Status)
//{
//   Display(("\n%s Failed: %d.\r\n", Function, Status));
//}

//   /* Displays a function success message.                              */
//static void DisplayFunctionSuccess(char *Function)
//{
//   Display(("\n%s success.\r\n", Function));
//}
//   /* The following function is for displaying The FW Version by reading*/
//   /* The Local version information form the FW.                        */
//static void DisplayFWVersion (void)
//{
//    FW_Version FW_Version_Details;
//    
//    /* This function retrieves the Local Version Information of the FW. */    
//    HCI_Read_Local_Version_Information(BluetoothStackID, &FW_Version_Details.StatusResult, &FW_Version_Details.HCI_VersionResult, &FW_Version_Details.HCI_RevisionResult, &FW_Version_Details.LMP_VersionResult, &FW_Version_Details.Manufacturer_NameResult, &FW_Version_Details.LMP_SubversionResult);
//    if (!FW_Version_Details.StatusResult)
//    {
//        /* This function prints The project type from firmware, Bits    */
//        /* 10 to 14 (5 bits) from LMP_SubversionResult parameter.       */
//        Display(("Project Type  : %d \r\n", ((FW_Version_Details.LMP_SubversionResult >> 10)) & 0x1F));
//        /* This function prints The version of the firmware. The first  */
//        /* number is the Major version, Bits 7 to 9 and 15 (4 bits) from*/
//        /* LMP_SubversionResult parameter, the second number is the     */
//        /* Minor Version, Bits 0 to 6 (7 bits) from LMP_SubversionResult*/
//        /* parameter.                                                   */
//        Display(("FW Version    : %d.%d \r\n", ((FW_Version_Details.LMP_SubversionResult >> 7) & 0x07) + ((FW_Version_Details.LMP_SubversionResult >> 12) & 0x08), FW_Version_Details.LMP_SubversionResult & 0x7F));
//    }
//    else
//        /* There was an error with HCI_Read_Local_Version_Information.  */
//        /* Function.                                                    */
//        DisplayFunctionError("HCI_Read_Local_Version_Information", FW_Version_Details.StatusResult);
//}


//   /* The following function is responsible for querying the memory     */
//   /* heap usage. This function will return zero on successful          */  
//   /* execution and a negative value on errors.                         */
//static int QueryMemory(ParameterList_t *TempParam)
//{
//   BTPS_MemoryStatistics_t MemoryStatistics;
//   int ret_val;
//
//   /* Get current memory buffer usage                                   */
//   ret_val = BTPS_QueryMemoryUsage(&MemoryStatistics, TRUE);
//   if(!ret_val)
//   {
//      Display(("\r\n"));
//      Display(("Heap Size:                %5d bytes\r\n", MemoryStatistics.HeapSize));
//      Display(("Current Memory Usage:\r\n"));
//      Display(("   Used:                  %5d bytes\r\n", MemoryStatistics.CurrentHeapUsed));
//      Display(("   Free:                  %5d bytes\r\n", MemoryStatistics.HeapSize - MemoryStatistics.CurrentHeapUsed));
//      Display(("Maximum Memory Usage:\r\n"));
//      Display(("   Used:                  %5d bytes\r\n", MemoryStatistics.MaximumHeapUsed));
//      Display(("   Free:                  %5d bytes\r\n", MemoryStatistics.HeapSize - MemoryStatistics.MaximumHeapUsed));
//      Display(("Framentation:\r\n"));
//      Display(("   Largest Free Fragment: %5d bytes\r\n", MemoryStatistics.LargestFreeFragment));
//      Display(("   Free Fragment Cound:   %5d\r\n",       MemoryStatistics.FreeFragmentCount));
//   }
//   else
//   {
//      Display(("Failed to get memory usage\r\n"));
//   }
//
//   return(ret_val);
//}




   /* The following function is responsible for displaying the current  */
   /* Command Options for either Serial Port Client or Serial Port      */
   /* Server.  The input parameter to this function is completely       */
   /* ignored, and only needs to be passed in because all Commands that */
   /* can be entered at the Prompt pass in the parsed information.  This*/
   /* function displays the current Command Options that are available  */
   /* and always returns zero.                                          */
//static int DisplayHelp(ParameterList_t *TempParam)
//{
//   if(UI_Mode == UI_MODE_IS_CLIENT)
//   {
//      Display(("\r\n"));
//      Display(("******************************************************************\r\n"));
//      Display(("* Command Options: Inquiry, DisplayInquiryList, Pair,            *\r\n"));
//      Display(("*                  EndPairing, PINCodeResponse, PassKeyResponse, *\r\n"));
//      Display(("*                  UserConfirmationResponse,                     *\r\n"));
//      Display(("*                  SetDiscoverabilityMode, SetConnectabilityMode,*\r\n"));
//      Display(("*                  SetPairabilityMode,                           *\r\n"));
//      Display(("*                  ChangeSimplePairingParameters,                *\r\n"));
//      Display(("*                  GetLocalAddress, GetLocalName, SetLocalName,  *\r\n"));
//      Display(("*                  GetClassOfDevice, SetClassOfDevice,           *\r\n"));
//      Display(("*                  GetRemoteName, SniffMode, ExitSniffMode,      *\r\n"));
//      Display(("*                  Open, Close, Read, Write,                     *\r\n"));
//      Display(("*                  GetConfigParams, SetConfigParams,             *\r\n"));
//      Display(("*                  GetQueueParams, SetQueueParams,               *\r\n"));
//      Display(("*                  Loopback, DisplayRawModeData,                 *\r\n"));
//      Display(("*                  AutomaticReadMode, SetBaudRate, Send          *\r\n"));
//      Display(("*                  Help, Quit                                    *\r\n"));
//      Display(("******************************************************************\r\n"));
//   }
//   else
//   {
//      if(UI_Mode == UI_MODE_IS_SERVER)
//      {
//         Display(("\r\n"));
//         Display(("******************************************************************\r\n"));
//         Display(("* Command Options: Inquiry, DisplayInquiryList, Pair,            *\r\n"));
//         Display(("*                  EndPairing, PINCodeResponse, PassKeyResponse, *\r\n"));
//         Display(("*                  UserConfirmationResponse,                     *\r\n"));
//         Display(("*                  SetDiscoverabilityMode, SetConnectabilityMode,*\r\n"));
//         Display(("*                  SetPairabilityMode,                           *\r\n"));
//         Display(("*                  ChangeSimplePairingParameters,                *\r\n"));
//         Display(("*                  GetLocalAddress, GetLocalName, SetLocalName,  *\r\n"));
//         Display(("*                  GetClassOfDevice, SetClassOfDevice,           *\r\n"));
//         Display(("*                  GetRemoteName, SniffMode, ExitSniffMode,      *\r\n"));
//         Display(("*                  Open, Close, Read, Write,                     *\r\n"));
//         Display(("*                  GetConfigParams, SetConfigParams,             *\r\n"));
//         Display(("*                  GetQueueParams, SetQueueParams,               *\r\n"));
//         Display(("*                  Loopback, DisplayRawModeData,                 *\r\n"));
//         Display(("*                  AutomaticReadMode, SetBaudRate, Send          *\r\n"));
//         Display(("*                  Help, Quit                                    *\r\n"));
//         Display(("******************************************************************\r\n"));
//      }
//      else
//      {
//         Display(("\r\n"));
//         Display(("******************************************************************\r\n"));
//         Display(("* Command Options: Server, Client, Help                          *\r\n"));
//         Display(("******************************************************************\r\n"));
//      }
//   }
//
//   return(0);
//}



   /* The following function is a utility function that exists to       */
   /* display the current Inquiry List (with Indexes).  This is useful  */
   /* in case the user has forgotten what Inquiry Index a particular    */
   /* Bluteooth Device was located in.  This function returns zero on   */
   /* successful execution and a negative value on all errors.          */
//static int DisplayInquiryList(ParameterList_t *TempParam)
//{
//   int          ret_val = 0;
//   unsigned int Index;
//
//   /* First, check that valid Bluetooth Stack ID exists.                */
//   if(BluetoothStackID)
//   {
//      /* Simply display all of the items in the Inquiry List.           */
////      Display(("Inquiry List: %d Devices%s\r\n\r\n", NumberofValidResponses, NumberofValidResponses?":":"."));
//
//      for(Index=0;Index<NumberofValidResponses;Index++)
//      {
//         BD_ADDRToStr(InquiryResultList[Index], Function_BoardStr);
//
////         Display((" Inquiry Result: %d, %s.\r\n", (Index+1), Function_BoardStr));
//      }
//
//      if(NumberofValidResponses)
//         Display(("\r\n"));
//
//      /* All finished, flag success to the caller.                      */
//      ret_val = 0;
//   }
//   else
//   {
//      /* No valid Bluetooth Stack ID exists.                            */
//      ret_val = INVALID_STACK_ID_ERROR;
//   }
//
//   return(ret_val);
//}

