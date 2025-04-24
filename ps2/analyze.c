#include "analyze.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

void analyzeURL(char *url, char *host, int *port, char *doc) {
    assert(url && host && port && doc);
    char *p = url;

    // 1) Skip protocol if present
    if      (strncmp(p, "http://", 7)  == 0) p += 7;
    else if (strncmp(p, "https://", 8) == 0) p += 8;

    // 2) Parse host
    char *h_end = p;
    while (*h_end && *h_end != ':' && *h_end != '/') h_end++;
    size_t hlen = h_end - p;
    strncpy(host, p, hlen);
    host[hlen] = '\0';

    // 3) Parse port (default 80)
    if (*h_end == ':') {
        *port = atoi(h_end + 1);
        // skip “:port”
        char *slash = strchr(h_end, '/');
        p = slash ? slash : h_end + strlen(h_end);
    } else {
        *port = 80;
        p = h_end;
    }

    // 4) Parse doc (must include leading '/')
    if (*p == '/') {
        strcpy(doc, p);       // copies slash + rest
    } else {
        strcpy(doc, "/");     // fallback, though doc is mandatory
    }
}

