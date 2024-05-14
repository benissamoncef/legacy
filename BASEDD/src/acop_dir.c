/* Fichier : @(#)acop_dir.c	1.3   Release : 1.3   Date : 11/17/08 
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acop_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache d'export WEB
* vers les ASF 
*************************************************
* HISTORIQUE :
*
* JMG	16/01/11	creation 1.1
* VRE	05/12/11	Ajout is_version_preprod (DEM 1015) 1.2
* JMG	30/01/12	correction timer 1.3
* JMG	29/11/13	traitement tacopp
* SFO   06/10/14 : Adaptation pour compilation sous linux
* AMA 	16/11/2020	liberation du semaphore au demarrage
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
************************************************/

#include <xzsem.h>
#include "acop_dir.h"
#include "xzems.h"

/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
static char *	version = "@(#)acop_dir.c	1.3 1.3 11/17/08 : commentaire" ;
static int version_pre_prod;

/* declaration de fonctions internes */
void acop_sortir();

/* definition de fonctions externes */
char *acop_ftp_get_heure();
/*declaration type d'executable : prod (tacop.x) ou pre prod (tacopp.x)*/
static int version_pre_prod;

int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
XDY_NomSite	pl_NomSite;
int		vl_ValRet;
char            vl_date[30];
int             vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	// securite cas ou tacop a plante en laisant le cemaphore ouvert	
	XZLG500_SemCall(1,XZLGC_SEM_FMC);

	/* Connexion au RTServer */
	vl_cr = XZSC_01InitCnxSup(argv[1]
		 ,argv[0]
		 ,XZSCC_INTERDIRE /* mise a jour horodate*/
		 ,XZSCC_AUTORISER /* systeme trace */
		 ,XZSCC_INTERDIRE /* synchro tsetat */
		 ,acop_sortir /* fontion de sortie */
		 ,"/dev/null"     /* redirection sortie standard */
		 ) ;


	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur XZSC_01InitCnxSup cr = %d",vl_cr);
	 	exit;
      	}
      	
        /* recuperation du nom du processus */
        version_pre_prod = 0;
	if ( !strcmp(argv[0],"tacopp.x"))
		version_pre_prod = 1;

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

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
	/* Appel de la fonction d'initialisation */
	vl_cr =acop_xml_init();
	
	if (vl_cr != XDC_OK) 
      	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,
			"erreur acop_export_init cr = %d",vl_cr);
	 	exit(0);
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
					errno=0;
				}
			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tacop %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				acop_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void acop_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "acop_sortir  : IN");

	/*A
	 * Sortie
	  */
if (is_version_preprod())
	strcpy(vl_cle,"cle_timer_acopp");
       else
	 sprintf( vl_cle , "cle_timer_acop" );
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

int is_version_preprod()
{
	return( version_pre_prod );
}
