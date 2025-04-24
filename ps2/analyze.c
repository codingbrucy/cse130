#include "analyze.h"
#include <string.h>
#include <assert.h>

void analyzeURL(char *url, char *host, int *port, char *doc)
{
/* TODO Q1: Parse the URL - Implement the URL decomposition.
   * input: URL the given url
   output: host : where you place the host part of the URL
            port : where you place the port number (if any)
            doc  : where you place the document name
   * hint: scan the url for special character such as : and /. These characters mark the beginning and end of the above variables.
*/ 
   assert(url != NULL && host != NULL && port != NULL && doc != NULL);

   char *p = url;
   char *host_start = NULL;
   char *doc_start = NULL;

   // Find the protocol (e.g., http://)
   if (strncmp(p, "http://", 7) == 0) {
      p += 7; // Skip the protocol
   } else if (strncmp(p, "https://", 8) == 0) {
      p += 8; // Skip the protocol
   }

   // Find the host
   host_start = p;
   while (*p && *p != ':' && *p != '/') {
      p++;
   }
   size_t host_length = p - host_start;
   strncpy(host, host_start, host_length);
   host[host_length] = '\0';

   // Check for port
   if (*p == ':') {
      p++;
      *port = atoi(p);
      while (*p && *p != '/') {
            p++;
      }
   } else {
      *port = 80; // Default port for HTTP
   }

   // Find the document
   if (*p == '/') {
      doc_start = ++p; // Skip the '/'
      while (*p) {
            p++;
      }
      size_t doc_length = p - doc_start;
      strncpy(doc, doc_start, doc_length);
      doc[doc_length] = '\0';
   } else {
      doc[0] = '\0'; // No document part
   }
  
}
