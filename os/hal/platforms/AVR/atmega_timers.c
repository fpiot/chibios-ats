

#include "atmega_timers.h"


uint16_t ratio_base[]={1024,256,64,8,1};
uint8_t clock_source_base[]={5,4,3,2,1};
uint16_t ratio_extended[]={1024,256,128,64,32,8,1};
uint8_t clock_source_extended[]={7,6,5,4,3,2,1};


uint8_t findBestPrescaler(uint16_t frequency, uint16_t *ratio ,uint8_t *clock_source,uint8_t n)
{
  uint8_t i;
  for(i=0;i<n;i++)
  {
    uint32_t result = F_CPU/ratio[i]/frequency;
    if(result > 256UL)
       return (i-1);
    if(result * ratio[i] * frequency == F_CPU)
      return i;
  };
  
}