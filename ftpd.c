/* - Ftp Server
 * Copyright (c) 1985, 1988, 1990, 1992, 1993, 1994, 2002
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if 0
static char sccsid[] = "@(#)ftpd.c	8.5 (Berkeley) 4/28/95";
#endif

/*
 * FTP server.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <alloca.h>

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif

#include <netinet/in.h>
#ifdef HAVE_NETINET_IN_SYSTM_H
#  include <netinet/in_systm.h>
#endif
#ifdef HAVE_NETINET_IP_H
#  include <netinet/ip.h>
#endif

#define	FTP_NAMES
#include <arpa/ftp.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <netdb.h>
#include <setjmp.h>
#include <signal.h>
#include <grp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fnmatch.h>

#include <tf_io.h>
#include <tf_fwio.h>
#include <tf_util.h>
#include <daemon.h>

#include "extern.h"
#include "localhost.h"

#ifndef LINE_MAX
# define LINE_MAX 2048
#endif

#ifndef LOG_FTP
# define LOG_FTP LOG_DAEMON	/* Use generic facility.  */
#endif

#ifndef MAP_FAILED
# define MAP_FAILED (void*)-1
#endif

/* Exported to ftpcmd.h.  */
struct	sockaddr_in data_dest; /* Data port.  */
struct	sockaddr_in his_addr;  /* Peer address.  */
int	logging;               /* Enable log to syslog.  */
int	type = TYPE_A;         /* Default TYPE_A.  */
int	form = FORM_N;                  /* Default FORM_N.  */
int	debug;                 /* Enable debug mode if 1.  */
int	timeout = 300;         /* Timeout after 5 minutes of inactivity.  */
int	maxtimeout = 7200;     /* Don't allow idle time to be set
					beyond 2 hours.  */
int	pdata = -1;            /* For passive mode.  */
char	*hostname;             /* Who we are.  */
int	usedefault = 1;	       /* For data transfers.  */
char	tmpline[7];            /* Temp buffer use in OOB.  */

int	global_turbo_mode = 0;
static tf_handle tf;
static int topfield_is_open = 0;
static int verbose = 0;
static int lockfd = -1;
static int got_sighup = 0;

static int no_dns = 1;
static int use_eplf = 0;
static char *topfield_cwd = 0;
static int topfield_device = 0;
static int opt_crc_check = 0;

jmp_buf  errcatch;

/* Requester credentials.  */
struct credentials cred;

static struct  sockaddr_in ctrl_addr;    /* Control address.  */
static struct  sockaddr_in data_source;  /* Port address.  */
static struct  sockaddr_in pasv_addr;    /* Pasv address.  */

static int      data = -1;       /* Port data connection socket.  */
static int      stru = STRU_F;     /* Avoid C keyword.  */
static int      stru_mode = MODE_S;/* Default STRU mode stru_mode = MODE_S.  */
static int      anon_only;       /* Allow only anonymous login.  */
static int      no_version;      /* Don't print version to client.  */
static int      daemon_mode;     /* Start in daemon mode.  */
static off_t    file_size;
static off_t    byte_count;
static sig_atomic_t transflag;   /* Flag where in a middle of transfer.  */
static const char *pid_file = PATH_FTPDPID;
#if !defined(CMASK) || CMASK == 0
#undef CMASK
#define CMASK 027
#endif
static int  defumask = CMASK;    /* Default umask value.  */
static int login_attempts;       /* Number of failed login attempts.  */
static int askpasswd;		 /* Had user command, ask for passwd.  */
static char curname[10];	 /* Current USER name.  */

/*
 * Timeout intervals for retrying connections
 * to hosts that don't accept PORT cmds.  This
 * is a kludge, but given the problems with TCP...
 */
#define	SWAITMAX	90	/* wait at most 90 seconds */
#define	SWAITINT	5	/* interval between retries */

static int swaitmax = SWAITMAX;
static int swaitint = SWAITINT;

#ifdef HAVE_SETPROCTITLE
char	proctitle[LINE_MAX];	/* initial part of title */
#endif /* SETPROCTITLE */

static void ack __P ((const char *));
#ifdef HAVE_LIBWRAP
static int  check_host __P ((struct sockaddr *sa));
#endif
static void complete_login __P ((struct credentials *));
static const char *curdir __P ((void));
static const char *topfield_dir __P ((const char *dir));
static FILE *dataconn __P ((const char *, off_t, const char *));
static void dolog __P ((struct sockaddr_in *, struct credentials *));
static void end_login __P ((struct credentials *));
static FILE *getdatasock __P ((const char *));
static void myoob __P ((int));
static void sigquit __P ((int));
static void sighup __P ((int));
static void usage __P ((const char *progname, int err));

static const char *short_options = "Aa:bcDdeEhlp:P:qRt:T:u:vV";
static struct option long_options[] =
{
	{ "anonymous-only", no_argument, 0, 'A' },
	{ "auth", required_argument, 0, 'a' },
	{ "turbo", no_argument, 0, 'b' },
	{ "crc-check", no_argument, 0, 'c' },
	{ "daemon", no_argument, 0, 'D' },
	{ "debug", no_argument, 0, 'd' },
	{ "device", required_argument, 0, 'X' },
	{ "eplf", no_argument, 0, 'E' },
	{ "noeplf", no_argument, 0, 'e' },
	{ "help", no_argument, 0, 'h' },
	{ "logging", no_argument, 0, 'l' },
	{ "pidfile", required_argument, 0, 'p' },
	{ "port", required_argument, 0, 'P' },
	{ "no-version", no_argument, 0, 'q' },
	{ "resolve-dns", no_argument, 0, 'R' },
	{ "timeout", required_argument, 0, 't' },
	{ "max-timeout", required_argument, 0, 'T' },
	{ "umask", required_argument, 0, 'u' },
	{ "verbose", no_argument, 0, 'v' },
	{ "version", no_argument, 0, 'V' },
	{ 0, 0, 0, 0 }
};

static void LOGCMD(const char *cmd, const char *file)
{
	if (logging > 1) {
		syslog(LOG_INFO,"%s %s%s", cmd, *(file) == '/' ? "" : curdir(), file);
	}
}

static void LOGCMD2(const char *cmd, const char *file1, const char *file2)
{
	if (logging > 1) {
		syslog(LOG_INFO,"%s %s%s %s%s", cmd, *(file1) == '/' ? "" : curdir(), file1, *(file2) == '/' ? "" : curdir(), file2);
	}
}

static void LOGBYTES(const char *cmd, const char *file, off_t cnt)
{
	if (logging > 1) { 
			if (cnt == (off_t)-1)  {
				syslog(LOG_INFO,"%s %s%s", cmd, *(file) == '/' ? "" : curdir(), file); 
			}
			else  {
				syslog(LOG_INFO, "%s %s%s = %Ld bytes", cmd, (*(file) == '/') ? "" : curdir(), file, (long long)cnt); 
			}
	}
}

static void
usage (const char *progname, int err)
{
	if (err != 0)
		{
			fprintf (stderr, "Usage: %s [OPTION] ...\n", progname);
			fprintf (stderr, "Try `%s --help' for more information.\n", progname);
		}
	else
		{
			fprintf (stdout, "Usage: %s [OPTION] ...\n", progname);
			puts ("Internet File Transfer Protocol server.\n\n\
    -A, --anonymous-only      Server configure for anonymous service only\n\
    -b, --turbo               Default to turbo mode on for transfers\n\
    -c, --crc-check           Enable crc check on received packets\n\
    -D, --daemon              Start the ftpd standalone\n\
    -d, --debug               Debug mode\n\
        --device n            Select topfield device (first is 0, second is 1, etc.)\n\
    -E, --eplf                Send listing in EPLF mode\n\
    -l, --logging             Increase verbosity of syslog messages\n\
    -p, --pidfile=[PIDFILE]   Change default location of pidfile\n\
    -P, --port=[port]         Change the port to listen on in server mode\n\
    -q, --no-version          Do not display version in banner\n\
    -R, --resolve-dns         Do reverse DNS lookups\n\
    -t, --timeout=[TIMEOUT]   Set default idle timeout\n\
    -T, --max-timeout         Reset maximum value of timeout allowed\n\
    -u, --umask               Set default umask(base 8)\n\
        --help                Print this message\n\
    -V, --version             Print version\n\
    -a, --auth=[AUTH]         Use AUTH for authentication, it can be:\n\
                                 default     passwd authentication.");
		}
	exit (err);
}

int
main(int argc, char *argv[], char **envp)
{
	extern void yyparse __P((void));
	int option;
	int port = -1;
	const char *progname = argv[0];

#ifdef HAVE_TZSET
	tzset(); /* In case no timezone database in ~ftp.  */
#endif

#ifdef HAVE_INITSETPROCTITLE
	/* Save start and extent of argv for setproctitle.  */
	initsetproctitle (argc, argv, envp);
#endif /* HAVE_INITSETPROCTITLE */

	while ((option = getopt_long (argc, argv, short_options,
				long_options, NULL)) != EOF)
		{
			switch (option)
	{
	case 'A': /* Anonymous ftp only.  */
		anon_only = 1;
		break;

	case 'a': /* Authentification method.  */
		fprintf(stderr, "Warning: option -a ignored\n");
		break;
	
	case 'c':
		opt_crc_check = 1;
		break;

	case 'D': /* Run ftpd as daemon.  */
		daemon_mode = 1;
		break;

	case 'd': /* Enable debug mode.  */
		debug = 1;
		break;

	case 'X': /* Select device */
		topfield_device = atoi(optarg);
		break;

	case 'E': /* Enable eplf listings */
		use_eplf = 1;
		break;

	case 'e': /* Old disable eplf listings */
		fprintf(stderr, "Warning: option -e/--noeplf is deprecated. eplf mode is off by default\n");
		break;

	case 'l': /* Increase logging level.  */
		logging++;	/* > 1 == Extra logging.  */
		break;

	case 'b': /* High speed (turbo) mode.  */
		global_turbo_mode = 1;
		break;

	case 'p': /* Override pid file */
		pid_file = optarg;
		break;

	case 'P': /* Override default port */
		port = atoi(optarg);
		break;

	case 'q': /* Don't include version number in banner.  */
		no_version = 1;
		break;

	case 'R': /* Use reverse DNS */
		no_dns = 0;
		break;

	case 't': /* Set default timeout value.  */
		timeout = atoi (optarg);
		if (maxtimeout < timeout)
			maxtimeout = timeout;
		break;

	case 'T': /* Maximum timeout allowed.  */
		maxtimeout = atoi (optarg);
		if (timeout > maxtimeout)
			timeout = maxtimeout;
		break;

	case 'u': /* Set umask.  */
		{
			long val = 0;

			val = strtol (optarg, &optarg, 8);
			if (*optarg != '\0' || val < 0)
				fprintf (stderr, "%s: bad value for -u", argv[0]);
			else
				defumask = val;
			break;
		}

	case 'h': /* Usage.  */
		usage (progname, 0);
		/* Not reached.  */

	case 'V': /* Version.  */
		printf ("ftpd (%s) %s\n", PACKAGE_NAME, PACKAGE_VERSION);
		exit (0);

	case 'v': /* Increment verbosity level */
		verbose++;
		break;

	case '?':
	default:
		usage (progname, 1);
		/* Not reached.  */
	}
		}

	/* Bail out, wrong usage */
	argc -= optind;
	if (argc != 0)
		usage (progname, 1);

	/* LOG_NDELAY sets up the logging connection immediately,
		 necessary for anonymous ftp's that chroot and can't do it later.  */
	openlog ("ftpd-topfield", LOG_PID | LOG_NDELAY, LOG_FTP);

	if (!debug) {
		(void) freopen (PATH_DEVNULL, "w", stderr);
	}

	/* If not running via inetd, we detach and dup(fd, 0), dup(fd, 1) the
		 fd = accept(). tcpd is check if compile with the support  */
	if (daemon_mode)
		{
			if (server_mode (pid_file, &his_addr, port, debug) < 0)
	exit (1);
		}
	else
		{
			socklen_t addrlen = sizeof (his_addr);
			if (getpeername (STDIN_FILENO, (struct sockaddr *)&his_addr,
					 &addrlen) < 0)
	{
		syslog (LOG_ERR, "getpeername (%s): %m", progname);
		exit (1);
	}
		}

	(void) signal (SIGINT, sigquit);
	(void) signal (SIGQUIT, sigquit);
	(void) signal (SIGTERM, sigquit);
	(void) signal (SIGPIPE, abort_transfer);
	(void) signal (SIGCHLD, SIG_IGN);
	if (signal (SIGURG, myoob) == SIG_ERR)
		syslog (LOG_ERR, "signal: %m");


	/* Ensure that the connection to the topfield is closed gracefully */
	atexit(close_topfield);

	{
		/* Need to use sigaction here so that read() does not automatically restart */
		struct sigaction sa;

		sa.sa_flags = 0;

		sigemptyset(&sa.sa_mask);
		sigaddset(&sa.sa_mask, SIGHUP);
		sa.sa_handler = sighup;
		sigaction(SIGHUP, &sa, NULL);
	}


	/* Get info on the ctrl connection.  */
	{
		socklen_t addrlen = sizeof (ctrl_addr);
		if (getsockname (STDIN_FILENO, (struct sockaddr *)&ctrl_addr, &addrlen) < 0) {
			syslog (LOG_ERR, "getsockname (%s): %m", progname);
			exit (1);
		}
	}

#if defined (IP_TOS) && defined (IPTOS_LOWDELAY) && defined (IPPROTO_IP)
	/* To  minimize delays for interactive traffic.  */
	{
		int tos = IPTOS_LOWDELAY;
		if (setsockopt (STDIN_FILENO, IPPROTO_IP, IP_TOS,
				(char *)&tos, sizeof(int)) < 0)
			syslog (LOG_WARNING, "setsockopt (IP_TOS): %m");
	}
#endif

#ifdef SO_OOBINLINE
	/* Try to handle urgent data inline.  */
	{
		int on = 1;
		if (setsockopt (STDIN_FILENO, SOL_SOCKET, SO_OOBINLINE,
				(char *)&on, sizeof (on)) < 0)
			syslog (LOG_ERR, "setsockopt: %m");
	}
#endif

#ifdef SO_KEEPALIVE
	/* Set keepalives on the socket to detect dropped connections.  */
	{
		int keepalive = 1;
		if (setsockopt (STDIN_FILENO, SOL_SOCKET, SO_KEEPALIVE,
				(char *)&keepalive, sizeof (keepalive)) < 0)
			syslog (LOG_WARNING, "setsockopt (SO_KEEPALIVE): %m");
	}
#endif

#ifdef	F_SETOWN
	if (fcntl (STDIN_FILENO, F_SETOWN, getpid ()) == -1)
		syslog (LOG_ERR, "fcntl F_SETOWN: %m");
#endif

	dolog (&his_addr, &cred);

	/* Deal with login disable.  */
	if (display_file (PATH_NOLOGIN, 530) == 0)
		{
			reply (530, "System not available.");
			exit (0);
		}

	/* Display a Welcome message if exists,
		 N.B. reply(220,) must follow.  */
	(void) display_file (PATH_FTPWELCOME, 220);

	/* Start at the root directory */
	topfield_cwd = strdup("/");

	hostname = localhost (no_dns);
	if (! hostname)
		perror_reply (550, "Local resource failure: malloc");

	/* Tell them we're ready to roll.  */
	if (!no_version)
		reply (220, "%s FTP server (%s %s) ready.",
		 hostname, PACKAGE_NAME, PACKAGE_VERSION);
	else
		reply (220, "%s FTP server ready.", hostname);

#if 0
	if (!tf.dev) {
		reply (220, "Running in fake mode");
	}
#endif

	/* Set the jump, if we have an error parsing,
		 come here and start fresh.  */
	(void) setjmp (errcatch);

	/* Roll.  */
	for (;;)
		(void) yyparse ();
	/* NOTREACHED */
}

int open_topfield(int intr_transfer)
{
	if (0) {
		syslog (LOG_DEBUG, "open_topfield(intr_transfer=%d), topfield_is_open=%d", intr_transfer, topfield_is_open);
	}
	if (!topfield_is_open) {
		int ret = topfield_open(&tf, topfield_device, TF_LOCK_HUP);
		if (debug) {
			syslog (LOG_DEBUG, "topfield_open(%d) returned %d", topfield_device, ret);
		}
		if (ret > 0) {
			/* Failed to lock either because something bad happened or
			 * the existing locker refused to give up the lock
			 */
			reply(550, "Topfield is already in use");
			return 0;
		}
		if (ret < 0) {
			reply(550, "Failed to connect to Topfield PVR");
			return 0;
		}

		/* REVISIT: timeout */
		tf.trace_level = verbose;
		tf.nocrc = opt_crc_check ? 0 : 1;

		if (tf_init(&tf) != 0) {
			daemon_release_lock(lockfd);
			topfield_close(&tf);
			reply(550, "Failed to establish communication with Topfield");
			return 0;
		}

		topfield_is_open = 1;
	}
	return topfield_is_open;
}

void close_topfield(void)
{
	if (0) {
		syslog (LOG_DEBUG, "close_topfield() topfield_is_open=%d", topfield_is_open);
	}
	if (topfield_is_open) {
		topfield_close(&tf);
		topfield_is_open = 0;
		daemon_release_lock(lockfd);
	}
}

void ftp_idle(void)
{
	/* If we received a SIGHUP, close our connection to the topfield */
	if (got_sighup) {
		close_topfield();
		got_sighup = 0;
	}
}

void cwd(const char *newpath)
{
	tf_dirent dirent;
	char *newcwd = strdup(tf_makename(topfield_cwd, newpath));

	/* Now see if this is a valid directory */
	if (strcmp(newcwd, "/firmware") == 0) {
		reply(250, "Entered firmware upgrade dir OK. Send .tfd file to upgrade");
	}
	else if (tf_stat(&tf, topfield_dir(newcwd), &dirent) != 0 || dirent.type != 'd') {
		reply(550, "Failed to chdir to %s", newpath);
		free(newcwd);
		return;
	}
	else {
		ack ("CWD");
	}

	free(topfield_cwd);
	topfield_cwd = newcwd;

	/*fprintf(stderr, "Now topfield_cwd=%s\n", topfield_cwd);*/
}

/* This is curdir as the user sees it */
static const char *curdir(void)
{
	return topfield_cwd;
}

/* Convert a (possibly) virtual dir to the actual dir on the topfield */
static const char *topfield_dir(const char *dir)
{
	/* Skip the /turbo prefix if necessary */
	if (strcmp(dir, "/turbo") == 0) {
		/*fprintf(stderr, "topfield_dir() %s => %s\n", dir, "/");*/
		return "/";
	}
	if (strncmp(dir, "/turbo/", 7) == 0) {
		/*fprintf(stderr, "topfield_dir() %s => %s\n", dir, dir + 6);*/
		return dir + 6;
	}
	/*fprintf(stderr, "topfield_dir() => %s\n", dir);*/
	return dir;
}

static int is_turbo(const char *path)
{
	if (global_turbo_mode) {
		return 1;
	}
	/* if topfield_dir() returns something different from curdir(), we are in turbo mode */
	if (strcmp(path, topfield_dir(path)) != 0) {
		return 1;
	}
	return 0;
}

time_t filetime(const char *path)
{
	tf_dirent dirent;

	const char *fullpath = topfield_dir(tf_makename(curdir(), path));

	if (tf_stat(&tf, fullpath, &dirent) != 0) {
		return -1;
	}
	else {
		return dirent.stamp;
	}
}

off_t filesize(const char *path)
{
	tf_dirent dirent;

	const char *fullpath = topfield_dir(tf_makename(curdir(), path));

	if (tf_stat(&tf, fullpath, &dirent) != 0 || dirent.type != 'f') {
		return -1;
	}
	else {
		return dirent.size;
	}
}

static void
sigquit (int signo)
{
#ifdef HAVE_STRSIGNAL
	syslog (LOG_ERR, "got signal %s", strsignal (signo));
#else
	syslog (LOG_ERR, "got signal %d", signo);
#endif
	dologout (-1);
}

static void
sighup (int signo)
{
	/* Just remember that we got this signal. We handle it
	 * during idle()
	 */
	got_sighup = 1;
}

static jmp_buf abort_buf;
static int abort_buf_valid;

void
abort_transfer(int signo)
{
	(void)signo;
	if (debug) {
		if (signo == 0) {
			syslog (LOG_DEBUG, "Received ABOR");
		}
		else {
			syslog (LOG_DEBUG, "lost connection");
		}
	}
	if (abort_buf_valid) {
		longjmp(abort_buf, 1);
	}
	/* Must not be doing a transfer, so ignore ABOR/SIGPIPE */
}

/* Helper function.  */
char *
sgetsave (const char *s)
{
	char *string;
	size_t len;

	if (s == NULL)
		s = "";

	len = strlen (s) + 1;
	string = malloc (len);
	if (string == NULL)
		{
			perror_reply (421, "Local resource failure: malloc");
			dologout (1);
			/* NOTREACHED */
		}
	/*  (void) strcpy (string, s); */
	memcpy (string, s, len);
	return string;
}

static void
complete_login (struct credentials *pcred)
{
	/* Display a login message, if it exists.
		N.B. reply(230,) must follow the message.  */
	(void) display_file (PATH_FTPLOGINMESG, 230);

	if (pcred->guest)
		{
			reply (230, "Guest login ok, access restrictions apply.");
#ifdef HAVE_SETPROCTITLE
			snprintf (proctitle, sizeof (proctitle), "%s: anonymous",
		pcred->remotehost);
			setproctitle ("%s",proctitle);
#endif /* HAVE_SETPROCTITLE */
			if (logging)
	syslog (LOG_INFO, "ANONYMOUS FTP LOGIN FROM %s",
		pcred->remotehost);
		}
	else
		{
			reply (230, "User %s logged in.", pcred->name);
#ifdef HAVE_SETPROCTITLE
			snprintf (proctitle, sizeof (proctitle),
		"%s: %s", pcred->remotehost, pcred->name);
			setproctitle ("%s",proctitle);
#endif /* HAVE_SETPROCTITLE */
			if (logging)
	syslog (LOG_INFO, "FTP LOGIN FROM %s as %s",
		pcred->remotehost, pcred->name);
		}
	(void) umask(defumask);
	return;
}

/* USER command.
	 Sets global passwd pointer pw if named account exists and is acceptable;
	 sets askpasswd if a PASS command is expected.  If logged in previously,
	 need to reset state.  */
void
user (const char *name)
{
	if (cred.logged_in)
		{
			if (cred.guest)
	{
		reply (530, "Can't change user from guest login.");
		return;
	}
			end_login (&cred);
		}

	/* Non zero means failed.  */
	if (auth_user (name, &cred) != 0)
		{
			/* If they gave us a reason.  */
			if (cred.message)
	{
		reply (530, "%s", cred.message);
		free (cred.message);
		cred.message = NULL;
	}
			else
	reply (530, "User %s access denied.", name);
			if (logging)
	syslog(LOG_NOTICE, "FTP LOGIN REFUSED FROM %s, %s",
				 cred.remotehost, name);
			return;
		}

	/* If the server is set to serve anonymous service only
		 the request have to come from a guest or a chrooted.  */
	if (anon_only && !cred.guest && !cred.dochroot)
		{
			reply (530, "Sorry, only anonymous ftp allowed");
			return;
		}

	if (logging)
		{
			strncpy (curname, name, sizeof (curname) - 1);
			curname [sizeof (curname) - 1] = '\0'; /* Make sure null terminated.  */
		}

	if (cred.message)
		{
			reply (331, "%s", cred.message);
			free (cred.message);
			cred.message = NULL;
		}
	else
		reply (331, "Password required for %s.", name);

	askpasswd = 1;

	/* Delay before reading passwd after first failed
		 attempt to slow down passwd-guessing programs.  */
	if (login_attempts)
		sleep ((unsigned) login_attempts);
}

/* Terminate login as previous user, if any, resetting state;
	 used when USER command is given or login fails.  */
static void
end_login (struct credentials *pcred)
{
	char *remotehost = pcred->remotehost;
	int atype = pcred->auth_type;

	if (pcred->name)
		free (pcred->name);
	if (pcred->passwd)
		{
			memset (pcred->passwd, 0, strlen (pcred->passwd));
			free (pcred->passwd);
		}
	if (pcred->homedir)
		free (pcred->homedir);
	if (pcred->rootdir)
		free (pcred->rootdir);
	if (pcred->shell)
		free (pcred->shell);
	if (pcred->pass) /* ??? */
		{
			memset (pcred->pass, 0, strlen (pcred->pass));
			free (pcred->pass);
		}
	if (pcred->message)
		free (pcred->message);
	memset (pcred, 0, sizeof (*pcred));
	pcred->remotehost = remotehost;
	pcred->auth_type = atype;
}

void
pass (const char *passwd)
{
	if (cred.logged_in || askpasswd == 0)
		{
			reply(503, "Login with USER first.");
			return;
		}
	askpasswd = 0;

	if (!cred.guest) /* "ftp" is the only account allowed no password.  */
		{
			/* Try to authenticate the user.  Failed if != 0.  */
			if (auth_pass (passwd, &cred) != 0)
	{
		/* Any particular reasons.  */
		if (cred.message)
			{
				reply (530, "%s", cred.message);
				free (cred.message);
				cred.message = NULL;
			}
		else
			reply (530, "Login incorrect.");
		if (logging)
			syslog (LOG_NOTICE, "FTP LOGIN FAILED FROM %s, %s",
				cred.remotehost, curname);
		if (login_attempts++ >= 5)
			{
				syslog(LOG_NOTICE, "repeated login failures from %s",
				 cred.remotehost);
				exit(0);
			}
		return;
	}
		}
	cred.logged_in = 1; /* Everything seems to be allright.  */
	complete_login (&cred);
	login_attempts = 0; /* This time successful.  */
}

void
reboot_topfield (void)
{
	tf_cmd_reset(&tf);

	if (tf.error) {
		perror_reply(550, "Failed to reboot");
	}
	else {
		ack("SITE REBOOT");
	}
}

void
retrieve (const char *cmd, const char *name)
{
	FILE *dout = 0;
	off_t current = restart_point;
	tf_dirent dirent;
	tf_buffer buf;
	int ret = -1;
	int retries = 0;
	size_t skip = 0;
	int aborted = 0;
	int in_get = 0;
	int replied = 0;
	int eof;

	char *userpath = tf_makename(curdir(), name);
	const char *srcpath = topfield_dir(userpath);

	int turbo = is_turbo(userpath);

	aborted = setjmp(abort_buf);
	abort_buf_valid = 1;
	transflag = 1;

	/* Retry a few times if necessary */
	while (!aborted) {

		syslog(LOG_INFO, "retrieve: %s @%Ld", srcpath, (long long)current);

		if (current % 512 != 0) {
			syslog(LOG_DEBUG, "Note: request for data not on 512 byte boundary (%Ld %% 512 == %Ld)", (long long)current, (long long)(current % 512));
			skip = current % 512;
			current -= skip;
		}

		if (retries++ >= 5) {
			syslog(LOG_WARNING, "retrieve: Too many retries, giving up");
			ret = -1;
			break;
		}

		if (turbo) {
			tf_cmd_turbo(&tf, 1);
		}

		ret = tf_cmd_get(&tf, srcpath, current, &dirent);

		/*fprintf(stderr, "tf_cmd_get() srcpath=%s, current=%Ld, ret=%d\n", srcpath, current, ret);*/
		//syslog(LOG_INFO, "tf_cmd_get(%s, current=%Ld) returned ret=%d, size=%Ld", srcpath, current, ret, dirent.size);

		if (ret != 0) {
			if (debug) {
				fprintf(stderr, "tf_cmd_get() returned %d\n", ret);
			}
			if (ret < 0) {
				reply (550, "Not a file: %s", name);
				replied = 1;
				break;
			}

			/* If the tf_cmd_get() fails, just try again */
			continue;
		}

		/* a get operation has begun which will need to be cancelled */
		in_get = 1;

		/* Open the data connection if not already done */
		if (!dout) {
			dout = dataconn (name, dirent.size, "w");

			if (!dout) {
				syslog(LOG_ERR, "Failed to open data connection: %m\n");
				ret = -1;
				break;
			}
			else {
				/* Put dout into nonblocking mode */
				long on = 1;
				fcntl(fileno(dout), F_SETFL, O_NONBLOCK, &on);
			}
		}

		eof = 0;

		while (ret == 0) {
			unsigned char *data;
			int len;

			/* The topfield will return data past the end of the file. Don't let it */
			if (current >= dirent.size) {
				//syslog(LOG_INFO, "past EOF (%Ld >= %Ld)", current, dirent.size);
				ret = 1;
				break;
			}

			// Get some data
			ret = tf_cmd_get_next(&tf, &buf);

			if (ret == 1) {
				//fprintf(stderr, "got EOF\n");
				break;
			}
			if (ret != 0) {
				//fprintf(stderr, "got error %d\n", ret);
				/* An error occurred */
				break;
			}

			/* Note: We used to have all sorts of handling here in case buf.offset != current,
			 *       but we never saw that in practice, so just assume we get what we ask for
			 */
			//fprintf(stderr, "got %d bytes @%lld\n", (int)buf.size, buf.offset);

			data = buf.data + skip;
			len = buf.size - skip;
			skip = 0;

#if 0
			/* Don't return data past the length of the file */
			if (current + len > dirent.size) {
				syslog(LOG_INFO, "would pass EOF, %Ld + len=%d > %Ld, so len %d -> %Ld", current, len, dirent.size, len, dirent.size - current);
				len = dirent.size - current;
			}
#endif

			/* Need to write data/len to the client */

			//fprintf(stderr, "Trying to write to %d bytes to the client\n", len);

			while (len) {
				fd_set fds;
				struct timeval to;
				int n;

				/* Set up for the select */
				FD_ZERO(&fds);
				FD_SET(fileno(dout), &fds);

				/* If we are in a get, we only wait 2 seconds for the fd to be writable.
				 * If we have already cancelled the get, though, we can wait an indefinite
				 * amount of time (but if we are interrupted while idle, give up).
				 */
				to.tv_sec = 2;
				to.tv_usec = 0;

				/* Wait for a response */
				n = select(fileno(dout) + 1, 0, &fds, 0, in_get ? &to : 0);
				if (n <= 0) {
					if (debug) {
						if (n == 0) {
							fprintf(stderr, "Timeout waiting to write to client\n");
						}
						else {
							fprintf(stderr, "select returned %d: %m\n", n);
						}
					}
					if (in_get) {
						if (debug) {
							fprintf(stderr, "Client is not ready in 2 seconds, so pausing the get\n");
						}
						tf_cmd_get_cancel(&tf);
						in_get = 0;
						if (turbo) {
							tf_cmd_turbo(&tf, 0);
						}
					}
					else {
						if (debug) {
							fprintf(stderr, "Client was interrupted while idle, so cancelling the get\n");
						}
						aborted = 1;
						ret = -200;
						break;
					}
				}
				else {
					int wrote = write(fileno(dout), data, len);
					if (wrote < 0) {
						//fprintf(stderr, "write() failed: %s\n", strerror(errno));
						ret = -200;
						break;
					}
					//fprintf(stderr, "Wrote %d of %d to the client\n", wrote, len);
					len -= wrote;
					data += wrote;
					current += wrote;
				}
			}
			if (!in_get) {
				/* Need to get out and resume the get */
				//fprintf(stderr, "Done write, but get is paused. Need to resume\n");
				break;
			}
		}

		//fprintf(stderr, "At end of queue read/write loop, in_get=%d, ret=%d\n", in_get, ret);

		if (ret <= 0) {
			/* Cancel anything which may be outstanding */
			if (!in_get) {
				if (debug) {
					fprintf(stderr, "Resuming paused get @%lld %s\n", current, srcpath);
				}
				continue;
			}
			if (tf_cmd_get_cancel(&tf) != 0) {
				syslog(LOG_WARNING, "retrieve: Failed to cancel after ret=%d", ret);

			}
			else if (ret == TF_ERR_IO) {
				if (debug) {
					fprintf(stderr, "Resuming interrupted get @%lld %s\n", (long long)current, srcpath);
				}
				continue;
			}
			if (ret == TF_ERR_IO) {
				syslog(LOG_WARNING, "retrieve: Failed to resume interrupted get");
			}
			else {
				syslog(LOG_WARNING, "retrieve: Cancelled get");
			}
		}

		/* All done */
		break;
	}
	abort_buf_valid = 0;

	/*fprintf(stderr, "Finished loop with ret=%d, in_get=%d, aborted=%d, current @%lld\n", ret, in_get, aborted, (long long)current);*/

	if (in_get) {
		tf_cmd_get_cancel(&tf);
	}
	if (aborted) {
		ret = -1;
	}
	if (dout) {
		fclose(dout);
	}
	if (turbo) {
		tf_cmd_turbo(&tf, 0);
	}

	/*fprintf(stderr, "Finishing up and closing data connection\n");*/
	if (ret >= 0) {
		syslog(LOG_INFO, "retrieve: Completed transfer of %s @%Ld", srcpath, (long long)current);
		reply (226, "Transfer complete.");
	}
	else if (!replied) {
		if (debug) {
			fprintf(stderr, "Transfer failed %lld bytes %s\n", (long long)current, srcpath);
		}
		perror_reply (550, name);
	}

	data = -1;
	pdata = -1;
}

/**
 * Accepts a firmware image (.tfd) file
 * 
 */
static void upgrade_firmware(const char *name)
{
	FILE *din;
	const char *pt = strrchr(name, '.');
	time_t start;
	int old_trace_level;
	unsigned long offset = 0;
	int ret;
	tf_fw_data_t fw_data;
	char *buffer;

	if (!pt || strcmp(pt, ".tfd") != 0) {
		reply(550, "May only store .tfd file to /firmware");
		return;
	}

	/* Looks good. Let's open the data connection and initiate the upgrade upgrade */
	din = dataconn (name, (off_t)-1, "r");
	if (!din) {
		perror_reply (426, "Data Connection");
		return;
	}

	if (tf_cmd_reset(&tf) != 0) {
		perror_reply(550, "Failed to reboot");
		fclose(din);
		return;
	}

	/* Need to close our connection and wait to reopen a connection to the boot loader */
	old_trace_level = tf.trace_level;

	topfield_close(&tf);

	/* Give it a moment to reboot */
	sleep(1);

	start = time(0);

	while (topfield_open(&tf, topfield_device, TF_LOCK_STD) != 0) {
		/* Wait up to 20 seconds to connect after reboot */
		if (time(0) - start > 20) {
			perror_reply(550, "Failed to reconnect after reboot");
			fclose(din);
			exit(1);
		}
		usleep(1000);
	}

	/* Connected to the bootloader OK */

	tf.trace_level = old_trace_level;
	tf.timeout = 4000;

	buffer = malloc(MAX_DATA_SIZE);

	/* Send an ID command */
	ret = tf_fw_upload(&tf, &fw_data);
	while (ret == 0) {
		size_t len;

		if (offset != fw_data.offset) {
			reply(426, "Can't support non-sequential data request! %ld != %ld", offset, fw_data.offset);
			break;
		}

		if (fw_data.len > MAX_DATA_SIZE) {
			reply(426, "Request for too much data (%d) @%ld", fw_data.len, fw_data.offset);
			break;
		}

		len = fread(buffer, 1, fw_data.len, din);
		/*printf("fread() returned %d\n", len);*/
		if (len != fw_data.len) {
			reply(426, "Failed to read %d bytes from stream", fw_data.len);
			break;
		}
		offset += len;

		/*fprintf(stderr, "Before tf_fw_upload_next() offset=%ld\n", offset);*/
		ret = tf_fw_upload_next(&tf, buffer, len, &fw_data);
		/*fprintf(stderr, "After tf_fw_upload_next() ret=%d, offset=%ld, len=%d, fw_data.offset=%lu, fw_data.len=%d\n", ret, offset, len, fw_data.offset, fw_data.len);*/
	}

	if (ret == 1) {
		reply (226, "Firmware upgrade completed successully -- you must reboot");
	}
	else if (ret < 0) {
		perror_reply (550, "Failed to upgrade firmware -- you must reboot");
	}

	fclose(din);

	topfield_close(&tf);

	/* Need to reconnect after reboot to continue */
	exit(0);
}

void
store (const char *name, const char *mode, int unique)
{
	FILE *din = 0;
	off_t current = restart_point;
	int ret;
	int aborted = 0;

	char *userpath = tf_makename(curdir(), name);
	const char *destpath = topfield_dir(userpath);

	int turbo = is_turbo(userpath);

	if (strcmp(destpath, "/firmware") == 0 || strncmp(destpath, "/firmware/", 10) == 0) {
		upgrade_firmware(name);
		return;
	}

	if (unique) {
		reply(550, "STOU is not supported by ftpd-topfield");
		return;
	}

	transflag = 1;

	syslog(LOG_INFO, "store: %s %s @%Ld", turbo ? "(turbo)" : "", destpath, (long long)current);

	/* Do we need to append to an existing file? */
	if (*mode == 'a') {
		tf_dirent dirent;

		/* Append if the file exists */
		if (tf_cmd_get(&tf, destpath, 0, &dirent) == 0) {
			tf_cmd_get_cancel(&tf);
			if (dirent.type == 'f') {
				/* It's a file which exists, so start here */
				current = dirent.size;
			}
		}
	}

	if (current == 0) {
		syslog(LOG_DEBUG, "store: deleting %s", destpath);

		/* Need to delete any existing file since it won't just be truncated */
		tf_cmd_delete(&tf, destpath);
	}

	if (turbo) {
		tf_cmd_turbo(&tf, 1);
	}

resume_put:

	syslog(LOG_INFO, "store: Starting at %s @%Ld", destpath, (long long)current);

	ret = tf_cmd_put(&tf, destpath, 0, time(0), current);

#ifdef DEBUG
	fprintf(stderr, "tf_cmd_put(destpath=%s, restart_point=%lld) = %d\n", destpath, current, ret);
#endif

	if (ret != 0) {
		perror_reply (553, name);

		LOGCMD (*mode == 'w' ? "put" : "append", name);
		if (turbo) {
			tf_cmd_turbo(&tf, 0);
		}
		return;
	}

	if (!din) {
		din = dataconn (name, (off_t)-1, "r");
		if (din == NULL) {
			tf_cmd_put_cancel(&tf);
		}
	}

	if (din && (aborted = setjmp(abort_buf)) == 0)  {
		char buf[MAX_PUT_SIZE];

		abort_buf_valid = 1;

		for (;;) {
			fd_set fds;
			struct timeval to;

			/* Set up for the select */
			FD_ZERO(&fds);
			FD_SET(fileno(din), &fds);

			/* Wait up to 5 seconds for din to be readable.
			 */
			to.tv_sec = 5;
			to.tv_usec = 0;

			/* Wait for a response */
			/*fprintf(stderr, "put: waiting for data\n");*/
			ret = select(fileno(din) + 1, &fds, 0, 0, &to);
			if (ret > 0) {
				int r;

				/*fprintf(stderr, "Input is now readable\n");*/
				ret = read(fileno(din), buf, sizeof(buf));

				/*fprintf(stderr, "put: read %d bytes - sending\n", ret);*/

				if (ret > 0) {
					r = tf_cmd_put_data(&tf, current, buf, ret);

					/*fprintf(stderr, "put: tf_cmd_put_data() returned %d\n", r);*/
					if (r != 0) {
						ret = r;
						break;
					}
					current += ret;
					byte_count += ret;
				}
			}
			if (ret <= 0 && errno != EINTR) {
				break;
			}
		}
		
		if (ret >= 0) {
			/*fprintf(stderr, "put: tf_cmd_put_done()\n");*/
			syslog(LOG_INFO, "store: Completed transfer of %s @%Ld", destpath, (long long)current);
			ret = tf_cmd_put_done(&tf);
			reply (226, "Transfer complete.");
		}
		else {
			/*fprintf(stderr, "put: tf_cmd_put_cancel()\n");*/

			/* Cancel anything which may be outstanding */
			if (tf_cmd_put_cancel(&tf) == 0 && ret == TF_ERR_IO) {
				if (debug) {
					fprintf(stderr, "Resuming interrupted put @%lld %s\n", (long long)current, name);
				}
				goto resume_put;
			}

			if (debug) {
				if (ret == TF_ERR_IO) {
					fprintf(stderr, "Failed to resume interrupted put\n");
				}
				else {
					fprintf(stderr, "Cancelled put\n");
				}
			}

			/* Failed to cancel the put, so give up */
			perror_reply (426, "Data Connection");
		}
		fclose(din);
	}
	abort_buf_valid = 0;
	if (aborted) {
		tf_cmd_put_cancel(&tf);
	}

	if (turbo) {
		tf_cmd_turbo(&tf, 0);
	}

	data = -1;
	pdata = -1;

	LOGBYTES (*mode == 'w' ? "put" : "append", name, byte_count);
}

static FILE *
getdatasock (const char *mode)
{
	int s, t, tries;

	if (data >= 0)
		return fdopen (data, mode);
	(void) seteuid ((uid_t)0);
	s = socket (AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		goto bad;

	/* Enables local reuse address.  */
	{
		int on = 1;
		if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,
				(char *) &on, sizeof(on)) < 0)
			goto bad;
	}

	/* anchor socket to avoid multi-homing problems */
	data_source.sin_family = AF_INET;
	data_source.sin_addr = ctrl_addr.sin_addr;
	for (tries = 1; ; tries++)
		{
			if (bind (s, (struct sockaddr *)&data_source,
		sizeof(data_source)) >= 0)
	break;
			if (errno != EADDRINUSE || tries > 10)
	goto bad;
			sleep (tries);
		}
	(void) seteuid ((uid_t)cred.uid);

#if defined (IP_TOS) && defined (IPTOS_THROUGHPUT) && defined (IPPROTO_IP)
	{
		int on = IPTOS_THROUGHPUT;
		if (setsockopt(s, IPPROTO_IP, IP_TOS, (char *)&on, sizeof(int)) < 0)
			syslog(LOG_WARNING, "setsockopt (IP_TOS): %m");
	}
#endif

	return (fdopen(s, mode));
 bad:
	/* Return the real value of errno (close may change it) */
	t = errno;
	(void) seteuid ((uid_t)cred.uid);
	(void) close(s);
	errno = t;
	return NULL;
}

static FILE *
dataconn (const char *name, off_t size, const char *mode)
{
	char sizebuf[32];
	FILE *file;
	int retry = 0;

	file_size = size;
	byte_count = 0;
	if (size != (off_t) -1)
		(void) snprintf(sizebuf, sizeof(sizebuf), " (%Ld bytes)",
				(long long)size);
	else
		*sizebuf = '\0';
	if (pdata >= 0)
		{
			struct sockaddr_in from;
			int s;
			socklen_t fromlen = sizeof (from);

			(void) signal (SIGALRM, toolong);
			(void) alarm ((unsigned) timeout);
			s = accept (pdata, (struct sockaddr *)&from, &fromlen);
			(void) alarm (0);
			if (s < 0)
	{
		reply(425, "Can't open data connection.");
		(void) close (pdata);
		pdata = -1;
		return NULL;
	}
			(void) close (pdata);
			pdata = s;
#if defined (IP_TOS) && defined (IPTOS_THROUGHPUT) && defined (IPPROTO_IP)
			/* Optimize throughput.  */
			{
	int tos = IPTOS_THROUGHPUT;
	(void) setsockopt (s, IPPROTO_IP, IP_TOS, (char *)&tos, sizeof (int));
			}
#endif
#ifdef SO_KEEPALIVE
			/* Set keepalives on the socket to detect dropped conns.  */
			{
	int keepalive = 1;
	(void) setsockopt (s, SOL_SOCKET, SO_KEEPALIVE,
				 (char *)&keepalive, sizeof (int));
			}
#endif
			reply (150, "Opening %s mode data connection for '%s'%s.",
			 type == TYPE_A ? "ASCII" : "BINARY", name, sizebuf);
			return fdopen (pdata, mode);
		}
	if (data >= 0)
		{
			reply (125, "Using existing data connection for '%s'%s.",
			 name, sizebuf);
			usedefault = 1;
			return fdopen (data, mode);
		}
	if (usedefault)
		data_dest = his_addr;
	usedefault = 1;
	file = getdatasock (mode);
	if (file == NULL)
		{
			reply (425, "Can't create data socket (%s,%d): %s.",
			 inet_ntoa (data_source.sin_addr),
			 ntohs (data_source.sin_port), strerror(errno));
			return NULL;
		}
	data = fileno (file);
	while (connect (data, (struct sockaddr *)&data_dest,
			sizeof (data_dest)) < 0)
		{
			if (errno == EADDRINUSE && retry < swaitmax)
	{
		sleep ((unsigned) swaitint);
		retry += swaitint;
		continue;
	}
			perror_reply (425, "Can't build data connection");
			(void) fclose (file);
			data = -1;
			return NULL;
		}
	reply (150, "Opening %s mode data connection for '%s'%s.",
	 type == TYPE_A ? "ASCII" : "BINARY", name, sizebuf);
	return file;
}

static void dump_dirent(FILE *fh, const char *filename, const tf_dirent *dirent)
{
	const char *mode = (dirent->type == 'f') ? "-rw-r--r--" : "drwxr-xr-x";
	struct tm *tm = localtime(&dirent->stamp);
	char timebuf[24];
	time_t now = time(0);

	/* Shall we send just the date, or the date and time? Do what ls does and
	 * send the date and time if the file is less than 1 year old
	 */
	if (now - dirent->stamp >= 365 * 24 * 3600) {
		strftime(timebuf, sizeof(timebuf), "%b %e  %Y", tm);
	}
	else {
		strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);
	}

	fprintf(fh, "%s   1 none     none     %12llu %s %s", mode, (long long)dirent->size, timebuf, filename ?: dirent->name);
}

/*
 * From lftp source
 
+i774.71425,m951188401,/,	users
+i774.49602,m917883130,r,s79126,	jgr_www2.exe

starts with +
comma separated
first character of field is type:
 i - ?
 m - modification time
 / - means directory
 r - means plain file
 s - size
 up - permissions in octal
 \t - file name follows.
*/

/**
 * Dump in EPLF format.
 */
static void dump_dirent_as_eplf(FILE *fh, const char *filename, const tf_dirent *dirent)
{
	fprintf(fh, "+");

	if (dirent->stamp) {
		fprintf(fh, "m%lu,", (unsigned long)dirent->stamp);
	}

	if (dirent->type == 'f') {
		fprintf(fh, "r,s%llu,\t%s\r\n", (long long)dirent->size, dirent->name);
	}
	else {
		fprintf(fh, "/,\t%s\r\n", dirent->name);
	}
}

void
statfilecmd (const char *path)
{
	const char *fullpath = topfield_dir(tf_makename(curdir(), path));
	tf_dirent dirent;

	lreply (211, "status of %s:", path);

	if (tf_stat(&tf, fullpath, &dirent) != 0) {
		printf("No such file: %s\r\n", path);
	}
	else {
		dump_dirent(stdout, path, &dirent);
		printf("\r\n");
	}
	reply (211, "End of Status");
}

void
statcmd (void)
{
	struct sockaddr_in *sin;
	u_char *a, *p;
	tf_size_result result;

	lreply (211, "%s FTP server status:", hostname);
	if (!no_version)
		printf ("     ftpd (%s) %s\r\n",
			PACKAGE_NAME, PACKAGE_VERSION);
#if defined(__DATE__) && defined(__TIME__)
	printf ("     Compiled: " __TIME__ ", " __DATE__ "\r\n");
#endif
	printf ("     Connected to %s", cred.remotehost);
	if (!isdigit (cred.remotehost[0]))
		printf (" (%s)", inet_ntoa (his_addr.sin_addr));
	printf ("\r\n");
	if (cred.logged_in)
		{
			if (cred.guest)
	printf ("     Logged in anonymously\r\n");
			else
	printf ("     Logged in as %s\r\n", cred.name);
		}
	else if (askpasswd)
		printf ("     Waiting for password\r\n");
	else
		printf ("     Waiting for user name\r\n");

	if (tf_cmd_size(&tf, &result) == 0) {
		printf("     HDD Free  %10u kiB %7u MiB %4u GiB\r\n", result.freek, result.freek / 1024,
			   result.freek / (1024 * 1024));
		printf("     HDD Total %10u kiB %7u MiB %4u GiB\r\n", result.totalk, result.totalk / 1024,
			   result.totalk / (1024 * 1024));
	}
	else {
		printf ("     Unable to get HDD size info\r\n");
	}
	printf ("     Turbo mode is %s\r\n", global_turbo_mode ? "ENABLED" : "DISABLED");
	printf ("     TYPE: %s", typenames[type]);
	if (type == TYPE_A || type == TYPE_E)
		printf (", FORM: %s", formnames[form]);
	if (type == TYPE_L)
#ifdef CHAR_BIT
		printf (" %d", CHAR_BIT);
#else
#if NBBY == 8
	printf (" %d", NBBY);
#else
	printf (" %d", bytesize);	/* need definition! */
#endif
#endif
	printf ("; STRUcture: %s; transfer MODE: %s\r\n",
		strunames[stru], modenames[stru_mode]);
	if (data != -1)
		printf ("     Data connection open\r\n");
	else if (pdata != -1)
		{
			printf ("     in Passive mode");
			sin = &pasv_addr;
			goto printaddr;
		}
	else if (usedefault == 0)
		{
			printf ("     PORT");
			sin = &data_dest;
		printaddr:
			a = (u_char *) &sin->sin_addr;
			p = (u_char *) &sin->sin_port;
#define UC(b) (((int) b) & 0xff)
			printf (" (%d,%d,%d,%d,%d,%d)\r\n", UC(a[0]),
				UC(a[1]), UC(a[2]), UC(a[3]), UC(p[0]), UC(p[1]));
#undef UC
		}
	else
		printf ("     No data connection\r\n");
	reply (211, "End of status");
}

void
fatal (const char *s)
{
	reply (451, "Error in server: %s\n", s);
	reply (221, "Closing connection due to server error.");
	dologout (0);
	/* NOTREACHED */
}

void
reply (int n, const char *fmt, ...)
{
	va_list ap;
#if defined(HAVE_STDARG_H) && defined(__STDC__) && __STDC__
	va_start (ap, fmt);
#else
	va_start (ap);
#endif
	(void)printf ("%d ", n);
	(void)vprintf (fmt, ap);
	(void)printf ("\r\n");
	(void)fflush (stdout);
	if (debug)
		{
			syslog (LOG_DEBUG, "<--- %d ", n);
#ifdef HAVE_VSYSLOG
			vsyslog (LOG_DEBUG, fmt, ap);
#endif
		}
}

void
lreply (int n, const char *fmt, ...)
{
	va_list ap;
#if defined(HAVE_STDARG_H) && defined(__STDC__) && __STDC__
	va_start (ap, fmt);
#else
	va_start (ap);
#endif
	(void)printf ("%d- ", n);
	(void)vprintf (fmt, ap);
	(void)printf ("\r\n");
	(void)fflush (stdout);
	if (debug)
		{
			syslog (LOG_DEBUG, "<--- %d- ", n);
#ifdef HAVE_VSYSLOG
			vsyslog (LOG_DEBUG, fmt, ap);
#endif
		}
}

static void
ack (const char *s)
{
	reply (250, "%s command successful.", s);
}

void
nack (const char *s)
{
	reply (502, "%s command not implemented.", s);
}

void
delete (const char *path)
{
	const char *fullpath = topfield_dir(tf_makename(curdir(), path));

	LOGCMD ("delete", path);

	tf_cmd_delete(&tf, fullpath);

	if (tf.error) {
		reply(550, "%s: %s.", path, tf_strerror(tf.error));
	}
	else {
		ack ("DELE");
	}
}

void
makedir (const char *path)
{
	const char *fullpath = topfield_dir(tf_makename(curdir(), path));

	LOGCMD ("mkdir", path);

	tf_cmd_mkdir(&tf, fullpath);

	if (tf.error) {
		perror_reply (550, path);
	}
	else {
		reply (257, "\"%s\" new directory created.", fullpath);
	}
}

void
removedir (const char *path)
{
	const char *fullpath = topfield_dir(tf_makename(curdir(), path));

	LOGCMD ("rmdir", path);

	tf_cmd_delete(&tf, fullpath);

	if (tf.error) {
		perror_reply(550, path);
	}
	else {
		ack("RMD");
	}
}

void
pwd (void)
{
		reply (257, "\"%s\" is current directory.", curdir());
}

char *
renamefrom (const char *path)
{
	tf_dirent dirent;

	const char *fullpath = topfield_dir(tf_makename(curdir(), path));

	if (tf_stat(&tf, fullpath, &dirent) != 0) {
			reply (550, path);
			return 0;
	}
	reply (350, "File exists, ready for destination name");
	return (char *)(path);
}

void
renamecmd (const char *from, const char *to)
{
	LOGCMD2 ("rename", from, to);

	char *frompath = strdup(topfield_dir(tf_makename(curdir(), from)));
	char *topath = strdup(topfield_dir(tf_makename(curdir(), to)));

	/*fprintf(stderr, "tf_cmd_rename: frompath=%s, topath=%s\n", frompath, topath);*/
	if (tf_cmd_rename (&tf, frompath, topath) != 0)
		perror_reply (550, "rename");
	else {
		ack ("RNTO");
	}

	free(topath);
	free(frompath);
}

static void
dolog (struct sockaddr_in *sin, struct credentials *pcred)
{
	const char *name = 0;

	if (!no_dns) {
		struct hostent *hp = gethostbyaddr ((char *)&sin->sin_addr,
								sizeof (struct in_addr), AF_INET);

		if (hp)
			name = hp->h_name;
	}
	if (!name) {
		name = inet_ntoa (sin->sin_addr);
	}

	if (pcred->remotehost)
		free (pcred->remotehost);
	pcred->remotehost = sgetsave (name);

#ifdef HAVE_SETPROCTITLE
	snprintf (proctitle, sizeof (proctitle), "%s: connected", pcred->remotehost);
	setproctitle ("%s",proctitle);
#endif /* HAVE_SETPROCTITLE */

	if (logging)
		syslog (LOG_INFO, "connection from %s", pcred->remotehost);
}

/*  Record logout in wtmp file
		and exit with supplied status.  */
void
dologout (int status)
{
	/* Racing condition with SIGURG: If SIGURG is receive
		 here, it will jump back has root in the main loop
		 David Greenman:dg@root.com.  */
	transflag = 0;

	close_topfield();

	/* beware of flushing buffers after a SIGPIPE */
	_exit (status);
}

static void
myoob (int signo)
{
	char *cp;

	(void)signo;

	syslog(LOG_DEBUG, "Got oob signal SIGURG, transflag=%d", transflag);

	/* only process if transfer occurring */
	if (!transflag)
		return;
	cp = tmpline;
	if (telnet_fgets (cp, 7, stdin) == NULL)
		{
			reply (221, "You could at least say goodbye.");
			dologout (0);
		}
	upper (cp);
	if (strcmp (cp, "ABOR\r\n") == 0)
		{
			tmpline[0] = '\0';
			reply (426, "Transfer aborted. Data connection closed.");
			reply (226, "Abort successful");
			abort_transfer(0);
		}
	if (strcmp (cp, "STAT\r\n") == 0)
		{
			if (file_size != (off_t) -1)
	reply (213, "Status: %Ld of %Ld bytes transferred",
				 (long long)byte_count, (long long)file_size);
			else
	reply (213, "Status: %Ld bytes transferred",
				 (long long)byte_count);
		}
}

/* Note: a response of 425 is not mentioned as a possible response to
	 the PASV command in RFC959. However, it has been blessed as
	 a legitimate response by Jon Postel in a telephone conversation
	 with Rick Adams on 25 Jan 89.  */
void
passive (int extended)
{
	socklen_t len;
	char *p, *a;

	pdata = socket (AF_INET, SOCK_STREAM, 0);
	if (pdata < 0)
		{
			perror_reply (425, "Can't open passive connection");
			return;
		}
	pasv_addr = ctrl_addr;
	pasv_addr.sin_port = 0;
	(void) seteuid ((uid_t)0);
	if (bind (pdata, (struct sockaddr *)&pasv_addr, sizeof (pasv_addr)) < 0)
		{
			(void) seteuid ((uid_t)cred.uid);
			goto pasv_error;
		}
	(void) seteuid ((uid_t)cred.uid);
	len = sizeof(pasv_addr);
	if (getsockname (pdata, (struct sockaddr *) &pasv_addr, &len) < 0)
		goto pasv_error;
	if (listen (pdata, 1) < 0)
		goto pasv_error;
	a = (char *) &pasv_addr.sin_addr;
	p = (char *) &pasv_addr.sin_port;

#define UC(b) (((int) b) & 0xff)

	if (extended) {
		reply (229, "Entering Extended Passive Mode (|||%d|)", ntohs(pasv_addr.sin_port));
	}
	else {
		reply (227, "Entering Passive Mode (%d,%d,%d,%d,%d,%d)", UC(a[0]),
		 UC(a[1]), UC(a[2]), UC(a[3]), UC(p[0]), UC(p[1]));
	}
	return;

 pasv_error:
	(void) close (pdata);
	pdata = -1;
	perror_reply (425, "Can't open passive connection");
	return;
}

/*
 * Format and send reply containing system error number.
 */
void
perror_reply (int code, const char *string)
{
	if (tf.error) {
		reply (code, "%s: %s.", string, tf_strerror (tf.error));
	}
	else {
		reply (code, "%s", string);
	}
}

static void list_one_entry(const tf_dirent *dirent, FILE *fh, int list, int use_eplf)
{
	if (list) {
		if (use_eplf) {
			dump_dirent_as_eplf(fh, 0, dirent);
		}
		else {
			dump_dirent(fh, 0, dirent);
			fprintf(fh, "\r\n");
		}
	}
	else {
		fprintf(fh, "%s\r\n", dirent->name);
	}
}

void
send_file_list (const char *whichf, int LIST)
{
	tf_dir_entries entries;
	tf_dirent dirent;
	const char *pattern = 0;
	int exact = 0;
	FILE *dout = 0;
	int count = 0;
	int sort_type = TF_SORT_NAME;
	int ret;
	char *fullname;

	/* Now whichf may be a directory name, a file name, a wildcard pattern or NULL or empty
	 * And it may be one of the above with an initial sort specification
	 * such as -S, -tr, etc.
	 */

	pattern = whichf;

#ifdef DEBUG
	syslog(LOG_INFO, "send_file_list(whichf=%s), LIST=%d", whichf, LIST);
#endif

	if (pattern && *pattern == '-') {
		int reverse = 0;

		pattern++;
		while (*pattern && *pattern != ' ') {
			switch (*pattern) {
				case 't':
				case 'u':
				case 'c':
					sort_type = TF_SORT_TIME;
					break;

				case 'S':
					sort_type = TF_SORT_SIZE;
					break;

				case 'r':
					reverse = 1;
					break;
			}
			pattern++;
		}

		if (reverse) {
			sort_type = -sort_type;
		}

		while (*pattern == ' ') {
			pattern++;
		}
	}

#ifdef DEBUG
	syslog(LOG_INFO, "send_file_list: pattern=%s, sort_type=%d", pattern, sort_type);
#endif

	if (strcmp(topfield_dir(curdir()), "/firmware") == 0) {
		dout = dataconn("file list", (off_t)-1, "w");
		if (dout) {
			fclose(dout);
			reply(226, "No files in virtual firmware directory");
		}
		else {
			perror_reply (550, "Data connection");
		}
		return;
	}
	
	/* See if the specified filename exists */
	fullname = tf_makename(topfield_dir(curdir()), pattern);

	if (pattern && *pattern) {
		/* If a parameter was given, what was it? */
		if (tf_stat(&tf, fullname, &dirent) == 0) {
			/* Exact file match, so list this one file or directory */
			exact = 1;
			fullname = tf_makename(topfield_dir(curdir()), 0);
		}
		else {
			/* Does not exist, so treat whichf as a pattern */
			fullname = tf_makename(topfield_dir(curdir()), 0);
		}
	}
	else {
#ifdef DEBUG
		syslog(LOG_INFO, "send_file_list: no pattern, so looking in dir %s", fullname);
#endif
	}

	ret = tf_cmd_dir_first(&tf, fullname, &entries);

	if (ret < 0) {
		reply(550, "%s: %s.", whichf, tf_strerror(ret));
		return;
	}

	if (entries.count > 0) {
		dout = dataconn("file list", (off_t)-1, "w");
		if (dout != NULL) {
			while (ret == 0) {
				int i;

				if (LIST) {
					tf_sort_dirents(&entries, sort_type);
				}

#ifdef DEBUG
				syslog(LOG_INFO, "send_file_list: examining %d entries with exact=%d and pattern=%s", entries.count, exact, pattern);
#endif

				for (i = 0; i < entries.count; i++) {
					if (pattern && *pattern) {
						if (exact && strcmp(pattern, entries.entry[i].name) != 0) {
							continue;
						}
						if (!exact && fnmatch(pattern, entries.entry[i].name, 0) != 0) {
							continue;
						}
					}
					count++;
					list_one_entry(&entries.entry[i], dout, LIST, use_eplf);
				}
				ret = tf_cmd_dir_next(&tf, &entries);
			}
		}
	}
	if (count == 0) {
		reply (550, "No files found.");
	}
	else if (ferror (dout) != 0) {
		perror_reply (550, "Data connection");
	}
	else {
		/* If at the top level, fake a 'firmware' directory */
		if (strcmp(fullname, "/") == 0 && (!pattern || !*pattern)) {
			tf_dirent d;

			d.stamp = 0;
			d.type = 'd';
			d.size = 0;
			strcpy(d.name, "firmware");
			d.attrib = 0;

			list_one_entry(&d, dout, LIST, use_eplf);

			if (strcmp(curdir(), topfield_dir(curdir())) == 0) {
				d.stamp = 0;
				d.type = 'd';
				d.size = 0;
				strcpy(d.name, "turbo");
				d.attrib = 0;

				list_one_entry(&d, dout, LIST, use_eplf);
			}
		}
		reply (226, "Transfer complete.");
	}

	if (dout != NULL) {
		fclose (dout);
	}
	data = -1;
	pdata = -1;
}
