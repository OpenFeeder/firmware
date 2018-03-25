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
    uint16_t numPitTagDeniedOrColorA;
    /* Number of PIT tags accepted or associated with color B */
    uint16_t numPitTagAcceptedOrColorB;

    uint16_t numPitTagGroup[4];    
    
    /* List of PIT tags denied */
    char pit_tags_list[MAX_PIT_TAGS_LIST_NUMBER][11];
    
    uint8_t reward_probability[MAX_PIT_TAGS_LIST_NUMBER];

    uint16_t numPitTagStored;
    
    bool isPitTagdeniedOrColorA[MAX_PIT_TAGS_LIST_NUMBER];

    bool didPitTagMatched;

    uint16_t pitTagIndexInList;

    uint8_t number_of_valid_pit_tag;

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
void findPitTagInList( void );
bool isPitTagDenied( void );
void clearPitTagBuffers( void );
bool measureRfidFreq( void );
void displayRfidFreq( void );

#endif /* _APP_RFID_HEADER_H */


/*******************************************************************************
 End of File
 */
