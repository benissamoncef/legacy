/*E*/
/* Fichier : $Id: eini_sur.c,v 1.25 2020/11/03 18:09:39 pc2dpdy Exp $     Release : $Revision: 1.25 $      Date :  $Date: 2020/11/03 18:09:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_sur.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de surveillance des taches equipement >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	13 Feb 1995	: Creation
* Mismer.D 	version 1.2 	14 Fev 1995 	: 
* Mismer.D 	version 1.3 	21 Fev 1995 	:
* Mismer.D 	version 1.4 	01 Mar 1995 	:
* Fontaine.C 	version 1.5 	19 Mai 1995 	: Reprise entete, historique
* Mismer.D 	version 1.6 	21 Jun 1995 	: Modif surveillance LT
* Mismer.D 	version 1.7 	28 Jun 1995 	: Trace abt tache isole
* Mismer.D 	version 1.6 	21 Jun 1995 	: Modif alarme analyseur
* Mismer.D 	version 1.7 	15 Nov 1995 	: Ajout initialisation varaible vl_NbAbonne
* Mismer.D 	version 1.8 	26 Avr 1996 	: Ajout traitement LT isol� (DEM/1052)
* Mismer.D      version 1.9     10 Sep 1996     : Ajout ligne direction sur RAZ PMV 4 lignes (DEM/1162)
* Mismer.D  	version 1.10	16 Sep 1996	: Modif pour extention ouest (RADT_W) 
* Mismer.D  	version 1.11	16 Sep 1996	: Modif pour extention ouest Suite (RADT_W) 
* Mismer.D	version 1.14	07 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.15	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.16	15 Jan 1998	: Modif PAL (DEM/1539)
* JMG		1.17		11/10/05	: ajout PMVA
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	23/06/09 v1.19	: renommage SPDN en SPCA DEM887
* PNI	21/09/09 v1.19&v1.20  : Analyse_Dg_Status : prise en cpte du / en debut de dg DEM910
* JMG	22/06/10	: portage Linux DEM/934  1.21
* XXX	xx/04/11	: correction surveillance datagroup  1.22
* VR	20/12/11		: Ajout PRV (DEM/1016)
* JPL	15/02/12	: Migration architecture HP ia64 : suppression avertissements mineurs (avec DEM 1018)  1.23
 * JMG	07/03/20 	: EMS 1.24
 * GR 09/07/2020        : EMS 1.25
 * GGY	08/12/23	: Ajout PIC (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "xzia.h"
#include "xzems.h"

#include "ex_mala.h"
#include "ex_msok.h"

#include "eini_cfg.h"
#include "eini_sur.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static int vm_rau=0;
static int vm_lcr = 0;
static int vm_tdp=0;
static int vm_video = 0;
static int vm_autotel =0;
static int vm_radt=0;
static int vm_dai = 0;
static int vm_tel=0;
static int vm_pmv=0;
static int vm_pmva=0;
static int vm_nav=0;
static int vm_baf=0;
static int vm_bad=0;
static int vm_prv=0;
static int vm_pic=0;
static int vm_pal=0;
static int vm_gtc=0;
static int vm_gtcnice=0;

static char *version = "$Id: eini_sur.c,v 1.25 2020/11/03 18:09:39 pc2dpdy Exp $ : eini_sur" ;

extern EINI_DONNEES_FEN_LCR *	        pg_DebListeRAU;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeQTX;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeTDP;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeVID;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeRDT;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeDAI;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListePMV;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListePICTO;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeNAV;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListePAL;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeGTC;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListePMVA;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeBAF;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeBAD;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListePRV;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListePIC;

/* declaration de fonctions internes */
int	ei_cherche_liste_type ( XDY_Eqt, EINI_DONNEES_FEN_LCR **, XDY_NomMachine, char * );
void	ei_surveillance ( XZEXT_CLE_PROG );
void                      sp_status(T_STR va_Dg, int va_Abonne);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  *
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_envoi_alarme_analyseur_DAI (	XDY_NomMachine	 pa_NomMachine,
					int		 va_Etat )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   int				vl_Fd;
   XDY_Texte			pl_LigneFich;
   XDY_Mot			vl_NumeroDAI = 0,
   				vl_NoAnaly   = 0,
   				vl_NumEqt    = 0;
   int				vl_NbParam   = 0;
   XDY_NomMachine		pl_NomMachine;
   XDY_Horodate			vl_Horodate;
   int           		vl_jourSemaine;
   XDY_District			vl_SiteGestion; 
   int           		vl_numero;
  
      XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
      /*A Recuperation du nom de la machine */
      if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
      {
	 /* Ecriture Trace */
	 XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
      } 

      /*A Ouverture du fichier de configuration des Analyseurs */
      /* Construction du nom du fichier XDF_Config_Analyseur_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Analyseur, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_Analyseur_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return (XDC_NOK);
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*A Lecture du numero d'equipement de l'analyseur */

             vl_NbParam = sscanf ( pl_LigneFich, " %hd %hd %hd %d",  
					&vl_NumeroDAI,
					&vl_NoAnaly,
					&vl_NumEqt,
					&vl_numero);
             vl_SiteGestion = vl_numero;
             
             /* Si tout les parametre ont ete lu */
             if ( vl_NbParam == 4 )
             {
                 /*B Appel de la fonction ex_env_alarme du module commun EX_MALA */
		 XZST_03EcritureTrace( XZSTC_FONCTION, "ex_env_alarme position 3");
                 ex_env_alarme  ( 	pl_NomMachine,
					vl_Horodate,
					XDC_EQT_ANA,
					vl_NumEqt,
					XZAMC_COM_TACHE_EQPT_HS,
					va_Etat,
					NULL,
					vl_SiteGestion);
             }
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );

      return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork etat d'affichage PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_RAZ_affichage_PMV ( XDY_Eqt va_Numero )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "$Id: eini_sur.c,v 1.25 2020/11/03 18:09:39 pc2dpdy Exp $ : ec_RAZ_affichage_PMV " ;
 
 XDY_Datagroup		pl_DG;
 XDY_NomSite        	pl_NomSite;
   

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 


	/*A Construction du datagroup */
	       
	XDG_EncodeDG2 ( pl_DG, XDG_ETAT_PMV, pl_NomSite) ;			

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PMV */
	if (!TipcSrvMsgWrite (  pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PMV),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, va_Numero,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO,
                          	T_IPC_FT_STR,  XZEXC_TEXTE_TACHE_PMV_ARRETEE,
                          	T_IPC_FT_INT2, XDC_PMV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO,
                          	T_IPC_FT_CHAR, XDC_PMV_PASFLASH,
                          	NULL))
	{
           XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_PMV non effectue" );
	}
	else
	{
           XZST_03EcritureTrace(XZSTC_INFO ,"Envoi message XDM_ETAT_PMV effectue" );
	}
	
	/*A Construction du datagroup */
	XDG_EncodeDG2 ( pl_DG, XDG_ETAT_PICTO, pl_NomSite) ;			

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PICTO */
	if (!TipcSrvMsgWrite (  pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PICTO),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, va_Numero,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_STR,  XDC_PICTO_PASPICTO,
                          	T_IPC_FT_INT2, XDC_PICTO_PASCLIGNO,
                          	NULL))
	{
           XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_Picto non effectue." );
	}
	else
	{
           XZST_03EcritureTrace(XZSTC_INFO ,"Envoi message XDM_ETAT_Picto effectue." );
	}
	       
	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork etat d'affichage PMVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_RAZ_affichage_PMVA ( XDY_Eqt va_Numero )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "$Id: eini_sur.c,v 1.25 2020/11/03 18:09:39 pc2dpdy Exp $ : ec_RAZ_affichage_PMV " ;
 
 XDY_Datagroup		pl_DG;
 XDY_NomSite        	pl_NomSite;
   

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 


	/*A Construction du datagroup */
	       
	XDG_EncodeDG2 ( pl_DG, XDG_ETAT_PMVA, pl_NomSite) ;			

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PMV */
	if (!TipcSrvMsgWrite (  pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PMVA),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, va_Numero,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMVA_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMVA_PASCLIGNO,
                          	T_IPC_FT_STR,  XZEXC_TEXTE_TACHE_PMVA_ARRETEE,
                          	T_IPC_FT_INT2, XDC_PMVA_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMVA_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMVA_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMVA_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMVA_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMVA_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PMVA_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PMVA_PASCLIGNO,
                          	T_IPC_FT_CHAR, XDC_PMVA_PASFLASH,
                          	NULL))
	{
           XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_PMVA non effectue" );
	}
	else
	{
           XZST_03EcritureTrace(XZSTC_INFO ,"Envoi message XDM_ETAT_PMVA effectue" );
	}
	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork etat d'affichage PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_RAZ_affichage_PRV ( XDY_Eqt va_Numero )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "@(#)eini_sur.c	1.21  12/07/10 : ec_RAZ_affichage_PRV " ;
 
 XDY_Datagroup		pl_DG;
 XDY_NomSite        	pl_NomSite;
   

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 


	/*A Construction du datagroup */
	       
	XDG_EncodeDG2 ( pl_DG, XDG_ETAT_PRV, pl_NomSite) ;			

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PRV */
	if (!TipcSrvMsgWrite (  pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PRV),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, va_Numero,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PRV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PRV_PASCLIGNO,
                          	T_IPC_FT_STR,  XZEXC_TEXTE_TACHE_PRV_ARRETEE,
                          	T_IPC_FT_INT2, XDC_PRV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PRV_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PRV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PRV_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PRV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PRV_PASCLIGNO,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_INT2, XDC_PRV_TEXTE,
                          	T_IPC_FT_STR,  "",
                          	T_IPC_FT_CHAR, XDC_PRV_PASCLIGNO,
                          	T_IPC_FT_CHAR, XDC_PRV_PASFLASH,
                          	NULL))
	{
           XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_PRV non effectue" );
	}
	else
	{
           XZST_03EcritureTrace(XZSTC_INFO ,"Envoi message XDM_ETAT_PRV effectue" );
	}
	
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork etat d'affichage PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_RAZ_affichage_PIC ( XDY_Eqt va_Numero )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version = "eini_sur.c	: ec_RAZ_affichage_PIC" ;
	XDY_Datagroup	pl_DG;
	XDY_NomSite  	pl_NomSite;
   
	/* Creation Callback Surveillance */
	/* Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace (XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 

	/* Construction du datagroup */     
	XDG_EncodeDG2 (pl_DG, XDG_ETAT_PIC, pl_NomSite) ;			

	/* Les affichages sont differents -> envoi du message XDM_ETAT_PIC */
	if (!TipcSrvMsgWrite (	pl_DG,
							XDM_IdentMsg(XDM_ETAT_PIC),
							XDM_FLG_SRVMSGWRITE,
							T_IPC_FT_INT2, va_Numero,
							T_IPC_FT_STR,  "",
							T_IPC_FT_INT2, XDC_PIC_PASFLASH,
							T_IPC_FT_STR,  XDC_PIC_TEXTE,
							NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ETAT_PIC non effectue" );
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_INFO ,"Envoi message XDM_ETAT_PIC effectue" );
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork pour reinit affichage PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_INIT_affichage_PMV ( void )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "$Id: eini_sur.c,v 1.25 2020/11/03 18:09:39 pc2dpdy Exp $ : ec_INIT_affichage_PMV " ;
 
 XDY_Datagroup		pl_DG;
 XDY_Datagroup		pl_DGInit;
 XDY_NomSite        	pl_NomSite;
  

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 


	/*A Construction du datagroup */
	       
        XDG_EncodeDG2 ( pl_DG, XDG_EInit_IHM_Picto, pl_NomSite );
	XDG_EncodeDG2 ( pl_DGInit, XDG_ETAT_PICTO, pl_NomSite);
				
        /*A Envoi du message XDM_EInit_IHM_Picto  */

        if (!TipcSrvMsgWrite (       pl_DG,
                                     XDM_IdentMsg(XDM_EInit_IHM_Picto),
                                     XDM_FLG_SRVMSGWRITE,
                                     T_IPC_FT_STR,  pl_DGInit,
                                     NULL))
        {
            /*A Trace envoi message XDM_EInit_IHM_Picto non effectue */
             XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_Picto.");
        }
        else
        {
             /*A Trace envoi message XDM_EInit_IHM_Picto */
             XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_Picto sur %s.", pl_DG );
        }

        XDG_EncodeDG2 ( pl_DG, XDG_EInit_IHM_PMV, pl_NomSite );
	XDG_EncodeDG2 ( pl_DGInit, XDG_ETAT_PMV, pl_NomSite);
				
        /*A Envoi du message XDM_EInit_IHM_PMV  */

        if (!TipcSrvMsgWrite (       pl_DG,
                                     XDM_IdentMsg(XDM_EInit_IHM_PMV),
                                     XDM_FLG_SRVMSGWRITE,
                                     T_IPC_FT_STR,  pl_DGInit,
                                     NULL))
        {
            /*A Trace envoi message XDM_EInit_IHM_PMV non effectue */
             XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PMV.");
        }
        else
        {
             /*A Trace envoi message XDM_EInit_IHM_PMV */
             XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PMV sur %s.", pl_DG );
        }
	
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork pour reinit affichage PMVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_INIT_affichage_PMVA ( void )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "$Id: eini_sur.c,v 1.25 2020/11/03 18:09:39 pc2dpdy Exp $ : ec_INIT_affichage_PMVA " ;
 
 XDY_Datagroup		pl_DG;
 XDY_Datagroup		pl_DGInit;
 XDY_NomSite        	pl_NomSite;
  

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 


        XDG_EncodeDG2 ( pl_DG, XDG_EInit_IHM_PMVA, pl_NomSite );
	XDG_EncodeDG2 ( pl_DGInit, XDG_ETAT_PMVA, pl_NomSite);
				
        /*A Envoi du message XDM_EInit_IHM_PMVA  */

        if (!TipcSrvMsgWrite (       pl_DG,
                                     XDM_IdentMsg(XDM_EInit_IHM_PMVA),
                                     XDM_FLG_SRVMSGWRITE,
                                     T_IPC_FT_STR,  pl_DGInit,
                                     NULL))
        {
            /*A Trace envoi message XDM_EInit_IHM_PMVA non effectue */
             XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PMVA.");
        }
        else
        {
             /*A Trace envoi message XDM_EInit_IHM_PMVA */
             XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PMVA sur %s.", pl_DG );
        }
	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork pour reinit affichage PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_INIT_affichage_PRV ( void )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
 static char *version = "@(#)eini_sur.c	1.21  12/07/10 : ec_INIT_affichage_PRV " ;
 
 XDY_Datagroup		pl_DG;
 XDY_Datagroup		pl_DGInit;
 XDY_NomSite        	pl_NomSite;
  

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 


        XDG_EncodeDG2 ( pl_DG, XDG_EInit_IHM_PRV, pl_NomSite );
	XDG_EncodeDG2 ( pl_DGInit, XDG_ETAT_PRV, pl_NomSite);
				
        /*A Envoi du message XDM_EInit_IHM_PRV  */

        if (!TipcSrvMsgWrite (       pl_DG,
                                     XDM_IdentMsg(XDM_EInit_IHM_PRV),
                                     XDM_FLG_SRVMSGWRITE,
                                     T_IPC_FT_STR,  pl_DGInit,
                                     NULL))
        {
            /*A Trace envoi message XDM_EInit_IHM_PRV non effectue */
             XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PRV.");
        }
        else
        {
             /*A Trace envoi message XDM_EInit_IHM_PRV */
             XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PRV sur %s.", pl_DG );
        }
	
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  envoi message RTwork pour reinit affichage PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ec_INIT_affichage_PIC (void )
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version = "eini_sur.c	: ec_INIT_affichage_PIC";
	
	XDY_Datagroup	pl_DG;
	XDY_Datagroup	pl_DGInit;
	XDY_NomSite		pl_NomSite;

	/* Creation Callback Surveillance */
	/* Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 

	XDG_EncodeDG2 (pl_DG, XDG_EInit_IHM_PIC, pl_NomSite );
	XDG_EncodeDG2 (pl_DGInit, XDG_ETAT_PIC, pl_NomSite);
				
	/* Envoi du message XDM_EInit_IHM_PIC */
	if (!TipcSrvMsgWrite (	pl_DG,
							XDM_IdentMsg(XDM_EInit_IHM_PIC),
							XDM_FLG_SRVMSGWRITE,
							T_IPC_FT_STR,  pl_DGInit,
							NULL))
	{
		/* Trace envoi message XDM_EInit_IHM_PIC non effectue */
		XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PIC.");
	}
	else
	{
		/* Trace envoi message XDM_EInit_IHM_PIC */
		XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PIC sur %s.", pl_DG );
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_surveillance (	XDY_NomMachine	 pa_NomMachine,
					int		 va_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
   EINI_MACHINE *		pl_Machine;
   int 				vl_Indice = 0;
   XDY_Datagroup		pl_NomDG;
   XDY_NomSite        		pl_NomSite;
   int				vl_ValRet = 0;
   T_BOOL			vl_WatchStatus;
   

	/*A Creation Callback Surveillance */
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 
        /* Recherche de la machine concerne */
        for ( pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
        {
           /* S'il s'agit de la machine concernee */
           if ( ( !strcmp (pl_Machine->NomMachine, pa_NomMachine) ) ||
                ( !strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES) ) )
           {
              for ( vl_Indice = 0; vl_Indice < pg_SizeTabConf; vl_Indice++)
              {
                 /* S'il s'agit de l'equipement concerne */
                 vl_ValRet = XDC_OK;
                 if ( ( pg_TabConf[vl_Indice].TypeEqt == va_TypeEqt ) || ( va_TypeEqt == XZECC_TYPE_EQT_ALL ) )
                 {
       	             if ( pl_Machine->Eqt[ec_rang_eqt(pg_TabConf[vl_Indice].TypeEqt)].Present == XZEXC_EQT_PRESENT )
                     {
                        
                        /* Selon le type d'equipement */
                        switch ( pg_TabConf[vl_Indice].TypeEqt )
                        {
                           case XZECC_TYPE_EQT_RAU :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_RAU, pl_NomSite );			
                              break;
                   
                           case XDC_EQT_GEN_LCR :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_QTX, pl_Machine->NomMachine );			
                              break;
                   
                           case XDC_EQT_TDP :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_TDP, pl_Machine->NomMachine );			
                              break;
                  
                           case XZECC_TYPE_EQT_VID :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_VIDEO, pl_NomSite );			
                              break;
                  
                           case XZECC_TYPE_EQT_TEL :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_AUTOTEL, pl_NomSite );			
                              break;
                  
                           case XZECC_TYPE_EQT_RDT :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_RADT, pl_Machine->NomMachine );			
                              break;
                  
                           case XZECC_TYPE_EQT_DAI :
                              XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_DAI, pl_Machine->NomMachine );			
                              break;
                  
                           case XZECC_TYPE_EQT_PMV :
 	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMV, pl_Machine->NomMachine );			
                              break;

                          case XZECC_TYPE_EQT_PICTO :
	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMV, pl_Machine->NomMachine );			
                             break;
                  
			   case XZECC_TYPE_EQT_PMVA :
				XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMVA, pl_Machine->NomMachine );
				break;

                           case XZECC_TYPE_EQT_NAV :
 	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_NAV, pl_Machine->NomMachine );			
                              break;

                           case XZECC_TYPE_EQT_PAL :
 	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PAL, pl_Machine->NomMachine );			
                              break;

			   case XZECC_TYPE_EQT_BAF:
			      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_BAF, pl_Machine->NomMachine );
			      break;

			   case XZECC_TYPE_EQT_BAD:
			      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_BAD, pl_Machine->NomMachine );
			      break;
				  
			   case XZECC_TYPE_EQT_PRV :
				XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PRV, pl_Machine->NomMachine );
				break;

			   case XZECC_TYPE_EQT_PIC :
				XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PIC, pl_Machine->NomMachine );
				break;

                           case XZECC_TYPE_EQT_GTC :
 	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_GTC, pl_Machine->NomMachine );			
                              break;

                           case XZECC_TYPE_EQT_NIC :
 	                      strcpy ( pl_NomDG, XDG_ECMD_GTC_NICE );			
                              break;
                                 
                           case XZECC_TYPE_EQT_SAT :
 	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SATIR, pl_Machine->NomMachine );			
                              break;

                           case XZECC_TYPE_EQT_OPE :
 	                      XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_OPER, pl_NomSite );			
                              break;

                           default: 
                              vl_ValRet = XDC_NOK;
                              break;
                       }
         
              
 	               /* pour chaque type si l'equipement est present */
 	               /*  avant verifier si watch deja armer pour cet equipement */
 	               if ( vl_ValRet == XDC_OK )
 	               {
	                  if ( !TipcMonSubjectSubscribeGetWatch( pl_NomDG, &vl_WatchStatus) )
	                  {
                             XZST_03EcritureTrace(XZSTC_WARNING, "Echec test watching Dg:%s", pl_NomDG );
	                  }
	                  else
	                  {
	                     if ( !vl_WatchStatus ) 
	                     {
	                        if ( !XDG_SurveillerDG( pl_NomDG) )
	                        {
                                   XZST_03EcritureTrace(XZSTC_WARNING, "Echec creation watching Dg:%s", pl_NomDG );
	                        }
	                     }
	                  }
	               }
	            }
	         }
	      }
	   } 
	}
 
 
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
Surveillance de TESUR de la machine voisine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void			  Analyse_Dg_Status(
	T_IPC_CONN 				   va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA 		   data,
	T_CB_ARG 				   arg)


/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*  
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{

   #ifndef _TIBCO_EMS
   T_STR			vl_Dg;
   #else
//   char             vl_Dg[100];
   #endif

   T_INT4			vl_Abonne ;
   XDY_NomSite		pl_NomSite;
   XZIAT_destinataire	vl_DestinataireSite;
   XDY_NomMachine		vl_NomMachine;

   #ifndef _TIBCO_EMS	
   T_STR        subject_name;
   T_STR        *subscribe_client_names;
   T_STR        start_client;
   T_STR        stop_client;
   #else
//   XDY_Texte    subject_name;
//   XDY_Texte    *subscribe_client_names;
//   XDY_Texte    start_client;
//   XDY_Texte    stop_client;
   #endif

   T_INT4  num_subscribe_client_names;

#ifdef _TIBCO_EMS

   /* fonction ems implementee */
   TipcMonSubjectsWatch(sp_status);


#else


  TipcMsgSetCurrent(data->msg, 0);
  if (!TipcMsgNextStr(data->msg, &vl_Dg)
      || !TipcMsgNextStrArray(data->msg, &subscribe_client_names,
                                &num_subscribe_client_names)
      || !TipcMsgNextStr(data->msg, &start_client)
      || !TipcMsgNextStr(data->msg, &stop_client))
  {
        XZST_03EcritureTrace(   XZSTC_WARNING,"Erreur reception DG_STATUS \n");
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_dgstatus_cb" );
        return;
  }
  vl_Abonne = num_subscribe_client_names;

	/*A Lecture du nombre d'abonnee */ 
	/*
	TipcMsgNextStr(data->msg,&vl_Dg) ;
	TipcMsgNextInt4(data->msg,&vl_Abonne) ;*/
	
	strcpy ( vl_NomMachine, "SP" );   

	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 
	strcpy (vl_DestinataireSite.site, pl_NomSite);
       
     /*   for ( vl_i = 0; vl_i < strlen ( vl_Dg ); vl_i ++ )
           *(vl_Dg+vl_i) = toupper ( *(vl_Dg+vl_i) );*/
/* DEM910 */
        vl_Dg++;
        XZST_03EcritureTrace( XZSTC_WARNING, "=>DG:<%s> Abt:%d.", vl_Dg, vl_Abonne  );

	sp_status( vl_Dg, vl_Abonne );
#endif
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
Surveillance de TESUR de la machine voisine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void			  sp_status(T_STR va_Dg, int va_Abonne)


/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*  
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
	char			vl_Dg[100] ;
	T_INT4			vl_Abonne ;
	XDY_Horodate		vl_Horodate;
	int           		vl_jourSemaine;
	XZEXT_MSG_SOCKET	pl_MsgTimer;
	int			vl_EqtTrouve;
   XDY_NomSite		pl_NomSite;
   XZIAT_destinataire	vl_DestinataireSite;
   EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_Debut = NULL;
   int			vl_TypeEqt = 0;
   int			vl_i       = 0;
   XDY_NomMachine		vl_NomMachine;
   XDY_NomMachine		pl_NomMachine;
   XDY_Texte		vl_MsgOperateur = "";
   XDY_Texte		vl_NomFichier   = "";
   XDY_NomEqt              vl_NomTypeEqt   = "";
        
	static int		vl_NbAbonneSATIR   = 1;
	static int		vl_NbAbonneTEL     = 1;
	static int		vl_NbAbonneGTCNice = 1;
	static int		vl_NbAbonneOPER    = 1;

   XZST_03EcritureTrace( XZSTC_WARNING, "sp_status : IN - va_Dg = [%s] len=%d - va_Abonne = %d", va_Dg, strlen(va_Dg),va_Abonne); 
	strcpy(vl_Dg,va_Dg);
	vl_Abonne=va_Abonne;
	if ( !strncmp(XDG_ECMD_RAU, 	 vl_Dg, strlen(XDG_ECMD_RAU)) ) 
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_RAU;
	   strcpy ( vl_NomMachine, va_Dg+strlen(XDG_ECMD_RAU)+1 );
      pl_Debut = pg_DebListeRAU;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache RAU      sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_QTX, 	 vl_Dg, strlen(XDG_ECMD_QTX)) )     
	{
	   vl_TypeEqt = XDC_EQT_GEN_LCR;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_QTX)+1 );
      pl_Debut = pg_DebListeQTX;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache Systeme LCR externe sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_TDP, 	 vl_Dg, strlen(XDG_ECMD_TDP)) )     
	{
	   vl_TypeEqt = XDC_EQT_TDP;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_TDP)+1 );
      pl_Debut = pg_DebListeTDP;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache Equipement UGTP (TDP) sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_VIDEO, 	 vl_Dg, strlen(XDG_ECMD_VIDEO)) )    
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_VID;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_VIDEO)+1 );
      pl_Debut = pg_DebListeVID;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache VIDEO    sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_AUTOTEL,  vl_Dg, strlen(XDG_ECMD_AUTOTEL)) )  
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_TEL;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_AUTOTEL)+1 );
	   if ( (vl_NbAbonneTEL == 0) && ( vl_Abonne > 0) )
	   {
	      sprintf ( vl_MsgOperateur, "Reprise communication tache Telephone sur %s.\0", vl_NomMachine );
	      XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
	      vl_NbAbonneTEL = vl_Abonne;
	   }
	   if ( (vl_NbAbonneTEL != 0) && ( vl_Abonne == 0) )
	   {
	      sprintf ( vl_MsgOperateur, "Perte  communication tache Telephone sur %s.\0", vl_NomMachine );
	      XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
	      vl_NbAbonneTEL = vl_Abonne;
	   }
	}
	if ( !strncmp(XDG_ECMD_RADT, 	 vl_Dg, strlen(XDG_ECMD_RADT)) )     
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_RDT;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_RADT)+1 );
      pl_Debut = pg_DebListeRDT;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache RADT      sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_DAI, 	 vl_Dg, strlen(XDG_ECMD_DAI)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_DAI;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_DAI)+1 );
      pl_Debut = pg_DebListeDAI;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache DAI       sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_PMV, 	 vl_Dg, strlen(XDG_ECMD_PMV)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_PMV;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_PMV)+1 );
      pl_Debut = pg_DebListePMV;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache PMV       sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_PMVA, 	vl_Dg, strlen(XDG_ECMD_PMVA)) )
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_PMVA;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_PMVA)+1 );
	   pl_Debut = pg_DebListePMVA;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache PMVA  sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_NAV, 	 vl_Dg, strlen(XDG_ECMD_NAV)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_NAV;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_NAV)+1 );
      pl_Debut = pg_DebListeNAV;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache NAV       sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_BAF, 	 vl_Dg, strlen(XDG_ECMD_BAF)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_BAF;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_BAF)+1 );
      pl_Debut = pg_DebListeBAF;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache BAF       sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_BAD, 	 vl_Dg, strlen(XDG_ECMD_BAD)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_BAD;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_BAD)+1 );
      pl_Debut = pg_DebListeBAD;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache BAD       sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_PRV, 	vl_Dg, strlen(XDG_ECMD_PRV)) )
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_PRV;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_PRV)+1 );
	   pl_Debut = pg_DebListePRV;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache PRV  sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_PIC, 	vl_Dg, strlen(XDG_ECMD_PIC)) )
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_PIC;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_PIC)+1 );
	   pl_Debut = pg_DebListePIC;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache PIC sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_PAL, 	 vl_Dg, strlen(XDG_ECMD_PAL)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_PAL;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_PAL)+1 );
           pl_Debut = pg_DebListePAL;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache PAL       sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_GTC, 	 vl_Dg, strlen(XDG_ECMD_GTC)) )      
	{
	   vl_TypeEqt = XZECC_TYPE_EQT_GTC;
	   strcpy ( vl_NomMachine, vl_Dg+strlen(XDG_ECMD_GTC)+1 );
           pl_Debut = pg_DebListeGTC;
	   sprintf ( vl_MsgOperateur, "Reprise communication tache mini-GTC  sur %s.\0", vl_NomMachine );
	}
	if ( !strncmp(XDG_ECMD_GTC_NICE, vl_Dg, strlen(XDG_ECMD_GTC_NICE)) ) 
	{
	   strcpy ( vl_NomMachine, "SPCA" );
	   vl_TypeEqt = XZECC_TYPE_EQT_NIC;
	   if ( (vl_NbAbonneGTCNice == 0) && ( vl_Abonne > 0) )
	   {
	      sprintf ( vl_MsgOperateur, "Reprise communication tache GTC Nice  sur %s.\0", vl_NomMachine );
	      XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
	      vl_NbAbonneGTCNice = vl_Abonne;
	   }
	   if ( (vl_NbAbonneGTCNice != 0) && ( vl_Abonne == 0) )
	   {
	      sprintf ( vl_MsgOperateur, "Perte   communication tache GTC Nice  sur %s.\0", vl_NomMachine );
	      XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
	      vl_NbAbonneGTCNice = vl_Abonne;
	   }
	}
	
	/*A Si il s'agit de la disparition d'un tache traitement */
   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
	/*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
	{
	    /* Ecriture Trace */
	    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
	} 
	
XZST_03EcritureTrace( XZSTC_INFO, "=>%s. Abonne=%d. pl_Debut=%ld.%s", vl_NomMachine,vl_Abonne,pl_Debut,vl_MsgOperateur);
        if ( vl_Abonne > 0 )
        {
         /*A Recherche de l'equipement concerne */
           for ( pl_EqtLCR = pl_Debut, vl_EqtTrouve = XDC_FAUX;
                 pl_EqtLCR != NULL;
                 pl_EqtLCR = pl_EqtLCR->Suivant )
           {

              /*A Si machine trouve */
              if ( !strcmp(pl_EqtLCR->NomLT, vl_NomMachine) )
              {
XZST_03EcritureTrace( XZSTC_INFO, "Envoi fin alarme pour eqt %d type %d de %s.",pl_EqtLCR->NumEqt,pl_EqtLCR->TypeEqt,pl_EqtLCR->NomLT);

		 XZST_03EcritureTrace( XZSTC_FONCTION, "ex_env_alarme position 4");
                 ex_env_alarme  ( 	pl_NomMachine,
					vl_Horodate,
/*!!					(vl_TypeEqt != XDC_EQT_GEN_LCR ) ? vl_TypeEqt : pl_EqtLCR->TypeEqt,  */
					pl_EqtLCR->TypeEqt,
					pl_EqtLCR->NumEqt,
					XZAMC_COM_TACHE_EQPT_HS,
					XDC_FAUX,
					NULL,
					pl_EqtLCR->SiteGestion);
                 vl_EqtTrouve = XDC_VRAI;
                 pl_EqtLCR->Abonne = vl_Abonne;
                 pl_EqtLCR->Alarme = XDC_FAUX;
	      }	
           }
           
           /*A Si reprise tache PMV alors envoi fin alarme PICTO concernes */
           if ( vl_TypeEqt == XZECC_TYPE_EQT_PMV )
           {
              ec_INIT_affichage_PMV ();
              pl_Debut = pg_DebListePICTO;
              /*A Recherche de l'equipement concerne */
              for ( pl_EqtLCR = pl_Debut;
                    pl_EqtLCR != NULL;
                    pl_EqtLCR = pl_EqtLCR->Suivant )
              {
                 /*A Si machine trouve */
                 if ( !strcmp(pl_EqtLCR->NomLT, vl_NomMachine) )
                 {
		    XZST_03EcritureTrace( XZSTC_FONCTION, "ex_env_alarme position 5");
                    ex_env_alarme  ( 	pl_NomMachine,
					vl_Horodate,
					XDC_EQT_PCT,
					pl_EqtLCR->NumEqt,
					XZAMC_COM_TACHE_EQPT_HS,
					XDC_FAUX,
					NULL,
					pl_EqtLCR->SiteGestion);
                    pl_EqtLCR->Abonne = vl_Abonne;
	         }	
              }
           }
	   else if (vl_TypeEqt == XZECC_TYPE_EQT_PMVA)
	   {
		ec_INIT_affichage_PMVA();
	   }
           
           /*A Si reprise tache DAI alors envoi fin alarme Analyseurs concernes */
           if ( (vl_TypeEqt == XZECC_TYPE_EQT_DAI) && (vl_EqtTrouve == XDC_VRAI) )
              ec_envoi_alarme_analyseur_DAI ( vl_NomMachine, XDC_FAUX );
	   
	   /*A Envoie message reprise tache traitement a l'operateur (XZIA06) */
           if ( vl_EqtTrouve == XDC_VRAI )
           {
              XZEX03_nom_eqt ( vl_TypeEqt, vl_NomTypeEqt );
              sprintf ( vl_NomFichier, "%s/%s_%s", XZEXC_PATH_FIC_ALARME, vl_NomTypeEqt, vl_NomMachine );
              if ( !access ( vl_NomFichier, F_OK ) )
              {
                 XZSS_06DetruireFichier ( vl_NomFichier);
                 XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
              }            
           }
        }
        else
        {
           /*A Recherche de l'equipement concerne */
           for ( pl_EqtLCR = pl_Debut, vl_EqtTrouve = XDC_FAUX;
                 pl_EqtLCR != NULL;
                 pl_EqtLCR = pl_EqtLCR->Suivant )
           {
              /*A Si machine trouve */
              if ( !strcmp(pl_EqtLCR->NomLT, vl_NomMachine) )
              {
                 pl_EqtLCR->Abonne = vl_Abonne;
                 vl_EqtTrouve = XDC_VRAI;
              }
           }
           
           /*A Si reprise tache PMV alors envoi fin alarme PICTO concernes */
           if ( vl_TypeEqt == XZECC_TYPE_EQT_PMV )
           {
              pl_Debut = pg_DebListePICTO;
              /*A Recherche de l'equipement concerne */
              for ( pl_EqtLCR = pl_Debut;
                    pl_EqtLCR != NULL;
                    pl_EqtLCR = pl_EqtLCR->Suivant )
              {
                 /*A Si machine trouve */
                 if ( !strcmp(pl_EqtLCR->NomLT, vl_NomMachine) )
                 {
                    pl_EqtLCR->Abonne = vl_Abonne;
                    vl_EqtTrouve = XDC_VRAI;
                 }
              }
           }
           
           if ( vl_EqtTrouve == XDC_VRAI )
           {
              sprintf ( pl_MsgTimer, "%s %lf 0 SUR_%02d_%s %s",
                		XZEXC_DEMANDE_REVEIL,
                		vl_Horodate + 10.0,
                		vl_TypeEqt,
                		vl_NomMachine,
                		XZEXC_FIN_MSG_SOCK );
              /* Envoie a la TETIM une demande reveil */      		
              sem_wait(&vg_semaphore); 
              if ( write ( vg_SockTETIM, pl_MsgTimer, strlen (pl_MsgTimer) ) <= 0 )
              {
                 sem_post(&vg_semaphore); 
                 XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );
                 sprintf ( pl_MsgTimer, "SUR_%02d_%s",
                		vl_TypeEqt,
                		vl_NomMachine );
                 ei_surveillance ( pl_MsgTimer );
              }
              else
              {
               sem_post(&vg_semaphore); 
              }
           }
        }

}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_surveillance ( XZEXT_CLE_PROG pa_CleTimer )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
   int				vl_ValRet = XDC_OK;
   EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_Debut;
   XDY_NomMachine		vl_NomMachine;
   XDY_NomMachine		pl_NomMachine;
   XDY_Horodate			vl_Horodate;
   int           		vl_jourSemaine;
   XDY_NomSite			pl_NomSite;
   XZIAT_destinataire		vl_DestinataireSite;
   int				vl_TypeEqt;
   int				vl_EqtTrouve;
   int				vl_Etat = XDC_VRAI;
   XDY_Texte			vl_MsgOperateur = "";
   XDY_Texte			vl_NomFichier   = "";
   XDY_NomEqt              	vl_NomTypeEqt   = "";

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN  ei_surveillance <%s>.", pa_CleTimer );

   /*A Analyse de la cle timer */
   if ( *pa_CleTimer == 'M' ) 
   {
      sscanf ( pa_CleTimer, "MODE_%s", vl_NomMachine );
      ei_traitement_mode_isole ( vl_NomMachine, 0 );
   }
   if ( sscanf ( pa_CleTimer+3, "_%02d_%s", &vl_TypeEqt, vl_NomMachine ) != 2 ) return;
   
   /*A Recuperation du site de la machine */
   if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
   } 
   strcpy (vl_DestinataireSite.site, pl_NomSite);

   vl_ValRet = ei_cherche_liste_type ( 	vl_TypeEqt, &pl_Debut, vl_NomMachine, vl_MsgOperateur );
         
   if ( vl_ValRet == XDC_OK )
   {
      XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
      /*A Recuperation du nom de la machine */
      if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
      {
	 /* Ecriture Trace */
	 XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
      } 
	 
      /*A Recherche de l'equipement concerne */
      for ( pl_EqtLCR = pl_Debut, vl_EqtTrouve = XDC_FAUX;
            pl_EqtLCR != NULL;
            pl_EqtLCR = pl_EqtLCR->Suivant )
      {
         /*A Si machine trouve */
         if (!strcmp(pl_EqtLCR->NomLT, vl_NomMachine))
         {
            switch ( *pa_CleTimer )
            {
              case 'S' :  /* Deconnection datagroup */
                 if ( pl_EqtLCR->Abonne < 1 )
                 {
                    vl_EqtTrouve      = XDC_VRAI;
                    pl_EqtLCR->Alarme = XDC_VRAI;
                 }
                 break;
              case 'D' :  /* Debut LT isol� */
                 vl_EqtTrouve      = XDC_VRAI;
                 pl_EqtLCR->Alarme = XDC_VRAI;
                 break;
              case 'F' :  /* Fin LT isol� */
                 vl_EqtTrouve 	    = XDC_VRAI;
                 pl_EqtLCR->Alarme  = ( pl_EqtLCR->Abonne < 1 ) ? XDC_VRAI : XDC_FAUX;
                 break;
            }
            vl_Etat = pl_EqtLCR->Alarme;
                 
            if ( vl_EqtTrouve == XDC_VRAI )
            {
               /*A Envoie de l'alarme pour l'equipement concernee */
		XZST_03EcritureTrace( XZSTC_FONCTION, "ex_env_alarme position 1");
               ex_env_alarme  ( 	pl_NomMachine,
					vl_Horodate,
/*!!					(vl_TypeEqt != XDC_EQT_GEN_LCR ) ? vl_TypeEqt : pl_EqtLCR->TypeEqt, */
					pl_EqtLCR->TypeEqt,
					pl_EqtLCR->NumEqt,
					XZAMC_COM_TACHE_EQPT_HS,
					pl_EqtLCR->Alarme,
					NULL,
					pl_EqtLCR->SiteGestion);
             if ( vl_TypeEqt == XZECC_TYPE_EQT_PMV )
	        ec_RAZ_affichage_PMV ( pl_EqtLCR->NumEqt );
             else if ( vl_TypeEqt == XZECC_TYPE_EQT_PMVA)
		ec_RAZ_affichage_PMVA (pl_EqtLCR->NumEqt);
			 else if ( vl_TypeEqt == XZECC_TYPE_EQT_PRV)
			ec_RAZ_affichage_PRV (pl_EqtLCR->NumEqt);
		 else if ( vl_TypeEqt == XZECC_TYPE_EQT_PIC)
			ec_RAZ_affichage_PIC (pl_EqtLCR->NumEqt);
	    }
         }
      }

      /*A Si arret tache PMV alors envoi debut alarme PICTO concernes */
      if ( (vl_TypeEqt == XZECC_TYPE_EQT_PMV) && (vl_EqtTrouve == XDC_VRAI) )
      {
         pl_Debut = pg_DebListePICTO;
         /*A Recherche de l'equipement concerne */
         for ( pl_EqtLCR = pl_Debut;
               pl_EqtLCR != NULL;
               pl_EqtLCR = pl_EqtLCR->Suivant )
         {
            /*A Si machine trouve */
            if ( !strcmp(pl_EqtLCR->NomLT, vl_NomMachine) )
            {
               /*A Envoie de l'alarme pour l'equipement concernee */
		XZST_03EcritureTrace( XZSTC_FONCTION, "ex_env_alarme position 2");
               ex_env_alarme  ( 	pl_NomMachine,
					vl_Horodate,
					XDC_EQT_PCT,
					pl_EqtLCR->NumEqt,
					XZAMC_COM_TACHE_EQPT_HS,
					vl_Etat,
					NULL ,
					pl_EqtLCR->SiteGestion);
            }
         }
      }

      if ( (vl_TypeEqt == XZECC_TYPE_EQT_DAI) && (vl_EqtTrouve == XDC_VRAI) )
        ec_envoi_alarme_analyseur_DAI ( vl_NomMachine, XDC_VRAI );

      /*A Envoie message reprise tache traitement a l'operateur (XZIA06) */
      if ( vl_EqtTrouve == XDC_VRAI )
      {
         XZEX03_nom_eqt ( vl_TypeEqt, vl_NomTypeEqt );
         sprintf ( vl_NomFichier, "%s/%s_%s", XZEXC_PATH_FIC_ALARME, vl_NomTypeEqt, vl_NomMachine );
         if ( (vl_Etat == XDC_VRAI) && access ( vl_NomFichier, F_OK ) )
         {
            XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
            close ( open(vl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP) );
         }            
         if ( (vl_Etat == XDC_FAUX) && !access ( vl_NomFichier, F_OK ) )
         {
            memcpy ( vl_MsgOperateur, "Reprise", 7);
            XZIA_06AffMsgSyst ( vl_DestinataireSite, vl_MsgOperateur );
            XZSS_06DetruireFichier ( vl_NomFichier);
         }            
      }
   }

   XZST_03EcritureTrace( XZSTC_FONCTION, "OUT ei_surveillance ." );


}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge d'envoyer a une tache une demande de reconnexion
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_traitement_mode_isole ( XDY_NomMachine pa_NomMachine, int va_Etat_LT )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
   int				vl_ValRet = XDC_OK;
   EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_Debut;
   EINI_MACHINE *		pl_Machine;
   int				vl_TypeEqt;
   int				vl_Indice;
   int				vl_Index;
   int				vl_TimerEnclenche = 0;
   XDY_Texte			vl_MsgOperateur = "";
   XDY_Horodate			vl_Horodate;
   int           		vl_jourSemaine;
   XZEXT_MSG_SOCKET		pl_MsgTimer;
   XDY_NomMachine		vl_NomST = "";
   XDY_Datagroup          	pl_NomDG = "";
   char                   	vl_typeqt[5];
   int				vl_mode = -1;

   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
   /* Recherche de la machine concerne */
   for ( pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
   {
      /* S'il s'agit de la machine concernee */
      if ( !strcmp (pl_Machine->NomMachine, pa_NomMachine) && (pl_Machine->TypeMachine == XZSCC_TYPEM_LT) )
      {
         /* Recherche du nom du serveur de terminaux */
         for ( vl_Indice = 0; vl_Indice < pg_SizeTabConf; vl_Indice++)
         {
            vl_TypeEqt = pg_TabConf[vl_Indice].TypeEqt;
       	    if ( (pl_Machine->Eqt[ec_rang_eqt(vl_TypeEqt)].Present == XZEXC_EQT_PRESENT) && (vl_TypeEqt == XDC_EQT_GTC) )
            {   
               for ( vl_Index = 0; vl_Index < EINI_MAX_LIGNE_PAR_EQT; vl_Index++)
                  if ( atoi(pl_Machine->LigneEqt[vl_Indice].Ligne[vl_Index].NumLigne) > 0 )
                  {
                      strcpy ( vl_NomST, pl_Machine->LigneEqt[vl_Indice].Ligne[vl_Index].NumeroST );
                      break;
                  }
               break;
            } 
         }
         vl_mode = ( (ex_presence_machine (vl_NomST) == XDC_OK) || (ex_presence_machine (pa_NomMachine) == XDC_OK) ) ? XZEXC_MODE_NORMAL : XZEXC_MODE_ISOLE;

         for ( vl_Indice = 0; vl_Indice < pg_SizeTabConf; vl_Indice++)
         {
            vl_TypeEqt = pg_TabConf[vl_Indice].TypeEqt;
       	    if ( (pl_Machine->Eqt[ec_rang_eqt(vl_TypeEqt)].Present == XZEXC_EQT_PRESENT) &&
       	         ((vl_TypeEqt == XDC_EQT_PMV) || (vl_TypeEqt == XDC_EQT_RAD) || 
       	          (vl_TypeEqt == XDC_EQT_DAI) || (vl_TypeEqt == XDC_EQT_NAV) || 
       	          (vl_TypeEqt == XDC_EQT_BAF) || (vl_TypeEqt == XDC_EQT_BAD) || 
				  (vl_TypeEqt == XDC_EQT_PRV) ||(vl_TypeEqt == XDC_EQT_BAD) ||
       	          (vl_TypeEqt == XDC_EQT_GTC) || (vl_TypeEqt == XDC_EQT_PAL) ||
				  (vl_TypeEqt == XDC_EQT_PIC)) &&
                 (ei_cherche_liste_type(vl_TypeEqt, &pl_Debut, pa_NomMachine, vl_MsgOperateur) == XDC_OK) )
            {       
               /*A Recherche de l'equipement concerne */
               for ( pl_EqtLCR = pl_Debut; pl_EqtLCR != NULL; pl_EqtLCR = pl_EqtLCR->Suivant )
               {
                  /*A Si machine trouve */
                  if ( !strncmp(pl_EqtLCR->NomLT, pa_NomMachine, strlen(pa_NomMachine)) )
                  { 
                     if ( (vl_mode == XZEXC_MODE_ISOLE ) && (pl_EqtLCR->Abonne > 0) )
                     {
                        XZEX03_nom_eqt ( vl_TypeEqt, vl_typeqt);
                        sprintf ( pl_NomDG, "ECMD_%s_%s", vl_typeqt, pa_NomMachine ) ;
                        /*A Envoi du message pour drop connect */
                        if (!TipcSrvMsgWrite (  pl_NomDG, XDM_IdentMsg(XDM_ELCR)
                          			,XDM_FLG_SRVMSGWRITE
                          			,T_IPC_FT_INT2 ,0 ,T_IPC_FT_INT2 ,0
                        		  	,T_IPC_FT_STR  ,pa_NomMachine
                        		  	,T_IPC_FT_STR  ,pa_NomMachine
                        		  	,T_IPC_FT_INT2 ,0
                        		  	,T_IPC_FT_REAL8,0.0 ,T_IPC_FT_REAL8,0.0
                        		  	,T_IPC_FT_STR  ,pa_NomMachine
                       		          	,NULL))
                       {
                          XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'emettre un message de drop connection sur %s", pl_NomDG );
                       }
                       else
                       {
                          XZST_03EcritureTrace( XZSTC_INFO, "Emission d'un message de drop connection sur %s", pl_NomDG );
                       }
                       sprintf ( pl_MsgTimer, "DEB_%02d_%s\0", vl_TypeEqt, pa_NomMachine );
                       ei_surveillance ( pl_MsgTimer );
                       if ( vl_TimerEnclenche == 0 )
                       {
                          vl_TimerEnclenche = 60;
                          sprintf ( pl_MsgTimer, "%s %lf 0 MODE_%s %s\0",
                		XZEXC_DEMANDE_REVEIL,
                		vl_Horodate + vl_TimerEnclenche,
                		pa_NomMachine,
                		XZEXC_FIN_MSG_SOCK );
                          /* Envoie a la TETIM une demande reveil */    
                          sem_wait(&vg_semaphore);   		
                          if ( write ( vg_SockTETIM, pl_MsgTimer, strlen (pl_MsgTimer) ) <= 0 )
                          {
                             XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );
                          }
                          else
                          {
                             XZST_03EcritureTrace ( XZSTC_DEBUG2, "Demande reveil pour mode isl� :%s.", pl_MsgTimer );
                          }
                          sem_post(&vg_semaphore); 
                          
                       }
                    }
                    if ( vl_mode == XZEXC_MODE_NORMAL )
                    {
                       sprintf ( pl_MsgTimer, "FIN_%02d_%s\0", vl_TypeEqt, pa_NomMachine );
                       ei_surveillance ( pl_MsgTimer );
                    }
                    break;
                  }
               }
            }
         }
         break;
      }
   }

}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_cherche_liste_type ( XDY_Eqt			va_TypeEqt,
				EINI_DONNEES_FEN_LCR	**pa_Liste,
				XDY_NomMachine		pa_NomMachine,
				char 			*pa_MsgOperateur )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
   int				vl_ValRet = XDC_OK;

   /* Selon le type d'equipement: positionner un pointeur sur le debut de la liste*/
   switch ( va_TypeEqt )
   {
      case XZECC_TYPE_EQT_RAU :
         *pa_Liste = pg_DebListeRAU;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache RAU      sur %s.\0", pa_NomMachine );
         break;
      case XDC_EQT_GEN_LCR :
         *pa_Liste = pg_DebListeQTX;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache Systeme LCR externe sur %s.\0", pa_NomMachine );
         break;
      case XDC_EQT_TDP :
         *pa_Liste = pg_DebListeTDP;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache Equipement UGTP (TDP) sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_VID :
         *pa_Liste = pg_DebListeVID;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache VIDEO    sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_RDT :
         *pa_Liste = pg_DebListeRDT;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache RADT     sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_DAI :
         *pa_Liste = pg_DebListeDAI;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache DAI      sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_PMV :
         *pa_Liste = pg_DebListePMV;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache PMV      sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_PMVA :
         *pa_Liste = pg_DebListePMVA;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache PMVA     sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_PICTO :
         *pa_Liste = pg_DebListePICTO;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache PICTO    sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_NAV :
         *pa_Liste = pg_DebListeNAV;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache NAV      sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_BAF :
         *pa_Liste = pg_DebListeBAF;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache BAF      sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_BAD :
         *pa_Liste = pg_DebListeBAD;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache BAD      sur %s.\0", pa_NomMachine );
         break;
	 case XZECC_TYPE_EQT_PRV :
         *pa_Liste = pg_DebListePRV;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache PRV     sur %s.\0", pa_NomMachine );
	      break;
	 case XZECC_TYPE_EQT_PIC :
         *pa_Liste = pg_DebListePIC;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache PIC sur %s.\0", pa_NomMachine );
	      break;
      case XZECC_TYPE_EQT_PAL :
         *pa_Liste = pg_DebListePAL;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache PAL      sur %s.\0", pa_NomMachine );
         break;
      case XZECC_TYPE_EQT_GTC :
         *pa_Liste = pg_DebListeGTC;
	 sprintf ( pa_MsgOperateur, "Perte   communication tache mini-GTC sur %s.\0", pa_NomMachine );
         break;
      default : vl_ValRet = XDC_NOK;
   }
   return ( vl_ValRet );

}
