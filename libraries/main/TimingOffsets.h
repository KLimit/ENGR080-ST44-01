#ifndef __TIMINGOFFSETS_H__
#define __TIMINGOFFSETS_H__

// all times are recorded in units of ms
// the offsets define when tasks happen within a loop period


#define LOOP_PERIOD 120
#define BT_LOOP_OFFSET 0
#define PRINTER_LOOP_OFFSET 20
#define IMU_LOOP_OFFSET 30
#define GPS_LOOP_OFFSET 40
#define ADC_LOOP_OFFSET 50
#define STATE_ESTIMATOR_LOOP_OFFSET 60
#define P_CONTROL_LOOP_OFFSET 65
#define SEND_DATA_OFFSET 70
#define LOGGER_LOOP_OFFSET 75


#endif
