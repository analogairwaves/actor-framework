#ifndef ACTOR_H
#define ACTOR_H

/******************************************************************************
* Includes
*******************************************************************************/
// #include "cmsis_os.h"
// #include "queue.h"
#include "state_machine.h"
#include "event.h"

#include "chip/osal.h"
#include "chip/os_port.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define ACTOR_MAX_NUMB					32
#define ACTOR_MAX_RETRY					5

/******************************************************************************
* Configuration Constants
*******************************************************************************/
#ifdef CMSIS_RTOS2
/* CMSIS RTOS v2 */
#define	portTHREAD_ATTR_T			osThreadAttr_t
#define	portTHREAD_HANDLE_T			osThreadId_t

#define	portEQUEUE_ATTR_T			osMessageQueueAttr_t
#define	portEQUEUE_HANDLE_T			osMessageQueueId_t

#define portWaitTimeout				osWaitForever

#else

typedef struct chip_os_task 	os_task_t;
typedef struct chip_os_queue	os_queue_t;
typedef enum chip_os_err		os_err_code;

#define	portTHREAD_ATTR_T			void *
#define	portTHREAD_HANDLE_T			os_task_t

#define	portEQUEUE_ATTR_T			void *
#define	portEQUEUE_HANDLE_T			os_queue_t

#define portWaitTimeout				CHIP_OS_TIME_FOREVER

#endif /* End ofdef CMSIS_RTOS2 */

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
				 uint8_t 					priority,
				 uint32_t 					stack_size,
				 portTHREAD_ATTR_T const*	p_thread_attr,
				 portEQUEUE_ATTR_T const*	p_equeue_attr,
				 uint32_t					equeue_max_len);

/* Direct posting an event to an active objec
 * @param: 	me:		Object to post to
 * @param: 	e:		Pointer to event to post
 * @return: true if posted successfully */

bool Active_post(Active * const me, EvtHandle_t const e);
uint8_t Active_GetID(Active* const me);
ActiveId_t Active_GetActiveByID(uint8_t id);
uint8_t Framework_Init();
#endif /* ACTOR_H */
