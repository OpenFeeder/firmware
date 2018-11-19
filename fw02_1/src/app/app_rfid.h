/**
 * @file app_rfid.h
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#ifndef _APP_RFID_HEADER_H
#define _APP_RFID_HEADER_H

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define MAX_PIT_TAGS_LIST_NUMBER            200
#define DEFAULT_TIMEOUT_READING_PIT_TAG     30  /* x160ms = timeout */

#define NUM_CONSECUTIVE_VALID_PIT_TAG 3

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    /* Number of PIT tags denied or associated with color A */
    int16_t num_pit_tag_denied_or_color_A;
    /* Number of PIT tags accepted or associated with color B */
    int16_t num_pit_tag_accepted_or_color_B;

    int16_t num_pit_tag_per_group[4];    
    
    /* List of PIT tags */
    char pit_tags_list[MAX_PIT_TAGS_LIST_NUMBER][11];
    
    char previous_pit_tags_str[11];
    struct tm previous_arrived_time;
    
    uint8_t reward_probability[MAX_PIT_TAGS_LIST_NUMBER];

    uint16_t num_pit_tag_stored;
    
    bool is_pit_tag_denied_or_color_A[MAX_PIT_TAGS_LIST_NUMBER];

    bool pit_tag_found_in_list;

    uint16_t pit_tag_index_in_list;

    uint8_t number_of_valid_pit_tag;
    
    uint8_t timeout_unique_visit;

} APP_DATA_PIT_TAG;

typedef enum
{
    /* In this state, the application opens the driver and then go in IDLE state after completion */
    DISPLAY_RFID_INIT,

    RFID_IDLE,

    RFID_DETECT_COMPLET_DATASTREAM,

    RFID_DETECT_FALSE_DATASTREAM,

    /* Application error state */
    DISPLAY_RFID_ERROR

} RFID_STATES;


// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern volatile bool g_new_value_of_em4095_rdyclk_measurement; // new value of a frequency measurement RDY/CLK of the EM4095 signal available
extern volatile uint16_t counter_positive_edge_rdyclk;
extern volatile uint8_t g_timeout_reading_pit_tag;


//******************************************************************************
//******************************************************************************
// Function prototypes
//******************************************************************************
//******************************************************************************

void APP_Rfid_Init( void );
bool APP_Rfid_Task( void );
bool isItANewPitTag(void);
void findPitTagInList( void );
bool isPitTagDenied( void );
void clearPitTagBuffers( void );
void clearPitTagSringBuffers( void );
bool measureRfidFreq( void );
void displayRfidFreq( void );

#endif /* _APP_RFID_HEADER_H */


/*******************************************************************************
 End of File
 */
