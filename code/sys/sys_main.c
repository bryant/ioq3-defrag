/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifndef DEDICATED
#include "SDL.h"
#endif

#include "sys_local.h"
#include "sys_loadlib.h"

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

static char binaryPath[ MAX_OSPATH ];
static char installPath[ MAX_OSPATH ];

/*
=================
Sys_SetBinaryPath
=================
*/
void Sys_SetBinaryPath(const char *path)
{
	Q_strncpyz(binaryPath, path, sizeof(binaryPath));
}

/*
=================
Sys_BinaryPath
=================
*/
char *Sys_BinaryPath(void)
{
	return binaryPath;
}

/*
=================
Sys_SetDefaultInstallPath
=================
*/
void Sys_SetDefaultInstallPath(const char *path)
{
	Q_strncpyz(installPath, path, sizeof(installPath));
}

/*
=================
Sys_DefaultInstallPath
=================
*/
char *Sys_DefaultInstallPath(void)
{
	if (*installPath)
		return installPath;
	else
		return Sys_Cwd();
}

/*
=================
Sys_In_Restart_f

Restart the input subsystem
=================
*/
void Sys_In_Restart_f( void )
{
	IN_Shutdown();
	IN_Init();
}

/*
=================
Sys_ConsoleInputInit

Start the console input subsystem
=================
*/
void Sys_ConsoleInputInit( void )
{
#ifdef DEDICATED
	TTY_Init( );
#endif
}

/*
=================
Sys_ConsoleInputShutdown

Shutdown the console input subsystem
=================
*/
void Sys_ConsoleInputShutdown( void )
{
#ifdef DEDICATED
	TTY_Shutdown( );
#endif
}

/*
=================
Sys_ConsoleInput

Handle new console input
=================
*/
char *Sys_ConsoleInput(void)
{
#ifdef DEDICATED
	return TTY_ConsoleInput( );
#endif

	return NULL;
}

/*
=================
Sys_Exit

Single exit point (regular exit or in case of error)
=================
*/
void Sys_Exit( int ex )
{
	Sys_ConsoleInputShutdown();

#ifndef DEDICATED
	SDL_Quit( );
#endif

#ifdef NDEBUG
	// _exit is called instead of exit since there are rumours of
	// GL libraries installing atexit calls that we don't want to call
	// FIXME: get some testing done with plain exit
	_exit(ex);
#else
	// Cause a backtrace on error exits
	assert( ex == 0 );
	exit(ex);
#endif
}

/*
=================
Sys_Quit
=================
*/
void Sys_Quit (void)
{
	CL_Shutdown ();
	Sys_Exit(0);
}

/*
=================
Sys_GetProcessorFeatures
=================
*/
cpuFeatures_t Sys_GetProcessorFeatures( void )
{
	cpuFeatures_t features = 0;

#ifndef DEDICATED
	if( SDL_HasRDTSC( ) )    features |= CF_RDTSC;
	if( SDL_HasMMX( ) )      features |= CF_MMX;
	if( SDL_HasMMXExt( ) )   features |= CF_MMX_EXT;
	if( SDL_Has3DNow( ) )    features |= CF_3DNOW;
	if( SDL_Has3DNowExt( ) ) features |= CF_3DNOW_EXT;
	if( SDL_HasSSE( ) )      features |= CF_SSE;
	if( SDL_HasSSE2( ) )     features |= CF_SSE2;
	if( SDL_HasAltiVec( ) )  features |= CF_ALTIVEC;
#endif

	return features;
}

/*
=================
Sys_Init
=================
*/
void Sys_Init(void)
{
	Cmd_AddCommand( "in_restart", Sys_In_Restart_f );
	Cvar_Set( "arch", OS_STRING " " ARCH_STRING );
	Cvar_Set( "username", Sys_GetCurrentUser( ) );
}

static struct Q3ToAnsiColorTable_s
{
	char Q3color;
	char *ANSIcolor;
} TTY_colorTable[ ] =
{
	{ COLOR_BLACK,    "30" },
	{ COLOR_RED,      "31" },
	{ COLOR_GREEN,    "32" },
	{ COLOR_YELLOW,   "33" },
	{ COLOR_BLUE,     "34" },
	{ COLOR_CYAN,     "36" },
	{ COLOR_MAGENTA,  "35" },
	{ COLOR_WHITE,    "0" }
};

static int TTY_colorTableSize =
	sizeof( TTY_colorTable ) / sizeof( TTY_colorTable[ 0 ] );

/*
=================
Sys_ANSIColorify

Transform Q3 colour codes to ANSI escape sequences
=================
*/
static void Sys_ANSIColorify( const char *msg, char *buffer, int bufferSize )
{
	int   msgLength, pos;
	int   i, j;
	char  *escapeCode;
	char  tempBuffer[ 7 ];

	if( !msg || !buffer )
		return;

	msgLength = strlen( msg );
	pos = 0;
	i = 0;
	buffer[ 0 ] = '\0';

	while( i < msgLength )
	{
		if( msg[ i ] == '\n' )
		{
			Com_sprintf( tempBuffer, 7, "%c[0m\n", 0x1B );
			strncat( buffer, tempBuffer, bufferSize );
			i++;
		}
		else if( msg[ i ] == Q_COLOR_ESCAPE )
		{
			i++;

			if( i < msgLength )
			{
				escapeCode = NULL;
				for( j = 0; j < TTY_colorTableSize; j++ )
				{
					if( msg[ i ] == TTY_colorTable[ j ].Q3color )
					{
						escapeCode = TTY_colorTable[ j ].ANSIcolor;
						break;
					}
				}

				if( escapeCode )
				{
					Com_sprintf( tempBuffer, 7, "%c[%sm", 0x1B, escapeCode );
					strncat( buffer, tempBuffer, bufferSize );
				}

				i++;
			}
		}
		else
		{
			Com_sprintf( tempBuffer, 7, "%c", msg[ i++ ] );
			strncat( buffer, tempBuffer, bufferSize );
		}
	}
}

/*
=================
Sys_Print
=================
*/
void Sys_Print( const char *msg )
{
#ifdef DEDICATED
	TTY_Hide();
#endif

	if( Cvar_VariableIntegerValue( "com_ansicolor" ) )
	{
		char ansiColorString[ MAXPRINTMSG ];
		Sys_ANSIColorify( msg, ansiColorString, MAXPRINTMSG );
		fputs( ansiColorString, stderr );
	}
	else
		fputs(msg, stderr);

#ifdef DEDICATED
	TTY_Show();
#endif
}

/*
=================
Sys_Error
=================
*/
void Sys_Error( const char *error, ... )
{
	va_list argptr;
	char    string[1024];

#ifdef DEDICATED
	TTY_Hide();
#endif

	CL_Shutdown ();

	va_start (argptr,error);
	Q_vsnprintf (string, sizeof(string), error, argptr);
	va_end (argptr);
	fprintf(stderr, "Sys_Error: %s\n", string);

	Sys_Exit( 1 );
}

/*
=================
Sys_Warn
=================
*/
void Sys_Warn( char *warning, ... )
{
	va_list argptr;
	char    string[1024];

	va_start (argptr,warning);
	Q_vsnprintf (string, sizeof(string), warning, argptr);
	va_end (argptr);

#ifdef DEDICATED
	TTY_Hide();
#endif

	fprintf(stderr, "Warning: %s", string);

#ifdef DEDICATED
	TTY_Show();
#endif
}

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int Sys_FileTime( char *path )
{
	struct stat buf;

	if (stat (path,&buf) == -1)
		return -1;

	return buf.st_mtime;
}

/*
=================
Sys_UnloadDll
=================
*/
void Sys_UnloadDll( void *dllHandle )
{
	if( !dllHandle )
	{
		Com_Printf("Sys_UnloadDll(NULL)\n");
		return;
	}

	Sys_UnloadLibrary(dllHandle);
}

/*
=================
Sys_TryLibraryLoad
=================
*/
static void* Sys_TryLibraryLoad(const char* base, const char* gamedir, const char* fname, char* fqpath )
{
	void* libHandle;
	char* fn;

	*fqpath = 0;

	fn = FS_BuildOSPath( base, gamedir, fname );
	Com_Printf( "Sys_LoadDll(%s)... \n", fn );

	libHandle = Sys_LoadLibrary(fn);

	if(!libHandle) {
		Com_Printf( "Sys_LoadDll(%s) failed:\n\"%s\"\n", fn, Sys_LibraryError() );
		return NULL;
	}

	Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn );
	Q_strncpyz ( fqpath , fn , MAX_QPATH ) ;

	return libHandle;
}

/*
=================
Sys_LoadDll

Used to load a development dll instead of a virtual machine
#1 look down current path
#2 look in fs_homepath
#3 look in fs_basepath
=================
*/
void *Sys_LoadDll( const char *name, char *fqpath ,
	intptr_t (**entryPoint)(int, ...),
	intptr_t (*systemcalls)(intptr_t, ...) )
{
	void  *libHandle;
	void  (*dllEntry)( intptr_t (*syscallptr)(intptr_t, ...) );
	char  curpath[MAX_OSPATH];
	char  fname[MAX_OSPATH];
	char  *basepath;
	char  *homepath;
	char  *pwdpath;
	char  *gamedir;

	assert( name );

	getcwd(curpath, sizeof(curpath));
	snprintf (fname, sizeof(fname), "%s" ARCH_STRING DLL_EXT, name);

	// TODO: use fs_searchpaths from files.c
	pwdpath = Sys_Cwd();
	basepath = Cvar_VariableString( "fs_basepath" );
	homepath = Cvar_VariableString( "fs_homepath" );
	gamedir = Cvar_VariableString( "fs_game" );

	libHandle = Sys_TryLibraryLoad(pwdpath, gamedir, fname, fqpath);

	if(!libHandle && homepath)
		libHandle = Sys_TryLibraryLoad(homepath, gamedir, fname, fqpath);

	if(!libHandle && basepath)
		libHandle = Sys_TryLibraryLoad(basepath, gamedir, fname, fqpath);

	if(!libHandle) {
		Com_Printf ( "Sys_LoadDll(%s) failed to load library\n", name );
		return NULL;
	}

	dllEntry = Sys_LoadFunction( libHandle, "dllEntry" );
	*entryPoint = Sys_LoadFunction( libHandle, "vmMain" );

	if ( !*entryPoint || !dllEntry )
	{
		Com_Printf ( "Sys_LoadDll(%s) failed to find vmMain function:\n\"%s\" !\n", name, Sys_LibraryError( ) );
		Sys_UnloadLibrary(libHandle);

		return NULL;
	}

	Com_Printf ( "Sys_LoadDll(%s) found vmMain function at %p\n", name, *entryPoint );
	dllEntry( systemcalls );

	return libHandle;
}

/*
========================================================================

EVENT LOOP

========================================================================
*/

#define MAX_QUED_EVENTS  256
#define MASK_QUED_EVENTS ( MAX_QUED_EVENTS - 1 )

sysEvent_t  eventQue[ MAX_QUED_EVENTS ];
int         eventHead = 0;
int         eventTail = 0;
byte        sys_packetReceived[ MAX_MSGLEN ];

/*
================
Sys_QueEvent

A time of 0 will get the current time
Ptr should either be null, or point to a block of data that can
be freed by the game later.
================
*/
void Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr )
{
	sysEvent_t  *ev;

	ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

	if ( eventHead - eventTail >= MAX_QUED_EVENTS )
	{
		Com_Printf("Sys_QueEvent: overflow\n");
		// we are discarding an event, but don't leak memory
		if ( ev->evPtr )
		{
			Z_Free( ev->evPtr );
		}
		eventTail++;
	}

	eventHead++;

	if ( time == 0 )
	{
		time = Sys_Milliseconds();
	}

	ev->evTime = time;
	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
}

/*
================
Sys_GetEvent

================
*/
sysEvent_t Sys_GetEvent( void )
{
	sysEvent_t  ev;
	char        *s;
	msg_t       netmsg;
	netadr_t    adr;

	// return if we have data
	if ( eventHead > eventTail )
	{
		eventTail++;
		return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}

	// pump the message loop
	// in vga this calls KBD_Update, under X, it calls GetEvent
	Sys_SendKeyEvents ();

	// check for console commands
	s = Sys_ConsoleInput();
	if ( s )
	{
		char  *b;
		int   len;

		len = strlen( s ) + 1;
		b = Z_Malloc( len );
		strcpy( b, s );
		Sys_QueEvent( 0, SE_CONSOLE, 0, 0, len, b );
	}

	// check for other input devices
	IN_Frame();

	// check for network packets
	MSG_Init( &netmsg, sys_packetReceived, sizeof( sys_packetReceived ) );
	if ( Sys_GetPacket ( &adr, &netmsg ) )
	{
		netadr_t  *buf;
		int       len;

		// copy out to a seperate buffer for qeueing
		len = sizeof( netadr_t ) + netmsg.cursize;
		buf = Z_Malloc( len );
		*buf = adr;
		memcpy( buf+1, netmsg.data, netmsg.cursize );
		Sys_QueEvent( 0, SE_PACKET, 0, 0, len, buf );
	}

	// return if we have data
	if ( eventHead > eventTail )
	{
		eventTail++;
		return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}

	// create an empty event to return

	memset( &ev, 0, sizeof( ev ) );
	ev.evTime = Sys_Milliseconds();

	return ev;
}

/*
=================
Sys_ParseArgs
=================
*/
void Sys_ParseArgs( int argc, char **argv )
{
	if( argc == 2 )
	{
		if( !strcmp( argv[1], "--version" ) ||
				!strcmp( argv[1], "-v" ) )
		{
			const char* date = __DATE__;
#ifdef DEDICATED
			fprintf( stdout, Q3_VERSION " dedicated server (%s)\n", date );
#else
			fprintf( stdout, Q3_VERSION " client (%s)\n", date );
#endif
			Sys_Exit(0);
		}
	}
}

#ifndef DEFAULT_BASEDIR
# ifdef MACOS_X
/*
=================
Sys_EscapeAppBundle

Discovers if passed dir is suffixed with the directory structure of a Mac OS X
.app bundle. If it is, the .app directory structure is stripped off the end and
the result is returned. If not, dir is returned untouched.
=================
*/
char *Sys_StripAppBundle( char *dir )
{
	static char cwd[MAX_OSPATH];

	Q_strncpyz(cwd, dir, sizeof(cwd));
	if(strcmp(basename(cwd), "MacOS"))
		return dir;
	Q_strncpyz(cwd, dirname(cwd), sizeof(cwd));
	if(strcmp(basename(cwd), "Contents"))
		return dir;
	Q_strncpyz(cwd, dirname(cwd), sizeof(cwd));
	if(!strstr(basename(cwd), ".app"))
		return dir;
	Q_strncpyz(cwd, dirname(cwd), sizeof(cwd));
	return cwd;
}
		// if run from an .app bundle, we want to also search its containing dir
#		define DEFAULT_BASEDIR Sys_StripAppBundle(Sys_BinaryPath())
#	else
#		define DEFAULT_BASEDIR Sys_BinaryPath()
#	endif
#endif

/*
=================
Sys_SigHandler
=================
*/
static void Sys_SigHandler( int signal )
{
	static qboolean signalcaught = qfalse;

	if( signalcaught )
	{
		printf( "DOUBLE SIGNAL FAULT: Received signal %d, exiting...\n", signal );
	}
	else
	{
		signalcaught = qtrue;
		printf( "Received signal %d, exiting...\n", signal );
#ifndef DEDICATED
		CL_Shutdown();
#endif
		SV_Shutdown( "Signal caught" );
	}

	Sys_Exit( 0 ); // Exit with 0 to avoid recursive signals
}

/*
=================
main
=================
*/
int main( int argc, char **argv )
{
	int   i;
	char  commandLine[ MAX_STRING_CHARS ] = { 0 };

	Sys_ParseArgs( argc, argv );
	Sys_SetBinaryPath( Sys_Dirname( argv[ 0 ] ) );
	Sys_SetDefaultInstallPath( DEFAULT_BASEDIR );

	// Concatenate the command line for passing to Com_Init
	for( i = 1; i < argc; i++ )
	{
		Q_strcat( commandLine, sizeof( commandLine ), argv[ i ] );
		Q_strcat( commandLine, sizeof( commandLine ), " " );
	}

	// Clear queues
	memset( &eventQue[ 0 ], 0, MAX_QUED_EVENTS * sizeof( sysEvent_t ) );
	memset( &sys_packetReceived[ 0 ], 0, MAX_MSGLEN * sizeof( byte ) );

	Com_Init( commandLine );
	NET_Init();

	Sys_ConsoleInputInit();

#ifndef _WIN32
	// Windows doesn't have these signals
	signal( SIGHUP, Sys_SigHandler );
	signal( SIGQUIT, Sys_SigHandler );
	signal( SIGTRAP, Sys_SigHandler );
	signal( SIGIOT, Sys_SigHandler );
	signal( SIGBUS, Sys_SigHandler );
#endif

	signal( SIGILL, Sys_SigHandler );
	signal( SIGFPE, Sys_SigHandler );
	signal( SIGSEGV, Sys_SigHandler );
	signal( SIGTERM, Sys_SigHandler );

	Cvar_Get( "com_ansicolor", "0", CVAR_ARCHIVE );

	while( 1 )
	{
#ifndef DEDICATED
		int appState = SDL_GetAppState( );

		// If we have no input focus at all, sleep a bit
		if( !( appState & ( SDL_APPMOUSEFOCUS | SDL_APPINPUTFOCUS ) ) )
			SDL_Delay( 16 );

		// If we're minimised, sleep a bit more
		if( !( appState & SDL_APPACTIVE ) )
			SDL_Delay( 32 );
#endif

		Com_Frame( );
	}

	return 0;
}

