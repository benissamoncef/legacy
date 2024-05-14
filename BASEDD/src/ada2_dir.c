		/* Fichier : @(#)ada2_dir.c	1.0   Release : 1.0   Date : 31/03/20
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ada2_dir.c				*
*************************************************
* DESCRIPTION :
* module directeur de la tache d'export datex 2
* vers des déstinataires 
*************************************************
* HISTORIQUE :
*
* ABE	31/03/20	Creation SAE-130	V1.0
* ABE	09/07/2020	Correction pour recette DEM149 V1.2
* ABE	09/07/2020	Correction pour recette, suppression appel ada2_xml_init() DEM149 V1.3
* ABE	23/07/2020	Suppression des patrouilleur de tada2 vers tapat DEM SAE149 V1.4
* ABE	07/09/2020	Ajout traitement TDP DEM-SAE130 V1.5
* ABE   16/09/2020	Ajout armement timer ada2_evt_cb_situation_coupparcoup DEM-SAE130 V1.6
* ABE   16/09/2020	Ajout joindre DG XDG_TIMER_TAGEN DEM-SAE130 V1.7
* ABE	23/10/2020	Synchro TDP avec les 6min DEM-SAE130 V1.8
* AMA 	16/11/2020	liberation du semaphore au demarrage DEM-SAE130 V1.9
 ABE	18/11/2020	Revert du traitement radt pour livraison de la libération du sémaphore à la fin dde la situation recap dans ada_evt.c DEM-SAE V1.10
* ABE	18/11/2020	Livraison publication radt DEM-SAE V1.11
* PNI	11/05/2021	KeepALive ajout� V1.12
************************************************/

#include <xzsem.h>
#include "ada2_dir.h"
#include "xzems.h"

#define CO_FIC_FREQUENCE        XDC_PATH_ADA "/frequence.txt"
#define	C_FREQ_RECAP_EVT	60
#define	C_FREQ_KEEP_ALIVE	600
#define C_FREQ_TRAFFIC		360	

/* définition des variables globales à l'application */


/* definition des variables locales au module */
static char *	version = "@(#)ada2_dir.c	1.0 1.0 31/03/20 : commentaire" ;
static int version_pre_prod;

/* declaration de fonctions internes */
void ada2_sortir();
void ada2_process_Timer_KEEPALIVE_cb();

/* definition de fonctions externes */
char *ada2_ftp_get_heure();
/*declaration type d'executable : prod (tada2.x) ou pre prod (tada2pp.x)*/
static int version_pre_prod;

XDY_Mot vg_numero_plan;

int main(int argc, char *argv[])
{
int		vl_IndPre       = 0;
int		vl_Attente      = 0;
int		vl_cr	= XDC_OK;
T_IPC_MSG	pl_Msg;
int vl_ValRet;
int                     vl_JourSemaine;
double                  vl_HorodateSec;
int                  vl_HorodateTemp;
char            vl_date[30];
int             vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
int vl_frequence_keep_alive;
int vl_frequence_recap;
int vl_frequence_traffic;		
FILE			*pl_fp;
char                    pl_ligne[200];
char                    pl_param[200];
XDY_Entier              vl_val;
XDY_Datagroup	vl_dg;

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
				ada2_sortir,
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

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_DEBUG1|XZSTC_DEBUG3|XZSTC_DEBUG3|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);				
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
				"%s : Connexion au serveur SQL de %s reussie.",version,vg_NomSite);

	sleep(10);

	vl_cr =ada2_xml_init();
	
	if (vl_cr != XDC_OK) 
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,
			"erreur ada2_xml_init cr = %d",vl_cr);
	 	exit(0);
    }	


	// securite cas ou tada a plante en laisant le cemaphore ouvert	
	XZLG500_SemCall(1,XZLGC_SEM_FMC);

	//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING,XZSTC_NECRAN);

	/* Lecture de la frequence d'exportation des donnees vers les ASjF */
	if ( (pl_fp = fopen(CO_FIC_FREQUENCE, "r")) == NULL ) {
			XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
									,CO_FIC_FREQUENCE );
			return(XDC_NOK);
	}

	/* On lit les lignes du fichier frequence.txt une par une */
	/* ------------------------------------------------------ */

	/* initialisation au fréquence par défaut */
	vl_frequence_keep_alive =C_FREQ_KEEP_ALIVE;					
	vl_frequence_recap =C_FREQ_RECAP_EVT;
	vl_frequence_traffic=C_FREQ_TRAFFIC;

	while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
			/* On ignore les lignes commencant par un # */
			if ( pl_ligne[0] == '#' )
					continue;

			/* Pour les autres lignes on recherche la frequence de notre tache */
			if (sscanf(pl_ligne,"%s\t%d", pl_param, &vl_val)==2)
			{
					if ( !strcmp( pl_param, "keep_alive") ) 
					{
						vl_frequence_keep_alive = vl_val;
						XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_dir frequence keep alive:%d ",vl_frequence_keep_alive );
					}
					else if ( !strcmp( pl_param, "recap") ) 
					{
						vl_frequence_recap = vl_val;
						XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_dir frequence recap :%d ",vl_frequence_recap );
					}
					else if ( !strcmp( pl_param, "traffic") )
					{
						vl_frequence_traffic= vl_val;
						XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_dir frequence traffic:%d ",vl_frequence_traffic );
					}
			}
			else 
			{
					XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le format demande !"
													,CO_FIC_FREQUENCE );
			}
	}
	fclose(pl_fp);
	
	/* Abonne keepalibe */
	/* Abonnement au data group keep alive */
	XDG_EncodeDG2(vl_dg, XDG_KEEPALIVE, vg_NomSite);
	XDG_JoindreDG(vl_dg);

	/* N'utilise pas le service generique car besoin appel proc stock�e sp�cifique */
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_KEEPALIVE),
	     		ada2_process_Timer_KEEPALIVE_cb,(T_CB_ARG) NULL)==NULL)
	  	{
	    		return (XZSCC_RTW_INV);
	  	}

	/*A
	 * Demande des timers cycliques
	 */
	 
	/* Programation timer */

	if (! XDG_JoindreDG(XDG_TIMER_DATEX2) )
	{
	/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,		
			"IN : ada2_dir : Abonnement sur %s non effectuee. \n " ,XDG_TIMER_DATEX2);
			return;
			
	}		

	if (!XDG_JoindreDG(XDG_TIMER_TAGEN) )
	{
	/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,		
			"IN : ada2_dir : Abonnement sur %s non effectuee. \n " ,XDG_TIMER_TAGEN);
			return;

	}		


	/* armement du timer de programmation pour situation recapitulative des evts */

	if (XZSM_01DemanderProgrammation(0,
					vl_frequence_recap,
					XDG_TIMER_DATEX2,
					XDM_AS_TIMER_SITU_RECAP,
					"fct_datex2_envoi_situ_recap",
					XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer des situation recap des evts non declenchee." );
	  return (XDC_NOK);
	}
	
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_SITU_RECAP), ada2_evt_cb_situation_recap,(T_CB_ARG)NULL) == NULL)
	{
			XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_SITU_RECAP non cree.",version );
			return XDC_NOK;
	}

	/* armement du timer de programmation pour situation coup par coup des evts */

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_SITU_CPC), ada2_evt_cb_situation_coupparcoup,(T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_SITU_CPC non cree.", version );
		return XDC_NOK;
	}

	XZSM_00InitServices();
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);

	/* Armemment timer TDP_FCD 6 min */
	// Récupération nbr secondes du temps now
	vl_HorodateTemp=(int)vl_HorodateSec;
	// secondes ecoulees depuis un multiple de 6minutes +1 min de marge
	vl_HorodateTemp= (360-(vl_HorodateTemp%360))+60;
	// Ajout au temp now
	vl_HorodateSec+=vl_HorodateTemp;

	XZST_03EcritureTrace( XZSTC_WARNING, "ada2_dir : debut du timer 6 min dans %d sec",vl_HorodateTemp);

	/* armement du timer de programmation pour donnees traffic */

	if (XZSM_01DemanderProgrammation(vl_HorodateSec,
					vl_frequence_traffic,
					XDG_TIMER_DATEX2,
					XDM_ADA_TIMER_RADT,
					"fct_datex2_envoi_donnee_traffic",
					XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer des donnees traffic non declenchee." );
	  return (XDC_NOK);
	}
	
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ADA_TIMER_RADT), ada2_get_radt,(T_CB_ARG)NULL) == NULL)
	{
			XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_RADT non cree.",version );
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
				{
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
						TutErrNumGet(), errno);
					errno=0;
				}
			}
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tada2 %d \n",vl_IndPre);
				
				/*A Arret de la tache */
				ada2_sortir();
		
			}
            
			/*B INdication Non Presence RTserver */
			if (errno)
				vl_IndPre++;

	
		}
		
	}  	/* Fin attente infinie */

}




void ada2_sortir()
{
int		vl_ValRet = XDC_OK;
XZSMT_CleProg 	vl_cle;


	XZST_03EcritureTrace(XZSTC_FONCTION, "ada2_sortir  : IN");

	/*A
	 * Sortie
	  */
       /* Liberer les ressources du timer */
#ifdef _HPUX_SOURCE
	XZSM_02AnnulerProgrammation( "fct_datex2_envoi_situ_recap",NULL,NULL);
	XZSM_02AnnulerProgrammation( "fct_datex2_envoi_donnee_traffic",NULL,NULL);
	XZSM_02AnnulerProgrammation( "fct_datex2_envoi_keep_alive",NULL,NULL);
#else
       XZSM_02AnnulerProgrammation( "fct_datex2_envoi_situ_recap",(char *) NULL,0);
       XZSM_02AnnulerProgrammation( "fct_datex2_envoi_donnee_traffic",(char *) NULL,0);
       XZSM_02AnnulerProgrammation( "fct_datex2_envoi_keep_alive",(char *) NULL,0);
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

void ada2_process_Timer_KEEPALIVE_cb(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
				   T_CB_ARG pa_arg)
{
XDY_Entier            vl_entier1,vl_entier2,vl_entier3,vl_entier4;
char                  vl_nom_autoroute[10];
char                  vl_str1[100],vl_str2[100],vl_str3[100];



XZST_03EcritureTrace (XZSTC_DEBUG1, "process message timer KEEPALIVE");
XZAE146_Recherche_Loc_Rds(
                          20000,
                          1,
                          vl_str3,
                          &vl_entier1 ,
                          &vl_entier2 ,
                          vl_str1,
                          vl_str2,
                          &vl_entier3,
                          &vl_entier4,
                          vl_nom_autoroute);
}

