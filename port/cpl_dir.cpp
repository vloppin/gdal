/**********************************************************************
 * $Id$
 *
 * Name:     cpl_dir.cpp
 * Project:  CPL - Common Portability Library
 * Purpose:  Directory manipulation.
 * Author:   Daniel Morissette, danmo@videotron.ca
 *
 **********************************************************************
 * Copyright (c) 1998, Daniel Morissette
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 *
 * $Log$
 * Revision 1.1  1999/02/25 04:52:00  danmo
 * *** empty log message ***
 *
 **********************************************************************/

#include "cpl_conv.h"
#include "cpl_string.h"


#ifdef _WIN32

/*=====================================================================
                   WIN32 / MSVC++ implementation
 *====================================================================*/

#include <io.h>

/**********************************************************************
 *                          CPLReadDir()
 *
 * Return a stringlist with the list of files in a directory.
 * The returned stringlist should be freed with CSLDestroy().
 *
 * Returns NULL if an error happened or if the directory could not
 * be read.
 **********************************************************************/
char **CPLReadDir(const char *pszPath)
{
    struct _finddata_t c_file;
    long    hFile;
    char    *pszFileSpec, **papszDir = NULL;

    if (strlen(pszPath) == 0)
        pszPath = ".";

    pszFileSpec = CPLStrdup(CPLSPrintf("%s\\*.*", pszPath));

    if ( (hFile = _findfirst( pszFileSpec, &c_file )) != -1L )
    {
        do
        {
            papszDir = CSLAddString(papszDir, c_file.name);
        } while( _findnext( hFile, &c_file ) == 0 );

        _findclose( hFile );
    }
    else
    {
        /* Should we generate an error???  
         * For now we'll just return NULL (at the end of the function)
         */
    }

    CPLFree(pszFileSpec);

    return papszDir;
}

#else

/*=====================================================================
                      POSIX (Unix) implementation
 *====================================================================*/

#include <sys/types.h>
#include <dirent.h>

/**********************************************************************
 *                          CPLReadDir()
 *
 * Return a stringlist with the list of files in a directory.
 * The returned stringlist should be freed with CSLDestroy().
 *
 * Returns NULL if an error happened or if the directory could not
 * be read.
 **********************************************************************/
char **CPLReadDir(const char *pszPath)
{
    DIR           *hDir;
    struct dirent *psDirEntry;
    char          **papszDir = NULL;

    if (strlen(pszPath) == 0)
        pszPath = ".";

    if ( (hDir = opendir(pszPath)) != NULL )
    {
        while( (psDirEntry = readdir(hDir)) != NULL )
        {
            papszDir = CSLAddString(papszDir, psDirEntry->d_name);
        }

        closedir( hDir );
    }
    else
    {
        /* Should we generate an error???  
         * For now we'll just return NULL (at the end of the function)
         */
    }

    return papszDir;
}

#endif
