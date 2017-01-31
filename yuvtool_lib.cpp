// YuvTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


/**
* @file
* libswscale API use example.
* @example scaling_video.c
*/

#include <map>

using namespace std;

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

std::map<string, AVPixelFormat> fmtMap;

struct MapInit {
    MapInit() {
        fmtMap["YUV420"] = AV_PIX_FMT_YUV420P;
        fmtMap["NV12"] = AV_PIX_FMT_NV12;
    }
} mapInit;

int ff_load_image(uint8_t *data[4], int linesize[4],
    int *w, int *h, enum AVPixelFormat *pix_fmt,
    const char *filename, void *log_ctx)
{
    AVInputFormat *iformat = NULL;
    AVFormatContext *format_ctx = NULL;
    AVCodec *codec;
    AVCodecContext *codec_ctx;
    AVFrame *frame;
    int frame_decoded, ret = 0;
    AVPacket pkt;
    AVDictionary *opt = NULL;

    av_init_packet(&pkt);

    av_register_all();

    iformat = av_find_input_format("image2");
    if ((ret = avformat_open_input(&format_ctx, filename, iformat, NULL)) < 0) {
        av_log(log_ctx, AV_LOG_ERROR,
            "Failed to open input file '%s'\n", filename);
        return ret;
    }

    if ((ret = avformat_find_stream_info(format_ctx, NULL)) < 0) {
        av_log(log_ctx, AV_LOG_ERROR, "Find stream info failed\n");
        return ret;
    }

    codec_ctx = format_ctx->streams[0]->codec;
    codec = avcodec_find_decoder(codec_ctx->codec_id);
    if (!codec) {
        av_log(log_ctx, AV_LOG_ERROR, "Failed to find codec\n");
        ret = AVERROR(EINVAL);
        goto end;
    }

    av_dict_set(&opt, "thread_type", "slice", 0);
    if ((ret = avcodec_open2(codec_ctx, codec, &opt)) < 0) {
        av_log(log_ctx, AV_LOG_ERROR, "Failed to open codec\n");
        goto end;
    }

    if (!(frame = av_frame_alloc())) {
        av_log(log_ctx, AV_LOG_ERROR, "Failed to alloc frame\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    ret = av_read_frame(format_ctx, &pkt);
    if (ret < 0) {
        av_log(log_ctx, AV_LOG_ERROR, "Failed to read frame from file\n");
        goto end;
    }

    ret = avcodec_decode_video2(codec_ctx, frame, &frame_decoded, &pkt);
    if (ret < 0 || !frame_decoded) {
        av_log(log_ctx, AV_LOG_ERROR, "Failed to decode image from file\n");
        if (ret >= 0)
            ret = -1;
        goto end;
    }

    *w = frame->width;
    *h = frame->height;
    *pix_fmt = (AVPixelFormat)frame->format;

    if ((ret = av_image_alloc(data, linesize, *w, *h, *pix_fmt, 16)) < 0)
        goto end;
    ret = 0;

    av_image_copy(data, linesize, (const uint8_t **)frame->data, frame->linesize, *pix_fmt, *w, *h);

end:
    av_packet_unref(&pkt);
    avcodec_close(codec_ctx);
    avformat_close_input(&format_ctx);
    av_frame_free(&frame);
    av_dict_free(&opt);

    if (ret < 0)
        av_log(log_ctx, AV_LOG_ERROR, "Error loading image file '%s'\n", filename);
    return ret;
}

int load_yuv_image(const char *filename, uint8_t *data[4], int linesize[4],
    int width, int height, AVPixelFormat pix_fmt, int frame_index, int* total_frame)
{
    FILE *fp;
    int file_size;
    int frame_size;

    fopen_s(&fp, filename, "rb");

    if (!fp) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    switch (pix_fmt)
    {
    case AV_PIX_FMT_NV12:
        frame_size = width * height + width * height / 2;
        if ((file_size - frame_size * frame_index) < frame_size) {
            fclose(fp);
            return -2;
        }
        fseek(fp, frame_size * frame_index, SEEK_SET);
        fread_s(data[0], width * height, width * height, 1, fp);
        fread_s(data[1], width * height / 2, width * height / 2, 1, fp);
        break;
    case AV_PIX_FMT_YUV420P:
        frame_size = width * height + width / 2 * height / 2 + width / 2 * height / 2;
        if ((file_size - frame_size * frame_index) < frame_size) {
            fclose(fp);
            return -2;
        }
        fseek(fp, frame_size * frame_index, SEEK_SET);
        fread_s(data[0], width * height, width * height, 1, fp);
        fread_s(data[1], width / 2 * height / 2, width / 2 * height / 2, 1, fp);
        fread_s(data[2], width / 2 * height / 2, width / 2 * height / 2, 1, fp);
        break;
    default:
        break;
    }

    *total_frame = file_size / frame_size;

    fclose(fp);
    return 0;
}

const uchar* yuv2rgb(const char* src_filename, int src_w, int src_h, char* src_fmt, int dst_w, int dst_h, int frame_idx, int* total_frame)
{
    uint8_t *src_data[4] = {};
    uint8_t *dst_data[4] = {};
    int src_linesize[4], dst_linesize[4];
    string srcFormat = src_fmt;
    enum AVPixelFormat src_pix_fmt = fmtMap[srcFormat];
    enum AVPixelFormat dst_pix_fmt = AV_PIX_FMT_RGB24;
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

    if ((ret = load_yuv_image(src_filename, src_data, src_linesize, src_w, src_h, src_pix_fmt, frame_idx, total_frame)) < 0) {
        fprintf(stderr, "Cannot load source yuv file\n");
        goto end;
    }

    /* buffer is going to be written to rawvideo file, no alignment */
    if ((ret = av_image_alloc(dst_data, dst_linesize,
        dst_w, dst_h, dst_pix_fmt, 1)) < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
        goto end;
    }
    dst_bufsize = ret;

    /* convert to destination format */
    sws_scale(sws_ctx, (const uint8_t * const*)src_data,
        src_linesize, 0, src_h, dst_data, dst_linesize);

end:
    av_freep(&src_data[0]);
    //av_freep(&dst_data[0]);
    sws_freeContext(sws_ctx);
    return dst_data[0];
}
