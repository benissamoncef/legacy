/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : agen_rdt.c				*
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
* JMG	01/10/08	creation DEM/835
* JMG            12/10/08        ajout XDG_AGEN_TIMER DEM 837
* JPL	18/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.3
************************************************/


#include "agen_rdt.h"

/* definition des constantes */

/* d≈finition des variables globales » l'application */


/* definition des variables locales au module */

static XZLGT_Nombre_Mem vm_nombre_mem;

static char *	version = "@(#)agen_ftp.c	1.9 1.9 11/06/06" ;


/* declaration de fonctions internes */
void agen_rdt_genere();


/* definition de fonctions externes */

#define C_FREQ_VERIF_EVT	300

/*A
*******************************************
* Fonction d'initialisation pour la tache
* serveur de donnees trafic
 Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de purge
* - charger le tableau de listes chainees
* contenant l'intitule relatif au code
* evenement Strada
*******************************************
* Arguments : Aucun
*******************************************/
int agen_rdt_init()
{
XZSMT_CleProg           vl_cle;
XZSCT_Datagroup         vl_Datagroup ;
int                     vl_JourSemaine;
double                  vl_HorodateSec;
XZSCT_NomMachine        pl_NomMachine   = "\0";
T_BOOL                  vl_etat;
int                     vl_val;
char                    vl_str1[80];
char                    *vl_chaine;
int                     vl_ValRet;
XDY_NomSite             pl_NomSite;
XZLGT_Liste_RADT	*vl_liste = NULL;

        /* Recuperation du nom de la machine locale */
        XZSC_07NomMachine(pl_NomMachine) ;


        /*A Lecture horodate courante systeme */
        XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);

        vl_HorodateSec+=C_FREQ_VERIF_EVT;
        /* armement du timer de programmation */
        if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
                                                0 ,
                                                XDG_AGEN_TIMER,
                                                XDM_AS_SRVEVT_STRADA_RDT,
                                                "cle_timer_rdt",
                                                XZSMC_SEC_VRAI) != XDC_OK)
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
          return (XDC_NOK);
        }

	XZLG200_GetStations(vl_liste);

	/*initialise la memoire partagee RDT*/
	if (XZLG982_Cree_Memoire_Partagee_RDT() != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_FATAL,"agen_rdt_init: pb appel a XZLG981_Accede_Memoire_Partagee_RDT");
		return (XDC_NOK);
        }
	
	/*cree le semaphore pour la memoire partagee*/
	if (XZLG501_SemCree("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_RDT) != XDC_OK) {
			XZST_03EcritureTrace( XZSTC_FATAL,"agen_rdt_init: pb appel a XZLG501_SemCree");
			return (XDC_NOK);
	}

	/*on lit les donnees ZDP en base au demarrage*/
	agen_rdt_genere();

        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_SRVEVT_STRADA_RDT), agen_rdt_genere,(T_CB_ARG)NULL) == NULL)

        {
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_SRVEVT_STRADA_RDT non cree.",version );
                return XDC_NOK;
        }

        /* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_INFO, "OUT : agen_rdt_init   " );
        return XDC_OK;

}









/* Fichier : @(#)agen_ftp.c	1.9       Release : 1.9        Date : 11/06/06
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE agen_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* generation des fichiers texte a exporter
*
******************************************************
* SEQUENCE D'APPEL :
* agen_ftp_genere();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void agen_rdt_genere()
{
  double                  vl_HorodateSec;
  int                     vl_JourSemaine;

  XZST_03EcritureTrace(XZSTC_WARNING,"agen_rdt_genere : IN");

  /*verrouille*/
  XZLG500_SemCall(-1,XZLGC_SEM_RDT);

  XZLG979_Ecrit_Memoire_Partagee_RDT();

  XZLG983_Update_Checksum();

  XZLG500_SemCall(1,XZLGC_SEM_RDT);

/*A Lecture horodate courante systeme */
        XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
        vl_HorodateSec+=C_FREQ_VERIF_EVT;
        /* armement du timer de programmation */
        if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
                                                0,
                                                XDG_AGEN_TIMER,
                                                XDM_AS_SRVEVT_STRADA_RDT,
                                                "cle_timer_rdt",
                                                XZSMC_SEC_VRAI) != XDC_OK)
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
          return ;
        }

  XZST_03EcritureTrace(XZSTC_WARNING,"agen_rdt_genere : OUT");
}

