#include <stdlib.h>

#include "protocol/wm_context.h"

void wm_cxt_destroy(wm_cxt **cxt) {
    if (*cxt) {
        if ((*cxt)->data) {
            free((*cxt)->data);
        }

        free(*cxt);
        *cxt = NULL;
    }
}
