
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
  SYS_Init();

  while (1)
  {
    SYS_TaskHandler();
    APP_TaskHandler();
  }
}
