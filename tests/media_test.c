#include "../kernel/hardware/media.h"
#include <assert.h>
int main(void){
    unsigned char pixels[4*8*4]={0};
    struct aion_framebuffer fb={pixels,sizeof(pixels),4,4,8,AION_MEDIA_PIXEL_BGRA8888};
    struct aion_audio_format audio;size_t offset,frames;
    assert(aion_framebuffer_validate(&fb)==0);
    assert(aion_framebuffer_pixel_offset(&fb,3,3,&offset)==0 && offset==108);
    assert(aion_framebuffer_pixel_offset(&fb,4,3,&offset)==AION_MEDIA_ERANGE);
    fb.bytes=32;assert(aion_framebuffer_validate(&fb)==AION_MEDIA_ERANGE);
    fb.bytes=sizeof(pixels);fb.stride=2;assert(aion_framebuffer_validate(&fb)==AION_MEDIA_EINVAL);
    assert(aion_audio_format_init(&audio,48000,2,16)==0 && audio.frame_bytes==4);
    assert(aion_audio_buffer_frames(&audio,1920,&frames)==0 && frames==480);
    assert(aion_audio_buffer_frames(&audio,1921,&frames)==AION_MEDIA_EINVAL);
    assert(aion_audio_format_init(&audio,0,2,16)==AION_MEDIA_EINVAL);
    return 0;
}
