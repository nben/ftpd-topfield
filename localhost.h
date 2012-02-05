#ifndef LOCALHOST_H
#define LOCALHOST_H

/* Return the name of the localhost.  This is just a wrapper for gethostname,
   which takes care of allocating a big enough buffer, and caches the result
   after the first call (so the result should be copied before modification).
   If something goes wrong, 0 is returned, and errno set.  */
/* We no longer use static buffers, is to dangerous and
   cause subtile bugs.  */
char *localhost(int no_dns);

#endif
