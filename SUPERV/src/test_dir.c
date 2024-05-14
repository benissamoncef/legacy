
/* fichiers inclus */

#include	<stdio.h>
#include	<sys/errno.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>
#include	<rtworks/ipc_proto.h>



main (argc,argv)
	int argc;
	char  ** argv;
{
T_INT4	socket_fd;
T_INT4  vl_NumPort;


	sscanf(argv[2],"%d",&vl_NumPort);
	printf ("Numero de Port %d \n",vl_NumPort);

	/*A essai dfe connection TCP */
	socket_fd = TutSocketCreateClientTcp(argv[1],vl_NumPort);
	if(socket_fd == -1)
	{
		TutOut("  Erreur de connexion TCP %d \n");
		printf ("Erreur UNIX %d \n",errno);
	}

}
