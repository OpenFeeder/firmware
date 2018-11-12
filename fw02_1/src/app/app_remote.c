/**
 * @file app_remote.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date
 */

#include "app.h"
//#include "digits.h"

MENU menu;

char currentMenuValue[6];

//char batteryState[] = "|||| ";
//char pileState[] = "||ii ";
char numberDays[] = "d012 ";
//char error[] = "noEr.";

//char pirDetect[] = "truE ";
char rfidDetect[] = "PtAG ";
//char doorStatus[] = "OPEn ";
char irDetect[] = "truE ";

//char siteID[] = "AAXX ";

int currentDigitModificationDirection;
int currentDigitPosition = 0;


void APP_remoteControlInitialize( void )
{
    appDataRc.state = APP_STATE_RC_INIT;
    appDataRc.previous_state = APP_STATE_RC_BATTERY_LEVEL_INIT;
}


void APP_remoteControlTask( void )
{

    uint16_t battery_level_step;

    switch ( appDataRc.state )
    {
        case APP_STATE_RC_INIT:
            if ( appDataRc.state != appDataRc.previous_state )
            {
                appDataRc.previous_state = appDataRc.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                printf( "\tAPP_STATE_RC_INIT\n" );
#endif
            }

            /* Display all segments */
            print7Segments( "8888:" );
            setDelayMs( START_MENU_DELAY );
            while ( false == isDelayMsEnding( ) );
            displayDigitFixe( );
            appDataRc.state = APP_STATE_RC_BATTERY_LEVEL_INIT;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_RC_BATTERY_LEVEL_INIT:
            if ( appDataRc.state != appDataRc.previous_state )
            {
                appDataRc.previous_state = appDataRc.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                printf( "\tAPP_STATE_RC_BATTERY_LEVEL_INIT\n" );
#endif
            }

            /* Display battery level */
            print7Segments( "bAtt" );
            setDelayMs( START_MENU_DELAY );
            while ( false == isDelayMsEnding( ) );
            battery_level_step = 4 * ( appData.battery_level - LOW_BATTERY_LEVEL ) / ( HIGH_BATTERY_LEVEL - LOW_BATTERY_LEVEL );
//            if ( 0 == battery_level_step )
//                print7Segments( "|    " );
//            if ( 1 == battery_level_step )
//                print7Segments( "||   " );
//            if ( 2 == battery_level_step )
//                print7Segments( "|||  " );
//            if ( 3 <= battery_level_step )
//                print7Segments( "|||| " );
            if ( 0 == battery_level_step )
                print7Segments( "0    " );
            if ( 1 == battery_level_step )
                print7Segments( "00   " );
            if ( 2 == battery_level_step )
                print7Segments( "000  " );
            if ( 3 <= battery_level_step )
                print7Segments( "0000 " );
            
            setDelayMs( START_MENU_DELAY );
            while ( false == isDelayMsEnding( ) );
            displayDigitFixe( );
            appDataRc.state = APP_STATE_RC_WAIT_ENTER_MENU;
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_RC_WAIT_ENTER_MENU:
            if ( appDataRc.state != appDataRc.previous_state )
            {
                appDataRc.previous_state = appDataRc.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                printf( "\tAPP_STATE_RC_WAIT_ENTER_MENU\n" );
#endif
            }

            switch ( appData.buttonPressed )
            {
                case BUTTON_DOWN:
                case BUTTON_UP:
                case BUTTON_RIGHT:
                case BUTTON_LEFT:
                    setCurrentMenu( N1_MENU );
                    print7Segments( menu.name );
                    displayDigitFixe( );
                    appDataRc.state = APP_STATE_RC_IDLE;
                    appData.buttonPressed = BUTTON_READ;
                    break;

                default:
                    break;
            }
            break;
            /* -------------------------------------------------------------- */

        case APP_STATE_RC_IDLE:
            if ( appDataRc.state != appDataRc.previous_state )
            {
                appDataRc.previous_state = appDataRc.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                printf( "\tAPP_STATE_RC_IDLE\n" );
#endif
                displayDigitFixe( );
            }

            switch ( appData.buttonPressed )
            {
                case BUTTON_DOWN:
                    if ( menu.nextMenu != NO )
                    {
                        setCurrentMenu( menu.nextMenu );
                        printCurrentMenu( );
                    }

                    displayDigitFixe( );
                    appDataRc.state = APP_STATE_RC_IDLE;
                    appData.buttonPressed = BUTTON_READ;
                    break;

                case BUTTON_UP:
                    if ( menu.previousMenu != NO )
                    {
                        setCurrentMenu( menu.previousMenu );
                        printCurrentMenu( );
                    }

                    displayDigitFixe( );
                    appDataRc.state = APP_STATE_RC_IDLE;
                    appData.buttonPressed = BUTTON_READ;
                    break;

                case BUTTON_RIGHT:
                    if ( menu.firstChild != NO )
                    {
                        setCurrentMenu( menu.firstChild );
                        printCurrentMenu( );
                        displayDigitFixe( );
                        appDataRc.state = APP_STATE_RC_IDLE;
                        appData.buttonPressed = BUTTON_READ;
                        break;
                    }

                    if ( menu.mode == WRITE || menu.mode == READ_WRITE )
                    {

                        currentDigitPosition = 0;

                        if ( menu.valueType == YEAR )
                            currentDigitPosition = 2; // Do not modify first and second digit for year menu
                        if ( menu.valueType == RGB )
                            currentDigitPosition = 1; // Do not modify first digit for RGB menus
                        //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                        //                        printf("Current digit position: %d (IDLE)\n", currentDigitPosition);
                        //#endif    
                        displayDigitBlink( currentDigitPosition );
                        appDataRc.state = APP_STATE_RC_MANIPULATE_DIGIT;
                        appData.buttonPressed = BUTTON_READ;
                        break;
                    }

                    displayDigitFixe( );
                    appDataRc.state = APP_STATE_RC_IDLE;
                    appData.buttonPressed = BUTTON_READ;
                    break;

                case BUTTON_LEFT:
                    if ( menu.parentMenu != NO )
                    {
                        setCurrentMenu( menu.parentMenu );
                        printCurrentMenu( );
                    }

                    displayDigitFixe( );
                    appDataRc.state = APP_STATE_RC_IDLE;
                    appData.buttonPressed = BUTTON_READ;
                    break;

                default:
                    break;
            }
            /* -------------------------------------------------------------- */

        case APP_STATE_RC_MANIPULATE_DIGIT:
            if ( appDataRc.state != appDataRc.previous_state )
            {
                appDataRc.previous_state = appDataRc.state;
#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_CURRENT_STATE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                printf( "\tAPP_STATE_RC_MANIPULATE_DIGIT\n" );
#endif
            }

            switch ( appData.buttonPressed )
            {
                case BUTTON_UP:
                    currentDigitModificationDirection = 1;
                    modifyCurrentDigit( currentDigitPosition, currentDigitModificationDirection );
                    setCurrentMenuValue( );
                    appData.buttonPressed = BUTTON_READ;
                    break;

                case BUTTON_DOWN:
                    currentDigitModificationDirection = -1;
                    modifyCurrentDigit( currentDigitPosition, currentDigitModificationDirection );
                    setCurrentMenuValue( );
                    appData.buttonPressed = BUTTON_READ;
                    break;

                case BUTTON_RIGHT:
                    currentDigitPosition++;
                    if ( currentDigitPosition < 4 )
                    {
                        displayDigitBlink( currentDigitPosition );
                    }
                    else
                    {
                        currentDigitPosition = 0;
                        storeCurrentMenu( );
                        appDataRc.state = APP_STATE_RC_IDLE;
                    }
                    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                    //                    printf("Current digit position: %d (MANIPULATE_DIGIT)\n", currentDigitPosition);
                    //#endif 

                    appData.buttonPressed = BUTTON_READ;
                    break;

                case BUTTON_LEFT:
                    currentDigitPosition--;
                    if ( menu.valueType == YEAR && currentDigitPosition == 1 )
                    {
                        currentDigitPosition = -1; // Do not modify first and second digit for year menu
                    }

                    if ( menu.valueType == RGB && currentDigitPosition == 0 )
                    {
                        currentDigitPosition = -1; // Do not modify first digit for RGB menus
                    }

                    if ( currentDigitPosition < 0 )
                    {
                        currentDigitPosition = 0;
                        printCurrentMenu( ); // FIXME: printCurrentMenu( )
                        appDataRc.state = APP_STATE_RC_IDLE;
                    }
                    else
                    {
                        displayDigitBlink( currentDigitPosition );
                    }
                    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
                    //                    printf("Current digit position: %d (MANIPULATE_DIGIT)\n", currentDigitPosition);
                    //#endif 

                    appData.buttonPressed = BUTTON_READ;
                    break;

                default:
                    break;
            }
            break;
            /* -------------------------------------------------------------- */

        default:
            break;
    }
}


void print7Segments( const char *buf )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "print7Segments()\n" );
    //#endif 
    uint8_t i;

    for ( i = 0; i < 4; i++ )
    {
        if ( buf[i] == '|' )
            appData.digit[i] = 0b11001001;
        else
            appData.digit[i] = charASCII_Digit7Seg_CA[( uint8_t ) buf[i]];
    }

    if ( buf[4] == ':' )
    {
        appData.digit[2] = appData.digit[2] & 0b01111111;
        appData.digit[3] = appData.digit[3] & 0b01111111;
    }

    if ( buf[4] == '.' )
    {
        appData.digit[3] = appData.digit[3] & 0b01111111;
    }
}


void modifyCurrentDigit( uint8_t currentDigitPosition, uint8_t currentDigitModificationDirection )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "modifyCurrentDigit()\n" );
    //#endif 

    uint8_t asciiValue;

    strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );

    asciiValue = currentMenuValue[currentDigitPosition] - 48;
    asciiValue = asciiValue + currentDigitModificationDirection;

    if ( asciiValue < 0 )
    {
        asciiValue = 0;
    }

    switch ( menu.valueType )
    {
        case YEAR:
            if ( asciiValue > 9 )
            {
                asciiValue = 9;
            }
            break;
            /* -------------------------------------------------------------- */

        case MONTH_DAY:
            if ( ( currentDigitPosition == 0 ) && ( asciiValue >= 1 ) )
            {
                if ( ( currentMenuValue[1] - 48 ) > 2 )
                {
                    asciiValue = 0;
                }
                else
                {
                    asciiValue = 1;
                }
            }

            if ( currentDigitPosition == 1 )
            {
                if ( ( ( currentMenuValue[0] - 48 ) == 1 ) && ( asciiValue > 2 ) )
                {
                    asciiValue = 2;
                }
                else if ( asciiValue > 9 )
                {
                    asciiValue = 9;
                }
            }

            // DONE: si on modifi le Digit 3 à une valeur > à 1 ( avec Digit 2 < 3) et que l'on revient sur le Digit 2 à 3
            // alors on peut avoir des valeurs de jour > à 31 !!!
            if ( ( currentDigitPosition == 2 ) && ( asciiValue >= 3 ) )
            {
                if ( ( currentMenuValue[3] - 48 ) > 1 )
                {
                    asciiValue = 2;
                }
                else
                {
                    asciiValue = 3;
                }
            }

            if ( currentDigitPosition == 3 )
            {
                if ( ( ( currentMenuValue[2] - 48 ) == 3 ) && ( asciiValue > 1 ) )
                {
                    asciiValue = 1;
                }
                else if ( asciiValue > 9 )
                {
                    asciiValue = 9;
                }
            }
            break;
            /* -------------------------------------------------------------- */

        case HOUR_MINUTE:
            if ( ( currentDigitPosition == 0 ) && ( asciiValue >= 2 ) )
            {
                if ( ( currentMenuValue[3] - 48 ) > 3 )
                {
                    asciiValue = 1;
                }
                else
                {
                    asciiValue = 2;
                }
            }

            if ( currentDigitPosition == 1 )
            {
                if ( ( ( currentMenuValue[0] - 48 ) == 2 ) && ( asciiValue > 3 ) )
                {
                    asciiValue = 3;
                }
                else if ( asciiValue > 9 )
                {
                    asciiValue = 9;
                }
            }

            if ( ( currentDigitPosition == 2 ) && ( asciiValue > 5 ) )
            {
                asciiValue = 5;
            }

            if ( currentDigitPosition == 3 && asciiValue > 9 )
            {
                asciiValue = 9;
            }
            break;
            /* -------------------------------------------------------------- */

        case RGB:
            /* Maximum value: 2?? */
            if ( currentDigitPosition == 1 && asciiValue > 2 )
            {
                asciiValue = 2;
            }

            if ( ( currentMenuValue[1] - 48 ) == 2 )
            {
                /* Maximum value: 25? */
                if ( currentDigitPosition == 2 && asciiValue > 5 )
                    asciiValue = 5;

                /* Maximum value: 255 */
                if ( currentDigitPosition == 3 && ( currentMenuValue[2] - 48 ) == 5 && asciiValue > 5 )
                    asciiValue = 5;
            }
            break;
            /* -------------------------------------------------------------- */

        default:
            break;
    }

    currentMenuValue[currentDigitPosition] = asciiValue + 48;
    setCurrentMenuValue( );
    printCurrentMenuValue( );
}


void printCurrentMenu( void )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "printCurrentMenu()\n" );
    //#endif 
    if ( strcmp( menu.name, "     " ) == 0 )
    {
        getCurrentMenuValue( );
        printCurrentMenuValue( );
    }
    else
    {
        print7Segments( menu.name );
        if ( ( menu.mode == READ ) || ( menu.mode == READ_WRITE ) )
        {
            setDelayMs( 1000 );
            while ( false == isDelayMsEnding( ) );
            getCurrentMenuValue( );
            printCurrentMenuValue( );
        }
    }
}


void setCurrentMenuValue( void )
{
    //    #if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
    //        printf( "setCurrentMenuValue()\n" );
    //    #endif 
    switch ( menu.currentMenu )
    {
        case N4_DATE_YEAR:
        case N4_DATE_MONTH_DAY:
        case N4_TIME_HOUR_MIN:
        case N4_START_HOUR_MIN:
        case N4_STOP_HOUR_MIN:
        case N4_LEDS_R:
        case N4_LEDS_G:
        case N4_LEDS_B:
            strncpy( appDataRc.digitBuffer, currentMenuValue, 5 );
            break;

        default:
            break;
    }
}


void getCurrentMenuValue( void )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE) && defined (DISPLAY_REMOTE_CONTROL_INFO )
    //    printf( "getCurrentMenuValue()\n" );
    //#endif 

    uint16_t battery_level_step, vbat_level_step;
    char error[6];
    
    switch ( menu.currentMenu )
    {
        case N4_BATTERY_STATE:
            battery_level_step = 4 * ( appData.battery_level - LOW_BATTERY_LEVEL ) / ( HIGH_BATTERY_LEVEL - LOW_BATTERY_LEVEL );
            if ( 0 == battery_level_step )
                strncpy( currentMenuValue, "0    ", 5 );
            if ( 1 == battery_level_step )
                strncpy( currentMenuValue, "00   ", 5 );
            if ( 2 == battery_level_step )
                strncpy( currentMenuValue, "000  ", 5 );
            if ( 3 <= battery_level_step )
                strncpy( currentMenuValue, "0000 ", 5 );
            break;

        case N4_PILE_STATE:
            vbat_level_step = 4 * ( appData.vbat_level - LOW_VBAT_LEVEL ) / ( HIGH_VBAT_LEVEL - LOW_VBAT_LEVEL );
            if ( 0 == vbat_level_step )
                strncpy( currentMenuValue, "0    ", 5 );
            if ( 1 == vbat_level_step )
                strncpy( currentMenuValue, "00   ", 5 );
            if ( 2 == vbat_level_step )
                strncpy( currentMenuValue, "000  ", 5 );
            if ( 3 <= vbat_level_step )
                strncpy( currentMenuValue, "0000 ", 5 );
            break;

        case N4_NUMBER_DAYS:
            strncpy( currentMenuValue, numberDays, 5 );
            break;

        case N4_ERRORS_LIST:            
            sprintf(error, "Er%02d.", appError.number);
            strncpy( currentMenuValue, error, 5 );
            break;

        case N4_DATE_YEAR:
            sprintf( appDataRc.digitBuffer, "20%02d ", appData.current_time.tm_year );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_DATE_MONTH_DAY:
            sprintf( appDataRc.digitBuffer, "%02d%02d.", appData.current_time.tm_mon, appData.current_time.tm_mday );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_TIME_HOUR_MIN:
            sprintf( appDataRc.digitBuffer, "%02d%02d:", appData.current_time.tm_hour, appData.current_time.tm_min );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_START_HOUR_MIN:
            sprintf( appDataRc.digitBuffer, "%02d%02d:", appDataAlarmWakeup.time.tm_hour, appDataAlarmWakeup.time.tm_min );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_STOP_HOUR_MIN:
            sprintf( appDataRc.digitBuffer, "%02d%02d:", appDataAlarmSleep.time.tm_hour, appDataAlarmSleep.time.tm_min );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_PIR_DETECT:

//            if ( true == appDataLog.bird_pir_sensor_status )
            if ( true == is_bird_detected )
            {
                strncpy( currentMenuValue, "truE ", 5 );
            }
            else
            {
                strncpy( currentMenuValue, "nonE ", 5 );
            }
            //            strncpy( currentMenuValue, pirDetect, 5 );
            break;

        case N4_RFID_DETECT:
            strncpy( currentMenuValue, rfidDetect, 5 );
            break;

        case N4_DOOR_STATUS:

            if ( DOOR_OPENED == appDataDoor.reward_door_status )
            {
                strncpy( currentMenuValue, "OPEn ", 5 );
            }
            else
            {
                strncpy( currentMenuValue, "cLSE ", 5 );
            }
            break;

        case N4_IR_DETECT:
            strncpy( currentMenuValue, irDetect, 5 );
            break;

        case N4_LEDS_R:
            sprintf( appDataRc.digitBuffer, "r%03u ", appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_LEDS_G:
            sprintf( appDataRc.digitBuffer, "G%03u ", appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N4_LEDS_B:
            sprintf( appDataRc.digitBuffer, "b%03u ", appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );
            strncpy( currentMenuValue, appDataRc.digitBuffer, 5 );
            break;

        case N3_SITE_ID:
            strncpy( currentMenuValue, appData.siteid, 5 );
            //            strncpy( currentMenuValue, siteID, 5 );
            break;

        default:
            break;
    }
}


void storeCurrentMenu( void )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "storeCurrentMenu()\n" );
    //#endif 
    switch ( menu.currentMenu )
    {

        case N4_DATE_YEAR:
            appData.current_time.tm_year = ( int ) ( 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
            RTCC_TimeSet( &appData.current_time );
            break;

        case N4_DATE_MONTH_DAY:
            appData.current_time.tm_mon = ( int ) ( 10 * ( appDataRc.digitBuffer[0] - 48 ) + ( appDataRc.digitBuffer[1] - 48 ) );
            appData.current_time.tm_mday = ( int ) ( 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
            RTCC_TimeSet( &appData.current_time );
            break;

        case N4_TIME_HOUR_MIN:
            appData.current_time.tm_hour = ( int ) ( 10 * ( appDataRc.digitBuffer[0] - 48 ) + ( appDataRc.digitBuffer[1] - 48 ) );
            appData.current_time.tm_min = ( int ) ( 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
            appData.current_time.tm_sec = 0;
            RTCC_TimeSet( &appData.current_time );
            break;

        case N4_START_HOUR_MIN:
            appDataAlarmWakeup.time.tm_hour = ( int ) ( 10 * ( appDataRc.digitBuffer[0] - 48 ) + ( appDataRc.digitBuffer[1] - 48 ) );
            appDataAlarmWakeup.time.tm_min = ( int ) ( 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
            appDataAlarmWakeup.time.tm_sec = 0;
            break;

        case N4_STOP_HOUR_MIN:
            appDataAlarmSleep.time.tm_hour = ( int ) ( 10 * ( appDataRc.digitBuffer[0] - 48 ) + ( appDataRc.digitBuffer[1] - 48 ) );
            appDataAlarmSleep.time.tm_min = ( int ) ( 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
            appDataAlarmSleep.time.tm_sec = 0;
            break;

            //        case N4_PIR_DETECT:
            //            strncpy( currentMenuValue, pirDetect, 5 );
            //            break;

        case N4_RFID_DETECT:
            strncpy( currentMenuValue, rfidDetect, 5 );
            break;

        case N4_DOOR_STATUS:
            //            strncpy( currentMenuValue, doorStatus, 5 );
            break;

        case N4_IR_DETECT:
            strncpy( currentMenuValue, irDetect, 5 );
            break;

        case N4_LEDS_R:
            appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] = ( uint16_t ) ( 100 * ( appDataRc.digitBuffer[1] - 48 ) + 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
//            OC1_SingleCompareValueSet( appDataAttractiveLeds.red[appDataAttractiveLeds.current_color_index] );
            break;

        case N4_LEDS_G:
            appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] = ( uint16_t ) ( 100 * ( appDataRc.digitBuffer[1] - 48 ) + 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
//            OC2_SingleCompareValueSet( appDataAttractiveLeds.green[appDataAttractiveLeds.current_color_index] );
            break;

        case N4_LEDS_B:
            appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] = ( uint16_t ) ( 100 * ( appDataRc.digitBuffer[1] - 48 ) + 10 * ( appDataRc.digitBuffer[2] - 48 ) + ( appDataRc.digitBuffer[3] - 48 ) );
//            OC3_SingleCompareValueSet( appDataAttractiveLeds.blue[appDataAttractiveLeds.current_color_index] );
            break;

            //        case N3_SITE_ID:
            //            strncpy( currentMenuValue, siteID, 5 );
            //            break;

        default:
            break;
    }
}


void printCurrentMenuValue( void )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "printCurrentMenuValue()\n" );
    //#endif 
    print7Segments( currentMenuValue );
}


void setCurrentMenu( MENUS_LEVELS menuLevel )
{
    //#if defined (USE_UART1_SERIAL_INTERFACE)
    //    printf( "setCurrentMenu()\n" );
    //#endif 
    switch ( menuLevel )
    {
        case N1_MENU:
            menu.currentMenu = N1_MENU;
            menu.parentMenu = NO;
            menu.firstChild = N2_SYSTEM;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "MEnU ", 5 );
            menu.mode = NONE;
            menu.level = 0;
            menu.valueType = NO_TYPE;
            break;

        case N2_SYSTEM:
            menu.currentMenu = N2_SYSTEM;
            menu.parentMenu = N1_MENU;
            menu.firstChild = N3_BATTERY;
            menu.nextMenu = N2_TIME_CLOCK;
            menu.previousMenu = NO;
            strncpy( menu.name, "SySt ", 5 );
            menu.mode = NONE;
            menu.level = 1;
            menu.valueType = NO_TYPE;
            break;

        case N3_BATTERY:
            menu.currentMenu = N3_BATTERY;
            menu.parentMenu = N2_SYSTEM;
            menu.firstChild = N4_BATTERY_STATE;
            menu.nextMenu = N3_PILE;
            menu.previousMenu = NO;
            strncpy( menu.name, "bAtt ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_BATTERY_STATE:
            menu.currentMenu = N4_BATTERY_STATE;
            menu.parentMenu = N3_BATTERY;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_PILE:
            menu.currentMenu = N3_PILE;
            menu.parentMenu = N2_SYSTEM;
            menu.firstChild = N4_PILE_STATE;
            menu.nextMenu = N3_OPERATING_TIME;
            menu.previousMenu = N3_BATTERY;
            strncpy( menu.name, "PILE ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_PILE_STATE:
            menu.currentMenu = N4_PILE_STATE;
            menu.parentMenu = N3_PILE;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_OPERATING_TIME:
            menu.currentMenu = N3_OPERATING_TIME;
            menu.parentMenu = N2_SYSTEM;
            menu.firstChild = N4_NUMBER_DAYS;
            menu.nextMenu = N3_ERRORS;
            menu.previousMenu = N3_PILE;
            strncpy( menu.name, "oP-t ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_NUMBER_DAYS:
            menu.currentMenu = N4_NUMBER_DAYS;
            menu.parentMenu = N3_OPERATING_TIME;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_ERRORS:
            menu.currentMenu = N3_ERRORS;
            menu.parentMenu = N2_SYSTEM;
            menu.firstChild = N4_ERRORS_LIST;
            menu.nextMenu = NO;
            menu.previousMenu = N3_OPERATING_TIME;
            strncpy( menu.name, "Err  ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_ERRORS_LIST:
            menu.currentMenu = N4_ERRORS_LIST;
            menu.parentMenu = N3_ERRORS;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N2_TIME_CLOCK:
            menu.currentMenu = N2_TIME_CLOCK;
            menu.parentMenu = N1_MENU;
            menu.firstChild = N3_DATE;
            menu.nextMenu = N2_TEST;
            menu.previousMenu = N2_SYSTEM;
            strncpy( menu.name, "tIME ", 5 );
            menu.mode = NONE;
            menu.level = 1;
            menu.valueType = NO_TYPE;
            break;

        case N3_DATE:
            menu.currentMenu = N3_DATE;
            menu.parentMenu = N2_TIME_CLOCK;
            menu.firstChild = N4_DATE_YEAR;
            menu.nextMenu = N3_TIME;
            menu.previousMenu = NO;
            strncpy( menu.name, "dAtE ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_DATE_YEAR:
            menu.currentMenu = N4_DATE_YEAR;
            menu.parentMenu = N3_DATE;
            menu.firstChild = NO;
            menu.nextMenu = N4_DATE_MONTH_DAY;
            menu.previousMenu = NO;
            strncpy( menu.name, "yyyy ", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = YEAR;
            break;

        case N4_DATE_MONTH_DAY:
            menu.currentMenu = N4_DATE_MONTH_DAY;
            menu.parentMenu = N3_DATE;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = N4_DATE_YEAR;
            strncpy( menu.name, "MMdd.", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = MONTH_DAY;
            break;

        case N3_TIME:
            menu.currentMenu = N3_TIME;
            menu.parentMenu = N2_TIME_CLOCK;
            menu.firstChild = N4_TIME_HOUR_MIN;
            menu.nextMenu = N3_START;
            menu.previousMenu = N3_DATE;
            strncpy( menu.name, "Hour ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_TIME_HOUR_MIN:
            menu.currentMenu = N4_TIME_HOUR_MIN;
            menu.parentMenu = N3_TIME;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "HHMM:", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = HOUR_MINUTE;
            break;

        case N3_START:
            menu.currentMenu = N3_START;
            menu.parentMenu = N2_TIME_CLOCK;
            menu.firstChild = N4_START_HOUR_MIN;
            menu.nextMenu = N3_STOP;
            menu.previousMenu = N3_TIME;
            strncpy( menu.name, "WAkE ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_START_HOUR_MIN:
            menu.currentMenu = N4_START_HOUR_MIN;
            menu.parentMenu = N3_START;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "HHMM:", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = HOUR_MINUTE;
            break;

        case N3_STOP:
            menu.currentMenu = N3_STOP;
            menu.parentMenu = N2_TIME_CLOCK;
            menu.firstChild = N4_STOP_HOUR_MIN;
            menu.nextMenu = NO;
            menu.previousMenu = N3_START;
            strncpy( menu.name, "SLEP ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_STOP_HOUR_MIN:
            menu.currentMenu = N4_STOP_HOUR_MIN;
            menu.parentMenu = N3_STOP;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "HHMM:", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = HOUR_MINUTE;
            break;

        case N2_TEST:
            menu.currentMenu = N2_TEST;
            menu.parentMenu = N1_MENU;
            menu.firstChild = N3_PIR;
            menu.nextMenu = N2_SITE;
            menu.previousMenu = N2_TIME_CLOCK;
            strncpy( menu.name, "tESt ", 5 );
            menu.mode = NONE;
            menu.level = 1;
            menu.valueType = NO_TYPE;
            break;

        case N3_PIR:
            menu.currentMenu = N3_PIR;
            menu.parentMenu = N2_TEST;
            menu.firstChild = N4_PIR_DETECT;
            menu.nextMenu = N3_RFID;
            menu.previousMenu = NO;
            strncpy( menu.name, "cPir ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_PIR_DETECT:
            menu.currentMenu = N4_PIR_DETECT;
            menu.parentMenu = N3_PIR;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_RFID:
            menu.currentMenu = N3_RFID;
            menu.parentMenu = N2_TEST;
            menu.firstChild = N4_RFID_DETECT;
            menu.nextMenu = N3_DOOR;
            menu.previousMenu = N3_PIR;
            strncpy( menu.name, "rFid ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_RFID_DETECT:
            menu.currentMenu = N4_RFID_DETECT;
            menu.parentMenu = N3_RFID;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_DOOR:
            menu.currentMenu = N3_DOOR;
            menu.parentMenu = N2_TEST;
            menu.firstChild = N4_DOOR_STATUS;
            menu.nextMenu = N3_IR;
            menu.previousMenu = N3_RFID;
            strncpy( menu.name, "door ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_DOOR_STATUS:
            menu.currentMenu = N4_DOOR_STATUS;
            menu.parentMenu = N3_DOOR;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = WRITE;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_IR:
            menu.currentMenu = N3_IR;
            menu.parentMenu = N2_TEST;
            menu.firstChild = N4_IR_DETECT;
            menu.nextMenu = N3_LEDS;
            menu.previousMenu = N3_DOOR;
            strncpy( menu.name, "CbIr ", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_IR_DETECT:
            menu.currentMenu = N4_IR_DETECT;
            menu.parentMenu = N3_IR;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 3;
            menu.valueType = NO_TYPE;
            break;

        case N3_LEDS:
            menu.currentMenu = N3_LEDS;
            menu.parentMenu = N2_TEST;
            menu.firstChild = N4_LEDS_R;
            menu.nextMenu = NO;
            menu.previousMenu = N3_IR;
            strncpy( menu.name, "LEdS", 5 );
            menu.mode = NONE;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        case N4_LEDS_R:
            menu.currentMenu = N4_LEDS_R;
            menu.parentMenu = N3_LEDS;
            menu.firstChild = NO;
            menu.nextMenu = N4_LEDS_G;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = RGB;
            break;

        case N4_LEDS_G:
            menu.currentMenu = N4_LEDS_G;
            menu.parentMenu = N3_LEDS;
            menu.firstChild = NO;
            menu.nextMenu = N4_LEDS_B;
            menu.previousMenu = N4_LEDS_R;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = RGB;
            break;

        case N4_LEDS_B:
            menu.currentMenu = N4_LEDS_B;
            menu.parentMenu = N3_LEDS;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = N4_LEDS_G;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ_WRITE;
            menu.level = 3;
            menu.valueType = RGB;
            break;

        case N2_SITE:
            menu.currentMenu = N2_SITE;
            menu.parentMenu = N1_MENU;
            menu.firstChild = N3_SITE_ID;
            menu.nextMenu = NO;
            menu.previousMenu = N2_TEST;
            strncpy( menu.name, "SitE ", 5 );
            menu.mode = NONE;
            menu.level = 1;
            menu.valueType = NO_TYPE;
            break;

        case N3_SITE_ID:
            menu.currentMenu = N3_SITE_ID;
            menu.parentMenu = N2_SITE;
            menu.firstChild = NO;
            menu.nextMenu = NO;
            menu.previousMenu = NO;
            strncpy( menu.name, "     ", 5 );
            menu.mode = READ;
            menu.level = 2;
            menu.valueType = NO_TYPE;
            break;

        default:
            break;
    }
}


void clearRemoteControlDisplay( void )
{
    print7Segments( "     " );
    setDelayMs( 100 ); /* Reload the delay. */
    while ( false == isDelayMsEnding( ) );
}


/*******************************************************************************
 End of File
 */
