/*E*/
/* Fichier : $Id: etdp_pma.c,v 1.19 2020/12/03 16:47:44 pc2dpdy Exp $      Release : $Revision: 1.19 $        Date : $Date: 2020/12/03 16:47:44 $
-----------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-----------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-----------------------------------------------------
* MODULE TETDP * FICHIER etdp_pma.c
-----------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de calcul de temps de parcours et d'annimation des PMVA de la tache TETDP
*
------------------------------------------------------------------------------------
* HISTORIQUE :
*
* MG	04/06/12 : Création (DEM/1014 PMA)
* PNI	16/10/14 : Modification de la politique d'afichage DEM1106 1.2
* NDE	27/07/15 : Pas de passage automatique au neutre en cas de fin de plan d'action (DEM1135) 1.3
* PNI 	05/04/18 : Correctif dans la comparaison ligne PMVA vs destination (DEM1286) 1.5
* LPE   21/08/20 : Ajout XZAO962_Calcul_TDP_FCD dans et_calcul_temps_de_parcours_PMVA DEM-SAE152 1.6
* ABE	25/09/20 : Correction Calcul TdP_FCD DEM-SAE152 1.8
* LPE	06/10/20 : Correction recherche destination dans et_calcul_temps_de_parcours_PMVA DEM-SAE152 1.9
* ABE	06/10/20 : Correction appel XZEZ02 DEM-SAE152 1.10
* PNI	08/10/20 : Pas de rafraichissement sur PMVA sans action 1.11
* GRO   09/10/20 : Récupération nom destination dans et_calcul_temps_de_parcours_PMVA DEM-SAE152 1.15		
* JPL	23/10/20 : Mise en majuscules du texte avant envoi (DEM SAE168)  1.16
* ABE	24/11/20 : Modification appel a XZA0962 suite a la modification de la procedure DEM-SAE152 1.17
* ABE	03/12/20 : Changement des seuils de validation TDP_FCD pour un const declarée dans xdc.h DEM-SAE152 1.19
* ABE	26/07/21 : TDP FCD Hybride sur trajets hors réseau DEM-SAE318 1.20
* ABE   15/11/21 : Ajout du sens du trajet vers la limite du réseau DEM-SAE318 1.21
-----------------------------------------------------------------------------------------*/ 

/* Fichiers inclus */
/* --------------- */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzat.h>
#include	<xzao.h>
#include	<xzep.h>
#include	<xzez.h>
#include	<xzad10.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"

#include	"etdp_don.h"
#include	"etdp_pma.h"

#include "../../SAIDEC/inc/drol_caltdp.h"

/* declaration des donnees globales */
extern XDY_District vg_site;

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* Declaration de variables locales */
/* -------------------------------- */

static char *version = "@(#)etdp_pma.c	1.1 04/06/12      : etdp_pma" ;

/* declaration de fonctions internes */

/* Definition de fonctions externes */
/* -------------------------------- */
extern char *etdp_ligne_majuscules(char *);


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*	Calcul d'un temps de parcours pour une destination et un PMVA donnés
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_calcul_temps_de_parcours_PMVA ( XDY_Eqt	va_NumPMVA,
				  	  	  	  	  XDY_Eqt	va_NumDest,
				  	  	  	  	  XDY_TDP *	va_tdp,
								  char	* va_nom_dest)
/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Calcul d'un temps de parcours pour une destination et un PMVA donnés
-----------------------------------------------------------------------------------------*/
{
  ETDP_CONFIG_PMVA *		pl_liste_pmva;
  XZEZT_Liste_Zdp *		pl_liste_zdp = NULL;
  XZEZT_Liste_Zdp *		pl_liste_zdp2 = NULL;
  XDY_PR        		va_pr_pmva=-1;
  XDY_Autoroute 		va_autoroute_pmva=0;
  XDY_Sens     			va_sens_pmva=0;
  XDY_PR        		vl_PR_d=-1;
  XDY_Autoroute 		vl_autoroute_d=0;
  XDY_Sens    			vl_sens_d=0;
  XDY_TDP       		vl_tdp_fcd=0;
  XDY_Octet    			vl_validite_fcd=0;
  XDY_Booleen			vl_validite_faible=1;
  XZEZT_Liste_Destinations *	pl_Dest;
  XDY_PR				va_pr_limite=0;
  XDY_Autoroute			va_autoroute_limite=0;
  XDY_Booleen			vl_destination_hors_reseau;
  XDY_Booleen			vl_FCD_calcul_hors_reseau=XDC_FAUX;
  XDY_Sens     			va_sens_limite=0;
  XDY_Octet						vl_inhb;


  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : et_calcul_temps_de_parcours pour PMVA : %d Dest : %d", 
	va_NumPMVA, va_NumDest );

  /* Test pour savoir si les tdp sur cette eqt sont inhibe */
  xzae59_Rechercher_EQT_TDP_INHIB(va_NumPMVA,XDC_EQT_PMVA,&vl_inhb);
  if (vl_inhb)
  {
	XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours_PMVA TDP PMVA %d inhibe, on retourne -1",va_NumPMVA);
	*va_tdp=-1;
	return (XDC_OK);
  }

  /*On recupere le PR, l'autoroute et le sens de la destination*/
 
    for ( pl_Dest = pg_ListeDest; pl_Dest != NULL; pl_Dest = pl_Dest->suivant ) {
    XZST_03EcritureTrace(XZSTC_INFO,"destination %d",pl_Dest->destination.Numero);
    if ( pl_Dest->destination.Numero == va_NumDest )
    {
       vl_PR_d           = pl_Dest->destination.PR;
       vl_sens_d 	 = pl_Dest->destination.Sens;
       vl_autoroute_d	 = pl_Dest->destination.Autoroute;
       strncpy(va_nom_dest,pl_Dest->destination.Libelle_PMV,14);
       va_nom_dest[14]='\0';	
       break;
    }
  }
  

  /* Recherche du PMVA dans la liste et récupération du numero d'autoroute, du PR et du sens du PMVA*/
  for ( pl_liste_pmva = pg_ListePMVA; pl_liste_pmva != NULL; pl_liste_pmva = pl_liste_pmva->suivant )
    if ( pl_liste_pmva->PMVA.Numero == va_NumPMVA )
    {
		va_autoroute_pmva=pl_liste_pmva->PMVA.Autoroute;
		va_pr_pmva=pl_liste_pmva->PMVA.PR;
		va_sens_pmva=pl_liste_pmva->PMVA.Sens;
		break;
    }

  /* Si le PMV existe alors calcul du temps de parcours */
  if ( pl_liste_pmva != NULL )
  {  

	vl_destination_hors_reseau=XZAO29_calcul_zone_hors_reseau (vl_autoroute_d,
                            		vl_PR_d,
                            		va_sens_pmva,
                            		vl_sens_d,
									&va_pr_limite,
									&va_autoroute_limite,
									&va_sens_limite); //Test si destination hors reseau ESCOTA 1=(ORANGE/MONTPELLIER/MARSEILLE) ou (MONACO/VINTIMILLE)

	vl_FCD_calcul_hors_reseau=vl_destination_hors_reseau; //Pour ne pas envoyer le TDP FCD si vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA et que FCD mauvais et donc terrain calcul complet mais r�sultat HS, alors que FCD est un calcul non complet

	/*Recuperation du TDP grace a l'appel a la procedure XZAO962*/
  	if (XZAO962_Calcul_TDP_FCD(  	va_autoroute_pmva,
                            		va_pr_pmva,
                            		va_sens_pmva,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_autoroute_limite : vl_autoroute_d,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_pr_limite : vl_PR_d, //Si on est hors reseau ORANGE/MONTPELLIER/MARESEILLE On calcul jusqu'à la limite du réseau, sinon les TDP_FCD sont fournis,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_sens_limite : vl_sens_d,
									0,
									0,
                            		&vl_tdp_fcd,
                            		&vl_validite_fcd,
                            		&vl_validite_faible)!= XDC_OK)	 
	{
        	XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours_PMVA : pb appel a XZAO962_Calcul_TDP_FCD");
        	return (XDC_NOK);
    }
 	 /*Si vl_validite_fcd >= 60% et que toutes les données ont une validite > 20% (vl_validite_faible = 0) alors on utilise le temps de parcous de la procedure XZAO962*/
  	if((vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD) && (vl_validite_faible == 0) && vl_destination_hors_reseau == XDC_FAUX)
		
	{
		*va_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi à la minute superieure*/
		XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours pour PMVA : %d validite TDP_FCD>%d Dest : %d Tdp : %d",va_NumPMVA, XDC_SEUIL_AFFICHAGE_TDP_FCD, va_NumDest, *va_tdp );
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMVA");
		return (XDC_OK);
	}
  
  /*Si vl_validite_fcd < 60% ou qu'on est sur un calcul hors réseau on utilise la xzez02 pour la partie hors réseau*/
	else	
	{ 		  
		if (vl_destination_hors_reseau == XDC_VRAI &&  (vl_validite_fcd<=XDC_SEUIL_AFFICHAGE_TDP_FCD || vl_validite_faible != 0)) // Si on a le TDP FCD non valide et qu'on est hors reseau, alors on calcul tout le trajet avec les donnees terrains
		{
			vl_destination_hors_reseau = XDC_FAUX;
		}
		else if (vl_destination_hors_reseau == XDC_VRAI) // Si la destination est hors reseau on le donne en nom de destination a la xzez02 pour qu'il exclut les zones non virtuelles
			strcpy(pl_liste_pmva->Nom,"HORS RESEAU");

  		XZST_03EcritureTrace(XZSTC_FONCTION,"Destination1_proche %d",pl_liste_pmva->Destination1_proche);
  		XZST_03EcritureTrace(XZSTC_FONCTION,"Destination1_loin %d",pl_liste_pmva->Destination1_loin);
  		XZST_03EcritureTrace(XZSTC_FONCTION,"Destination2_proche %d",pl_liste_pmva->Destination2_proche);
  		XZST_03EcritureTrace(XZSTC_FONCTION,"Destination2_loin %d",pl_liste_pmva->Destination2_loin);
     		/****************************************************************/
     		if (va_NumDest==pl_liste_pmva->Destination1_proche)
    	 		pl_liste_zdp = pl_liste_pmva->Graphe_Destination1;
     		else if (va_NumDest==pl_liste_pmva->Destination1_loin)
    	 		pl_liste_zdp = pl_liste_pmva->Graphe_Destination1Lointaine;
     		else if (va_NumDest==pl_liste_pmva->Destination2_proche)
    	 		pl_liste_zdp = pl_liste_pmva->Graphe_Destination2;
     		else if (va_NumDest==pl_liste_pmva->Destination2_loin)
    	 		pl_liste_zdp = pl_liste_pmva->Graphe_Destination2Lointaine;
     		/***************************************************************/

		for (pl_liste_zdp2=pl_liste_zdp;pl_liste_zdp2!=NULL;pl_liste_zdp2=pl_liste_zdp2->suivant) {
			XZST_03EcritureTrace(XZSTC_FONCTION,"%d %d",
					pl_liste_pmva->PMVA.Numero,
					pl_liste_zdp2->zdp.Code_Zone);
     		}

     		XZEZ02_Calcul_TDP (pl_liste_pmva->PMVA.Autoroute,
			 		vl_destination_hors_reseau == XDC_VRAI ? va_pr_limite : pl_liste_pmva->PMVA.PR,
			 		pl_liste_pmva->PMVA.Sens,
			 		va_NumDest,
			 		pl_liste_zdp,
			 		pg_ListeDest,
			 		pg_ListeZone,
			 		va_tdp,
			 		pl_liste_pmva->Nom);
		
     		strcpy(va_nom_dest,pl_liste_pmva->Nom);
     		/*XZEZ05_Calcul_TDP_PMVA(pl_liste_pmva->PMVA.Autoroute,
					pl_liste_pmva->PMVA.PR,
					pl_liste_pmva->PMVA.Sens,
					va_Numdest1_p,
					va_Numdest1_l,
					va_Numdest2_p,
					va_Numdest2_l,
					pl_liste_zdp,
					pg_ListeDest,
					va_tdp);*/



		
		/*Tdp en fonction du tdp issu de XZEZ02_Calcul_TDP*/

		// TDP HYBRIDE
		if(*va_tdp>=0 && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD && (vl_validite_faible == 0) && (vl_destination_hors_reseau == XDC_VRAI) )
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours TDP hybride. FCD = %d, XZEZ02 = %d",(vl_tdp_fcd + 59) / 60,*va_tdp);
			*va_tdp = *va_tdp + (vl_tdp_fcd + 59) / 60;
			XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours TDP hybride. TDP =%d",*va_tdp);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMVA ");
			return (XDC_OK);
		}

		XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD23_Calculer_TdP_PMV_Dest : XZEZ02 TDP = %d",*va_tdp);
		if (*va_tdp<=0 && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA && vl_FCD_calcul_hors_reseau==XDC_FAUX) /*Si le tdp vaut -1 et si la validite issue de XZAO962 >=40% alors on recupere le tdp issu de XZAO962*/
		{
			*va_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi à la minute superieure*/
			XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours pour PMVA : %d, TDP equi <0 et validite TDP_FCD>=%d Dest : %d Tdp : %d",XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA, va_NumPMVA, va_NumDest, *va_tdp );
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMVA");	
				return (XDC_OK);
		}
		else /*Sinon on utilise le tdp issu de XZEZ02_Calcul_TDP*/
		{
			XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours pour PMVA : %d Dest : %d Tdp : %d", va_NumPMVA, va_NumDest, *va_tdp );
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMVA");
			return (XDC_OK);
		}
	}
  }
  else
  {
     XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMVA : %d non trouve", va_NumPMVA );
     return (XDC_NOK);
  }
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*	Rafraichissement des PMVA
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_Rafraichissment_PMVA ()
/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*	Rafrachissement des PMVA
-----------------------------------------------------------------------------------------*/
{
  XDY_TDP			vl_TDP = 0,vl_TDP1proche = 0,vl_TDP1loin = 0,vl_TDP2proche = 0,vl_TDP2loin = 0;
  ETDP_CONFIG_PMVA *		pl_PMVA;
  XDY_Etat_PMVA			vl_EtatPMVA;
  char	vl_texte[255];
  int	vl_js;
  XDY_Horodate	vl_horodate;
  char vl_nom_dest[12]="";
  char vl_nom_dest1proche[20]="";
  char vl_nom_dest1loin[20]="";
  char vl_nom_dest2proche[20]="";
  char vl_nom_dest2loin[20]="";
  char vl_nom[30]="";
  int vl_cr1proche=0,vl_cr1loin=0,vl_cr2proche=0, vl_cr2loin=0;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : et_Rafraichissment_PMVA " );

  /* Recherche du PMVA dans la liste */

for ( pl_PMVA = pg_ListePMVA; pl_PMVA != NULL; pl_PMVA = pl_PMVA->suivant ) {
	if ((pl_PMVA->PMVA.SiteGestion != vg_site) || (pl_PMVA->NumAction <= 0)){
	XZST_03EcritureTrace(XZSTC_INFO,"PMVA=%d site=%d ou Pas daction=%d",pl_PMVA->PMVA.Numero,pl_PMVA->PMVA.SiteGestion,pl_PMVA->NumAction);
      continue;
	}

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : PMVA %d(%d/%d/%d/%d), Action=%d",pl_PMVA->PMVA.Numero,
				pl_PMVA->EtatPMVA.Dest1_proche, pl_PMVA->EtatPMVA.Dest1_loin, pl_PMVA->EtatPMVA.Dest2_proche, pl_PMVA->EtatPMVA.Dest2_loin,pl_PMVA->NumAction);
	if ( (pl_PMVA->EtatPMVA.Dest1_proche || pl_PMVA->EtatPMVA.Dest1_loin || pl_PMVA->EtatPMVA.Dest2_proche
			|| pl_PMVA->EtatPMVA.Dest2_loin ) ) {

	memcpy ( &vl_EtatPMVA, &pl_PMVA->EtatPMVA, sizeof(XDY_Etat_PMVA) );

	if ( pl_PMVA->EtatPMVA.Dest1_proche)
		vl_cr1proche=et_calcul_temps_de_parcours_PMVA(pl_PMVA->EtatPMVA.Numero, pl_PMVA->EtatPMVA.Dest1_proche, &vl_TDP1proche,vl_nom_dest1proche);

	if ( pl_PMVA->EtatPMVA.Dest1_loin)
		vl_cr1loin=et_calcul_temps_de_parcours_PMVA(pl_PMVA->EtatPMVA.Numero, pl_PMVA->EtatPMVA.Dest1_loin, &vl_TDP1loin,vl_nom_dest1loin);

	if ( pl_PMVA->EtatPMVA.Dest2_proche)
		vl_cr2proche=et_calcul_temps_de_parcours_PMVA(pl_PMVA->EtatPMVA.Numero, pl_PMVA->EtatPMVA.Dest2_proche, &vl_TDP2proche,vl_nom_dest2proche);

	if ( pl_PMVA->EtatPMVA.Dest2_loin)
		vl_cr2loin=et_calcul_temps_de_parcours_PMVA(pl_PMVA->EtatPMVA.Numero, pl_PMVA->EtatPMVA.Dest2_loin, &vl_TDP2loin,vl_nom_dest2loin);


if ( pl_PMVA->EtatPMVA.Dest1_proche==XDC_DESTINATION_TRAVERSEE) {
          et_calcul_traversee(pl_PMVA->EtatPMVA.Numero, &vl_TDP);
          if (!strncmp(vl_EtatPMVA.Texte3, "TRAVERSEE", 9)) {
            if (vl_TDP > 600)
              strcpy(vl_EtatPMVA.Texte3,"");
            else if (vl_TDP > 59) {
              sprintf( &vl_EtatPMVA.Texte3[11], "%1dH%02d", vl_TDP/60, vl_TDP%60 );
              vl_EtatPMVA.Texte3[15] = '\0';
            }
            else if (vl_TDP > 0) {
              sprintf(&vl_EtatPMVA.Texte3[11],"%2dMN",vl_TDP);
              vl_EtatPMVA.Texte3[15] = '\0';
            }
          }
          else if (!strncmp(vl_EtatPMVA.Texte4, "TRAVERSEE", 9)) {
            if (vl_TDP > 600)
              strcpy(vl_EtatPMVA.Texte4,"");
            else if (vl_TDP > 59) {
              sprintf( &vl_EtatPMVA.Texte4[11], "%1dH%02d", vl_TDP/60, vl_TDP%60 );
              vl_EtatPMVA.Texte4[15] = '\0';
            }
            else if (vl_TDP > 0) {
              sprintf(&vl_EtatPMVA.Texte4[11],"%2dMN",vl_TDP);
              vl_EtatPMVA.Texte4[15] = '\0';
            }
	  }
          else if (!strncmp(vl_EtatPMVA.Texte5, "TRAVERSEE", 9)) {
            if (vl_TDP > 600)
              strcpy(vl_EtatPMVA.Texte5,"");
            else if (vl_TDP > 59) {
              sprintf( &vl_EtatPMVA.Texte5[11], "%1dH%02d", vl_TDP/60, vl_TDP%60 );
              vl_EtatPMVA.Texte5[15] = '\0';
            }
            else if (vl_TDP > 0) {
              sprintf(&vl_EtatPMVA.Texte5[11],"%2dMN",vl_TDP);
              vl_EtatPMVA.Texte5[15] = '\0';
            }
	  }
}


	if (pl_PMVA->EtatPMVA.Dest1_proche) {
XZST_03EcritureTrace(XZSTC_WARNING,"IN : et_Rafraichissment_PMVA Dest1_proche %s %s %d-4:%d %d", vl_EtatPMVA.Texte1,vl_nom_dest1proche ,strncmp(vl_EtatPMVA.Texte1,vl_nom_dest1proche,10),strncmp(vl_EtatPMVA.Texte4,vl_nom_dest1proche,strlen(vl_nom_dest1proche)),vl_TDP1proche);
	}

	if (pl_PMVA->EtatPMVA.Dest1_proche && (strncmp(vl_EtatPMVA.Texte1,vl_nom_dest1proche,strlen(vl_nom_dest1proche))==NULL)){
			vl_TDP=vl_TDP1proche;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte1,"               ");
				sprintf(&vl_EtatPMVA.Texte1[11],"xxMN");
				vl_EtatPMVA.Texte1[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte1[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte1[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte1[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte1[15] = '\0';
XZST_03EcritureTrace(XZSTC_WARNING,"IN : et_Rafraichissment_PMVA  %s %d %s", vl_nom_dest1proche,vl_TDP, vl_EtatPMVA.Texte1 );
			}
			else {
				strcpy(vl_EtatPMVA.Texte1,"               ");
				sprintf(&vl_EtatPMVA.Texte1[11],"xxMN");
				vl_EtatPMVA.Texte1[15] = '\0';
			}
	}
	if (pl_PMVA->EtatPMVA.Dest1_loin && (strncmp(vl_EtatPMVA.Texte2,vl_nom_dest1loin,strlen(vl_nom_dest1loin))==NULL)){
			vl_TDP=vl_TDP1loin;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte2,"               ");
				sprintf(&vl_EtatPMVA.Texte2[11],"xxMN");
				vl_EtatPMVA.Texte2[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte2[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte2[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte2[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte2[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMVA.Texte2,"               ");
				sprintf(&vl_EtatPMVA.Texte2[11],"xxMN");
				vl_EtatPMVA.Texte2[15] = '\0';
			}
	}

	if (pl_PMVA->EtatPMVA.Dest1_proche && (strncmp(vl_EtatPMVA.Texte3,vl_nom_dest1proche,strlen(vl_nom_dest1proche))==NULL)) { 
			vl_TDP=vl_TDP1proche;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte3,"               ");
				sprintf(&vl_EtatPMVA.Texte3[11],"xxMN");
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
	}
	else if (pl_PMVA->EtatPMVA.Dest1_loin && (strncmp(vl_EtatPMVA.Texte3,vl_nom_dest1loin,strlen(vl_nom_dest1loin))==NULL)) {
			vl_TDP=vl_TDP1loin;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte3,"               ");
				sprintf(&vl_EtatPMVA.Texte3[11],"xxMN");
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
	}
	else if (pl_PMVA->EtatPMVA.Dest2_proche && (strncmp(vl_EtatPMVA.Texte3,vl_nom_dest2proche,strlen(vl_nom_dest2proche))==NULL)) {
			vl_TDP=vl_TDP2proche;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte3,"               ");
				sprintf(&vl_EtatPMVA.Texte3[11],"xxMN");
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
	}
	else if (pl_PMVA->EtatPMVA.Dest2_loin && (strncmp(vl_EtatPMVA.Texte3,vl_nom_dest2loin,strlen(vl_nom_dest2loin))==NULL)) {
			vl_TDP=vl_TDP2loin;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte3,"               ");
				sprintf(&vl_EtatPMVA.Texte3[11],"xxMN");
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte3[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte3[15] = '\0';
			}
	}

	if (pl_PMVA->EtatPMVA.Dest1_proche && (strncmp(vl_EtatPMVA.Texte4,vl_nom_dest1proche,strlen(vl_nom_dest1proche))==NULL)) {
			vl_TDP=vl_TDP1proche;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
/*XZST_03EcritureTrace(XZSTC_WARNING,"IN : et_Rafraichissment_PMVA  %s %d %s", vl_nom_dest1proche,vl_TDP, vl_EtatPMVA.Texte4 );*/
	}
	else if (pl_PMVA->EtatPMVA.Dest1_loin && (strncmp(vl_EtatPMVA.Texte4,vl_nom_dest1loin,strlen(vl_nom_dest1loin))==NULL)) {
			vl_TDP=vl_TDP1loin;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
	}
	else if (pl_PMVA->EtatPMVA.Dest2_proche && (strncmp(vl_EtatPMVA.Texte4,vl_nom_dest2proche,strlen(vl_nom_dest2proche))==NULL)) {
			vl_TDP=vl_TDP2proche;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
	}
	else if (pl_PMVA->EtatPMVA.Dest2_loin && (strncmp(vl_EtatPMVA.Texte4,vl_nom_dest2loin,strlen(vl_nom_dest2loin))==NULL)) {
			vl_TDP=vl_TDP2loin;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte4[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte4[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMVA.Texte4,"               ");
				sprintf(&vl_EtatPMVA.Texte4[11],"xxMN");
				vl_EtatPMVA.Texte4[15] = '\0';
			}
	}
	
	if (pl_PMVA->EtatPMVA.Dest2_loin && (strncmp(vl_EtatPMVA.Texte5,vl_nom_dest2loin,strlen(vl_nom_dest2loin))==NULL)){
			vl_TDP=vl_TDP2loin;
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMVA.Texte5,"               ");
				sprintf(&vl_EtatPMVA.Texte5[11],"xxMN");
				vl_EtatPMVA.Texte5[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMVA.Texte5[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMVA.Texte5[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMVA.Texte5[11],"%2dMN",vl_TDP);
				vl_EtatPMVA.Texte5[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMVA.Texte5,"               ");
				sprintf(&vl_EtatPMVA.Texte5[11],"xxMN");
				vl_EtatPMVA.Texte5[15] = '\0';
			}
	}



 XZST_03EcritureTrace(XZSTC_WARNING,"et_Rafraichissment_PMVA precedent  texte1:%s texte2:%s texte3:%s texte4=%s texte5=%s",
		 	 	 	 	 	 	pl_PMVA->EtatPMVA.Texte1,
								pl_PMVA->EtatPMVA.Texte2,
								pl_PMVA->EtatPMVA.Texte3,
								pl_PMVA->EtatPMVA.Texte4,
								pl_PMVA->EtatPMVA.Texte5 );

    /*A on supprime les espaces de debut de ligne */
    if (!strncmp(vl_EtatPMVA.Texte1,"          ",10))
      vl_EtatPMVA.Texte1[0]= '\0';
    if (!strncmp(vl_EtatPMVA.Texte2,"          ",10))
      vl_EtatPMVA.Texte2[0]= '\0';
    if (!strncmp(vl_EtatPMVA.Texte3,"          ",10))
      vl_EtatPMVA.Texte3[0]= '\0';
    if (!strncmp(vl_EtatPMVA.Texte4,"          ",10))
      vl_EtatPMVA.Texte4[0]= '\0';
    if (!strncmp(vl_EtatPMVA.Texte5,"          ",10))
      vl_EtatPMVA.Texte5[0]= '\0';
/*    if (vl_EtatPMVA.Texte2[0]==' ')
      vl_EtatPMVA.Texte2[0]= '\0';
    if (vl_EtatPMVA.Texte4[0]==' ')
      vl_EtatPMVA.Texte4[0]= '\0';
    if (vl_EtatPMVA.Texte5[0]==' ')
      vl_EtatPMVA.Texte5[0]= '\0';*/

    /*A on met le texte du message en majuscules */
//    etdp_ligne_majuscules(vl_EtatPMVA.Texte1);
//    etdp_ligne_majuscules(vl_EtatPMVA.Texte2);
//    etdp_ligne_majuscules(vl_EtatPMVA.Texte3);
//    etdp_ligne_majuscules(vl_EtatPMVA.Texte4);
//    etdp_ligne_majuscules(vl_EtatPMVA.Texte5);


 XZST_03EcritureTrace(XZSTC_WARNING,"et_Rafraichissment_PMVA texte1:%s texte2:%s texte3:%s texte4:%s texte5:%s",
		 	 	 	 	 	 	 vl_EtatPMVA.Texte1,
		 	 	 	 	 	 	 vl_EtatPMVA.Texte2,
		 	 	 	 	 	 	 vl_EtatPMVA.Texte3,
		 	 	 	 	 	 	 vl_EtatPMVA.Texte4,
		 	 	 	 	 	 	 vl_EtatPMVA.Texte5 );

          if ( strcmp(vl_EtatPMVA.Texte2,pl_PMVA->EtatPMVA.Texte2) ||
               strcmp(vl_EtatPMVA.Texte1,pl_PMVA->EtatPMVA.Texte1) || strcmp(vl_EtatPMVA.Texte3,pl_PMVA->EtatPMVA.Texte3) ||
			   strcmp(vl_EtatPMVA.Texte3,pl_PMVA->EtatPMVA.Texte4) ||
			   strcmp(vl_EtatPMVA.Texte3,pl_PMVA->EtatPMVA.Texte5) )
          {
        	  /* PNI : 1.10  memcpy ( &pl_PMV->EtatPMV, &vl_EtatPMV, sizeof(XDY_Etat_PMV) ); on conserve la commande initiale d'affichage tdp */
        	  /* S'il y a une destination valide , on l'affiche, sinon on met l'heure */
        	  if (( vl_EtatPMVA.Texte2[0] != '\0' ) || (vl_EtatPMVA.Texte3[0] != '\0') || (vl_EtatPMVA.Texte1[0] != '\0') || (vl_EtatPMVA.Texte4[0] != '\0') || (vl_EtatPMVA.Texte5[0] != '\0'))
        	  {
        		  /* PNI : 1.10 memcpy ( &pl_PMV->EtatPMV, &vl_EtatPMV, sizeof(XDY_Etat_PMV) ); on conserve la commande initiale d'affichage tdp */
        		  XZEP05_Commande_Raf_TDP_PMVA ( vl_EtatPMVA , pl_PMVA->NumAction, pl_PMVA->PMVA.NomMachine );
        		  if  (vl_EtatPMVA.Texte1[0] != '\0')
        			  strcpy(pl_PMVA->EtatPMVA.Texte1,vl_EtatPMVA.Texte1);
        		  if  (vl_EtatPMVA.Texte2[0] != '\0')
        			  strcpy(pl_PMVA->EtatPMVA.Texte2,vl_EtatPMVA.Texte2);
        		  if  (vl_EtatPMVA.Texte3[0] != '\0')
        			  strcpy(pl_PMVA->EtatPMVA.Texte3,vl_EtatPMVA.Texte3);
        		  if  (vl_EtatPMVA.Texte4[0] != '\0')
        			  strcpy(pl_PMVA->EtatPMVA.Texte4,vl_EtatPMVA.Texte4);
        		  if  (vl_EtatPMVA.Texte5[0] != '\0')
        			  strcpy(pl_PMVA->EtatPMVA.Texte5,vl_EtatPMVA.Texte5);
        	  }
        	  else
        	  {
        		  vl_EtatPMVA.Affichage1 = vl_EtatPMVA.Affichage2 = vl_EtatPMVA.Affichage3 = vl_EtatPMVA.Affichage4 = vl_EtatPMVA.Affichage5 = XDC_PMV_HEURE;
        		  vl_EtatPMVA.Flash = XDC_PMVA_PASFLASH;
        		  vl_EtatPMVA.Clign1 = vl_EtatPMVA.Clign2 = vl_EtatPMVA.Clign3 = vl_EtatPMVA.Clign4 = vl_EtatPMVA.Clign5 = XDC_PMVA_PASCLIGNO;
        		  vl_EtatPMVA.DepassSeuil = XDC_FAUX;
        		  strcpy ( vl_EtatPMVA.Texte1,    "" );
        		  strcpy ( vl_EtatPMVA.Alternat1, "" );
        		  strcpy ( vl_EtatPMVA.Texte2,    "" );
        		  strcpy ( vl_EtatPMVA.Alternat2, "" );
        		  strcpy ( vl_EtatPMVA.Texte3,    "" );
        		  strcpy ( vl_EtatPMVA.Alternat3, "" );
        		  strcpy ( vl_EtatPMVA.Texte4,    "" );
        		  strcpy ( vl_EtatPMVA.Alternat4, "" );
        		  strcpy ( vl_EtatPMVA.Texte5,    "" );
        		  strcpy ( vl_EtatPMVA.Alternat5, "" );

				/* PNI : 1.10 memcpy ( &pl_PMV->EtatPMV, &vl_EtatPMV, sizeof(XDY_Etat_PMV) ); on conserve la commande initiale d'affichage tdp */
				/* S'il y a une action alors terminer l'action */
				if ( pl_PMVA->NumAction > 0 )
				{
					/* Lecture vl_horodate */
					XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );
					xzat250_CompteRendu_PMVA ( -pl_PMVA->NumAction, vl_horodate, XDC_NOK,pl_PMVA->PMVA.SiteGestion );
					XZEP04_Commande_PMVA ( vl_EtatPMVA , pl_PMVA->NumAction, pl_PMVA->PMVA.NomMachine );


				}

				XZST_03EcritureTrace(XZSTC_WARNING,"Plus de destination affichage texte1:%s texte2:%s texte4:%s texte5:%s",
									pl_PMVA->EtatPMVA.Texte1,
									pl_PMVA->EtatPMVA.Texte2,
									pl_PMVA->EtatPMVA.Texte4,
									pl_PMVA->EtatPMVA.Texte5);
			 }
          }
          }
    }

	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_Rafraichissment_PMVA " );

    return (XDC_OK);
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Recherche si zone concerner par affichage PMVA
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_Recherche_zone_PMVA ( 	XDY_Autoroute		va_autoroute_pmva,
								XDY_PR				va_pr_pmva,
								XDY_Sens			va_sens_pmva,
								XDY_Eqt				va_dest,
								XDY_Eqt				va_NumZone )
/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Recherche si zone concerner par affichage PMVA
*
-----------------------------------------------------------------------------------------*/
{
static char *version = "@(#)etdp_pma.c	1.14 06/23/11      : et_Recherche_zone_PMVA" ;

XDY_PR				vl_pr_destination=0;
XDY_Booleen			vl_Type_Loc=0;
XZEZT_Liste_Zdp *		pl_Zone;
XZEZT_Liste_Destinations *	pl_Dest;


  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s auto:%d pr:%ld sens:%d dest:%d  zone:%d",
		  	  	  	  	  	  	  version,
		  	  	  	  	  	  	  va_autoroute_pmva,
		  	  	  	  	  	  	  va_pr_pmva,
		  	  	  	  	  	  	  va_sens_pmva,
		  	  	  	  	  	  	  va_dest,
		  	  	  	  	  	  	  va_NumZone );

  /* Recherche de la destination dans la liste */
  for ( pl_Dest = pg_ListeDest; pl_Dest != NULL; pl_Dest = pl_Dest->suivant )
    if ( pl_Dest->destination.Numero == va_dest )
    {
       vl_pr_destination = pl_Dest->destination.PR;
       vl_Type_Loc 	 = pl_Dest->destination.Type_Localisation;
       break;
    }

  /*recherche de la zdp associee au pmv*/
  for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
  {
    /*si ma zdp est sur la bonne autoroute et dans le bon sens*/
    if ( pl_Zone->zdp.Numero == va_NumZone )
    {
       if ( (pl_Zone->zdp.Autoroute==va_autoroute_pmva) &&
            (pl_Zone->zdp.Sens==va_sens_pmva) )
       {
         /* Traitement du sens NORD */
         if ( (va_sens_pmva == XDC_SENS_NORD) || (va_sens_pmva == XDC_SENS_ENTREE_NORD) )
         {
             if ( (pl_Zone->zdp.Type_Zone == vl_Type_Loc) &&
                  (pl_Zone->zdp.PR_Debut <= vl_pr_destination) &&
                  (vl_pr_destination < pl_Zone->zdp.PR_Fin) )
             {
                /* Zone entre la destination et le debut de la zdp de la destination */
                XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone DEST " );
                return (XDC_OK);
             }
             if ( (pl_Zone->zdp.Type_Zone == vl_Type_Loc) &&
                  (pl_Zone->zdp.PR_Debut < va_pr_pmva) &&
                  (va_pr_pmva <= pl_Zone->zdp.PR_Fin) )
             {
                /* Zone entre PMVA et la fin de la zdp du PMVA */
                XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone PMVA " );
                return (XDC_OK);
             }
             /* Zone entierement comprises entre le PMVA et la destination*/
             if ( (pl_Zone->zdp.Type_Zone == XDC_ZDP_TDP_SECTION) &&
                  (va_pr_pmva > pl_Zone->zdp.PR_Fin) &&
                  (vl_pr_destination < pl_Zone->zdp.PR_Debut) )
             {
                XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone INTER" );
                return (XDC_OK);
             }
         }
         /* Traitement du sens SUD */
         else
         {
             /* recherche de la zdp associee a la destination*/
             if ( (pl_Zone->zdp.Type_Zone == vl_Type_Loc) &&
                  (pl_Zone->zdp.PR_Debut < vl_pr_destination) &&
                  (vl_pr_destination <= pl_Zone->zdp.PR_Fin) )
             {
                /* Zone entre la destination et le debut de la zdp de la destination */
                XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone DEST " );
                return (XDC_OK);
             }
             if ( (pl_Zone->zdp.Type_Zone == vl_Type_Loc) &&
                  (pl_Zone->zdp.PR_Debut <= va_pr_pmva) &&
                  (va_pr_pmva < pl_Zone->zdp.PR_Fin) )
             {
                 /* Zone entre PMVA et la fin de la zdp du PMVA */
                 XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone PMVA " );
                 return (XDC_OK);
             }
             /* Zone entierement comprises entre le PMVA et la destination*/
             if ( (pl_Zone->zdp.Type_Zone == XDC_ZDP_TDP_SECTION) &&
                  (va_pr_pmva < pl_Zone->zdp.PR_Debut) &&
                  (vl_pr_destination > pl_Zone->zdp.PR_Fin) )
             {
                XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone INTER" );
                return (XDC_OK);
             }
          }
      }


    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: %s ", version );
  return (XDC_NOK);

}
