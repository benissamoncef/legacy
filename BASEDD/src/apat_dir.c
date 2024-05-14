/* Fichier : @(#)apat_dir.c	1.0   Release : 1.0   Date : 31/03/20
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : apat_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache d'export datex 2
* vers des déstinataires 
*************************************************
* HISTORIQUE :
*
* ABE	22/07/20	: Creation DEM SAE149 V1.1
* GGY	05/02/24	: Adaptation code pour rabbitmq DEM-483
*
************************************************/

#include <xzsem.h>
#include "apat_dir.h"
#include "xzems.h"

/* définition des variables globales à l'application */


/* definition des variables locales au module */
static char *	version = "@(#)apat_dir.c	1.0 1.0 31/03/20 : commentaire" ;
static int version_pre_prod;

/* declaration de fonctions internes */
void apat_sortir();

/* definition de fonctions externes */

/*declaration type d'executable : prod (tapat.x) */

int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
int vl_ValRet;
int                     vl_JourSemaine;
double                  vl_HorodateSec;
char            vl_date[30];
int             vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
int vg_frequence_keep_alive;
int vg_frequence_recap;				
FILE				*pl_fp;
char                    pl_ligne[200];
char                    pl_param[200];
XDY_Entier              vl_val;

	//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING,XZSTC_NECRAN);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	 * Connexion au RTserver et initialisation des variables
	 */
	if( XZSC_01InitCnxSup(   argv[1],
				argv[ 0 ],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				apat_sortir,
				"/dev/null" ) != XDC_OK )
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
	   exit( XDC_NOK );
	}
	else
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_INFO, "Ouverture de la connexion avec le RTserver." );
	}

	XZSC_07NomMachine(vg_NomMachine);
	
	/*A
	 * Recuperation du site de la machine 
	 */
	if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom du site impossible . " );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_INFO, "Recuperation du Nom du site=%s",vg_NomSite );
	}

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_SGBD|XZSTC_DEBUG2,XZSTC_NECRAN);				
	//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING,XZSTC_NECRAN);				

		
	/*A
	** Connexion  la base
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(vg_NomSite)) != XDC_OK)
	{
			/*B
			** Ecriture Trace
			*/
			XZST_03EcritureTrace( XZSTC_WARNING,
				"%s : Connexion u serveur SQL de %s impossible .",version,vg_NomSite);
			exit;
	}
	XZST_03EcritureTrace( XZSTC_WARNING,
				"%s : Connexion u serveur SQL de %s reussie.",version,vg_NomSite);

	/*A
	 * Demande des timers cycliques
	 */
	 
	/* Programation timer */
	XZSM_00InitServices();
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
	if (! XDG_JoindreDG(XDG_TIMER_PAT) )
	{
	/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,		
			"IN : apat_dir : Abonnement sur %s non effectuee. \n " ,XDG_TIMER_PAT);
			return;
			
	}		
	
	/* armement du callback pour PAT */
	if (XZSM_01DemanderProgrammation(0,
					60,
					XDG_TIMER_PAT,
					XDM_AS_TIMER_PAT,
					"fct_get_all_pat_active",
					XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de recap patrouilleur non declenchee." );
	  return (XDC_NOK);
	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_PAT), apat_envoie_pat_en_cours,(T_CB_ARG)NULL) == NULL)
	{
			XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_PAT non cree.",version );
			return XDC_NOK;
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
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tapat %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				apat_sortir();
			}

			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void apat_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "apat_sortir  : IN");

	/*A
	 * Sortie
	 */

	sprintf( vl_cle , "fct_get_all_pat_active" );
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

