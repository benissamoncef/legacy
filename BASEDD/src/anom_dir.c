/* Fichier : $Id: anom_dir.c,v 1.2 2003/01/21 16:52:00 gesconf Exp $ Release : $Revision: 1.2 $   Date : $Date: 2003/01/21 16:52:00 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : anom_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache 
* de dialogue avec le SAE NOMADE
*************************************************
* HISTORIQUE :
*
* JMG	07/12/01	creation 1.1
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "anom_dir.h"
#include "xzems.h"


/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
static char *	version = "$Id: anom_dir.c,v 1.2 2003/01/21 16:52:00 gesconf Exp $ $Revision: 1.2 $ $Date: 2003/01/21 16:52:00 $";


/* declaration de fonctions internes */
void anom_sortir();

/* definition de fonctions externes */


int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
XDY_NomSite	pl_NomSite;
int		vl_ValRet;
char    *pl_nom;
char vl_tache[50];

 if ((pl_nom = strrchr(argv[0], '/')) == NULL)
   strcpy(vl_tache,argv[0]);
 else
   strcpy(vl_tache,++pl_nom);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/* Connexion au RTServer */
	vl_cr = XZSC_01InitCnxSup(argv[1]
		 ,vl_tache
		 ,XZSCC_INTERDIRE /* mise a jour horodate*/
		 ,XZSCC_AUTORISER /* systeme trace */
		 ,XZSCC_INTERDIRE /* synchro tsetat */
		 ,anom_sortir /* fontion de sortie */
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
	vl_cr = anom_import_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur anom_init cr = %d",vl_cr);
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
				{
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
					errno = 0;
				}
			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tanom %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				anom_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void anom_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "anom_sortir  : IN");

	/*A
	 * Sortie
	  */
        sprintf( vl_cle , "cle_timer_nomade" );

       /* Liberer les ressources du timer */
       XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);

	/*A
	** Fermeture du fichier trace 
	*/
	
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	
	exit(0);
}
