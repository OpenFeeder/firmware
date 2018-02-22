/**
 * @file app_data_logger.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 08/09/2016
 */

#ifndef _APP_DATA_LOGGER_HEADER_H
#define _APP_DATA_LOGGER_HEADER_H


/* Nb de caracteres max sur une ligne du fichier LOG. */
#define MAX_CHAR_PER_LINE 70

/* Quantite de donnees a stocker dans le buffer avant ecriture dans le fichier LOG
 * --> donc nombre de passages d'oiseau
 */
#define MAX_NUM_DATA_TO_STORE 20

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

typedef struct
{
    /* Log file name - 8.3 convention - Upper case only */
    char filename[13];
    /* Data separator in log file */
    char separator[2];

    /* Character buffer to store data before export to the log file */
    char buffer[MAX_CHAR_PER_LINE * MAX_NUM_DATA_TO_STORE];
    /* Number of written characters in the buffer */
    unsigned int nCharBuffer;
    /* Number of line stored in the buffer */
    uint8_t numDataStored;

    /* Bird Data */
    struct tm bird_arrived_time;
    struct tm bird_quit_time;
    char bird_pit_tag_str[11];
    bool bird_pir_sensor_status;
    bool is_pit_tag_denied;
    bool is_reward_taken;

    /* Attractive LEDs color*/
    uint8_t attractive_leds_current_color_index;

    uint8_t door_status_when_bird_arrived;

    int16_t battery_level[24][2];
    uint8_t numBatteryLevelStored;

    int16_t rfid_freq[96][3];
    uint8_t numRfidFreqStored;
    
    bool data_flush_before_error;
    
    bool log_birds;
    bool log_udid;
    bool log_events;
    bool log_errors;
    bool log_battery;
    bool log_rfid;

} APP_DATA_LOG;

bool dataLog( bool );
bool setLogFileName( void );
void GetTimestamp( FILEIO_TIMESTAMP * );

void clearLogBuffer( void );
void clearRfidFreqBuffer( void );
void clearBatteryBuffer( void );

FILEIO_RESULT logBatteryLevel( void );
FILEIO_RESULT logRfidFreq( void );
FILEIO_RESULT logUDID(void);

#endif /* _APP_DATA_LOGGER_HEADER_H */


/*******************************************************************************
 End of File
 */
