
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"

static void APP_TaskHandler(void)
{
  // Put your application code here
}

int main(void)
{
  //SYS_Init(); //Commented out until wireless hardware is tuned

  while (1)
  {
    //SYS_TaskHandler(); //Commented out until wireless hardware is tuned
    APP_TaskHandler();
  }
}
