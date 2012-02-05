#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "extern.h"


/* Always accept anything as anonymous login */
int
auth_user (const char *name, struct credentials *pcred)
{
  int len;

  pcred->guest = 0;

  if (pcred->message) {
    free (pcred->message);
  }
  len = 64 + strlen (name);
  pcred->message = malloc (len);

  if (pcred->message == NULL) {
    return -1;
  }

  pcred->guest = 1;
  strcpy (pcred->message,
		  "Guest login ok, type your name as password.");
  return 0;
}

int
auth_pass (const char *passwd, struct credentials *pcred)
{
  return 0;
}
