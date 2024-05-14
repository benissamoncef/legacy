/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : agen_tdp.c				*
*************************************************
* DESCRIPTION :
* gestion des temps de parcours en memoire partagee
* 
*************************************************
* HISTORIQUE :
* JMG	01/10/08	creation DEM 835
* JMG            12/10/08        ajout XDG_AGEN_TIMER DEM 837
* JPL	18/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.3
* JMG	1512/15 : ajout RDS dans ZDP (TDP ASF 1.4) DEM1151
************************************************/


#include "agen_tdp.h"

/* definition des constantes */

/* d�finition des variables globales � l'application */


/* definition des variables locales au module */

static XZLGT_Nombre_Mem vm_nombre_mem;

static char *	version = "@(#)agen_ftp.c	1.9 1.9 11/06/06" ;


/* declaration de fonctions internes */
void agen_tdp_genere();


/* definition de fonctions externes */

#define C_FREQ_VERIF_EVT	360

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
int agen_tdp_init()
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
XZLGT_Liste_Zdp		*vl_liste = NULL;
XDY_Horodate            vl_DateEnSeconde, vl_DateReveil;
long                    vl_prochaineMinute;
long                    vl_seconde, vl_minute, vl_heure;
long                    vl_p = 6,
			vl_m = 0;
        /* Recuperation du nom de la machine locale */
        XZSC_07NomMachine(pl_NomMachine) ;

	/*A
	** Lire l'horodate actuelle
	 ** et programmer le premier declenchement de reveil aux minutes suivantes :
	 ** 00 06 12 18 24...
	 ** plus 40 secondes
	 */
	 XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_DateEnSeconde);
	 vl_seconde = ((long) vl_DateEnSeconde) % 3600;
	 /* declenche le reveil toutes les vl_p minutes, en commencant par la minute vl_m */
	  vl_minute = (((vl_seconde / 60) + vl_p - vl_m) % 60) / vl_p * vl_p + vl_m;
	  vl_heure = ((long) vl_DateEnSeconde) - vl_seconde;
	  vl_prochaineMinute = (vl_heure + (vl_minute * 60) + 40);
	  vl_DateReveil = (XDY_Horodate) vl_prochaineMinute;

        /* armement du timer de programmation */
        if (XZSM_01DemanderProgrammation(       vl_DateReveil,
                                                C_FREQ_VERIF_EVT ,
                                                XDG_AGEN_TIMER,
                                                XDM_AS_SRVEVT_STRADA_TDP,
                                                "cle_timer_agen_tdp",
                                                XZSMC_SEC_VRAI) != XDC_OK)
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
          return (XDC_NOK);
        }

sleep(10);

	XZLG100_GetZones(0,vl_liste);

	/*initialise la memoire partagee TDP*/
	if (XZLG996_Cree_Memoire_Partagee_TDP() != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_FATAL,"agen_tdp_init: pb appel a XZLG996_Cree_Memoire_Partagee_TDP");
		return (XDC_NOK);
        }
	
	/*cree le semaphore pour la memoire partagee*/
	if (XZLG501_SemCree("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_TDP) != XDC_OK) {
			XZST_03EcritureTrace( XZSTC_FATAL,"agen_tdp_init: pb appel a XZLG501_SemCree");
			return (XDC_NOK);
	}

	/*on lit les donnees ZDP en base au demarrage*/
	agen_tdp_genere();

        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_SRVEVT_STRADA_TDP), agen_tdp_genere,(T_CB_ARG)NULL) == NULL)

        {
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_SRVEVT_STRADA non cree.",version );
                return XDC_NOK;
        }

        /* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_INFO, "OUT : agen_tdp_init   cle : %s" ,vl_cle);
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
void agen_tdp_genere()
{
  double                  vl_HorodateSec;
  int                     vl_JourSemaine;

  XZST_03EcritureTrace(XZSTC_WARNING,"agen_tdp_genere : IN");

  /*verrouille*/
  XZLG500_SemCall(-1,XZLGC_SEM_TDP);

  XZLG990_Ecrit_Memoire_Partagee_ZDP();

  XZLG983_Update_Checksum();

  XZLG500_SemCall(1,XZLGC_SEM_TDP);

/*A Lecture horodate courante systeme */
        XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
        vl_HorodateSec+=C_FREQ_VERIF_EVT;
        /* armement du timer de programmation */
  /*      if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
                                                0,
                                                XDG_AGEN_TIMER,
                                                XDM_AS_SRVEVT_STRADA_TDP,
                                                "cle_timer_tdp",
                                                XZSMC_SEC_VRAI) != XDC_OK)
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
          return ;
        }
*/
  XZST_03EcritureTrace(XZSTC_INFO,"agen_tdp_genere : OUT");
}

