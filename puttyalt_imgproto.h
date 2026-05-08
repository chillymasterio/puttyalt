#ifndef PUTTYALT_IMGPROTO_H
#define PUTTYALT_IMGPROTO_H

typedef enum {
    IMG_PROTO_NONE = 0,
    IMG_PROTO_SIXEL,
    IMG_PROTO_ITERM2,
    IMG_PROTO_KITTY
} ImageProtocol;

typedef struct ImageConfig {
    ImageProtocol protocol;
    int max_width;
    int max_height;
    int enabled;
    int inline_display;
} ImageConfig;

void imgproto_init(ImageConfig *cfg);
int  imgproto_detect(const unsigned char *data, int len);
void imgproto_set_protocol(ImageConfig *cfg, ImageProtocol proto);
int  imgproto_supports_sixel(void);

#endif
