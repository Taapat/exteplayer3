/*
 * linuxdvb output/writer handling.
 *
 * konfetti 2010
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

/* ***************************** */
/* Includes                      */
/* ***************************** */
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "misc.h"
#include "writer.h"

/* ***************************** */
/* Makros/Constants              */
/* ***************************** */

#define WRITER_DEBUG

#ifdef WRITER_DEBUG

static short debug_level = 0;

#define writer_printf(level, x...) do { \
if (debug_level >= level) printf(x); } while (0)
#else
#define writer_printf(level, x...)
#endif

#ifndef WRITER_SILENT
#define writer_err(x...) do { printf(x); } while (0)
#else
#define writer_err(x...)
#endif

/* ***************************** */
/* Types                         */
/* ***************************** */

/* ***************************** */
/* Varaibles                     */
/* ***************************** */

static Writer_t * AvailableWriter[] = {
    &WriterAudioAAC,
    &WriterAudioAACHE,
    &WriterAudioAACPLUS,
    &WriterAudioAC3,
    &WriterAudioEAC3,
    &WriterAudioMP3,
    &WriterAudioMPEGL3,
    &WriterAudioPCM,
    &WriterAudioIPCM,
    &WriterAudioLPCM,
    &WriterAudioDTS,
    &WriterAudioWMA,
    &WriterAudioWMAPRO,
    
    &WriterVideoH264,
    &WriterVideoH263,
    &WriterVideoMPEG4,
    &WriterVideoMPEG2,
    &WriterVideoMPEG1,
    &WriterVideoVC1,
    &WriterVideoDIVX3,
    //&WriterVideoWMV,
    NULL
};

    // &WriterAudioIPCM,
    // &WriterAudioPCM,
    // &WriterAudioAC3,
    // &WriterAudioAAC,
    // &WriterAudioDTS,
    // &WriterAudioWMA,
    // &WriterAudioFLAC,
    // &WriterAudioVORBIS,

    // &WriterVideoMPEG2,
    // &WriterVideoMPEGH264,
    // &WriterVideoH264,
    // &WriterVideoDIVX,
    // &WriterVideoFOURCC,
    // &WriterVideoMSCOMP,
    // &WriterVideoWMV,
    // &WriterVideoH263,
    // &WriterVideoFLV,
    // &WriterVideoVC1,

/* ***************************** */
/* Prototypes                    */
/* ***************************** */

/* ***************************** */
/*  Functions                    */
/* ***************************** */
ssize_t write_with_retry(int fd, const void *buf, size_t size)
{
    ssize_t ret;
    int retval = 0;
    while(size > 0 && 0 == PlaybackDieNow(0))
    {
        ret = write(fd, buf, size);
        //printf("[%d] write [%lld]\n", fd, ret);
        if (ret < 0)
        {
            switch(errno)
            {
                case EINTR:
                case EAGAIN:
                    usleep(1000);
                    continue;
                default:
                    retval = -3;
                    break;
            }
            if (retval < 0)
            {
                break;
            }
        }
            
        if (ret < 0)
        {
            return ret;
        }
        
        size -= ret;
        buf += ret;
        
        if(size > 0)
        {
            if( usleep(1000) )
            {
                writer_err("usleep error \n");
            }
        }
    }
    return 0;
}

ssize_t writev_with_retry(int fd, const struct iovec *iov, size_t ic) 
{
    ssize_t len = 0;
    int i = 0;
    for(i=0; i<ic; ++i)
    {
        write_with_retry(fd, iov[i].iov_base, iov[i].iov_len); 
        len += iov[i].iov_len;
        if(PlaybackDieNow(0))
        {
            return -1;
        }
    }
    return len;
}

Writer_t* getWriter(char* encoding)
{
    int i;

    for (i = 0; AvailableWriter[i] != NULL; i++)
    {
        if (strcmp(AvailableWriter[i]->caps->textEncoding, encoding) == 0)
        {
            writer_printf(50, "%s: found writer \"%s\" for \"%s\"\n", __func__, AvailableWriter[i]->caps->name, encoding);
            return AvailableWriter[i];
        }
    }

    writer_printf(1, "%s: no writer found for \"%s\"\n", __func__, encoding);

    return NULL;
}

Writer_t* getDefaultVideoWriter()
{
    int i;

    for (i = 0; AvailableWriter[i] != NULL; i++)
    {
        if (strcmp(AvailableWriter[i]->caps->textEncoding, "V_MPEG2") == 0)
        {
            writer_printf(50, "%s: found writer \"%s\"\n", __func__, AvailableWriter[i]->caps->name);
            return AvailableWriter[i];
        }
    }

    writer_printf(1, "%s: no writer found\n", __func__);

    return NULL;
}

Writer_t* getDefaultAudioWriter()
{
    int i;

    for (i = 0; AvailableWriter[i] != NULL; i++)
    {
        if (strcmp(AvailableWriter[i]->caps->textEncoding, "A_MP3") == 0)
        {
            writer_printf(50, "%s: found writer \"%s\"\n", __func__, AvailableWriter[i]->caps->name);
            return AvailableWriter[i];
        }
    }

    writer_printf(1, "%s: no writer found\n", __func__);

    return NULL;
}

