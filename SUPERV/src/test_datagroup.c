/*   Fichier permettant de joindre un Datagroup ou de 
*		de le supprimer */


/* fichiers inclus */

#include	<stdio.h>
#include	<sys/errno.h>

#include "xdg.h"

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>
#include	<rtworks/ipc_proto.h>



main (argc,argv)
	int argc;
	char  ** argv;
{
int 	vl_Nombre = 0;



	sscanf(argv[1],"%d",&vl_Nombre);

switch (vl_Nombre) {

	case 1:  	/* Abonnement datagroup */
		/* Abonnemen Datagroup */
		XDG_JoindreDG(argv[2]);
		break;

	case 2:  	/* Abonnement datagroup */
		/* Desabonnement du Datagroup */
	 	XDG_QuitterDG(argv[2]);

		break;

	default :
		break;
	}
}
