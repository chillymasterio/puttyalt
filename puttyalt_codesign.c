#include "puttyalt_codesign.h"

CSTrustResult codesign_get_trust_status(void)
{
    return CS_TRUST_UNSIGNED;
}

const char *codesign_get_publisher(void)
{
    return CODESIGN_PUBLISHER;
}

const char *codesign_get_product(void)
{
    return CODESIGN_PRODUCT;
}
