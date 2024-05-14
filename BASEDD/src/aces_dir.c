/* Fichier : $RCSfile: aces_dir.c,v $   Release : $Revision: 1.3 $   Date : $Date: 1999/03/19 16:06:23 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : aces_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache client evenement
* pour le PC STRADA
*************************************************
* HISTORIQUE :
*
* Penot	08 mai 1997 : Creation 			V1.1
* Niepceron 02/03/1999 : correction de la cel du timer v1.3
************************************************/

#include <xzsem.h>
#include "aces_dir.h"


/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
static char *	version = "$Id: aces_dir.c,v 1.3 1999/03/19 16:06:23 gesconf Exp $ : commentaire" ;


/* declaration de fonctions internes */
void aces_sortir();

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
		 ,aces_sortir /* fontion de sortie */
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

	/* Appel de la fonction d'initialisation */
	vl_cr = aces_evt_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur aces_evt_init cr = %d",vl_cr);
	 	exit;
      	}

	
	/* Attente bloquante des messages du RTServer */
	while(1)
	{
      		/* Recuperation du msg RTworks suivant */
	 	pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		if(pl_Msg != NULL)
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
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache aces %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				aces_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

	
}




void aces_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "aces_sortir  : IN  \n");
	
	/*A
	** Fermeture du fichier trace 
	*/
	
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	
	
	/* Liberation des ressources utilisees dans aces_export.c */
	aces_evt_freemem();
	
	/*A
	 * Sortie 
	 */
	sprintf( vl_cle , "cle_timer_client_evt" );
		
	/* Liberer les ressources du timer */
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
	
	exit(0);
}
