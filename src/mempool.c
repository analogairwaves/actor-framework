#include "freertos/FreeRTOS.h"
#include "mempool.h"
#ifdef MEMPOOL_H_

/******************************************************************************
* Includes
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define MEMPOOL_TESTING						(1)

#if (CMSIS_RTOS2 == 1)
#define POOL_SMALL_SIZE						50
#define POOL_MEDIUM_SIZE					20
#define POOL_BIG_SIZE						10

#define POOL_SMALL_BLOCKSIZE				10
#define POOL_MEDIUM_BLOCKSIZE				20
#define POOL_BIG_BLOCKSIZE					50
#define POOL_NUMB_SIZE						3	/* SMALL, MEDIUM, BIG */
#else

#define POOL_SIZE							(1024*5) /* 5KB */

#endif /* End of (CMSIS_RTOS2 == 1) */

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define UINT_PTR_CAST(x)                        ((uintptr_t)(x))
#define IS_ALLOC_IN_REGION(ptr, region)         configASSERT(                             \
    UINT_PTR_CAST(ptr) >= UINT_PTR_CAST((region)->start_addr)                       \
    && UINT_PTR_CAST(ptr) < (UINT_PTR_CAST((region)->start_addr) + (region)->size)  \
)
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
#if (CMSIS_RTOS2 == 1)
static MPool_t MemPool[] = {
		{NULL, POOL_SMALL_SIZE, POOL_SMALL_BLOCKSIZE},
		{NULL, POOL_MEDIUM_SIZE, POOL_MEDIUM_BLOCKSIZE},
		{NULL, POOL_BIG_SIZE, POOL_BIG_BLOCKSIZE},
};
MPool_t* Mempool_P = MemPool;

#else
static uint8_t region_mempool_data[1024];
lwmem_region_t mem_pool_regions[] = {
    { region_mempool_data, sizeof(region_mempool_data) },
    { NULL, 0 }
};
#endif /* End of (CMSIS_RTOS2 == 1) */


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
bool Mempool_Init(void)
{
	/* Create memory pool */
	#if (CMSIS_RTOS2 == 1)
	for(uint8_t idx=0; idx< (sizeof(MemPool)/sizeof(MemPool[0])); idx++)
	{
		osMemoryPoolId_t status = osMemoryPoolNew(MemPool[idx].Capacity, MemPool[idx].Blocksize, NULL);
		configASSERT(status);
		MemPool[idx].Handle = status;
	}
	#else
	#endif /* End of (CMSIS_RTOS2 == 1) */

	configASSERT(lwmem_assignmem(mem_pool_regions));

	#if (MEMPOOL_TESTING == 1)
	void* ptr_1;
	ptr_1 = lwmem_malloc(64);
    IS_ALLOC_IN_REGION(ptr_1, &mem_pool_regions[0]);
    lwmem_free(ptr_1);
	#endif /* End of (MEMPOOL_TESTING == 1) */

	return true;
}
#endif /* MEMPOOL_H_ */
