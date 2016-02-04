#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DURATION		60
#define REPORT_PERIOD	1

int main( int argc, char * argv [] )
{
	int buffer_size;
	
	if( argc == 1 )
	{
		fprintf( stderr, "Размер блока подразумевается 4096.\n" );
		buffer_size = 4096;
	}
	else if( argc != 2 )
	{
		fprintf( stderr, "Usage: rpmtest [размер блока в байтах]\n" );
		return 6;
	}
	else
	{
		buffer_size = atoi( argv[1] );
		if( buffer_size < 0 || buffer_size > 65536 )
		{
			fprintf( stderr, "[FATAL]: Размер буфера в %d байт неприемлим.\n", buffer_size );
			return 7;
		}
		fprintf( stderr, "[INFO]: Размер буфера %d байт.\n", buffer_size );
	}

	int iterations = 0;
	
	int fd;
	fd = open( "gyroscope", O_RDWR | O_CREAT, 0666 );
	if( -1 == fd )
		return 1;

	unsigned char * buffer = malloc( buffer_size );
		
	ssize_t items = write( fd, buffer, buffer_size );
	if( items != buffer_size )
		return 2;
		
	int code = fsync( fd );
	if( code == -1 )
		return 3;
	
	struct timeval tv_start;
	gettimeofday (&tv_start, NULL);
	
	struct timeval tv_end;
	memcpy( &tv_end, &tv_start, sizeof( struct timeval ) );
	tv_end.tv_sec += DURATION;
	
	struct timeval tv_report;
	memcpy( &tv_report, &tv_start, sizeof( struct timeval ) );  
	tv_report.tv_sec += REPORT_PERIOD;

	struct timeval tv_current;
	do
    {
		iterations++;
		if( -1 == lseek( fd, 0, SEEK_SET ) )
			return 4;
			
		if( buffer_size != write( fd, buffer, buffer_size ) )
			return 5;
			
		fdatasync (fd);
		
		gettimeofday( &tv_current, NULL );
		
		struct timeval tv_diff;
		timersub( &tv_current, &tv_start, &tv_diff );
		
		double elapsed = tv_diff.tv_sec + tv_diff.tv_usec*1e-6;
		
		while( timercmp( &tv_report, &tv_current, < ) )
		{
      		printf( "%d %.3f %.3f\n", iterations, elapsed, ((double)iterations) / (elapsed/DURATION) );
      		tv_report.tv_sec += REPORT_PERIOD;
      	}
    }
	while( timercmp (&tv_current, &tv_end, <) );
	
	return 0;
}
                                                                                                                                                                                                             