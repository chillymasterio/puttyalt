/* puttyalt_vkbackend.c - Vulkan-style backend descriptor (C99 model). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define VKBACKEND_MAX_QUEUES   8
#define VKBACKEND_NAME_LEN     48
#define VKBACKEND_MIN_IMAGES   2
#define VKBACKEND_MAX_IMAGES   8

enum vkbackend_format {
    VKBACKEND_FMT_UNDEFINED = 0, VKBACKEND_FMT_BGRA8 = 1,
    VKBACKEND_FMT_RGBA8 = 2, VKBACKEND_FMT_RGBA16F = 3
};
enum vkbackend_feature {
    VKBACKEND_FEAT_GEOMETRY = 1u << 0, VKBACKEND_FEAT_TESSELLAT = 1u << 1,
    VKBACKEND_FEAT_COMPUTE = 1u << 2, VKBACKEND_FEAT_ANISOTROPY = 1u << 3,
    VKBACKEND_FEAT_ALL = 0x0Fu
};
typedef struct vkbackend_desc {
    char     device_name[VKBACKEND_NAME_LEN];
    int32_t  device_index;
    uint32_t queue_family_count;
    uint32_t queue_counts[VKBACKEND_MAX_QUEUES];
    uint32_t swapchain_images;
    int      color_format;
    uint32_t feature_flags;
    uint64_t configured_ms;
    int      ready;
} vkbackend_desc;

int vkbackend_init(vkbackend_desc *d, uint64_t now_ms) {
    if (!d) return -1;
    memset(d, 0, sizeof(*d));
    strcpy(d->device_name, "none");
    d->device_index = -1;
    d->swapchain_images = VKBACKEND_MIN_IMAGES;
    d->color_format = VKBACKEND_FMT_BGRA8;
    d->configured_ms = now_ms;
    d->ready = 0;
    return 0;
}

int vkbackend_select_device(vkbackend_desc *d, const char *name,
                            int32_t index, const uint32_t *queues,
                            uint32_t queue_count, uint64_t now_ms) {
    uint32_t i;
    if (!d || !name || !queues || index < 0) return -1;
    if (queue_count == 0 || queue_count > VKBACKEND_MAX_QUEUES) return -1;
    for (i = 0; i < queue_count; i++) {
        if (queues[i] == 0) return -1;
        d->queue_counts[i] = queues[i];
    }
    d->queue_family_count = queue_count;
    strncpy(d->device_name, name, VKBACKEND_NAME_LEN - 1);
    d->device_name[VKBACKEND_NAME_LEN - 1] = '\0';
    d->device_index = index;
    d->configured_ms = now_ms;
    d->ready = 1;
    return 0;
}

int vkbackend_set_swapchain_images(vkbackend_desc *d, uint32_t count) {
    if (!d || count < VKBACKEND_MIN_IMAGES || count > VKBACKEND_MAX_IMAGES)
        return -1;
    d->swapchain_images = count;
    return 0;
}

int vkbackend_set_color_format(vkbackend_desc *d, int format) {
    if (!d || format <= VKBACKEND_FMT_UNDEFINED || format > VKBACKEND_FMT_RGBA16F)
        return -1;
    d->color_format = format;
    return 0;
}

int vkbackend_enable_feature(vkbackend_desc *d, uint32_t feature) {
    if (!d || feature == 0 || (feature & ~(uint32_t)VKBACKEND_FEAT_ALL) != 0)
        return -1;
    d->feature_flags |= feature;
    return 0;
}

int vkbackend_has_feature(const vkbackend_desc *d, uint32_t feature) {
    if (!d || feature == 0) return 0;
    return (d->feature_flags & feature) == feature ? 1 : 0;
}

int vkbackend_validate(const vkbackend_desc *d) {
    if (!d || !d->ready || d->device_index < 0) return -1;
    if (d->queue_family_count == 0 ||
        d->queue_family_count > VKBACKEND_MAX_QUEUES) return -1;
    if (d->swapchain_images < VKBACKEND_MIN_IMAGES ||
        d->swapchain_images > VKBACKEND_MAX_IMAGES) return -1;
    if (d->color_format <= VKBACKEND_FMT_UNDEFINED) return -1;
    return 0;
}

int vkbackend_describe(const vkbackend_desc *d, char *buf, size_t buflen) {
    int n;
    if (!d || !buf || buflen == 0) return -1;
    n = snprintf(buf, buflen,
                 "vk[%s #%d] queues=%u images=%u fmt=%d feat=0x%02X %s",
                 d->device_name, d->device_index, d->queue_family_count,
                 d->swapchain_images, d->color_format, d->feature_flags,
                 vkbackend_validate(d) == 0 ? "valid" : "invalid");
    if (n < 0 || (size_t)n >= buflen) return -1;
    return n;
}
