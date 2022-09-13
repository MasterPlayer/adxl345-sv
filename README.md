# adxl345

Компонент, предназначенный для организации взаимодействия с акселерометром ADXL345. 
Содержит в своем составе два регистровых пространства, одно для конфигурации компонента, второе отображает состояние регистров ADXL345

Необходимость подобного компонента обуславливается тем, что при его отсутствии, при обычной организации, контроллер I2C процессора соединяется с компонентом напрямую, а также контроллер прерываний


## Организация аппаратной части

Функционал компонента позволяет: 
1) Организация обработки прерывания
2) Запрос данных от ADXL345 по таймеру
3) Механизм калибровки устройства
4) Разрешение/запрет прерываний
 - Организация механизма обработки прерывания посредством чтения регистров устройства
5) Сбор статистики использования
- Количество операций чтения
- Количество операций записи
- Скорость чтения в байтах/с
- Скорость записи в байтах/с
- Время калибровки
- Состояние линка между ADXL345 и компонентом
- Текущее состояние внутренней логики (работа/состояние простоя)
- Состояние калибровки (не запущена, в процессе, завершена)
6) Установка адреса I2C
7) Управление компонентом (сброс/реинициализация)
8) Запрос указанного количества данных с ADXL345 с указанного адреса



## Организация софтовой части 

Организация софта включает в себя два отдельных блока: работа с регистрами компонента(config, cfg) и работа с регистрами устроства (device, dev)

### Configuration register 

Регистровое пространство предназначено для управления компонентом. 


### Device register

Набор API для взаимодействия с регистрами устройства (device)

Представленный список функций, реализующих взаимодействие с регистрами устройства представлен в таблице 

Примечание: Все функции можно разделить на несколько категорий:
`set` - Установка передаваемого в функцию значения в регистр устройства
`get` - Возврат значения регистра(в качестве параметра передается указатель)
`is` - Возврат TRUE/FALSE, если передаваемый в функцию параметр соответствует значению в регистре
`has` - Возврат TRUE/FALSE 

Примечание : некоторые функции работают над несколькими полями(FIELD).  

ADDR | REG[BIT]           | FIELD NAME   | FUNCTIONS |
-----|--------------------|--------------|-----------|
0x00 | DEVICE ID[7:0]     | DEVICE ID    | axi_adxl_get_device_id(axi_adxl *ptr, uint8_t *device_id)
0x1D | THRESH_TAP[7:0]    | THRESH TAP   | axi_adxl_set_thresh_tap(axi_adxl* ptr, uint8_t thresh_tap)
0x1D | THRESH_TAP[7:0]    | THRESH TAP   | axi_adxl_get_thresh_tap(axi_adxl* ptr, uint8_t *thresh_tap)
0x1E | OFSX[7:0]          | OFSX         | axi_adxl_set_ofsx(axi_adxl* ptr, int8_t ofsx)
0x1E | OFSX[7:0]          | OFSX         | axi_adxl_get_ofsx(axi_adxl* ptr, int8_t *ofsx)
0x1F | OFSY[7:0]          | OFSY         | axi_adxl_set_ofsy(axi_adxl* ptr, int8_t ofsy)
0x1F | OFSY[7:0]          | OFSY         | axi_adxl_get_ofsy(axi_adxl* ptr, int8_t *ofsy)
0x20 | OFSZ[7:0]          | OFSZ         | axi_adxl_set_ofsz(axi_adxl* ptr, int8_t ofsz)
0x20 | OFSZ[7:0]          | OFSZ         | axi_adxl_get_ofsz(axi_adxl* ptr, int8_t *ofsz)
0x21 | DUR[7:0]           | DUR          | axi_adxl_set_dur(axi_adxl* ptr, uint8_t duration)
0x21 | DUR[7:0]           | DUR          | axi_adxl_get_dur(axi_adxl* ptr, uint8_t *duration)
0x22 | LATENT[7:0]        | LATENT       | axi_adxl_set_latent(axi_adxl* ptr, uint8_t latent)
0x22 | LATENT[7:0]        | LATENT       | axi_adxl_get_latent(axi_adxl* ptr, uint8_t *latent)
0x23 | WINDOW[7:0]        | WINDOW       | axi_adxl_set_window(axi_adxl *ptr, uint8_t window)
0x23 | WINDOW[7:0]        | WINDOW       | axi_adxl_get_window(axi_adxl *ptr, uint8_t *window)
0x24 | THRESH_ACT[7:0]    | THRESH_ACT   | axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t thresh_act)
0x24 | THRESH_ACT[7:0]    | THRESH_ACT   | axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *thresh_act)
0x25 | THRESH_INACT[7:0]  | THRESH_INACT | axi_adxl_set_thresh_inact(axi_adxl *ptr, uint8_t thresh_inact)
0x25 | THRESH_INACT[7:0]  | THRESH_INACT | axi_adxl_get_thresh_inact(axi_adxl *ptr, uint8_t *thresh_inact)
0x26 | TIME_INACT[7:0]    | TIME_INACT   | axi_adxl_set_time_inact(axi_adxl *ptr, uint8_t time_inact)
0x26 | TIME_INACT[7:0]    | TIME_INACT   | axi_adxl_get_time_inact(axi_adxl *ptr, uint8_t *time_inact)
0x27 | ACT_INACT_CTL[7]   | ACT AC/DC    | axi_adxl_set_activity_acdc(axi_adxl* ptr, enum acdc_enum acdc_mode)
0x27 | ACT_INACT_CTL[7]   | ACT AC/DC    | axi_adxl_get_activity_acdc(axi_adxl* ptr, enum acdc_enum *acdc_mode)
0x27 | ACT_INACT_CTL[7]   | ACT AC/DC    | axi_adxl_has_activity_ac(axi_adxl* ptr)
0x27 | ACT_INACT_CTL[7]   | ACT AC/DC    | axi_adxl_has_activity_dc(axi_adxl* ptr)
0x27 | ACT_INACT_CTL[6]   | ACT_X EN     | axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask)
0x27 | ACT_INACT_CTL[6]   | ACT_X EN     | axi_adxl_enable_activity_control(axi_adxl* ptr, enum act_enum act_mask)
0x27 | ACT_INACT_CTL[6]   | ACT_X EN     | axi_adxl_disable_activity_control(axi_adxl* ptr, enum act_enum act_mask)
0x27 | ACT_INACT_CTL[5]   | ACT_Y EN     | axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask)
0x27 | ACT_INACT_CTL[5]   | ACT_Y EN     | axi_adxl_enable_activity_control(axi_adxl* ptr, enum act_enum act_mask)
0x27 | ACT_INACT_CTL[5]   | ACT_Y EN     | axi_adxl_disable_activity_control(axi_adxl* ptr, enum act_enum act_mask)
0x27 | ACT_INACT_CTL[4]   | ACT_Z EN     | axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask)
0x27 | ACT_INACT_CTL[4]   | ACT_Z EN     | axi_adxl_enable_activity_control(axi_adxl* ptr, enum act_enum act_mask)
0x27 | ACT_INACT_CTL[4]   | ACT_Z EN     | axi_adxl_disable_activity_control(axi_adxl* ptr, enum act_enum act_mask)
0x27 | ACT_INACT_CTL[3]   | INACT AC/DC  | axi_adxl_set_inactivity_acdc(axi_adxl* ptr, enum acdc_enum acdc_mode)
0x27 | ACT_INACT_CTL[3]   | INACT AC/DC  | axi_adxl_get_inactivity_acdc(axi_adxl* ptr, enum acdc_enum *acdc_mode)
0x27 | ACT_INACT_CTL[3]   | INACT AC/DC  | axi_adxl_has_inactivity_ac(axi_adxl* ptr)
0x27 | ACT_INACT_CTL[3]   | INACT AC/DC  | axi_adxl_has_inactivity_dc(axi_adxl* ptr)
0x27 | ACT_INACT_CTL[2]   | INACT_X EN   | axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask)
0x27 | ACT_INACT_CTL[2]   | INACT_X EN   | axi_adxl_enable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask)
0x27 | ACT_INACT_CTL[2]   | INACT_X EN   | axi_adxl_disable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask)
0x27 | ACT_INACT_CTL[1]   | INACT_Y EN   | axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask)
0x27 | ACT_INACT_CTL[1]   | INACT_Y EN   | axi_adxl_enable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask)
0x27 | ACT_INACT_CTL[1]   | INACT_Y EN   | axi_adxl_disable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask)
0x27 | ACT_INACT_CTL[0]   | INACT_Z EN   | axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask)
0x27 | ACT_INACT_CTL[0]   | INACT_Z EN   | axi_adxl_enable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask)
0x27 | ACT_INACT_CTL[0]   | INACT_Z EN   | axi_adxl_disable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask)
0x28 | THRESH_FF[7:0]     | THRESH_FF    | axi_adxl_set_thresh_ff(axi_adxl* ptr, uint8_t thresh_ff)
0x28 | THRESH_FF[7:0]     | THRESH_FF    | axi_adxl_get_thresh_ff(axi_adxl* ptr, uint8_t *thresh_ff)
0x29 | TIME_FF[7:0]       | TIME_FF      | axi_adxl_set_time_ff(axi_adxl* ptr, uint8_t time_ff)
0x29 | TIME_FF[7:0]       | TIME_FF      | axi_adxl_get_time_ff(axi_adxl* ptr, uint8_t *time_ff)
0x2A | TAP_AXES[3]        | SUPRESS      | axi_adxl_is_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[3]        | SUPRESS      | axi_adxl_switch_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[2]        | TAP_X EN     | axi_adxl_is_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[2]        | TAP_X EN     | axi_adxl_switch_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[1]        | TAP_Y_EN     | axi_adxl_is_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[1]        | TAP_Y_EN     | axi_adxl_switch_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[0]        | TAP_Z_EN     | axi_adxl_is_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2A | TAP_AXES[0]        | TAP_Z_EN     | axi_adxl_switch_tap_axes(axi_adxl* ptr, uint8_t mask)
0x2B | ACT_TAP_STATUS[6]  | ACT_X_SRC    | axi_adxl_is_act_src_status(axi_adxl* ptr, enum act_tap_status_enum act)
0x2B | ACT_TAP_STATUS[5]  | ACT_Y_SRC    | axi_adxl_is_act_src_status(axi_adxl* ptr, enum act_tap_status_enum act)
0x2B | ACT_TAP_STATUS[4]  | ACT_Z_SRC    | axi_adxl_is_act_src_status(axi_adxl* ptr, enum act_tap_status_enum act)
0x2B | ACT_TAP_STATUS[3]  | ASLEEP       | axi_adxl_has_asleep_status(axi_adxl* ptr)
0x2B | ACT_TAP_STATUS[2]  | TAP_X_SRC    | axi_adxl_is_tap_src_status(axi_adxl* ptr, enum act_tap_status_enum tap)
0x2B | ACT_TAP_STATUS[1]  | TAP_Y_SRC    | axi_adxl_is_tap_src_status(axi_adxl* ptr, enum act_tap_status_enum tap)
0x2B | ACT_TAP_STATUS[0]  | TAP_Z_SRC    | axi_adxl_is_tap_src_status(axi_adxl* ptr, enum act_tap_status_enum tap)
0x2C | BW_RATE[4]         | LOW POWER    | axi_adxl_set_bw_rate(axi_adxl *ptr, enum bw_rate_enum bw_rate)
0x2C | BW_RATE[4]         | LOW POWER    | axi_adxl_get_bw_rate(axi_adxl *ptr, enum bw_rate_enum *bw_rate)
0x2C | BW_RATE[3:0]       | RATE         | axi_adxl_set_bw_rate(axi_adxl *ptr, enum bw_rate_enum bw_rate)
0x2C | BW_RATE[3:0]       | RATE         | axi_adxl_get_bw_rate(axi_adxl *ptr, enum bw_rate_enum *bw_rate)
0x2D | POWER_CTL[5]       | LINK         | axi_adxl_enable_linking_mode(axi_adxl* ptr)
0x2D | POWER_CTL[5]       | LINK         | axi_adxl_disable_linking_mode(axi_adxl* ptr)
0x2D | POWER_CTL[5]       | LINK         | axi_adxl_has_linking_mode(axi_adxl* ptr)
0x2D | POWER_CTL[4]       | AUTO         | axi_adxl_enable_autosleep_mode(axi_adxl* ptr)
0x2D | POWER_CTL[4]       | AUTO         | axi_adxl_disable_autosleep_mode(axi_adxl* ptr)
0x2D | POWER_CTL[4]       | AUTO         | axi_adxl_has_autosleep_mode(axi_adxl* ptr)
0x2D | POWER_CTL[3]       | MEASURE      | axi_adxl_enable_msmt(axi_adxl *ptr)
0x2D | POWER_CTL[3]       | MEASURE      | axi_adxl_disable_msmt(axi_adxl *ptr)
0x2D | POWER_CTL[3]       | MEASURE      | axi_adxl_has_msmt(axi_adxl* ptr)
0x2D | POWER_CTL[2]       | SLEEP        | axi_adxl_enable_sleep_mode(axi_adxl* ptr)
0x2D | POWER_CTL[2]       | SLEEP        | axi_adxl_disable_sleep_mode(axi_adxl* ptr)
0x2D | POWER_CTL[2]       | SLEEP        | axi_adxl_has_sleep_mode(axi_adxl* ptr)
0x2D | POWER_CTL[1:0]     | WAKEUP       | axi_adxl_set_wakeup(axi_adxl* ptr, enum wakeup_enum wakeup)
0x2D | POWER_CTL[1:0]     | WAKEUP       | axi_adxl_get_wakeup(axi_adxl* ptr, enum wakeup_enum *wakeup)
0x2D | POWER_CTL[1:0]     | WAKEUP       | axi_adxl_is_wakeup(axi_adxl* ptr, enum wakeup_enum wakeup)
0x2E | INT_ENABLE[7]      | DATA_READY   | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[7]      | DATA_READY   | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[7]      | DATA_READY   | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[6]      | SINGLE_TAP   | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[6]      | SINGLE_TAP   | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[6]      | SINGLE_TAP   | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[5]      | DOUBLE_TAP   | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[5]      | DOUBLE_TAP   | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[5]      | DOUBLE_TAP   | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[4]      | ACTIVITY     | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[4]      | ACTIVITY     | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[4]      | ACTIVITY     | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[3]      | INACTIVITY   | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[3]      | INACTIVITY   | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[3]      | INACTIVITY   | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[2]      | FREE_FALL    | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[2]      | FREE_FALL    | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[2]      | FREE_FALL    | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[1]      | WATERMARK    | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[1]      | WATERMARK    | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[1]      | WATERMARK    | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[0]      | OVERRUN      | axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[0]      | OVERRUN      | axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2E | INT_ENABLE[0]      | OVERRUN      | axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr)
0x2F | INT_MAP[7]         | DATA_READY   | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[7]         | DATA_READY   | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[7]         | DATA_READY   | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[6]         | SINGLE_TAP   | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[6]         | SINGLE_TAP   | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[6]         | SINGLE_TAP   | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[5]         | DOUBLE_TAP   | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[5]         | DOUBLE_TAP   | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[5]         | DOUBLE_TAP   | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[4]         | ACTIVITY     | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[4]         | ACTIVITY     | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[4]         | ACTIVITY     | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[3]         | INACTIVITY   | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[3]         | INACTIVITY   | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[3]         | INACTIVITY   | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[2]         | FREE_FALL    | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[2]         | FREE_FALL    | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[2]         | FREE_FALL    | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[1]         | WATERMARK    | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[1]         | WATERMARK    | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[1]         | WATERMARK    | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[0]         | OVERRUN      | axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map)
0x2F | INT_MAP[0]         | OVERRUN      | axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x2F | INT_MAP[0]         | OVERRUN      | axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask)
0x30 | INT_SOURCE[7]      | DATA_READY   | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[7]      | DATA_READY   | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[6]      | SINGLE_TAP   | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[6]      | SINGLE_TAP   | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[5]      | DOUBLE_TAP   | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[5]      | DOUBLE_TAP   | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[4]      | ACTIVITY     | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[4]      | ACTIVITY     | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[3]      | INACTIVITY   | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[3]      | INACTIVITY   | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[2]      | FREE_FALL    | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[2]      | FREE_FALL    | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[1]      | WATERMARK    | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[1]      | WATERMARK    | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x30 | INT_SOURCE[0]      | OVERRUN      | axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask)
0x30 | INT_SOURCE[0]      | OVERRUN      | axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt)
0x31 | DATA_FORMAT[7]     | SELFTEST     | axi_adxl_enable_selftest(axi_adxl *ptr)
0x31 | DATA_FORMAT[7]     | SELFTEST     | axi_adxl_disable_selftest(axi_adxl* ptr)
0x31 | DATA_FORMAT[7]     | SELFTEST     | axi_adxl_has_selftest(axi_adxl* ptr)
0x31 | DATA_FORMAT[6]     | SPI          | axi_adxl_set_spi_mode(axi_adxl* ptr, enum spi_enum spi_mode)
0x31 | DATA_FORMAT[6]     | SPI          | axi_adxl_has_spi_3_wire(axi_adxl* ptr)
0x31 | DATA_FORMAT[6]     | SPI          | axi_adxl_has_spi_4_wire(axi_adxl* ptr)
0x31 | DATA_FORMAT[5]     | INT_INVERT   | axi_adxl_enable_int_invert(axi_adxl* ptr)
0x31 | DATA_FORMAT[5]     | INT_INVERT   | axi_adxl_disable_int_invert(axi_adxl* ptr)
0x31 | DATA_FORMAT[5]     | INT_INVERT   | axi_adxl_has_int_invert(axi_adxl* ptr)
0x31 | DATA_FORMAT[3]     | FULL_RES     | axi_adxl_set_range(axi_adxl* ptr, enum range_enum range)
0x31 | DATA_FORMAT[3]     | FULL_RES     | axi_adxl_get_range(axi_adxl* ptr, enum range_enum* range)
0x31 | DATA_FORMAT[3]     | FULL_RES     | axi_adxl_is_range(axi_adxl* ptr, enum range_enum range)
0x31 | DATA_FORMAT[2]     | JUSTIFY      | axi_adxl_set_justify_msb(axi_adxl* ptr)
0x31 | DATA_FORMAT[2]     | JUSTIFY      | axi_adxl_set_justify_lsb(axi_adxl* ptr)
0x31 | DATA_FORMAT[2]     | JUSTIFY      | axi_adxl_has_justify_msb(axi_adxl* ptr)
0x31 | DATA_FORMAT[2]     | JUSTIFY      | axi_adxl_has_justify_lsb(axi_adxl* ptr)
0x31 | DATA_FORMAT[1:0]   | RANGE        | axi_adxl_set_range(axi_adxl* ptr, enum range_enum range)
0x31 | DATA_FORMAT[1:0]   | RANGE        | axi_adxl_get_range(axi_adxl* ptr, enum range_enum* range)
0x31 | DATA_FORMAT[1:0]   | RANGE        | axi_adxl_is_range(axi_adxl* ptr, enum range_enum range)
0x32 | DATAX0[7:0]        | DATAX0       | axi_adxl_get_datax(ptr)
0x33 | DATAX1[7:0]        | DATAX1       | axi_adxl_get_datax(ptr)
0x34 | DATAY0[7:0]        | DATAY0       | axi_adxl_get_datay(ptr)
0x35 | DATAY1[7:0]        | DATAY1       | axi_adxl_get_datay(ptr)
0x36 | DATAZ0[7:0]        | DATAZ0       | axi_adxl_get_dataz(ptr)
0x37 | DATAZ1[7:0]        | DATAZ1       | axi_adxl_get_dataz(ptr)
0x38 | FIFO_CTL[7:6]      | FIFO_MODE    | axi_adxl_set_fifo_mode(axi_adxl* ptr, enum fifo_mode_enum fifo_mode)
0x38 | FIFO_CTL[7:6]      | FIFO_MODE    | axi_adxl_is_fifo_mode(axi_adxl* ptr, enum fifo_mode_enum fifo_mode)
0x38 | FIFO_CTL[5]        | TRIGGER      | axi_adxl_enable_trigger(axi_adxl *ptr)
0x38 | FIFO_CTL[5]        | TRIGGER      | axi_adxl_disable_trigger(axi_adxl *ptr)
0x38 | FIFO_CTL[5]        | TRIGGER      | axi_adxl_has_trigger(axi_adxl *ptr)
0x38 | FIFO_CTL[4:0]      | SAMPLES      | axi_adxl_set_samples(axi_adxl *ptr, uint8_t samples)
0x38 | FIFO_CTL[4:0]      | SAMPLES      | axi_adxl_get_samples(axi_adxl *ptr, uint8_t *samples)
0x39 | FIFO_STATUS[5:0]   | ENTRIES      | axi_adxl_get_fifo_sts_entries(axi_adxl *ptr, uint8_t *entries)
0x39 | FIFO_STATUS[7]     | TRIGGER      | axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr)
