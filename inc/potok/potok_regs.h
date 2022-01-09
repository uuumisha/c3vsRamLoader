//-------Внутренние регистры ПОТОК-----------------
#define A24base 		                    0x12001
#define AM_Mask			                    0x12003
#define IRQPr			                      0x12005
#define Version			                    0x12007
//-------------------------------------------------

//-------------------------------------------------

//---------------- Регистры модуля (доступ в режиме A24D16)---------------------------------------------------------------
#define CONTROL                         0x00 //Регистр управления АМК З/Ч
#define POINTER_TABLE_ADDRESS           0x02 //Адрес расположения таблицы указателей ОУ З/Ч
#define BASIC_STATUS                    0x04 //Регистр базового ответного слова ОУ З/Ч
#define INTERRUPT_MASK                  0x07 //Маска запрета запросов прерывания З/Ч
#define INTERRUPT_VECTOR                0x06 //Вектор запроса прерывания Ч
#define INTERRUPT_REQUEST               0x06 //Регистр имитации запросов прерывания З
//#define INTERRUPT_VECTOR              0x09 //Вектор запроса прерывания З/Ч
#define AUXILIARY_VECTOR                0x08 //Доп. Вектор запроса прерывания Ч
#define CONFIGURATION_2                 0x08 //Регистр конфигурации N2 З
#define RTC_HIGH_WORD                   0x0A //Старшее слово RTC Ч
#define RTC_LOW_WORD                    0x0C //Младшее слово RTC Ч
#define RTC_CONTROL                     0x0E //Регистр управления RTC З/Ч
#define READ_FIFO                       0x10 //Чтение FIFO контроллера прерываний Ч
#define RESET_FIFO                      0x10 //Сброс FIFO контроллера прерываний З
#define CONFIGURATION_1                 0x12 //Регистр конфигурации N1 З/Ч
#define MT_CONTROL                      0x14 //Регистр управления монитором АМК З/Ч
#define LAST_COMMAND                    0x16 //Последнее командное слово ОУ Ч
#define LAST_STATUS                     0x18 //Последнее ответное слово ОУ Ч
#define FRAME_A_POINTER                 0x1A //Адрес списка сообщений FRAME_A З/Ч
#define FRAME_A_LENGTH                  0x1C //Длина списка сообщений FRAME_A З/Ч
#define RESET_TERMINAL                  0x1E //Программный сброс АМК З
#define FRAME_B_POINTER                 0x20 //Адрес списка сообщений FRAME_B З/Ч
#define FRAME_B_LENGTH                  0x22 //Длина списка сообщений FRAME_B З/Ч
#define ENCODER_STATUS                  0x24 //Состояние кодера мультиплексного канала Ч
#define CONDITION                       0x26 //Регистр Состояния АМК Ч
#define BCU_FRAME_GAP                   0x28 //Пауза между списками сообщений КШ З/Ч
#define CONFIGURATION_3                 0x2A //Регистр конфигурации N3 З/Ч
#define READ_MT_FIFO_WORD               0x2C //Чтение МТ_FIFO Ч
#define ENCODER_DATA                    0x2E //Данные кодера мультиплексного канала З/Ч
#define ENCODER_DATA_TX_REQUEST         0x30 //Запрос на выдачу кодером слова данных,(с синхроимпульсом слова данных) З
#define ENCODER_COMAND_TX_REQUEST       0x32//Запрос на выдачу кодером командного слова,(с синхроимпульсом команды) З
#define MT_FIFO_LW                      0x34 //Младшие 16 бит слова MT_FIFO Ч
#define MT_FIFO_HW                      0x36 //Старшие 16 бит слова MT_FIFO Ч
#define MT_STATUS                       0x38 //Регистр состояния МТ
#define RT_MESSAGE_ADDR                 0x3FFE //13 бит - адрес таблицы данных, связанной с указателем.
//------------------------------------------------------------------------------------------------------------------------

//---------------- Вспомогательные определения  --------------------------------------------------------------------------
#define NA_ADDR0                        0x0000
#define NA_ADDR1                        0x8000

#define POTOK_MIC_SIZE                  0x8000

#define START_BASE_ADDR                 0x0080
#define BASE_SIZE                       0x0066

#define BASE_ADDR_KK                 0x0400
