/*
 * bluetooth_functions.c
 *
 *  Created on: 22/6/2015
 *      Author: Tomas Funk
 */

#include "bluetooth_internal.h"

#include "fsm_client.h"

#define PROTOCOL_DESCRIPTOR_LIST_ATTR  0x0004

extern osSemaphoreId sem_bt_conectedHandle;
extern osMailQId queue_bt_packet_recievedHandle;
extern osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_fsm_eventsHandle;

/* Holds the Handle of the opened Bluetooth Protocol Stack                      */
unsigned int               BluetoothStackID;
/* Holds the UI Mode.                                                           */
int                        UI_Mode;
/* Contains the Handle of the most recent SPP Port.                             */
static int                 SerialPortID;
/* Contains the Handle of the SPP Server Port.                                  */
static int                 ServerPortID;
/* Holds the Connection Handle of the most recent SPP Connection.               */
static Word_t              Connection_Handle;
/* Flags whether or not there is currently an active connection                 */
static Boolean_t           Connected;
/* Hold the Serial Port Service Record of SDP Service Record.                   */
static DWord_t             SPPServerSDPHandle;
/* Contains the inquiry result received from the last performed inquiry.        */
static device_info_t       InquiryResultList[MAX_INQUIRY_RESULTS];
/* Holds the number of valid inquiry results within the inquiry results array.  */
static unsigned int        NumberofValidResponses;
/* Holds BD_ADDR <-> Link Keys for pairing.                                     */
static LinkKeyInfo_t       LinkKeyInfo[MAX_SUPPORTED_LINK_KEYS];
/* Holds the current BD_ADDR which is currently pairing or authenticating.      */
static BD_ADDR_t           CurrentRemoteBD_ADDR;
/* Holds the current BD_ADDR of the device which is paired/connected.           */
static BD_ADDR_t           SelectedBD_ADDR;
/* Holds the current I/O Capabilities that are to be used for SSP.              */
static GAP_IO_Capability_t IOCapability;
/* Flags whether or not Out of Band Secure Simple Pairing exchange is supported.*/
static Boolean_t           OOBSupport;
/* Flags whether or not Man in the Middle (MITM) protection is to be requested  */
/* during a Secure Simple Pairing procedure.                                    */
static Boolean_t           MITMProtection;
/* Holds a NULL BD_ADDR for compataration purposes                              */
static BD_ADDR_t           NullADDR;

   /* The following function is responsible for opening the SS1         */
   /* Bluetooth Protocol Stack.  This function accepts a pre-populated  */
   /* HCI Driver Information structure that contains the HCI Driver     */
   /* Transport Information.  This function returns zero on successful  */
   /* execution and a negative value on all errors.                     */

int OpenStack(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization)
{
   int                        Result;
   int                        ret_val = 0;
   Byte_t                     Status;
   BD_ADDR_t                  BD_ADDR;
   L2CA_Link_Connect_Params_t L2CA_Link_Connect_Params;

  /* Initialize BTPSKNRl.                                        */
  BTPS_Init(BTPS_Initialization);

  /* Initialize the Stack                                        */
  Result = BSC_Initialize(HCI_DriverInformation, 0);

  /* Next, check the return value of the initialization to see if*/
  /* it was successful.                                          */
  if(Result > 0)
  {
    HAL_GPIO_WritePin(GPIOC, UI_LED_B_Pin,GPIO_PIN_SET);
    osDelay(500);
    HAL_GPIO_WritePin(GPIOC, UI_LED_B_Pin,GPIO_PIN_RESET);
    /* The Stack was initialized successfully, inform the user  */
    /* and set the return value of the initialization function  */
    /* to the Bluetooth Stack ID.                               */
    BluetoothStackID = Result;

    /* Initialize the default Secure Simple Pairing parameters. */
    IOCapability     = DEFAULT_IO_CAPABILITY;
    OOBSupport       = FALSE;
    MITMProtection   = DEFAULT_MITM_PROTECTION;

    /* Set the Name for the initial use.              */
    GAP_Set_Local_Device_Name (BluetoothStackID,APP_DEMO_NAME);

    /* Go ahead and allow Master/Slave Role Switch.             */
    L2CA_Link_Connect_Params.L2CA_Link_Connect_Request_Config  = cqAllowRoleSwitch;
    L2CA_Link_Connect_Params.L2CA_Link_Connect_Response_Config = csMaintainCurrentRole;

    L2CA_Set_Link_Connection_Configuration(BluetoothStackID, &L2CA_Link_Connect_Params);

    if(HCI_Command_Supported(BluetoothStackID, HCI_SUPPORTED_COMMAND_WRITE_DEFAULT_LINK_POLICY_BIT_NUMBER) > 0)
    {
       HCI_Write_Default_Link_Policy_Settings(BluetoothStackID,
                                              (HCI_LINK_POLICY_SETTINGS_ENABLE_MASTER_SLAVE_SWITCH|
                                               HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE), &Status);
    }
    /* Delete all Stored Link Keys.                             */
    ASSIGN_BD_ADDR(BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    DeleteLinkKey(BD_ADDR);
  }
  else
  {
    /* The Stack was NOT initialized successfully, inform the   */
    /* user and set the return value of the initialization      */
    /* function to an error.                                    */
//    HAL_GPIO_WritePin(GPIOC, UI_LED_R_Pin,GPIO_PIN_SET);
    BluetoothStackID = 0;
    ret_val          = UNABLE_TO_INITIALIZE_STACK;
  }

  return(ret_val);
}

 /* The following function is used to initialize the application      */
 /* instance.  This function should open the stack and prepare to     */
 /* execute commands based on user input.  The first parameter passed */
 /* to this function is the HCI Driver Information that will be used  */
 /* when opening the stack and the second parameter is used to pass   */
 /* parameters to BTPS_Init.  This function returns the               */
 /* BluetoothStackID returned from BSC_Initialize on success or a     */
 /* negative error code (of the form APPLICATION_ERROR_XXX).          */
int InitializeApplication(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization)
{
   int ret_val = APPLICATION_ERROR_UNABLE_TO_OPEN_STACK;

   /* Initiailize some defaults.                                        */
   SerialPortID           = 0;
   UI_Mode                = UI_MODE_SELECT;
   NumberofValidResponses = 0;
   // Pa que se calle el compilador, hay que quitarlo en cuanto se pueda
   UNUSED(NumberofValidResponses);
   /* Next, makes sure that the Driver Information passed appears to be */
   /* semi-valid.                                                       */
   if((HCI_DriverInformation) && (BTPS_Initialization))
   {
      /* Try to Open the stack and check if it was successful.          */
      if(!OpenStack(HCI_DriverInformation, BTPS_Initialization))
      {
         /* The stack was opened successfully.  Now set some defaults.  */

         /* First, attempt to set the Device to be Connectable.         */
         SetConnectabilityMode(cmConnectableMode);
         SetDiscoverabilityMode(dmGeneralDiscoverableMode);
         SetPairabilityMode(pmPairableMode_EnableSecureSimplePairing);
         HCI_Register_Event_Callback(BluetoothStackID, HCI_Event_Callback, (unsigned long)NULL);
         ASSIGN_BD_ADDR(NullADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
         OpenServer();

         ret_val = (int)BluetoothStackID;
      }
      else
      {
        CloseStack();
      }
   }
   return(ret_val);
}
/* Dummy function for complete the form the stack, yet to be implemented */
static int BTPSInitCallback(int Length, char *Message)
{
    return TRUE;
}

   /* The following function handles the sleep indication callbacks from*/
   /* the HCI transport.                                                */
static void Sleep_Indication_Callback(Boolean_t SleepAllowed, unsigned long CallbackParameter)
{
   /* Verify parameters.                                                */
   if(CallbackParameter)
   {
      if(SleepAllowed)
      {
         /* Attempt to suspend the transport.                           */
         HCITR_COMSuspend(TRANSPORT_ID);
      }
   }
}

/* Initialize all BT stack related stuff */
void bluetooth_init(void)
{
   int                           Result;
   BTPS_Initialization_t         BTPS_Initialization;
   HCI_DriverInformation_t       HCI_DriverInformation;
   HCI_HCILLConfiguration_t      HCILLConfig;
   HCI_Driver_Reconfigure_Data_t DriverReconfigureData;
    /* First check to see if the Stack has already been opened.          */
   if( !BluetoothStackID)
   {
     /* Configure the UART Parameters.                                    */
     HCI_DRIVER_SET_COMM_INFORMATION(&HCI_DriverInformation, 1, VENDOR_BAUD_RATE, cpUART_RTS_CTS);
     HCI_DriverInformation.DriverInformation.COMMDriverInformation.InitializationDelay = 100;

     /* Set up the application callbacks.                                 */
     BTPS_Initialization.MessageOutputCallback = BTPSInitCallback;

     /* Initialize the application.                                       */
     if((Result = InitializeApplication(&HCI_DriverInformation, &BTPS_Initialization)) > 0)
     {
        /* Register a sleep mode callback if we are using HCILL Mode.     */
        if((HCI_DriverInformation.DriverInformation.COMMDriverInformation.Protocol == cpHCILL)
           || (HCI_DriverInformation.DriverInformation.COMMDriverInformation.Protocol == cpHCILL_RTS_CTS))
        {
           HCILLConfig.SleepCallbackFunction        = Sleep_Indication_Callback;
           HCILLConfig.SleepCallbackParameter       = 0;
           DriverReconfigureData.ReconfigureCommand = HCI_COMM_DRIVER_RECONFIGURE_DATA_COMMAND_CHANGE_HCILL_PARAMETERS;
           DriverReconfigureData.ReconfigureData    = (void *)&HCILLConfig;

           /* Register the sleep mode callback.  Note that if this        */
           /* function returns greater than 0 then sleep is currently     */
           /* enabled.                                                    */
           Result = HCI_Reconfigure_Driver((unsigned int)Result, FALSE, &DriverReconfigureData);
           if(Result > 0)
           {
              /* Flag that sleep mode is enabled.                         */
             Result = 0;
           }
        }
     }
   }
}

   /* The following function is responsible for closing the SS1         */
   /* Bluetooth Protocol Stack.  This function requires that the        */
   /* Bluetooth Protocol stack previously have been initialized via the */
   /* OpenStack() function.  This function returns zero on successful   */
   /* execution and a negative value on all errors.                     */
int CloseStack(void)
{
   int ret_val = UNABLE_TO_INITIALIZE_STACK;

   /* First check to see if the Stack has been opened.                  */
   if(BluetoothStackID)
   {
      /* Simply close the Stack                                         */
      BSC_Shutdown(BluetoothStackID);

      /* Flag that the Stack is no longer initialized.                  */
      BluetoothStackID = 0;

      /* Flag success to the caller.                                    */
      ret_val          = 0;
      HAL_GPIO_WritePin(GPIOC, UI_LED_B_Pin,GPIO_PIN_RESET);
   }

   return(ret_val);
}


   /* The following function is responsible for opening a Serial Port   */
   /* Server on the Local Device.  This function opens the Serial Port  */
   /* Server on the specified RFCOMM Channel.  This function returns    */
   /* zero if successful, or a negative return value if an error        */
   /* occurred.                                                         */
int OpenServer(void)
{
  int  ret_val;
  char *ServiceName;

  /* First check to see if a valid Bluetooth Stack ID exists and there is no */
  /* other port opened.          */
  if((BluetoothStackID) || (ServerPortID > 0 ))
  {
    /* Simply attempt to open an Serial Server, on RFCOMM Server Port 1. */
    ret_val = SPP_Open_Server_Port(BluetoothStackID, 1, SPP_Event_Callback, (unsigned long)0);
    /* If the Open was successful, then note the Serial Port Server ID.  */
    if(ret_val > 0)
    {
      /* Note the Serial Port Server ID of the opened Serial Port Server.*/
      ServerPortID = ret_val;
      /* Create a Buffer to hold the Service Name.             */
      if((ServiceName = BTPS_AllocateMemory(64)) != NULL)
      {
        /* The Server was opened successfully, now register a */
        /* SDP Record indicating that an Serial Port Server   */
        /* exists. Do this by first creating a Service Name.  */
        BTPS_SprintF(ServiceName, "Serial Port Server Port %d", (int)(ServerPortID));

        /* Now that a Service Name has been created try to    */
        /* Register the SDP Record.                           */
        ret_val = SPP_Register_Generic_SDP_Record(BluetoothStackID, ServerPortID, ServiceName, &SPPServerSDPHandle);

        /* If there was an error creating the Serial Port     */
        /* Server's SDP Service Record then go ahead an close */
        /* down the server an flag an error.                  */
        if(ret_val < 0)
        {
          SPP_Close_Server_Port(BluetoothStackID, ServerPortID);
          /* Flag that there is no longer an Serial Port     */
          /* Server Open.                                    */
          ServerPortID = 0;
          /* Flag that we are no longer connected.           */
          Connected    = FALSE;
          ret_val      = UNABLE_TO_REGISTER_SERVER;
        }
        /* Free the Service Name buffer.                      */
        BTPS_FreeMemory(ServiceName);
      }
    }else{
      ret_val = UNABLE_TO_REGISTER_SERVER;
    }
  }
  else
  {
    /* A Server is already open, or stack isn't    */
    ret_val = FUNCTION_ERROR;
  }
  return(ret_val);
}


   /* The following function is responsible for closing a Serial Port   */
   /* Server that was previously opened via a successful call to the    */
   /* OpenServer() function.  This function returns zero if successful  */
   /* or a negative return error code if there was an error.            */
int CloseServer(void)
{
   int ret_val = FUNCTION_ERROR;

  /* First check to see if a valid Bluetooth Stack ID exists and there is an */
  /* open port .          */
  if((BluetoothStackID) || (ServerPortID == 0 ))
  {
    /* If there is an SDP Service Record associated with the Serial*/
    /* Port Server then we need to remove it from the SDP Database.*/
    if(SPPServerSDPHandle)
    {
      SPP_Un_Register_SDP_Record(BluetoothStackID, SerialPortID, SPPServerSDPHandle);
      /* Flag that there is no longer an SDP Serial Port Server   */
      /* Record.                                                  */
      SPPServerSDPHandle = 0;
    }

    /* Finally close the Serial Port Server.                       */
    ret_val = SPP_Close_Server_Port(BluetoothStackID, ServerPortID);

    if(ret_val < 0)
    {
      ret_val = FUNCTION_ERROR;
    }
    else
    {
      /* Flag that there is no Serial Port Server currently open.    */
      ServerPortID = 0;
      SerialPortID = 0;

      /* Flag that we are no longer connected.                       */
      Connected    = FALSE;
    }

  }

   return(ret_val);
}

   /* The following function is responsible for initiating a connection */
   /* with a Remote Serial Port Server.  This function returns zero if  */
   /* successful and a negative value if an error occurred.             */
int OpenRemoteServer(BD_ADDR_t remote_address, uint16_t server_port)
{
  int ret_val = INVALID_STACK_ID_ERROR;
  int Result;

  /* First, check that valid Bluetooth Stack ID exists.             */
  if(BluetoothStackID)
  {
    /* Next, check that we are in client mode and there is not an  */
    /* Serial Port Client already open.                            */
    if(!SerialPortID)
    {
      /* Now let's attempt to open the Remote Serial Port      */
      /* Server.                                               */
      Result = SPP_Open_Remote_Port(BluetoothStackID, remote_address,
                        server_port, SPP_Event_Callback, (unsigned long)0);

      if(Result > 0)
      {
        /* Note the Serial Port Client ID.                    */
        SerialPortID = Result;
        /* Flag success to the caller.                        */
        ret_val = 0;
        /* Save the BD_ADDR so we can Query the Connection    */
        /* handle when receive Connection Confirmation Event. */
        SelectedBD_ADDR = remote_address;
      }
      else
      {
        /* remote port couldn't be oppened    */
        ret_val = FUNCTION_ERROR;
      }
    }
    else
    {
      ret_val = INVALID_PARAMETERS_ERROR;
    }
  }

  return(ret_val);
}

   /* The following function is responsible for terminating a connection*/
   /* with a Remote Serial Port Server.  This function returns zero if  */
   /* successful and a negative value if an error occurred.             */
int CloseRemoteServer(void)
{
  int ret_val = INVALID_STACK_ID_ERROR;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /* Next, check that we are in client mode and there is an         */
    /* Serial Port Client already open.                               */
    if((UI_Mode == UI_MODE_IS_CLIENT) && (SerialPortID))
    {
      /* Simply close the Serial Port Client.                     */
      SPP_Close_Port(BluetoothStackID, SerialPortID);

      /* Flag that there is no longer a Serial Port Client        */
      /* connected.                                               */
      SerialPortID = 0;

      /* Flag that we are no longer connected.                    */
      Connected    = FALSE;

      /* Flag success to the caller.                              */
      ret_val      = 0;
    }
    else
    {
      ret_val = INVALID_PARAMETERS_ERROR;
    }
  }

  return(ret_val);
}

   /* The following thread is responsible for checking changing the     */
   /* current Baud Rate used to talk to the Radio.                      */
   /* * NOTE * This function ONLY configures the Baud Rate for a TI     */
   /*          Bluetooth chipset.                                       */
int SetBaudRate(DWord_t baud_rate)
{
  int ret_val = INVALID_STACK_ID_ERROR;

  /* First check to see if the parameters required for the execution of*/
  /* this function appear to be semi-valid.                            */
  if(BluetoothStackID)
  {
    /* Next, write the command to the device.                      */
    ret_val = VS_Update_UART_Baud_Rate(BluetoothStackID, baud_rate);
  }

  return ret_val;
}


   /* The following function is responsible for setting the name of the */
   /* local Bluetooth Device to a specified name.  This function returns*/
   /* zero on successful execution and a negative value on all errors.  */
int SetLocalName(char *device_name)
{
   int ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
     /* Attempt to submit the command.                              */
     ret_val = GAP_Set_Local_Device_Name(BluetoothStackID, device_name);

     /* Check the return value of the submitted command for success.*/
     ret_val = (ret_val)?FUNCTION_ERROR:0;
   }
   return(ret_val);
}

   /* The following function is responsible for setting the Class of    */
   /* Device of the local Bluetooth Device to a Class Of Device value.  */
   /* This function returns zero on successful execution and a negative */
   /* value on all errors.                                              */
int SetClassOfDevice(int class_device)
{
  int               ret_val = INVALID_STACK_ID_ERROR;
  Class_of_Device_t Class_of_Device;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /* Attempt to submit the command.                              */
    ASSIGN_CLASS_OF_DEVICE(Class_of_Device, (Byte_t)((class_device) & 0xFF),
                                      (Byte_t)(((class_device) >> 8) & 0xFF),
                                      (Byte_t)(((class_device) >> 16) & 0xFF));

    ret_val = GAP_Set_Class_Of_Device(BluetoothStackID, Class_of_Device);

    /* Check the return value of the submitted command for success.*/
    ret_val = (ret_val)?FUNCTION_ERROR:0;
  }

  return(ret_val);
}

   /* The following function is responsible for setting the Pairability */
   /* Mode of the local device.  This function returns zero on          */
   /* successful execution and a negative value on all errors.          */
static int SetPairabilityMode(GAP_Pairability_Mode_t  PairabilityMode)
{
   int    Result;
   int    ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
         Result = GAP_Set_Pairability_Mode(BluetoothStackID, PairabilityMode);

         /* Next, check the return value to see if the command was      */
         /* issued successfully.                                        */
         if(!Result)
         {
            /* The Mode was changed successfully.                       */
            Result = GAP_Register_Remote_Authentication(BluetoothStackID, GAP_Event_Callback, (unsigned long)0);

            ret_val = (Result)?Result:0;
         }
   }

   return(ret_val);
}

   /* The following function is responsible for setting the             */
   /* Connectability Mode of the local device.  This function returns   */
   /* zero on successful execution and a negative value on all errors.  */
static int SetConnectabilityMode(GAP_Connectability_Mode_t ConnectableMode)
{
   int                       Result;
   int                       ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      Result = GAP_Set_Connectability_Mode(BluetoothStackID, ConnectableMode);
      /* After setting, notify the caller */
      ret_val = (Result >= 0)? 0:FUNCTION_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the             */
   /* Discoverability Mode of the local device.  This function returns  */
   /* zero on successful execution and a negative value on all errors.  */
int SetDiscoverabilityMode(GAP_Discoverability_Mode_t DiscoverabilityMode)
{
   int Result;
   int ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
         Result = GAP_Set_Discoverability_Mode(BluetoothStackID, DiscoverabilityMode,0);
         ret_val = (Result >= 0)?0:FUNCTION_ERROR;
   }
   return ret_val;
}

   /* The following function is responsible for changing the Secure     */
   /* Simple Pairing Parameters that are exchanged during the Pairing   */
   /* procedure when Secure Simple Pairing (Security Level 4) is used.  */
   /* This function returns zero on successful execution and a negative */
   /* value on all errors.                                              */
int SetSimplePairingParameters(GAP_IO_Capability_t iocap, Boolean_t mim_proct)
{
   int ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      IOCapability = iocap;
      /* Finally map the Man in the Middle (MITM) Protection valid.  */
      MITMProtection = mim_proct;
      /* Flag success to the caller.                                 */
      ret_val = 0;
   }
   return(ret_val);
}

   /* The following function is responsible for setting the current     */
   /* queuing parameters that are used by SPP/RFCOMM (into L2CAP).  This*/
   /* function will return zero on successful execution and a negative  */
   /* value on errors.                                                  */
int SetQueueParams(unsigned int maximum_number_data_packets,unsigned int queue_threshold)
{
  int ret_val = INVALID_STACK_ID_ERROR;

  /* First check to see if the parameters required for the execution of*/
  /* this function appear to be semi-valid.                            */
  if(BluetoothStackID)
  {
    /* Parameters have been specified, go ahead and write them to  */
    /* the stack.                                                  */
    ret_val = SPP_Set_Queuing_Parameters(BluetoothStackID,
                                          maximum_number_data_packets,
                                          queue_threshold);

    ret_val = (ret_val >= 0)?0:FUNCTION_ERROR;
  }

   return(ret_val);
}

   /* The following function is responsible for setting the current     */
   /* configuration parameters that are used by SPP.  This function will*/
   /* return zero on successful execution and a negative value on       */
   /* errors.                                                           */
int SetConfigParams(uint16_t MaximumFrameSize, uint16_t TransmitBufferSize, uint16_t ReceiveBufferSize)
{
  int ret_val = INVALID_STACK_ID_ERROR;
  SPP_Configuration_Params_t SPPConfigurationParams;

  /* First check to see if the parameters required for the execution of*/
  /* this function appear to be semi-valid.                            */
  if(BluetoothStackID)
  {
    /* Next check to see if the parameters required for the execution */
    /* of this function appear to be semi-valid.                      */
    /* Parameters have been specified, go ahead and write them to  */
    /* the stack.                                                  */
    SPPConfigurationParams.MaximumFrameSize   = MaximumFrameSize;
    SPPConfigurationParams.TransmitBufferSize = TransmitBufferSize;
    SPPConfigurationParams.ReceiveBufferSize  = ReceiveBufferSize;

    ret_val = SPP_Set_Configuration_Parameters(BluetoothStackID, &SPPConfigurationParams);

    ret_val = (ret_val < 0)?FUNCTION_ERROR:ret_val;
  }

  return(ret_val);
}


   /* The following function is responsible for querying the current    */
   /* queuing parameters that are used by SPP/RFCOMM (into L2CAP).  This*/
   /* function will return zero on successful execution and a negative  */
   /* value on errors.                                                  */
int GetQueueParams(unsigned int *params)
{
  int ret_val = INVALID_STACK_ID_ERROR;
  /* First check to see if the parameters required for the execution of*/
  /* this function appear to be semi-valid.                            */
  if(BluetoothStackID)
  {
    /* Simply query the queuing parameters.                           */
    ret_val = SPP_Get_Queuing_Parameters(BluetoothStackID, &params[0], &params[1]);

    ret_val = (ret_val >= 0)?0:FUNCTION_ERROR;
  }

  return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Device Name of the specified remote Bluetooth Device.  This       */
   /* function returns zero on successful execution and a negative value*/
   /* on all errors.                                                    */
int GetRemoteName(BD_ADDR_t remote_address)
{
  int ret_val = INVALID_STACK_ID_ERROR;;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /* Attempt to submit the command.                              */
    ret_val = GAP_Query_Remote_Device_Name(BluetoothStackID, remote_address, GAP_Event_Callback, (unsigned long)0);

    /* Check the return value of the submitted command for success.*/
    ret_val = (ret_val)?FUNCTION_ERROR:0;
  }

  return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Device Address of the local Bluetooth Device.  This function      */
   /* returns zero on successful execution and a negative value on all  */
   /* errors.                                                           */
int GetLocalAddress(BD_ADDR_t local_address)
{
   int       ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Attempt to submit the command.                                 */
      ret_val = GAP_Query_Local_BD_ADDR(BluetoothStackID, &local_address);

      /* Check the return value of the submitted command for success.   */
      ret_val = (ret_val)?FUNCTION_ERROR:0;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Device Name of the local Bluetooth Device.  This function returns */
   /* zero on successful execution and a negative value on all errors.  */
int GetLocalName(char *LocalName, uint16_t lenght)
{
  int   ret_val = INVALID_STACK_ID_ERROR;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /* Attempt to submit the command.                              */
    ret_val = GAP_Query_Local_Device_Name(BluetoothStackID, lenght, LocalName);

    /* Check the return value of the submitted command for success.*/
    ret_val = (ret_val)?FUNCTION_ERROR:0;
  }

  return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Class of Device of the local Bluetooth Device.  This function     */
   /* returns zero on successful execution and a negative value on all  */
   /* errors.                                                           */
int GetClassOfDevice(Class_of_Device_t Class_of_Device)
{
   int    ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Attempt to submit the command.                                 */
      ret_val = GAP_Query_Class_Of_Device(BluetoothStackID, &Class_of_Device);

      /* Check the return value of the submitted command for success.   */
      ret_val = (ret_val)?FUNCTION_ERROR:0;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the current    */
   /* configuration parameters that are used by SPP.  This function will*/
   /* return zero on successful execution and a negative value on       */
   /* errors.                                                           */
int GetConfigParams(SPP_Configuration_Params_t *SPPConfigurationParams)
{
  int ret_val = INVALID_STACK_ID_ERROR;

   /* First check to see if the parameters required for the execution of*/
   /* this function appear to be semi-valid.                            */
   if(BluetoothStackID)
   {
      /* Simply query the configuration parameters.                     */
      ret_val = SPP_Get_Configuration_Parameters(BluetoothStackID, SPPConfigurationParams);

      ret_val = (ret_val < 0)?FUNCTION_ERROR:ret_val;
   }

   return(ret_val);
}

   /* The following function is responsible for performing a General    */
   /* Inquiry for discovering Bluetooth Devices.  This function requires*/
   /* that a valid Bluetooth Stack ID exists before running.  This      */
   /* function returns zero is successful or a negative value if there  */
   /* was an error.                                                     */
int Inquiry(uint32_t timeout)
{
   int ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Use the GAP_Perform_ function to perform an Inquiry.  */
      /* The Inquiry will last 10 seconds or until MAX_INQUIRY_RESULTS  */
      /* Bluetooth Devices are found.  When the Inquiry Results become  */
      /* available the GAP_Event_Callback is called.                    */
      ret_val = GAP_Perform_Inquiry(BluetoothStackID, itGeneralInquiry, 0, 0, timeout,
                                   MAX_INQUIRY_RESULTS, GAP_Event_Callback, (unsigned long)NULL);
      /* Next, check to see if the GAP_Perform_Inquiry() function was   */
      /* successful.                                                    */
      if(!ret_val)
      {
         /* The Inquiry appears to have been sent successfully.         */
         NumberofValidResponses = 0;
      }
   }

   return(ret_val);
}

uint32_t bt_get_remote_devices(device_info_t* inquired_addr)
{
  uint32_t i;

  for( i = 0; i < NumberofValidResponses; i++ )
  {
    inquired_addr[i] = InquiryResultList[i];
  }
  return NumberofValidResponses;
}
   /* The following function is responsible for initiating bonding with */
   /* a remote device.  This function returns zero on successful        */
   /* execution and a negative value on all errors.                     */

int Pair(GAP_Bonding_Type_t bondtype, uint16_t table_pos)
{
  int                ret_val = INVALID_STACK_ID_ERROR;
  GAP_Bonding_Type_t BondingType;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    ret_val = FUNCTION_ERROR;
    /* Next, make sure that we are not already connected.             */
    if((!Connected) && (!COMPARE_BD_ADDR(InquiryResultList[table_pos].physical_address, NullADDR)))
    {
      BondingType = bondtype;
      CurrentRemoteBD_ADDR = InquiryResultList[table_pos].physical_address;

      /* Before we submit the command to the stack, we need to    */
      /* make sure that we clear out any Link Key we have stored  */
      /* for the specified device.                                */
      DeleteLinkKey(CurrentRemoteBD_ADDR);

      /* Attempt to submit the command.                           */
      ret_val = GAP_Initiate_Bonding(BluetoothStackID, CurrentRemoteBD_ADDR,
                              BondingType, GAP_Event_Callback, (unsigned long)0);
      if(!ret_val)
      {
        SelectedBD_ADDR = CurrentRemoteBD_ADDR;
        ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
      }
    }

  }
  return(ret_val);
}

   /* The following function is responsible for ending a previously     */
   /* initiated bonding session with a remote device.  This function    */
   /* returns zero on successful execution and a negative value on all  */
   /* errors.                                                           */
int EndPairing(void)
{
   int ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
     ret_val = FUNCTION_ERROR;
     if(Connected)
     {
         /* Attempt to submit the command.                              */
         ret_val = GAP_End_Bonding(BluetoothStackID, SelectedBD_ADDR);

         /* Check the return value of the submitted command for success.*/
         if(!ret_val)
         {
            /* Cleans PairedRemoteBD_ADDR */
            ASSIGN_BD_ADDR(SelectedBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
         }
      }
   }
   return(ret_val);
}

   /* The following function is responsible for issuing a GAP           */
   /* Authentication Response with a PIN Code value specified via the   */
   /* input parameter.  This function returns zero on successful        */
   /* execution and a negative value on all errors.                     */
int PINCodeResponse(char *pin)
{
  int                              ret_val = INVALID_STACK_ID_ERROR;
  PIN_Code_t                       PINCode;
  GAP_Authentication_Information_t GAP_Authentication_Information;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    ret_val = FUNCTION_ERROR;
    /* First, check to see if there is an on-going Pairing operation  */
    /* active.                                                        */
    if(!COMPARE_BD_ADDR(CurrentRemoteBD_ADDR, NullADDR))
    {
      /* Parameters appear to be valid, go ahead and convert the  */
      /* input parameter into a PIN Code.                         */

      /* Initialize the PIN code.                                 */
      ASSIGN_PIN_CODE(PINCode, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

      BTPS_MemCopy(&PINCode, pin, BTPS_StringLength(pin));

      /* Populate the response structure.                         */
      GAP_Authentication_Information.GAP_Authentication_Type      = atPINCode;
      GAP_Authentication_Information.Authentication_Data_Length   = (Byte_t)(BTPS_StringLength(pin));
      GAP_Authentication_Information.Authentication_Data.PIN_Code = PINCode;

      /* Submit the Authentication Response.                      */
      ret_val = GAP_Authentication_Response(BluetoothStackID, CurrentRemoteBD_ADDR, &GAP_Authentication_Information);
      if(!ret_val){
        /* Flag that there is no longer a current Authentication    */
        /* procedure in progress.                                   */
        ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
      }
    }
  }
  return(ret_val);
}


   /* The following function is responsible for issuing a GAP           */
   /* Authentication Response with a Pass Key value specified via the   */
   /* input parameter.  This function returns zero on successful        */
   /* execution and a negative value on all errors.                     */
int PassKeyResponse(char *Passkey)
{
  int                              ret_val = INVALID_STACK_ID_ERROR;
  GAP_Authentication_Information_t GAP_Authentication_Information;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    ret_val = FUNCTION_ERROR;
    /* First, check to see if there is an on-going Pairing operation  */
    /* active.                                                        */
    if(!COMPARE_BD_ADDR(CurrentRemoteBD_ADDR, NullADDR) && ((BTPS_StringLength(Passkey) <= GAP_PASSKEY_MAXIMUM_NUMBER_OF_DIGITS)))
    {
      /* Parameters appear to be valid, go ahead and populate the */
      /* response structure.                                      */
      GAP_Authentication_Information.GAP_Authentication_Type     = atPassKey;
      GAP_Authentication_Information.Authentication_Data_Length  = (Byte_t)(sizeof(DWord_t));
      GAP_Authentication_Information.Authentication_Data.Passkey = (DWord_t)(Passkey);

      /* Submit the Authentication Response.                      */
      ret_val = GAP_Authentication_Response(BluetoothStackID, CurrentRemoteBD_ADDR, &GAP_Authentication_Information);

      /* Flag that there is no longer a current Authentication    */
      /* procedure in progress.                                   */
      ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    }
  }
  return(ret_val);
}

  /* The following function is responsible for issuing a GAP           */
  /* Authentication Response with a User Confirmation value specified  */
  /* via the input parameter.  This function returns zero on successful*/
  /* execution and a negative value on all errors.                     */
int UserConfirmationResponse(Boolean_t confirmation)
{
  int                              ret_val = INVALID_STACK_ID_ERROR;
  GAP_Authentication_Information_t GAP_Authentication_Information;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    ret_val = FUNCTION_ERROR;
    /* First, check to see if there is an on-going Pairing operation  */
    /* active.                                                        */
    if(!COMPARE_BD_ADDR(CurrentRemoteBD_ADDR, NullADDR))
    {
      /* Parameters appear to be valid, go ahead and populate the */
      /* response structure.                                      */
      GAP_Authentication_Information.GAP_Authentication_Type          = atUserConfirmation;
      GAP_Authentication_Information.Authentication_Data_Length       = (Byte_t)(sizeof(Byte_t));
      GAP_Authentication_Information.Authentication_Data.Confirmation = confirmation;

      /* Submit the Authentication Response.                      */
      ret_val = GAP_Authentication_Response(BluetoothStackID, CurrentRemoteBD_ADDR, &GAP_Authentication_Information);

      /* Flag that there is no longer a current Authentication    */
      /* procedure in progress.                                   */
      ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    }
  }
  return(ret_val);
}


   /* The following function is responsible for putting a specified     */
   /* connection into HCI Sniff Mode with passed in parameters.         */
int SniffMode( Word_t Sniff_Max_Interval, Word_t Sniff_Min_Interval, Word_t Sniff_Attempt, Word_t Sniff_Timeout)
{
  Byte_t Status;
  int       Result;
  int       ret_val = INVALID_STACK_ID_ERROR;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /* Make sure the connection handle is valid.                */
    if(Connection_Handle)
    {
      /* Now that we have the connection try and go to Sniff.  */
      Result = HCI_Sniff_Mode(BluetoothStackID, Connection_Handle, Sniff_Max_Interval, Sniff_Min_Interval, Sniff_Attempt, Sniff_Timeout, &Status);

      ret_val = ((!Result) && (!Status))?FUNCTION_ERROR:0;
    }
  }
  return(ret_val);
}

   /* The following function is responsible for attempting to Exit      */
   /* Sniff Mode for a specified connection.                            */
int ExitSniffMode(void)
{
  Byte_t    Status;
  int    Result;
  int    ret_val = INVALID_STACK_ID_ERROR;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /*Checks for connection handle */
    if(Connection_Handle)
    {
      /* Attempt to Exit Sniff Mode for the Specified Device.        */
      Result = HCI_Exit_Sniff_Mode(BluetoothStackID, Connection_Handle, &Status);
      ret_val = ((!Result) && (!Status))?FUNCTION_ERROR:0;
    }
  }

  return(ret_val);
}

   /* The following function is responsible for sending a number of     */
   /* characters to a remote device to which a connection exists.  The  */
   /* function receives a parameter that indicates the number of byte to*/
   /* be transferred.  This function will return zero on successful     */
   /* execution and a negative value on errors.                         */
int SendData(uint16_t length, char *buff)
{
  int ret_val = -1; /* -1 means the transmision couldn't be done */

      /* Verify that there is a connection that is established.         */

  if(SerialPortID)
  {
    ret_val = SPP_Data_Write(BluetoothStackID, SerialPortID, length, (Byte_t *)buff);
  }

  return ret_val;
}

   /* The following function is a utility function that exists to delete*/
   /* the specified Link Key from the Local Bluetooth Device.  If a NULL*/
   /* Bluetooth Device Address is specified, then all Link Keys will be */
   /* deleted.                                                          */
int DeleteLinkKey(BD_ADDR_t BD_ADDR)
{
   int       Result;
   Byte_t    Status_Result;
   Word_t    Num_Keys_Deleted = 0;
   BD_ADDR_t NULL_BD_ADDR;

   Result = HCI_Delete_Stored_Link_Key(BluetoothStackID, BD_ADDR, TRUE, &Status_Result, &Num_Keys_Deleted);

   /* Any stored link keys for the specified address (or all) have been */
   /* deleted from the chip.  Now, let's make sure that our stored Link */
   /* Key Array is in sync with these changes.                          */

   /* First check to see all Link Keys were deleted.                    */
   ASSIGN_BD_ADDR(NULL_BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

   if(COMPARE_BD_ADDR(BD_ADDR, NULL_BD_ADDR))
   {
      BTPS_MemInitialize(LinkKeyInfo, 0, sizeof(LinkKeyInfo));
   }
   else
   {
      /* Individual Link Key.  Go ahead and see if know about the entry */
      /* in the list.                                                   */
      for(Result=0;(Result<sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Result++)
      {
         if(COMPARE_BD_ADDR(BD_ADDR, LinkKeyInfo[Result].BD_ADDR))
         {
            LinkKeyInfo[Result].BD_ADDR = NULL_BD_ADDR;
            break;
         }
      }
   }

   return(Result);
}

   /* The following function is responsible for converting data of type */
   /* BD_ADDR to a string.  The first parameter of this function is the */
   /* BD_ADDR to be converted to a string.  The second parameter of this*/
   /* function is a pointer to the string in which the converted BD_ADDR*/
   /* is to be stored.                                                  */
void BD_ADDRToStr(BD_ADDR_t Board_Address, BoardStr_t BoardStr)
{
   BTPS_SprintF((char *)BoardStr, "0x%02X%02X%02X%02X%02X%02X", Board_Address.BD_ADDR5,
                Board_Address.BD_ADDR4, Board_Address.BD_ADDR3, Board_Address.BD_ADDR2,
                Board_Address.BD_ADDR1, Board_Address.BD_ADDR0);
}

   /*********************************************************************/
   /*                         Event Callbacks                           */
   /*********************************************************************/

   /* The following function is for the GAP Event Receive Data Callback.*/
   /* This function will be called whenever a Callback has been         */
   /* registered for the specified GAP Action that is associated with   */
   /* the Bluetooth Stack.  This function passes to the caller the GAP  */
   /* Event Data of the specified Event and the GAP Event Callback      */
   /* Parameter that was specified when this Callback was installed.    */
   /* The caller is free to use the contents of the GAP Event Data ONLY */
   /* in the context of this callback.  If the caller requires the Data */
   /* for a longer period of time, then the callback function MUST copy */
   /* the data into another Data Buffer.  This function is guaranteed   */
   /* NOT to be invoked more than once simultaneously for the specified */
   /* installed callback (i.e.  this function DOES NOT have be          */
   /* reentrant).  It Needs to be noted however, that if the same       */
   /* Callback is installed more than once, then the callbacks will be  */
   /* called serially.  Because of this, the processing in this function*/
   /* should be as efficient as possible.  It should also be noted that */
   /* this function is called in the Thread Context of a Thread that the*/
   /* User does NOT own.  Therefore, processing in this function should */
   /* be as efficient as possible (this argument holds anyway because   */
   /* other GAP Events will not be processed while this function call is*/
   /* outstanding).                                                     */
   /* * NOTE * This function MUST NOT Block and wait for events that    */
   /*          can only be satisfied by Receiving other GAP Events.  A  */
   /*          Deadlock WILL occur because NO GAP Event Callbacks will  */
   /*          be issued while this function is currently outstanding.  */
void BTPSAPI GAP_Event_Callback(unsigned int BluetoothStackID, GAP_Event_Data_t *GAP_Event_Data, unsigned long CallbackParameter)
{
  int                               Result;
  int                               Index,character_index = 0;
  GAP_Inquiry_Event_Data_t         *GAP_Inquiry_Event_Data;
  GAP_Remote_Name_Event_Data_t     *GAP_Remote_Name_Event_Data;
  GAP_Authentication_Information_t  GAP_Authentication_Information;
  fsm_event_f gap_event = fsm_no_event;
  wchar_t character = 0;
  /* First, check to see if the required parameters appear to be       */
  /* semi-valid.                                                       */
  if((BluetoothStackID) && (GAP_Event_Data))
  {
    /* The parameters appear to be semi-valid, now check to see what  */
    /* type the incoming event is.                                    */
    switch(GAP_Event_Data->Event_Data_Type)
    {
      case etInquiry_Entry_Result:
        /* Yet to study if it's neccesary */
        break;
      case etInquiry_With_RSSI_Entry_Result:
        /* Yet to study if it's neccesary */
        break;
      case etExtended_Inquiry_Entry_Result:
        /* Yet to study if it's neccesary */
        break;
      case etInquiry_Result:
        /* The GAP event received was of type Inquiry_Result.       */
        GAP_Inquiry_Event_Data = GAP_Event_Data->Event_Data.GAP_Inquiry_Event_Data;
        /* Store a list of all the devices found performing the inquiry.  */
        for(Index=0;(Index<GAP_Inquiry_Event_Data->Number_Devices) && (Index<MAX_INQUIRY_RESULTS);Index++)
        {
          InquiryResultList[Index].physical_address = GAP_Inquiry_Event_Data->GAP_Inquiry_Data[Index].BD_ADDR;
        }
        NumberofValidResponses = GAP_Inquiry_Event_Data->Number_Devices;
        gap_event = fsm_h2h_start_connect;
        while(osMailPut(queue_fsm_eventsHandle, (void *) &gap_event) != osOK)
        {
          osDelay(1);
        }
        break;
      case etEncryption_Change_Result:
        /* Yet to study if it's neccesary */
        break;
      case etEncryption_Refresh_Complete:
        /* Yet to study if it's neccesary */
        break;
      case etRemote_Features_Result:
        /* Yet to study if it's neccesary */
        break;
      case etRemote_Version_Information_Result:
        /* Yet to study if it's neccesary */
        break;
      case etRemote_Name_Result:
        /* Yet to study if it's neccesary */
        GAP_Remote_Name_Event_Data = GAP_Event_Data->Event_Data.GAP_Remote_Name_Event_Data;
        for( Index = 0; Index < NumberofValidResponses; Index++)
        {
          if(COMPARE_BD_ADDR(InquiryResultList[Index].physical_address, GAP_Remote_Name_Event_Data->Remote_Device))
          {
              while ((character = GAP_Remote_Name_Event_Data->Remote_Name[character_index]) != 0)
              {
                /* If string still fits into area
                 * store character in area->string
                 */
                if (character_index < MAX_STRING_LENGTH - 1)
                {
                  /* If there is enough space to keep copying the string */
                  InquiryResultList[Index].Name[character_index] = character;
                }
                else
                {
                    /* Close string */
                    InquiryResultList[Index].Name[character_index] = 0;
                    break;
                }
                /* Next character */
                character_index++;
              }
              break;
          }
        }
        if( Index == NumberofValidResponses - 1)
        {
          /* Signal that all the names have been retrieved */
          gap_event = fsm_h2h_ok;
          while(osMailPut(queue_fsm_eventsHandle, (void *) &gap_event) != osOK)
          {
            osDelay(1);
          }
        }
        break;
      case etAuthentication:
        /* An authentication event occurred, determine which type of*/
        /* authentication event occurred.                           */
        switch(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->GAP_Authentication_Event_Type)
        {
          case atLinkKeyRequest:
            /* Setup the authentication information response structure.       */
            GAP_Authentication_Information.GAP_Authentication_Type    = atLinkKey;
            GAP_Authentication_Information.Authentication_Data_Length = 0;

            /* See if we have stored a Link Key for the specified */
            /* device.                                            */
            for(Index = 0 ;Index < MAX_SUPPORTED_LINK_KEYS; Index++)
            {
              if(COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR,
                        GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device))
              {
                /* Link Key information stored,  respond with the stored Link Key */
                GAP_Authentication_Information.Authentication_Data_Length   = sizeof(Link_Key_t);
                GAP_Authentication_Information.Authentication_Data.Link_Key = LinkKeyInfo[Index].LinkKey;

                break;
              }
            }
            /* Submit the authentication response.                */
            Result = GAP_Authentication_Response(BluetoothStackID,
                        GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device,
                        &GAP_Authentication_Information);

            /* Check the result of the submitted command.         */
            if( Result != 0)
            {
              /* Notify of error */
//              HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin, GPIO_PIN_SET);
            }
            break;
          case atPINCodeRequest:
            /* Since we are using with "just work" this is unnecessary*/
            break;
          case atAuthenticationStatus:
            /* Flag that there is no longer a current             */
            /* Authentication procedure in progress.              */
            ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            break;
          case atLinkKeyCreation:
            /* Store the link Key in either a free location OR    */
            /* over the old key location.                         */
            // Vaya mierdon se montan pa buscar una direccion
            for(Index = 0, Result = -1 ;Index < MAX_SUPPORTED_LINK_KEYS; Index++)
            {
              if(COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR,
                      GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device))
              {
                break;
              }
              else
              {
                if((Result == (-1)) && (COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR, NullADDR)))
                {
                  Result = Index;
                }
              }
            }

            /* If we didn't find a match, see if we found an empty*/
            /* location.                                          */
            if(Index == MAX_SUPPORTED_LINK_KEYS)
              Index = Result;

            /* Check to see if we found a location to store the   */
            /* Link Key information into.                         */
            if(Index != (-1))
            {
              LinkKeyInfo[Index].BD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;
              LinkKeyInfo[Index].LinkKey = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Link_Key_Info.Link_Key;
            }
            else
            {
              /* Notify of error */
//              HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin, GPIO_PIN_SET);
            }

            break;
          case atIOCapabilityRequest:
            /* Setup the Authentication Information Response structure.      */
            GAP_Authentication_Information.GAP_Authentication_Type                                      = atIOCapabilities;
            GAP_Authentication_Information.Authentication_Data_Length                                   = sizeof(GAP_IO_Capabilities_t);
            GAP_Authentication_Information.Authentication_Data.IO_Capabilities.IO_Capability            = (GAP_IO_Capability_t)IOCapability;
            GAP_Authentication_Information.Authentication_Data.IO_Capabilities.MITM_Protection_Required = MITMProtection;
            GAP_Authentication_Information.Authentication_Data.IO_Capabilities.OOB_Data_Present         = OOBSupport;

            /* Submit the Authentication Response.                */
            Result = GAP_Authentication_Response(BluetoothStackID,
                         GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device,
                         &GAP_Authentication_Information);

            /* Check the result of the submitted command.         */
            if( Result != 0)
            {
              /* Notify of error */
//              HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin, GPIO_PIN_SET);
            }
            break;
          case atUserConfirmationRequest:
            CurrentRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;
            /* Invoke JUST Works Process...                    */
            GAP_Authentication_Information.GAP_Authentication_Type          = atUserConfirmation;
            GAP_Authentication_Information.Authentication_Data_Length       = (Byte_t)sizeof(Byte_t);
            GAP_Authentication_Information.Authentication_Data.Confirmation = TRUE;

            /* Submit the Authentication Response.             */

            Result = GAP_Authentication_Response(BluetoothStackID,
                         GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device,
                         &GAP_Authentication_Information);
            if( Result == 0)
            {
              /* Flag that there is no longer a current          */
              /* Authentication procedure in progress.           */
              ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            }
            else
            {
              /* Notify error in confirmation */
//              HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin, GPIO_PIN_SET);
            }
            break;
          case atPasskeyRequest:
            /* Since we are using with "just work" this is unnecessary*/
            break;
          case atPasskeyNotification:
            /* Since we are using with "just work" this is unnecessary*/
            break;
          case atKeypressNotification:
            /* Yet to study if it's neccesary */
            break;
          case atRemoteOutOfBandDataRequest:
            /* Yet to study if it's neccesary */
            break;
          case atIOCapabilityResponse:
            /* Remote ioCapabities, this should be used, but not yet */
            break;
          case atSecureSimplePairingComplete:
            /* Yet to study if it's neccesary */
            break;
          default:
            /* An unknown/unexpected GAP event was received.            */
            HAL_GPIO_WritePin(GPIOC,UI_LED_G_Pin, GPIO_PIN_SET);
            break;
        }
        break;
      default:
        /* An unknown/unexpected GAP event was received.            */
        HAL_GPIO_WritePin(GPIOC,UI_LED_G_Pin, GPIO_PIN_SET);
        break;
    }
  }
  else
  {
    /* There was an error with one or more of the input parameters.   */
//    HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin, GPIO_PIN_SET);
  }
}

   /* The following function is for an SPP Event Callback.  This        */
   /* function will be called whenever a SPP Event occurs that is       */
   /* associated with the Bluetooth Stack.  This function passes to the */
   /* caller the SPP Event Data that occurred and the SPP Event Callback*/
   /* Parameter that was specified when this Callback was installed.    */
   /* The caller is free to use the contents of the SPP Event Data ONLY */
   /* in the context of this callback.  If the caller requires the Data */
   /* for a longer period of time, then the callback function MUST copy */
   /* the data into another Data Buffer.  This function is guaranteed   */
   /* NOT to be invoked more than once simultaneously for the specified */
   /* installed callback (i.e.  this function DOES NOT have be          */
   /* reentrant).  It Needs to be noted however, that if the same       */
   /* Callback is installed more than once, then the callbacks will be  */
   /* called serially.  Because of this, the processing in this function*/
   /* should be as efficient as possible.  It should also be noted that */
   /* this function is called in the Thread Context of a Thread that the*/
   /* User does NOT own.  Therefore, processing in this function should */
   /* be as efficient as possible (this argument holds anyway because   */
   /* another SPP Event will not be processed while this function call  */
   /* is outstanding).                                                  */
   /* * NOTE * This function MUST NOT Block and wait for Events that can*/
   /*          only be satisfied by Receiving SPP Event Packets.  A     */
   /*          Deadlock WILL occur because NO SPP Event Callbacks will  */
   /*          be issued while this function is currently outstanding.  */
void BTPSAPI SPP_Event_Callback(unsigned int BluetoothStackID, SPP_Event_Data_t *SPP_Event_Data, unsigned long CallbackParameter)
{
  int       Result;
  bt_packet_t packet = {.packet_content = {0}};
  fsm_event_f spp_event = fsm_no_event;
  /* First, check to see if the required parameters appear to be       */
  /* semi-valid.                                                       */
  if((SPP_Event_Data) && (BluetoothStackID))
  {
    /* The parameters appear to be semi-valid, now check to see what  */
    /* type the incoming event is.                                    */
    switch(SPP_Event_Data->Event_Data_Type)
    {
      case etPort_Open_Indication:
        /* Save the Serial Port ID for later use.                   */
        SerialPortID = SPP_Event_Data->Event_Data.SPP_Open_Port_Indication_Data->SerialPortID;
        /* Flag that we are now connected.                          */
        Connected  = TRUE;
        /* Query the connection handle.                             */
        Result = GAP_Query_Connection_Handle(BluetoothStackID,
                        SPP_Event_Data->Event_Data.SPP_Open_Port_Indication_Data->BD_ADDR,
                        &Connection_Handle);
        if(Result != 0)
        {
          /* Failed to Query the Connection Handle.                */
          Result           = 0;
          Connection_Handle = 0;
        }else{
          /* think for some actions when we already connected */
          HAL_GPIO_WritePin(GPIOC,UI_LED_G_Pin, GPIO_PIN_SET);
        }
        break;
      case etPort_Open_Confirmation:
        /* A Client Port was opened.  The Status indicates the      */
        /* Status of the Open.                                      */
        spp_event =fsm_h2h_error;
        /* Check the Status to make sure that an error did not      */
        /* occur.                                                   */
        if(SPP_Event_Data->Event_Data.SPP_Open_Port_Confirmation_Data->PortOpenStatus)
        {
          /* An error occurred while opening the Serial Port so    */
          /* invalidate the Serial Port ID.                        */
          SerialPortID      = 0;
          Connection_Handle = 0;

          /* Flag that we are no longer connected.                 */
          Connected         = FALSE;
        }
        else
        {
          /* Flag that we are now connected.                       */
          Connected  = TRUE;

          /* Query the connection Handle.                          */
          Result = GAP_Query_Connection_Handle(BluetoothStackID, SelectedBD_ADDR, &Connection_Handle);
          if(Result)
          {
            /* Failed to Query the Connection Handle.             */
            Result           = 0;
            Connection_Handle = 0;
          }
          else
          {
//            spp_event = fsm_h2h_start_gen;
            spp_event = fsm_no_event;
          }
        }
        while(osMailPut(queue_fsm_eventsHandle, (void *) &spp_event) != osOK)
        {
          osDelay(1);
        }
        break;
      case etPort_Close_Port_Indication:
        /* The Remote Port was Disconnected.                        */
        /* If we are the client side, close the Vserialport         */
        if(UI_Mode == UI_MODE_IS_CLIENT)
        {
          SerialPortID = 0;
        }
        Connection_Handle = 0;
        /* Flag that we are no longer connected.                    */
        Connected         = FALSE;
        break;
      case etPort_Status_Indication:
        /* Display Information about the new Port Status.           */
        break;
      case etPort_Data_Indication:
        /* We recieve a packet and send it to bt_rx task */
        SPP_Data_Read(BluetoothStackID, SerialPortID, PACKET_SIZE, (Byte_t *)packet.packet_content);
        /* put into the queue */
        while(osMailPut(queue_bt_packet_recievedHandle, (void *) &packet) != osOK)
        {
          osDelay(1);
        }
        break;
      case etPort_Transmit_Buffer_Empty_Indication:
        /* The transmit buffer is now empty after being full. */
        /* Signal the bt_tx_thread we can send again */

        break;
      case etPort_Line_Status_Indication:
        break;
      case etPort_Send_Port_Information_Indication:
        /* This should be OUR INFO, consider redo it */
        /* Simply Respond with the information that was sent to us. */
        Result = SPP_Respond_Port_Information(BluetoothStackID,
                     SPP_Event_Data->Event_Data.SPP_Send_Port_Information_Indication_Data->SerialPortID,
                     &SPP_Event_Data->Event_Data.SPP_Send_Port_Information_Indication_Data->SPPPortInformation);
        break;
      case etPort_Send_Port_Information_Confirmation:
        break;
      case etPort_Query_Port_Information_Indication:
        break;
      case etPort_Query_Port_Information_Confirmation:
        break;
      case etPort_Open_Request_Indication:
        /* A remote port is requesting a connection.                */
        /* Accept the connection always.                            */
        Result = SPP_Open_Port_Request_Response(BluetoothStackID,
                   SPP_Event_Data->Event_Data.SPP_Open_Port_Request_Indication_Data->SerialPortID,
                   TRUE);
        if( Result == 0)
        {
          SelectedBD_ADDR = SPP_Event_Data->Event_Data.SPP_Open_Port_Request_Indication_Data->BD_ADDR;
        }
        break;
      default:
        /* An unknown/unexpected SPP event was received.            */
        HAL_GPIO_WritePin(GPIOC,UI_LED_G_Pin, GPIO_PIN_SET);
        break;
    }
  }
  else
  {
    /* There was an error with one or more of the input parameters.   */
//    HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin, GPIO_PIN_SET);
  }
}

   /* The following function is responsible for processing HCI Mode     */
   /* change events.                                                    */
void BTPSAPI HCI_Event_Callback(unsigned int BluetoothStackID, HCI_Event_Data_t *HCI_Event_Data, unsigned long CallbackParameter)
{
   char *Mode;

   /* Make sure that the input parameters that were passed to us are    */
   /* semi-valid.                                                       */
   if((BluetoothStackID) && (HCI_Event_Data))
   {
      /* Process the Event Data.                                        */
      switch(HCI_Event_Data->Event_Data_Type)
      {
         case etMode_Change_Event:
            if(HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data)
            {
               switch(HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data->Current_Mode)
               {
                  case HCI_CURRENT_MODE_HOLD_MODE:
                     Mode = "Hold";
                     break;
                  case HCI_CURRENT_MODE_SNIFF_MODE:
                     Mode = "Sniff";
                     break;
                  case HCI_CURRENT_MODE_PARK_MODE:
                     Mode = "Park";
                     break;
                  case HCI_CURRENT_MODE_ACTIVE_MODE:
                  default:
                     Mode = "Active";
                     break;
               }
            }
            break;
         default:
            break;
      }
   }
   // Pa que se calle el compilador, hay que quitarlo en cuanto se pueda
   UNUSED(Mode);
}

static int ExtractRFCOMMPortNumber(SDP_Service_Search_Attribute_Response_Data_t *SDPServiceSearchAttributeResponse, int *RFCOMMPortList)
{
  int NumberOfAttributes;
  int NumberOfRecords;
  int RFCOMMChannel;
  int RetVal=-1;
  int Index;
  int RecordIndex;
  int NumberElements;
  int PortCount=0;
  UUID_128_t UUID_128;
  UUID_128_t RFCOMM_UUID;
  SDP_Data_Element_t *SDP_Data_Element;
  SDP_Data_Element_t *SDP_Data_Element1;

  SDP_ASSIGN_RFCOMM_UUID_128(RFCOMM_UUID);

  /* This is a response received when we queried Attribute ID 0x04 */
  NumberOfRecords = SDPServiceSearchAttributeResponse->Number_Service_Records;

  for(RecordIndex = 0; RecordIndex < NumberOfRecords; RecordIndex++)
  {
    NumberOfAttributes = SDPServiceSearchAttributeResponse->SDP_Service_Attribute_Response_Data[RecordIndex].Number_Attribute_Values;

    if(NumberOfAttributes)
    {
      SDP_Data_Element = SDPServiceSearchAttributeResponse->SDP_Service_Attribute_Response_Data[RecordIndex].SDP_Service_Attribute_Value_Data[0].SDP_Data_Element;

      if((SDP_Data_Element->SDP_Data_Element_Type == deSequence) && (SDP_Data_Element->SDP_Data_Element_Length))
      {
        /* Data Element Sequence Exists, now */
        /* let's loop through the Data Element */
        /* Sequence. */
        for(Index=0; Index < SDP_Data_Element->SDP_Data_Element_Length;Index++)
        {
          if(((SDP_Data_Element1 = &(SDP_Data_Element->SDP_Data_Element.SDP_Data_Element_Sequence[Index])) != NULL)
             && (SDP_Data_Element1->SDP_Data_Element_Type == deSequence)
               && ((NumberElements = SDP_Data_Element1->SDP_Data_Element_Length) > 0)
                 && ((SDP_Data_Element1 = SDP_Data_Element1->SDP_Data_Element.SDP_Data_Element_Sequence) != NULL))
          {
            if((SDP_Data_Element1->SDP_Data_Element_Type == deUUID_16) ||
               (SDP_Data_Element1->SDP_Data_Element_Type == deUUID_32) ||
                 (SDP_Data_Element1->SDP_Data_Element_Type == deUUID_128))
              {
                /* Initialize the BASE UUID. */
                SDP_ASSIGN_BASE_UUID(UUID_128);

                switch(SDP_Data_Element1->SDP_Data_Element_Type)
                {
                  case deUUID_16:
                  /* First normal-lize the*/
                  /* 16 Bit UUID to 128 */
                  /* Bits. */
                  ASSIGN_SDP_UUID_16_TO_SDP_UUID_128(UUID_128, SDP_Data_Element1->SDP_Data_Element.UUID_16);
                  break;
                  case deUUID_32:
                  /* First normal-lize the*/
                  /* 32 Bit UUID to 128 */
                  /* Bits. */
                  ASSIGN_SDP_UUID_32_TO_SDP_UUID_128(UUID_128, SDP_Data_Element1->SDP_Data_Element.UUID_32);
                  break;
                  case deUUID_128:
                  /* Simply assign the */
                  /* 128 Bit UUID to the */
                  /* UUID value. */
                  UUID_128 = SDP_Data_Element1->SDP_Data_Element.UUID_128;
                  break;
                }

                /* Now let's check to see if */
                /* there is a match to RFCOMM */
                if((COMPARE_UUID_128(RFCOMM_UUID, UUID_128)) && (NumberElements > 1))
                {
                if(SDP_Data_Element1[1].SDP_Data_Element_Type == deUnsignedInteger1Byte)
                {
                  RFCOMMChannel = SDP_Data_Element1[1].SDP_Data_Element.UnsignedInteger1Byte;

                  /* Add server port to return data */
                  RFCOMMPortList[PortCount] = RFCOMMChannel;
                  PortCount++;
                  RetVal = 0;
                }
              }
            }
            else
            {
            /* Some unknown Protocol */
            /* Descriptor, so just ignore */
            /* it. */
            }
          }
        }
      }
    }

  }

  return(RetVal);
}

void BTPSAPI SDP_EventCallback(unsigned int BluetoothStackID, unsigned int SDPRequestID,
                SDP_Response_Data_t *SDP_Response_Data, unsigned long CallbackParameter)
{
  int RFCOMMPortList[5];
  fsm_event_f sdp_event;
  /* First, check to see if the required parameters appear to be */
  /* semi-valid. */
  if((SDP_Response_Data != NULL) && (BluetoothStackID))
  {
    /* The parameters appear to be semi-valid, now check to see what */
    /* type the incoming Event is. */
    switch(SDP_Response_Data->SDP_Response_Data_Type)
    {
      case rdTimeout:
        /* Yet to study if it's neccesary */
        break;
      case rdConnectionError:
        /* Yet to study if it's neccesary */
        break;
      case rdErrorResponse:
        /* Yet to study if it's neccesary */
        break;
      case rdServiceSearchResponse:
        /* Yet to study if it's neccesary */
        break;
      case rdServiceAttributeResponse:
        /* Yet to study if it's neccesary */
        break;
      case rdServiceSearchAttributeResponse:
        /* A SDP Service Search Attribute Response was received, */
        BTPS_MemInitialize(RFCOMMPortList, 0, sizeof(RFCOMMPortList));
        sdp_event = fsm_h2h_error;
        /* Call only when you queried for Attribute ID PROTOCOL DESCRIPTOR LIST (0x0004) */
        if(ExtractRFCOMMPortNumber(&SDP_Response_Data->SDP_Response_Data.SDP_Service_Search_Attribute_Response_Data, RFCOMMPortList) == 0)
        {
          if(!OpenRemoteServer(SelectedBD_ADDR, RFCOMMPortList[0]))
          {
            sdp_event = fsm_no_event;
          }
        }
        while(osMailPut(queue_fsm_eventsHandle, (void *) &sdp_event) != osOK)
        {
          osDelay(1);
        }
        break;
      case rdServiceAttributeResponse_Raw:
        /* Yet to study if it's neccesary */
        break;
      case rdServiceSearchAttributeResponse_Raw:
        /* Yet to study if it's neccesary */
        break;
      default:
        break;
    }
  }
}

/* The following function is responsible for issuing a Service Search*/
/* Attribute Request to a Remote SDP Server. This function returns */
/* zero if successful and a negative value if an error occurred. */
int SPPServiceDiscovery(BD_ADDR_t RemoteBDAddr)
{
  int Result;
  int ret_val;
  SDP_UUID_Entry_t SDPUUIDEntry;
  SDP_Attribute_ID_List_Entry_t AttributeID;

  /* First, check that valid Bluetooth Stack ID exists. */
  if(BluetoothStackID)
  {
    /* First let's build the UUID 32 value(s). */
    SDPUUIDEntry.SDP_Data_Element_Type = deUUID_16;
    ASSIGN_SDP_UUID_16(SDPUUIDEntry.UUID_Value.UUID_16, 0x11, 0x01);

    AttributeID.Attribute_Range = (Boolean_t)FALSE;
    AttributeID.Start_Attribute_ID = PROTOCOL_DESCRIPTOR_LIST_ATTR;
    AttributeID.End_Attribute_ID = 0;

    /* Finally submit the SDP Request. */
    Result = SDP_Service_Search_Attribute_Request(BluetoothStackID, RemoteBDAddr,
                      1, &SDPUUIDEntry, 1, &AttributeID, SDP_EventCallback,(unsigned long)0);

    if(Result > 0)
    {
      /* Flag success to the caller. */
      SelectedBD_ADDR = RemoteBDAddr;
      ret_val = 0;
    }
    else
    {
      /* Flag success to the caller. */
      ret_val = 0;
    }

  }
  else
  {
    /* No valid Bluetooth Stack ID exists. */
    ret_val = -1;
  }

  return(ret_val);
}

