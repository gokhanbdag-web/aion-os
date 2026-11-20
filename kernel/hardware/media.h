#ifndef AION_MEDIA_H
#define AION_MEDIA_H
#include <stddef.h>
#include <stdint.h>
#define AION_MEDIA_OK 0
#define AION_MEDIA_EINVAL (-22)
#define AION_MEDIA_ERANGE (-34)
#define AION_MEDIA_PIXEL_BGRA8888 1u
#define AION_MEDIA_PIXEL_RGBA8888 2u
struct aion_framebuffer {
    void *base;
    size_t bytes;
    uint32_t width,height,stride,pixel_format;
};
struct aion_audio_format {
    uint32_t sample_rate;
    uint16_t channels,bits_per_sample;
    uint32_t frame_bytes;
};
int aion_framebuffer_validate(const struct aion_framebuffer *fb);
int aion_framebuffer_pixel_offset(const struct aion_framebuffer *fb,uint32_t x,uint32_t y,size_t *offset);
int aion_audio_format_init(struct aion_audio_format *out,uint32_t sample_rate,uint16_t channels,uint16_t bits_per_sample);
int aion_audio_buffer_frames(const struct aion_audio_format *format,size_t buffer_bytes,size_t *frames);
#endif
