// YuvTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


/**
* @file
* libswscale API use example.
* @example scaling_video.c
*/

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>
}

static void load_yuv_image(const char *filename, uint8_t *data[4], int linesize[4],
    int width, int height, int frame_index)
{
    FILE *fp;

    fopen_s(&fp, filename, "rb");
    if (fp) {
        fread_s(data[0], width * height, width * height, 1, fp);
        fread_s(data[1], width * height / 2, width * height / 2, 1, fp);
    }
}

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h)
{
    uint8_t *src_data[4], *dst_data[4];
    int src_linesize[4], dst_linesize[4];
    int dst_w = src_w, dst_h = src_h;
    enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_NV12, dst_pix_fmt = AV_PIX_FMT_RGB24;
    const char *dst_size = NULL;
    int dst_bufsize;
    struct SwsContext *sws_ctx;
    int i = 0, ret;

    /* create scaling context */
    sws_ctx = sws_getContext(src_w, src_h, src_pix_fmt,
        dst_w, dst_h, dst_pix_fmt,
        SWS_BILINEAR, NULL, NULL, NULL);
    if (!sws_ctx) {
        fprintf(stderr,
            "Impossible to create scale context for the conversion "
            "fmt:%s s:%dx%d -> fmt:%s s:%dx%d\n",
            av_get_pix_fmt_name(src_pix_fmt), src_w, src_h,
            av_get_pix_fmt_name(dst_pix_fmt), dst_w, dst_h);
        ret = AVERROR(EINVAL);
        goto end;
    }

    /* allocate source and destination image buffers */
    if ((ret = av_image_alloc(src_data, src_linesize,
        src_w, src_h, src_pix_fmt, 16)) < 0) {
        fprintf(stderr, "Could not allocate source image\n");
        goto end;
    }

    /* buffer is going to be written to rawvideo file, no alignment */
    if ((ret = av_image_alloc(dst_data, dst_linesize,
        dst_w, dst_h, dst_pix_fmt, 1)) < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
        goto end;
    }
    dst_bufsize = ret;

    /* generate synthetic video */
    load_yuv_image(src_filename, src_data, src_linesize, src_w, src_h, 0);

    /* convert to destination format */
    sws_scale(sws_ctx, (const uint8_t * const*)src_data,
        src_linesize, 0, src_h, dst_data, dst_linesize);

end:
    av_freep(&src_data[0]);
    //av_freep(&dst_data[0]);
    sws_freeContext(sws_ctx);
    return dst_data[0];
}
