/* Fichier : acts_dir.c   Release : 1.1   Date : 05/15/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acts_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache client trafic
* pour le PC STRADA
*************************************************
* HISTORIQUE :
*
* PENOT	05 Mai 1997 : Creation 		V1.1
* Niepceron 	05 Jan 1998 : intergestionnaire dem/1724 v1.4
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "acts_dir.h"
#include "xzems.h"


/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
static char *	version = "@(#)acts_dir.c	1.1 05/15/97 : commentaire" ;


/* declaration de fonctions internes */
void acts_sortir();

/* definition de fonctions externes */

/* procedure d'initialisation du seveur trafic
 	  	- Lancement du NTServeur
 	  	- Lancement de acts_traf_init()                 */

int main (int argc, char * argv[]) {

int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr		= XDC_OK;
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
		 ,XZSCC_INTERDIRE 	/* mise a jour horodate	*/
		 ,XZSCC_AUTORISER 	/* systeme trace 	*/
		 ,XZSCC_INTERDIRE 	/* synchro tsetat 	*/
		 ,acts_sortir		/* fontion de sortie 	*/  
		 ,"/dev/null"     	/* redirection sortie standard */
		 ) ;


	if (vl_cr != XDC_OK) {
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur XZSC_01InitCnxSup cr = %d",vl_cr);
	 	exit;
      	}
      	 
      	/*A
        ** Recupertion du site de la machine
        */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, 
                	"%s : Recupertion du Nom du site impossible.",version);
                exit(XDC_NOK);
        }
        XZST_03EcritureTrace( XZSTC_DEBUG1, 
                	"%s : Recupertion du Nom du site %s.",version,pl_NomSite);

	/*A 
	** Connexion  la base 
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
        {
                /*B
                ** Ecriture Trace
                */
                XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion u serveur SQL de %s impossible .",version,pl_NomSite);
                exit;
        }
        XZST_03EcritureTrace( XZSTC_WARNING,
                	"%s : Connexion u serveur SQL de %s reussie.",version,pl_NomSite);

	/* Appel de la fonction d'initialisation */
	vl_cr = acts_traf_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur acts_traf_init cr = %d",vl_cr);
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
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache acts %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				acts_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void acts_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "acts_sortir  : IN  \n");
	
	/*A
	** Fermeture du fichier trace 
	*/
	
	XZST_02FermetureTrace();
	
	/*A
	 * Sortie 
	 */
	sprintf( vl_cle , "cle_timer_client_radt" );
		
	/* Liberer les ressources du timer */
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
	
	/* Liberer la liste chainee des equipements */
	acts_traf_freemem();
	
	exit(0);


}
