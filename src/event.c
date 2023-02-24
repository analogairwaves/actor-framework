#include <stdio.h>
#include "event.h"

#ifdef EVENT_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include "freertos/FreeRTOS.h"
#ifndef CMSIS_RTOS2
#include "lwmem/lwmem.h"
#else
#include "mempool.h"
#endif /* End of not define CMSIS_RTOS2 */


/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

/* Return the address of allocated memory */
EvtHandle_t Event_New(eSignal sig, uint16_t evt_size)
{
	EvtHandle_t p_e = NULL;
	#if (CMSIS_RTOS2 == 1)
	uint8_t idx;
	for(idx=0; idx < POOL_NUMB_SIZE; idx++)
	{
		if(evt_size <= (Mempool_P+idx)->Blocksize)
		{
			p_e = osMemoryPoolAlloc((Mempool_P+idx)->Handle, portWaitTimeout);
			if(p_e != (EvtHandle_t) NULL)
			{
				p_e->sig = sig;
				p_e->xdata.is_dynamic = (idx+1);
				p_e->xdata.ref_cnt = 0;
				break;
			}
		}
	}
	#else
	p_e = lwmem_malloc(evt_size); //p_e hold the mem addr to store event struct
	configASSERT(p_e); //FIXME: Cant alloc new event
	if(p_e != (EvtHandle_t) NULL)
	{
		p_e->sig = sig;
		p_e->xdata.is_dynamic = 1;
		p_e->xdata.ref_cnt = 0;
	}
	#endif /* End of (CMSIS_RTOS2 == 1) */
	return p_e;
}

/* Event garbage collector */
void Event_GC(EvtHandle_t p_e)
{
	if (p_e->xdata.is_dynamic != 0)
	{
		portDISABLE_INTERRUPTS();
		/* The last one use this event ?*/
		if(p_e->xdata.ref_cnt > 1)
		{
			p_e->xdata.ref_cnt--;
			portENABLE_INTERRUPTS();
		}
		else
		{
			portENABLE_INTERRUPTS();
			#if (CMSIS_RTOS2 == 1)
			uint8_t idx= p_e->xdata.is_dynamic - 1;
			osStatus_t status_free;
			status_free = osMemoryPoolFree((Mempool_P+idx)->Handle, p_e);
			configASSERT(status_free == osOK);
			#else
			lwmem_free(p_e);
			#endif /* End of (CMSIS_RTOS2 == 1) */
		}
	}
}
#endif /* EVENT_H_ */
