/* Fichier : @(#)amisol_dir.c     1.0   Release : 1.0   Date :06/2002
*************************************************
* Elcimai Informatique
*************************************************
* MODULE  : BASEDD                              
* FICHIER : amisol_dir.c                          
*************************************************
* DESCRIPTION :
* module directeur de la tache d'export MISOL 
* Cette tache transfert des donnees par tcpip
* vers machine MISOL2
*************************************************
* HISTORIQUE :
* ET	: creation
* SFO   06/10/14 : Adaptation pour compilation sous linux
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "amisol_dir.h"
#include "xzems.h"


/* definition des variables globales  a l'application *

/* definition des variables locales au module */
static char *   version = "@(#)amisol_dir.c       1.0 06/02 : commentaire" ;


/* declaration de fonctions internes */
void amisol_sortir();

/* definition de fonctions externes */

int main (int argc, char * argv[]) {
int             vl_IndPre       = 0;
int             vl_Attente      = 0;
int             vl_cr           = XDC_OK;
T_IPC_MSG       pl_Msg;
XDY_NomSite     pl_NomSite;
int             vl_ValRet;

	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/* Connexion au RTServer , Initialisation fichier trace*/
	/* argv[1]: nomApplication, argv[0]: NomTache */
	 vl_cr = XZSC_01InitCnxSup(argv[1]
	         ,argv[0]
	  	 ,XZSCC_INTERDIRE       /* mise a jour horodate */
	   	 ,XZSCC_AUTORISER       /* systeme trace        */
	    	 ,XZSCC_INTERDIRE       /* synchro tsetat       */
		 ,amisol_sortir         /* fontion de sortie    */
	         ,"/dev/null"           /* redirection sortie standard /dev/null */
				) ;
	

	if (vl_cr != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur XZSC_01InitCnxSup cr = %d",vl_cr);
		exit;
			}

	 /*A
	 ** Recupertion du site de la machine
	 */
	 if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	 {
		XZST_03EcritureTrace( XZSTC_WARNING, "%s : Recupertion du Nom du site impossible.",version);
		exit(XDC_NOK);
	}
	 XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : Recupertion du Nom du site %s.",version,pl_NomSite);

	/*A
	* Connexion a la base *
	*/
	 if ((vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
	 {
		 /*B
		 ** Ecriture Trace
		 */
		 XZST_03EcritureTrace( XZSTC_WARNING, "%s : Connexion au serveur SQL de %s impossible .",version,pl_NomSite);
		 exit;
	 }
/*
	 XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_DEBUG1|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
*/

	 XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_MESSAGE,XZSTC_NECRAN);

	/* Appel de la fonction d'initialisation */
	 vl_cr = amisol_transfert_init();

	if (vl_cr != XDC_OK)
	 {
		 XZST_03EcritureTrace(XZSTC_WARNING, "erreur amisol_transfert_init cr = %d",vl_cr);
		 exit;
	 }

	 /* Attente bloquante des messages du RTServer */
	  while(1)
		  {
			  /* Recuperation du msg RTworks suivant */
			  pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

			  if(isMessageNotNULL(pl_Msg))
			  {
				  TipcSrvMsgProcess(pl_Msg);
				  TipcMsgDestroy(pl_Msg);

				  /* Memorisation Test Presence RTserver */
				  vl_IndPre = 0;
			  }
			  /* erreur RTWORKS */
		  	 else
			  {
				  /* Test de Presence RTserver */
				  if (vl_IndPre == 0)
				  {
					if (errno)
				 	 	XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n", TutErrNumGet(), errno);
				  }
				  else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
				  {
					  XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache amisol %d \n",vl_IndPre);

					  /*A Arret de la tache */
					  amisol_sortir();
				  }

				  /*B INdication Non Presence RTserver */
				if (errno)
				  vl_IndPre++;


			  }

		  }       /* Fin attente infinie */

 }




/*A
************************************************************
* fonction d'arret de la tache *
************************************************************
*/
void amisol_sortir()
{
	int             vl_ValRet = XDC_OK;
	XZSMT_CleProg   vl_cle;

	XZST_03EcritureTrace(XZSTC_FONCTION, "amisol_sortir  : IN");

	/* liberation des ressources utilisees dans amisol_tcpip  fait dans amisol_transfert*/
	/*amisol_free_ressources();*/
	
	/*A
	 * Sortie
  	 */
	  sprintf( vl_cle , "cle_timer_amisol" );
	  /* Liberer les ressources du timer */
#ifdef _HPUX_SOURCE
	  XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
	  XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif
	  /*A
	  ** Fermeture du fichier trace
	  */

	  if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	  {
		  XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	  }

	  exit(0);

}

