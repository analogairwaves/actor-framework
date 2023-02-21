#ifndef PS_H_
#define PS_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "actor.h"

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


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void Subscribe(ActiveId_t const actor, EvtId_t e);
void UnSubscribe(ActiveId_t const actor, EvtId_t e);
void UnSubscribeAll(ActiveId_t const actor);
void Publish(EvtId_t e);

#endif /* PS_H_ */
