#include "includes.h" 

#include "utils.h"
#include "rfal_nfc.h"
#include "st25r3916.h"

int main(void)
{
	systerm_init();

	while(1)
	{
			can_rev_decode();
			app_dispatch();

			HAL_GPIO_TogglePin(MAIN_LED_GPIO_Port, MAIN_LED_Pin);
   }
}