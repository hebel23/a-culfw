#include "board.h"

#include <stdint.h>

#ifdef USE_RF_MODE

#ifdef ARM
#include <utility/trace.h>
#endif

#include "rf_mode.h"
#include "display.h"
#include "ttydata.h"
#include "rf_receive.h"
#include "delay.h"
#include "cc1100.h"
#include "fastrf.h"
#include "fncollection.h"

#ifdef HAS_ASKSIN
#include "rf_asksin.h"
#endif
#ifdef HAS_MORITZ
#include "rf_moritz.h"
#endif
#ifdef HAS_RWE
#include "rf_rwe.h"
#endif
#ifdef HAS_RFNATIVE
#include "rf_native.h"
#endif
#ifdef HAS_INTERTECHNO
#include "intertechno.h"
#endif
#ifdef HAS_SOMFY_RTS
#include "somfy_rts.h"
#endif
#ifdef HAS_MBUS
#include "rf_mbus.h"
#endif
#ifdef HAS_KOPP_FC
#include "kopp-fc.h"
#endif
#ifdef HAS_ZWAVE
#include "rf_zwave.h"
#endif
#ifdef HAS_MAICO
#include "rf_maico.h"
#endif
#ifdef HAS_RWE
#include "rf_rwe.h"
#endif
#ifdef HAS_ZWAVE
#include "rf_zwave.h"
#endif

CC1101_t CC1101;

void init_RF_mode(void) {
#ifdef HAS_MULTI_CC
  for(uint8_t x=0; x < HAS_MULTI_CC; x++) {
#else
  for(uint8_t x=0; x < 1; x++) {
#endif
    CC1101.RF_mode[x] = RF_mode_off;
    CC1101.RF_mode_save[x] = RF_mode_off;
    CC1101.tx_report[x] = 0;
    hal_CC_GDO_init(x,INIT_MODE_OUT_CS_IN);
  }
  CC1101.instance = 0;
}

uint8_t is_changed_RF_mode(void) {
  return CC1101.RF_mode_save[CC1101.instance] != CC1101.RF_mode[CC1101.instance];
}

uint8_t is_RF_mode(RF_mode_t mode) {
  return CC1101.RF_mode[CC1101.instance] == mode;
}

void change_RF_mode(RF_mode_t mode) {
  save_RF_mode();
  if(mode != CC1101.RF_mode[CC1101.instance]) {
    set_RF_mode(mode);
  }
}

void save_RF_mode(void) {
  CC1101.RF_mode_save[CC1101.instance] = CC1101.RF_mode[CC1101.instance];
}

void set_RF_mode(RF_mode_t mode) {

  CC1101.RF_mode[CC1101.instance] = mode;

  switch(mode) {
    case RF_mode_slow:
      set_txrestore();
      if(!CC1101.tx_report[CC1101.instance])
        CC1101.RF_mode[CC1101.instance] = RF_mode_off;
      if(!(CC1101.instance < NUM_SLOWRF))
        CC1101.RF_mode[CC1101.instance] = RF_mode_off;
      break;
#ifdef HAS_ASKSIN
    case RF_mode_asksin:
      rf_asksin_init();
      break;
#endif
#ifdef HAS_MORITZ
    case RF_mode_moritz:
      rf_moritz_init();
      break;
#endif
#ifdef HAS_MAICO
    case RF_mode_maico:
      rf_maico_init();
      break;
#endif
#ifdef HAS_RFNATIVE
    case RF_mode_native1:
      native_init(1);
      break;
    case RF_mode_native2:
      native_init(2);
      break;
    case RF_mode_native3:
      native_init(3);
      break;
#endif
#ifdef HAS_SOMFY_RTS
    case RF_mode_somfy:
      somfy_rts_tunein();
      break;
#endif
#ifdef HAS_INTERTECHNO
    case RF_mode_intertechno:
      it_tunein();
      break;
#endif
#ifdef HAS_RWE
    case RF_mode_rwe:
      rf_rwe_init();
      break;
#endif
#ifdef HAS_FASTRF
    case RF_mode_fast:
      ccInitChip(EE_FASTRF_CFG);
      ccRX();
      fastrf_on = 1;
      break;
#endif
#ifdef HAS_ZWAVE
    case RF_mode_zwave:
      rf_zwave_init();
      break;
#endif
    default:
      set_ccoff();
#ifdef HAS_FASTRF
      fastrf_on = 0;
#endif
#ifdef HAS_ZWAVE
      zwave_on = 0;
#endif
      CC1101.RF_mode[CC1101.instance] = RF_mode_off;
  }
  TRACE_INFO_WP("%d:Set RF mode to %d\r\n",CC1101.instance,CC1101.RF_mode[CC1101.instance]);
  my_delay_ms(3);
}

RF_mode_t get_RF_mode(void) {
  return CC1101.RF_mode[CC1101.instance];
}

uint8_t restore_RF_mode(void) {
  if(CC1101.RF_mode_save[CC1101.instance] != CC1101.RF_mode[CC1101.instance]) {
    set_RF_mode(CC1101.RF_mode_save[CC1101.instance]);
    return 1;
  }
  return 0;
}

#endif
