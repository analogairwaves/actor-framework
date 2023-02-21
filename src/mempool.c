#include "mempool.h"
#include "freertos/FreeRTOS.h"
#ifdef MEMPOOL_H_

/******************************************************************************
* Includes
*******************************************************************************/

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
static MPool_t MemPool[] = {
		{NULL, POOL_SMALL_SIZE, POOL_SMALL_BLOCKSIZE},
		{NULL, POOL_MEDIUM_SIZE, POOL_MEDIUM_BLOCKSIZE},
		{NULL, POOL_BIG_SIZE, POOL_BIG_BLOCKSIZE},
};

MPool_t* Mempool_P = MemPool;
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
bool Mempool_Init(void){
	/* Create memory pool */
	// for(uint8_t idx=0; idx< (sizeof(MemPool)/sizeof(MemPool[0])); idx++)
	// {
	// 	osMemoryPoolId_t status = osMemoryPoolNew(MemPool[idx].Capacity, MemPool[idx].Blocksize, NULL);
	// 	configASSERT(status);
	// 	MemPool[idx].Handle = status;
	// }
	return true;
}
#endif /* MEMPOOL_H_ */
