/*E*/
/* Fichier : $Id: erdt_cfg.c,v 1.46 2021/01/06 08:44:41 akkaakka Exp $       Release : $Revision: 1.46 $        Date : $Date: 2021/01/06 08:44:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Module de configuration de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	07 Dec 1994	: Creation
* Volcic.F	version 1.2	26 Jan 1995	: Modif Config Pt Mes (v1.2)
* Volcic.F	version 1.3	28 Jan 1995	:
* Volcic.F	version 1.4	28 Jan 1995	:
* Volcic.F	version 1.5	28 Jan 1995	:
* Volcic.F	version 1.6	28 Jan 1995	:
* Volcic.F	version 1.7	13 Fev 1995	:
* Volcic.F	version 1.8	20 Mar 1995	: Modif Nom Cstes TIMER
* Volcic.F	version 1.9	30 Mar 1995	: Modif PATH fichiers config 
* Volcic.F	version 1.10	06 Avr 1995	: Modif Affectation var. vg_Timers 
* Volcic.F	version 1.11	06 Avr 1995	: Modif Affectation var. vg_Timers
* Volcic.F	version 1.12	04 Mai 1995	: Ajout Init Tableaux Mesures, Reprise entete, historique
* Mismer.D	version 1.13	07 Jun 1995	: Modif condition prise en compte nouvelle config
* Volcic.F	version 1.14	04 Jul 1995	: Modifs diverses
* Volcic.F	version 1.15	04 Jul 1995	: Modifs diverses
* Volcic.F	version 1.16	04 Jul 1995	: Modifs diverses
* Volcic.F	version 1.17	04 Jul 1995	: Modifs diverses
* Volcic.F	version 1.18	13 Oct 1995	: Modif init des tableaux Stations Amont et Aval
* Volcic.F	version 1.19	14 Nov 1995	: Suppression TipcSrvGetSock(RTServer)
* Volcic.F	version 1.20	11 Dec 1995	: Ajout xzat26
* Volcic.F	version 1.21	28 Dec 1995	: Ajout Fermeture fichier DIV
* Volcic.F	version 1.22	23 Jan 1996	: Ajout test commentaire dans fichier IdPassWord
* Mismer.D	version 1.23	05 Mar 1996	: Correction fichier de seuil niveau de traffic et config voie (DEM/1050)
* Mismer.D	version 1.24	05 Jun 1996	: Correction fichier de seuil niveau de traffic  (DEM/1037)
*						: appel de ex_lire_Idpassw a la place er_lire_Idpassw (DEM/1135)
* Mismer.D      version 1.24    15 Jul 1996     : Correction pour pb trop de station (DEM/1150)
* Volcic.F	version 1.25	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.26	19 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.27	04 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.28	21 Oct 1996	: Modif station virtuelle EQT12 (DEM/1252)
* Mismer.D	version 1.29	12 Dec 1996	: Calcul des niveaux de services pourstations virtuelles (DEM/1338)
* Mismer.D	version 1.30	29 Mai 2000	: Modif pour synchro MI entre la 10eme et la 15eme seconde (DEM/111)
* Hilmarcher    version 1.31    10 mai 2005     : Modif pour prise en compte alertes DEM465 
* Hilmarcher    version 1.32    22 juin 2005    : modif smartsockets: TipcSrvDgGetRecv devient TipcSrvSubjectGetSubscribe 
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	23/11/09	: Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908) 1.34
* JPL	24/11/09	: Restauration des numeros de version des fonctions 1.35
* JMG	12/12/10	: portage linux DEM/934 1.36
* JPL	17/03/11	: Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=)  1.37
* JPL	11/01/12	: Ajout fonction de dump de la configuration (pour DEM 997)  1.38
* VR	22/02/12	: Ajout er_lire_params_qualif() (DEM/1016)
* JMG	15/02/13	: correction stations virtuelles
* PNI		12/12/16 : Prise en compte des vitesses/PL pour les nouvelles stations en �changeur V1.42 DEM1213
* JPL	15/02/18 : Migration Linux 64 bits (DEM 1274) : Init. complete structures et lecture config RADT sans debordement  1.43
* JMG	31/10/18 : ajout DEM1306 IP 1.44
* PNI	07/12/18 : prise en compte du type de station sans trame inverse pour Toulon 1.45
* PNI	08/12/20 : time remplacer par XZSM_07 SAE_191
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "xzah26.h"
#include "xzat.h"

#include "ex_mrtf.h"

#include "erdt_cfg.h"
#include "erdt_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
XDY_Booleen	vm_Timers30Sec = XDC_FAUX;
extern XDY_Booleen	vg_mode_simu;

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int   vg_SockTEMIP;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Cette fonction effectue l'initialisation des tableaux de mesures
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_init_tab_mes (  )
			  
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*  Cette fonction effectue l'initialisation des tableaux de mesures
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_init_tab_mes" ;
   
   XDY_Entier	vl_i = 0;
   XDY_Entier	vl_j = 0;
   XDY_Entier	vl_k = 0;
   XDY_Entier	vl_l = 0;
  
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
   /*A Initialisation des mesures 1 mn */
   for ( vl_i = 0; vl_i < ERDTC_NB_DIV_MOY; vl_i ++ )
   {
      pg_DIV_Moy[vl_i] = 0;
   }
	
   for ( vl_i = 0; vl_i < ERDTC_NUM_MAX_STATION; vl_i ++)
   {
      for ( vl_j = 0; vl_j < ERDTC_NB_CHAUSSEE; vl_j ++)
      {
         pg_Nb_Voie[vl_i][vl_j] = 0;
	 pg_Nb_Donnees_Jour[vl_i][vl_j] = -1;
         pg_Nb_Q_Nuls[vl_i][vl_j] = 0;
      }
      pg_Nb_PtMes[vl_i] = 0;
      pg_Tab_RADT[vl_i] = -1;
   }
		
   for ( vl_i = 0; vl_i < ERDTC_NB_STATION; vl_i ++ )
   {
      for ( vl_j = 0; vl_j < ERDTC_NB_CHAUSSEE; vl_j ++ )
      {
	  for ( vl_k = 0; vl_k < ERDTC_NB_VOIE_CHAUS; vl_k ++ )
	  {
	       for ( vl_l = 0; vl_l < ERDTC_NB_INDEX_1MN; vl_l ++ )
	       {
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Horodate = 0.0;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Numero = 0;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Periode = ERDTC_MINUTE;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Sens = XDC_SENS_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Voie = XDC_VOIE_INCONNUE;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		  pg_ERDT_Mes_1mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
					
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Horodate = 0.0;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Numero = 0;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Periode = ERDTC_MINUTE;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Sens = XDC_SENS_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Voie = XDC_VOIE_INCONNUE;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		  pg_ERDT_Mes_6mn_gl[vl_i][vl_j][vl_k][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
	       }
	   }
       }
   }        
   
   /*B Initialisation des mesures 6 mn */
   for ( vl_i = 0; vl_i < ERDTC_NB_STATION; vl_i ++ )
   {
      for ( vl_j = 0; vl_j < ERDTC_NB_CHAUSSEE; vl_j ++ )
      {
         for ( vl_k = 0; vl_k < ERDTC_NB_VOIE_CHAUS; vl_k ++ )
         {
            for ( vl_l = 0; vl_l < ERDTC_NB_INDEX_6MN; vl_l ++ )
            {
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Numero = 0;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Periode = ERDTC_6MINUTE;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Sens = XDC_SENS_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Voie = XDC_VOIE_INCONNUE;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_6mn[vl_i][vl_j][vl_k][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
            }	
				
	 }
			
	 pg_ERDT_Mes_6mn_PL[vl_i][vl_j].Horodate = 0.0;
	 pg_ERDT_Mes_6mn_PL[vl_i][vl_j].Numero = 0;
	 pg_ERDT_Mes_6mn_PL[vl_i][vl_j].Sens = XDC_SENS_INCONNU;
	 pg_ERDT_Mes_6mn_PL[vl_i][vl_j].Cumul = 0;
	 pg_ERDT_Mes_6mn_PL[vl_i][vl_j].NB_PL = 0;
      }	
   }        
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   return (XDC_OK);	
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_
*  et met a jour le liste des structures de donnees DIV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_lire_config_DIV	(  )
			  
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Cette fonction effectue la lecture du fichier XDF_CONF_DIST_INTER_VEHI
* et memorise les 5 classes de DIV 
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_lire_config_DIV" ;
   
   XDY_FichEquext   pl_PathEtFicS;
   int vl_Fd = 0;   
   XDY_Texte		pl_Ligne;
   int vl_Nb_Param = 0;   
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
        
   /*A Initialisation du tableau des seuils par station */
   /*A Ouverture du fichier des Seuils de niveau de trafic de la station */
   sprintf ( pl_PathEtFicS, "%s/%s", ERDTC_PATH_FIC_DFT, ERDTC_FIC_DIV_DFT );
   	      
   if ( ( vl_Fd = open ( pl_PathEtFicS, O_RDONLY ) ) == -1 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s. introuvable\n", version, pl_PathEtFicS ); 
      return (XDC_NOK);
   }
   			
   while ( ex_LireLigneFich ( vl_Fd, pl_Ligne, sizeof ( pl_Ligne ) ) != XZEXC_EOF ) 
   {
      if ( *pl_Ligne != ERDTC_COMMENTAIRE )
      {
	 /*B  Extraction des DIV et Remplissage du tableau des DIV */
	 if ( ( vl_Nb_Param = sscanf ( pl_Ligne, "%d\t%d\t%d\t%d\t%d",  
					&pg_DIV[0],
					&pg_DIV[1],
					&pg_DIV[2],
					&pg_DIV[3],
					&pg_DIV[4] ) ) == 5 )
	 {
		  		
	    XZST_03EcritureTrace ( XZSTC_DEBUG3, "DIV: %d\t%d\t%d\t%d\t%d",
		  				&pg_DIV[0],
						&pg_DIV[1],
						&pg_DIV[2],
						&pg_DIV[3],
						&pg_DIV[4] );
	 }
      }
   }
   /*A Fermeture du fichier XDF_CONF_DIST_INTER_VEHI */
   XZSS_13FermerFichier( vl_Fd );
        
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_RADT_<NomMachine>
*  et met a jour le liste des structures de donnees RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_lire_config_RADT	( XDY_NomMachine pa_NomMachine )
			  
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Cette fonction effectue la lecture du fichier XDF_Config_RADT_<NomMachine>
* et met a jour le liste des structures de donnees RADT, Chaussee et Point de Mesures 
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_lire_config_RADT" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd = 0;
   XDY_FichEquext		pl_PathEtFic;
   XDY_Texte			pl_LigneFich;
   ERDT_DONNEES_RADT		vl_DonneesRADT;
   ERDT_DONNEES_RADT		*pl_DonCrt_RADT;
   XDY_Mot			vl_Liaison = 0;
   XDY_Mot			vl_Indiv = 0;
   XDY_Mot			vl_Num_Tab_RADT = 0;
   XZEXT_MSG_SOCKET             pl_message;

   int				vl_NbParam  = 0;
   int				vl_i = 0, vl_j  = 0;
   int				vl_site;
  	char vl_adr[20];

   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
      
   /*A  Ouverture du fichier de configuration des RADT */
   sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_RADT, pa_NomMachine );
      
   if ( ( vl_Fd = open ( pl_PathEtFic, O_RDONLY ) ) == -1 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_config_RADT : fichier %s. introuvable\n", pl_PathEtFic ); 
      return ( XDC_NOK );
   }
      
   /*A  Marquage des structures RADT  pour de determiner les eventuelles suppression */
   for ( pl_DonCrt_RADT = pg_debutRADT; pl_DonCrt_RADT != NULL; pl_DonCrt_RADT = pl_DonCrt_RADT->Suivant )
   {
      /* Effacement du numero de ST pour marquage */
      strcpy ( pl_DonCrt_RADT->ConfRADT.NomServeur, " ");
   }
 
   for ( vl_i = 0; vl_i < ERDTC_NUM_MAX_STATION; vl_i ++ )
   {
       for ( vl_j = 0; vl_j < ERDTC_NB_CHAUSSEE; vl_j ++ )
       {
	   pg_Nb_Voie[vl_i][vl_j] = 0;
       }
       pg_Nb_PtMes[vl_i] = 0;
       pg_Tab_RADT[vl_i] = -1;
   }

   /*A  Lecture de chaque ligne du fichier */
   while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
   {
      if ( *pl_LigneFich != ERDTC_COMMENTAIRE )
      {
	 /* Extraction de la config RADT */
	 memset ( (void *) &vl_DonneesRADT, 0, sizeof (vl_DonneesRADT) );
	 vl_NbParam = sscanf ( pl_LigneFich, "%hd %s %hd %3c %s %hd %hd %hd %hd %hd %d %d %s %d %s",  
			&vl_DonneesRADT.ConfRADT.NumeroStation,
			vl_DonneesRADT.ConfRADT.NomServeur,
			&vl_Liaison,
			vl_DonneesRADT.ConfRADT.RGS,
			vl_DonneesRADT.ConfRADT.Alertes,
			&vl_Indiv,
			&vl_DonneesRADT.ConfRADT.NumStationAmontS1,
			&vl_DonneesRADT.ConfRADT.NumStationAvalS1,
			&vl_DonneesRADT.ConfRADT.NumStationAmontS2,
			&vl_DonneesRADT.ConfRADT.NumStationAvalS2,
			&vl_site,
			&vl_DonneesRADT.ConfRADT.Port,
			vl_DonneesRADT.ConfRADT.AdresseIP,
			&vl_DonneesRADT.ConfRADT.Type,
			vl_DonneesRADT.ConfRADT.IdLCR);
             
	  vl_DonneesRADT.ConfRADT.SiteGestion=vl_site;
          if ( vl_NbParam == 15 )
          {
             /*B  Inserer dans la liste des structures config RADT */
             vl_DonneesRADT.ConfRADT.Liaison = vl_Liaison;
   	     vl_DonneesRADT.ConfRADT.DonneesIndividuelles = vl_Indiv;
             
	     /*B Initialisation des indices du tableau des RADT */
	     if ( pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumeroStation] == -1)
		pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumeroStation] = vl_Num_Tab_RADT++;
   	     if ( vl_DonneesRADT.ConfRADT.NumStationAmontS1 != 0 )
   	     {
   	       	if ( pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAmontS1] == -1)
   	       	   pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAmontS1] = vl_Num_Tab_RADT++;
   	     }
   	     if ( vl_DonneesRADT.ConfRADT.NumStationAvalS1 != 0 )
   	     {
   	       	if ( pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAvalS1] == -1)
   	       	   pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAvalS1] = vl_Num_Tab_RADT++;
   	     }
    	     if ( vl_DonneesRADT.ConfRADT.NumStationAmontS2 != 0 )
   	     {
   	       	if ( pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAmontS2] == -1)
   	       	   pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAmontS2] = vl_Num_Tab_RADT++;
   	     }
   	     if ( vl_DonneesRADT.ConfRADT.NumStationAvalS2 != 0 )
   	     {
   	       	if ( pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAvalS2] == -1)
   	       	   pg_Tab_RADT[vl_DonneesRADT.ConfRADT.NumStationAvalS2] = vl_Num_Tab_RADT++;
   	     }
   	     /* Si pas une station virtuelle alors ajout dans la liste */
/*!!   	     if ( vl_Liaison )*/
   	        er_ins_liste_RADT ( &vl_DonneesRADT, sizeof(vl_DonneesRADT.ConfRADT) );

	  	/*connnexion eventuelle a temip*/
		if (strcmp(vl_DonneesRADT.ConfRADT.AdresseIP,XZECC_IP_VIDE)) {
			/*RADT IP*/
			if (vg_SockTEMIP==-1) {
				sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,
						XDC_EQT_RAD);
				ex_cnx_client (    XZEXC_ADR_SOCK_TERDT,
							vl_adr,
							&vg_SockMask,
							 &vg_SockTEMIP);
			}
			sprintf ( pl_message, "%s%s %d %c%s",
				XZEXC_CONFIG_IP,
				vl_DonneesRADT.ConfRADT.AdresseIP,
				vl_DonneesRADT.ConfRADT.Port,
				XZECC_PROT_LCR,
				XZEXC_FIN_MSG_SOCK );
         sem_wait(&vg_semaphore); 
			if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK) {
            sem_post(&vg_semaphore); 
				XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
				return (XDC_NOK);
			}
         sem_post(&vg_semaphore); 
		}
   	  }
   	  else 
   	  {
   	     XZST_03EcritureTrace ( XZSTC_WARNING, "%s : Probleme lecture ligne fichier %s", version, pl_PathEtFic);
   	     XZSS_13FermerFichier( vl_Fd );

   	     XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   	     return(XDC_NOK);
   	  }
       }
    }
      
    /*A Fermeture du fichier XDF_Config_RADT_<NomMachine> */
    XZSS_13FermerFichier( vl_Fd );
      
    /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
    for ( pl_DonCrt_RADT = pg_debutRADT; pl_DonCrt_RADT != NULL; )
    {
       /* Si le numero de ST est effacer */
       if ( strlen ( pl_DonCrt_RADT->ConfRADT.NomServeur ) <= 1 )
       {
          /* Suppression de la liste */
          er_sup_liste_RADT ( pl_DonCrt_RADT->ConfRADT.NumeroStation );
          pl_DonCrt_RADT = pg_debutRADT;
       }
       else
       {
          /* Passage a l'element suivant */
          pl_DonCrt_RADT = pl_DonCrt_RADT->Suivant;
       }
    }
      
    /*A  Init de l'etat de service et de l'activite */
    for ( pl_DonCrt_RADT = pg_debutRADT; pl_DonCrt_RADT != NULL; pl_DonCrt_RADT = pl_DonCrt_RADT->Suivant )
    {
       pl_DonCrt_RADT->Etat_Service  = 0;
       pl_DonCrt_RADT->EtatTrans = ERDTC_EQT_LIBRE;
       pl_DonCrt_RADT->Mode = XZEXC_MODE_NORMAL;
       pl_DonCrt_RADT->ConfRADT.CapteurPoids = XDC_FAUX;
       
       XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config RADT: %d %s %d %s %s %d %d %d %d %d %d %d %d %d",  
					pl_DonCrt_RADT->ConfRADT.NumeroStation,
					pl_DonCrt_RADT->ConfRADT.NomServeur,
					pl_DonCrt_RADT->ConfRADT.Liaison,
					pl_DonCrt_RADT->ConfRADT.RGS,
					pl_DonCrt_RADT->ConfRADT.Alertes,
					pl_DonCrt_RADT->ConfRADT.DonneesIndividuelles,
					pl_DonCrt_RADT->ConfRADT.NumStationAmontS1,
					pl_DonCrt_RADT->ConfRADT.NumStationAvalS1,
					pl_DonCrt_RADT->ConfRADT.NumStationAmontS2,
					pl_DonCrt_RADT->ConfRADT.NumStationAvalS2,
					pl_DonCrt_RADT->Mode,
					pl_DonCrt_RADT->Etat_Service,
					pl_DonCrt_RADT->EtatTrans,
					pl_DonCrt_RADT->ConfRADT.SiteGestion);
   }			
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   return ( vl_ValRet );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Point_Mesure_<NomMachine>
*  et met a jour le liste des structures de donnees Points de Mesures
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_lire_config_PtMes ( XDY_NomMachine pa_NomMachine )
			  
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Cette fonction effectue la lecture du fichier XDF_Config_Point_Mesure_<NomMachine>
* et met a jour le liste des structures de donnees Points de Mesures
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_lire_config_PtMes" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd = 0;
   int				vl_FdSeuil = 0;
   XDY_FichEquext		pl_PathEtFic;
   XDY_FichEquext		pl_PathEtFicS;
   XDY_Texte			pl_LigneFich;
   XDY_Texte			pl_Ligne;
   char				pl_Contexte[3];
   ERDT_DONNEES_PtMes		vl_DonneesPtMes;
   ERDT_DONNEES_PtMes		*pl_DonCrt_PtMes;
   ERDT_DONNEES_RADT		*pl_ListeRADT;
   int				vl_NbParam  = 0;
   XDY_NomST			vl_NomST;
   XDY_Mot			vl_Sens = 0;
   XDY_Mot			vl_Anc_Voie = 0;
   XDY_Mot			vl_Boucle = 0;
   XDY_Mot			vl_BoucleInv = 0;
   XDY_Mot			vl_Voie = 0;
   XDY_Mot			vl_Poids = 0;
   int				vl_Sens_Seuils = 0;
   int				vl_Sens_Trafic = 0;
   int				vl_i  = 0;
   int				vl_j  = 0;
   int				vl_k  = 0;
   int				vl_l  = 0;
   int				vl_Seuils0 = 0;
   int				vl_Seuils1 = 0;
   int				vl_Seuils2 = 0;
   int				vl_Seuils3 = 0;
   int				vl_Seuils4 = 0;
   int				vl_Seuils5 = 0;
   int				vl_Seuils6 = 0;
   int				vl_Seuils7 = 0;
   int				vl_Seuils8 = 0;
   int				vl_Seuils9 = 0;
   int				vl_NumContexte  = 0;
   XDY_Mot			vl_Num_RADT = 0;
   XDY_Mot			vl_Anc_Num_RADT = 0;
   char				vl_Texte_Entier[20];
   int				vl_retcode = 0;
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
   /*A  Ouverture du fichier de configuration des PtMes */
   sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Point_Mesure, pa_NomMachine );
      
   if ( ( vl_Fd = open ( pl_PathEtFic, O_RDONLY ) ) == -1 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_config_PtMes : fichier %s. introuvable\n", pl_PathEtFic ); 
      return ( XDC_NOK );
   }
      
   /*A  Marquage des structures PtMes pour de determiner les eventuelles suppression */
   for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant )
   {
     /* Effacement du numero de ST pour marquage */
     pl_DonCrt_PtMes->ConfPtMes.NumeroStation = 0;
     pl_DonCrt_PtMes->ConfPtMes.Boucle = 0;
   }
       
   /*A  Lecture de chaque ligne du fichier */
   while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
   {
      if ( *pl_LigneFich != ERDTC_COMMENTAIRE )
      {
	 /*B Extraction de la config PtMes */
	 memset ( (void *) &vl_DonneesPtMes, 0, sizeof (vl_DonneesPtMes) );
	 if ( ( vl_NbParam = sscanf ( pl_LigneFich, "%hd %hd %hd %hd %hd %hd",  
					&vl_DonneesPtMes.ConfPtMes.NumeroStation,
					&vl_Sens,
					&vl_Boucle,
					&vl_BoucleInv,
					&vl_Voie,
					&vl_Poids ) ) == 6 )
	 {
	    /*B Inserer dans la liste des structures config PtMes */
            for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL ; pl_ListeRADT = pl_ListeRADT->Suivant )
              if ( vl_DonneesPtMes.ConfPtMes.NumeroStation == pl_ListeRADT->ConfRADT.NumeroStation )
                 break;
	    if ( pl_ListeRADT == NULL )
	       continue;
	       
            vl_Num_RADT = vl_DonneesPtMes.ConfPtMes.NumeroStation;
            vl_DonneesPtMes.ConfPtMes.Sens = vl_Sens;
            vl_DonneesPtMes.ConfPtMes.Boucle = vl_Boucle;
	    vl_DonneesPtMes.ConfPtMes.BoucleInv = vl_BoucleInv;
	    vl_DonneesPtMes.ConfPtMes.Voie = vl_Voie;
	    vl_DonneesPtMes.ConfPtMes.Poids = vl_Poids;
	    vl_DonneesPtMes.EtatVoie = XDC_VOIE_SENS_NORMAL; 
		  
	    if ( ( vl_retcode = er_ins_liste_PtMes ( &vl_DonneesPtMes, sizeof(vl_DonneesPtMes.ConfPtMes) ) ) == XDC_ECHEC )
	    {
	       XZST_03EcritureTrace( XZSTC_WARNING, "%s : Impossible de mettre a jour les points de mesure de la station RADT %d", version, vl_Num_RADT );
	       sprintf (vl_Texte_Entier, "%d", vl_Num_RADT);
	       if ( ( vl_retcode = XZST_10ArchiverMsgSyst ( 0 , "%s : Impossible de mettre a jour les points de mesure de la station RADT %s",
		        					 version, vl_Texte_Entier, NULL ) ) != XDC_OK )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING,"%s : Appel XZST_10ArchiverMsgSyst retourne %d", version, vl_retcode );
               }
	       return (XDC_NOK);
            }
        	  
            /*B Si le point de mesure existe deja on continue a lire le fichier de config des points de mesure */
            if ( vl_retcode == XDC_MODIF )
               continue;
        	       
            /*B Comptage du nombre de points de mesures et de voies par station */
            if ( vl_Boucle >= 0 && vl_BoucleInv != vl_Boucle && vl_BoucleInv !=ERDTC_BOUCL_SS_TRAME)
	       pg_Nb_PtMes[vl_Num_RADT] += 2;
		
            if ( vl_Boucle >= 0 && vl_BoucleInv != vl_Boucle && vl_BoucleInv ==ERDTC_BOUCL_SS_TRAME)
	       pg_Nb_PtMes[vl_Num_RADT] ++;

	    if ( vl_Boucle >= 0 && vl_BoucleInv == vl_Boucle )
	       pg_Nb_PtMes[vl_Num_RADT] ++;
		
            if ( vl_Sens >= 1 && vl_Sens <= 8 )
	       pg_Nb_Voie[vl_Num_RADT][vl_Sens] ++;
		
	    /*B Si le numero de station est different du precedent */
	    if ( vl_Num_RADT != vl_Anc_Num_RADT )
	    {
	       /*B Initialisation des Niveaux de Service */
	       for ( vl_j = 0; vl_j < ERDTC_NB_CHAUS_SEUIL; vl_j ++ )
		  for ( vl_k = 0; vl_k < ERDTC_NB_TYPE_NIVEAU; vl_k ++ )
		      pg_Niveau[pg_Tab_RADT[vl_Num_RADT]][vl_j][vl_k] = XZERC_NIV_FLUIDE;
	       vl_Anc_Num_RADT = vl_Num_RADT;
   	    }
                   
   	    /*B Initialisation des mesures 1 mn */
   	    for ( vl_l = 0; vl_l < ERDTC_NB_INDEX_1MN; vl_l ++ )
   	    {
	        pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Numero = vl_Num_RADT;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Periode = ERDTC_MINUTE;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Sens = vl_Sens;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Voie = XDC_VOIE_SENS_NORMAL;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
	 	pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
					
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Numero = vl_Num_RADT;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Periode = ERDTC_MINUTE;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Sens =  vl_Sens;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Voie = XDC_VOIE_SENS_NORMAL;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
		
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Numero = vl_Num_RADT;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Periode = ERDTC_MINUTE;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Sens = vl_Sens;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Voie = XDC_VOIE_SENS_NORMAL;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
					
	 	pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Numero = vl_Num_RADT;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Periode = ERDTC_MINUTE;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Sens =  vl_Sens;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Voie = XDC_VOIE_SENS_NORMAL;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
		    
		  XZST_03EcritureTrace( XZSTC_DEBUG2, "%s : Init Circul Voie : %d\t%d\t%d\t%d", version, 
		    						pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Voie,
		                                                pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Voie,
		                                                pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Voie,
		                                                pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Voie  );
	    }

   	    /*B Initialisation des mesures 6 mn */
	    for ( vl_l = 0; vl_l < ERDTC_NB_INDEX_6MN; vl_l ++ )
   	    {
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Numero = vl_Num_RADT;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Periode = ERDTC_6MINUTE;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Sens = vl_Sens;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Voie = XDC_VOIE_SENS_NORMAL;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
					
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Horodate = 0.0;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Numero = vl_Num_RADT;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Periode = ERDTC_6MINUTE;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Sens = vl_Sens;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Voie = XDC_VOIE_SENS_NORMAL;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValV.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
		pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][0][vl_l].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
					
	     }

           }
   	   else 
   	   {
   	      XZST_03EcritureTrace ( XZSTC_WARNING, "%s : Probleme lecture ligne fichier %s", version, pl_PathEtFic);
   	      XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   	      return(XDC_NOK);
   	   }
	    
   	   /*B Initialisation des mesures PL*/
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Horodate = 0.0;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Numero = vl_Num_RADT;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Sens = vl_Sens;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Cumul = 0;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].NB_PL = 0;
					
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][0].Horodate = 0.0;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][0].Numero = vl_Num_RADT;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][0].Sens = vl_Sens;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][0].Cumul = 0;
	   pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][0].NB_PL = 0;
	}
     }
      
     /*A  Fermeture du fichier XDF_Config_PtMes_<NomMachine> */
     XZSS_13FermerFichier( vl_Fd );
      
     /*B Initialisation du tableau des seuils par station
         Ouverture du fichier des Seuils de niveau de trafic de la station */
     for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL ; pl_ListeRADT = pl_ListeRADT->Suivant )
     {
        vl_Num_RADT = pl_ListeRADT->ConfRADT.NumeroStation;
        sprintf ( pl_PathEtFicS, "%s/%s_R%d%s", ERDTC_PATH_FIC_SNT, ERDTC_FIC_SNT, vl_Num_RADT, ERDTC_EXT_FIC_TXT );
      
   	if ( ( vl_FdSeuil = open ( pl_PathEtFicS, O_RDONLY ) ) == -1 )
  	{
           XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s. introuvable\n", version, pl_PathEtFicS ); 
        	 
           /*B Si le fichier des Seuils de niveau de trafic de la station n'existe pas => 
       	       Ouverture du fichier des Seuils de niveau de trafic par defaut */
           sprintf ( pl_PathEtFicS, "%s/%s", ERDTC_PATH_FIC_SNT, ERDTC_FIC_SNT_DFT );
           if ( ( vl_FdSeuil = open ( pl_PathEtFicS, O_RDONLY ) ) == -1 )
  	   {
  	      XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s. introuvable\n", version, pl_PathEtFicS ); 
              return ( XDC_NOK );
           }
   	}
   			
   	vl_NumContexte = 0;
   	vl_Sens_Seuils = 0;
   	   
   	/*B Lecture du fichier des Seuils de niveau de trafic de la station */
   	while ( ex_LireLigneFich ( vl_FdSeuil, pl_Ligne, sizeof ( pl_Ligne ) ) != XZEXC_EOF ) 
      	{
           if ( *pl_Ligne != ERDTC_COMMENTAIRE )
           {
	      if ( vl_NumContexte >= 5 )
	      {
	   	 vl_NumContexte = 0;
	   	 if ( vl_Sens_Seuils == 1 ) break;
	   	 else vl_Sens_Seuils = 1;
	      }
	   			 		
	      /*B Extraction des seuils de niveau de trafic et Remplissage du tableau des seuils de trafic */
	      if ( ( vl_NbParam = sscanf ( pl_Ligne, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",  
								&vl_Sens_Trafic,
								pl_Contexte,
								&vl_Seuils0,
								&vl_Seuils1,
								&vl_Seuils2,
								&vl_Seuils3,
								&vl_Seuils4,
								&vl_Seuils5,
								&vl_Seuils6,
								&vl_Seuils7,
								&vl_Seuils8,
								&vl_Seuils9 ) ) == 12 )
	       {
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][0] = vl_Seuils0;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][1] = vl_Seuils1;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][2] = vl_Seuils2;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][3] = vl_Seuils3;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][4] = vl_Seuils4;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][5] = vl_Seuils5;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][6] = vl_Seuils6;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][7] = vl_Seuils7;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][8] = vl_Seuils8;
		  pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][9] = vl_Seuils9;
		  		
		  XZST_03EcritureTrace ( XZSTC_DEBUG3, "Seuils Niveau Service: %d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		  						vl_Sens_Trafic,
								pl_Contexte,
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][0],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][1],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][2],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][3],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][4],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][5],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][6],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][7],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][8],
								pg_Seuils[pg_Tab_RADT[vl_Num_RADT]][vl_Sens_Seuils][vl_NumContexte][9] );
		   vl_NumContexte ++;
	   	}
	        else 
   	        {
   	           XZST_03EcritureTrace ( XZSTC_WARNING, "%s : Probleme lecture ligne fichier %s", version, vl_FdSeuil);
   	           break;
   	        }
	     }
         }
         /*A Fermeture du fichier des Seuils de niveau de trafic de la stationdu fichier */
         XZSS_13FermerFichier( vl_FdSeuil );
      }
   
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( pl_DonCrt_PtMes->ConfPtMes.NumeroStation <= 0 && pl_DonCrt_PtMes->ConfPtMes.Boucle <= 0 )
         {
            /* Suppression de la liste */
            er_sup_liste_PtMes ( pl_DonCrt_PtMes->ConfPtMes.NumeroStation );
            pl_DonCrt_PtMes = pg_debutPtMes;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant;
         }
      }
     
      /*A Ecriture trace de la config pour debug et init de l'etat des voies */
      for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes  = pl_DonCrt_PtMes->Suivant )
      {
          pl_DonCrt_PtMes->EtatVoie  = XDC_VOIE_SENS_NORMAL;
          
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config PtMes: %d %d %d %d %d %d %d",  
					pl_DonCrt_PtMes->ConfPtMes.NumeroStation,
					pl_DonCrt_PtMes->ConfPtMes.Sens,
					pl_DonCrt_PtMes->ConfPtMes.Boucle,
					pl_DonCrt_PtMes->ConfPtMes.BoucleInv,
					pl_DonCrt_PtMes->ConfPtMes.Voie,
					pl_DonCrt_PtMes->ConfPtMes.Poids,
					pl_DonCrt_PtMes->EtatVoie);
      }			
      XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
      return ( vl_ValRet );
}

   

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Cette fonction effectue la mise a jour de la liste des structures de donnees 
*  Chaussees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_lire_config_Chaussee ( XDY_NomMachine pa_NomMachine )
			  
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*:678

* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Cette fonction effectue la mise a jour de la liste des structures de donnees 
* Chaussees
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_lire_config_Chaussee" ;
   
   int				vl_ValRet = XDC_OK;
   int				vl_premier_Chaussee = 0;
   ERDT_DONNEES_PtMes		*pl_DonCrt_PtMes;
   ERDT_DONNEES_Chaussee	vl_DonneesChaussee;
   ERDT_DONNEES_Chaussee	*pl_DonCrt_Chaussee;
   int				vl_Anc_Station = 0;
   int				vl_Anc_Sens = 0;
  
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);

   memset ( (void *) &vl_DonneesChaussee, 0, sizeof (vl_DonneesChaussee) );
      
   /*A Marquage des structures Chaussee pour de determiner les eventuelles suppression */
   for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; pl_DonCrt_Chaussee = pl_DonCrt_Chaussee->Suivant )
   {
      /* Effacement du numero de ST pour marquage */
      pl_DonCrt_Chaussee->ConfChaussee.NumeroStation = 0;
   }
 
   vl_premier_Chaussee = XDC_VRAI;
      
   if ( vl_premier_Chaussee == XDC_FAUX )
   {
      /*B Inserer dans la liste des structures config Chaussee */
      er_ins_liste_Chaussee ( &vl_DonneesChaussee, sizeof(vl_DonneesChaussee.ConfChaussee) );
   }
   else
   {
      vl_premier_Chaussee = XDC_FAUX;
   }
      
   /*A Remplissage des structures des chaussees en fonction des RADT et des points de mesures */
   for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant )
   {
      if ( pl_DonCrt_PtMes->ConfPtMes.NumeroStation != vl_Anc_Station ) 
      {
         vl_Anc_Station = vl_DonneesChaussee.ConfChaussee.NumeroStation;
         vl_Anc_Sens = 0;
      }
             
      if ( pl_DonCrt_PtMes->ConfPtMes.Sens != vl_Anc_Sens )
      {
         vl_DonneesChaussee.ConfChaussee.NumeroStation  = pl_DonCrt_PtMes->ConfPtMes.NumeroStation;
         vl_DonneesChaussee.ConfChaussee.Sens	    = pl_DonCrt_PtMes->ConfPtMes.Sens;
                 
          /*B Inserer dans la liste des structures config Chaussee */
          er_ins_liste_Chaussee ( &vl_DonneesChaussee, sizeof(vl_DonneesChaussee.ConfChaussee) );
          vl_Anc_Sens    = vl_DonneesChaussee.ConfChaussee.Sens;
      }
   }
      
   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; )
   {
      /* Si le numero de ST est effacer */
      if ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation <= 0 )
      {
         /* Suppression de la liste */
         er_sup_liste_Chaussee ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation );
         pl_DonCrt_Chaussee = pg_debutChaussee;
      }
      else
      {
         /* Passage a l'element suivant */
         pl_DonCrt_Chaussee = pl_DonCrt_Chaussee->Suivant;
      }
   }
     
   /*A Ecriture trace de la config pour debug et init du sens des differentes voies */
   vl_Anc_Sens = 0;
   for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; pl_DonCrt_Chaussee= pl_DonCrt_Chaussee->Suivant )
   {
      if ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation == vl_Anc_Station ) 
      {
	 vl_Anc_Sens = 0;
	 vl_Anc_Station = vl_DonneesChaussee.ConfChaussee.NumeroStation;
      }

      if  ( pl_DonCrt_Chaussee->ConfChaussee.Sens != vl_Anc_Sens )
      {
	  pg_Nb_Chaussees[pl_DonCrt_Chaussee->ConfChaussee.NumeroStation] ++;
	  vl_Anc_Sens = pl_DonCrt_Chaussee->ConfChaussee.Sens;
      }
          
      XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Chaussee: %d %d",  
					pl_DonCrt_Chaussee->ConfChaussee.NumeroStation,
					pl_DonCrt_Chaussee->ConfChaussee.Sens );
   }	   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   return (XDC_OK);
}  



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_CONF_RADT_VIRTUELLE
*  et met a jour le liste des structures des RADT virtuelles
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_lire_RADT_Virtuelle	( void )
			  
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Cette fonction effectue la lecture du fichier XDF_CONF_RADT_VIRTUELLE
* et met a jour le liste des structures des RADT virtuelles 
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_lire_RADT_Virtuelle" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd = 0;
   int				vl_Index = 0;
   XDY_FichEquext		pl_PathEtFic;
   XDY_Texte			pl_LigneFich;
   ERDT_CONF_RADT_VIRTUELLE	vl_DonneesRADT;
   int				vl_NbParam  = 0;
   int				vl_Chaussee1, vl_Chaussee2, vl_Chaussee3;
   ERDT_DONNEES_RADT		*pl_DonCrt_RADT;
   ERDT_DONNEES_Chaussee	*pl_DonCrt_Chaussee;
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
      
   /*A  Ouverture du fichier de configuration des RADT */
   sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_CONF_RADT_VIRTUELLE );
      
   if ( ( vl_Fd = open ( pl_PathEtFic, O_RDONLY ) ) == -1 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_config_RADT : fichier %s. introuvable\n", pl_PathEtFic ); 
      return ( XDC_OK );
   }

   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for ( vl_Index = 0; vl_Index < ERDTC_NUM_MAX_STATION; vl_Index++ )
   {
       /* Suppression de la liste */
       pg_Tab_RADT_Virtuelle[vl_Index].NumStation1 = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].NumStationVirtuelle = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].Chaussee1 = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].Chaussee2 = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].NumStation2 = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].NumStation3 = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].Chaussee3 = 0;
       pg_Tab_RADT_Virtuelle[vl_Index].Sens = 0;
   }

   /*A  Lecture de chaque ligne du fichier */
   while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
   {
      if ( *pl_LigneFich != ERDTC_COMMENTAIRE )
      {
	 /* Extraction de la config RADT */
	 vl_NbParam = sscanf ( pl_LigneFich, "%hd %hd %hd %hd %hd %hd %hd",  
			&vl_DonneesRADT.NumStationVirtuelle,
			&vl_DonneesRADT.NumStation1,
			&vl_DonneesRADT.Chaussee1,
			&vl_DonneesRADT.NumStation2,
			&vl_DonneesRADT.Chaussee2,
			&vl_DonneesRADT.NumStation3,
			&vl_DonneesRADT.Chaussee3);
          XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_config_RADT :donnees%d: %d %d %d %d %d %d %d",pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle],
		vl_DonneesRADT.NumStationVirtuelle,
	  	vl_DonneesRADT.NumStation1,
		vl_DonneesRADT.Chaussee1,
		vl_DonneesRADT.NumStation2,
		vl_DonneesRADT.Chaussee2,
		vl_DonneesRADT.NumStation3,
		vl_DonneesRADT.Chaussee3);

          if ( vl_NbParam == 7 )
          {
             /*A Recherche si la station est sur ce local */
             vl_Chaussee1 = vl_Chaussee2 = vl_Chaussee3 = XDC_FAUX;
	     if (vl_DonneesRADT.NumStation2==-1)
		vl_Chaussee2 = XDC_VRAI;
	     if (vl_DonneesRADT.NumStation3==-1)
		vl_Chaussee3 = XDC_VRAI;
             for ( pl_DonCrt_RADT = pg_debutRADT; pl_DonCrt_RADT != NULL; pl_DonCrt_RADT = pl_DonCrt_RADT->Suivant ) {
                if ( pl_DonCrt_RADT->ConfRADT.NumeroStation == vl_DonneesRADT.NumStation1 )
                {
                   for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; pl_DonCrt_Chaussee = pl_DonCrt_Chaussee->Suivant ) {
      	              if ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation == vl_DonneesRADT.NumStation1 ) 
      	              {
      	                 if ( pl_DonCrt_Chaussee->ConfChaussee.Sens == abs(vl_DonneesRADT.Chaussee1) ) {
								vl_Chaussee1 = XDC_VRAI;
						}
                      }
		    }
		}
		if (vl_DonneesRADT.NumStation2!=-1)
		  if ( pl_DonCrt_RADT->ConfRADT.NumeroStation == vl_DonneesRADT.NumStation2 )
                  {
                     for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; 
					pl_DonCrt_Chaussee = pl_DonCrt_Chaussee->Suivant ) {
                        if ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation == vl_DonneesRADT.NumStation2 )
                        {
                           if ( pl_DonCrt_Chaussee->ConfChaussee.Sens == abs(vl_DonneesRADT.Chaussee2) ) vl_Chaussee2 = XDC_VRAI;
                        }
		     }
                  }
               	if (vl_DonneesRADT.NumStation3 != -1) 
		  if ( pl_DonCrt_RADT->ConfRADT.NumeroStation == vl_DonneesRADT.NumStation3 )
                  {
                     for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; 
					  pl_DonCrt_Chaussee = pl_DonCrt_Chaussee->Suivant ) {
                        if ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation == vl_DonneesRADT.NumStation3 )
                        {
                           if ( pl_DonCrt_Chaussee->ConfChaussee.Sens == abs(vl_DonneesRADT.Chaussee3) ) vl_Chaussee3 = XDC_VRAI;
                        }
		    }
                  }
	XZST_03EcritureTrace ( XZSTC_WARNING, "vl_Chaussee1 %d, vl_Chaussee2 %d, vl_Chaussee3 %d   %d",vl_Chaussee1 , vl_Chaussee2, vl_Chaussee3,pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]);
                   if ( vl_Chaussee1 && vl_Chaussee2 && vl_Chaussee3)
                   {
	XZST_03EcritureTrace ( XZSTC_WARNING, "vl_Chaussee1 %d, vl_Chaussee2 %d, vl_Chaussee3 %d   %d/%d/%d/%d",vl_Chaussee1 , vl_Chaussee2, vl_Chaussee3,pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle],pg_Tab_RADT[vl_DonneesRADT.NumStation1],pg_Tab_RADT[vl_DonneesRADT.NumStation2],pg_Tab_RADT[vl_DonneesRADT.NumStation3]);
                      /*B  MAJ de la structures config RADT virtulle */
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].NumStationVirtuelle = vl_DonneesRADT.NumStationVirtuelle;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].NumStation1 = vl_DonneesRADT.NumStation1;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].Chaussee1 = vl_DonneesRADT.Chaussee1;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].NumStation2 = vl_DonneesRADT.NumStation2;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].Chaussee2 = vl_DonneesRADT.Chaussee2;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].NumStation3 = vl_DonneesRADT.NumStation3;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].Chaussee3 = vl_DonneesRADT.Chaussee3;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStationVirtuelle]].Sens = vl_DonneesRADT.Chaussee1;
                      pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStation1]].NumStation1 = vl_DonneesRADT.NumStationVirtuelle;
		      if (vl_DonneesRADT.NumStation2!=-1)
                        pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStation2]].NumStation2 = vl_DonneesRADT.NumStationVirtuelle;
		      if (vl_DonneesRADT.NumStation3!=-1)
                        pg_Tab_RADT_Virtuelle[pg_Tab_RADT[vl_DonneesRADT.NumStation3]].NumStation3 = vl_DonneesRADT.NumStationVirtuelle;
                   }
		}
   	  }
   	  else 
   	  {
   	     XZST_03EcritureTrace ( XZSTC_WARNING, "%s : Probleme lecture ligne fichier %s", version, pl_PathEtFic);
   	     XZSS_13FermerFichier( vl_Fd );

   	     XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   	     return(XDC_NOK);
   	  }
       }
    }
      
    /*A Fermeture du fichier XDF_CONF_RADT_VIRTUELLE */
    XZSS_13FermerFichier( vl_Fd );
      
    /*A  Init de l'etat de service et de l'activite */
    for ( vl_Index = 0; vl_Index < ERDTC_NUM_MAX_STATION; vl_Index++ )
    {
       if (pg_Tab_RADT_Virtuelle[vl_Index].NumStationVirtuelle==0) {
		pg_Tab_RADT_Virtuelle[vl_Index].NumStation1=0;
		pg_Tab_RADT_Virtuelle[vl_Index].NumStation2=0;
		pg_Tab_RADT_Virtuelle[vl_Index].NumStation3=0;
       }
       if ( pg_Tab_RADT_Virtuelle[vl_Index].NumStation1 != 0 )
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config RADT Virtuelle(%d) : %d %d %d %d %d %d %d",
					vl_Index,
                                        pg_Tab_RADT_Virtuelle[vl_Index].NumStationVirtuelle,
                                        pg_Tab_RADT_Virtuelle[vl_Index].NumStation1,
                                        pg_Tab_RADT_Virtuelle[vl_Index].Chaussee1,
                                        pg_Tab_RADT_Virtuelle[vl_Index].NumStation2,
                                        pg_Tab_RADT_Virtuelle[vl_Index].Chaussee2,
					 pg_Tab_RADT_Virtuelle[vl_Index].NumStation3,
					pg_Tab_RADT_Virtuelle[vl_Index].Chaussee3 );
   }
 
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   return ( vl_ValRet );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Cette fonction effectue la mise a jour du type de station de la liste des structures de donnees 
*  RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_recherche_type_RADT ( )
			  
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Cette fonction effectue la mise a jour du champ "TypeStation" de la liste des structures de donnees 
* RADT
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_recherche_type_RADT" ;
   
   ERDT_DONNEES_RADT		*pl_DonCrt_RADT;
   ERDT_DONNEES_PtMes		*pl_DonCrt_PtMes;
   ERDT_DONNEES_Chaussee	*pl_DonCrt_Chaussee;
   XDY_Booleen			pl_Tab_Sens[9];
   int				vl_Type = 0;
   int				vl_Sens = 0;
  
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
   
   /*B Initialisation des variables temporaires */
   for ( vl_Sens = 0; vl_Sens < ERDTC_NB_CHAUSSEE; vl_Sens ++ )
       pl_Tab_Sens[vl_Sens] = XDC_FAUX;
   
   /*A Recherche pour chaque RADT dans la liste des Chaussees des differents sens de la station */
   for ( pl_DonCrt_RADT = pg_debutRADT; pl_DonCrt_RADT != NULL; pl_DonCrt_RADT = pl_DonCrt_RADT->Suivant )
   {
      for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant )
         if ( pl_DonCrt_PtMes->ConfPtMes.Poids && (pl_DonCrt_PtMes->ConfPtMes.NumeroStation == pl_DonCrt_RADT->ConfRADT.NumeroStation) ) 
             pl_DonCrt_RADT->ConfRADT.CapteurPoids = XDC_VRAI;
         
      for ( pl_DonCrt_Chaussee = pg_debutChaussee; pl_DonCrt_Chaussee != NULL; pl_DonCrt_Chaussee = pl_DonCrt_Chaussee->Suivant )
      {
      	 if ( pl_DonCrt_Chaussee->ConfChaussee.NumeroStation == pl_DonCrt_RADT->ConfRADT.NumeroStation )
      	    pl_Tab_Sens[pl_DonCrt_Chaussee->ConfChaussee.Sens] = XDC_VRAI;
      }
      /*B S'il existe un sens sud, on verifie que ce n'est pas une gare de peage */
      if ( pl_Tab_Sens[XDC_SENS_SUD] )
      {
         for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant )
         {
            if ( ( pl_DonCrt_PtMes->ConfPtMes.NumeroStation == pl_DonCrt_RADT->ConfRADT.NumeroStation ) &&
                 ( pl_DonCrt_PtMes->ConfPtMes.Sens == XDC_SENS_SUD ) )
      	    {
      	       /*B Si la boucle inverse est differente de la boucle normale => ce n'est pas une gare de peage */
               if ( pl_DonCrt_PtMes->ConfPtMes.BoucleInv != pl_DonCrt_PtMes->ConfPtMes.Boucle ) {
            	  vl_Type += ERDTC_CHAUSSEE;
		  /* si le no de boucle inv = ERDTC_BOUCL_SS_TRAME(99) alors il s'agit d'une station TOULON qui ne renvoie pas les donn�es pour la boucle inverse */
		  if (pl_DonCrt_PtMes->ConfPtMes.BoucleInv == ERDTC_BOUCL_SS_TRAME)
		  	pl_DonCrt_PtMes->ConfPtMes.BoucleInv= pl_DonCrt_PtMes->ConfPtMes.Boucle;
		}
               /*B Si la boucle inverse est egale a la boucle normale => c'est une gare de peage */
               else 
            	  vl_Type += ERDTC_PEAGE;
               break;
            }
         }
       }
      	 
      /*B S'il existe un sens nord et pas de sens sud, on verifie que ce n'est pas une gare de peage */
      if ( ( pl_Tab_Sens[XDC_SENS_NORD] ) && ( !pl_Tab_Sens[XDC_SENS_SUD] ) )
      {
         for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant )
         {
            if ( ( pl_DonCrt_PtMes->ConfPtMes.NumeroStation == pl_DonCrt_RADT->ConfRADT.NumeroStation ) &&
                 ( pl_DonCrt_PtMes->ConfPtMes.Sens == XDC_SENS_NORD ) )
      	    {
      	       /*B Si la boucle inverse est differente de la boucle normale => ce n'est pas une gare de peage */
               if ( pl_DonCrt_PtMes->ConfPtMes.BoucleInv != pl_DonCrt_PtMes->ConfPtMes.Boucle )
	       {
            	  vl_Type += ERDTC_CHAUSSEE;
		  /* si le no de boucle inv = ERDTC_BOUCL_SS_TRAME(99) alors il s'agit d'une station TOULON qui ne renvoie pas les donn�es pour la boucle inverse */
                  if (pl_DonCrt_PtMes->ConfPtMes.BoucleInv == ERDTC_BOUCL_SS_TRAME)
                          pl_DonCrt_PtMes->ConfPtMes.BoucleInv= pl_DonCrt_PtMes->ConfPtMes.Boucle;

               }
	       /*B Si la boucle inverse est egale a la boucle normale => c'est une gare de peage */
               else 
            	  vl_Type += ERDTC_PEAGE;
               break;
            }
         }
       }

      /*B S'il existe un sens entree ou sortie echangeur c'est un echangeur */
      if ( ( pl_Tab_Sens[XDC_SENS_ENTREE_NORD] ) || ( pl_Tab_Sens[XDC_SENS_ENTREE_SUD] ) ||
           ( pl_Tab_Sens[XDC_SENS_SORTIE_NORD] ) || ( pl_Tab_Sens[XDC_SENS_SORTIE_SUD] ) ) {
         for ( pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant )
         {
            if ( ( pl_DonCrt_PtMes->ConfPtMes.NumeroStation == pl_DonCrt_RADT->ConfRADT.NumeroStation ) &&
                 ( pl_DonCrt_PtMes->ConfPtMes.Sens != XDC_SENS_NORD )&&
                 ( pl_DonCrt_PtMes->ConfPtMes.Sens != XDC_SENS_SUD ))
      	    {
      	       /*B Si la boucle inverse est differente de la boucle normale => ce n'est pas une gare de peage */
	   	XZST_03EcritureTrace ( XZSTC_WARNING,"Station echangeur %d sens:%d boucle:%d boucleinv:%d ",pl_DonCrt_RADT->ConfRADT.NumeroStation,pl_DonCrt_PtMes->ConfPtMes.Sens,pl_DonCrt_PtMes->ConfPtMes.Boucle,pl_DonCrt_PtMes->ConfPtMes.BoucleInv);
               if ( pl_DonCrt_PtMes->ConfPtMes.BoucleInv == pl_DonCrt_PtMes->ConfPtMes.Boucle ) {
 	         vl_Type += ERDTC_ECHANGEUR;
	   	XZST_03EcritureTrace ( XZSTC_WARNING,"Station echangeur %d  ",pl_DonCrt_RADT->ConfRADT.NumeroStation);
		}
	       else
	       {
                   /* si le no de boucle inv = ERDTC_BOUCL_SS_TRAME(99) alors il s'agit d'une station TOULON qui ne renvoie pas les donn�es pour la boucle inverse */
                   if (pl_DonCrt_PtMes->ConfPtMes.BoucleInv == ERDTC_BOUCL_SS_TRAME)
                              pl_DonCrt_PtMes->ConfPtMes.BoucleInv= pl_DonCrt_PtMes->ConfPtMes.Boucle;
	   	XZST_03EcritureTrace ( XZSTC_WARNING,"Station echangeur %d nouvelle generation : idem chaussee ",pl_DonCrt_RADT->ConfRADT.NumeroStation);
		}
               break;
            }
         }


	/*if (pl_DonCrt_RADT->ConfRADT.NumeroStation<194)
         vl_Type += ERDTC_ECHANGEUR;
	else
	   XZST_03EcritureTrace ( XZSTC_WARNING,"Station echangeur %d nouvelle generation : idem chaussee ",pl_DonCrt_RADT->ConfRADT.NumeroStation);*/
           
      }

      /*B Suivant le type trouve on en deduit le type de la station RADT */
      pl_DonCrt_RADT->ConfRADT.TypeStation = vl_Type;
      
      XZST_03EcritureTrace ( XZSTC_DEBUG3, "%s : RADT = %d\tType_Station = %d",
                                version, pl_DonCrt_RADT->ConfRADT.NumeroStation, pl_DonCrt_RADT->ConfRADT.TypeStation);
       
      /*B Remise a zero des variables temporaires */
      for ( vl_Sens = 0; vl_Sens < ERDTC_NB_CHAUSSEE; vl_Sens ++ )
         pl_Tab_Sens[vl_Sens] = XDC_FAUX;
      vl_Type = 0;
   }
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
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

void	er_controle	( 	int 		va_NumSock,
				XDY_NomMachine	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket de TESRV
*   pa_NomMachine	: Nom de la machine
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
*   Si message d'init alors lancer la fonction er_init
*   Si message d'arret alors lancer la fonction er_sortir
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_controle" ;

   int			vl_IndCnx = 0;
   int			vl_LgMsg  = 0;
   int			vl_retcode = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
   
   /*A Lecture du message recu dans la socket appelante */
   if ( ( vl_retcode = ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) ) != XDC_OK )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /*A S'il s'agit d'une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
         /*B Arreter la tache par appel de er_sortir */
         er_sortir ();
      }
   
      /*A Si le message est correct */
      if ( vl_LgMsg != 0 ) 
      {
  	  /*A S'il s'agit d'une demande d'arret */
  	  if ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
  	  {
  	     /*A Arreter la tache par appel de er_sortir */
  	     er_sortir ();
  	  }

  	  /*A S'il s'agit d'une demande d'Init */
  	  if ( strstr ( pl_Msg, XZEXC_DEMANDE_INIT ) != NULL )
  	  {
  	     /*A Reconfiguration de la tache par appel de er_init */
  	     er_init ( pa_NomMachine );
  	  }
  	  
  	  /*A S'il s'agit du changement du mode de fonctionnement */
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
    XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Effectue l'initialisation de la tache TERDT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	er_init	( XDY_NomMachine  pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomMachine	: Nom de la machine
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
*
*   Appel de ex_mini
*   Lecture des fichier de configuration des RADT et des Points de Mesures
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*   Abonnements aux datgroups 6mn des LTVoisins
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_init" ;

   ERDT_DONNEES_RADT 		*pl_DonCrt;
   ERDT_DONNEES_RADT 		*pl_Pointeur_Liste;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   XDY_Datagroup		pl_DG_Am;
   XDY_Datagroup		pl_DG_Av;
   T_BOOL          		vl_EtatDg = FALSE;
   int				vl_SockProt = 0;
   XDY_Eqt			vl_NumEqtAmontS1 = 0;
   XDY_Eqt			vl_St_Am_LocalS1 = XDC_FAUX;
   XDY_Eqt			vl_NumEqtAmontS2 = 0;
   XDY_Eqt			vl_St_Am_LocalS2 = XDC_FAUX;
   XDY_Eqt			vl_NumEqtAvalS1 = 0;
   XDY_Eqt			vl_St_Av_LocalS1 = XDC_FAUX;
   XDY_Eqt			vl_NumEqtAvalS2 = 0;
   XDY_Eqt			vl_St_Av_LocalS2 = XDC_FAUX;
   int				vl_ValRet = 0;
   int				vl_retcode = 0;

   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
   
   /*A Lecture des fichiers de configuration et remplissage des structure de donnees */ 
   /*A Configuration des RADT (appel de la fonction er_lire_config_RADT) */
   if ( ( vl_retcode = er_lire_config_RADT ( pa_NomMachine ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_config_RADT retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Configuration des Points de Mesures (appel de la fonction er_lire_config_PtMes) */
   if ( ( vl_retcode = er_lire_config_PtMes ( pa_NomMachine ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_config_PtMes retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Configuration des Chaussees (appel de la fonction er_lire_config_Chaussee) */
   if ( ( vl_retcode = er_lire_config_Chaussee ( pa_NomMachine ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_config_Chaussee retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Configuration des Chaussees virtuelle (appel de la fonction er_lire_RADT_Virtuelle) */
   if ( ( vl_retcode = er_lire_RADT_Virtuelle (  ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_RADT_Virtuelle retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Configuration des type de station (appel de la fonction er_recherche_type_RADT) */
   if ( ( vl_retcode = er_recherche_type_RADT ( ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_recherche_type_RADT retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Configuration des parametres de qualification*/
   if ( ( vl_retcode = er_lire_params_qualif ( ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_params_qualif retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Appel Init Configutation des voies (appel de la fonction xzah26.c) */
   if ( ( vl_retcode = XZAH26_InitRadtDai (pa_NomMachine ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"%s: xzah26_InitRadtDai retourne %d", version,vl_retcode);
	return (XDC_NOK);
   }
    
   /*A Appel Init Configutation etat (appel de la fonction xzat26.c) */
   if ( ( vl_retcode = xzat26_Init_Etats_Eqts ( XDC_EQT_RAD, XDC_ETAT_TOUS_EQT, pa_NomMachine ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"%s: xzat26_Init_Etats_Eqts retourne %d", version,vl_retcode);
	return (XDC_NOK);
   }
   /*A Si LT <> ext ouest */
   if (!vg_Mac_Ext_Ouest)
   {
      /*A Configuration des DIV (appel de la fonction er_lire_config_DIV) */
      if ( ( vl_retcode = er_lire_config_DIV ( ) ) != XDC_OK )
      {
   	 XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_config_DIV retourne %d", vl_retcode);
	 return (XDC_NOK);
      }
   }

   /*B Dump de la configuration apres sa lecture */
   /*er_dump_config ();*/


   /*A Lecture du password de la machine (user/password) */
   if ( ( vl_retcode = ex_lire_idpassw ( pa_NomMachine, vg_idpassw ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: er_lire_idpassw retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   XZST_03EcritureTrace( XZSTC_INFO, "Pwd:<%s> .", vg_idpassw );	
   
   /*A Ouverture du fichier d'alarme */
   if ( ( vl_retcode = ex_OuvertureFichierMsg(vg_NomMachine) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"er_init: ex_OuvertureFichierMsg retourne %d", vl_retcode);
	return (XDC_NOK);
   }
   
   /*A Connexion socket en mode client pour communiquer avec la tache TELMi */
   /*A Pour toutes les RADT recherche de l'existence de la tache protocole associee */ 
   for ( pl_DonCrt = pg_debutRADT; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      
      if ( pl_DonCrt->ConfRADT.Liaison > 0 )
      {
         /*A Construction du nom de la tache protocole telmi */
         sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrt->ConfRADT.Liaison );
         sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_RDTTELM, pl_DonCrt->ConfRADT.Liaison );
      
	 if (strcmp(pl_DonCrt->ConfRADT.AdresseIP,XZECC_IP_VIDE)) {
	 	pl_DonCrt->Socket = vg_SockTEMIP;
		
	}

         /*A Recherche de l'existence de la socket sur tout le masque des sockets */
         for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
         {
	    if (!strcmp(pl_DonCrt->ConfRADT.AdresseIP,XZECC_IP_VIDE)) {
            /*A Si la socket existe => Mise a jour du numero de socket et Sortie de la boucle */
            if ( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
            {
               pl_DonCrt->Socket = pl_Socket->desc_sock;
               break;
            }
	    }
         }
      
         /*A Si la socket avec la tache telmi n'existe pas */
	 if (!strcmp(pl_DonCrt->ConfRADT.AdresseIP,XZECC_IP_VIDE)) {
         if ( pl_Socket == NULL )
         {
            /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
            if ( ( vl_retcode = ex_cnx_client ( pl_TacheTrt,
						pl_TacheProt,
						&vg_SockMask,
						&vl_SockProt ) ) != XDC_OK )
	    {
	       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );	
	    }
	    else
	    {
	       /*A Memorisation du numero de socket */
	       pl_DonCrt->Socket = vl_SockProt;
	    }	
	 }	
	 }
      }
   }
   
   
   /*A
   ** Abonnement aux datagroups voisins :
   ** XDG_EMES_RADT_6MN_R<N³EqtVoisin> : Mesures LT voisin
   ** XDG_EMES_RADT_6GL_R<N³EqtVoisin> : Mesures LT voisin
   **
   ** Abonnement aux datagroups voisins :
   ** XDG_EMES_RADT_6MN_R<N³EqtVoisin>
   ** Pour la Station Amont 
   **
   ** Pour toutes les RADT recherche des stations Amont et Aval
   ** Abonnements aux datagroups si l'abonnement n'existe pas encore 
   */
   
   for ( pl_DonCrt = pg_debutRADT; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      
      if ( pl_DonCrt->ConfRADT.Liaison > 0 )
      {
         /*A Affectation des variables locales vl_NumEqtAmont et vl_NumEqtAval */
         vl_NumEqtAmontS1 = pl_DonCrt->ConfRADT.NumStationAmontS1;
         vl_NumEqtAvalS1  = pl_DonCrt->ConfRADT.NumStationAvalS1;
         vl_NumEqtAmontS2 = pl_DonCrt->ConfRADT.NumStationAmontS2;
         vl_NumEqtAvalS2  = pl_DonCrt->ConfRADT.NumStationAvalS2;
         pl_Pointeur_Liste = pl_DonCrt;
      
         /*A Abonnements aux Datagroup XDG_EMES_RADT_6MN_V<Numero> Voisins si ceux ci n'existent pas */
         for ( pl_DonCrt = pg_debutRADT, vl_St_Am_LocalS1 = vl_St_Av_LocalS1 = vl_St_Am_LocalS2 = vl_St_Av_LocalS2 = XDC_FAUX; 
	       pl_DonCrt != NULL; 
	       pl_DonCrt = pl_DonCrt->Suivant )
         { 
	    if ( vl_NumEqtAmontS1 == pl_DonCrt->ConfRADT.NumeroStation )
	       vl_St_Am_LocalS1 = XDC_VRAI;
	    if ( vl_NumEqtAvalS1 == pl_DonCrt->ConfRADT.NumeroStation )
               vl_St_Av_LocalS1 = XDC_VRAI;
	    if ( vl_NumEqtAmontS2 == pl_DonCrt->ConfRADT.NumeroStation )
	       vl_St_Am_LocalS2 = XDC_VRAI;
	    if ( vl_NumEqtAvalS2 == pl_DonCrt->ConfRADT.NumeroStation )
               vl_St_Av_LocalS2 = XDC_VRAI;
         }
      
         /*A Construction du nom du DataGroup pour la station Amont si celle-ci est non nulle */
         if ( ( vl_NumEqtAmontS1 != 0 ) && ( vl_St_Am_LocalS1 != XDC_VRAI ) )
         {
               sprintf ( pl_DG_Am, "%s%d", XDG_EMES_RADT_V, vl_NumEqtAmontS1 );
      
               /*B Joindre ce datagroup s'il n'est pas deja joint */
               TipcSrvSubjectGetSubscribe ( pl_DG_Am, &vl_EtatDg );
      
               if ( !vl_EtatDg )
               {
      	           if ( !XDG_JoindreDG( pl_DG_Am ) )
        	   {
              		XZST_03EcritureTrace ( XZSTC_WARNING, "er_init : Echec abonnement au Datagroup %s ", pl_DG_Am );
              		vl_ValRet = XDC_NOK;
           	   }
           	   else
           	   {
              		XZST_03EcritureTrace ( XZSTC_DEBUG3, "er_init : Abonnement au Datagroup %s ", pl_DG_Am );
           	   }
               }
         }
         if ( ( vl_NumEqtAmontS2 != 0 ) && ( vl_St_Am_LocalS2 != XDC_VRAI ) )
         {
            sprintf ( pl_DG_Am, "%s%d", XDG_EMES_RADT_V, vl_NumEqtAmontS2 );
      
            /*B Joindre ce datagroup s'il n'est pas deja joint */
            TipcSrvSubjectGetSubscribe ( pl_DG_Am, &vl_EtatDg );
      
            if ( !vl_EtatDg )
            {
      		if ( !XDG_JoindreDG( pl_DG_Am ) )
        	{
              		XZST_03EcritureTrace ( XZSTC_WARNING, "er_init : Echec abonnement au Datagroup %s ", pl_DG_Am );
              		vl_ValRet = XDC_NOK;
           	}
           	else
           	{
              		XZST_03EcritureTrace ( XZSTC_DEBUG3, "er_init : Abonnement au Datagroup %s ", pl_DG_Am );
           	}
            }
         }
      
         /*B Construction du nom du DataGroup pour la station Aval si celle-ci est non nulle */
         if ( ( vl_NumEqtAvalS1 != 0 ) && ( vl_St_Av_LocalS1 != XDC_VRAI ) )
         {
            sprintf ( pl_DG_Av, "%s%d", XDG_EMES_RADT_V, vl_NumEqtAvalS1 );
      
            /*B Joindre ce datagroup s'il n'est pas deja joint */
            TipcSrvSubjectGetSubscribe ( pl_DG_Av, &vl_EtatDg );
      
            if ( !vl_EtatDg )
            {
      		if ( !XDG_JoindreDG( pl_DG_Av ) )
        	{
              		XZST_03EcritureTrace ( XZSTC_WARNING, "er_init : Echec abonnement au Datagroup %s ", pl_DG_Av );
              		vl_ValRet = XDC_NOK;
           	}
           	else
           	{
              		XZST_03EcritureTrace ( XZSTC_DEBUG3, "er_init : Abonnement au Datagroup %s ", pl_DG_Av );
           	}
            }
         }
         if ( ( vl_NumEqtAvalS2 != 0 ) && ( vl_St_Av_LocalS2 != XDC_VRAI ) )
         {
            sprintf ( pl_DG_Av, "%s%d", XDG_EMES_RADT_V, vl_NumEqtAvalS2 );
      
            /*B Joindre ce datagroup s'il n'est pas deja joint */
            TipcSrvSubjectGetSubscribe ( pl_DG_Av, &vl_EtatDg );
      
            if ( !vl_EtatDg )
            {
      		if ( !XDG_JoindreDG( pl_DG_Av ) )
        	{
              		XZST_03EcritureTrace ( XZSTC_WARNING, "er_init : Echec abonnement au Datagroup %s ", pl_DG_Av );
              		vl_ValRet = XDC_NOK;
           	}
           	else
           	{
              		XZST_03EcritureTrace ( XZSTC_DEBUG3, "er_init : Abonnement au Datagroup %s ", pl_DG_Av );
           	}
            }
          }
          pl_DonCrt = pl_Pointeur_Liste;
      }
  }
  XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
  return (XDC_OK);    
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Effectue les operations de sortie de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_sortir ( )

/*
* ARGUMENTS EN ENTREE :
*   aucun
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
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_sortir" ;

   ERDT_DONNEES_RADT 		*pl_RADT;
   ERDT_DONNEES_RADT 		*pl_RADTALiberer;
   ERDT_DONNEES_PtMes		*pl_PtMes;
   ERDT_DONNEES_PtMes		*pl_PtMesALiberer;
   ERDT_DONNEES_Chaussee	*pl_Chaussee;
   ERDT_DONNEES_Chaussee	*pl_ChausseeALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat = 0;
   ssize_t			vl_retcode = 0;

   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
   
   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2 ( pl_NomDG, "ERDT_JOURNUIT_RADT", vg_NomMachine) ;			
   
   XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT,pl_NomDG,er_journuit,vg_NomSite,&vl_resultat);

   /*A Liberation des ressources memoires */

   /*A Liberation des ressources memoires des donnees des RADT */
   for ( pl_RADT = pg_debutRADT; pl_RADT != NULL; free(pl_RADTALiberer) )
   {
      if ( pl_RADT->Don_HisRADT.Fd != NULL ) fclose ( pl_RADT->Don_HisRADT.Fd );
      pl_RADTALiberer = pl_RADT;
      pl_RADT         = pl_RADT->Suivant;
   }  
   
   /*A Liberation des ressources memoires des donnees Point de Mesures */
   for ( pl_PtMes = pg_debutPtMes; pl_PtMes != NULL; free(pl_PtMesALiberer) )
   {
      pl_PtMesALiberer = pl_PtMes;
      pl_PtMes         = pl_PtMes->Suivant;
   } 
      
   /*A Liberation des ressources memoires des donnees Chaussee */
   for ( pl_Chaussee = pg_debutChaussee; pl_Chaussee != NULL; free(pl_ChausseeALiberer) )
   {
      pl_ChausseeALiberer = pl_Chaussee;
      pl_Chaussee         = pl_Chaussee->Suivant;
   } 
      
   /*A Envoie du message d'arret aux taches protocoles */
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TELM, strlen(XZEXC_ADR_SOCK_TELM) ) )
      {
         /*A Construction du message d'arret */
         sprintf ( pl_message, "%s%s%s", XZEXC_DEMANDE_ARRET, pl_Socket->adresse, XZEXC_FIN_MSG_SOCK );

         /*A Envoie de la demande d'arret */
         sem_wait(&vg_semaphore); 
         if ( ( vl_retcode = write ( pl_Socket->desc_sock, pl_message, strlen (pl_message) ) ) != strlen (pl_message) )
         { 
            XZST_03EcritureTrace(XZSTC_WARNING, "er_sortir : Impossible d'emettre une demande d'arret" );
         }
         sem_post(&vg_semaphore); 

      }
   }
   
   /*A Attente de 3 secondes pour l'arret de la tache */
   sleep ( XZEXC_ATTENTE_ARRET_TACHE );
      
   /*A Fermeture de toutes les sockets */
	 
   /*B Pour toutes les sockets du masque */
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      /*B Fermeture de la socket courante */
      ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );

   /*A Fin de la tache  exit (XDC_OK) */
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   exit (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Effectue les enclenchement des Timers Cycliques
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_Lancement_Timer ( int va_SockTIM )

/*
* ARGUMENTS EN ENTREE :
*   aucun
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
*   Arme les timers 30s 1mn et 6mn
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_Lancement_Timer" ;

   ldiv_t		vl_Result_Div;
   time_t		vl_time = 0;
   XDY_Horodate		vl_horodateSec	= 0.0;
   XZSMT_Horodate	vl_heure_string;
	
   XZEXT_MSG_SOCKET	pl_MsgTimer;
   ERDT_DONNEES_RADT 	*pl_listeRADT;

   int			vl_Minutes	= 0;
   int			vl_Secondes	= 0;
   XDY_Entier		vl_retcode	= 0;
    int		vl_i;

   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
	
   /*A Demande de l'heure courante */
   /*time ( &vl_time );
   vl_horodateSec = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_i, &vl_horodateSec );
   /*A Conversion de l'heure recue (en secondes) en date (char[20]) */
   if ( ( vl_retcode = XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heure_string ) ) != XDC_OK )
   {
      if ( ( vl_retcode = XZST_10ArchiverMsgSyst ( 0 , "erdt_cfg : Probleme pour conversion heure RTWORKS, la tache TREDT ne peut pas demarer", NULL ) ) != XDC_OK )
      {	
         XZST_03EcritureTrace(XZSTC_WARNING,"%s : Appel XZST_10ArchiverMsgSyst retourne ", version, vl_retcode );
      }
      XZST_03EcritureTrace(XZSTC_FATAL,"%s : Appel XZSM_11ConversionHorodate retourne ", version, vl_retcode );
   }
	
   vl_Minutes = atoi ( &vl_heure_string[ERDTC_DIXMINUTE_STRING] );
   vl_Secondes = atoi ( &vl_heure_string[ERDTC_SECONDE_STRING] );
   vl_Result_Div = ldiv ( vl_Minutes, 6 );
		
   XZST_03EcritureTrace(XZSTC_DEBUG3,"%s : Minutes = %d \t\t\t Reste ResultDiv = %d retourne ", version, vl_Minutes, vl_Result_Div.rem);
	      			
   /*A Si 0 <= Nb_Sec <= 5 et Timer 1 min non enclenche => enclenchement du Timer 1Min */
   if ( ( vl_Secondes >= 0 ) && ( vl_Secondes <= 5 ) && ( vg_TimersMin == XDC_FAUX ) )
   { 
       /*A Si LT <> ext ouest */
       if (!vg_Mac_Ext_Ouest)
       {
	   /*B Programation du reveil toutes les minutes de l'heure pleine */
	   ETIM_REVEIL_CYCLIQUE ( ERDTC_REVEIL_1MN, ERDTC_TIMER_1MN, va_SockTIM, pl_MsgTimer )
       }

       /*B Affectation de la variable  vg_TimersMin a faux */
       vg_TimersMin = XDC_VRAI;
   }
   /*A Si 10 <= Nb_Sec <= 15 et Timer 30 seconde non enclenche => enclenchement du Timer 30 secondes */
   if ( ( vl_Secondes >= 10 ) && ( vl_Secondes <= 15 ) && ( vm_Timers30Sec == XDC_FAUX ) )
   {

	  /*B Programation du reveil toutes les 30s  */
	 ETIM_REVEIL_CYCLIQUE ( ERDTC_REVEIL_30S, ERDTC_TIMER_30S, va_SockTIM, pl_MsgTimer )

	/*B Affectation de la variable  vm_Timers30Sec a vrai */
       vm_Timers30Sec = XDC_VRAI;
    }


   /*A Si Nb_Min n'est pas un multiple de 6min  => enclenchement du Timer 30Min */
   if ( ( vl_Result_Div.rem != 0 ) && ( vl_Secondes >= 15 ) && ( vl_Secondes <= 20 ) && ( vg_Timers30Min == XDC_FAUX ) )
   {
      ETIM_REVEIL_CYCLIQUE ( ERDTC_REVEIL_30MN, ERDTC_TIMER_30MN, va_SockTIM, pl_MsgTimer )
      vg_Timers30Min = XDC_VRAI;
      XZST_03EcritureTrace(XZSTC_DEBUG3,"%s : Timer 30 min = %d ", version, vg_Timers30Min);
   }
   	
   /*A Si 5 <= Nb_Sec <= 10 et Timer 6 min non enclenche => enclenchement du Timer 6Min */
   if (!vg_mode_simu) {
	   if ( ( vl_Result_Div.rem == 0 ) && ( vl_Secondes >= 5 ) && ( vl_Secondes <= 10 ) && ( vg_Timers6Min == XDC_FAUX ) )
	   {
		   ETIM_REVEIL_CYCLIQUE ( ERDTC_REVEIL_6MN, ERDTC_TIMER_6MN, va_SockTIM, pl_MsgTimer )
    		  vg_Timers6Min = XDC_VRAI;
		   XZST_03EcritureTrace(XZSTC_DEBUG3,"%s : Timer 6 min = %d ", version, vg_Timers6Min);
	   }
   }
   else {
	   if (vg_Timers6Min == XDC_FAUX ) {
		   ETIM_REVEIL_CYCLIQUE ( ERDTC_REVEIL_6MN, ERDTC_TIMER_6MN, va_SockTIM, pl_MsgTimer )
	       vg_Timers6Min = XDC_VRAI;
	   }
   }

   /*A Si tous les Timers sont enclenches => positionne Timers_Enclenches a vrai */
   if ( ( vg_TimersMin == XDC_VRAI ) && ( vg_Timers6Min == XDC_VRAI ) && ( vg_Timers30Min == XDC_VRAI ) )
   {		
	vg_Timers_Enclenches = XDC_VRAI;
   }
   
   if ( vg_Timers_Enclenches == XDC_VRAI )
   {	            
	/*B Programation du reveil a minuit (0h00) */
        ETIM_SYNCHRO_DATE ( XZEXC_HEURE_MINUIT, ERDTC_TIMER_MINUIT, vg_SockTIM, pl_MsgTimer )
	
 	/*B Programation de l'heure de la synchro date */
	ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, ERDTC_TIMER_SYNCHRO, vg_SockTIM, pl_MsgTimer )
	
   	/*B Pour tous les RADT de la machine => demande mesure 6mn %PL et demande mesure 6mn Q V TO si la RADT n'est pas HS */
	 if (vg_mode_simu) {
		    XZST_03EcritureTrace( XZSTC_WARNING, "mode SIMU");
		     XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_i, &vl_horodateSec );
		    er_lit_fichier_simu(vl_horodateSec);
	    return ;
	  }
	else  {
        for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
        {
             /*B demande mesure 6mn %PL et demande mesure 6mn Q V TO */ 
             if ( !(pl_listeRADT->Etat_Service & XDC_EQT_HORS_SRV) && (pl_listeRADT->ConfRADT.Liaison > 0) )
             {
                 /*B demande mesure 6mn PL si le type de station n'est pas de type echangeur */ 
                 if ( pl_listeRADT->ConfRADT.TypeStation != ERDTC_ECHANGEUR ) {
		      if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP,XZECC_IP_VIDE))
                      	er_EnvoyerTrame(	"MB LC",
                                   	XZEXC_TRAME_MES_6MN_PL,
                                   	pl_listeRADT->ConfRADT.RGS,
                                   	XZEXC_NOCMD,
	                           	XZEXC_PRIORITEFAIBLE,
                                   	pl_listeRADT->Socket );
		  	else
				EnvoyerTrameIP("MB LC",
					XZEXC_TRAME_MES_6MN_PL,
					pl_listeRADT->ConfRADT.AdresseIP,
					pl_listeRADT->ConfRADT.Port,
					pl_listeRADT->ConfRADT.RGS,
					XZEXC_NOCMD,
					XZEXC_PRIORITEFAIBLE,
					pl_listeRADT->Socket);
                  }

                  sleep ( 1 );
                       
                  /*B demande mesure 6mn Q V TO */ 
		  if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP,XZECC_IP_VIDE))
                    er_EnvoyerTrame( 	"MB",
                                   	XZEXC_TRAME_MES_6MN_Q_V_TO,
                                   	pl_listeRADT->ConfRADT.RGS,
                                   	XZEXC_NOCMD,
	                           	XZEXC_PRIORITEFAIBLE,
                                   	pl_listeRADT->Socket);
	          else
		      EnvoyerTrameIP("MB",
		      		XZEXC_TRAME_MES_6MN_Q_V_TO,
				pl_listeRADT->ConfRADT.AdresseIP,
				pl_listeRADT->ConfRADT.Port,
				pl_listeRADT->ConfRADT.RGS,
				XZEXC_NOCMD,
				XZEXC_PRIORITEFAIBLE,
				pl_listeRADT->Socket);
  	     }
  	} 
	}
   }  

   XZST_03EcritureTrace ( XZSTC_DEBUG3, "%s Timers = %d", version, vg_Timers_Enclenches);
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
}

		

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Effectue la lecture des fichiers derniere heure
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_Lecture_derniere_heure ( ERDT_DONNEES_RADT *pa_Sation )

/*
* ARGUMENTS EN ENTREE :
*   aucun
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
*   Effectue la lecture des fichiers derniere heure et mets a jour les variables horodate pour chaque station
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.38 01/11/12 : er_Lecture_derniere_heure" ;

   FILE			*vl_Fd 		= NULL;
   char			pl_Heures[50]	= "";
   XDY_Texte		pl_PathEtFic	= "";
   XDY_Horodate		vl_horodate	= 0.0;
   XDY_Horodate		vl_heure_actuelle	= 0.0;
   XZSMT_Horodate	pl_Horo_Txt 		= "";
   
           /* Arrondi de l'horodate aux heures pleines */
           XZSM_11ConversionHorodate ( vg_Date_Actuelle, &pl_Horo_Txt );
           strcpy ( pl_Horo_Txt+ERDTC_HEURE_STRING, ":00:01" );
           XZSM_13ConversionHorodateSec( pl_Horo_Txt, &vl_heure_actuelle);
   
           /*B Lecture de l'horodate des dernieres mesures recues dans un fichier */
   	   sprintf ( pl_PathEtFic, "%s/%s_%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_HEUR_QVTO, vg_NomMachine,  pa_Sation->ConfRADT.NumeroStation );
 	   if ( (vl_Fd = fopen (pl_PathEtFic, "r+")) == NULL )
           {	
              XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s inexistant.", version, pl_PathEtFic ); 
              pa_Sation->Don_HisRADT.Horodate_Histo = vg_Date_Actuelle;	   
              XZST_03EcritureTrace( XZSTC_INFO, "%s : HORO Histo = %lf pour Station = %d",
                                       version, pa_Sation->Don_HisRADT.Horodate_Histo, pa_Sation->ConfRADT.NumeroStation );
	   }
	   else
	   {
	      fgets ( pl_Heures, sizeof(pl_Heures), vl_Fd );
	      fclose ( vl_Fd );
              XZSM_13ConversionHorodateSec(pl_Heures,&vl_horodate);
	      pa_Sation->Don_HisRADT.Horodate_Histo = (vl_horodate < (vg_Date_Actuelle - (72 * 3600))) ? vg_Date_Actuelle : vl_horodate;
	      XZST_03EcritureTrace( XZSTC_INFO, "%s : HORO Histo = %s => %lf  pour Station = %d", version,
              									pl_Heures,
              									pa_Sation->Don_HisRADT.Horodate_Histo, 
              									pa_Sation->ConfRADT.NumeroStation );
	   }
           
           /*B Initialisation de donnees historique */
   	   pa_Sation->Don_HisRADT.Demande_Histo 	= XDC_FAUX;	   
       	   pa_Sation->Don_HisSilhouette.Demande_Histo 	= XDC_FAUX;
       	   pa_Sation->Don_HisEssieu.Demande_Histo 	= XDC_FAUX;
       	   pa_Sation->Don_HisPoids.Demande_Histo 	= XDC_FAUX;
       	   pa_Sation->Don_HisRADT.Fd		= NULL;
       	   pa_Sation->Don_HisSilhouette.Fd 	= NULL;
       	   pa_Sation->Don_HisEssieu.Fd 		= NULL;
       	   pa_Sation->Don_HisPoids.Fd 		= NULL;

           /*B Lecture de l'horodate des dernieres mesures Pesage recues dans un fichier */
   	   sprintf ( pl_PathEtFic, "%s/%s_%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_HEUR_PESAGE, vg_NomMachine,  pa_Sation->ConfRADT.NumeroStation );
           pa_Sation->Don_HisSilhouette.Horodate_Histo 	= vg_Date_Actuelle;	
           pa_Sation->Don_HisEssieu.Horodate_Histo 	= vg_Date_Actuelle;	
           pa_Sation->Don_HisPoids.Horodate_Histo 	= vg_Date_Actuelle;	
 	   if ( (!access (pl_PathEtFic, F_OK)) &&  ((vl_Fd = fopen (pl_PathEtFic, "r+")) != NULL ) )
 	   {
	      if ( (fgets ( pl_Heures, sizeof(pl_Heures), vl_Fd )) != NULL )
	      {
                 XZSM_13ConversionHorodateSec(pl_Heures,&vl_horodate);
	         pa_Sation->Don_HisSilhouette.Horodate_Histo = (vl_horodate < (vg_Date_Actuelle - (72 * 3600))) ? vl_heure_actuelle : vl_horodate;
	         if ( (fgets ( pl_Heures, sizeof(pl_Heures), vl_Fd )) != NULL )
	         {
                    XZSM_13ConversionHorodateSec(pl_Heures,&vl_horodate);
	            pa_Sation->Don_HisEssieu.Horodate_Histo = (vl_horodate < (vg_Date_Actuelle - (72 * 3600))) ? vl_heure_actuelle : vl_horodate;
	            if ( (fgets ( pl_Heures, sizeof(pl_Heures), vl_Fd )) != NULL )
	            {
                       XZSM_13ConversionHorodateSec(pl_Heures,&vl_horodate);
	               pa_Sation->Don_HisPoids.Horodate_Histo = (vl_horodate < (vg_Date_Actuelle - (72 * 3600))) ? vl_heure_actuelle : vl_horodate;
	            }
	         }
	      }
	      fclose ( vl_Fd );
	      XZST_03EcritureTrace( XZSTC_INFO, "%s : HORO Histo = %s => %.0lf %.0lf %.0lf pour Station = %d", version,
              						pl_Heures,
              						pa_Sation->Don_HisSilhouette.Horodate_Histo,
              						pa_Sation->Don_HisEssieu.Horodate_Histo,
              					        pa_Sation->Don_HisPoids.Horodate_Histo,
              						pa_Sation->ConfRADT.NumeroStation );
            }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_CONFIG_PARAMS_TERDT
*  et m�morisation des param�tres de qualification
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


int er_lire_params_qualif	( void )
			  
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*  Cette fonction effectue la lecture du fichier XDF_CONFIG_PARAMS_TERDT
*  et m�morisation des param�tres de qualification
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cfg.c	1.36 12/07/10 : er_lire_params_qualif" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd = 0;
   int				vl_Index = 0;
   XDY_FichEquext		pl_PathEtFic;
   XDY_Texte			pl_LigneFich;
   ERDT_CONF_REG_PARAMS	vl_Donnees_REG_PARAM;
   int				vl_NbParam  = 0;
   int				vl_Chaussee1, vl_Chaussee2;
   ERDT_DONNEES_RADT		*pl_DonCrt_RADT;
   ERDT_DONNEES_Chaussee	*pl_DonCrt_Chaussee;
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);
      
   /*A  Ouverture du fichier de configuration */
   sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, 
	XDF_Config_Params_TERDT,vg_NomMachine );
      
   if ( ( vl_Fd = open ( pl_PathEtFic, O_RDONLY ) ) == -1 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_params_qualif : fichier %s. introuvable\n", pl_PathEtFic ); 
      return ( XDC_OK );
   }

   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for ( vl_Index = 0; vl_Index < ERDTC_NUM_REG_PARAMS; vl_Index++ )
   {
       /* Suppression de la liste */
       pg_Tab_REG_PARAMS[vl_Index].Numero = 0;
       pg_Tab_REG_PARAMS[vl_Index].Type = 0;
       strcpy (pg_Tab_REG_PARAMS[vl_Index].Nom,"");
       pg_Tab_REG_PARAMS[vl_Index].Valeur = 0;
   }

    vl_Index = 0;
   /*A  Lecture de chaque ligne du fichier */
   while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_params_qualif : ligne '%s'",pl_LigneFich);
      if ( *pl_LigneFich != ERDTC_COMMENTAIRE )
      {
	 /* Extraction de la config */
	 vl_NbParam = sscanf ( pl_LigneFich, " %d\t%d\t%d\t%s",  
			&vl_Donnees_REG_PARAM.Numero,
			&vl_Donnees_REG_PARAM.Type,
			 &vl_Donnees_REG_PARAM.Valeur,
			vl_Donnees_REG_PARAM.Nom );
			
	XZST_03EcritureTrace( XZSTC_WARNING, "er_lire_params_qualif(%d) : %d %d %s %d",	
			vl_NbParam,
			vl_Donnees_REG_PARAM.Numero,vl_Donnees_REG_PARAM.Type,
			vl_Donnees_REG_PARAM.Nom,vl_Donnees_REG_PARAM.Valeur);
	/* Remplissage du tableau */
			
			pg_Tab_REG_PARAMS[vl_Donnees_REG_PARAM.Numero].Numero = 
					vl_Donnees_REG_PARAM.Numero;
			pg_Tab_REG_PARAMS[vl_Donnees_REG_PARAM.Numero].Type = 
					vl_Donnees_REG_PARAM.Type;
			strcpy (pg_Tab_REG_PARAMS[vl_Donnees_REG_PARAM.Numero].Nom,
					vl_Donnees_REG_PARAM.Nom);
			pg_Tab_REG_PARAMS[vl_Donnees_REG_PARAM.Numero].Valeur = 
						vl_Donnees_REG_PARAM.Valeur;
			vl_Index++;
       }
    }
      
    /*A Fermeture du fichier XDF_Config_Params_TERDT */
    XZSS_13FermerFichier( vl_Fd );
      
    /*A  Init de l'etat de service et de l'activite */
    for ( vl_Index = 0; vl_Index < ERDTC_NUM_REG_PARAMS; vl_Index++ )
    {
       if ( pg_Tab_REG_PARAMS[vl_Index].Numero != 0 )
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config REG PARAMS : %d %d %s %d",  
					pg_Tab_REG_PARAMS[vl_Index].Numero,
					pg_Tab_REG_PARAMS[vl_Index].Type,
					pg_Tab_REG_PARAMS[vl_Index].Nom,
					pg_Tab_REG_PARAMS[vl_Index].Valeur );
   }			
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
   return ( vl_ValRet );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Effectue la trace (dump) de la configuration
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_dump_config (  )
{
	FILE	*vl_Fd	= NULL;
	int	i, j;
	ERDT_DONNEES_RADT	*pl_RADT = NULL;
	ERDT_DONNEES_PtMes	*pl_PtMes = NULL;
	ERDT_DONNEES_Chaussee	*pl_Chaussee = NULL;

	vl_Fd = fopen ("/tmp/dump_config_RADT", "w+");
	if (vl_Fd == NULL)
		return;

	/* Dump des index des donnees des stations RADT */
	fprintf (vl_Fd, "pg_Tab_RADT :\n");
	for (i = 0;  i < ERDTC_NUM_MAX_STATION;  i++) {
		fprintf (vl_Fd, "%4d", pg_Tab_RADT[i]);
		if (i % 50 == 0) {
			fprintf (vl_Fd, "\n\n");
		} else if (i % 10 == 0) {
			fprintf (vl_Fd, "\n");
		} else if (i % 5 == 0) {
			fprintf (vl_Fd, "  ");
		}
	}
	fprintf (vl_Fd, "\n\n\n");

	/* Dump du nombre de points de mesures par station */
	fprintf (vl_Fd, "pg_Nb_PtMes :\n");
	for (i = 0;  i < ERDTC_NUM_MAX_STATION;  i++) {
		fprintf (vl_Fd, "%4d", pg_Nb_PtMes[i]);
		if (i % 50 == 0) {
			fprintf (vl_Fd, "\n\n");
		} else if (i % 10 == 0) {
			fprintf (vl_Fd, "\n");
		} else if (i % 5 == 0) {
			fprintf (vl_Fd, "  ");
		}
	}
	fprintf (vl_Fd, "\n\n\n");

	/* Dump du nombre de chaussees par station */
	fprintf (vl_Fd, "pg_Nb_Chaussees :\n");
	for (i = 0;  i < ERDTC_NUM_MAX_STATION;  i++) {
		fprintf (vl_Fd, "%4d", pg_Nb_Chaussees[i]);
		if (i % 50 == 0) {
			fprintf (vl_Fd, "\n\n");
		} else if (i % 10 == 0) {
			fprintf (vl_Fd, "\n");
		} else if (i % 5 == 0) {
			fprintf (vl_Fd, "  ");
		}
	}
	fprintf (vl_Fd, "\n\n\n");

	/* Dump du nombre de voies par station et par chaussee */
	fprintf (vl_Fd, "pg_Nb_Voie :\n");
	for (i = 0;  i < ERDTC_NUM_MAX_STATION;  i++) {
		fprintf (vl_Fd, "(%d", pg_Nb_Voie[i][0]);
		for (j = 1;  j < ERDTC_NB_CHAUSSEE;  j++) {
			fprintf (vl_Fd, ",%d", pg_Nb_Voie[i][j]);
		}
		fprintf (vl_Fd, ")");
		if (i % 50 == 0) {
			fprintf (vl_Fd, "\n\n");
		} else if (i % 10 == 0) {
			fprintf (vl_Fd, "\n");
		} else if (i % 5 == 0) {
			fprintf (vl_Fd, "   ");
		} else {
			fprintf (vl_Fd, " ");
		}
	}
	fprintf (vl_Fd, "\n\n\n");

	/*A Dump de la table des stations virtuelles */
	fprintf (vl_Fd, "pg_Tab_RADT_Virtuelle :\n");
	for (i = 0;  i < ERDTC_NUM_MAX_STATION;  i++) {
		if ( ( pg_Tab_RADT_Virtuelle[i].NumStation1 != 0 )  ||
			( pg_Tab_RADT_Virtuelle[i].NumStationVirtuelle != 0 )  ||
			( pg_Tab_RADT_Virtuelle[i].Chaussee1 != 0 )  ||
			( pg_Tab_RADT_Virtuelle[i].Chaussee2 != 0 )  ||
			( pg_Tab_RADT_Virtuelle[i].Sens != 0 ) )
		{
			fprintf (vl_Fd, "[%3d] : Station/Virtuelle = %d/%d Station2 = %d Station3 = %d Chaussee1 = %d  Chaussee2 = %d Chaussee3 = %d Sens = %d\n",
					i,
					pg_Tab_RADT_Virtuelle[i].NumStation1,
					pg_Tab_RADT_Virtuelle[i].NumStationVirtuelle,
					pg_Tab_RADT_Virtuelle[i].NumStation2,
					pg_Tab_RADT_Virtuelle[i].NumStation3,
					pg_Tab_RADT_Virtuelle[i].Chaussee1,
					pg_Tab_RADT_Virtuelle[i].Chaussee2,
					pg_Tab_RADT_Virtuelle[i].Chaussee3,
					pg_Tab_RADT_Virtuelle[i].Sens);
		}
	}

	/* Dump de la liste des stations RADT */
	fprintf (vl_Fd, "Liste des stations (pg_debutRADT) :\n");
	for (pl_RADT = pg_debutRADT;  pl_RADT != NULL;  pl_RADT = pl_RADT->Suivant) {
		fprintf (vl_Fd, "No/type/virt. = %d/%d/%d  Serveur=%s-%d-%s  Stations amont=%d,%d aval=%d,%d   Site=%d\n",
				(int) pl_RADT->ConfRADT.NumeroStation,
				(int) pl_RADT->ConfRADT.TypeStation,
				(int) pl_RADT->ConfRADT.Virtuelle,
				pl_RADT->ConfRADT.NomServeur,
				(int) pl_RADT->ConfRADT.Liaison,
				pl_RADT->ConfRADT.RGS,
				(int) pl_RADT->ConfRADT.NumStationAmontS1,
				(int) pl_RADT->ConfRADT.NumStationAvalS1,
				(int) pl_RADT->ConfRADT.NumStationAmontS2,
				(int) pl_RADT->ConfRADT.NumStationAvalS2,
				(int) pl_RADT->ConfRADT.SiteGestion);
	}
	fprintf (vl_Fd, "\n\n\n");

	/* Dump de la liste des points de mesure */
	fprintf (vl_Fd, "Liste des points de mesure (pg_debutPtMes) :\n");
	for (pl_PtMes = pg_debutPtMes;  pl_PtMes != NULL;  pl_PtMes = pl_PtMes->Suivant) {
		fprintf (vl_Fd, "Station1 = %d  Sens = %d  Boucle/Inv = %d/%d  Voie = %d  Poids = %d\n",
				(int) pl_PtMes->ConfPtMes.NumeroStation,
				(int) pl_PtMes->ConfPtMes.Sens,
				(int) pl_PtMes->ConfPtMes.Boucle,
				(int) pl_PtMes->ConfPtMes.BoucleInv,
				(int) pl_PtMes->ConfPtMes.Voie,
				(int) pl_PtMes->ConfPtMes.Poids);
	}
	fprintf (vl_Fd, "\n\n\n");

	/* Dump de la liste des chaussees */
	fprintf (vl_Fd, "Liste des chaussees (pg_debutChaussee) :\n");
	for (pl_Chaussee = pg_debutChaussee;  pl_Chaussee != NULL;  pl_Chaussee = pl_Chaussee->Suivant) {
		fprintf (vl_Fd, "Station1 = %d  Sens = %d\n",
				(int) pl_Chaussee->ConfChaussee.NumeroStation,
				(int) pl_Chaussee->ConfChaussee.Sens);
	}
	fprintf (vl_Fd, "\n\n\n");

	fclose (vl_Fd);
}
