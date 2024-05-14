/* Fichier : agen_dir.c   Relese : 1.3   Date : 09/19/11	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : agen_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de l tache  passerelle generique
* 
*************************************************
* HISTORIQUE :
*
* Guilhou	01/11/08 : Creation 			V1.1 DEM 835
* JMG	15/02/11 : suppression appel a agen_xml_init datex v2 1.2
* JMG	18/09/11 : correction liberation timers dem 998 1.3
* SFO   06/10/14 : Adaptation pour compilation sous linux
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "agen_dir.h"
#include "xzems.h"


/* d�finition des vriables globales � l'application */


/* definition des vriables locales au module */
static char *	version = "@(#)agen_dir.c	1.3 09/19/11 : commentaire" ;


/* declration de fonctions internes */
void agen_sortir();

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
		 ,agen_sortir /* fontion de sortie */
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
	agen_evt_init();

	agen_tdp_init();
	
	agen_rdt_init();


	/* Attente bloqunte des messages du RTServer */
	while(1)
	{
      		/* Recupertion du msg RTworks suivant */
	 	pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		if(isMessageNotNULL(pl_Msg))
		{

			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memoristion Test Presence RTserver */
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
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache agen %d \n",vl_IndPre);
				
				/*A Arret de l tache */
				agen_sortir();
			}

			/*B INdiction Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void agen_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_WARNING, "agen_sortir  : IN  \n");

	XZLG502_SemFree();

	XZLG989_Libere_Memoire();

	
	/* Libertion des ressources utilisees dans agen_export.c */
	agen_free_ressources();
	
	/*A
	 * Sortie 
	 */
	sprintf( vl_cle , "cle_timer_datexII" );
#ifdef _HPUX_SOURCE
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
	XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif
	sprintf( vl_cle , "cle_timer_tdp" );
#ifdef _HPUX_SOURCE
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
	XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif
	sprintf( vl_cle , "cle_timer_rdt" );
#ifdef _HPUX_SOURCE
	XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
	XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif
	
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	exit(0);
}
