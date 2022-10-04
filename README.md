# adxl345

Далее по тексту необходимо придерживаться следующих правил, что:
- Устройство(Device) - ADXL345, датчик-акселерометр
- Компонент(Component) - блок, реализованный средствами аппаратуры (FPGA) для выполнения операций с устройством по обмену данными и обработкой некоторых событий.
- Процессор - устройство, реализующее взаимодействие с устройством посредством софта, также необходим для организации работы системы прерываний.

Компонент, предназначенный для организации взаимодействия с акселерометром ADXL345.
Содержит в своем составе два регистровых пространства, одно для конфигурации компонента, второе отображает состояние регистров ADXL345

![image](https://user-images.githubusercontent.com/45385195/190917937-3cbdae9c-6f92-4572-ba88-66952a13a197.png)

Фактически, является прослойкой с некоторым внутренним функционалом между устройством и процессором. 

Способ организации обычного подключения:

![image](https://user-images.githubusercontent.com/45385195/190915665-3b935188-340b-48e3-8684-f7d8bad636c1.png)

Организация подключения компонента: 

![image](https://user-images.githubusercontent.com/45385195/190915854-4f238de4-a355-454c-9040-9e1d4095c064.png)

Работу компонента следует рассматривать в двух режимах:
1) Работа в pooling-режиме;
2) Работа в режиме прерываний. 

Необходимость подобного компонента обуславливается тем, что при его отсутствии, при обычной организации, контроллер I2C процессора соединяется с компонентом напрямую (включая контроллер прерываний). Соединение процессора и ADXL345 напрямую негативно влияет на производительность первого, так как при выполнении любой операции запроса данных от устройства необходимо ожидание этих самих данных при работе в pooling-режиме, при котором исполнение кода процессором приостанавливается до тех пор, пока не будут считаны данные от устройства. 

Данный компонент решает эту ситуацию таким образом, что актуальные данные всегда хранятся во внутренней памяти компонента, и процессор может обращаться к этой памяти по высокоскоростному (относительно скорости IIC-интерфейса) интерфейсу для получения данных через интерфейс AXI-Lite. 

![image](https://user-images.githubusercontent.com/45385195/190867208-7fad59d3-53f9-447b-97e2-0401d6005dc0.png)

При работе с прерываниями, существует необходимость запроса данных в процессе обработки прерываний. Это действие необходимо для того, чтобы опустить сигнал прерывания от устройства в ноль. До тех пор, пока не будет произведено чтение внутренних регистров устройства, прерывание будет активным. 

![image](https://user-images.githubusercontent.com/45385195/190867266-62cf96a7-1c27-48f2-9168-67e1be40496c.png)

1) **Для pooling mode:**
Использование данного компонента позволяет сократить время ожидания данных, так как компонент выполняет функцию запроса данных по интервальному таймеру. Такая функция обеспечивает то, что при запросе актуальных данных от устройства со стороны процессора, программа фактически запрашивает данные из регистрового пространства, которое находится внутри компонента. Подробнее - на рисунке ниже

![image](https://user-images.githubusercontent.com/45385195/190867232-effed265-bf6a-4968-86f7-fac077a36eb1.png)

2) **Для режима прерываний:**
Данный компонент имеет возможность обработки прерывания от устройства. Обработка прерывания включает в себя последовательность действий, в которую входят:
- Ожидание сигнала прерывания от устройства
- Установка указателя адреса 0x30 для чтения данных от устройства
- Выполнение операции запроса данных для чтения данных от устройства
- Обработка некоторых полей читаемых данных
Если после обработки сигнал прерывания от устройства ADXL345 держится в единице, действия повторяются. 
Если после обработки прерывания сигнал прерывания падает в ноль, то сигнал прерывания передается от компонента на процессор, чтобы он вычитал актуальные данные. При этом, если был активен режим чтения данных от ADXL345 по таймеру, обновление данных по таймеру приостанавливается до тех пор, пока программа не обработает прерывание. 

![image](https://user-images.githubusercontent.com/45385195/190867549-676dbcd9-a232-4720-8c89-7b67ffbfbece.png)

## Организация аппаратной части

Функционал компонента содержит: 
1) Механизм обработки прерывания
2) Механизм запроса данных от ADXL345 по внутреннему таймеру, задаваемому пользователем
3) Механизм калибровки устройства (инициируется пользователем, и не требует его участия в вычислениях)
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


### Временные диаграммы

1) Работа опроса по интервальному таймеру:
Если бит INTERVAL_REQUESTION равен единице, то запустится таймер, который будет считать от REQUESTION_INTERVAL до 0. При достижении нулевого значения, вызывается генерация события о необходимости запроса данных от устройства

![image](https://user-images.githubusercontent.com/45385195/190918060-64688dcf-29a8-446e-84c1-0f68f6be550d.png)

2) Работа процесса калибровки совместно с опросом по интервальному таймеру:
Калибровка имеет приоритет выше чем интервальный опрос. При выполнении калибровки сообщения о событии интервального таймера игнорируются. 
Процесс калибровки заключается в периодическом отправлении запросов на чтение данных с регистров DATA*. 
Интервал запроса вычисляется внутри компонента, на него оказывают влияние следующие параметры:
- поле CLK_PERIOD в адресном пространстве cfg
- I2C CLOCK PERIOD на основе настройки
- поле BW_RATE устройства для определения текущего режима из адресного пространства dev
Данный интервал нельзя изменить со стороны пользователя. Значение данного интервала заносится в регистр адресного пространства CFG со смещением 0x20 и 0x24
Таким образом исключается ситуация, при которой устройство крайне редко обновляет данные, при этом калибровка данных происходит основываясь на высокой скорости. 
Количество итераций для калибровки берется на основе поля COUNT_LIMIT, которое имеется в адресном пространстве CFG. На основе этого поля вычисляется количество итераций по формуле `ITERATION_COUNT = 2^COUNT_LIMIT`. Количество итераций обозначает количество запросов данных на устройство. 
Принцип работы процесса калибровки:
- Пользователь запускает процесс калибровки, устанавливая бит START(reg 0x08, bit 8) в единицу в конфигурационном регистровом пространстве.
- Компонент обнуляет текущие значения OFSX, OFSY, OFSZ у устройства, а также внутренние регистры суммы, средних значений и т.д.
- Компонент запускает оптимальный таймер запроса данных и ждет сигнал от таймера
- При получении сигнала, компонент отправляет запрос на чтение регистров 0x32-0x37(DATA** регистры)
- Компонент получает текущие значения DATA, и суммирует их с аккумулятором. Процесс запроса данных повторяется до тех пор, пока не будет достигнуто значение ITERATION_COUNT
- После того, как количество запросов данных от механизма калибровки = ITERATION_COUNT, компонент вычисляет среднее арифметическое значение на основе количества итераций, а также вычисляет значения OFSX OFSY OFSZ согласно приему, описанному в даташите на ADXL345
- Компонент отправляет команду записи на устройство и обновленные данные OFSX, OFSY, OFSZ на устройство, при этом выставляя флаг COMPLETE (reg 0x08, bit16)

![image](https://user-images.githubusercontent.com/45385195/190918250-8fafc9d9-5f05-4911-8a46-15826689c203.png)

3) Работа системы прерываний совместно с интервальным опросом

![изображение](https://user-images.githubusercontent.com/45385195/193796526-d7acf7fa-0a5f-4a45-979d-b3933e9355ca.png)

Если у компонента активирована функция обработки прерывания, а также интервальный опрос, то процесс работы будет выстраиваться следующим образом:
- Компонент обрабатывает события по INTERVAL_REQUESTION
- При возникновении прерывания компонент переходит в обработку прерывания, выполняя необходимые действия
- Если сигнал прерывания от устройства падает в ноль, то компонент заканчивает обработку прерываний и поднимает флаг прерывания для процессора. 
При активных прерываниях и отсутствии обработки прерываний от процессора(сброс флага через INTR_ACK) компонент НЕ ВЫПОЛНЯЕТ новых запросов данных до тех пор, пока процессор не считает прерывание. 
- Процессор должен считать данные по прерыванию и отправить INTR_ACK компоненту для того чтобы снять активный сигнал прерывания.
- Если старое прерывание никто не считал, при этом возникает новое событие прерывания, то компонент перейдет к обработке нового прерывания

4) Работа компонента в ситуации, когда много прерываний остаются без обработки со стороны процессора

![изображение](https://user-images.githubusercontent.com/45385195/193798040-ceb35c12-f016-4cf5-b154-e5b04e99869a.png)

Если процессор не отвечает на прерывание, и возникает новое прерывание со стороны устройства, компонент обрабатывает эти прерывания, но ничего не делает с сигналом ADXL_IRQ, который находится в единице до тех пор, пока процессор не обработает прерывание.

5) Работа калибрации и прерываний

![изображение](https://user-images.githubusercontent.com/45385195/193798579-48219d8c-c8c5-48b3-a950-e45c83aa2d70.png)

Прерывания имеют высший приоритет. Механизм калибровки приостанавливается на время обработки прерываний. При отсутствии считывания прерывания со стороны процессора, калибровка не выполняется. Данные, получаемые в процессе прерывания, не воспринимаются механизмом калибровки(так как калибровка их не запрашивала). 

6) Работа интервального таймера и прерываний

Данный механизм работы нежелателен, так как два действия противоречат друг другу. Суть в том, что если интервал запроса будет частым, то прерывания возникать НЕ БУДУТ, так как данные с устройства будут читаться достаточно быстро и поднимать флаг прерывания со стороны устройства не имеет смысла - так как данные читаются. 
Если интервалы запросов будут большими, то прерывания будут возникать, не всегда и не постоянно, так как часть данных от устройства читается механизмом интервального опроса. 

## Организация софтовой части 

Взаимодействие реализуется посредством небольшого API, которое содержит в своем составе все функции и методы для работы со всеми полями.

![изображение](https://user-images.githubusercontent.com/45385195/193805135-a9854c57-88fc-47d5-92f3-004c170f7a52.png)

Организация софта включает в себя два отдельных блока: 
- работа с регистрами компонента(config, cfg) 
- работа с регистрами устроства (device, dev)

### Регистровое пространство configuration

Предназначено для управления компонентом и содержит набор регистров и полей с различными правами доступа к этим полям/регистрам. 

В таблице представлен список регистров, полей, маска нахождения поля в регистре и набор функций для организации взаимодействия с полями и регистрами

ADDR | REG[BIT] | FIELD NAME | ACCESS | MASK | Functions
-----|----------|------------|--------|------|----------                
0x00 | ctl_reg[31:24] | VERSION_MAJOR | RO | 0xFF000000 | axi_adxl_get_version_major(axi_adxl* ptr, uint8_t* major)
0x00 | ctl_reg[23:16] | VERSION_MINOR | RO | 0x00FF0000 | axi_adxl_get_version_minor(axi_adxl* ptr, uint8_t* minor)
0x00 | ctl_reg[15] | LINK_ON | RO | 0x00008000 | axi_adxl_has_link(axi_adxl* ptr)
0x00 | ctl_reg[14:8] | I2C_ADDRESS | R/W | 0x00007F00 | axi_adxl_get_iic_address(axi_adxl *ptr, uint8_t *iic_address)
0x00 | ctl_reg[14:8] | I2C_ADDRESS | R/W | 0x00007F00 | axi_adxl_set_iic_address(axi_adxl *ptr, uint8_t iic_address)
0x00 | ctl_reg[7] | ON_WORK | RO | 0x00000080 | axi_adxl_has_work(axi_adxl* ptr)
0x00 | ctl_reg[6] | SINGLE_REQUEST_COMPLETE | RO | 0x00000040 | axi_adxl_has_single_request_completed(axi_adxl* ptr)
0x00 | ctl_reg[5] | RESERVED | | |            
0x00 | ctl_reg[4] | INTR_ACK | WC | 0x00000010 | axi_adxl_irq_ack(axi_adxl *ptr)
0x00 | ctl_reg[3] | SINGLE_REQUEST | R/W | 0x00000008 | axi_adxl_enable_single_request(axi_adxl *ptr)
0x00 | ctl_reg[2] | IRQ_ALLOW | R/W | 0x00000004 | axi_adxl_has_irq_allow(axi_adxl* ptr)
0x00 | ctl_reg[2] | IRQ_ALLOW | R/W | 0x00000004 | axi_adxl_switch_irq_allow(axi_adxl *ptr)
0x00 | ctl_reg[1] | INTERVAL_REQUESTION | R/W | 0x00000002 | axi_adxl_enable_interval_requestion(axi_adxl *ptr, uint32_t requestion_interval)
0x00 | ctl_reg[1] | INTERVAL_REQUESTION | R/W | 0x00000002 | axi_adxl_disable_interval_requestion(axi_adxl *ptr)
0x00 | ctl_reg[1] | INTERVAL_REQUESTION | R/W | 0x00000002 | axi_adxl_has_interval_requestion(axi_adxl *ptr)
0x00 | ctl_reg[0] | RESET_LOGIC | R/W | 0x00000001 | axi_adxl_has_reset(axi_adxl *ptr)
0x00 | ctl_reg[0] | RESET_LOGIC | R/W | 0x00000001 | axi_adxl_reset(axi_adxl *ptr)
0x04 | request_interval_reg[31:0] | REQUESTION INTERVAL | R/W | 0xFFFFFFFF | axi_adxl_set_requestion_interval(axi_adxl* ptr, uint32_t requestion_interval)
0x04 | request_interval_reg[31:0] | REQUESTION INTERVAL | R/W | 0xFFFFFFFF | axi_adxl_get_requestion_interval(axi_adxl* ptr, uint32_t* requestion_interval)
0x08 | calibration_count_reg[17] | IN_PROGRESS | RO | 0x00020000 | axi_adxl_has_calibration_in_progress(axi_adxl *ptr)
0x08 | calibration_count_reg[16] | COMPLETE | RO | 0x00010000 | axi_adxl_has_calibration_complete(axi_adxl* ptr)
0x08 | calibration_count_reg[8] | START | R/W | 0x00000100 | axi_adxl_calibration_start(axi_adxl *ptr)
0x08 | calibration_count_reg[4:0] | PWR_COUNT_LIMIT | R/W | 0x0000001F | axi_adxl_set_calibration_pwr_count_limit(axi_adxl* ptr, uint8_t pwr_count_limit)
0x08 | calibration_count_reg[4:0] | PWR_COUNT_LIMIT | R/W | 0x0000001F | axi_adxl_get_calibration_pwr_count_limit(axi_adxl* ptr, uint8_t* pwr_count_limit)
0x0C | read_valid_count_reg[31:0] | READ_VALID_COUNT | RO | 0xFFFFFFFF | axi_adxl_get_read_valid_count(axi_adxl* ptr, uint32_t* read_valid_count)
0x10 | write_valid_count_reg[31:0] | WRITE_VALID_COUNT | RO | 0xFFFFFFFF | axi_adxl_get_write_valid_count(axi_adxl* ptr, uint32_t* write_valid_count)
0x14 | read_transactions_lsb_reg[31:0] | READ_TRANSACTIONS_LSB | RO | 0xFFFFFFFF | axi_adxl_get_read_transactions(axi_adxl* ptr, uint64_t* read_transactions)
0x18 | read_transactions_msb_reg[31:0] | READ_TRANSACTIONS_MSB | RO | 0xFFFFFFFF | axi_adxl_get_read_transactions(axi_adxl* ptr, uint64_t* read_transactions)
0x1C | clk_period_reg[31:0] | CLK_PERIOD | RO | 0xFFFFFFFF | axi_adxl_get_clk_period(axi_adxl* ptr, uint32_t* clk_period)
0x20 | opt_request_interval_lsb_reg[31:0] | OPT_REQUEST_INTERVAL_LSB | RO | 0xFFFFFFFF | axi_adxl_get_opt_request_interval(axi_adxl* ptr, uint64_t* opt_request_interval)
0x24 | opt_request_interval_msb_reg[15:0] | OPT_REQUEST_INTERVAL_MSB | RO | 0x0000FFFF | axi_adxl_get_opt_request_interval(axi_adxl* ptr, uint64_t* opt_request_interval)
0x28 | data_width_reg[31:0] | DATA_WIDTH | RO | 0xFFFFFFFF | axi_adxl_get_data_width(axi_adxl* ptr, uint32_t* data_width)
0x2C | calibration_time_lsb_reg[31:0] | CALIBRATION_TIME_LSB | RO | 0xFFFFFFFF | axi_adxl_get_calibration_time(axi_adxl* ptr, uint64_t* calibration_time)
0x30 | calibration_time_msb_reg[31:0] | CALIBRATION_TIME_MSB | RO | 0xFFFFFFFF | axi_adxl_get_calibration_time(axi_adxl* ptr, uint64_t* calibration_time)
0x34 | write_transactions_lsb_reg[31:0] | WRITE_TRANSACTIONS_LSB | RO | 0xFFFFFFFF | axi_adxl_get_write_transactions(axi_adxl* ptr, uint64_t* write_transactions)
0x38 | write_transactions_msb_reg[31:0] | WRITE_TRANSACTIONS_MSB | RO | 0xFFFFFFFF | axi_adxl_get_write_transactions(axi_adxl* ptr, uint64_t* write_transactions)
0x3C | single_req_params_reg[5:0] | START_ADDRESS | R/W | 0x0000003F | axi_adxl_set_start_address(axi_adxl* ptr, uint8_t start_address)
0x3C | single_req_params_reg[5:0] | START_ADDRESS | R/W | 0x0000003F | axi_adxl_get_start_address(axi_adxl* ptr, uint8_t *start_address)
0x3C | single_req_params_reg[13:8] | SIZE | R/W | 0x00003F00 | axi_adxl_set_size(axi_adxl* ptr, uint8_t size)
0x3C | single_req_params_reg[13:8] | SIZE | R/W | 0x00003F00 | axi_adxl_get_size(axi_adxl* ptr, uint8_t* size)

### Регистровое пространство Device

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




