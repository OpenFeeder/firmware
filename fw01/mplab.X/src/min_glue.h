/*  minIni glue functions for Microchip's "Memory Disk Drive" file system
 *  library, as presented in Microchip application note AN1045.
 *
 *  By CompuPhase, 2011-2014
 *  This "glue file" is in the public domain. It is distributed without
 *  warranties or conditions of any kind, either express or implied.
 *
 *  (The "Microchip Memory Disk Drive File System" is copyright (c) Microchip
 *  Technology Incorporated, and licensed at its own terms.)
 */

#define INI_BUFFERSIZE  30       /* maximum line length, maximum path length */

#include <string.h>
#include "fileio.h"

#define INI_FILETYPE                   FILEIO_OBJECT
#define ini_openread(filename,file)    ((FILEIO_Open((file), (filename),FILEIO_OPEN_READ)) != FILEIO_RESULT_FAILURE)
#define ini_openwrite(filename,file)   ((FILEIO_Open((file), (filename),FILEIO_OPEN_WRITE)) != FILEIO_RESULT_FAILURE)
#define ini_openrewrite(filename,file) ((FILEIO_Open((file), (filename),FILEIO_OPEN_APPEND)) != FILEIO_RESULT_FAILURE)
#define ini_close(file)                (FILEIO_Close((file)) == FILEIO_RESULT_SUCCESS)
#define ini_write(buffer,file)         (FILEIO_Write((buffer), 1, strlen(buffer), (*file)) > 0)
#define ini_remove(filename)           (FILEIO_Remove((filename)) == FILEIO_RESULT_SUCCESS)

#define INI_FILEPOS                    long int
#define ini_tell(file,pos)             (*(pos) = FILEIO_Tell((file)))
#define ini_seek(file,pos)             (FILEIO_Seek((file), (pos), FILEIO_SEEK_SET) == FILEIO_RESULT_SUCCESS)

#define INI_READONLY
#define PORTABLE_STRNICMP
#define NDEBUG

//static int ini_read( char *buffer, int size, INI_FILETYPE *file );
int ini_read( char *buffer, int size, INI_FILETYPE *file );

#ifndef INI_READONLY

static int ini_rename( const char *source, const char *dest )
{
    /*FILEIO_OBJECT* ftmp; 
    FILEIO_Open(ftmp, (source), FILEIO_OPEN_READ);
    FILEIO_Rename((dest), ftmp);
    return FILEIO_Close(ftmp) == FILEIO_RESULT_SUCCESS;*/

    return FILEIO_Rename( ( source ), ( dest ) ) == FILEIO_RESULT_SUCCESS;
}
#endif
