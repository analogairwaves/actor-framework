#ifndef EVENT_H_
#define EVENT_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/



/******************************************************************************
* Typedefs
*******************************************************************************/
typedef uint16_t eSignal;
typedef struct Event_t Evt;
typedef Evt* EvtHandle_t;

typedef struct
{
	uint8_t ref_cnt     : 6; /* Allow multicast and only delete when gabage collector with value 0 */
    uint8_t is_dynamic  : 2; /* 0-> Static, >0 dynamic and the value is the pool with idx -1 (example, dynamic = 1-> pool 0) */
}Evt_extra;

typedef enum {
	INIT_SIG=1,
	ENTRY_SIG,
	EXIT_SIG,
	USER_SIG,
}ReservedSignals;

enum Signal
{
	/* BLE signals */
	SIG_IDLE = USER_SIG,
	/* .... */
	SIG_MAX
};

struct Event_t
{
	eSignal     sig;			/* Inheritance */
    Evt_extra   xdata;			/* Handle static & dynamic events */
};

/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Function declarations
*******************************************************************************/
EvtHandle_t Event_New(eSignal sig, uint16_t evt_size);
void Event_GC(EvtHandle_t e);

#endif /* EVENT_H_ */
