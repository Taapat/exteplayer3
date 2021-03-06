/*
 * bcm_ioctls.h
 *
 * 
 *
 * 
 */

#ifndef H_DVB_BCM_H
#define H_DVB_BCM_H

typedef struct video_codec_data
{
    int32_t length;
    uint8_t *data;
} video_codec_data_t;
#define VIDEO_SET_CODEC_DATA _IOW('o', 80, video_codec_data_t)

typedef enum { 
    CT_MPEG1, 
    CT_MPEG2, 
    CT_H264, 
    CT_DIVX311, 
    CT_DIVX4, 
    CT_MPEG4_PART2, 
    CT_VC1, 
    CT_VC1_SM 
} video_codec_type_t;

typedef enum {
	STREAMTYPE_UNKNOWN = -1,
	STREAMTYPE_MPEG2 = 0,
	STREAMTYPE_MPEG4_H264 = 1,
	STREAMTYPE_H263 = 2,
	STREAMTYPE_VC1 = 3,
	STREAMTYPE_MPEG4_Part2 = 4,
	STREAMTYPE_VC1_SM = 5,
	STREAMTYPE_MPEG1 = 6,
	STREAMTYPE_XVID = 10,
	STREAMTYPE_DIVX311 = 13,
	STREAMTYPE_DIVX4 = 14,
	STREAMTYPE_DIVX5 = 15
} video_stream_type_t;



typedef enum {
	AUDIOTYPE_UNKNOWN = -1,
	AUDIOTYPE_AC3 = 0,
	AUDIOTYPE_MPEG = 1,
	AUDIOTYPE_DTS = 2,
	AUDIOTYPE_LPCM = 6,
	AUDIOTYPE_AAC = 8,
	AUDIOTYPE_AAC_HE = 9,
	AUDIOTYPE_MP3 = 0xa,
	AUDIOTYPE_AAC_PLUS = 0xb,
	AUDIOTYPE_DTS_HD = 0x10,
	AUDIOTYPE_WMA = 0x20,
	AUDIOTYPE_WMA_PRO = 0x21,
	AUDIOTYPE_AC3_PLUS = 0x22,
	AUDIOTYPE_AMR = 0x23,
	AUDIOTYPE_RAW = 0xf
} audio_stream_type_t;




#endif /* H_DVB_BCM_H */

