/*E*/
/*  Fichier : $Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $      $Revision: 1.22 $        $Date: 2021/06/15 17:25:33 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAD * FICHIER xzad.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAD101_Initialiser_TDP :
*    Initialise le module de calcul des Temps de Parcours (lecture config, abonnement).
*
**********************************
*  XZAD102_Terminer_TDP :
*    Termine l'utilisation du module (desabonnement messagerie, liberation ressources).
*
**********************************
*  XZAD103_Temps_De_Parcours_PMV :
*    Calcule et fournit a un client les donnees TdP actuelles pour un PMV donne.
*
**********************************
*  XZAD104_Temps_De_Parcours_PMVA :
*    Calcule et fournit a un client les donnees TdP actuelles pour un PMVA donne.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	08/02/13 : Creation d'apres TETDP - Initialisation seulement (DEM 1061)  1.1
* JPL	14/02/13 : Support origine et destination sur unique zone ; plus de patch Monaco  1.2
* JPL	15/02/13 : Mesures directement dans les donnees des ZDP  1.3
* JPL	15/02/13 : Table des ZDP indexee par numero et non par code  1.4
* JPL	15/02/13 : Traitement reception des mesures de TdP temps reel  1.5
* JPL	25/02/13 : Controle et memorisation destinations PMV / PMVA a la lecture configuration  1.6
* JPL	25/02/13 : Ajout fonctions de calcul de tous les TdP d'un PMV / PMVA pour un client  1.7
* JPL	27/02/13 : RAZ donnees temps des Parcours ; correction destination 2 proche PMVA  1.8
* PNI	09/07/13 : Ajout de traces pour données tdp manquante  1.9
* PNI	09/07/13 : Ne pas prendre en compte la validité de la station virtuelle  1.10
* JPL	08/02/17 : Prise en compte parametre supplementaire a la fonction d'ajout d'une ZDP  1.11
* PNI	29/03/17 : Verrue pour remplacement de la zone 172 en cas de non données (en cas de fermeture sophia) par la 173 (Antibes Est) 1.12 DEM1217
* JPL	24/04/17 : Externalisation type XZADT_Liste_ZDP et fonction XZAD_Calcul_TDP  1.13
* JPL	16/05/17 : Externalisation actions XZAD_Activer_Graphe et XZAD110_Demarrer_TDP  1.14
* JPL	05/10/17 : Lecture destinations PMV[A] : eqt., destination, code zdp sur 2 octets; xzao4001 remplace xzao400 (DEM 1236)  1.15
* JMG	13/11/19 : Tsurcharge TDP DEM1365 1.16
* LPE   21/08/20 : Ajout XZAO962_Calcul_TDP_FCD dans XZAD_Calcul_TDP DEM-SAE152 1.17
* ABE   05/10/20 : correction algo retour appel XZAO962 XZAD_Calcul_TDP DEM-SAE152 1.18
* LPE	19/11/20 : XZAD23_Calculer_TdP_PMV_Dest : Correction de l'appel a XZA0962 suite a la modification de la procédure DEM-SAE152 1.20
* ABE	03/12/20 : Changement des seuils de validation TDP_FCD pour un const declarée dans xdc.h DEM-SAE152 1.21
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */

#include	<xdy.h>
#include	<xdc.h>

#include	<xzst.h>
#include	<xzsm.h>

#include	<xzao.h>
#include	<xzao25.h>
#include	<xzao27.h>

#include	<xzad05.h>
#include	<xzad.h>


/*
** Definitions de constantes
** -------------------------
*/



/*
** Definitions de types locaux
** ---------------------------
*/

/* Liste de destinations Temps de Parcours */
typedef struct S_XZADT_Liste_Destinations
{
	XDY_Destination				destination;
	struct S_XZADT_Liste_Destinations	*suivant;
} XZADT_Liste_Destinations;



/* Donnees de mesures de Temps de Parcours */
typedef struct
{
	XDY_TDP		TDP6mn;
	XDY_Mot		Validite6mn;
} XZADT_Mesure_ZDP;



/* Liste de PMV affichant des Temps de Parcours */
typedef struct
{
	XZAOT_PMV_TDP			PMV;
	XDY_Nom				Nom;
	XDY_Destination *		Destination_Proche;
	XDY_Destination *		Destination_Loin;
	XDY_Destination *		Destination_Noeud;
	XZADT_Liste_ZDP *		Graphe_Dest_Proche;
	XZADT_Liste_ZDP	*		Graphe_Dest_Loin;
	XZADT_Liste_ZDP	*		Graphe_Dest_Noeud;
	void *				suivant;
}
XZADT_Liste_PMV_TDP;



/* Liste de PMVA affichant des Temps de Parcours */
typedef struct
{
	XZAOT_PMA_TDP		PMVA;
	XDY_Nom			Nom;
	XDY_Destination *	Destination1_proche;
	XDY_Destination *	Destination1_loin;
	XDY_Destination *	Destination2_proche;
	XDY_Destination *	Destination2_loin;
	XZADT_Liste_ZDP *	Graphe_proche1;
	XZADT_Liste_ZDP *	Graphe_loin1;
	XZADT_Liste_ZDP *	Graphe_proche2;
	XZADT_Liste_ZDP *	Graphe_loin2;
	void *			suivant;
}
XZADT_Liste_PMVA_TDP;



/*
** Declaration de variables
** ------------------------
*/



/*
** Declaration de variables internes
** ---------------------------------
*/

static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : xzad.c" ;

static	XDY_District		vm_Site = 0;


/* Table complete des zones lue en base */
static	XDY_ZDP *		tm_ZDP = NULL;
static	XDY_Entier		vm_Nb_ZDP = 0;

/* Table supplementaire d'index par NUMERO des ZDP pour acces direct */
static	int *			tm_i_zdp = NULL;
static	XDY_Entier		vm_Nb_Idx_Zdp = 0;



/* Liste complete des destinations Temps de Parcours */
XZADT_Liste_Destinations *	pm_ListeDest = NULL;



/* Liste des panneaux capables d'afficher des temps de parcours */
XZADT_Liste_PMV_TDP *		pm_ListePMV  = NULL;
XZADT_Liste_PMVA_TDP *		pm_ListePMVA = NULL;



/* Reference au callback de traitement des messages de mesures TdP */
T_CB		vm_CB_XZAD_Mesure_tdp = NULL;
T_CB		vm_CB_surcharge = NULL;



/* Declaration de fonctions internes */
/* --------------------------------- */


/* Definition de fonctions externes */
/* -------------------------------- */



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonctions utilitaires de mise a jour et acces des valeurs des ZdP.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen XZAD_Validite_TDP ( XDY_Entier va_num_zdp )
{
	if ( (va_num_zdp >= 0)  &&  (va_num_zdp < vm_Nb_Idx_Zdp) ) {
		if ( tm_i_zdp[va_num_zdp] >= 0) {
			/* Le champ "Traitee" est utilise pour memoriser la validite */
			if (tm_ZDP[tm_i_zdp[va_num_zdp]].Traitee == XDC_VRAI) {
				return (XDC_VRAI);
			}
		}
	}

	return (XDC_FAUX);
}



static int XZAD_TDP_Valide ( XDY_Entier  va_num_zdp,
                             XDY_Octet   va_valide)
{
	if ( (va_num_zdp >= 0)  &&  (va_num_zdp < vm_Nb_Idx_Zdp) ) {
		if ( tm_i_zdp[va_num_zdp] >= 0) {
			/* Utiliser le champ "Traitee" pour memoriser la validite */
			tm_ZDP[tm_i_zdp[va_num_zdp]].Traitee = va_valide;
			return (XDC_OK);
		}
	}

	return (XDC_NOK);
}


static void XZAD_surcharge_cb (
	T_IPC_CONN                      va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	T_CB_ARG                        pa_Arg)
{
#ifdef _TIBCO_EMS
 XDY_Mot	vl_zone;
 XDY_Entier vl_surcharge;
#else
 T_INT2	vl_zone;
 T_INT4 vl_surcharge;
#endif
	int			i;

 if (!TipcMsgRead(pa_Data -> msg,
 		T_IPC_FT_INT2, &vl_zone,
		T_IPC_FT_INT4, &vl_surcharge,
		NULL))
  {
	XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de lire le message XDM_ETDP_SURCHARGE");
	return;
  }
  else
  {
  	XZST_03EcritureTrace(XZSTC_FONCTION," XZAD_surcharg_cb : zone %d surcharge %d",
		vl_zone, vl_surcharge);
  }
	/*B Rechercher la ZdP */
	for (i = 0;  i < vm_Nb_ZDP;  i = i + 1) {
		if (tm_ZDP[i].Numero == vl_zone) {
			break;
		}
	}
	if (i >= vm_Nb_ZDP) {
		XZST_03EcritureTrace (XZSTC_WARNING, "ZDP inconnue %d", vl_zone);
		return ;
	}
        tm_ZDP[i].TDP_Surcharge =vl_surcharge;

}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonction de traitement des messages de mesures TdP 6mn par zone.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void XZAD_Mesure_tdp_6mn_cb (
	T_IPC_CONN                      va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	T_CB_ARG                        pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*
*   Message RTworks
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*   Aucun
*
*
* CONDITION D'UTILISATION
*
*  Appel sur reception d'un message de mesures TdP 6mn pour une ZdP.
*
* FONCTION
*
*  Mise a jour de la valeur et de la validite de la mesure pour la zone ZdP.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_Mesure_tdp_6mn_cb" ;
#ifdef _TIBCO_EMS
	XDY_Horodate		vl_Horodate;
	XDY_Mot		vl_Num_Zone;
	XDY_Mot		vl_TDP6mn;
	XDY_Mot		vl_Validite6mn;
#else
	T_REAL8		vl_Horodate;
	T_INT2		vl_Num_Zone;
	T_INT2		vl_TDP6mn;
	T_INT2		vl_Validite6mn;
#endif

	/* Recuperation des parametres du message XDM_EMES_TDP_6MN_PAR_ZONE */
	if ( !TipcMsgRead (pa_Data -> msg,
	                      T_IPC_FT_REAL8,	&vl_Horodate,
	                      T_IPC_FT_INT2, 	&vl_Num_Zone,
	                      T_IPC_FT_INT2, 	&vl_TDP6mn,
	                      T_IPC_FT_INT2, 	&vl_Validite6mn,
	                      NULL) )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Impossible de lire le message de mesures TdP 6mn");
	}
	else
	{
		/* Mettre a jour la valeur de la mesure et sa validite */
		if ( (vl_Num_Zone >= 0)  &&  (vl_Num_Zone < vm_Nb_Idx_Zdp)  &&  (tm_i_zdp[vl_Num_Zone] >= 0) )
		{
			XZST_03EcritureTrace (XZSTC_MESSAGE, "Mesure ZdP %d : TdP=%d, Val=%d",
			                         (int) vl_Num_Zone, (int) vl_TDP6mn, (int) vl_Validite6mn);

			tm_ZDP[tm_i_zdp[vl_Num_Zone]].TDP_Courant = (XDY_TDP) vl_TDP6mn;
			XZAD_TDP_Valide ((XDY_Entier) vl_Num_Zone, (XDY_Octet) vl_Validite6mn);
		}
		else
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Mesure recue pour ZdP %d invalide");
		}
	}
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Calcul du Temps de Parcours d'un point donne par <Autoroute, Sens, PR>
*  jusqu'a une destination donnee.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD_Calcul_TDP ( XDY_Autoroute      va_autoroute,
                             XDY_PR             va_PR,
                             XDY_Sens           va_sens,
                             XDY_Destination   *pa_Destination,
                             XZADT_Liste_ZDP   *pa_Graphe,
                             XDY_TDP           *pa_TDP,
                             XDY_Booleen       *pa_Tdp_Valide)

/*
* ARGUMENTS EN ENTREE :
*
*   va_autoroute,
*   va_PR,
*   va_sens           : Definition du point de depart (loc. panneau) ;
*   pa_Destination    : Donnees decrivant la destination ;
*   pa_Graphe         : Liste de ZdP constituant le graphe de parcours.
*
* ARGUMENTS EN SORTIE :
*
*   *pa_TDP           : Temps de parcours calcule ;
*   *pa_Tdp_Valide    : Indicateur de validite.
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Parcours du graphe de zones ZdP et somme des temps elementaires.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_Calcul_TDP" ;

	XDY_TDP		vl_tdp_total = 0;
	XDY_TDP		vl_tdp_zone = 0;
	XDY_TDP		vl_tdp_calcul = 0;
	XDY_Mot		vl_Ordre_PR;
	double		vl_longueur_zone;
	double		vl_Coef;
	XDY_Booleen	vl_Origine_Sur_ZdP, vl_Dest_Sur_ZdP;

	XZADT_Liste_ZDP	*pl_Zone;
	XDY_ZDP		*pl_Zdp;
	XDY_Booleen	vl_arreter=XDC_FAUX;
	
	XDY_TDP       	vl_tdp_fcd=0;
	XDY_Octet    	vl_validite_fcd=0;
	XDY_Booleen	vl_validite_faible=1;	

	XDY_Booleen			vl_destination_hors_reseau;
  	XDY_Booleen			vl_FCD_calcul_hors_reseau=XDC_FAUX;
	XDY_PR        		vl_PR_d=-1;
	XDY_Autoroute 		vl_autoroute_d=0;
	XDY_Sens    		vl_sens_d=0;
	XDY_PR				va_pr_limite=0;
	XDY_Autoroute		va_autoroute_limite=0;
	XDY_Sens     		va_sens_limite=0;
	XDY_Sens			vl_TDP_terrain_valide=XDC_VRAI;

	/* Valeur par defaut, invalide */
	(*pa_TDP) = (XDY_TDP) (-1);
	(*pa_Tdp_Valide) = XDC_FAUX;


	/*A
	** Verifier les valeurs des parametres d'entree
	*/

	if ( (pa_Destination == NULL)  ||  (pa_Graphe == NULL) )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "%s <%d-%d-%d> : Destination ou graphe invalide",
		                                     version, (int) va_autoroute, (int) va_sens, (int) va_PR);
		return (XDC_NOK);
	}


	XZST_03EcritureTrace (XZSTC_FONCTION, "IN : %s de <%d-%d-%d> a destination %d", version,
	                                         (int) va_autoroute, (int) va_sens, (int) va_PR, pa_Destination->Numero);

	vl_destination_hors_reseau=XZAO29_calcul_zone_hors_reseau (pa_Destination->Autoroute,
                            		pa_Destination->PR,
                            		va_sens,
                            		pa_Destination->Sens,
									&va_pr_limite,
									&va_autoroute_limite,
									&va_sens_limite); //Test si destination hors rÃ©seau ESCOTA 1=(ORANGE/MONTPELLIER/MARSEILLE) ou (MONACO/VINTIMILLE)

	vl_FCD_calcul_hors_reseau=vl_destination_hors_reseau; //Pour ne pas envoyer le TDP FCD si vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA et que FCD < XDC_SEUIL_AFFICHAGE_TDP_FCD et terrain calcul complet mais résultat HS, alors que FCD est un calcul non complet


	/*Récupération du TDP grâce à l'appel à la procedure XZAO962*/

  	if (XZAO962_Calcul_TDP_FCD(  	va_autoroute,
                            		va_PR,
                            		va_sens,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_autoroute_limite : pa_Destination->Autoroute,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_pr_limite : pa_Destination->PR,//Si on est hors reseau ORANGE/MONTPELLIER/MARESEILLE/MONACO/VINTIMILLE On calcul jusqu'a  la limite du reseau, sinon les TDP_FCD sont fournis,
                            		vl_destination_hors_reseau == XDC_VRAI ? va_sens_limite : pa_Destination->Sens,
                            		0,
                            		0,
                            		&vl_tdp_fcd,
                            		&vl_validite_fcd,
                            		&vl_validite_faible)!= XDC_OK) {
        	XZST_03EcritureTrace(XZSTC_WARNING,"XZAD_Calcul_TDP : pb appel a XZAO962_Calcul_TDP_FCD");
        	return (XDC_NOK);
        }
							
  	/*Si vl_validite_fcd >= 60% et que toutes les données ont une validite > 20% (vl_validite_faible == 0) alors on utilise le temps de parcours de la procedure XZAO962,si on est pas sur une destination hors reseau*/
  	if((vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD) && (vl_validite_faible == 0) && vl_destination_hors_reseau == XDC_FAUX)		
	{
		(*pa_TDP) = (vl_tdp_fcd + 59) / 60 ;        
		(*pa_Tdp_Valide) = XDC_VRAI; 
		XZST_03EcritureTrace(XZSTC_INFO,"Du PR %d à la destination %d Tdp de %d", (int) va_PR, pa_Destination->Numero, *pa_TDP);
		return (XDC_OK);
	}  	
	else /*Si vl_validite_fcd < 60%*/
	{
		if (vl_destination_hors_reseau == XDC_VRAI &&  (vl_validite_fcd<=XDC_SEUIL_AFFICHAGE_TDP_FCD || vl_validite_faible != 0)) // Si on a le TDP FCD non valide et qu'on est hors reseau, alors on calcul tout le trajet avec les donnees terrains
		{
			vl_destination_hors_reseau=XDC_FAUX;
		}

		/*A Pour chaque zone ZdP du graphe, */
		for (pl_Zone = pa_Graphe;  pl_Zone != NULL;  pl_Zone = pl_Zone->suivant)
		{
			pl_Zdp = pl_Zone->p_zdp;

			/*A si la zone est en section courante */
			if (pl_Zdp->Type_Zone == XDC_ZDP_TDP_SECTION)
			{
				/*A alors la valeur TdP est la valeur courante */
				if (pl_Zdp->TDP_Surcharge > 0) {
			  	XZST_03EcritureTrace (XZSTC_INFO,"surcharge > 0");
			  	vl_tdp_zone = pl_Zdp->TDP_Surcharge;
				}
				else if (pl_Zdp->TDP_Surcharge==-1) {
			  	vl_tdp_zone=0;
			  	XZST_03EcritureTrace (XZSTC_INFO,"surcharge  = -1");
				}
				else {
			  	vl_tdp_zone = pl_Zdp->TDP_Courant;
				XZST_03EcritureTrace (XZSTC_INFO,"surcharge = 0 = tdp courant");
				}
/*** si on est sur la zone Route du Parc -> Entree Sophia (172) et pas de données alors on continue pour faire le calcul avec la zone ROute du Parc -> Antibes Est (173) **/
				if (pl_Zdp->Numero==172) {
			    		if (vl_tdp_zone>0) {
						vl_arreter=XDC_VRAI;
					}
					else {
						vl_arreter=XDC_FAUX;
						vl_tdp_zone=0;
					}
				}
/*** si on est sur la zone Route du Parc -> Antibes Est (173) et qu'on avait des données sur Route du Parc -> Entree Sophia (172) (vl_arreter==XDC_VRAI)  alors on ne tient pas compte de cette zone pour le calcul (tdp=1)  **/
				if ((pl_Zdp->Numero==173) && (vl_arreter==XDC_VRAI))
					vl_tdp_zone=1;
			
			}
			else
			{
				/*A sinon prendre la valeur TdP habituelle */
				if (pl_Zdp->TDP_Surcharge > 0)
			  		vl_tdp_zone = pl_Zdp->TDP_Surcharge;
				else if (pl_Zdp->TDP_Surcharge==-1)
			  		vl_tdp_zone=0;
				else
			  		vl_tdp_zone = pl_Zdp->TDP_Normal;
				XZST_03EcritureTrace (XZSTC_INFO,"%s : zone virtuelle de %d a %d;%d tdpn=%d,zdp type=%d => temps=%d.",version,pl_Zdp->PR_Debut, pl_Zdp->PR_Fin,pl_Zdp->Sens,pl_Zdp->TDP_Normal,pl_Zdp->Type_Zone,vl_tdp_zone);
			}
			vl_Coef = (double) 1.0;

			if (pa_Destination->Numero==85)
				XZST_03EcritureTrace (XZSTC_INFO,"%s : pl_Zdp->Numero=%d;temps=%d %d %d",version,pl_Zdp->Numero,vl_tdp_zone,vl_arreter, XZAD_Validite_TDP (pl_Zdp->Numero));

			/*A s'il n'y a pas de donnee TdP pour cette zone, si vl_validite_fcd>=40% alors on utilise le tdp de XZAO962 sinon abandonner */ /** sauf si on est sur la zone 172 **/
			if ((pl_Zdp->Numero!=172)) 
			{
		  		if ( (vl_tdp_zone <= 0)  ||  ((XZAD_Validite_TDP (pl_Zdp->Numero) == XDC_FAUX && (pl_Zdp->Numero!=173)) && (pl_Zdp->Type_Zone == XDC_ZDP_TDP_SECTION) ) )
		  		{
			  		if (vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA && (vl_FCD_calcul_hors_reseau==XDC_FAUX) ) //Si le TDP FCD est valide et qu'on est pas sur un destination hors reseau
            		{
						(*pa_TDP) = (vl_tdp_fcd + 59) / 60;
						(*pa_Tdp_Valide) = XDC_VRAI;	
						XZST_03EcritureTrace(XZSTC_INFO,"Du PR %d à la destination %d Tdp de %d", (int) va_PR, pa_Destination->Numero, *pa_TDP);				
                				return (XDC_OK);
            		}
					else
					{
						if ((pl_Zdp->Code_Zone < 200 || pl_Zdp->Code_Zone > 300) && vl_destination_hors_reseau == XDC_VRAI)//Si le code zone n'est pas virtuelle et qu'on est en calcul hors reseau, on ne sors pas car, on ne va de toute façon pas prendre la valeur terrain, mais celle FCD
						{
							XZST_03EcritureTrace (XZSTC_WARNING, "Pas de donnee de %d a %d;%d tdpn=%d,zdp type=%d no=%d, mais pas darret car non pris en compte pour le calcul hybride", pl_Zdp->PR_Debut, pl_Zdp->PR_Fin,pl_Zdp->Sens,pl_Zdp->TDP_Normal,pl_Zdp->Type_Zone,pl_Zdp->Numero);
						}
						else
						{
							XZST_03EcritureTrace (XZSTC_WARNING, "%s : Pas de donnee de %d a %d;%d tdpn=%d,zdp type=%d no=%d, code zone=%d",version, pl_Zdp->PR_Debut, pl_Zdp->PR_Fin,pl_Zdp->Sens,pl_Zdp->TDP_Normal,pl_Zdp->Type_Zone,pl_Zdp->Numero,pl_Zdp->Code_Zone);
							return (XDC_OK);	
						}
						
		  			}
				}
			}

			/*A Tester si l'origine ou la destination sont situees dans la zone ZdP */
			vl_Origine_Sur_ZdP = XDC_FAUX;
			if ( (pl_Zdp->Autoroute == va_autoroute)  && (pl_Zdp->PR_Debut < va_PR)  &&  (va_PR < pl_Zdp->PR_Fin) )
			{
				vl_Origine_Sur_ZdP = XDC_VRAI;
			}

			vl_Dest_Sur_ZdP = XDC_FAUX;
			if ( (pl_Zdp->Autoroute == pa_Destination->Autoroute)  && (pl_Zdp->PR_Debut < pa_Destination->PR)  &&  (pa_Destination->PR < pl_Zdp->PR_Fin) )
			{
				vl_Dest_Sur_ZdP = XDC_VRAI;
			}


			if ( (vl_Origine_Sur_ZdP == XDC_VRAI)  ||  (vl_Dest_Sur_ZdP == XDC_VRAI) )
			{
				vl_Ordre_PR = XZAO27_Ordre_PR (pl_Zdp->Autoroute);
				vl_longueur_zone = (double) (pl_Zdp->PR_Fin - pl_Zdp->PR_Debut);
			}


			/*A si l'origine et la destination sont toutes deux dans la zone ZdP alors */
			if ( (vl_Origine_Sur_ZdP == XDC_VRAI)  &&  (vl_Dest_Sur_ZdP == XDC_VRAI) )
			{
				/*A le temps est proportionnel a la zone entre origine et destination */
				vl_Coef = (double) abs (pa_Destination->PR - va_PR) / vl_longueur_zone;
			}
			else if (vl_Origine_Sur_ZdP == XDC_VRAI)
			{
				/*A sinon si l'origine est comprise dans la zone ZdP alors */
				/*A considerer le temps proportionnel a la fin de zone parcourue */
				if ( ( (pl_Zdp->Sens == XDC_SENS_SUD)  &&  (vl_Ordre_PR == XDC_PR_NOR) )  ||  
				( (pl_Zdp->Sens == XDC_SENS_NORD)  &&  (vl_Ordre_PR == XDC_PR_INV) ) )
				{
					vl_Coef = (double) (pl_Zdp->PR_Fin - va_PR) / vl_longueur_zone;
				}
				else
				{
					vl_Coef = (double) (va_PR - pl_Zdp->PR_Debut) / vl_longueur_zone;
				}
			}
			else if (vl_Dest_Sur_ZdP == XDC_VRAI)
			{
				/*A sinon si la destination est comprise dans la zone ZdP alors */
				/*A considerer aussi le temps proportionnel au debut de zone parcouru */
				if ( ( (pl_Zdp->Sens == XDC_SENS_SUD)  &&  (vl_Ordre_PR == XDC_PR_NOR) )  ||
			     	( (pl_Zdp->Sens == XDC_SENS_NORD)  &&  (vl_Ordre_PR == XDC_PR_INV) ) )
				{
					vl_Coef = (double) (pa_Destination->PR - pl_Zdp->PR_Debut) / vl_longueur_zone;
				}
				else
				{
					vl_Coef = (double) (pl_Zdp->PR_Fin - pa_Destination->PR) / vl_longueur_zone;
				}
			}

			if (vl_destination_hors_reseau == XDC_FAUX) //Si on est pas sur un destination hors reseau, on prend tout
				vl_TDP_terrain_valide = XDC_VRAI;
			else if (pl_Zdp->Code_Zone >= 200 && pl_Zdp->Code_Zone <= 300) // Si on est sur une destination hors reseau, on prend que les zones virtuelle entre 200 et 300, et on fait de l'hybride FCD + terrain
				vl_TDP_terrain_valide = XDC_VRAI;
			else
				vl_TDP_terrain_valide = XDC_FAUX;

			/*A ajouter le temps de parcours au temps total */
			if (vl_TDP_terrain_valide)
				vl_tdp_total = vl_tdp_total + (XDY_TDP) ((double) vl_tdp_zone * vl_Coef);

			XZST_03EcritureTrace (XZSTC_INFO, "Numero: %d, Code_Zone:%d",pl_Zdp->Numero,pl_Zdp->Code_Zone);
			XZST_03EcritureTrace (XZSTC_INFO, "ZDP de %d a %d : TDP = %d, Coef=%lf, Ajoute au total: %d, Total %d ",pl_Zdp->PR_Debut, pl_Zdp->PR_Fin, vl_tdp_zone, vl_Coef, vl_TDP_terrain_valide, vl_tdp_total);
		}

	if(vl_destination_hors_reseau==XDC_VRAI && vl_validite_fcd>=XDC_SEUIL_AFFICHAGE_TDP_FCD && vl_validite_faible == 0)
	{
		XZST_03EcritureTrace (XZSTC_INFO, "TDP HYBRIDE TDP FCD : %d, ajoute au tdp terrain :%d",vl_tdp_fcd,vl_tdp_total);
		vl_tdp_total = vl_tdp_total + vl_tdp_fcd; // Si on est sur une destination hors reseau, que le tdp FCD est valide, alors on le rajoute au TDP total
		XZST_03EcritureTrace (XZSTC_INFO, "TDP HYBRIDE arr. min= %d",(vl_tdp_total + 59) / 60);
	}

	/*A Convertir la valeur TdP en minutes, arrondies a la minute superieure */
	(*pa_TDP) = (vl_tdp_total + 59) / 60;
	(*pa_Tdp_Valide) = XDC_VRAI;	
	return (XDC_OK);
	}
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Creation de la liste des zones Temps de Parcours.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_ajouter_ZDP ( int va_nb_zdp, int va_i_zdp,
                              XDY_ZDP va_ZDP,
                              char *va_nom_eqt,
                              XDY_Entier va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   va_nb_zdp         : Nombre total de ZdP ;
*   va_i_zdp          : Indice de cette ZdP ;
*   va_ZDP            : Donnees ZDP ;
*   va_nom_eqt        : Nom de l'equipement ZDP.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des zones
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_ajouter_ZDP" ;


	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: XZAD_ajouter_ZDP %d", va_i_zdp);

	/*A Au premier appel, allouer la memoire pour la table complete des ZdP */
	if (va_i_zdp == 0)
	{
		tm_ZDP = (XDY_ZDP *) calloc ( (size_t) va_nb_zdp, sizeof (XDY_ZDP) );
		if ( tm_ZDP == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "OUT: XZAD_ajouter_ZDP Allocation memoire impossible");
		}
	}

	/*A Si la table n'a pu etre creee, aucune ZdP n'est prise en compte */
	if ( tm_ZDP == NULL )
	{
		return (XDC_NOK);
	}


	/*A Ajouter la ZdP dans la table et mettre a jour ses donnees */
	tm_ZDP[va_i_zdp] = va_ZDP;
	XDY_Supprimer_Espaces_De_Fin (tm_ZDP[va_i_zdp].Libelle);
	vm_Nb_ZDP = vm_Nb_ZDP + 1;

	tm_ZDP[va_i_zdp].Franchissement_Seuil = XDC_SEUIL_TDP_ABSENT;
	tm_ZDP[va_i_zdp].Seuil_Haut = (tm_ZDP[va_i_zdp].TDP_Normal * tm_ZDP[va_i_zdp].Seuil_Haut) / 100;
	tm_ZDP[va_i_zdp].Seuil_Bas = (tm_ZDP[va_i_zdp].TDP_Normal * tm_ZDP[va_i_zdp].Seuil_Bas) / 100;
         tm_ZDP[va_i_zdp].TDP_Surcharge = 0;

	XZST_03EcritureTrace ( XZSTC_INFO,
	                          "ZDP %d, code %d, type %d <%s>, Site %d, Loc %s-%d-%d a %d, TDP Normal/Max/Courant %d/%d/%d, Seuil Bas/Haut %d/%d",
	                          tm_ZDP[va_i_zdp].Numero,
	                          (int) tm_ZDP[va_i_zdp].Code_Zone,
	                          (int) tm_ZDP[va_i_zdp].Type_Zone,
	                          tm_ZDP[va_i_zdp].Libelle,
	                          (int) tm_ZDP[va_i_zdp].Site,
	                          tm_ZDP[va_i_zdp].NomAutoroute,
	                          (int) tm_ZDP[va_i_zdp].Sens,
	                          tm_ZDP[va_i_zdp].PR_Debut,
	                          tm_ZDP[va_i_zdp].PR_Fin,
	                          (int) tm_ZDP[va_i_zdp].TDP_Normal,
	                          (int) tm_ZDP[va_i_zdp].TDP_Max,
	                          (int) tm_ZDP[va_i_zdp].TDP_Courant,
	                          tm_ZDP[va_i_zdp].Seuil_Bas,
	                          tm_ZDP[va_i_zdp].Seuil_Haut);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Creation de la liste des destinations Temps de Parcours
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_ajouter_Destination ( XDY_Destination va_Destination,
                                      XDY_Entier va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   va_Destination    : Donnees destination.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_ajouter_Destination" ;

	XZADT_Liste_Destinations *	pl_NewDest = NULL;

	XZST_03EcritureTrace ( XZSTC_INFO, "Destination %d, Msg TFM <%s>, Msg PMV <%s>, Loc %d-%d-%d, Type_Loc %d",
	                                      va_Destination.Numero,
	                                      va_Destination.Libelle_TFM,
	                                      va_Destination.Libelle_PMV,
	                                      (int) va_Destination.Autoroute,
	                                      (int) va_Destination.Sens,
	                                      va_Destination.PR,
	                                      (int) va_Destination.Type_Localisation);

	/*A Allocation memoire pour la nouvelle structure */
	pl_NewDest = (XZADT_Liste_Destinations *) calloc ( (size_t) 1, sizeof (XZADT_Liste_Destinations) );
	if ( pl_NewDest == NULL )
	{
		XZST_03EcritureTrace (XZSTC_FATAL, " OUT: XZAD_ajouter_Destination Allocation memoire impossible");
		return (XDC_NOK);
	}

	/*A Mise a jour de la structure a inserer */
	pl_NewDest->destination = va_Destination;

	/*A Ajout de la structure dans la liste */
	pl_NewDest->suivant = pm_ListeDest;
	pm_ListeDest        = pl_NewDest;

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Creation de la liste des PMV
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_ajouter_PMV ( XZAOT_PMV_TDP	va_PMV,
                              XDY_Entier	va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   va_PMV            : Donnees PMV.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des PMV
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_ajouter_PMV" ;

	XZADT_Liste_PMV_TDP *	pl_NewPMV = NULL;
	XDY_NomAuto		vl_NomAutoroute = "";

	if (va_PMV.SiteGestion == vm_Site)
	{
		/*A Allouer la memoire pour le nouvel equipement */
		pl_NewPMV = (XZADT_Liste_PMV_TDP *) calloc ( (size_t) 1, sizeof (XZADT_Liste_PMV_TDP) );
		if ( pl_NewPMV == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, " OUT: XZAD_ajouter_PMV Allocation memoire impossible");
			return (XDC_NOK);
		}

		XZAO25_Nom_Autoroute (va_PMV.Autoroute, vl_NomAutoroute);
		XZST_03EcritureTrace (XZSTC_INFO, "Ajout PMV %d, Loc %s-%d-%d, LT %s",
		                         va_PMV.Numero, vl_NomAutoroute, va_PMV.Sens, va_PMV.PR, va_PMV.NomMachine);

		/*A Mettre a jour les donnees du PMV */
		pl_NewPMV->PMV = va_PMV;
		sprintf ( pl_NewPMV->Nom, " PMV %s-%d-%d", vl_NomAutoroute, va_PMV.Sens, va_PMV.PR );

		pl_NewPMV->Destination_Proche = NULL;
		pl_NewPMV->Destination_Loin = NULL;
		pl_NewPMV->Destination_Noeud = NULL;

		pl_NewPMV->Graphe_Dest_Proche = NULL;
		pl_NewPMV->Graphe_Dest_Loin = NULL;
		pl_NewPMV->Graphe_Dest_Noeud = NULL;

		/*A Inserer le PMV dans la liste */
		pl_NewPMV->suivant = pm_ListePMV;
		pm_ListePMV        = pl_NewPMV;
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Creation de la liste des PMVA
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static int XZAD_ajouter_PMVA ( XZAOT_PMA_TDP	va_PMVA,
                               XDY_Entier	va_NumAppel )
/*
* ARGUMENTS EN ENTREE :
*
*   va_PMVA           : Donnees PMVA.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des PMV
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_ajouter_PMVA" ;

	XZADT_Liste_PMVA_TDP *	pl_NewPMVA = NULL;
	XDY_NomAuto		vl_NomAutoroute = "";

	if (va_PMVA.SiteGestion == vm_Site)
	{
		/*A Allouer la memoire pour le nouvel equipement */
		pl_NewPMVA = (XZADT_Liste_PMVA_TDP *) calloc ( (size_t) 1, sizeof (XZADT_Liste_PMVA_TDP) );
		if ( pl_NewPMVA == NULL)
		{
			XZST_03EcritureTrace (XZSTC_FATAL, " OUT: XZAD_ajouter_PMVA Allocation memoire impossible");
			return (XDC_NOK);
		}

		XZAO25_Nom_Autoroute (va_PMVA.Autoroute, vl_NomAutoroute);
		XZST_03EcritureTrace (XZSTC_INFO, "Ajout PMVA %d, Loc %s-%d-%d, LT %s",
		                         va_PMVA.Numero, vl_NomAutoroute, va_PMVA.Sens, va_PMVA.PR, va_PMVA.NomMachine);

		/*A Mettre a jour les donnees du PMVA */
		pl_NewPMVA->PMVA = va_PMVA;
		sprintf ( pl_NewPMVA->Nom, " PMVA %s-%d-%d", vl_NomAutoroute, va_PMVA.Sens, va_PMVA.PR );

		pl_NewPMVA->Destination1_proche = NULL;
		pl_NewPMVA->Destination1_loin = NULL;
		pl_NewPMVA->Destination2_proche = NULL;
		pl_NewPMVA->Destination2_loin = NULL;

		pl_NewPMVA->Graphe_proche1 = NULL;
		pl_NewPMVA->Graphe_loin1 = NULL;
		pl_NewPMVA->Graphe_proche2 = NULL;
		pl_NewPMVA->Graphe_loin2 = NULL;

		/*A Inserer le PMVA dans la liste */
		pl_NewPMVA->suivant = pm_ListePMVA;
		pm_ListePMVA        = pl_NewPMVA;
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Creation d'une des listes de destinations pour les PMV.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_ajouter_Dest_pmv ( XDY_Eqt va_PMV, XDY_Mot va_Destination,
                                      XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
                                      int va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   va_PMV            : Numero de PMV ;
*   va_Destination    : Numero de destination ;
*   va_TypeDestination: Type de destination ;
*   va_zdp            : Code de la ZDP maillon du graphe.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
	XZADT_Liste_PMV_TDP		*pl_PMV = NULL;
	XZADT_Liste_Destinations	*pl_Lst_Dest = NULL;
	XDY_Destination			*pl_Dest = NULL;
	XDY_Destination			*pl_ExDest = NULL;	/* Ancienne valeur du meme type */
	XZADT_Liste_ZDP			*pl_NewZDP;

	int			i;

	/*B Rechercher la ZdP */
	for (i = 0;  i < vm_Nb_ZDP;  i = i + 1) {
		if (tm_ZDP[i].Code_Zone == va_zdp) {
			break;
		}
	}
	if (i >= vm_Nb_ZDP) {
		XZST_03EcritureTrace (XZSTC_WARNING, "ZDP inconnue %d, graphe PMV %d", va_zdp, va_PMV);
		return (XDC_OK);
	}


	/*B Rechercher la destination */
	for ( pl_Lst_Dest = pm_ListeDest;  pl_Lst_Dest != NULL;  pl_Lst_Dest = pl_Lst_Dest->suivant )
	{
		if ( pl_Lst_Dest->destination.Numero == va_Destination )
		{
			pl_Dest = &(pl_Lst_Dest->destination);
			break;
		}
	}
	if (pl_Lst_Dest == NULL) {
		XZST_03EcritureTrace (XZSTC_WARNING, "Destination inconnue %d, graphe PMV %d", va_Destination, va_PMV);
		return (XDC_OK);
	}


	/*B Rechercher le PMV */
	for ( pl_PMV = pm_ListePMV;  pl_PMV != NULL;  pl_PMV = pl_PMV->suivant )
	{
		if ( pl_PMV->PMV.Numero == va_PMV )
		{
			break;
		}
	}


	/*A Si le PMV est trouve alors */
	if ( pl_PMV != NULL )
	{
		XZST_03EcritureTrace (XZSTC_INFO, "XZAD_ajouter_Dest_pmv : pmv=%d, dest=%d, type=%d, code zdp=%d",
		                                     va_PMV, va_Destination, va_TypeDestination, va_zdp);

		/*A Creer un nouveau maillon de liste pointant sur l'objet ZdP */
		pl_NewZDP = (XZADT_Liste_ZDP *) calloc ( (size_t) 1, sizeof (XZADT_Liste_ZDP) );
		if ( pl_NewZDP == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "OUT: XZAD_ajouter_Dest_pmv Allocation memoire impossible");
			return (XDC_NOK);
		}
		pl_NewZDP->p_zdp = &(tm_ZDP[i]);
		pl_NewZDP->suivant = NULL;


		/*A et inserer ce maillon dans le graphe de la destination du PMV de type adequat */
		switch (va_TypeDestination) {
			case XDC_DESTINATION_PROCHE :
				pl_ExDest = pl_PMV->Destination_Proche;
				pl_PMV->Destination_Proche = pl_Dest;

				pl_NewZDP->suivant = pl_PMV->Graphe_Dest_Proche;
				pl_PMV->Graphe_Dest_Proche = pl_NewZDP;
				break;

			case XDC_DESTINATION_LOINTAINE :
				pl_ExDest = pl_PMV->Destination_Loin;
				pl_PMV->Destination_Loin = pl_Dest;

				pl_NewZDP->suivant = pl_PMV->Graphe_Dest_Loin;
				pl_PMV->Graphe_Dest_Loin = pl_NewZDP;
				break;

			case XDC_DESTINATION_NOEUD :
				pl_ExDest = pl_PMV->Destination_Noeud;
				pl_PMV->Destination_Noeud = pl_Dest;

				pl_NewZDP->suivant = pl_PMV->Graphe_Dest_Noeud;
				pl_PMV->Graphe_Dest_Noeud = pl_NewZDP;
				break;

			default :
				XZST_03EcritureTrace (XZSTC_WARNING, "Type destination inconnu %d", va_TypeDestination);
		}

		/*B Verifier la coherence du graphe de ce type */
		if (pl_ExDest != NULL)
		{
			if (pl_ExDest->Numero != pl_Dest->Numero)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Conflit de destinations de type %d, PMV %d",
				                         va_TypeDestination, va_PMV);
			}
		}
		return (XDC_OK);
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Creation d'une des listes de destinations pour les PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_ajouter_Dest_pmva ( XDY_Eqt va_PMVA, XDY_Mot va_Destination,
                                       XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
                                       int va_NumAppel )
/*
* ARGUMENTS EN ENTREE :
*
*   va_PMVA           : Numero de PMVA ;
*   va_Destination    : Numero de destination ;
*   va_TypeDestination: Type de destination ;
*   va_zdp            : Code de la ZDP maillon du graphe.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
	XZADT_Liste_PMVA_TDP		*pl_PMVA = NULL;
	XZADT_Liste_Destinations	*pl_Lst_Dest = NULL;
	XDY_Destination			*pl_Dest = NULL;
	XDY_Destination			*pl_ExDest = NULL;	/* Ancienne valeur du meme type */
	XZADT_Liste_ZDP			*pl_NewZDP;
	int				i;

	/*B Rechercher la ZdP */
	for (i = 0;  i < vm_Nb_ZDP;  i = i + 1) {
		if (tm_ZDP[i].Code_Zone == va_zdp) {
			break;
		}
	}
	if (i >= vm_Nb_ZDP) {
		XZST_03EcritureTrace (XZSTC_WARNING, "ZDP non trouvee %d", va_zdp);
		return (XDC_OK);
	}


	/*B Rechercher la destination */
	for ( pl_Lst_Dest = pm_ListeDest;  pl_Lst_Dest != NULL;  pl_Lst_Dest = pl_Lst_Dest->suivant )
	{
		if ( pl_Lst_Dest->destination.Numero == va_Destination )
		{
			pl_Dest = &(pl_Lst_Dest->destination);
			break;
		}
	}
	if (pl_Lst_Dest == NULL) {
		XZST_03EcritureTrace (XZSTC_WARNING, "Destination inconnue %d, graphe PMVA %d", va_Destination, va_PMVA);
		return (XDC_OK);
	}


	/*B Rechercher le PMVA */
	for ( pl_PMVA = pm_ListePMVA; pl_PMVA != NULL; pl_PMVA = pl_PMVA->suivant )
	{
		if ( pl_PMVA->PMVA.Numero == va_PMVA )
		{
			break;
		}
	}


	/*A Si le PMVA est trouve alors */
	if ( pl_PMVA != NULL )
	{
		XZST_03EcritureTrace (XZSTC_INFO, "XZAD_ajouter_Dest_pmva : pmva=%d, dest=%d, type=%d, code zdp=%d",
		                                     va_PMVA, va_Destination, va_TypeDestination, va_zdp);

		/*A Creer un nouveau maillon de liste pointant sur l'objet ZdP */
		pl_NewZDP = (XZADT_Liste_ZDP *) calloc ( (size_t) 1, sizeof (XZADT_Liste_ZDP) );
		if ( pl_NewZDP == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "OUT: XZAD_ajouter_Dest_pmva Allocation memoire impossible");
			return (XDC_NOK);
		}
		pl_NewZDP->p_zdp = &(tm_ZDP[i]);
		pl_NewZDP->suivant = NULL;


		/*A et inserer ce maillon dans le graphe de la destination du PMVA de type adequat */
		switch (va_TypeDestination) {
			case XDC_DEST_UN_PROCHE :
				pl_ExDest = pl_PMVA->Destination1_proche;
				pl_PMVA->Destination1_proche = pl_Dest;

				pl_NewZDP->suivant = pl_PMVA->Graphe_proche1;
				pl_PMVA->Graphe_proche1 = pl_NewZDP;
				break;

			case XDC_DEST_UN_LOIN :
				pl_ExDest = pl_PMVA->Destination1_loin;
				pl_PMVA->Destination1_loin = pl_Dest;

				pl_NewZDP->suivant = pl_PMVA->Graphe_loin1;
				pl_PMVA->Graphe_loin1 = pl_NewZDP;
				break;

			case XDC_DEST_DEUX_PROCHE :
				pl_ExDest = pl_PMVA->Destination2_proche;
				pl_PMVA->Destination2_proche = pl_Dest;

				pl_NewZDP->suivant = pl_PMVA->Graphe_proche2;
				pl_PMVA->Graphe_proche2 = pl_NewZDP;
				break;

			case XDC_DEST_DEUX_LOIN :
				pl_ExDest = pl_PMVA->Destination2_loin;
				pl_PMVA->Destination2_loin = pl_Dest;

				pl_NewZDP->suivant = pl_PMVA->Graphe_loin2;
				pl_PMVA->Graphe_loin2 = pl_NewZDP;
				break;

			default :
				XZST_03EcritureTrace (XZSTC_WARNING, "Type destination inconnu %d", va_TypeDestination);
		}

		/*B Verifier la coherence du graphe de ce type */
		if (pl_ExDest != NULL)
		{
			if (pl_ExDest->Numero != pl_Dest->Numero)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Conflit de destinations de type %d, PMVA %d",
				                         va_TypeDestination, va_PMVA);
			}
		}
		return (XDC_OK);
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Lit en base les valeurs actuelles de Temps de Parcours pour les zones utilisees.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_Lire_Valeurs_TDP ()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Lit en base de donnees la derniere valeur de mesure de chaque zone utilisee.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_Lire_Valeurs_TDP" ;

	XDY_ZDP			*pl_Zdp;
	XDY_TDP			vl_Temps_Courant;

	int           		vl_jour;
	XDY_Horodate  		vl_horodate;
	int			vl_cr = XDC_OK;
	int			i;


	/*A
	** Lire en base de donnees les valeurs de TdP courantes pour chaque zone utilisee
	*/

	/*B Lire l'horodate systeme courante */
	XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_jour, &vl_horodate);

	/* Pour chaque numero de zone ZdP */
	for (i = 0;  i < vm_Nb_Idx_Zdp;  i = i + 1)
	{
		/* s'il est le numero d'une zone utilisee alors */
		if (tm_i_zdp[i] >= 0)
		{
			pl_Zdp = &(tm_ZDP[tm_i_zdp[i]]);

			/*A Rechercher la valeur de TdP courante pour cette zone */
			vl_cr = XZAD05_Dernier_TDP ((XDY_Eqt) pl_Zdp->Numero, vl_horodate, &vl_Temps_Courant);
			if ((vl_cr == XDC_OK) && (vl_Temps_Courant>0))
			{
				pl_Zdp->TDP_Courant = vl_Temps_Courant;
				XZAD_TDP_Valide (pl_Zdp->Numero, (XDY_Octet) XDC_VRAI);
			}
			else
			{
				pl_Zdp->TDP_Courant = 0;
				XZAD_TDP_Valide (pl_Zdp->Numero, (XDY_Octet) XDC_FAUX);
			}
		}
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Enregistre (indexe) chacune des zones du graphe.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void XZAD_Activer_Graphe (XZADT_Liste_ZDP *pa_Graphe)
{
	XDY_ZDP	*pl_zdp;

	while (pa_Graphe != NULL)
	{
		pl_zdp = pa_Graphe->p_zdp;
		tm_i_zdp[(int) pl_zdp->Numero] = (pl_zdp - &(tm_ZDP[0]));

		pa_Graphe = pa_Graphe->suivant;
	}
}



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Recense toutes les zones Temps de Parcours utilisees et constitue leur table indexee.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAD_Indexer_ZDP ()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Alloue une liste d'index des ZdP par numero, invalides par defaut ;
*   pour chaque panneau concerne par l'affichage de Temps de parcours,
*   active chaque graphe declare (initialise les index des zones utilisees).
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD_Indexer_ZDP" ;

	XDY_Entier	vl_numero_max = 0;

	XZADT_Liste_PMV_TDP	*pl_PMV = NULL;
	XZADT_Liste_PMVA_TDP	*pl_PMVA = NULL;

	int		vl_cr = XDC_OK;
	int		i;


	/*A
	** Determiner le numero maximal des ZdP
	*/

	for (i = 0;  i < vm_Nb_ZDP;  i = i + 1)
	{
		if (tm_ZDP[i].Numero > vl_numero_max)
		{
			vl_numero_max = tm_ZDP[i].Numero;
		}
	}


	/*A
	** Allouer une table pour les index par numero (a partir de 1)
	*/

	tm_i_zdp = (int *) calloc ( (size_t) (vl_numero_max + 1), sizeof (int) );
	if ( tm_i_zdp == NULL )
	{
		XZST_03EcritureTrace (XZSTC_FATAL, "OUT %s : Allocation memoire impossible", version);
		return (XDC_NOK);
	}
	vm_Nb_Idx_Zdp = vl_numero_max + 1;


	/*A
	** Initialiser les index des zones utilisees pour les destinations Temps de Parcours
	*/

	for (i = 0;  i < vm_Nb_Idx_Zdp;  i = i + 1)
	{
		tm_i_zdp[i] = (-1);
	}

	for (pl_PMV = pm_ListePMV;  pl_PMV != NULL;  pl_PMV = pl_PMV->suivant)
	{
		XZAD_Activer_Graphe (pl_PMV->Graphe_Dest_Proche);
		XZAD_Activer_Graphe (pl_PMV->Graphe_Dest_Loin);
		XZAD_Activer_Graphe (pl_PMV->Graphe_Dest_Noeud);
	}

	for (pl_PMVA = pm_ListePMVA;  pl_PMVA != NULL;  pl_PMVA = pl_PMVA->suivant)
	{
		XZAD_Activer_Graphe (pl_PMVA->Graphe_proche1);
		XZAD_Activer_Graphe (pl_PMVA->Graphe_proche2);
		XZAD_Activer_Graphe (pl_PMVA->Graphe_loin1);
		XZAD_Activer_Graphe (pl_PMVA->Graphe_loin2);
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonctions utilitaires.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

/*********************
static void Dump_ZDP ()
{
	int	i;

	for (i = 0;  i < vm_Nb_Idx_Zdp;  i++)
	{
		if (tm_i_zdp[i] >= 0)
		{
			XZST_03EcritureTrace (XZSTC_DEBUG1, "ZDP %d (code %d), TdP=%d",
			                         i, tm_ZDP[tm_i_zdp[i]].Code_Zone, tm_ZDP[tm_i_zdp[i]].TDP_Courant);
		}
	}
}
*********************/




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Initialisation de la reception et du traitement
*  des messages de mesures Temps de Parcours.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD_Abonnement_Mesures_TDP ()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Armement de la fonction "callback" de traitement des messages de mesures TdP recus ;
*   Abonnement au Datagroup de reception des messages de mesures TdP
*   pour chaque zone ZdP utilisee.
*
-----------------------------------------------------------------------------------------*/
{
	XDY_Datagroup	pl_NomDG;
	T_BOOL		vl_EtatDg = FALSE;

	int		vl_cr = XDC_NOK;
	int		i;


	/*A
	** Armer la Callback sur reception des messages de mesures TdP zones
	*/

	if (vm_CB_XZAD_Mesure_tdp == NULL)
	{
		vm_CB_XZAD_Mesure_tdp = TipcSrvProcessCbCreate (XDM_IdentMsg (XDM_EMES_TDP_6MN_PAR_ZONE),
		                                                XZAD_Mesure_tdp_6mn_cb,
		                                                (T_CB_ARG) NULL);
		if (vm_CB_XZAD_Mesure_tdp == NULL)
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "Echec armement Callback messages de mesures TdP");
			return (XDC_NOK);
		}
	}

	if (vm_CB_surcharge == NULL)
	{
		XDG_JoindreDG(XDG_ETDP_SURCHARGE);
		vm_CB_surcharge = TipcSrvProcessCbCreate (XDM_IdentMsg (XDM_ETDP_SURCHARGE),
		                                                XZAD_surcharge_cb,
		                                                (T_CB_ARG) NULL);
		if (vm_CB_surcharge == NULL)
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "Echec armement Callback messages surcharge");
			return (XDC_NOK);
		}
	}

	/*A
	** S'abonner aux messages de mesures TdP pour les zones utilisees
	*/

	/* Pour chaque numero de zone ZdP */
	for (i = 0;  i < vm_Nb_Idx_Zdp;  i = i + 1)
	{
		/* s'il est le numero d'une zone utilisee alors */
		if (tm_i_zdp[i] >= 0)
		{
			/* s'abonner au Datagroup des mesures TdP de cette zone */
			sprintf (pl_NomDG, "%s_%d", XDG_EMES_UGTP_6MN, tm_ZDP[tm_i_zdp[i]].Numero);

			TipcSrvSubjectGetSubscribe (pl_NomDG, &vl_EtatDg);
			if ( vl_EtatDg )
			{
				vl_cr = XDC_OK;
			}
			else if ( XDG_JoindreDG (pl_NomDG) )
			{
				vl_cr = XDC_OK;
			}
			else
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Echec abonnement au Datagroup %s", pl_NomDG);
			}
		}
	}


	return (vl_cr);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Initialisation du module de calcul des Temps de Parcours.
*
*  Lecture en base de donnees de la configuration des zones Tdp,
*  des destinations TdP, des equipements PMV et PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD101_Initialiser_TDP (XDY_District va_site)

/*
* ARGUMENTS EN ENTREE :
*
*   va_site           : Numero du site local.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Lecture en base de donnees des PMV et PMVA pilotes dans le secteur,
*   des zones ZdP et des destinations TdP.
*   Remplissage des structures de donnees.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD101_Initialiser_TDP" ;

	int			vl_cr = XDC_OK;
	XDY_Entier		vl_NbElement = 0;


	/*A
	** Memoriser le numero du site local
	*/

	vm_Site = va_site;


	/*A
	** Initialiser les listes des zones et destinations Temps de Parcours
	*/

	vl_cr = XZAO4001_Liste_Zones_De_Parcours ( XDC_BASE_CFG,
	                                              (XDY_District) 0,
	                                              (XDY_FonctionInt) XZAD_ajouter_ZDP,
	                                              &vl_NbElement );
	if (vl_cr != XDC_OK)
		return (XDC_NOK);

	vl_cr = XZAO401_Lire_Liste_Destination_TDP ( XDC_BASE_CFG,
	                                                (XDY_FonctionInt) XZAD_ajouter_Destination,
	                                                &vl_NbElement );
	if (vl_cr != XDC_OK)
		return (XDC_NOK);

	/*A
	** Initialiser la liste des equipements panneaux
	*/

	vl_cr = XZAO410_Lire_Liste_PMV_TDP (XZAD_ajouter_PMV, &vl_NbElement);
	if (vl_cr != XDC_OK)
		return (XDC_NOK);

	vl_cr = XZAO468_Lire_Liste_PMVA_TDP (XZAD_ajouter_PMVA, &vl_NbElement );
	if (vl_cr != XDC_OK)
		return (XDC_NOK);


	/*A
	** Renseigner les destinations des panneaux, ainsi que leur graphe de zones
	*/

	vl_cr = XZAO470_Lire_Liste_Destination_PMV (XDC_BASE_CFG,
	                                               (XDY_FonctionInt) XZAD_ajouter_Dest_pmv,
	                                               &vl_NbElement);
	if (vl_cr != XDC_OK)
		return (XDC_NOK);

	vl_cr = XZAO469_Lire_Liste_Destination_PMVA (XDC_BASE_CFG,
	                                                (XDY_FonctionInt) XZAD_ajouter_Dest_pmva,
	                                                &vl_NbElement );
	if (vl_cr != XDC_OK)
		return (XDC_NOK);


	/*B
	** Indexer les ZdP UTILISES DANS LES GRAPHES selon leur numero
	*/

	vl_cr = XZAD_Indexer_ZDP ();
	if (vl_cr != XDC_OK)
		return (XDC_NOK);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Demarrage de la fonction de mesure des Temps de Parcours.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD110_Demarrer_TDP ()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Initialisation des mesures des ZDP utilisees par lecture de leur derniere valeur.
*   Abonnement aux messages de mesures pour les zones utilisees.
*
-----------------------------------------------------------------------------------------*/
{
	int	vl_cr;

	/*A
	** Lire en base les valeurs actuelles de TdP des zones utilisees
	*/

	vl_cr = XZAD_Lire_Valeurs_TDP ();
	if (vl_cr != XDC_OK)
		return (XDC_NOK);


	//Dump_ZDP ();


	/*A
	** S'abonner aux messages de mesures TdP pour les zones utilisees
	*/

	vl_cr = XZAD_Abonnement_Mesures_TDP ();
	if (vl_cr != XDC_OK)
		return (XDC_NOK);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Terminaison du module de calcul des Temps de Parcours.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD102_Terminer_TDP ()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte, XDC_NOK sinon.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Desabonnement de la messagerie.
*
-----------------------------------------------------------------------------------------*/
{
	XDY_Datagroup		pl_NomDG;
	T_BOOL			vl_EtatDg = FALSE;

	int			vl_cr = XDC_OK;
	int			i;


	/*A
	** Se desabonner aux messages de mesures TdP pour les zones utilisees
	*/

	/* Pour chaque numero de zone ZdP */
	for (i = 0;  i < vm_Nb_Idx_Zdp;  i = i + 1)
	{
		/* s'il est le numero d'une zone utilisee alors */
		if (tm_i_zdp[i] >= 0)
		{
			/* desabonnement au Datagroup des mesures TdP de cette zone */
			sprintf (pl_NomDG, "%s_%d", XDG_EMES_UGTP_6MN, tm_ZDP[tm_i_zdp[i]].Numero);
			TipcSrvSubjectGetSubscribe (pl_NomDG, &vl_EtatDg);

			if (vl_EtatDg)
			{
				if ( !XDG_QuitterDG (pl_NomDG) )
				{
					XZST_03EcritureTrace (XZSTC_WARNING, "Echec desabonnement au Datagroup %s", pl_NomDG);
					vl_cr = XDC_NOK;
				}
			}
		}
	}


	/*A
	** Desarmer la Callback sur reception des messages de mesures TdP zones
	*/

	if (vm_CB_XZAD_Mesure_tdp != NULL)
	{
		if (TutCbDestroy (vm_CB_XZAD_Mesure_tdp) == FALSE)
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Echec desarmement Callback messages de mesures TdP");
			vl_cr = XDC_NOK;
		}
		vm_CB_XZAD_Mesure_tdp = NULL;
	}

	return (vl_cr);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Calcule et fournit a un client les donnees de Temps de Parcours
*  affichables sur un PMV de numero donne au moment de la demande.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD103_Temps_De_Parcours_PMV (XDY_Eqt	va_Numero_PMV,
                                   XZADT_Temps_De_Parcours_PMV	*pa_TDP)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Numero_PMV     : Numero du PMV.
*
* ARGUMENTS EN SORTIE :
*
*   *pa_TDP           : Donnees de temps de parcours pour le PMV.
*
* CODE RETOUR :
*
*   XDC_OK		Execution correcte
*   XZADC_ERR_INVALIDE	Un parametre d'entree est invalide
*   XZADC_ERR_INCONNU	Le numero n'est pas celui d'un PMV affichant des TdP
*   XDC_NOK		Une erreur s'est produite.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Recherche si le PMV est susceptible d'afficher des temps de parcours ;
*   si c'est le cas, calcule tous les temps de parcours actuels pour le PMV.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD103_Temps_De_Parcours_PMV" ;

	XZADT_Liste_PMV_TDP	*pl_PMV = NULL;
	XDY_Booleen		vl_Tdp_Valide;
	int			vl_cr = XDC_OK;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN: %s numero %d", version, va_Numero_PMV);

	/*B Verifier la validite des parametres d'entree */
	if (pa_TDP == NULL)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "%s: Zone de donnees TdP NULLe", version);
		return (XZADC_ERR_INVALIDE);
	}

	/*A
	** Initialiser les valeurs retournees par defaut
	*/

	memset ((void *) pa_TDP, 0, sizeof (pa_TDP));
	pa_TDP->Dest_Proche.Numero = XDC_DTP_AUCUNE;
	pa_TDP->Dest_Loin.Numero = XDC_DTP_AUCUNE;
	pa_TDP->Dest_Noeud.Numero = XDC_DTP_AUCUNE;


	/*A
	** Rechercher le PMV dans la liste
	*/

	for ( pl_PMV = pm_ListePMV;  pl_PMV != NULL;  pl_PMV = pl_PMV->suivant )
	{
		if ( pl_PMV->PMV.Numero == va_Numero_PMV )
		{
			break;
		}
	}

	if (pl_PMV == NULL)
	{
		XZST_03EcritureTrace (XZSTC_INFO, "Le PMV %d n'affiche pas de TdP", va_Numero_PMV);
		return (XZADC_ERR_INCONNU);
	}


	/*A
	** Pour chaque type de destination pour le PMV,
	** si une configuration TDP existe alors calculer sa valeur
	*/

	if (pl_PMV->Destination_Proche != NULL)
	{
		pa_TDP->Dest_Proche = *(pl_PMV->Destination_Proche);
		vl_cr = XZAD_Calcul_TDP ( pl_PMV->PMV.Autoroute,
		                          pl_PMV->PMV.PR,
		                          pl_PMV->PMV.Sens,
		                          pl_PMV->Destination_Proche,
		                          pl_PMV->Graphe_Dest_Proche,
		                          &pa_TDP->TDP_Proche,
		                          &vl_Tdp_Valide );
	}

	if (pl_PMV->Destination_Loin != NULL)
	{
		pa_TDP->Dest_Loin = *(pl_PMV->Destination_Loin);
		vl_cr = XZAD_Calcul_TDP ( pl_PMV->PMV.Autoroute,
		                          pl_PMV->PMV.PR,
		                          pl_PMV->PMV.Sens,
		                          pl_PMV->Destination_Loin,
		                          pl_PMV->Graphe_Dest_Loin,
		                          &pa_TDP->TDP_Loin,
		                          &vl_Tdp_Valide );
	}

	if (pl_PMV->Destination_Noeud != NULL)
	{
		pa_TDP->Dest_Noeud = *(pl_PMV->Destination_Noeud);
		vl_cr = XZAD_Calcul_TDP ( pl_PMV->PMV.Autoroute,
		                          pl_PMV->PMV.PR,
		                          pl_PMV->PMV.Sens,
		                          pl_PMV->Destination_Noeud,
		                          pl_PMV->Graphe_Dest_Noeud,
		                          &pa_TDP->TDP_Noeud,
		                          &vl_Tdp_Valide );
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Calcule et fournit a un client les donnees de Temps de Parcours
*  affichables sur un PMVA de numero donne au moment de la demande.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD104_Temps_De_Parcours_PMVA (XDY_Eqt	va_Numero_PMVA,
                                    XZADT_Temps_De_Parcours_PMVA	*pa_TDP)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Numero_PMVA     : Numero du PMVA.
*
* ARGUMENTS EN SORTIE :
*
*   *pa_TDPA          : Donnees de temps de parcours pour le PMVA.
*
* CODE RETOUR :
*
*   XDC_OK		Execution correcte
*   XZADC_ERR_INVALIDE	Un parametre d'entree est invalide
*   XZADC_ERR_INCONNU	Le numero n'est pas celui d'un PMV affichant des TdP
*   XDC_NOK		Une erreur s'est produite.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Recherche si le PMVA est susceptible d'afficher des temps de parcours ;
*   si c'est le cas, calcule tous les temps de parcours actuels pour le PMVA.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzad.c,v 1.22 2021/06/15 17:25:33 devgfi Exp $ : XZAD104_Temps_De_Parcours_PMVA" ;

	XZADT_Liste_PMVA_TDP	*pl_PMVA = NULL;
	XDY_Booleen		vl_Tdp_Valide;
	int			vl_cr = XDC_OK;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN: %s numero %d", version, va_Numero_PMVA);

	/*B Verifier la validite des parametres d'entree */
	if (pa_TDP == NULL)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "%s: Zone de donnees TdP NULLe", version);
		return (XZADC_ERR_INVALIDE);
	}

	/*A
	** Initialiser les valeurs retournees par defaut
	*/

	memset ((void *) pa_TDP, 0, sizeof (pa_TDP));
	pa_TDP->Dest1_Proche.Numero = XDC_DTP_AUCUNE;
	pa_TDP->Dest1_Loin.Numero = XDC_DTP_AUCUNE;
	pa_TDP->Dest2_Proche.Numero = XDC_DTP_AUCUNE;
	pa_TDP->Dest2_Loin.Numero = XDC_DTP_AUCUNE;


	/*A
	** Rechercher le PMVA dans la liste
	*/

	for ( pl_PMVA = pm_ListePMVA;  pl_PMVA != NULL;  pl_PMVA = pl_PMVA->suivant )
	{
		if ( pl_PMVA->PMVA.Numero == va_Numero_PMVA )
		{
			break;
		}
	}

	if (pl_PMVA == NULL)
	{
		XZST_03EcritureTrace (XZSTC_INFO, "Le PMVA %d n'affiche pas de TdP", va_Numero_PMVA);
		return (XZADC_ERR_INCONNU);
	}


	/*A
	** Pour chaque type de destination pour le PMVA,
	** si une configuration TDP existe alors calculer sa valeur
	*/

	if (pl_PMVA->Destination1_proche != NULL)
	{
		pa_TDP->Dest1_Proche = *(pl_PMVA->Destination1_proche);
		vl_cr = XZAD_Calcul_TDP ( pl_PMVA->PMVA.Autoroute,
		                          pl_PMVA->PMVA.PR,
		                          pl_PMVA->PMVA.Sens,
		                          pl_PMVA->Destination1_proche,
		                          pl_PMVA->Graphe_proche1,
		                          &pa_TDP->TDP1_Proche,
		                          &vl_Tdp_Valide );
	}

	if (pl_PMVA->Destination1_loin != NULL)
	{
		pa_TDP->Dest1_Loin = *(pl_PMVA->Destination1_loin);
		vl_cr = XZAD_Calcul_TDP ( pl_PMVA->PMVA.Autoroute,
		                          pl_PMVA->PMVA.PR,
		                          pl_PMVA->PMVA.Sens,
		                          pl_PMVA->Destination1_loin,
		                          pl_PMVA->Graphe_loin1,
		                          &pa_TDP->TDP1_Loin,
		                          &vl_Tdp_Valide );
	}

	if (pl_PMVA->Destination2_proche != NULL)
	{
		pa_TDP->Dest2_Proche = *(pl_PMVA->Destination2_proche);
		vl_cr = XZAD_Calcul_TDP ( pl_PMVA->PMVA.Autoroute,
		                          pl_PMVA->PMVA.PR,
		                          pl_PMVA->PMVA.Sens,
		                          pl_PMVA->Destination2_proche,
		                          pl_PMVA->Graphe_proche2,
		                          &pa_TDP->TDP2_Proche,
		                          &vl_Tdp_Valide );
	}

	if (pl_PMVA->Destination2_loin != NULL)
	{
		pa_TDP->Dest2_Loin = *(pl_PMVA->Destination2_loin);
		vl_cr = XZAD_Calcul_TDP ( pl_PMVA->PMVA.Autoroute,
		                          pl_PMVA->PMVA.PR,
		                          pl_PMVA->PMVA.Sens,
		                          pl_PMVA->Destination2_loin,
		                          pl_PMVA->Graphe_loin2,
		                          &pa_TDP->TDP2_Loin,
		                          &vl_Tdp_Valide );
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonctions utilitaires :
*
*  Recherche d'une destination de Temps de Parcours par son numero ou son nom.
*  Recherche d'une zone de mesures Temps de Parcours par son code.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD_Destination_De_Numero (	XDY_Entier		va_numero,
					XDY_Destination		**ppa_Destination)
{
	XZADT_Liste_Destinations	*pl_dest = pm_ListeDest;

	(*ppa_Destination) = NULL;

	while (pl_dest != NULL) {
		if (pl_dest->destination.Numero == va_numero) {
			(*ppa_Destination) = &pl_dest->destination;
			return (XDC_OK);
		}
		pl_dest = pl_dest->suivant;
	}

	return (XDC_NOK);
}


int XZAD_Destination_De_Nom (	XDY_Nom			pa_nom,
				XDY_Destination		**ppa_Destination)
{
	XZADT_Liste_Destinations	*pl_dest = pm_ListeDest;

	(*ppa_Destination) = NULL;

	while (pl_dest != NULL) {
		if (strcmp (pl_dest->destination.Libelle_PMV, pa_nom) == 0) {
			(*ppa_Destination) = &pl_dest->destination;
			return (XDC_OK);
		}
		pl_dest = pl_dest->suivant;
	}

	return (XDC_NOK);
}


int XZAD_ZDP_De_Code (	XDY_Entier	va_code_zdp,
			XDY_ZDP		**ppa_ZDP)
{
	int		i;

	(*ppa_ZDP) = NULL;

	for (i = 0;  i < vm_Nb_ZDP;  i++) {
		if ( tm_ZDP[i].Code_Zone == va_code_zdp) {
			(*ppa_ZDP) = &tm_ZDP[i];
			return (XDC_OK);
		}
	}

	return (XDC_NOK);
}
