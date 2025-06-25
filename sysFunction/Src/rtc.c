#include "rtc.h"

rtc_parameter_struct rtc_para_struct;

static uint8_t RTC_Dec2BCD(uint8_t val) {
    uint8_t BCD_high = 0;

    while (val >= 10) {
        BCD_high++;
        val -= 10;
    }

    return ((uint8_t) (BCD_high << 4) | val);
}

static uint8_t RTC_BCD2Dec(uint8_t bcd) {
    int decimal = 0;
    int temp = 1;
    int number = 0;

    while (bcd > 0) {
        number = bcd % 16;
        decimal += number * temp;
        temp *= 10;
        bcd /= 16;
    }
    return decimal;
}

void RTC_Set_Time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec) {
    rtc_para_struct.factor_asyn = 0x7F;
    rtc_para_struct.factor_syn = 0xFF;
    rtc_para_struct.year = RTC_Dec2BCD(year);
    rtc_para_struct.month = RTC_Dec2BCD(month);
    rtc_para_struct.date = RTC_Dec2BCD(date);
    rtc_para_struct.hour = RTC_Dec2BCD(hour);
    rtc_para_struct.minute = RTC_Dec2BCD(min);
    rtc_para_struct.second = RTC_Dec2BCD(sec);
    rtc_para_struct.display_format = RTC_24HOUR;

    rtc_init(&rtc_para_struct);
}

void RTC_Set_Time_Struct(time_tt *time) {
    RTC_Set_Time(time->year, time->month, time->date, time->hour, time->min, time->sec);
}

void RTC_Get_Time(time_tt *time) {
    rtc_current_time_get(&rtc_para_struct);
    time->year = RTC_BCD2Dec(rtc_para_struct.year);
    time->month = RTC_BCD2Dec(rtc_para_struct.month);
    time->date = RTC_BCD2Dec(rtc_para_struct.date);
    time->hour = RTC_BCD2Dec(rtc_para_struct.hour);
    time->min = RTC_BCD2Dec(rtc_para_struct.minute);
    time->sec = RTC_BCD2Dec(rtc_para_struct.second);
}

void RTC_Config(void) {
    rcu_periph_clock_enable(RCU_PMU);
    pmu_backup_write_enable();
    rcu_osci_on(RCU_LXTAL);
    rcu_osci_stab_wait(RCU_LXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
    if (RTC_BKP0 == 0x1145) {
    } else {
        RTC_BKP0 = 0x1145;
        RTC_Set_Time(25, 1, 1, 12, 0, 0);
    }
}
