/* Fichier : @(#)amisol_transfert.c  1.0   Release : 1.0   Date : 06/02   *
*************************************************
* MODULE  : BASEDD                              *
* FICHIER : amisol_transfert.c                       *
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
* + appel des fonctions du process
*************************************************
* HISTORIQUE :
* ETR: creation
************************************************/
#include "amisol_transfert.h"

/* definition des constantes */

#define CO_FIC_FREQUENCE        XDC_PATH_STRADA "/frequence.txt"
#define CO_DELAI_FREQUENCE      300
#define CO_MAX_CARACTERE	200

/*  ---->  besoin ajouter freq dans fichiers/stra/frequence.txt*/

/* definition des variables globales a l'application */

/* definition des variables locales au module */
static char *   version = "@(#)amisol_transfert.c    1.0 06/02 : commentaire" ;

static FILE *pm_fd;

int nb_seq = 0;

/* definition de fonctions externes */

/*A
*******************************************
* Fonction d'initialisation pour la tache
* Elle lit les fichiers de parametrage
* et arme le  timer de programmation 
*
* vers Fonction amisol_transfert_tcpip
*******************************************
* Arguments : Aucun
* Conditions: 
* - nom de la tache = tami2.x
*******************************************/
int amisol_transfert_init()
{
XZSMT_CleProg           vl_cle;
int                     vl_JourSemaine;
double                  vl_HorodateSec;
XZSCT_NomMachine        pl_NomMachine   = "\0";
FILE                    *pl_fp;
char                    pl_param[CO_MAX_CARACTERE];
char                    pl_ligne[CO_MAX_CARACTERE];
XDY_Entier              vl_val;
int                     vl_frequence=NULL;
int                     vl_station;

 XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : amisol_transfert_init" );

	 /* Recuperation du nom de la machine locale */
	 XZSC_07NomMachine(pl_NomMachine) ;

	 /* Lecture de la frequence d'exportation des donnees vers MISOL2 */
	 if ((pl_fp = fopen(CO_FIC_FREQUENCE, "r")) == NULL ) 
	 {
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,CO_FIC_FREQUENCE );
		return(XDC_NOK);
	 }

	 /* On lit les lignes du fichier frequence.txt une par une */
	 /* ------------------------------------------------------ */

	 while ( fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
	/* On ignore les lignes commencant par un # */
		 if ( pl_ligne[0] == '#' )
			 continue;

	 /* Pour les autres lignes on recherche la frequence de notre tache */
		 if (sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val)==2)
		 {
		 	if ( !strcmp( pl_param, "tami2.x") ) 
			{
			 vl_frequence = vl_val;
			 break;
		 	}
	 	}
	 	else
		 XZST_03EcritureTrace( XZSTC_WARNING, "le fichier %s n'a pas le format demande !" ,CO_FIC_FREQUENCE );

	}/*end while*/
	


	 if (vl_frequence==NULL) {
	   XZST_03EcritureTrace(XZSTC_WARNING, "pas de frequence dans le fichier %s : utilisation freq par default %d" , CO_FIC_FREQUENCE , CO_DELAI_FREQUENCE);
	   vl_frequence= CO_DELAI_FREQUENCE;
	 }


	 fclose(pl_fp);

	   /*A
	   * Initialisation de la callback du timer de programmation
	   * -------------------------------------------------------
	   */
	    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AMISOL_TIMER), amisol_transfert_tcpip,(T_CB_ARG)NULL) == NULL)
	    /* --> creation constante dans xdm.h , ajout dans xdm.c. Attention, besoin de recompiler tstim.x !! */
	    {
	  	 XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AMISOL_TIMER non cree.",version) ;
	  	 return XDC_NOK;
	     }

	     XZSM_00InitServices();

	     /*A Lecture horodate courante systeme */
	     XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
	
	     sprintf( vl_cle , "cle_timer_amisol" );
	
	     /*A Abonnement au datagroup pour que le timer nous envoie les messages */
	      if (! XDG_JoindreDG(XDG_AMISOL_TIMER) )
	      /* --->  creation constante XDG_AMISOL_TIMER dans xdg.h */
	       {
		       /* Ecriture Trace */
		       XZST_03EcritureTrace( XZSTC_WARNING, "Abonnement sur %s non effectuee. \n " ,XDG_AMISOL_TIMER);
		       return XDC_NOK;
		}

	
	     /* armement du timer de programmation */
	     /* ---------------------------------- */
	     if (XZSM_01DemanderProgrammation(   vl_HorodateSec,
						 vl_frequence,
						 XDG_AMISOL_TIMER,
						 XDM_AMISOL_TIMER,
						 vl_cle,
						 NULL) != XDC_OK)
	     {
		 /* Ecriture Trace */
		 XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de  transfert misol2 non declenchee. \n " );
		 return XDC_NOK;
	     }


	/* Ecriture Trace */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : amisol_transfert_init" );
	return XDC_OK;

}

/*A
*******************************************
* Fonction de gestion des appels
* - recuperation des donnees
* - etablissement connexion 
* - transfert des donnees
* - gestion fin execution
*******************************************
* Arguments : aucun 
*******************************************/

void amisol_transfert_tcpip()
{

	/* re_init du compteur de sequence 1 fois par semaine , soit 1680 sur une base 6min */
	if (nb_seq > 1680)
	{ nb_seq = 0;}

	++nb_seq;


	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_FONCTION, "In : amisol_transfert_tcpip");
	XZST_03EcritureTrace( XZSTC_WARNING, "===== >>> SEQ %d  DEBUT <<< ========== ", nb_seq);
	
	/* recuperation des donnees */
	/* construction d'1 tableau des mesures a envoyer*/
	if (XZAE175_Get_6Min_Sequence((XDY_FonctionInt)amisol_tcpip_construct_table_mesure) != XDC_OK)
	 {
	 	XZST_03EcritureTrace( XZSTC_WARNING, "amisol_transfert_tcpip : pb appel XZAE175");
		return;
	 }

	/*init connexion tcpip */
	if(amisol_tcpip_connexion_init() != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_connexion_init : pb creation socket ou connexion MISOL2");
		amisol_free_ressources();

		return;
	}

	/*envoi header message : id, tc mes */
	if(amisol_tcpip_manage_header_message() != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : pb Identification MISOL2 ou non acceptation mode de transfert par MISOL2");
		amisol_tcpip_connexion_close();
		amisol_free_ressources();	
		
		return;
	}

	XZST_03EcritureTrace( XZSTC_INFO, "envoi header amisol_tcpip_header_message OK");
	
	/* envoi des donnees */
	if(amisol_tcpip_send_body_message() != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_send_body_message : pb lors du transfert des donnees vers  MISOL2");
		amisol_tcpip_connexion_close();
		amisol_free_ressources();

		return;
	}

	XZST_03EcritureTrace( XZSTC_INFO, "envoi donnees amisol_tcpip_send_body_message OK");

	/* envoi end of message */
	if(amisol_tcpip_manage_end_message() != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_end_message : pb FIN message  vers  MISOL2");
		amisol_tcpip_connexion_close();
		amisol_free_ressources();

		return;

	}
	 XZST_03EcritureTrace( XZSTC_INFO, "envoi footer amisol_tcpip_manage_end_message OK");

	/*fermeture session */
	amisol_tcpip_connexion_close();

	XZST_03EcritureTrace( XZSTC_WARNING, " =================  >>>  DIALOGUE avec MISOL2 SEQUENCE %d reussi <<< ==============",nb_seq);
	XZST_03EcritureTrace(XZSTC_FONCTION, "amisol_transfert_tcpip : OUT ");
	return;
}
