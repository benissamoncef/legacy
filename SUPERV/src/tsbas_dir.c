/*E*/
/* Fichier : %W   1.11    Release : 1.11        Date : 09/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_DIR * FICHIER tsbas_dir.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module est le module directeur de la tache tsbas
*	qui permet d'ecrire un message sur une xterm lors du 
* 	basculemnet des serveurs.
*
*******************************************************/

#include <rtworks/ipc.h>
#include "xzst.h"
#include "xzsc.h"


main(int argc, char *argv[])
{
	T_IPC_MSG               pl_Msg;
	int			vl_ValRet;


	printf ("   Basculement du serveur de donnees\n\n");
	printf ("  Duree d'interruption: 10 mn \n\n");
	printf ("  Merci de votre patience \n\n");

	vl_ValRet = XZSC_01InitCnxSup(  argv[2], argv[0],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				NULL,"/dev/null");

	
	while(1)
      	{
	      XZST_03EcritureTrace (XZSTC_WARNING, "Attente infinie d'un message \n");
	       
		/* attente infinie d'un message */
		if (TipcSrvCheck(T_IO_CHECK_WRITE,5.0))
		{
	       		XZST_03EcritureTrace (XZSTC_WARNING, "Serveur present  \n");
			exit(0);
		}
		else
		{
	       		XZST_03EcritureTrace (XZSTC_WARNING, "Serveur non present  \n");
		}
	}
}	/* Fin du Main */
