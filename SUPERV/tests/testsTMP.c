#include <stdio.h>
#include "xzst.h"
#include "xzsc.h"
#include "xdc.h"

#include        <rtworks/common.h>
#include        <rtworks/ipc_proto.h>
#include        <rtworks/ipc.h>


static void sh_sortir()
{
	/* Affichage trace */
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN sh_sortir  " );

	exit(0);
}

/******************
Programme principal pour la tache temporaire testsTMP
*
***********/

main (int argc,char * argv[])

{
	char *  Nom_Appli = "migrazur";

	/****  Connexion au RTServer ***/
	XZSC_01InitCnxSup (Nom_Appli,argv[0],
			XZSCC_INTERDIRE,
			XZSCC_INTERDIRE,
			XZSCC_INTERDIRE,
			sh_sortir,"/dev/null");
	for (;;)
	{
		if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)
					 != T_ERR_CONN_EOF)
		 {
		 /* Affichage trace */
		 XZST_03EcritureTrace(XZSTC_FATAL
			 ,"TipcSrvMainLoop a echoue  ");

		 break;    /* for */
	 	}
	 }

}

