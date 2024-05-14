/*E*/
/*Fichier :  @(#)etdp_pmv.c	1.14      Release : 1.14        Date : 06/23/11
-------------------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_pmv.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de calcul de temps de parcours et d'annimation des PMV  de la tache TETDP
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.3	05 Jun 1997	: Ajout alerte sur panne zone (DEM/1447)
* Mismer.D	version 1.4	06 Jun 1997	: Ajout alerte sur panne zone suite (DEM/1447)
* Mismer.D	version 1.5	13 Oct 1997	: Modif pour affichage minutes pour TDP (DEM/1505)
* Mismer.D      version 1.6     16 Oct 1997     : Modif pour affichage minutes pour TDP suite (DEM/1505)
* Niepceron.P	version 1.9	29 avr 2005	: DEM464 Modif et_Rafraichissment_PMV :2 destinations PMV : La dest altern devient la second dest (3eme ligne)
						: Prise en cpte d'une TDP invalide (<= 0)
* Niepceron.P	version 1.10	25 mai 2006	: Modif pour reprise rafraichissement apres perte de donn�es tdp
* Niepceron.P	version 1.11	25 Aout 2006	: Remplt par xx du tdp non valide dans la memorisation des destination DEM581
* JMG   13/11/07        : ajout site de gestion DEM715  1.12
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK ou NOK pour plusieurs fonctions  1.13
* LCL	22/006/11 : TDP A8 1.14
* LCL 04/11/11 : Correction envoi TdP aux PMV sur 15 car au lieu de 14        1.15
* VR		21/11/11 : Ajout Destination Noeud dans fonction et_Rafraichissement_PMV v1.16 (DEM/1014)
* JMG	04/07/12 : corrections DEM/1014
* PNI	20/01/15 : Modification de la pol d'affichage PMV DEM1106 v1.18
* PNI	06/02/15 : Correctif sur suppression des espaces de debut de ligne V1.19
* NDE	27/07/15 : Pas de passage automatique au neutre en cas de fin de plan d'action (DEM1135) 1.20
* JMG	17/01/17 : gestion temps de traversse lot 23 1.21
* PNI 	05/04/18 : Correctif dans la comparaison ligne PMV vs destination (DEM1286) 1.22
* JMG	08/04/19 : forcage non clignotement dans et_Rafraichissment_PMV 1.23
* LPE   21/08/20 : Ajout XZAO962_Calcul_TDP_FCD dans et_calcul_temps_de_parcours DEM-SAE152 1.24
* ABE	02/10/20 : Correction Calcul TdP_FCD DEM-SAE152 1.26
* LPE	06/10/20 : Correction recherche destination dans et_calcul_temps_de_parcours DEM-SAE152 1.27
* ABE	06/10/20 : Correction appel XZEZ02 DEM-SAE152 1.28
* PNI	08/10/20 : Pas de rafraichissement sur PMV sans action 1.29
* GRO   09/10/20 : R�cup�ration nom destination dans et_calcul_temps_de_parcours DEM-SAE152 1.34
* JPL	23/10/20 : Mise en majuscules du texte avant envoi (DEM SAE168)  1.35
* ABE	24/11/20 : Modification appel a XZA0962 suite a la modification de la procedure DEM-SAE152 1.36
* ABE	03/12/20 : Changement des seuils de validation TDP_FCD pour un const declar�e dans xdc.h DEM-SAE152 1.37
* JPL	02/06/21 : Calcul_traversee: verif. no PMV, gestion TdP. Raf. PMV: init. vars, traces (SAE-286)  1.38
* JPL	02/06/21 : Cadrer a gauche l'affichage precedent (pour les lignes de longueur > 15)  1.39
* ABE	26/07/21 : TDP FCD Hybride sur trajets hors réseau DEM-SAE318 1.40
* ABE   15/11/21 : Ajout du sens du trajet vers la limite du réseau DEM-SAE318 1.41
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

#include "../../SAIDEC/inc/drol_caltdp.h"

/* declaration des donnees globales */
extern XDY_District vg_site;

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* Declaration de variables locales */
/* -------------------------------- */

static char *version = "@(#)etdp_pmv.c	1.14 06/23/11      : etdp_pmv" ;

/* Definition de fonctions externes */
/* -------------------------------- */
extern char *etdp_ligne_majuscules(char *);

		
int et_calcul_traversee(XDY_Eqt       va_NumPMV,
			XDY_TDP *     va_tdp)
{				
  ETDP_CONFIG_PMV *             pl_liste_pmv;
  XZEZT_Liste_Zdp *             pl_liste_zdp;
  XZEZT_Liste_Zdp *             pl_liste_zdp2;
  XDY_PR vl_pr_queue, vl_pr_tete,vl_pr_min, vl_pr_max;
  int vl_numero;
  XDY_Octet vl_cle;
  XDY_TDP       		        vl_tdp_fcd=0;
  XDY_Octet    			        vl_validite_fcd=0;
  XDY_Booleen			        vl_validite_faible=1;
  int 							vl_ValRet;
  XDY_Octet						vl_inhb;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : et_calcul_traversee");
  *va_tdp=0;

  /* Test pour savoir si les tdp sur cette eqt sont inhibe */
  xzae59_Rechercher_EQT_TDP_INHIB(va_NumPMV,XDC_EQT_PMV,&vl_inhb);
  if (vl_inhb)
  {
	XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_traversee TDP PMV %d inhibe, on retourne -1",va_NumPMV);
	*va_tdp=-1;
	return (XDC_OK);
  }

  /*recherche du PMV dans la liste*/
  for ( pl_liste_pmv = pg_ListePMV; pl_liste_pmv != NULL; pl_liste_pmv = pl_liste_pmv->suivant )
    if ( pl_liste_pmv->PMV.Numero == va_NumPMV )
    {
       break;
    }

  if ( pl_liste_pmv == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "et_calcul_traversee : PMV %d NON TROUVE", va_NumPMV);
    return (XDC_NOK);
  }
  else XZST_03EcritureTrace(XZSTC_INFO, "et_calcul_traversee : PMV %d, Loc %d-%d-%d, Action %d", pl_liste_pmv->PMV.Numero, pl_liste_pmv->PMV.Autoroute, pl_liste_pmv->PMV.Sens, pl_liste_pmv->PMV.PR, pl_liste_pmv->NumAction);

  /*recherche la FMC bouchon associee a ce PMV*/
  vl_ValRet = XZAC96_Lire_FMC_Data(pl_liste_pmv->NumAction, va_NumPMV, &vl_numero, &vl_cle, &vl_pr_queue, &vl_pr_tete);
  if ( vl_ValRet == XDC_OK ){
    XZST_03EcritureTrace(XZSTC_FONCTION, "FMC bouchon trouvee %d/%d, PR %d/%d",
			vl_numero , vl_cle, vl_pr_queue, vl_pr_tete);
  } else {
    XZST_03EcritureTrace(XZSTC_WARNING, "et_calcul_traversee : pb appel a XZAC96_Lire_FMC_Data");
    return (XDC_NOK);
  }
  vl_pr_min = vl_pr_queue <= vl_pr_tete ? vl_pr_queue : vl_pr_tete;
  vl_pr_max = vl_pr_queue >= vl_pr_tete ? vl_pr_queue : vl_pr_tete;

  if ( pl_liste_pmv != NULL )
  {
    pl_liste_zdp = pl_liste_pmv->Graphe_Destination;


  /*Recuperation du TDP grace a l'appel a la procedure XZAO962 (entre PMV et fin bouchon si bouchon est sur la m�me autoroute*/

  if (XZAO962_Calcul_TDP_FCD(  	pl_liste_pmv->PMV.Autoroute,
                            		pl_liste_pmv->PMV.PR,
                            		pl_liste_pmv->PMV.Sens,
                            		0,
									vl_pr_tete,
                            		0,
                                    vl_numero,
                                    vl_cle,
                            		&vl_tdp_fcd,
                            		&vl_validite_fcd,
                            		&vl_validite_faible)!= XDC_OK)
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_traversee : pb appel a XZAO962_Calcul_TDP_FCD");
        return (XDC_NOK);
  }
  
  /*Si vl_validite_fcd >= 60% et que toutes les donn�es ont une validite > 20% (vl_validite_faible = 0) alors on utilise le temps de parcous de la procedure XZAO962*/
  if((vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD) && (vl_validite_faible == 0))
    {		
        *va_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi a la minute superieure*/
	    XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_traversee: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d validite tdp_fcd>%d. Tdp=%d", pl_liste_pmv->PMV.Autoroute, pl_liste_pmv->PMV.PR,vl_validite_fcd,vl_validite_faible,XDC_SEUIL_AFFICHAGE_TDP_FCD, vl_tdp_fcd);
        return (XDC_OK);
    }
  else{

        XZEZ10_Calcul_Traversee(pl_liste_pmv->PMV.Autoroute, pl_liste_pmv->PMV.PR, pl_liste_pmv->PMV.Sens, vl_pr_min, vl_pr_max, pl_liste_zdp, 
	    pg_ListeZone,va_tdp);

        if (*va_tdp<=0 && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA) // Si le tdp est n�gatif ou nul et si la validite issue de XZAO962 >=40% alors on recupere le tdp issu de XZAO962*/
        {
            *va_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi a la minute superieure*/
                XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_traversee: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d, validite tdp_fcd>%d & ESCOTA tdp<=0. Tdp=%d",pl_liste_pmv->PMV.Autoroute, pl_liste_pmv->PMV.PR,vl_validite_fcd,vl_validite_faible,XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA, *va_tdp);
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : traversee %d",*va_tdp);
                return (XDC_OK);
        }
        else // Sinon on utilise le tdp issu de XZEZ10_Calcul_Traversee
        {       XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_traversee: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d. Tdp ESCCOTA=%d",pl_liste_pmv->PMV.Autoroute, pl_liste_pmv->PMV.PR,vl_validite_fcd,vl_validite_faible, *va_tdp);
                XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : traversee %d",*va_tdp);
                return (XDC_OK);
        }
  }
}				

  return (XDC_OK);
}		

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Calcul d'un temps de parcours pour une destination et un PMV donn�s
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_calcul_temps_de_parcours ( XDY_Eqt	va_NumPMV, 
				  XDY_Eqt	va_NumDest,
				  XDY_TDP *	va_tdp ,
				  char	*va_nom)

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
*	Calcul d'un temps de parcours pour une destination et un PMV donn�s
*
-----------------------------------------------------------------------------------------*/
{
  ETDP_CONFIG_PMV *		pl_liste_pmv;
  XZEZT_Liste_Zdp *		pl_liste_zdp;
  XZEZT_Liste_Zdp *		pl_liste_zdp2;
  XDY_PR        		va_pr_pmv=-1;
  XDY_Autoroute 		va_autoroute_pmv=0;
  XDY_Sens     			va_sens_pmv=0;
  XDY_PR        		vl_PR_d=-1;
  XDY_Autoroute 		vl_autoroute_d=0;
  XDY_Sens     			vl_sens_d=0;
  XZEZT_Liste_Destinations *	pl_Dest;
  XDY_TDP       		vl_tdp_fcd=0;
  XDY_Octet    			vl_validite_fcd=0;
  XDY_Booleen			vl_validite_faible=1;
  XDY_PR				va_pr_limite=0;
  XDY_Booleen 			vl_destination_hors_reseau;
  XDY_Booleen			vl_FCD_calcul_hors_reseau=XDC_FAUX;
  XDY_Autoroute			va_autoroute_limite=0;
  XDY_Sens     			va_sens_limite=0;
  XDY_Octet						vl_inhb;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : et_calcul_temps_de_parcours pour PMV : %d Dest : %d", va_NumPMV, va_NumDest );
  
  /* Test pour savoir si les tdp sur cette eqt sont inhibe */
  xzae59_Rechercher_EQT_TDP_INHIB(va_NumPMV,XDC_EQT_PMV,&vl_inhb);
  if (vl_inhb)
  {
	XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours TDP PMV %d inhibe, on retourne -1",va_NumPMV);
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
       strncpy(va_nom,pl_Dest->destination.Libelle_PMV,14);
       va_nom[14]='\0';		
       break;
    }
  }
  
  
  /* Recherche du PMV dans la liste et recuperation du numero d'autoroute, du PR et du sens du PMV */
  for ( pl_liste_pmv = pg_ListePMV; pl_liste_pmv != NULL; pl_liste_pmv = pl_liste_pmv->suivant )
    if ( pl_liste_pmv->PMV.Numero == va_NumPMV )
    {
      va_autoroute_pmv=pl_liste_pmv->PMV.Autoroute;
	  va_pr_pmv=pl_liste_pmv->PMV.PR;
      va_sens_pmv=pl_liste_pmv->PMV.Sens;
      break;
    }
  
  /* Si le PMV existe alors calcul du temps de parcours */
  
  if ( pl_liste_pmv != NULL) 
  {

	vl_destination_hors_reseau=XZAO29_calcul_zone_hors_reseau (vl_autoroute_d,
                            		vl_PR_d,
                            		va_sens_pmv,
                            		vl_sens_d,
									&va_pr_limite,
									&va_autoroute_limite,
									&va_sens_limite); //Test si destination hors réseau ESCOTA 1=(ORANGE/MONTPELLIER/MARSEILLE)  ou (MONACO/VINTIMILLE)
	
	vl_FCD_calcul_hors_reseau=vl_destination_hors_reseau; //Pour ne pas envoyer le TDP FCD si vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA et que FCD mauvais et donc terrain calcul complet mais r�sultat HS, alors que FCD est un calcul non complet
	
	/*Recuperation du TDP grace a l'appel a la procedure XZAO962*/

	XZST_03EcritureTrace(XZSTC_WARNING,"etdp_pmv : et_calcul_temps_de_parcours Autoroute dest = %d, PR_limite = %d, PR_dest=%d",vl_autoroute_d,va_pr_limite,vl_PR_d);

  	if (XZAO962_Calcul_TDP_FCD(  	va_autoroute_pmv,
                            		va_pr_pmv,
                            		va_sens_pmv,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_autoroute_limite : vl_autoroute_d,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_pr_limite : vl_PR_d, //Si on est hors réseau ORANGE/MONTPELLIER/MARESEILLE On calcul jusqu'à la limite du réseau, sinon les TDP_FCD sont fournis
                            		vl_destination_hors_reseau == XDC_VRAI ? va_sens_limite : vl_sens_d,
									0,
									0,		
                            		&vl_tdp_fcd,
                            		&vl_validite_fcd,
                            		&vl_validite_faible)!= XDC_OK)
	{
        XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours : pb appel a XZAO962_Calcul_TDP_FCD");
        return (XDC_NOK);
    }
	 
  	/*Si vl_validite_fcd >= 60%, que toutes les donn�es ont une validite > 20% (vl_validite_faible = 0) et qu'on est sur un calcul réseau interne ESCOTA alors on utilise le temps de parcous de la procedure XZAO962*/
  	if((vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD) && (vl_validite_faible == 0)&& vl_destination_hors_reseau == XDC_FAUX)
	{
		*va_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi � la minute superieure*/ 
		XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours pour PMV : %d, validite TDP_FCD>%d Dest : %d Tdp : %d", va_NumPMV, XDC_SEUIL_AFFICHAGE_TDP_FCD, va_NumDest, *va_tdp );
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMV");
		return (XDC_OK);
	}
  
  	/*Si vl_validite_fcd < 60% ou qu'on est sur un calcul hors réseau on utilise la xzez02 pour la partie hors réseau*/
	
	else
	{
		if (vl_destination_hors_reseau == XDC_VRAI &&  (vl_validite_fcd<=XDC_SEUIL_AFFICHAGE_TDP_FCD || vl_validite_faible != 0)) // Si on a le TDP FCD non valide et qu'on est hors reseau, alors on calcul tout le trajet avec les donnees terrains
		{
			vl_destination_hors_reseau=XDC_FAUX;
		}
		else if (vl_destination_hors_reseau == XDC_VRAI) // Si la destination est hors reseau on le donne en nom de destination a la xzez02 pour qu'il exclut les zones non virtuelles
			strcpy(va_nom,"HORS RESEAU");

		XZST_03EcritureTrace(XZSTC_FONCTION,"d1=%d, d2=%d, d3=%d",
			pl_liste_pmv->Destination,
			pl_liste_pmv->DestinationLointaine,
			pl_liste_pmv->DestinationNoeud);
		if (va_NumDest==pl_liste_pmv->Destination)
			pl_liste_zdp = pl_liste_pmv->Graphe_Destination;
		else if (va_NumDest==pl_liste_pmv->DestinationLointaine)
			pl_liste_zdp = pl_liste_pmv->Graphe_DestinationLointaine;
		else
			pl_liste_zdp = pl_liste_pmv->Graphe_DestinationNoeud;
		for (pl_liste_zdp2=pl_liste_zdp;pl_liste_zdp2!=NULL;pl_liste_zdp2=pl_liste_zdp2->suivant) {
			XZST_03EcritureTrace(XZSTC_FONCTION,"%d %d",
			pl_liste_pmv->PMV.Numero,
			pl_liste_zdp2->zdp.Code_Zone);
		}
		XZEZ02_Calcul_TDP (pl_liste_pmv->PMV.Autoroute,
				vl_destination_hors_reseau == XDC_VRAI ? va_pr_limite : pl_liste_pmv->PMV.PR,
					//pl_liste_pmv->PMV.PR,
				pl_liste_pmv->PMV.Sens,
				va_NumDest,
				pl_liste_zdp /*pg_ListeZone*/,
				pg_ListeDest,
				pg_ListeZone,
				va_tdp,va_nom );
	
		/*Tdp en fonction du tdp issu de XZEZ02_Calcul_TDP*/
			
		// TDP HYBRIDE
		if(*va_tdp>=0 && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD && (vl_validite_faible == 0) && (vl_destination_hors_reseau==XDC_VRAI))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours TDP hybride. FCD = %d, XZEZ02 = %d",(vl_tdp_fcd + 59) / 60,*va_tdp);
			*va_tdp = *va_tdp + (vl_tdp_fcd + 59) / 60;
			XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours TDP hybride. TDP =%d",*va_tdp);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMV ");
			return (XDC_OK);
		}

	
		XZST_03EcritureTrace(XZSTC_FONCTION,"et_calcul_temps_de_parcours : XZEZ02 TDP = %d",*va_tdp);
		if (*va_tdp<=0 && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA && vl_FCD_calcul_hors_reseau==XDC_FAUX) /*Si le tdp vaut -1 et si la validite issue de XZAO962 >=40% alors on recupere le tdp issu de XZAO962*/
		{
			*va_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi � la minute superieure*/
			XZST_03EcritureTrace(XZSTC_WARNING,"et_calcul_temps_de_parcours pour PMV : %d , TDP equi <0 et validite TDP_FCD>=%d Dest : %d Tdp : %d", va_NumPMV, XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA, va_NumDest, *va_tdp );
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMV");
			return (XDC_OK);
		}
		else /*Sinon on utilise le tdp issu de XZEZ02_Calcul_TDP*/
		{
			XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours pour PMV : %d Dest : %d Tdp : %d", va_NumPMV, va_NumDest, *va_tdp );
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_calcul_temps_de_parcours pour PMV ");
			return (XDC_OK);
		}
	}
  }
  else
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"OUT: et_calcul_temps_de_parcours pour PMV : %d non trouve", va_NumPMV );
     return (XDC_NOK);
  }
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Calcul des franchissement de seuil des zones temps de parcours
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_calcul_seuil_TDP ( XDY_Eqt	va_NumUGTP )

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
*	Calcul des franchissement de seuil des zones temps de parcours
*
-----------------------------------------------------------------------------------------*/
{
  XDY_Octet			vl_EtatSeuil = 0;
  XZEZT_Liste_Zdp *	pl_Zone;

  XZST_03EcritureTrace( XZSTC_INTERFACE,"et_calcul_seuil_TDP : in %d",va_NumUGTP);
  /* Recherche de la zone dans la liste */
  for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
    if ( (pl_Zone->zdp.Maitre == vg_DonneesUGTP[va_NumUGTP].Numero) &&
         (pl_Zone->zdp.Code_Zone != 0) )
    {
    
	 XZST_03EcritureTrace( XZSTC_INFO,"SEUIL %d %d %d",
				va_NumUGTP,
				pl_Zone->zdp.Code_Zone,
				vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone]);

        if ( (vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone] == XDC_VRAI) &&
             !(vg_DonneesUGTP[va_NumUGTP].Zone[pl_Zone->zdp.Code_Zone].Etat_Service) )
          pl_Zone->zdp.TDP_Courant = vg_Mesure[va_NumUGTP].TDP6mn[pl_Zone->zdp.Code_Zone];
	else /*PNI : DEM464 : Si les derni�res 6mn sont invalident alors le TDP_Courant est nul (invalide) */
         pl_Zone->zdp.TDP_Courant = 0;
	  
        vl_EtatSeuil = XDC_SEUIL_TDP_ABSENT;
        if ( (pl_Zone->zdp.Franchissement_Seuil == XDC_SEUIL_TDP_ABSENT) &&
             (pl_Zone->zdp.TDP_Courant > pl_Zone->zdp.Seuil_Haut) )
        {
            vl_EtatSeuil = XDC_SEUIL_TDP_HAUT;
            pl_Zone->zdp.Franchissement_Seuil = XDC_SEUIL_TDP_HAUT;
        }
        if ( (pl_Zone->zdp.Franchissement_Seuil != XDC_SEUIL_TDP_ABSENT) &&
             (pl_Zone->zdp.TDP_Courant < pl_Zone->zdp.Seuil_Bas) )
        {
            vl_EtatSeuil = XDC_SEUIL_TDP_BAS;
            pl_Zone->zdp.Franchissement_Seuil = XDC_SEUIL_TDP_ABSENT;
        }
        
        if ( vl_EtatSeuil != XDC_SEUIL_TDP_ABSENT )
        {
           XZST_03EcritureTrace( XZSTC_WARNING,"Envoi franchissement seuil %s pour zone:%d", 
           			(vl_EtatSeuil == XDC_SEUIL_TDP_HAUT) ? "HAUT" : "BAS",
           			pl_Zone->zdp.Numero );
           XZAD10_Seuil_TDP(	pl_Zone->zdp.Numero,
				pl_Zone->zdp.Site,
				vl_EtatSeuil,
				vg_DonneesUGTP[va_NumUGTP].Horodate6mn );
        }
        
    }

    return (XDC_OK);
}



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Determine si une ligne de texte affiche ou non le Temps de parcours
*	pour une destination donnee par son libelle.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen et_Affichage_Destination ( char *pa_Txt_Affiche, char *pa_Lib_Destination)
{
  char	*pl_Affich;
  char	*pl_Dest;

  /* Ignorer les espaces en debut des lignes */
  for ( pl_Affich = pa_Txt_Affiche;  *pl_Affich == ' ';  pl_Affich++ )
    ;

  for ( pl_Dest = pa_Lib_Destination;  *pl_Dest == ' ';  pl_Dest++ )
    ;

  /* Le TdP pour la destination est affiche si le texte commence par le libelle destination */
  if ( strncmp ( pl_Affich, pl_Dest, strlen ( pl_Dest) ) == 0 ) {
    return ( XDC_VRAI );
  }

  return ( XDC_FAUX );
}



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Rafraichissement des PMV
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_Rafraichissment_PMV (  )

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
*	Rafrachissement des PMV
*
-----------------------------------------------------------------------------------------*/
{
  XDY_TDP			vl_TDP = 0;
  ETDP_CONFIG_PMV *		pl_PMV;
  XDY_Etat_PMV			vl_EtatPMV_prec, vl_EtatPMV;
  char	vl_texte[255];
  int				vl_js;
  XDY_Horodate			vl_horodate;
  char		vl_nom[15] = "";
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : et_Rafraichissment_PMV " );
  
  /* Recherche du PMV dans la liste */

for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant ) {
    if ((pl_PMV->PMV.SiteGestion != vg_site) || (pl_PMV->NumAction <=0)) {
	XZST_03EcritureTrace(XZSTC_INFO,"PMV=%d site=%d ou Pas daction=%d",pl_PMV->EtatPMV.Numero,pl_PMV->PMV.SiteGestion,pl_PMV->NumAction);
      continue;
	}

    if ( (pl_PMV->EtatPMV.Destination || pl_PMV->EtatPMV.DestinationAlternat || pl_PMV->EtatPMV.DestinationNoeud) )
    {
	XZST_03EcritureTrace(XZSTC_INFO,"PMV=%d Dest=%d;Alt=%d;Noeud=%d, action=%d",pl_PMV->EtatPMV.Numero,pl_PMV->EtatPMV.Destination ,pl_PMV->EtatPMV.DestinationAlternat,pl_PMV->EtatPMV.DestinationNoeud,pl_PMV->NumAction);
	/* Cadrer a gauche l'affichage precedent (pour les lignes de longueur > 15) */
	memcpy ( &vl_EtatPMV_prec, &pl_PMV->EtatPMV, sizeof(XDY_Etat_PMV) );
	XDY_Supprimer_Espaces ( vl_EtatPMV_prec.Texte1 );
	XDY_Supprimer_Espaces ( vl_EtatPMV_prec.Texte2 );
	XDY_Supprimer_Espaces ( vl_EtatPMV_prec.Texte3 );

	vl_EtatPMV = vl_EtatPMV_prec;
	vl_EtatPMV.Clign1 = vl_EtatPMV.Clign2 = vl_EtatPMV.Clign3 = vl_EtatPMV.Clign4 = XDC_PMV_PASCLIGNO;

	if ( pl_PMV->EtatPMV.Destination ) 
	{
	  if ( (pl_PMV->EtatPMV.Destination==XDC_DESTINATION_TRAVERSEE) &&
	     (pl_PMV->EtatPMV.DestinationAlternat==XDC_DESTINATION_TRAVERSEE) &&
	     (pl_PMV->EtatPMV.DestinationNoeud==XDC_DESTINATION_TRAVERSEE)) {
		et_calcul_traversee(pl_PMV->EtatPMV.Numero, &vl_TDP);
		if (!strncmp(vl_EtatPMV.Texte3, "TRAVERSEE", 9)) {
		    if (vl_TDP > 600) {
		      strcpy(vl_EtatPMV.Texte3,"");
		    } else if (vl_TDP > 59) {
		      sprintf( &vl_EtatPMV.Texte3[11], "%1dH%02d", vl_TDP/60, vl_TDP%60 );
		      vl_EtatPMV.Texte3[15] = '\0';
		    }
		    else if (vl_TDP > 0) {
		      sprintf(&vl_EtatPMV.Texte3[11],"%2dMN",vl_TDP);
		      vl_EtatPMV.Texte3[15] = '\0';
		    }
		}
	  }
	  else {
		et_calcul_temps_de_parcours(pl_PMV->EtatPMV.Numero, pl_PMV->EtatPMV.Destination, &vl_TDP,vl_nom);
		XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours Dest1 Tdp=%d, Destination=%s(%d cars), Dest en ligne 1:%d - en ligne 3:%d", vl_TDP,vl_nom,strlen(vl_nom), et_Affichage_Destination(vl_EtatPMV.Texte1,vl_nom), et_Affichage_Destination(vl_EtatPMV.Texte3,vl_nom));

		if (et_Affichage_Destination(vl_EtatPMV.Texte1,vl_nom) == XDC_VRAI) {
			
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMV.Texte1,"               ");
				sprintf(&vl_EtatPMV.Texte1[11],"xxMN");
				vl_EtatPMV.Texte1[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMV.Texte1[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMV.Texte1[15] = '\0';
			}
			else if (vl_TDP > 0) {
				sprintf(&vl_EtatPMV.Texte1[11],"%2dMN",vl_TDP);
				vl_EtatPMV.Texte1[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMV.Texte1,"               ");
				sprintf(&vl_EtatPMV.Texte1[11],"xxMN");
				vl_EtatPMV.Texte1[15] = '\0';
			}
		}

		if (et_Affichage_Destination(vl_EtatPMV.Texte3,vl_nom) == XDC_VRAI) {
			
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMV.Texte3,"               ");
				sprintf(&vl_EtatPMV.Texte3[11],"xxMN");
				vl_EtatPMV.Texte3[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMV.Texte3[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMV.Texte3[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMV.Texte3[11],"%2dMN",vl_TDP);
				vl_EtatPMV.Texte3[15] = '\0';
			}
		}
	  }
	}

	if ( ( pl_PMV->EtatPMV.DestinationAlternat ) && (pl_PMV->EtatPMV.DestinationAlternat!=XDC_DESTINATION_TRAVERSEE))
	{
		et_calcul_temps_de_parcours ( pl_PMV->EtatPMV.Numero, pl_PMV->EtatPMV.DestinationAlternat, &vl_TDP,vl_nom );
		XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours Alt Tdp=%d, Destination=%s(%d cars), Dest en ligne 2:%d - en ligne 3:%d", vl_TDP,vl_nom,strlen(vl_nom), et_Affichage_Destination(vl_EtatPMV.Texte2,vl_nom), et_Affichage_Destination(vl_EtatPMV.Texte3,vl_nom));

		if (et_Affichage_Destination(vl_EtatPMV.Texte2,vl_nom) == XDC_VRAI) {
			
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMV.Texte2,"               ");
				sprintf(&vl_EtatPMV.Texte2[11],"xxMN");
				vl_EtatPMV.Texte2[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMV.Texte2[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMV.Texte2[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMV.Texte2[11],"%2dMN",vl_TDP);
				vl_EtatPMV.Texte2[15] = '\0';
			}
			else {
				strcpy(vl_EtatPMV.Texte2,"               ");
				sprintf(&vl_EtatPMV.Texte2[11],"xxMN");
				vl_EtatPMV.Texte2[15] = '\0';
			}
		}

		if (et_Affichage_Destination(vl_EtatPMV.Texte3,vl_nom) == XDC_VRAI) {
			
			if ( vl_TDP > 600 )
			{
				strcpy(vl_EtatPMV.Texte3,"               ");
				sprintf(&vl_EtatPMV.Texte3[11],"xxMN");
				vl_EtatPMV.Texte3[15] = '\0';
			}
			else if ( vl_TDP > 59 )
			{
				sprintf(&vl_EtatPMV.Texte3[11],"%1dH%02d",vl_TDP/60,vl_TDP%60);
				vl_EtatPMV.Texte3[15] = '\0';
			}
			else if (vl_TDP > 0)
			{
				sprintf(&vl_EtatPMV.Texte3[11],"%2dMN",vl_TDP);
				vl_EtatPMV.Texte3[15] = '\0';
			}
		}
	}
		  
//TODO
	if ( ( pl_PMV->EtatPMV.DestinationNoeud  ) && (pl_PMV->EtatPMV.DestinationNoeud!=XDC_DESTINATION_TRAVERSEE))
	{
		et_calcul_temps_de_parcours ( pl_PMV->EtatPMV.Numero, pl_PMV->EtatPMV.DestinationNoeud, &vl_TDP ,vl_nom);
		XZST_03EcritureTrace(XZSTC_INFO,"et_calcul_temps_de_parcours 3 Tdp=%d, Destination=%s(%d cars), Dest en ligne 3:%d", vl_TDP,vl_nom,strlen(vl_nom), et_Affichage_Destination(vl_EtatPMV.Texte3,vl_nom));

		if (et_Affichage_Destination(vl_EtatPMV.Texte3,vl_nom) == XDC_VRAI) {
	
                                if ( vl_TDP > 600 )
                                {
                                        sprintf( &vl_EtatPMV.Texte3[11], ETDPC_TEXTE_TDP_HOR, 9, 59 );
                                        vl_EtatPMV.Texte3[15] = '\0';
                                }
                                /* la duree est comprise entre 1 heure et 10 heures */
                                else if ( vl_TDP > 59 )
                                {
                                        sprintf( &vl_EtatPMV.Texte3[11], ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
                                        vl_EtatPMV.Texte3[15] = '\0';
                                }
								else if ( vl_TDP > 0 ) 
								{
								sprintf ( &vl_EtatPMV.Texte3[11], ETDPC_TEXTE_TDP, vl_TDP );
									vl_EtatPMV.Texte3[15] = '\0';
								}
								else
								{
									vl_EtatPMV.Texte3[0] = '\0';
								if (vl_EtatPMV.DestinationNoeud!=0) vl_EtatPMV_prec.Texte3[11] = vl_EtatPMV_prec.Texte3[12] = 'x';	/* V1.11 */
								}
		}
	}

 /*XZST_03EcritureTrace(XZSTC_WARNING,"et_Rafraichissment_PMV avant trim texte1:%s texte2:%s texte3:%s",vl_EtatPMV.Texte1, vl_EtatPMV.Texte2,vl_EtatPMV.Texte3 );*/

  /*A on supprime les espaces de debut de ligne */
    if (!strncmp(vl_EtatPMV.Texte1,"          ",10))
      vl_EtatPMV.Texte1[0]= '\0';
    if (!strncmp(vl_EtatPMV.Texte2,"          ",10))
      vl_EtatPMV.Texte2[0]= '\0';
    if (!strncmp(vl_EtatPMV.Texte3,"          ",10))
      vl_EtatPMV.Texte3[0]= '\0';

  /*A on met le texte du message en majuscules */
//  etdp_ligne_majuscules(vl_EtatPMV.Texte1);
//  etdp_ligne_majuscules(vl_EtatPMV.Texte2);
//  etdp_ligne_majuscules(vl_EtatPMV.Texte3);

 XZST_03EcritureTrace(XZSTC_WARNING,"et_Rafraichissment_PMV precedent  texte1:%s, texte2:%s, texte3=%s", pl_PMV->EtatPMV.Texte1, pl_PMV->EtatPMV.Texte2, pl_PMV->EtatPMV.Texte3 );
 XZST_03EcritureTrace(XZSTC_WARNING,"et_Rafraichissment_PMV texte1:%s,texte2:%s,texte3:%s; diff=%d,Cmd=%d", vl_EtatPMV.Texte1, vl_EtatPMV.Texte2, vl_EtatPMV.Texte3,( strcmp(vl_EtatPMV.Texte2,vl_EtatPMV_prec.Texte2) ||strcmp(vl_EtatPMV.Texte1,vl_EtatPMV_prec.Texte1) || strcmp(vl_EtatPMV.Texte3,vl_EtatPMV_prec.Texte3) ), pl_PMV->NumAction);

          if ( strcmp(vl_EtatPMV.Texte2,vl_EtatPMV_prec.Texte2) ||
               strcmp(vl_EtatPMV.Texte1,vl_EtatPMV_prec.Texte1) || 
			   strcmp(vl_EtatPMV.Texte3,vl_EtatPMV_prec.Texte3) )
          {
            /* PNI : 1.10  memcpy ( &pl_PMV->EtatPMV, &vl_EtatPMV, sizeof(XDY_Etat_PMV) ); on conserve la commande initiale d'affichage tdp */

	     /* S'il y a une destination valide , on l'affiche, sinon on met l'heure */
	     if (( vl_EtatPMV.Texte2[0] != '\0' ) || (vl_EtatPMV.Texte1[0] != '\0') || (vl_EtatPMV.Texte3[0] != '\0'))
	     {
                /* PNI : 1.10 memcpy ( &pl_PMV->EtatPMV, &vl_EtatPMV, sizeof(XDY_Etat_PMV) ); on conserve la commande initiale d'affichage tdp */
XZST_03EcritureTrace(XZSTC_WARNING,"envoi cmde %d texte1:%s texte2:%s texte3:%s",pl_PMV->NumAction,vl_EtatPMV.Texte1, vl_EtatPMV.Texte2, vl_EtatPMV.Texte3);
             	XZEP03_Commande_Raf_TDP_PMV ( vl_EtatPMV , pl_PMV->NumAction, pl_PMV->PMV.NomMachine );
		if  (vl_EtatPMV.Texte1[0] != '\0')	strcpy(pl_PMV->EtatPMV.Texte1,vl_EtatPMV.Texte1);
		if  (vl_EtatPMV.Texte2[0] != '\0')	strcpy(pl_PMV->EtatPMV.Texte2,vl_EtatPMV.Texte2);
		if  (vl_EtatPMV.Texte3[0] != '\0')	strcpy(pl_PMV->EtatPMV.Texte3,vl_EtatPMV.Texte3);
	     }
 	     else  {

                /* PNI : 1.10 memcpy ( &pl_PMV->EtatPMV, &vl_EtatPMV, sizeof(XDY_Etat_PMV) ); on conserve la commande initiale d'affichage tdp */
		/* S'il y a une action alors terminer l'action */
		if ( pl_PMV->NumAction > 0 )	{
             		XZEP03_Commande_Raf_TDP_PMV ( vl_EtatPMV , pl_PMV->NumAction, pl_PMV->PMV.NomMachine );
			if  (vl_EtatPMV.Texte1[0] != '\0')	strcpy(pl_PMV->EtatPMV.Texte1,vl_EtatPMV.Texte1);
			if  (vl_EtatPMV.Texte2[0] != '\0')	strcpy(pl_PMV->EtatPMV.Texte2,vl_EtatPMV.Texte2);
			if  (vl_EtatPMV.Texte3[0] != '\0')	strcpy(pl_PMV->EtatPMV.Texte3,vl_EtatPMV.Texte3);
	         	 /* Lecture vl_horodate */
        	 	/* XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );*/
	         	/* xzat25_CompteRendu_PMV ( -pl_PMV->NumAction, vl_horodate, XDC_NOK,pl_PMV->PMV.SiteGestion );*/
		 	/* XZEP01_Commande_PMV ( vl_EtatPMV , pl_PMV->NumAction, pl_PMV->PMV.NomMachine );*/
		}
 XZST_03EcritureTrace(XZSTC_WARNING,"Plus de destination affichage texte1:%s texte2:%s texte3:%s",pl_PMV->EtatPMV.Texte1, pl_PMV->EtatPMV.Texte2, pl_PMV->EtatPMV.Texte3); 		
	     }
		
          }		
    
      }

  }
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: et_Rafraichissment_PMV " );

    return (XDC_OK);
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Recherche si zone concerner par affichage PMV 
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int et_Recherche_zone_PMV ( 	XDY_Autoroute			va_autoroute_pmv,
				XDY_PR				va_pr_pmv,
				XDY_Sens			va_sens_pmv,
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
*	Recherche si zone concerner par affichage PMV 
*
-----------------------------------------------------------------------------------------*/
{
static char *version = "@(#)etdp_pmv.c	1.14 06/23/11      : et_Recherche_zone_PMV" ;

XDY_PR				vl_pr_destination=0;
XDY_Booleen			vl_Type_Loc=0;
XZEZT_Liste_Zdp *		pl_Zone;
XZEZT_Liste_Destinations *	pl_Dest;


  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s auto:%d pr:%ld sens:%d dest:%d  zone:%d", version, va_autoroute_pmv, va_pr_pmv, va_sens_pmv, va_dest, va_NumZone );

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
       if ( (pl_Zone->zdp.Autoroute==va_autoroute_pmv) && 
            (pl_Zone->zdp.Sens==va_sens_pmv) )  
       {
         /* Traitement du sens NORD */
         if ( (va_sens_pmv == XDC_SENS_NORD) || (va_sens_pmv == XDC_SENS_ENTREE_NORD) )
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
                  (pl_Zone->zdp.PR_Debut < va_pr_pmv) && 
                  (va_pr_pmv <= pl_Zone->zdp.PR_Fin) )
             {
                /* Zone entre PMV et la fin de la zdp du PMV */
                XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone PMV " );
                return (XDC_OK);
             }
             /* Zone entierement comprises entre le PMV et la destination*/
             if ( (pl_Zone->zdp.Type_Zone == XDC_ZDP_TDP_SECTION) &&
                  (va_pr_pmv > pl_Zone->zdp.PR_Fin) &&
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
                  (pl_Zone->zdp.PR_Debut <= va_pr_pmv) && 
                  (va_pr_pmv < pl_Zone->zdp.PR_Fin) )
             {
                 /* Zone entre PMV et la fin de la zdp du PMV */
                 XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone PMV " );
                 return (XDC_OK);
             }
             /* Zone entierement comprises entre le PMV et la destination*/
             if ( (pl_Zone->zdp.Type_Zone == XDC_ZDP_TDP_SECTION) &&
                  (va_pr_pmv < pl_Zone->zdp.PR_Debut) && 
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


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
* Met en majuscules la cha�ne fournie en entree.
*-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *etdp_ligne_majuscules (char *Texte)

/*
* ARGUMENTS EN ENTREE :
*   Une chaine de caracteres
*
* ARGUMENTS EN SORTIE :
*   La chaine en majuscules
*
* CODE RETOUR :
*   La chaine en majuscules
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------------------------------*/
{
	int		i;
	int		l;
	unsigned char	c;

	l = (int) strlen(Texte);
	for (i=0; i < l; i++) {
		c = (unsigned char) Texte[i];

		switch((int) Texte[i]) {
		case 'à' :	Texte[i] = 'A';		break;
		case 'â' :	Texte[i] = 'A';		break;
		case 'é' :	Texte[i] = 'E';		break;
		case 'è' :	Texte[i] = 'E';		break;
		case 'ê' :	Texte[i] = 'E';		break;
		case 'î' :	Texte[i] = 'I';		break;
		case 'ô' :	Texte[i] = 'O';		break;
		case 'ù' :	Texte[i] = 'U';		break;
		case 'û' :	Texte[i] = 'U';		break;

		case 'À' :	Texte[i] = 'A';		break;
		case 'Â' :	Texte[i] = 'A';		break;
		case 'É' :	Texte[i] = 'E';		break;
		case 'È' :	Texte[i] = 'E';		break;
		case 'Ê' :	Texte[i] = 'E';		break;
		case 'Î' :	Texte[i] = 'I';		break;
		case 'Ô' :	Texte[i] = 'O';		break;
		case 'Ù' :	Texte[i] = 'U';		break;
		case 'Û' :	Texte[i] = 'U';		break;
		default :	Texte[i] = toupper((int) c);
		}
	}

	return(Texte);
}