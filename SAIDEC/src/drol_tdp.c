/*E*/
/*Fichier : $Id: drol_tdp.c,v 1.6 2018/06/07 09:09:56 devgfi Exp $    Release : $Revision: 1.6 $        Date : $Date: 2018/06/07 09:09:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
------------------------------------------------------
* MODULE drol_TDP * FICHIER drol_tdp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf fichier drol_tdp.h 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	24/02/97 : Creation 1.1
* JPL	14/01/11 : Migration HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=)  1.3
* JPL	14/01/11 : Migration HP ia64 (DEM 971) : Retour C.R. (drol_fct_XZAD04); Zones des portions de texte 'static'  1.4
* JMG	31/01/12 : tdp etendu dem/1014 1.5
* JPL	09/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.6
------------------------------------------------------*/

/* fichiers inclus */
#include "drol_caltdp.h"
#include "drol_tdp.h"

/* definitions de constantes */
#define CLASSE_AUTOROUTE     "autoroute"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: drol_tdp.c,v 1.6 2018/06/07 09:09:56 devgfi Exp $ $Revision: 1.6 $ $Date: 2018/06/07 09:09:56 $" ;
int  vg_num_zdp_pmv = 0;


/* declaration de fonctions internes */
static void drol_tdp_cb();
static int drol_fct_XZAD04();


static void drol_tdp_cree_obj_pmv(      XDY_Autoroute   va_autoroute,
					XDY_Sens        va_sens,
					XDY_Entier      va_code_zone,
					XDY_Entier	va_destination,
					XDY_Octet	va_type_destination,
					XDY_PR		va_pr_debut_zdp,
					XDY_PR		va_pr_debut_zdp_fourche,
					XDY_Entier	va_destination_fourche);


/* definition de fonctions externes */
extern T_PTR FctAutoroute();



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* initialisation du module de calcul des TDP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int drol_tdp_init()

/*
* ARGUMENTS EN ENTREE :
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
------------------------------------------------------*/
{
  static char *version = "@(#)drol_tdp.c	1.6 1.6 07/21/00";
	
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

  /*appel XZAD20*/
  if (XZAD20_Init_Calcul_TdP() != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tdp_init: pb dans l'appel a XZAD20_Init_Calcul_TdP");
    return (XDC_NOK);
  }

  /*appel XZEA35*/
  if (XZAD27_Abonnement_TDP((XDY_Mot) 0,drol_tdp_cb,XDG_S_TDP6) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tdp_init: pb dans l'appel a XZAD27_Abonnement_TDP");
    return (XDC_NOK);
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
  return (XDC_OK);
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* fct appelee sur reception d'un tdp
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void drol_tdp_cb(XDY_EMes_TdP_6mn	va_tdp,int *pa_resultat)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee dans xzea35
*
* FONCTION 
*
------------------------------------------------------*/
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tdp_cb: num %d, dispo %d, temps %d, horodate %lf",
					va_tdp.Numero,
					va_tdp.Dispo,
					va_tdp.Temps,
					va_tdp.Horodate);

	/*si mesure valide*/
	if (va_tdp.Dispo) {
	  /*appel XZAD21*/
	  if (XZAD21_Maj_TdP_ZdP(va_tdp.Numero,va_tdp.Temps)!=XDC_OK) {
	    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tdp_cb: pb dans l'appel a XZAD21_Maj_TdP_ZdP(%d)",
						va_tdp.Numero);
	    *pa_resultat=XDC_NOK;
	  }
	}
	*pa_resultat=XDC_OK;
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de declenchement de calcul de temps de parcours
* et de recherche des zdp perturbees et revenues a
* la normale
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int drol_tdp_calcul(	XDY_Horodate	va_horodate,
				XDY_Entier	va_numero_fmc,
				XDY_Octet	va_cle_fmc,
				XDY_Octet	va_type_pa,
				XDY_PR		va_pr,
				XDY_PR		va_prtete,
				XDY_Sens	va_sens,
				XDY_Octet	va_type_fmc)

/*
* ARGUMENTS EN ENTREE :
* va_horodate	: horodate de calcul du plan d'actions
* va_numero_fmc	: numero de la fmc
* va_cle_fmc	: cle de la fmc
* va_type_pa 	: type de plan d'actions	XZDDC_PA_AVEC_TFM/XZDDC_PA_SANS_TFM
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee dans xzea35
*
* FONCTION
*
------------------------------------------------------*/
{
 
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : drol_tdp_calcul");

  XZST_03EcritureTrace(XZSTC_INTERFACE,"drol_tdp_calcul: site %d, horodate %lf, num %d, cle %d, pr %d, prtete %d",
					vg_site,
					va_horodate,
					va_numero_fmc,
					va_cle_fmc,
					va_pr,
					va_prtete);

  /*mise a 0 des compteurs de zones */
  /*utilises pour la creation dynamiques des objets dans le RTie*/
  /*vg_num_zdp_tfm=0;*/
  vg_num_zdp_pmv=0;

  /*mise a 0 de la liste des franchissements*/
  XZAD29_Reinit_Franchissement();

  /*appel a XZAD04*/
  if (XZAD04_Liste_Franchissements(	vg_site,
					va_horodate,
					va_numero_fmc,
					va_cle_fmc,
					drol_fct_XZAD04) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tdp_calcul: pb dans l'appel a XZAD04_Liste_Franchissements");
    return (XDC_NOK);
  }

  /*si un message Trafic FM doit etre calcule POUR LA FMC BOUCHON*/
#if 0
  if ( (va_type_pa==XZDDC_PA_AVEC_TFM) && (va_type_fmc==XZAEC_FMC_QueueBouchon))
    if (XZAD25_Calculer_Zones_TFM(va_numero_fmc,va_cle_fmc,drol_tdp_cree_obj_tfm) != XDC_OK) 
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_tdp_calcul: pb dans l'appel a XZAD25_Calculer_Zones_TFM");
#endif  
  /*calcul des zones PMV*/
  if (XZAD26_Calculer_Zones_PMV(va_numero_fmc,va_cle_fmc,(XDY_Fonction)drol_tdp_cree_obj_pmv,va_pr,va_prtete,va_sens,va_type_fmc) != XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tdp_calcul: pb dans l'appel a XZAD26_Calculer_Zones_PMV");

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : drol_tdp_calcul");
  return (XDC_OK);

}



/*------------------------------------------------------
* SERVICE RENDU :
* fct utilisateur passee dans XZAD04
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_fct_XZAD04(     XZAD04_Ligne    va_seuil)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee dans xzad04
*
* FONCTION
*
------------------------------------------------------*/
{
  XDY_Octet	vl_type;
  int		vl_cr = XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : drol_fct_XZAD04");

  XZST_03EcritureTrace(XZSTC_INTERFACE,"drol_fct_XZAD04: numero %d, horodate fin %lf",
						va_seuil.Numero,
						va_seuil.HorodateFin);

  /*si l'horodate de fin est nulle */
  if (va_seuil.HorodateFin==0) 
    /*seuil haut*/
    vl_type=XDC_SEUIL_TDP_HAUT;
  else
    vl_type=XDC_SEUIL_TDP_BAS;

  /*appel XZAD24*/
  if (XZAD24_Maj_Franchissement(va_seuil.Numero,vl_type) != XDC_OK) 
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_fct_XZAD04: pb dans l'appel a XZAD24_Maj_Franchissement(%d, %d)",
					va_seuil.Numero,
					vl_type);
    vl_cr = XDC_OK;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : drol_fct_XZAD04");
  return (vl_cr);
}




/*------------------------------------------------------
* SERVICE RENDU :
* fct utilisateur passee dans XZAD26
* creation d'un objet zone_pmv dans le RTie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void drol_tdp_cree_obj_pmv(	XDY_Autoroute	va_autoroute,	
					XDY_Sens	va_sens,
					XDY_Entier	va_zone,
					XDY_Entier	va_destination,
					XDY_Octet	va_type_destination,
					XDY_PR		va_pr_debut_zdp,
					XDY_PR		va_pr_debut_zdp_fourche,
					XDY_Entier	va_destination_fourche
					)

/*
* ARGUMENTS EN ENTREE :
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
------------------------------------------------------*/
{
}




#if 0
/*------------------------------------------------------
* SERVICE RENDU :
* calcul du message temps de parcours a afficher sur 
* un PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern char *calculer_tdp_pmv_dest(	XDY_PR va_pr_pmv, XDY_Autoroute va_autoroute, XDY_Sens va_sens, XDY_Entier va_destination)

/*
* ARGUMENTS EN ENTREE :
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
------------------------------------------------------*/
{
  static T_CHAR		Texte[100];
  XDY_TDP		vl_tdp;
  XDY_Nom		vl_libelle;
  int			i=0,j=0;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s calculer_tdp_pmv_dest",version);


  XZST_03EcritureTrace(XZSTC_INTERFACE,"calculer_msg_tdp_pmv: pr %d, autoroute %d, sens %d, dest %d"
				      ,(int)va_pr_pmv
				      ,(int)va_autoroute
				      ,(int)va_sens
				      ,(int)va_destination
		      ) ;

  /*calcul du tdp entre le PMV et la destination*/
  if (XZAD23_Calculer_TdP_PMV_Dest(	(int)va_pr_pmv
					,(XDY_Autoroute)va_autoroute
					,(XDY_Sens)va_sens
					,(int)va_destination
					,&vl_tdp
					) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"calculer_msg_tdp_pmv: pb dans l'appel a XZAD23_Calculer_TdP_PMV_Dest");
      return("");
  }

  /*recupere le libelle PMV de la destination*/
  if (XZAD27_Libelle_Destination((int)va_destination,vl_libelle) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"calculer_msg_tdp_pmv: pb dans l'appel a XZAD27_Libelle_Destination");
  }

  /*A on supprime les espaces de debut de ligne */
  while ( vl_libelle[j] == ' ' )
	  j++;
  for ( i=j; i<strlen(vl_libelle); i++)
	  vl_libelle[i-j] = vl_libelle[i];
  vl_libelle[i-j] = '\0';

  /*A on supprime les espaces de fin de ligne */
  i = (int) strlen(vl_libelle);
  for (j=i-1;j>=0;j--) {
	  if ( vl_libelle[j] == ' ' )
		  vl_libelle[j] = '\0';
	  else break;
  }

  /*on renvoie le resultat  */
  sprintf(Texte,"%d MN JUSQU'A\\%s"
  		,vl_tdp
	       ,vl_libelle);
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : calculer_msg_tdp_pmv retourne %s", Texte);
  return (Texte);
}

#endif
