/* Fichier : $RCSfile: acet_dir.c,v $   Release : $Revision: 1.1 $   Date : $Date: 2016/09/29 15:40:06 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acet_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache client evenement
* pour le PC STRADA
*************************************************
* HISTORIQUE :
*
*JMG	13/02/16 creation
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "acet_dir.h"
#include "xzems.h"

/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
static char *	version = "$Id: acet_dir.c,v 1.1 2016/09/29 15:40:06 pc2dpdy Exp $ : commentaire" ;


/* declaration de fonctions internes */
void acet_sortir();

/* definition de fonctions externes */


int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
XDY_NomSite	pl_NomSite;
int		vl_ValRet;


	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/* Connexion au RTServer */
	vl_cr = XZSC_01InitCnxSup(argv[1]
		 ,argv[0]
		 ,XZSCC_INTERDIRE /* mise a jour horodate*/
		 ,XZSCC_AUTORISER /* systeme trace */
		 ,XZSCC_INTERDIRE /* synchro tsetat */
		 ,acet_sortir /* fontion de sortie */
		 ,"/dev/null"     /* redirection sortie standard */
		 ) ;


	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur XZSC_01InitCnxSup cr = %d",vl_cr);
	 	exit;
      	}
      	/*A
        ** Recuperation du site de la machine
        */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, 
                	"%s : Recuperation du Nom du site impossible.",version);
                exit(XDC_NOK);
        }
        XZST_03EcritureTrace( XZSTC_DEBUG1, 
                	"%s : Recuperation du Nom du site %s.",version,pl_NomSite);

	/*A 
	** Connexion a la base 
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
        {
                /*B
                ** Ecriture Trace
                */
                XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion au serveur SQL de %s impossible .",version,pl_NomSite);
                exit;
        }
        XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion au serveur SQL de %s reussie.",version,pl_NomSite);

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
	/* Appel de la fonction d'initialisation */
	vl_cr = acet_evt_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur acet_evt_init cr = %d",vl_cr);
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
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache acet %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				acet_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

	
}




void acet_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "acet_sortir  : IN  \n");
	
	/*A
	** Fermeture du fichier trace 
	*/
	
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	
	
	/* Liberation des ressources utilisees dans acet_export.c */
	
	/*A
	 * Sortie 
	 */
	sprintf( vl_cle , "cle_timer_saga" );
		
	/* Liberer les ressources du timer */
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
	
	exit(0);
}
