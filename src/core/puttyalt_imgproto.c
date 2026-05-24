#include <string.h>
#include "puttyalt_imgproto.h"

void imgproto_init(ImageConfig *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->protocol = IMG_PROTO_NONE;
    cfg->max_width = 800;
    cfg->max_height = 600;
    cfg->enabled = 0;
}

int imgproto_detect(const unsigned char *data, int len)
{
    /* Check for Sixel introducer: ESC P ... q */
    for (int i = 0; i < len - 2; i++) {
        if (data[i] == 0x1B && data[i+1] == 'P') {
            /* Look for 'q' terminator */
            for (int j = i + 2; j < len; j++) {
                if (data[j] == 'q') return IMG_PROTO_SIXEL;
            }
        }
    }

    /* Check for iTerm2 image protocol: ESC ] 1337 ; ... */
    for (int i = 0; i < len - 6; i++) {
        if (data[i] == 0x1B && data[i+1] == ']' &&
            data[i+2] == '1' && data[i+3] == '3' &&
            data[i+4] == '3' && data[i+5] == '7')
            return IMG_PROTO_ITERM2;
    }

    return IMG_PROTO_NONE;
}

void imgproto_set_protocol(ImageConfig *cfg, ImageProtocol proto)
{
    cfg->protocol = proto;
    cfg->enabled = (proto != IMG_PROTO_NONE);
}

int imgproto_supports_sixel(void)
{
    /* In full build, check terminal DA response */
    return 0;
}
