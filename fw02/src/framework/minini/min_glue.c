/**
 * @file min_glue.c
 * @author OpenFeeder Team <https://github.com/orgs/OpenFeeder/people>
 * @version 1.0
 * @date 
 */

#include "min_glue.h"

// DONE: Warning: 'ini_read' defined but not used.
/* Since the Memory Disk Drive file system library reads only blocks of files,
 * the function to read a text line does so by "over-reading" a block of the
 * maximum size and truncating it behind the end-of-line (eol).
 */
//static int ini_read( char *buffer, int size, INI_FILETYPE *file )
int ini_read( char *buffer, int size, INI_FILETYPE *file )
{
    size_t numread = size;
    char *eol; /* end-of-line */

    if ( ( numread = FILEIO_Read( buffer, 1, size, file ) ) == 0 )
    {
        return 0; /* at EOF */
    }

    if ( ( eol = strchr( buffer, '\n' ) ) == NULL )
    {
        eol = strchr( buffer, '\r' );
    }

    if ( eol != NULL )
    {
        /* terminate the buffer */
        *++eol = '\0';
        /* "unread" the data that was read too much */
        FILEIO_Seek( file, -( int ) ( numread - ( size_t ) ( eol - buffer ) ), FILEIO_SEEK_CUR );
    }

    return 1;
}


/*******************************************************************************
 End of File
 */
