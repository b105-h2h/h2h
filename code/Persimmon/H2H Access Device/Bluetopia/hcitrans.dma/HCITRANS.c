/*****< hcitrans.c >***********************************************************/
/*      Copyright 2012 - 2013 Stonestreet One.                                */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*  HCITRANS - HCI Transport Layer for use with Bluetopia.                    */
/*                                                                            */
/*  Author:  Marcus Funk                                                      */
/*                                                                            */
/*** MODIFICATION HISTORY *****************************************************/
/*                                                                            */
/*   mm/dd/yy  F. Lastname    Description of Modification                     */
/*   --------  -----------    ------------------------------------------------*/
/*   11/08/12  M. Funk        Initial creation.                               */
/*   06/30/16  T. Valencia    Ported to STM32L1                               */
/******************************************************************************/

#include "HCITRANS.h"       /* HCI Transport Prototypes/Constants.            */

   /* Internal Variables to this Module     */

int                      HCITransportOpen        = 0;
uint8_t                  RxBuffer[INPUT_BUFFER_SIZE];
unsigned long            COMDataCallbackParameter;
HCITR_COMDataCallback_t  COMDataCallbackFunction;
osSemaphoreId            sem_bt_data_receiveHandle;
osThreadId               bt_recieveTaskHandle;

void Start_bt_transportTask(void const * argument)
{
  for(;;)
  {
    /* If the transport is open and we have a new data              */
    if(osSemaphoreWait(sem_bt_data_receiveHandle , osWaitForever) == osOK)
    {
      if(HCITransportOpen)
      {
        /* Call the upper layer back with the data.                    */
        (*COMDataCallbackFunction)(TRANSPORT_ID, 1,RxBuffer, COMDataCallbackParameter);
        /* We start a new reception */
        HAL_UART_Receive_DMA(&huart1, RxBuffer, 1);
      }
    }

  }
}
   /* The following function is responsible for opening the HCI         */
   /* Transport layer that will be used by Bluetopia to send and receive*/
   /* COM (Serial) data.  This function must be successfully issued in  */
   /* order for Bluetopia to function.  This function accepts as its    */
   /* parameter the HCI COM Transport COM Information that is to be used*/
   /* to open the port.  The final two parameters specify the HCI       */
   /* Transport Data Callback and Callback Parameter (respectively) that*/
   /* is to be called when data is received from the UART.  A successful*/
   /* call to this function will return a non-zero, positive value which*/
   /* specifies the HCITransportID that is used with the remaining      */
   /* transport functions in this module.  This function returns a      */
   /* negative return value to signify an error.                        */
int BTPSAPI HCITR_COMOpen(HCI_COMMDriverInformation_t *COMMDriverInformation, 
                          HCITR_COMDataCallback_t COMDataCallback, 
                          unsigned long CallbackParameter)
{
   int ret_val;

   /* First, make sure that the port is not already open and make sure  */
   /* that valid COMM Driver Information was specified.                 */
   if((!HCITransportOpen) && (COMMDriverInformation) && (COMDataCallback))
   {
      /* Initialize the return value for success.                       */
      ret_val               = TRANSPORT_ID;

      /* Initialize the context .                                       */
      
      COMDataCallbackFunction  = COMDataCallback;
      COMDataCallbackParameter = CallbackParameter;

      /* If this is the first time opening the stack, create the task   */
      /* and the semaphore                                              */
      if(!sem_bt_data_receiveHandle)
      {
        /* Create the event that will be used to signal data has arrived. */
        osSemaphoreDef(sem_BtRxDMA);
        sem_bt_data_receiveHandle = osSemaphoreCreate(osSemaphore(sem_BtRxDMA), 1);
        osSemaphoreWait(sem_bt_data_receiveHandle , osWaitForever);
      }  

      if(!bt_recieveTaskHandle)
      {
         osThreadDef(BtRxDmaTask, Start_bt_transportTask, osPriorityRealtime, 0, 256);
         bt_recieveTaskHandle = osThreadCreate(osThread(BtRxDmaTask), NULL);
      }
      if((sem_bt_data_receiveHandle)&&(bt_recieveTaskHandle))
      {
        HAL_UART_Init(&huart1);                                           
        BTPS_Delay(10);
        HAL_GPIO_WritePin(GPIOC,BT_nSHUTD_Pin,GPIO_PIN_SET);
        BTPS_Delay(250);
        
        /* Flag that the HCI Transport is open.                           */
        HCITransportOpen = 1;
        /* Start the reception */ 
        HAL_UART_Receive_DMA(&huart1, RxBuffer, 1);
      }else{
         HCITransportOpen = 0;
         ret_val = HCITR_ERROR_UNABLE_TO_OPEN_TRANSPORT;
      }
  }else{
      ret_val = HCITR_ERROR_UNABLE_TO_OPEN_TRANSPORT;
  }
   return(ret_val);
}

   /* The following function is responsible for closing the specific HCI*/
   /* Transport layer that was opened via a successful call to the      */
   /* HCITR_COMOpen() function (specified by the first parameter).      */
   /* Bluetopia makes a call to this function whenever an either        */
   /* Bluetopia is closed, or an error occurs during initialization and */
   /* the driver has been opened (and ONLY in this case).  Once this    */
   /* function completes, the transport layer that was closed will no   */
   /* longer process received data until the transport layer is         */
   /* Re-Opened by calling the HCITR_COMOpen() function.                */
   /* * NOTE * This function *MUST* close the specified COM Port.  This */
   /*          module will then call the registered COM Data Callback   */
   /*          function with zero as the data length and NULL as the    */
   /*          data pointer.  This will signify to the HCI Driver that  */
   /*          this module is completely finished with the port and     */
   /*          information and (more importantly) that NO further data  */
   /*          callbacks will be issued.  In other words the very last  */
   /*          data callback that is issued from this module *MUST* be a*/
   /*          data callback specifying zero and NULL for the data      */
   /*          length and data buffer (respectively).                   */
void BTPSAPI HCITR_COMClose(unsigned int HCITransportID)
{
   HCITR_COMDataCallback_t COMDataCallback;

   /* Check to make sure that the specified Transport ID is valid.      */
   if((HCITransportID == TRANSPORT_ID) && (HCITransportOpen))
   {
      /* Flag that the HCI Transport is no longer open.                 */
      HCITransportOpen = 0;
      /* We use the HAL de init function                                */ 
      HAL_UART_MspDeInit(&huart1);
      /* Place the Bluetooth Device in Reset.                           */
      HAL_GPIO_WritePin(GPIOC,BT_nSHUTD_Pin,GPIO_PIN_RESET);

      /* Signal the receive thread to terminate and wait for it to      */
      /* close.                                                         */
      xSemaphoreGive(sem_bt_data_receiveHandle);
      
//      while( osThreadTerminate (bt_recieveTaskHandle) != osOK){
//        BTPS_Delay(1);
//      }
//      /* Close the semaphore.                                           */
//      osSemaphoreDelete(sem_bt_data_receiveHandle);
      
      /* Note the Callback information.                                 */
      COMDataCallback   = COMDataCallbackFunction;

      COMDataCallbackFunction = NULL;

      /* All finished, perform the callback to let the upper layer know */
      /* that this module will no longer issue data callbacks and is    */
      /* completely cleaned up.                                         */
      if(COMDataCallback)
         (*COMDataCallback)(TRANSPORT_ID, 0, NULL, COMDataCallbackParameter);

      COMDataCallbackParameter = 0;

   }
}

   /* The following function is responsible for instructing the         */
   /* specified HCI Transport layer (first parameter) that was opened   */
   /* via a successful call to the HCITR_COMOpen() function to          */
   /* reconfigure itself with the specified information.                */
   /* * NOTE * This function does not close the HCI Transport specified */
   /*          by HCI Transport ID, it merely reconfigures the          */
   /*          transport.  This means that the HCI Transport specified  */
   /*          by HCI Transport ID is still valid until it is closed via*/
   /*          the HCI_COMClose() function.                             */
void BTPSAPI HCITR_COMReconfigure(unsigned int HCITransportID, HCI_Driver_Reconfigure_Data_t *DriverReconfigureData)
{
   HCI_COMMReconfigureInformation_t *ReconfigureInformation;

   /* Check to make sure that the specified Transport ID is valid.      */
   if((HCITransportID == TRANSPORT_ID) && (HCITransportOpen) && (DriverReconfigureData))
   {
      if((DriverReconfigureData->ReconfigureCommand == HCI_COMM_DRIVER_RECONFIGURE_DATA_COMMAND_CHANGE_COMM_PARAMETERS) && (DriverReconfigureData->ReconfigureData))
      {
         ReconfigureInformation = (HCI_COMMReconfigureInformation_t *)(DriverReconfigureData->ReconfigureData);

         /* Check if the baud rate needs to change.                     */
         if(ReconfigureInformation->ReconfigureFlags & HCI_COMM_RECONFIGURE_INFORMATION_RECONFIGURE_FLAGS_CHANGE_BAUDRATE)
         {
            huart1.Init.BaudRate = ReconfigureInformation->BaudRate;
            if (HAL_UART_Init(&huart1) != HAL_OK)
            {
              Error_Handler();
            }            
         }
      }
   }
}

   /* The following function is responsible for actually sending data   */
   /* through the opened HCI Transport layer (specified by the first    */
   /* parameter).  Bluetopia uses this function to send formatted HCI   */
   /* packets to the attached Bluetooth Device.  The second parameter to*/
   /* this function specifies the number of bytes pointed to by the     */
   /* third parameter that are to be sent to the Bluetooth Device.  This*/
   /* function returns a zero if the all data was transferred           */
   /* successfully or a negative value if an error occurred.  This      */
   /* function MUST NOT return until all of the data is sent (or an     */
   /* error condition occurs).  Bluetopia WILL NOT attempt to call this */
   /* function repeatedly if data fails to be delivered.  This function */
   /* will block until it has either buffered the specified data or sent*/
   /* all of the specified data to the Bluetooth Device.                */
   /* * NOTE * The type of data (Command, ACL, SCO, etc.) is NOT passed */
   /*          to this function because it is assumed that this         */
   /*          information is contained in the Data Stream being passed */
   /*          to this function.                                        */
int BTPSAPI HCITR_COMWrite(unsigned int HCITransportID, unsigned int Length, unsigned char *Buffer)
{
   int ret_val;
   HAL_UART_StateTypeDef current_state;

   /* Check to make sure that the specified Transport ID is valid and   */
   /* the output buffer appears to be valid as well.                    */
   if((HCITransportID == TRANSPORT_ID) && (HCITransportOpen) && (Length) && (Buffer))
   {
     
     current_state = HAL_UART_GetState(&huart1);
      while((current_state != HAL_UART_STATE_READY)
            &&(current_state != HAL_UART_STATE_BUSY_RX)){
          BTPS_Delay(1);
      }
      HAL_UART_Transmit_DMA(&huart1, Buffer, Length);
      
      ret_val = 0;
   }else{
     ret_val = HCITR_ERROR_WRITING_TO_PORT;
   }
   return(ret_val);
}

   /* The following function is responsible for suspending the HCI COM  */
   /* transport.  It will block until the transmit buffers are empty and*/
   /* all data has been sent then put the transport in a suspended      */
   /* state.  This function will return a value of 0 if the suspend was */
   /* successful or a negative value if there is an error.              */
   /* * NOTE * An error will occur if the suspend operation was         */
   /*          interrupted by another event, such as data being received*/
   /*          before the transmit buffer was empty.                    */
   /* * NOTE * The calling function must lock the Bluetooth stack with a*/
   /*          call to BSC_LockBluetoothStack() before this function is */
   /*          called.                                                  */
   /* * NOTE * This function should only be called when the baseband    */
   /*          low-power protocol in use has indicated that it is safe  */
   /*          to sleep.  Also, this function must be called            */
   /*          successfully before any clocks necessary for the         */
   /*          transport to operate are disabled.                       */

int BTPSAPI HCITR_COMSuspend(unsigned int HCITransportID)
{
   int ret_val;

#ifdef SUPPORT_TRANSPORT_SUSPEND

   if(HCITransportID == TRANSPORT_ID)
   {
      /* Signal that we are waiting for a suspend operation to complete.*/
      UartContext.SuspendState = hssSuspendWait;

      /* Set the CTS interrupt.                                         */
      SetSuspendGPIO(TRUE);

      /* Wait for the UART transmit buffer and FIFO to be empty.        */
      while(((UartContext.TxBytesFree != OUTPUT_BUFFER_SIZE) || (UartContext.TxPreviousDMALength) 
             || (USART_GetFlagStatus(HCITR_UART_BASE, USART_FLAG_TC) != SET)) && (UartContext.SuspendState == hssSuspendWait))
         BTPS_Delay(1);

      /* Confirm that no data was received in this time and suspend the */
      /* UART.                                                          */
      DisableInterrupts();

      if(UartContext.SuspendState == hssSuspendWait)
      {
         UartContext.SuspendState = hssSuspended;

         /* Disable the UART clock.                                     */
//         DisableUartPeriphClock();

         ret_val = 0;
      }
      else
      {
         /* Data was received, abort suspending the UART.               */
         SetSuspendGPIO(FALSE);

         ret_val = HCITR_ERROR_SUSPEND_ABORTED;
      }

      EnableInterrupts();

  }else{
      ret_val = HCITR_ERROR_INVALID_PARAMETER;
  }
#else

   ret_val = HCITR_ERROR_INVALID_PARAMETER;

#endif

   return(ret_val);
}

