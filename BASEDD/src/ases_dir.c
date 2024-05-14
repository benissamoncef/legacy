/* Fichier : ases_dir.c   Relese : 1.3   Date : 03/26/07	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ases_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de l tache serveur de fmc
* pour le PC STRADA
*************************************************
* HISTORIQUE :
*
* Guilhou	08 mi 1997 : Creation 			V1.1
* SFO   06/10/14 	: Adaptation pour compilation sous linux
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
* LCL	04/10/23	: Adaptation CNA DEM-483 : lecture site principal en base
************************************************/

#include <xzsem.h>
#include "ases_dir.h"
#include "xzems.h"

/* d�finition des vriables globales � l'application */


/* definition des vriables locales au module */
static char *	version = "@(#)ases_dir.c	1.3 03/26/07 : commentaire" ;
XDY_Mot		vg_SitePrincipal = 0;
XDY_NomSite vg_NomSitePrincipal;


/* declration de fonctions internes */
void ases_sortir();

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


	/* Connexion u RTServer */
	vl_cr = XZSC_01InitCnxSup(argv[1]
		 ,argv[0]
		 ,XZSCC_INTERDIRE /* mise a jour horodate*/
		 ,XZSCC_AUTORISER /* systeme tarce */
		 ,XZSCC_INTERDIRE /* synchro tsetat */
		 ,ases_sortir /* fontion de sortie */
		 ,"/dev/null"     /* redirection sortie stndard */
		 ) ;


	if (vl_cr != XDC_OK) 
      	{
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
XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
	/* Appel de l fonction d'initialisation */
	vl_cr = ases_evt_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur ases_evt_init cr = %d",vl_cr);
	 	exit;
      	}

	XZST_03EcritureTrace(XZSTC_WARNING,
			"Attente message RTServer");
	
	/* Attente bloqunte des messages du RTServer */
	while(1)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"avant pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);");
      		/* Recupertion du msg RTworks suivant */
	 	pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"apres pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);");

		if(isMessageNotNULL(pl_Msg))
		{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"TipcSrvMsgProcess(pl_Msg);");

			TipcSrvMsgProcess(pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"TipcMsgDestroy(pl_Msg);");
			TipcMsgDestroy(pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"Memo test presence rtserver");

			/* Memoristion Test Presence RTserver */
			vl_IndPre = 0;
         	}
         	
	 	/* erreur RTWORKS */
	 	else 
         	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur RTWORKS");
			/* Test de Presence RTserver */
			if (vl_IndPre == 0) 
			{
				if (errno)
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache ases %d \n",vl_IndPre);
				
				/*A Arret de l tache */
				ases_sortir();
			}

			/*B INdiction Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void ases_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "ases_sortir  : IN  \n");
	
	/*A
	** Fermeture du fichier trce 
	*/
	
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	
	
	/* Libertion des ressources utilisees dans ases_export.c */
	ases_free_ressources();
	
	/*A
	 * Sortie 
	 */
	sprintf( vl_cle , "cle_timer_srvevt" );
		
	/* Liberer les ressources du timer */
#ifdef _HPUX_SOURCE
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
	XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif	
	exit(0);
}
