#define SET_BIT(DDR,bit)    (DDR|=1<<bit)
#define RESET_BIT(DDR,bit)  (DDR &= ~(1<<bit))
#define TOGGLE_BIT(DDR,bit) (DDR ^=(1<<bit))
#define GET_BIT(DDR,bit)    ((DDR>>bit)&1)