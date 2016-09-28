/*
 * bluetooth_internal.h
 *
 *  Created on: 22/6/2015
 *      Author: Smau
 */

#ifndef BLUETOOTH_INTERNAL_H_
#define BLUETOOTH_INTERNAL_H_

#include "HCITRANS.h"
#include "BTVSAPI.h"             /* Vendror Specific header.                  */
#include "SS1BTPS.h"             /* Main SS1 Bluetooth Stack Header.          */
#include "BTPSKRNL.h"            /* BTPS Kernel Header.                       */

#include "bt_const_and_struct.h"
#include "command_interpreter.h"

#define VENDOR_BAUD_RATE          2000000L
#define PACKET_SIZE               32

typedef struct bt_packet_t{
  char       packet_content[PACKET_SIZE];
}bt_packet_t;

/* Init/close function                           */

void bluetooth_init(void);
int CloseStack(void);

/* SPP server functions */
int OpenServer(void);
int CloseServer(void);

int OpenRemoteServer(BD_ADDR_t remote_address, uint16_t server_port);
int CloseRemoteServer(void);

/* Setter */
int SetBaudRate(DWord_t baud_rate);
int SetLocalName(char *device_name);
int SetClassOfDevice(int class_device);
int SetPairabilityMode(GAP_Pairability_Mode_t  PairabilityMode);
int SetConnectabilityMode(GAP_Connectability_Mode_t ConnectableMode);
int SetDiscoverabilityMode(GAP_Discoverability_Mode_t DiscoverabilityMode);
int SetSimplePairingParameters(GAP_IO_Capability_t iocap, Boolean_t mim_proct);
int SetQueueParams(unsigned int maximum_number_data_packets,unsigned int queue_threshold);
int SetConfigParams(uint16_t MaximumFrameSize, uint16_t TransmitBufferSize, uint16_t ReceiveBufferSize);
int SPPServiceDiscovery(BD_ADDR_t RemoteBDAddr);

/* Getters */

int GetQueueParams(unsigned int *params);
int GetRemoteName(BD_ADDR_t remote_address);
int GetLocalAddress(BD_ADDR_t local_addres);
int GetLocalName(char *LocalName, uint16_t lenght);
int GetClassOfDevice(Class_of_Device_t Class_of_Device);
int GetConfigParams(SPP_Configuration_Params_t *SPPConfigurationParams);

/* interative functions                        */

int Inquiry(uint32_t timeout);
int Pair(GAP_Bonding_Type_t bondtype, uint16_t table_pos);
int EndPairing(void);
int PINCodeResponse(char *pin);
int PassKeyResponse(char *Passkey);
int UserConfirmationResponse(Boolean_t confirmation);
int SniffMode( Word_t Sniff_Max_Interval, Word_t Sniff_Min_Interval, 
              Word_t Sniff_Attempt, Word_t Sniff_Timeout);
int ExitSniffMode(void);

int SendData(uint16_t length, char *buff);

int DeleteLinkKey(BD_ADDR_t BD_ADDR);

/* so called Parser */

void BD_ADDRToStr(BD_ADDR_t Board_Address, BoardStr_t BoardStr);

/* Getter */

uint32_t bt_get_remote_devices(device_info_t* inquired_addr);

/* Callbacks for bt events                                   */
void BTPSAPI GAP_Event_Callback(unsigned int BluetoothStackID, GAP_Event_Data_t *GAP_Event_Data, unsigned long CallbackParameter);
void BTPSAPI SPP_Event_Callback(unsigned int BluetoothStackID, SPP_Event_Data_t *SPP_Event_Data, unsigned long CallbackParameter);
void BTPSAPI HCI_Event_Callback(unsigned int BluetoothStackID, HCI_Event_Data_t *HCI_Event_Data, unsigned long CallbackParameter);


#endif /* BLUETOOTH_INTERNAL_H_ */