/*
 * can_test.c
 *
 *  Created on: Jul 23, 2025
 *      Author: mrohi
 */

#include "main.h"


CAN_RxHeaderTypeDef can_rx = {0};
uint8_t rxData[8];
int flag = 0;

void can_init(void)
{
		MX_CAN1_Init();
		MX_CAN2_Init();

		HAL_CAN_Start(&mycan1);
		HAL_CAN_Start(&mycan2);

		HAL_CAN_ActivateNotification(&mycan2, CAN_IT_RX_FIFO0_MSG_PENDING);

		config_filters();
}

void can_test()
{
		can_tx();

		if(flag)
		{
			if(can_rx.StdId == 0x123)
			{
				printf("Message received %s\n", rxData);
			}
			flag = 0;
		}
}

void config_filters(void)
{
	CAN_FilterTypeDef mycanfilter = {0};
	mycanfilter.FilterActivation = CAN_FILTER_ENABLE;
	mycanfilter.FilterBank = 14;
	mycanfilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	mycanfilter.FilterIdHigh = 0x123 << 5;
	mycanfilter.FilterIdLow = 0x0000;
	mycanfilter.FilterMaskIdHigh = 0x7FF << 5;
	mycanfilter.FilterMaskIdLow = 0x0000;
	mycanfilter.FilterMode = CAN_FILTERMODE_IDMASK;
	mycanfilter.FilterScale = CAN_FILTERSCALE_32BIT;
	mycanfilter.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&mycan2, &mycanfilter);
}


void can_test_tx(void)
{
	CAN_TxHeaderTypeDef can_tx = {0};
	can_tx.DLC = 8;
	can_tx.ExtId = 0;
	can_tx.StdId = 0x123;
	can_tx.IDE = CAN_ID_STD;
	can_tx.RTR = CAN_RTR_DATA;

	uint8_t txData[8] = {'C', 'A', 'N', 'T', 'E', 'S', 'T', '\0'};

	uint32_t tx_Mailbox;
	HAL_CAN_AddTxMessage(&mycan1, &can_tx, txData, &tx_Mailbox);

	while (HAL_CAN_GetTxMailboxesFreeLevel(&mycan1) == 0) {}

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN2)
	{
		flag = 1;
		HAL_CAN_GetRxMessage(&mycan2, CAN_RX_FIFO0, &can_rx, rxData);
	}
}
