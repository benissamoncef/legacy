/*E*/
/*Fichier :  $Id: egab_cfg.c,v 1.2 2018/10/26 15:57:01 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 15:57:01 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de creation des fichiers de config des equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG		13/03/20128: Creation 
* JMG	 15/01/18 : gestion reveil 1.2
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "egab_cfg.h"
#include "egab_dir.h"
#include "egab_util.h"
#include "xzao301.h"
#include "xzat71.h"
#include "xzao565.h"
#include "xzao861.h"

/* definitions de constantes */
#define CFG_NIV_TRACE_1	XZSTC_INFO
#define CFG_NIV_TRACE_2 XZSTC_DEBUG1
#define C_MODE_SUD      XDC_SENS_SUD
#define C_MODE_NORD     XDC_SENS_NORD

/* definitions de types locaux */

/* definition de macro locales */
extern int memoriser_bouchon(XDY_Entier va_numeroQ, XDY_Entier va_cleQ,
                        XDY_Entier va_numeroT, XDY_Entier va_cleT,
                        XDY_Autoroute va_autorouteQ,
                        XDY_Sens va_sensQ,
                        XDY_PR va_prQ,
                        XDY_PR va_prT,
                        XDY_Octet va_PtCarQ,
                        XDY_Octet va_PtCarT, XDY_Entier va_numero_cause,
                        XDY_Octet va_cle_cause,
                        XDY_Autoroute va_autorouteT,
                        XDY_Sens va_sensT,
                        XDY_Entier *pa_cr);
extern int bouchon_bretelle(T_Liste_ECH *pa_ech, int va_mode) ;
extern int bouchon_bretelle_debordant(T_Liste_ECH *pa_ech, int va_mode) ;
extern int bouchon_bretelle_continuant(T_Liste_ECH *pa_ech, int va_mode);


/* declaration de variables locales */
int vl_InitCfg = XDC_NOK;

/* declaration de variables globales */
T_Liste_ECH *pg_ech = NULL;
T_Liste_CAB* 		pg_CAB = NULL;
XDY_District		vg_NumeroSite;
char                 vg_idpassw[18] = "";
INFO_EQT_FEN_LCR_IP     *pg_DebutListeCABFLcr=NULL;
int vg_Mode_Fonct;
/* declaration de variables externes */
extern XDY_NomSite  vg_NomSite;
extern T_LISTE_CHAINEE *pg_liste_bouchons ;

int creer_liste_CAB (XZAOT_CAP va_cap);

int creer_liste_ech(XZAOT301_Echangeur va_ech);


int cfg_init(XDY_NomMachine        pa_NomMachine, XDY_Mot  *va_MaxEqtLigne)
{
	int			vl_ValRet = XDC_OK;
	int 		i = 0;
	XDY_Basedd      vl_Basedd_in;
	XDY_Entier     vl_Resultat;
	T_Liste_ECH *pl_ech =NULL;
	T_CAB *pl_cab1;
	XDY_Horodate vl_horodate;
	int vl_JourSemaine_Msg;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : cfg_init Simu." );

	/*A
	 * Connexion a la base de donnees
	 */
	if ( ( vl_ValRet = XZAG02_Connexion_Base( vg_NomSite ) ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet );
	   return XDC_NOK;
	}

	vg_NumeroSite=XZSC_22NumeroSiteLocal();
	XZST_03EcritureTrace( CFG_NIV_TRACE_1,"Site Local = %d" ,vg_NumeroSite);

	/*A Lecture du password de la machine (user/password) */
	if ( ( vl_ValRet = ex_lire_idpassw ( pa_NomMachine, vg_idpassw ) ) != XDC_OK )
	{
	XZST_03EcritureTrace(XZSTC_WARNING,"ex_lire_idpassw retourne %d", vl_ValRet);
	return (XDC_NOK);
	}
	/* chargement de la liste des echangeurs*/
	if (XZAO301_Liste_Echangeurs(XDC_BASE_CFG,creer_liste_ech, &vl_Resultat) != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO301_Liste_Echangeurs");
		return (XDC_NOK);
	}

	/* Chargement de la liste des CAB */
	if (XZAO861_Lire_Liste_CAP( XDC_BASE_CFG, pa_NomMachine,1,(XDY_FonctionInt)creer_liste_CAB,  &vl_Resultat) != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO861_Lire_Liste_CAP");
		return (XDC_NOK);
	}

	XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
				(int *) &vl_JourSemaine_Msg, (double *) &vl_horodate);
	/*
	pg_liste_bouchons=NULL;
	pl_ech = pg_ech;
	while (pl_ech !=NULL) {
		pl_cab1 = pl_ech->ech.cab;
		if (pl_cab1!=NULL) {
			XZAE570_Bouchons_Proches(vl_horodate,
               	         	pl_ech->ech.echangeur.NumeroAutoroute,
               	         	pl_ech->ech.echangeur.PR,
               	         	XDC_SENS_SORTIE_SUD,
               	         	pl_ech->ech.echangeur.NumeroEchangeur,
               	         	(XDY_FonctionInt)memoriser_bouchon);
			XZAE570_Bouchons_Proches(vl_horodate,
               	         	pl_ech->ech.echangeur.NumeroAutoroute,
               	         	pl_ech->ech.echangeur.PR,
               	         	XDC_SENS_SORTIE_NORD,
               	         	pl_ech->ech.echangeur.NumeroEchangeur,
               	         	(XDY_FonctionInt)memoriser_bouchon);
		  
			pl_ech->ech.cab->detection1=EGABC_NO_DETECTION;
			pl_ech->ech.cab->detection3=EGABC_NO_DETECTION;
			pl_ech->ech.cab->detection2=EGABC_NO_DETECTION;
			pl_ech->ech.cab->detection4=EGABC_NO_DETECTION;
		if (bouchon_bretelle(pl_ech,C_MODE_SUD)==XDC_OK) {
			pl_ech->ech.cab->detection1=EGABC_DETECTION;
		}
		if (bouchon_bretelle(pl_ech,C_MODE_NORD) == XDC_OK)  {
			pl_ech->ech.cab->detection2=EGABC_DETECTION;
		}
		if (bouchon_bretelle_debordant(pl_ech,C_MODE_SUD)==XDC_OK) {
			pl_ech->ech.cab->detection3=EGABC_DETECTION;
		}
		if (bouchon_bretelle_debordant(pl_ech,C_MODE_NORD)==XDC_OK) {
			pl_ech->ech.cab->detection4=EGABC_DETECTION;
		}
		if (bouchon_bretelle_continuant(pl_ech,C_MODE_SUD)==XDC_OK) {
			pl_ech->ech.cab->detection1=EGABC_DETECTION;
			pl_ech->ech.cab->detection3=EGABC_DETECTION;
		}
		if (bouchon_bretelle_continuant(pl_ech,C_MODE_NORD)==XDC_OK) {
			pl_ech->ech.cab->detection2=EGABC_DETECTION;
			pl_ech->ech.cab->detection4=EGABC_DETECTION;
		}
		}
		pl_ech = pl_ech->suivant;
	 }
	 */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : cfg_init ");
	return (XDC_OK);
}


int ec_lire_config_CAP (XDY_NomMachine                pa_NomMachine)
{
   int                          vl_ValRet = XDC_OK;
   int                          vl_Fd,i,vl_NumEqt = 0;
   char                         pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte                    pl_LigneFich;
   int                          vl_NbPICTO = 0;
	XZAOT_CAP vl_cap;
	int vl_NbParam;

sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_CAP, pa_NomMachine );

      /* Ouverture du fichier XDF_Config_CFE_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic );
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
          /*B Extraire la config CFE */
            if ( *pl_LigneFich == 'N' )
            {

		vl_NbParam = sscanf ( pl_LigneFich,      "N %hd %s %d %d %s %d",
				&vl_cap.NumeroCAP,
				vl_cap.AdresseIP,
				vl_cap.PortMaitre,
				vl_cap.PortEsclave,
				vl_cap.AdresseRgs,
				vl_cap.SiteGestion);	
		if (vl_NbParam != 6) {
			XZST_03EcritureTrace( XZSTC_WARNING, "pb decodage %s", pl_LigneFich);
			return (XDC_NOK);
		}
		creer_liste_CAB(vl_cap);
	    }
	  }
	}
        return (XDC_OK);
}

/* Creation de la fonction uilisateur de creation de la liste des PRV*/
int creer_liste_CAB (XZAOT_CAP va_cap)
{
	T_Liste_CAB *pl_NewCAB;
	T_Liste_CAB *pl_CAB;
	T_Liste_ECH *pl_ech;
	XDY_Octet                       vl_Dispo;
	XDY_Horodate                    vl_horodate;
	int                             vl_jourSemaine;
	XZEXT_MSG_SOCKET             pl_message;


	XZST_03EcritureTrace ( XZSTC_FONCTION,	"IN: creer_liste_CAB : numero=%d, autoroute=%s, type=%d, va_ech=%d,va_sens=%d",
			va_cap.NumeroCAP,
			va_cap.Autoroute,
			va_cap.Type,
			va_cap.PointCaracteristique,
			va_cap.PR,va_cap.Sens);

	if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME ,
		(int *) &vl_jourSemaine,
		(double *) &vl_horodate) != XDC_OK){
		XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante") ;
		return;
	}

	/*A Recherche de l echangeur du CAB */
	pl_ech = pg_ech;
	while (pl_ech!= NULL)
	{
		if (pl_ech->ech.echangeur.NumeroEchangeur == va_cap.PointCaracteristique)
			break;
		pl_ech=pl_ech->suivant;
	}

	if (pl_ech == NULL)
	{
		/*trace warning*/
		XZST_03EcritureTrace ( XZSTC_WARNING, "creer_liste_CAB : le CAB %d n'appartient a aucun echangeur existant (%d)!", va_cap.NumeroCAP, va_cap.PointCaracteristique);
		return XDC_NOK;
	}

	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewCAB = (T_Liste_CAB *) malloc ( sizeof(T_Liste_CAB))) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_CAB Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	pl_NewCAB->cab.config = va_cap;
	pl_ech->ech.cab = &(pl_NewCAB->cab);

	pl_NewCAB->cab.detection1 = EGABC_NO_DETECTION;
	pl_NewCAB->cab.detection2 = EGABC_NO_DETECTION;
	pl_NewCAB->cab.detection3 = EGABC_NO_DETECTION;
	pl_NewCAB->cab.detection4 = EGABC_NO_DETECTION;
	pl_NewCAB->cab.Socket = vg_SockTEMIP;
	pl_NewCAB->suivant= pg_CAB;
	pg_CAB = pl_NewCAB;


	/*demande a TEMIP de connexion a l equipement*/
	sprintf ( pl_message, "%s%s %d %c%s",
		XZEXC_CONFIG_IP,
		pl_NewCAB->cab.config.AdresseIP,
		pl_NewCAB->cab.config.PortEsclave,
		XZECC_PROT_LCR_E, 
		XZEXC_FIN_MSG_SOCK );

	sem_wait(&vg_semaphore); 
	/* Envoie de la demande d'arret */
	if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK )
	  {
	      /* Ecriture trace impossible d'emettre une demande d'arret */
	   XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret." );
        }
	sem_post(&vg_semaphore); 

        sprintf ( pl_message, "%s%s %d %c%s",
                XZEXC_CONFIG_IP,
                pl_NewCAB->cab.config.AdresseIP,
                pl_NewCAB->cab.config.PortMaitre,
                XZECC_PROT_LCR,
                XZEXC_FIN_MSG_SOCK );

		sem_wait(&vg_semaphore); 
        /* Envoie de la demande d'arret */
        if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK )
          {
              /* Ecriture trace impossible d'emettre une demande d'arret */
           XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret." );
        }
		sem_post(&vg_semaphore); 

	
	/*on envoie un ST au demarrage*/
	/*sprintf(pl_message, "ST ID=%s\0", vg_idpassw);*/
	sprintf(pl_message, "ST \0");
	EnvoyerTrameIP(
                                   pl_message,
                                   XZEXC_TRAME_STAT_COMP,
                                   pl_NewCAB->cab.config.AdresseIP,
                                   pl_NewCAB->cab.config.PortMaitre,
                                   pl_NewCAB->cab.config.AdresseRgs,
                                   XZEXC_NOCMD,
                                   XZEXC_PRIORITEFAIBLE,
                                   pl_NewCAB->cab.Socket);

	sprintf(pl_message, "DT \0");
	EnvoyerTrameIP(
                                   pl_message,
                                   XZEXC_TRAME_STAT_COMP,
                                   pl_NewCAB->cab.config.AdresseIP,
                                   pl_NewCAB->cab.config.PortEsclave,
                                   pl_NewCAB->cab.config.AdresseRgs,
                                   XZEXC_NOCMD,
                                   XZEXC_PRIORITEFAIBLE,
                                   pl_NewCAB->cab.Socket);
	XZST_03EcritureTrace ( XZSTC_FONCTION,"OUT: creer_liste_CAB");
	return (XDC_OK);
}


/* Creation de la fonction uilisateur de creation de la liste des PRV*/
int creer_liste_ech (XZAOT301_Echangeur va_ech)
{
	XZST_03EcritureTrace ( XZSTC_INTERFACE,"creer_liste_ech : va_ech=%d, va_nom=%s",
			va_ech.NumeroEchangeur,va_ech.NomEchangeur);

	T_Liste_ECH *pl_NewEch;
	T_Liste_ECH *pl_Ech;


	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewEch = (T_Liste_ECH *) malloc ( sizeof(T_Liste_ECH)) ) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_station Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	pl_NewEch->ech.echangeur = va_ech;
	pl_NewEch->ech.cab=NULL;
	pl_NewEch->ech.actif = XDC_FAUX;
	pl_NewEch->ech.detection_sud = C_PAS_BOUCHON;
	pl_NewEch->ech.detection_nord = C_PAS_BOUCHON;
	pl_NewEch->ech.bouchon = NULL;
	pl_NewEch->ech.ListeAlertes=NULL;
	pl_NewEch->suivant = pg_ech;
	pg_ech = pl_NewEch;

	XZST_03EcritureTrace ( XZSTC_FONCTION,"OUT: creer_liste_ech");
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet selon message socket recu de tesrv
*    - soit de terminer la tache
*    - soit de configurer la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    ec_controle     (       int             va_NumSock,
                                XDY_NomMachine  pa_NomMachine )
/*
* ARGUMENTS EN ENTREE :
*   va_NumSock          : Numero de la socket de TESRV
*   pa_NomMachine       : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Lecture du message socket
*   Si message d'init alors lancer la fonction cfg_init
*   Si message d'arret alors lancer la fonction ec_sortir
*
------------------------------------------------------*/
{
   static char *version = "$Id: egab_cfg.c,v 1.2 2018/10/26 15:57:01 pc2dpdy Exp $ : ec_controle" ;

   int                  vl_IndCnx = 0,
                        vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET     pl_Msg;
   XDY_Mot              vl_MaxEqtLigne = 0;

	XZST_03EcritureTrace( XZSTC_WARNING, " in ec_controle");
   /*A
    *  Lecture du message recu dans la socket appelante
    */

   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {
      /*A Si il s'agit d'une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
         /*B Arreter la tache par appel de ec_sortir */
	XZST_03EcritureTrace( XZSTC_WARNING, " in ec_controle");
         dir_sortie();
      }

      /* Si le message est correct  */
      if ( vl_LgMsg != 0 )
      {
          /*A Si il s'agit d'une demande d'arret */
          if ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
          {
             /*A Arreter la tache par appel de ec_sortir */
             dir_sortie ();
          }

          /*A Si il s'agit d'une demande d'Init */
          if ( strstr ( pl_Msg, XZEXC_DEMANDE_INIT ) != NULL )
          {
             /*A Reconfiguration de la tache par appel de ec_init */
             cfg_init ( pa_NomMachine, &vl_MaxEqtLigne );
          }
          /*A Si il s'agit du changement du mode de fonctionnement */
          if ( strstr ( pl_Msg, XZEXC_MODE_FONCT_NORMAL ) != NULL )
          {
             /*A Maj de la variable en question */
             vg_Mode_Fonct = XZEXC_MODE_NORMAL;
             ex_RecopieFichierMsg(pa_NomMachine );
          }
          if ( strstr ( pl_Msg, XZEXC_MODE_FONCT_ISOLE ) != NULL )
          {
             /*A Maj de la variable en question */
             vg_Mode_Fonct = XZEXC_MODE_ISOLE;
          }
       }
    }
}

