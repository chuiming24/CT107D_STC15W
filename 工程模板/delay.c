#include <config.h>
#include "delay.h"

void Delay_ms(uint z)
{
	uint x, y;

	for (x = 0; x < 114; x++)
	{
		for (y = 0; y < z; y++);
	}
}