/*E*/
/*  Fichier : $Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2021/06/15 17:31:20 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE ANET * FICHIER anet_cyc.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module des traitements cycliques de la tache TANET.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	03/04/17 : Creation (DEM 1221)  1.1
* JPL	10/05/17 : Gestion de l'etat d'affichage des PMV / PMVA  1.2
* JPL	12/05/17 : Version complete avec mise au neutre des PMV / PMVA  1.3
* JPL	17/05/17 : Commande de chaines vides pour alternats  1.4
* JPL	17/05/17 : Prise en compte des minuscules et majuscules pour les motifs de TdP affiches  1.5
* JPL	09/04/18 : Appel XZAC640 pour tout panneau valide, sans texte de neutre, avec texte affiché et mode (DEM 1278)  1.6
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include	<xdc.h>
#include	<xdy.h>
#include	<xzst.h>
#include	<xzsm.h>

#include	<xzac640.h>

#include	"anet_cfg.h"
#include	"anet_cyc.h"


/* definitions de constantes */



/* definitions de types locaux */



/* definition de macro locales */



/* Declaration de variables */
/* ------------------------ */

extern	XDY_Booleen	vg_Init_A_Faire;


static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_cyc.c" ;


static T_CB	vm_CB_ANET_Timer = NULL;



/* Declaration de fonctions internes */
/* --------------------------------- */

static int anet_traitement_cyclique ();



/* Definition de fonctions externes */
/* -------------------------------- */



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Traitement des evenements de type Timer.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void anet_Timer_Sequenceur_cb (
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*	Appel sur reception d'un evenement TIMER.
*
* FONCTION
*	Effectue les traitement suivants :
*	   - Consulte et met a jour l'etat des equipements PMV[A] et ZdP ;
*	   - Calcule les temps de parcours pour les destinations configurees des PMV[A] ;
*	   - Commande l'affichage programme sur les equipements PMV[A] inoccupes.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_Timer_Sequenceur_cb" ;

	int	vl_cr;

	XZST_03EcritureTrace (XZSTC_INFO, "          TOP TIMER MISE AU NEUTRE SAGA");

	vl_cr = anet_traitement_cyclique ();
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Lancement du Timer de mise au neutre.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_cyc_lancement_timer ()

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK si l'operation se deroule correctement, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*   Au demarrage de la tache.
*
* FONCTION
*   Active le timer de mise au neutre des equipements panneaux.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_cyc_lancement_timer" ;

	XDY_Datagroup		pl_dg_timer;
	XZSMT_CleProg		pl_cle_timer;
	int			vl_JourSemaine;
	XDY_Horodate		vl_DateEnSeconde, vl_DateReveil;
	long			vl_prochaineMinute;
	int			vl_cr = 0;
	long			vl_seconde, vl_minute, vl_heure;
	long			vl_p = ANET_PERIODE_MISE_AU_NEUTRE, 	/* pour concision */
				vl_m = ANET_PERIODE_PREMIERE_MIN;	/* pour concision */
				

	/*A
	** Initialisation des services Timer
	*/

	XZSM_00InitServices ();

	XDG_EncodeDG2 (pl_dg_timer, XDG_ANEUTR_TIMER, vg_ConfigNeutreSaga.NomMachine);
	sprintf (pl_cle_timer, "%s_%s", XDG_ANEUTR_TIMER, vg_ConfigNeutreSaga.NomMachine);


	/*A
	** Lire l'horodate actuelle
	** et programmer le premier declenchement de reveil a la premiere minute,
	** plus 20 secondes
	*/

	XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_DateEnSeconde);

	vl_seconde = ((long) vl_DateEnSeconde) % 3600;
	/* declenche le reveil toutes les vl_p minutes, en commencant par la minute vl_m */
	vl_minute = (((vl_seconde / 60) + vl_p - vl_m) % 60) / vl_p * vl_p + vl_m;
	vl_heure = ((long) vl_DateEnSeconde) - vl_seconde;
	vl_prochaineMinute = (vl_heure + (vl_minute * 60) + 20);
	
	vl_DateReveil = (XDY_Horodate) vl_prochaineMinute;

	/*!!! Ne pas programmer de nouveau reveil, mais s'abonner a la reception du reveil de la tache taneu.x */
//	/*A Programmer le declenchement du reveil a l'horodate voulue */
//	vl_cr = XZSM_01DemanderProgrammation (vl_DateReveil, (ANET_PERIODE_MISE_AU_NEUTRE * 60),
//	                                      pl_dg_timer, XDM_ANEUTR_TIMER,
//	                                      pl_cle_timer, 2);
//	if (vl_cr == XDC_OK)
//	{
//		XZST_03EcritureTrace (XZSTC_INFO, "Timer de mise au neutre SAGA programme a %02d:%02d:20",
//		                         (int) ((vl_prochaineMinute / 3600) % 24), (int) ((vl_prochaineMinute / 60) % 60));
//	}
//	else
//	{
//		XZST_03EcritureTrace (XZSTC_WARNING, "Timer de mise au neutre SAGA non enclenche (cr=%d)", vl_cr);
//		return (XDC_NOK);
//	}


	/*A
	** Armement de la Callback de traitement du message Timer de mise au neutre
	*/

	vm_CB_ANET_Timer = TipcSrvProcessCbCreate (XDM_IdentMsg (XDM_ANEUTR_TIMER),
	                                             (T_IPC_CONN_PROCESS_CB_FUNC) anet_Timer_Sequenceur_cb,
	                                             (T_CB_ARG) NULL);
	if (vm_CB_ANET_Timer == NULL)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Erreur init callback Timer de mise au neutre SAGA");
		return (XDC_NOK);
	}


	/*A
	** Abonnement au Datagroup de reception du message Timer de mise au neutre
	*/

	if (! XDG_JoindreDG (pl_dg_timer))
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Abonnement SAGA au Datagroup %s non effectue", pl_dg_timer);
		return (XDC_NOK);
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Arret du Timer de mise au neutre.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_cyc_arret_timer ()

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK si l'operation se deroule correctement, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*   Au demarrage de la tache.
*
* FONCTION
*   Active le timer de mise au neutre des equipements panneaux.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_cyc_arret_timer" ;

	XDY_Datagroup	pl_dg_timer;
	T_BOOL		vl_EtatDg = FALSE;

	int		vl_cr = XDC_OK;

	XDG_EncodeDG2 (pl_dg_timer, XDG_ANEUTR_TIMER, vg_ConfigNeutreSaga.NomMachine);

	/*A
	** Annuler la programmation du reveil de mise au neutre
	*/

	/*!!! Le reveil utilise est celui de la tache taneu.x */
//	XZSM_02AnnulerProgrammation (XZSMC_CLEPROG_NUL, pl_dg_timer, 0);


	/*A
	** Desabonnement au Datagroup de reception du message Timer de mise au neutre
	*/

	TipcSrvSubjectGetSubscribe (pl_dg_timer, &vl_EtatDg);
	if (vl_EtatDg)
	{
		if (! XDG_QuitterDG (pl_dg_timer))
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Echec desabonnement SAGA au Datagroup %s", pl_dg_timer);
			vl_cr = XDC_NOK;
		}
	}


	/*A
	** Desarmement de la Callback de traitement du message Timer de mise au neutre
	*/

	if (vm_CB_ANET_Timer != NULL)
	{
		if (TutCbDestroy (vm_CB_ANET_Timer) == FALSE)
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Echec desarmement Callback Timer SAGA de mise au neutre");
			vl_cr = XDC_NOK;
		}
		vm_CB_ANET_Timer = NULL;
	}


	/* Temporiser pour attendre la fin des echanges en cours */
	sleep (3);

	return (vl_cr);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise a jour de l'etat des PMV / PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_MAJ_etat_PMV_SAGA ( int		va_nb_PMV,
                             int		va_i_PMV,
                             XZATT_EtatEqt	va_PMV )

/*
* ARGUMENTS EN ENTREE :
*
*   va_nb_PMV         : Nombre total d'equipements lus en base ;
*   va_i_PMV          : Numero d'ordre (rang) de l'equipement fourni ;
*   va_PMV            : Donnees de l'equipement PMV.
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Mise a jour de l'etat du PMV indique.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_MAJ_etat_PMV_SAGA" ;

	int		vl_i_PMV;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s numero %d", version, (int) va_PMV.NumeroEqt);

	/*A Rechercher le PMV dans la table par son numero */
	for ( vl_i_PMV = 0;  vl_i_PMV < vg_Nb_PMV_SAGA;  vl_i_PMV++ )
	{
		if ( tg_Liste_PMV_SAGA[vl_i_PMV].NumeroEqt == va_PMV.NumeroEqt)
		{
			break;
		}
	}


	if (vl_i_PMV >= vg_Nb_PMV_SAGA)
	{
		/* L'equipement SAGA n'est pas un PMV ou un PMVA */
		return (XDC_OK);
	}


	/*A Si le PMV est trouve alors mettre a jour son etat */
	tg_Liste_PMV_SAGA[vl_i_PMV].EtatEqt = va_PMV.EtatEqt;


	/*A En phase d'initialisation, renseigner les caracteristiques des PMV et PMVA */
	if (XDC_VRAI == vg_Init_A_Faire) {
		tg_Liste_PMV_SAGA[vl_i_PMV].NumAuto = va_PMV.NumAuto;
		tg_Liste_PMV_SAGA[vl_i_PMV].PR = va_PMV.PR;
		tg_Liste_PMV_SAGA[vl_i_PMV].Sens = va_PMV.Sens;

		XZST_03EcritureTrace ( XZSTC_INFO, "PMV SAGA %d: Loc=%d-%d-%d",
		                          (int) tg_Liste_PMV_SAGA[vl_i_PMV].NumeroEqt,
		                          (int) tg_Liste_PMV_SAGA[vl_i_PMV].NumAuto,
		                          (int) tg_Liste_PMV_SAGA[vl_i_PMV].Sens,
		                          (int) tg_Liste_PMV_SAGA[vl_i_PMV].PR);
	}

	XZST_03EcritureTrace ( XZSTC_INFO, "PMV SAGA %d: ETAT=%d",
	                          (int) tg_Liste_PMV_SAGA[vl_i_PMV].NumeroEqt, (int) tg_Liste_PMV_SAGA[vl_i_PMV].EtatEqt);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Determination de l'etat d'une ligne d'affichage PMV / PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen anet_Ligne_PMV_SAGA_Est_Vide ( char	*pa_Ligne )
{
	char	*pl_car;

	/* Une ligne est consideree vide ssi. elle n'est constituee que d'espaces et de points */
	for (pl_car = pa_Ligne;  (*pl_car);  pl_car++) {
		if ( (' ' != *pl_car)  &&  ('.' != *pl_car) ) {
			return (XDC_NON);
		}
	}

	return (XDC_OUI);
}



XDY_Booleen anet_Ligne_PMV_SAGA_Est_TDP ( char	*pa_Ligne )
{
	int	vl_lgr_ligne = strlen (pa_Ligne);
	char	*pl_car = pa_Ligne + (vl_lgr_ligne - 4);
	int	vl_c1 = pl_car[0], vl_c2 = pl_car[1], vl_c3 = pl_car[2], vl_c4 = pl_car[3];

	/* Le contenu de la ligne est considere comme indiquant un Temps de parcours     */
	/* si sa fin est un motif du type "XhXX" ou "XXmn" ou " Xmn" precede d'un espace */

	if (vl_lgr_ligne < 4) {
		return (XDC_NON);
	}

	if ( (vl_lgr_ligne >= 5)  &&  (' ' != pa_Ligne[vl_lgr_ligne - 5]) ) {
		return (XDC_NON);
	}

	if ( isdigit (vl_c1)  &&  ('H' == toupper (vl_c2))  &&  isdigit (vl_c3)  &&  isdigit (vl_c4) ) {
		return (XDC_OUI);
	}

	if ( (' ' == vl_c1 || isdigit (vl_c1))  &&  isdigit (vl_c2)  && ('M' == toupper (vl_c3))  &&  ('N' == toupper (vl_c4)) ) {
		return (XDC_OUI);
	}

	return (XDC_NON);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Determination de l'etat d'affichage des PMV / PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_PMV_SAGA_Est_Au_Neutre ( ANET_PMV_A_SAGA	*pa_PMV )

/*
* ARGUMENTS EN ENTREE :
*
*   pa_PMV            : Donnees du PMV dont l'etat est a analyser.
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*   XDC_OUI ou XDC_NON selon que le panneau est au neutre (temps de parcours ou autre).
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Analyse de toutes les lignes du PMV : recherche de texte autre que temps de parcours.
*
-----------------------------------------------------------------------------------------*/
{
	int	vl_est_neutre = XDC_OUI;
	int	i;

	char	*tl_lignes_PMV[] = {
		pa_PMV->Affiche.Texte1,
		pa_PMV->Affiche.Texte2,
		pa_PMV->Affiche.Texte3,
		pa_PMV->Affiche.Texte4,
		pa_PMV->Affiche.Texte5
	};

	for (i = 0;  (i < pa_PMV->NbLignes)  &&  (XDC_OUI == vl_est_neutre);  i++) {
		if (anet_Ligne_PMV_SAGA_Est_Vide (tl_lignes_PMV[i]) != XDC_OUI  &&
		    anet_Ligne_PMV_SAGA_Est_TDP (tl_lignes_PMV[i]) != XDC_OUI)
		{
			vl_est_neutre = XDC_NON;
		}
	}

	XZST_03EcritureTrace (XZSTC_INFO, "PMV SAGA %d %s neutre", (int) pa_PMV->NumeroEqt, (XDC_OUI == vl_est_neutre ? "" : "non"));

	return (vl_est_neutre);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Fonction utilitaire consituant une ligne de texte de longueur donnee
*   indiquant un Temps de Parcours a afficher sur un PMV ou un PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static char *anet_Ligne_TDP_SAGA ( XDY_Nom	pa_Destination,
                                   XDY_TDP	va_Tdp,
                                   XDY_Octet	va_Lgr_Ligne)

{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_Ligne_TDP_SAGA" ;

	static	char	vl_Ligne_PMV[XDC_Lon_Ligne_Eqt_SAGA];		/* Ligne de texte retournee */

	char	vl_txt_tdp[8] = "";
	size_t	vl_lgr_dest, vl_lgr_max_dest;


	/*B Par defaut, retourner une ligne vide */
	sprintf (vl_Ligne_PMV, "%*c", va_Lgr_Ligne, ' ');


	/*A
	** Verifier la validite des parametres d'entree
	*/

	if ( (pa_Destination == NULL)  ||  (va_Tdp <= 0) )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "%s: destination ou valeur TdP (%d) invalide", version, va_Tdp);
		return (vl_Ligne_PMV);
	}


	/*A
	** Constituer la valeur textuelle de Temps de Parcours (avec separateur)
	*/

	/* Si le temps de parcours est trop grand alors le borner */
	if (va_Tdp > 600)	/* 10 heures */
	{
		strcpy (vl_txt_tdp, " 9h59");
	}
	else if (va_Tdp > 59)
	{
		/* sinon s'il est superieur a 1h00 alors l'afficher en heures */
		sprintf (vl_txt_tdp, " %1dh%02d", va_Tdp / 60, va_Tdp % 60);
	}
	else
	{
		/* sinon l'afficher en minutes */
		sprintf (vl_txt_tdp, " %dmn", va_Tdp);
	}


	/*A
	** Constituer la ligne a afficher avec la destination de longueur bornee
	** et le texte de Temps de Parcours cadre a droite
	*/

	vl_lgr_max_dest = va_Lgr_Ligne - strlen (vl_txt_tdp);

	vl_lgr_dest = strlen (pa_Destination);
	if (vl_lgr_dest > vl_lgr_max_dest) {
		vl_lgr_dest = vl_lgr_max_dest;
	}
	strncpy (vl_Ligne_PMV, pa_Destination, vl_lgr_dest);

	strcpy (&(vl_Ligne_PMV[vl_lgr_max_dest]), vl_txt_tdp);

	return (vl_Ligne_PMV);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise au neutre d'un PMV SAGA en etat de fonctionnement et non occupe.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int anet_PMV_SAGA_Au_Neutre ( ANET_PMV_A_SAGA	*pa_PMV )

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
*   Cycliquement, sur declenchement du Timer.
*
* FONCTION
*
*   Calcule les temps de parcours configures pour le PMV indique,
*   qui n'est pas hors service et n'est pas utilise pour un evenement.
*   Commande l'affichage de ces temps de parcours, ou du texte neutre a defaut.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_PMV_SAGA_Au_Neutre" ;

	char		ll_Lignes[ANET_NB_LIG_PMV_SAGA][XDC_Lon_Ligne_Eqt_SAGA];

	XDY_Texte	vl_Texte_Affiche = "";
	XDY_Octet	vl_Fin_Action = XDC_FAUX;
	XDY_Entier	vl_Num_Action = 0;
	XDY_Octet	vl_Type_Neutre = 0;

	XDY_TDP		vl_TdP;
	XDY_Booleen	vl_TdP_Valide;
	int		vl_nb_TdP = 0;
  	XDY_Octet	vl_inhb;

	char	*tl_lignes_PMV[] = {
		pa_PMV->Affiche.Texte1,
		pa_PMV->Affiche.Texte2,
		pa_PMV->Affiche.Texte3,
		pa_PMV->Affiche.Texte4,
		pa_PMV->Affiche.Texte5
	};

	char		*pl_texte;
	int		vl_i;
	int		vl_cr;

	/* Initialiser les lignes (blanches) */
	for ( vl_i = 0;  vl_i < ANET_NB_LIG_PMV_SAGA;  vl_i++ ) {
		sprintf (ll_Lignes[vl_i], "%*c", pa_PMV->NbCarLigne, ' ');
	}


	/* Pour chaque ligne affichant un Temps de Parcours */
	for ( vl_i = 0;  vl_i < ANET_MAX_LIG_TDP;  vl_i++ ) {
		/* si une destination est configureee pour cette ligne alors */
		if (NULL != pa_PMV->DestTdP[vl_i].destination) {
			
			/* Test pour savoir si les tdp sur cette eqt sont inhibe */
			xzae59_Rechercher_EQT_TDP_INHIB(pa_PMV->NumeroEqt,XDC_EQT_PMV,&vl_inhb);
			if (vl_inhb)
			{
				XZST_03EcritureTrace(XZSTC_INFO,"anet_PMV_SAGA_Au_Neutre TDP PMV %d inhibe, on retourne -1",pa_PMV->NumeroEqt);
				vl_TdP=-1;
				return (XDC_OK);
			}
			else
				XZAD_Calcul_TDP (pa_PMV->NumAuto, pa_PMV->PR, pa_PMV->Sens,
			                    pa_PMV->DestTdP[vl_i].destination, pa_PMV->DestTdP[vl_i].graphe,
			                    &vl_TdP, &vl_TdP_Valide);

			/* si un Temps de Parcours est disponible alors constituer le message TdP */
			if (vl_TdP > 0) {
				strcpy (ll_Lignes[vl_i], anet_Ligne_TDP_SAGA (pa_PMV->DestTdP[vl_i].destination->Libelle_PMV,
				                                                 vl_TdP, pa_PMV->NbCarLigne));
				vl_nb_TdP = vl_nb_TdP + 1;
			}
		}
	}


	/* S'il n'y a aucun Temps de Parcours disponible alors mettre l'affichage au neutre */
/*
	if ( vl_nb_TdP <= 0) {
		char	*vl_Derniere_Ligne = ll_Lignes[pa_PMV->NbLignes - 1];
		strcpy (&(vl_Derniere_Ligne[pa_PMV->NbCarLigne - 3]), "...");
	}
*/


	/* Constituer l'état d'affichage transmis à la procédure de mise au neutre :  */
	/* une seule ligne sans espaces si du texte est affiché, sinon la chaine vide */
	if (anet_PMV_SAGA_Est_Au_Neutre (pa_PMV)) {
		strcpy (vl_Texte_Affiche, XDC_CHAINE_VIDE);
	} else {
		pl_texte = vl_Texte_Affiche;
		for ( vl_i = 0;  vl_i < pa_PMV->NbLignes;  vl_i++ ) {
			XDY_Copie_Sans_Espaces (pl_texte, tl_lignes_PMV[vl_i]);
			pl_texte = pl_texte + strlen (pl_texte);
		}
		strupper (vl_Texte_Affiche);
	}


	XZST_03EcritureTrace (XZSTC_INFO, "PMV SAGA %s au neutre: /%s/%s/%s/",
				 pa_PMV->NomEqt,
				 ll_Lignes[0], ll_Lignes[1], ll_Lignes[2]);

	/* Tenter la mise au neutre du panneau si possible */
	vl_cr = XZAC640_Mise_Au_Neutre_PMVx_SAGA ((XDY_Eqt) pa_PMV->NumeroEqt, vg_ConfigNeutreSaga.NumeroSite,
						    vl_Texte_Affiche,
						    vl_Fin_Action,
						    ll_Lignes[0], ll_Lignes[1], ll_Lignes[2], ll_Lignes[3], ll_Lignes[4],
						    XDC_CHAINE_VIDE, XDC_CHAINE_VIDE, XDC_CHAINE_VIDE, XDC_CHAINE_VIDE, XDC_CHAINE_VIDE,
						    &vl_Num_Action,
						    &vl_Type_Neutre);
	XZST_03EcritureTrace (XZSTC_INFO, "Mise au neutre: CR=%d, Action en cours=%d, Type neutre=%d",
	                         (int) vl_cr, (int) vl_Num_Action, (int) vl_Type_Neutre);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Traitement des evenements de type Timer.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int anet_traitement_cyclique ()

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
*	Appel sur reception d'un evenement TIMER.
*
* FONCTION
*
*  - Consulte et met a jour l'etat des equipements PMV[A] ;
*
*  - Pour chaque PMV[A] qui n'est pas en alarme majeure ou hors service,
*    s'il affiche actuellement un temps de parcours ou est au neutre alors
*    calcule les temps de parcours a afficher s'il y a lieu et commande leur affichage.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cyc.c,v 1.7 2021/06/15 17:31:20 devgfi Exp $ : anet_traitement_cyclique" ;

	int	vl_cr = XDC_OK;
	int	i = 0;


	/*A
	**        Reinitialiser l'etat des panneaux
	** (au cas ou ils ne seraient pas extraits de la base)
	*/

	for (i = 0;  i < vg_Nb_PMV_SAGA;  i++)
	{
		tg_Liste_PMV_SAGA[i].EtatEqt = ANET_EQT_ETAT_INCONNU;
	}


	/*A
	** Lire en base et mettre a jour l'etat des equipements PMV et PMVA SAGA
	*/

	vl_cr = XZAT01_Liste_Eqt_Dispo ( (XDY_TypeEqt) XDC_EQT_SAGA,
	                                    (XDY_District) vg_ConfigNeutreSaga.NumeroSite,
	                                    (XDY_Etat_Eqt) XDC_EQT_HS,
	                                    (XDY_Etat_Eqt) XDC_EQT_NON_HS,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Horodate) XZSMC_HORODATE_NUL,
	                                    anet_MAJ_etat_PMV_SAGA );


	/*A
	** Mettre au neutre les equipements PMV et PMVA qui peuvent l'etre
	*/

	for (i = 0;  i < vg_Nb_PMV_SAGA;  i++)
	{
		if ( (tg_Liste_PMV_SAGA[i].EtatEqt & XDC_EQT_MINEUR) != tg_Liste_PMV_SAGA[i].EtatEqt) {
			XZST_03EcritureTrace (XZSTC_INFO, "PMV SAGA %s en alarme", tg_Liste_PMV_SAGA[i].NomEqt);
		}
		else if ( (tg_Liste_PMV_SAGA[i].Affiche.Numero == tg_Liste_PMV_SAGA[i].NumeroEqt)	/* etat affiche connu */
		            /*&&  XDC_OUI == anet_PMV_SAGA_Est_Au_Neutre (&tg_Liste_PMV_SAGA[i])*/ )
		{
			anet_PMV_SAGA_Au_Neutre (&tg_Liste_PMV_SAGA[i]);
		}
	}

	return (XDC_OK);
}
