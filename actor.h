#ifndef ACTOR_H
#define ACTOR_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "cmsis_os.h"
#include "queue.h"
#include "state_machine.h"
#include "mempool.h"
#include "event.h"


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define ACTOR_MAX_NUMB					32
#define ACTOR_MAX_RETRY					5

/******************************************************************************
* Configuration Constants
*******************************************************************************/
/* CMSIS RTOS v2 */
#define	portTHREAD_ATTR_T			osThreadAttr_t
#define	portTHREAD_HANDLE_T			osThreadId_t

#define	portEQUEUE_ATTR_T			osMessageQueueAttr_t
#define	portEQUEUE_HANDLE_T			osMessageQueueId_t

#define portWaitTimeout				osWaitForever
/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/
typedef struct Active Active; /* forward declaration */
typedef Active* ActiveId_t;

/* Active Object base class */
struct Active {

	StateMachine_t				sm;		 			/* state machine*/

	portTHREAD_HANDLE_T			thread_handle;		/* private thread */
	portTHREAD_ATTR_T const*	thread_param;

	/* Multiple-write / Single read access */
	portEQUEUE_HANDLE_T			equeue_handle;		/* private message queue */
	portEQUEUE_ATTR_T const*	equeue_param;
    /* active object data added in subclasses of Active */
};

/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void Active_Init(Active *const				me,
				 StateHandler				initial_statehandler,
				 portTHREAD_ATTR_T const*	p_thread_attr,
				 portEQUEUE_ATTR_T const*	p_equeue_attr,
				 uint32_t					equeue_max_len);

/* Direct posting an event to an active objec
 * @param: 	me:		Object to post to
 * @param: 	e:		Pointer to event to post
 * @return: true if posted successfully */

bool Active_post(Active * const me, EvtId_t const e);
uint8_t Active_GetID(Active* const me);
ActiveId_t Active_GetActiveByID(uint8_t id);
#endif /* ACTOR_H */
