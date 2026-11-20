#include "media.h"
#include <stdint.h>
int aion_framebuffer_validate(const struct aion_framebuffer *fb){
    size_t row,required;
    if(!fb || !fb->base || !fb->width || !fb->height || fb->stride<fb->width)return AION_MEDIA_EINVAL;
    if(fb->pixel_format!=AION_MEDIA_PIXEL_BGRA8888 && fb->pixel_format!=AION_MEDIA_PIXEL_RGBA8888)return AION_MEDIA_EINVAL;
    if((size_t)fb->stride>SIZE_MAX/4u)return AION_MEDIA_ERANGE;
    row=(size_t)fb->stride*4u;
    if((size_t)fb->height>SIZE_MAX/row)return AION_MEDIA_ERANGE;
    required=row*(size_t)fb->height;
    if(required>fb->bytes)return AION_MEDIA_ERANGE;
    return AION_MEDIA_OK;
}
int aion_framebuffer_pixel_offset(const struct aion_framebuffer *fb,uint32_t x,uint32_t y,size_t *offset){
    int rc=aion_framebuffer_validate(fb);
    if(rc)return rc;
    if(!offset)return AION_MEDIA_EINVAL;
    if(x>=fb->width || y>=fb->height)return AION_MEDIA_ERANGE;
    *offset=((size_t)y*(size_t)fb->stride+(size_t)x)*4u;
    return AION_MEDIA_OK;
}
int aion_audio_format_init(struct aion_audio_format *out,uint32_t rate,uint16_t channels,uint16_t bits){
    if(!out || rate<8000u || rate>384000u || channels<1u || channels>8u || (bits!=8u && bits!=16u && bits!=24u && bits!=32u))return AION_MEDIA_EINVAL;
    out->sample_rate=rate;out->channels=channels;out->bits_per_sample=bits;out->frame_bytes=(uint32_t)channels*(uint32_t)(bits/8u);
    return AION_MEDIA_OK;
}
int aion_audio_buffer_frames(const struct aion_audio_format *format,size_t bytes,size_t *frames){
    if(!format || !frames || !format->frame_bytes)return AION_MEDIA_EINVAL;
    if(bytes%format->frame_bytes)return AION_MEDIA_EINVAL;
    *frames=bytes/format->frame_bytes;
    return AION_MEDIA_OK;
}
