/******************************************************************************
* Includes
*******************************************************************************/
#include "actor.h" /* Free Active Object interface */

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
static ActiveId_t Active_List[ACTOR_MAX_NUMB]={0};


/******************************************************************************
* Private Functions
*******************************************************************************/
/*..........................................................................*/
/* Event-loop thread function for all Active Objects (FreeRTOS task signature) */
static void Active_eventLoop(void *pvParameters) {
	Active *me = (Active *)pvParameters;
	configASSERT(me); /* Active object must be provided */

	for (;;) {   /* for-ever "superloop" */

		EvtId_t e; /* pointer to event object ("message") */

		/* wait for any event and receive it into object 'e' */

		osMessageQueueGet(me->equeue_handle, (void*)&e, NULL ,portWaitTimeout); /* BLOCKING! */

		configASSERT(e);

		/* dispatch event to the active object 'me' */
		StateMachine_Dispatch(&me->sm, e);			/* NO BLOCKING! */

		Event_GC(e);

	}
}


/******************************************************************************
* Function Definitions
*******************************************************************************/
void Active_Init(Active *const				me,
				 StateHandler				initial_statehandler,
				 portTHREAD_ATTR_T const*	p_thread_attr,
				 portEQUEUE_ATTR_T const*	p_equeue_attr,
				 uint32_t					equeue_max_len)
{
	static uint8_t active_id = 0;
    configASSERT(me); /* Active object must be provided */
	StateMachine_Init(&me->sm, initial_statehandler);

	/* Initialize the Thread */
	osThreadId_t thread_status = osThreadNew(&Active_eventLoop, me, p_thread_attr);
    configASSERT(thread_status);
	me->thread_handle = thread_status;
	me->thread_param = p_thread_attr;

	/* Initialize the Event queue */
	osMessageQueueId_t equeue_status;
	equeue_status = osMessageQueueNew(equeue_max_len, sizeof(EvtId_t), p_equeue_attr);
    configASSERT(equeue_status);
	me->equeue_handle = equeue_status;
	me->equeue_param = p_equeue_attr;
	Active_List[active_id++] = me;
}

uint8_t Active_GetID(Active* const me)
{
	for(uint8_t idx=0; idx < ACTOR_MAX_NUMB; idx++)
	{
		if(me == Active_List[idx])
			return idx;
	}
	return 0xFF;
}

ActiveId_t Active_GetActiveByID(uint8_t id)
{
	if(id <= ACTOR_MAX_NUMB)
	{
		return Active_List[id];
	}
	return NULL;
}

/*..........................................................................*/
bool Active_post(Active * const me, EvtId_t const e){

	bool ret = false;
	osStatus_t status;
	for(uint8_t count=0; count < ACTOR_MAX_RETRY; count++)
	{
		status = osMessageQueuePut(me->equeue_handle, &e, 0, 0);
		if (status == osOK)
		{
			if(e->xdata.is_dynamic != 0)
			{
				portDISABLE_INTERRUPTS();
				e->xdata.ref_cnt++;
				portENABLE_INTERRUPTS();
			}
			ret = true;
			break;
		}
		else if (status == osErrorTimeout)
		{
			osDelay(50); /* Retry to put in case of full after 50 ticks */
		}
		else
		{
			configASSERT(0);
		}
	}
	return ret;
}

