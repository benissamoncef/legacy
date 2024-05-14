/*E*/
/*  Fichier : $Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/11/26 14:26:00 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER aneu_cyc.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module des traitements cycliques de la tache TANEU.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/01/13 : Creation - Mise en place timer de mise au neutre (DEM 1061)  1.1
* JPL	14/02/13 : Ajout lecture des equipement et pilotage des PMV  1.2
* JPL	26/02/13 : Ajout pilotage des PMVA  1.3
* NDE	11/08/15 : Changement de periode de cycle de mise au neutre (DEM 1135) 1.4
* JPL	02/02/17 : Decoupage temps de parcours PMVA sur deux lignes (DEM 1215)  1.5
* JPL	18/12/18 : Constante de longueur commune des lignes PMV et PMVA constituees  1.6
* JPL	03/01/19 : Gestion d'un equipement ignore, ou un seul equipement gere  1.7
* JMG	16/01/20 : interface XZAC610 XZAC840 pour EQT_FIL 1.8
* JPL	18/11/20 : Mise en majuscules des messages PMV et PMVA avant mise au neutre (DEM SAE168)  1.9
* PNI	26/11/20 : Mise en majuscules des messages TDP PMV et PMVA avant mise au neutre (DEM SAE168)  1.10
* GGY	05/04/23 : Ajout test inhibition du TDP sur les PVM / PMVA (DEM-SAE431)
* LCL	21§04/21 : Correction recherche TDP inhibe : on continue la boucle
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include	<xdc.h>
#include	<xdy.h>
#include	<xzst.h>
#include	<xzsm.h>

#include	<xzat01.h>
#include	<xzad.h>
#include	<xzac610.h>
#include	<xzac840.h>

#include	"aneu_cfg.h"
#include	"aneu_cyc.h"


/* definitions de constantes */

#define		CM_LGR_LIGNE_PANNEAUX	15


//#define		CM_LGR_MAX_DEST		10				/* Nb. cars. disponibles pour destination */


#define		CM_NB_LIGNES_PMVA	5

#define		CM_NB_DEST_PMVA		4



/* definitions de types locaux */



/* definition de macro locales */



/* Declaration de variables locales */
/* -------------------------------- */

static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_cyc.c" ;

static	XDY_Ligne_PMV	vm_Ligne_Vide = "               ";


static T_CB	vm_CB_ANEUTR_Timer = NULL;



/* Declaration de variables */
/* ------------------------ */

extern	XDY_Eqt		const	vg_pmv_gere, vg_pmv_ignore;
extern	XDY_Eqt		const	vg_pmva_gere, vg_pmva_ignore;



/* Declaration de fonctions internes */
/* --------------------------------- */

static	int	aneu_traitement_cyclique ();
static	char	*aneu_ligne_majuscules(char *);



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

void aneu_Timer_Sequenceur_cb (
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
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_Timer_Sequenceur_cb" ;

	int	vl_cr;

	XZST_03EcritureTrace (XZSTC_INFO, "          TOP TIMER MISE AU NEUTRE");

	vl_cr = aneu_traitement_cyclique ();
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

int aneu_cyc_lancement_timer ()

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
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_cyc_lancement_timer" ;

	XDY_Datagroup		pl_dg_timer;
	XZSMT_CleProg		pl_cle_timer;
	int			vl_JourSemaine;
	XDY_Horodate		vl_DateEnSeconde, vl_DateReveil;
	long			vl_prochaineMinute;
	int			vl_cr = 0;
	long			vl_seconde, vl_minute, vl_heure;
	long			vl_p = ANEU_PERIODE_MISE_AU_NEUTRE, 	/* pour concision */
				vl_m = ANEU_PERIODE_PREMIERE_MIN;	/* pour concision */
				

	/*A
	** Initialisation des services Timer
	*/

	XZSM_00InitServices ();

	XDG_EncodeDG2 (pl_dg_timer, XDG_ANEUTR_TIMER, vg_ConfigNeutr.NomMachine);
	sprintf (pl_cle_timer, "%s_%s", XDG_ANEUTR_TIMER, vg_ConfigNeutr.NomMachine);


	/*A
	** Lire l'horodate actuelle
	** et programmer le premier declenchement de reveil aux minutes suivantes :
	** 01, 07, 13, 19, 25, 31, 37, 43, 49, 55
	** plus 20 secondes
	*/

	XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_DateEnSeconde);

	vl_seconde = ((long) vl_DateEnSeconde) % 3600;
	/* declenche le reveil toutes les vl_p minutes, en commencant par la minute vl_m */
	vl_minute = (((vl_seconde / 60) + vl_p - vl_m) % 60) / vl_p * vl_p + vl_m;
	vl_heure = ((long) vl_DateEnSeconde) - vl_seconde;
	vl_prochaineMinute = (vl_heure + (vl_minute * 60) + 20);
	
	vl_DateReveil = (XDY_Horodate) vl_prochaineMinute;

	/*A Programmer le declenchement du reveil a l'horodate voulue */
	vl_cr = XZSM_01DemanderProgrammation (vl_DateReveil, (ANEU_PERIODE_MISE_AU_NEUTRE * 60),
	                                      pl_dg_timer, XDM_ANEUTR_TIMER,
	                                      pl_cle_timer, 2);
	if (vl_cr == XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_INFO, "Timer de mise au neutre programme a %02d:%02d:20",
		                         (int) ((vl_prochaineMinute / 3600) % 24), (int) ((vl_prochaineMinute / 60) % 60));
	}
	else
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Timer de mise au neutre non enclenche (cr=%d)", vl_cr);
		return (XDC_NOK);
	}


	/*A
	** Armement de la Callback de traitement du message Timer de mise au neutre
	*/

	vm_CB_ANEUTR_Timer = TipcSrvProcessCbCreate (XDM_IdentMsg (XDM_ANEUTR_TIMER),
	                                             (T_IPC_CONN_PROCESS_CB_FUNC) aneu_Timer_Sequenceur_cb,
	                                             (T_CB_ARG) NULL);
	if (vm_CB_ANEUTR_Timer == NULL)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Erreur init callback Timer de mise au neutre");
		return (XDC_NOK);
	}


	/*A
	** Abonnement au Datagroup de reception du message Timer de mise au neutre
	*/

	if (! XDG_JoindreDG (pl_dg_timer))
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Abonnement au Datagroup %s non effectue", pl_dg_timer);
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

int aneu_cyc_arret_timer ()

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
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_cyc_arret_timer" ;

	XDY_Datagroup	pl_dg_timer;
	T_BOOL		vl_EtatDg = FALSE;

	int		vl_cr = XDC_OK;

	XDG_EncodeDG2 (pl_dg_timer, XDG_ANEUTR_TIMER, vg_ConfigNeutr.NomMachine);

	/*A
	** Annuler la programmation du reveil de mise au neutre
	*/

	XZSM_02AnnulerProgrammation (XZSMC_CLEPROG_NUL, pl_dg_timer, 0);


	/*A
	** Desabonnement au Datagroup de reception du message Timer de mise au neutre
	*/

	TipcSrvSubjectGetSubscribe (pl_dg_timer, &vl_EtatDg);
	if (vl_EtatDg)
	{
		if (! XDG_QuitterDG (pl_dg_timer))
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Echec desabonnement au Datagroup %s", pl_dg_timer);
			vl_cr = XDC_NOK;
		}
	}


	/*A
	** Desarmement de la Callback de traitement du message Timer de mise au neutre
	*/

	if (vm_CB_ANEUTR_Timer != NULL)
	{
		if (TutCbDestroy (vm_CB_ANEUTR_Timer) == FALSE)
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Echec desarmement Callback Timer de mise au neutre");
			vl_cr = XDC_NOK;
		}
		vm_CB_ANEUTR_Timer = NULL;
	}


	/* Temporiser pour attendre la fin des echanges en cours */
	sleep (3);

	return (vl_cr);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise a jour de l'etat des PMV.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aneu_MAJ_etat_PMV ( int		va_nb_PMV,
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
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_MAJ_etat_PMV" ;

	int		vl_i_PMV;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s numero %d", version, (int) va_PMV.NumeroEqt);

	/*A Rechercher le PMV dans la table par son numero */
	for ( vl_i_PMV = 0;  vl_i_PMV < vg_Nbr_PMV;  vl_i_PMV++ )
	{
		if ( tg_Liste_PMV[vl_i_PMV].NumeroEqt == va_PMV.NumeroEqt)
		{
			break;
		}
	}


	if (vl_i_PMV >= vg_Nbr_PMV)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "OUT %s: PMV No %d inconnu", version, (int) va_PMV.NumeroEqt);
		return (XDC_NOK);
	}


	/*A Si le PMV est trouve alors mettre a jour son etat */
	tg_Liste_PMV[vl_i_PMV].EtatEqt = va_PMV.EtatEqt;

	XZST_03EcritureTrace ( XZSTC_INFO, "PMV %d: ETAT=%d", (int) tg_Liste_PMV[vl_i_PMV].NumeroEqt,
	                                                      (int) tg_Liste_PMV[vl_i_PMV].EtatEqt);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise a jour de l'etat des PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aneu_MAJ_etat_PMVA ( int		va_nb_PMVA,
                                int		va_i_PMVA,
                                XZATT_EtatEqt	va_PMVA )

/*
* ARGUMENTS EN ENTREE :
*
*   va_nb_PMVA        : Nombre total d'equipements lus en base ;
*   va_i_PMVA         : Numero d'ordre (rang) de l'equipement fourni ;
*   va_PMVA           : Donnees de l'equipement PMVA.
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
*   Mise a jour de l'etat du PMVA indique.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_MAJ_etat_PMVA" ;

	int		vl_i_PMVA;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s numero %d", version, (int) va_PMVA.NumeroEqt);

	/*A Rechercher le PMVA dans la table par son numero */
	for ( vl_i_PMVA = 0;  vl_i_PMVA < vg_Nb_PMVA;  vl_i_PMVA++ )
	{
		if ( tg_ListePMVA[vl_i_PMVA].NumeroEqt == va_PMVA.NumeroEqt)
		{
			break;
		}
	}


	if (vl_i_PMVA >= vg_Nb_PMVA)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "OUT %s: PMVA No %d inconnu", version, (int) va_PMVA.NumeroEqt);
		return (XDC_NOK);
	}


	/*A Si le PMVA est trouve alors mettre a jour son etat */
	tg_ListePMVA[vl_i_PMVA].EtatEqt = va_PMVA.EtatEqt;

	XZST_03EcritureTrace ( XZSTC_INFO, "PMVA %d: ETAT=%d", (int) tg_ListePMVA[vl_i_PMVA].NumeroEqt,
	                                                       (int) tg_ListePMVA[vl_i_PMVA].EtatEqt);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Fonction utilitaire consituant une ligne de texte indiquant un Temps de Parcours
*   a afficher sur PMV ou sur PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static char *aneu_Ligne_TDP (	XDY_Nom		pa_Destination,
				XDY_TDP		va_Tdp)

{
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_Ligne_TDP" ;

	static	XDY_Ligne_PMV	vl_Ligne_PMV;	/* Ligne de texte retournee */

	char	vl_txt_tdp[8] = "";
	size_t	vl_lgr_dest, vl_lgr_max_dest;


	/*B Par defaut, retourner une ligne vide */
	strcpy (vl_Ligne_PMV, vm_Ligne_Vide);


	/*A
	** Verifier la validite des parametres d'entree
	*/

	if ( (pa_Destination == NULL)  ||  (va_Tdp <= 0) )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "%s: destination ou valeur TdP invalide", version);
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
		sprintf (vl_txt_tdp, " %1dH%02d", va_Tdp / 60, va_Tdp % 60);
	}
	else
	{
		/* sinon l'afficher en minutes */
		sprintf (vl_txt_tdp, " %dMN", va_Tdp);
	}


	/*A
	** Constituer la ligne a afficher avec la destination de longueur bornee
	** et le texte de Temps de Parcours cadre a droite
	*/

	vl_lgr_max_dest = CM_LGR_LIGNE_PANNEAUX - strlen (vl_txt_tdp);

	vl_lgr_dest = strlen (pa_Destination);
	if (vl_lgr_dest > vl_lgr_max_dest)
	{
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
*   Met en majuscules la cha�ne fournie en entr�e.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static char *aneu_ligne_majuscules (char *va_Texte)

/*
-------------------------------------------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   Le texte en majuscules.
*
* CODE RETOUR :
*   Le texte en majuscules.
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
-----------------------------------------------------------------------------------------*/

{
	int	i;

	for (i = 0;  i < strlen (va_Texte);  i++) {
		va_Texte[i] = toupper ((int) va_Texte[i]);
	}

	return (va_Texte);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise au neutre des PMV en etat de fonctionnement.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aneu_PMV_Au_Neutre ( )

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
*   Pour chaque PMV qui n'est pas en alarme majeure ou hors service,
*   calculer les temps de parcours a afficher s'il y a lieu
    puis executer la commande de mise au neutre.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_PMV_Au_Neutre" ;

	XZADT_Temps_De_Parcours_PMV	vl_TdP_PMV;

	XDY_Eqt		vl_Eqt = 0;
	XDY_Ligne_PMV	vl_Ligne1, vl_Ligne2, vl_Ligne3;
	XDY_Octet	vl_Fin_Action = XDC_FAUX;
	XDY_Entier	vl_Num_Action = 0;
	XDY_Octet	vl_Type_Neutre = 0;
  	XDY_Octet	vl_inhb = 0;

	int		vl_i;
	int		vl_cr;

	/*A Pour chaque PMV du site */
	for ( vl_i = 0;  vl_i < vg_Nbr_PMV;  vl_i++ )
	{
		/* si l'equipement est en etat de fonctionnement alors */
		if ( (tg_Liste_PMV[vl_i].EtatEqt & XDC_EQT_MINEUR) == tg_Liste_PMV[vl_i].EtatEqt)
		{
			vl_Eqt = tg_Liste_PMV[vl_i].NumeroEqt;

			/* Test pour savoir si ce PVM est inhibe */
			xzae59_Rechercher_EQT_TDP_INHIB(vl_Eqt,XDC_EQT_PMV,&vl_inhb);
			if (vl_inhb)
			{
				XZST_03EcritureTrace(XZSTC_INFO,"aneu_PMV_Au_Neutre: TDP PMV %d inhibe, on retourne -1",vl_Eqt);
				vl_TdP_PMV.TDP_Proche = -1;
				vl_TdP_PMV.TDP_Loin = -1;
				vl_TdP_PMV.TDP_Noeud = -1;
			}
			else
			{
				/* Calculer les donnees de Temps de Parcours affichables */
				vl_cr = XZAD103_Temps_De_Parcours_PMV (vl_Eqt, &vl_TdP_PMV);

				strcpy (vl_Ligne1, vm_Ligne_Vide);
				strcpy (vl_Ligne2, vm_Ligne_Vide);
				strcpy (vl_Ligne3, vm_Ligne_Vide);

				if ( (vl_TdP_PMV.Dest_Proche.Numero != XDC_DTP_AUCUNE)  &&  (vl_TdP_PMV.TDP_Proche > 0) )
				{
					strcpy (vl_Ligne1, aneu_Ligne_TDP (vl_TdP_PMV.Dest_Proche.Libelle_PMV, vl_TdP_PMV.TDP_Proche));
				}

				if ( (vl_TdP_PMV.Dest_Loin.Numero != XDC_DTP_AUCUNE)  &&  (vl_TdP_PMV.TDP_Loin > 0) )
				{
					strcpy (vl_Ligne2, aneu_Ligne_TDP (vl_TdP_PMV.Dest_Loin.Libelle_PMV, vl_TdP_PMV.TDP_Loin));
				}

				if ( (vl_TdP_PMV.Dest_Noeud.Numero != XDC_DTP_AUCUNE)  &&  (vl_TdP_PMV.TDP_Noeud > 0) )
				{
					strcpy (vl_Ligne3, aneu_Ligne_TDP (vl_TdP_PMV.Dest_Noeud.Libelle_PMV, vl_TdP_PMV.TDP_Noeud));
				}


				/* Mettre en majuscules le texte du message */
				aneu_ligne_majuscules (vl_Ligne1);
				aneu_ligne_majuscules (vl_Ligne2);
				aneu_ligne_majuscules (vl_Ligne3);


				XZST_03EcritureTrace (XZSTC_INFO, "PMV %s au neutre: Dest TdP %d/%d/%d //%s/%s/%s//",
										tg_Liste_PMV[vl_i].NomEqt,
										vl_TdP_PMV.Dest_Proche.Numero,
										vl_TdP_PMV.Dest_Loin.Numero,
										vl_TdP_PMV.Dest_Noeud.Numero,
										vl_Ligne1, vl_Ligne2, vl_Ligne3);

				if ( (vl_Eqt != vg_pmv_ignore)  &&  ((vg_pmv_gere == 0) || (vg_pmv_gere == vl_Eqt))  &&
					(vg_pmva_gere == 0) )				/* ignorer les PMV si seul un PMVA est g�r� */
				{
				/* Tenter la mise au neutre du panneau si possible */
				vl_cr = XZAC610_Mise_Au_Neutre_PMV (vl_Eqt,
													vg_ConfigNeutr.NumeroSite,
													vl_Fin_Action,
													vl_Ligne1,
													vl_Ligne2,
													vl_Ligne3,
													(XDY_Mot) vl_TdP_PMV.Dest_Proche.Numero,
													(XDY_Mot) vl_TdP_PMV.Dest_Loin.Numero,
													(XDY_Mot) vl_TdP_PMV.Dest_Noeud.Numero,
													&vl_Num_Action,
													&vl_Type_Neutre,1);
				XZST_03EcritureTrace (XZSTC_INFO, "Mise au neutre: CR=%d, Action en cours=%d, Type neutre=%d",
										(int) vl_cr, (int) vl_Num_Action, (int) vl_Type_Neutre);
				}
			}
		}
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Mise au neutre des PMVA en etat de fonctionnement.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aneu_PMVA_Au_Neutre ()

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
*   Pour chaque PMVA qui n'est pas en alarme majeure ou hors service,
*   calculer les temps de parcours a afficher s'il y a lieu
    puis executer la commande de mise au neutre.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_PMVA_Au_Neutre" ;

	XZADT_Temps_De_Parcours_PMVA	vl_TdP_PMVA;

	XDY_Eqt		vl_Eqt = 0;
	XDY_Octet	vl_Fin_Action = XDC_FAUX;
	XDY_Entier	vl_Num_Action = 0;
	XDY_Octet	vl_Type_Neutre = 0;
  	XDY_Octet	vl_inhb = 0;

	XDY_Destination	*tl_dests[] = { 
		&vl_TdP_PMVA.Dest1_Proche,
		&vl_TdP_PMVA.Dest1_Loin,
		&vl_TdP_PMVA.Dest2_Proche,
		&vl_TdP_PMVA.Dest2_Loin
	};

	XDY_TDP		*ll_TdP[] = { 
		&vl_TdP_PMVA.TDP1_Proche,
		&vl_TdP_PMVA.TDP1_Loin,
		&vl_TdP_PMVA.TDP2_Proche,
		&vl_TdP_PMVA.TDP2_Loin
	};

	XDY_Ligne_PMV	ll_lignes[2 * CM_NB_LIGNES_PMVA];			/* Taille double (si 5 libelles longs configures) */

	int		vl_nb_lib_tdp_longs = 0;
	char		*pl_marqueur;
	char		*pl_libelle;
	int		vl_lig;
	int		vl_i, vl_k;
	int		vl_cr;

	/*A Pour chaque PMVA du site */
	for ( vl_i = 0;  vl_i < vg_Nb_PMVA;  vl_i++ )
	{
		/* si l'equipement est en etat de fonctionnement alors */
		if ( (tg_ListePMVA[vl_i].EtatEqt & XDC_EQT_MINEUR) == tg_ListePMVA[vl_i].EtatEqt)
		{
			vl_Eqt = tg_ListePMVA[vl_i].NumeroEqt;

			/* Test pour savoir si ce PVM est inhibe */
			xzae59_Rechercher_EQT_TDP_INHIB(vl_Eqt,XDC_EQT_PMVA,&vl_inhb);
			if (vl_inhb)
			{
				XZST_03EcritureTrace(XZSTC_INFO,"aneu_PMVA_Au_Neutre: TDP PMVA %d inhibe, on retourne -1",vl_Eqt);
				vl_TdP_PMVA.TDP1_Proche = -1;
				vl_TdP_PMVA.TDP1_Loin = -1;
				vl_TdP_PMVA.TDP2_Proche = -1;
				vl_TdP_PMVA.TDP2_Loin = -1;
			}
			else
			{
				/* Calculer les donnees de Temps de Parcours affichables */
				vl_cr = XZAD104_Temps_De_Parcours_PMVA (vl_Eqt, &vl_TdP_PMVA);

				for (vl_k = 0;  vl_k < CM_NB_LIGNES_PMVA;  vl_k++) {
					strcpy (ll_lignes[vl_k], vm_Ligne_Vide);
				}

				/* Rechercher les destinations de Temps de Parcours longues, a decouper */
				vl_nb_lib_tdp_longs = 0;
				for (vl_k = 0;  vl_k < CM_NB_DEST_PMVA;  vl_k++) {
					if ( (tl_dests[vl_k]->Numero != XDC_DTP_AUCUNE)  &&
						strstr (tl_dests[vl_k]->Libelle_PMV, XZADC_LIB_TDP_MRQ_COUP) != NULL )
					{
						vl_nb_lib_tdp_longs = vl_nb_lib_tdp_longs + 1;
					}
				}

				vl_lig = 0;
				for (vl_k = 0;  vl_k < CM_NB_DEST_PMVA;  vl_k++, vl_lig++) {
					pl_libelle = tl_dests[vl_k]->Libelle_PMV;
					pl_marqueur = strstr (pl_libelle, XZADC_LIB_TDP_MRQ_COUP);

					/* s'il y a un temps de parcours a afficher alors */
					if ( (tl_dests[vl_k]->Numero != XDC_DTP_AUCUNE)  &&  (*(ll_TdP[vl_k]) > 0) ) {
						/* si le marqueur de cesure est trouve alors decouper le libelle sur deux lignes */
						if (pl_marqueur != NULL) {
							strncpy (ll_lignes[vl_lig], pl_libelle, (size_t) (pl_marqueur - pl_libelle));
							vl_lig = vl_lig + 1;
							pl_marqueur = pl_marqueur + strlen (XZADC_LIB_TDP_MRQ_COUP);
							strcpy (ll_lignes[vl_lig], aneu_Ligne_TDP (pl_marqueur, *(ll_TdP[vl_k])));
						} else {
							strcpy (ll_lignes[vl_lig], aneu_Ligne_TDP (pl_libelle, *(ll_TdP[vl_k])));
						}
					} else {
						/* en l'absence de temps de parcours, occuper deux lignes si le libelle l'indique */
						if (pl_marqueur != NULL) {
							vl_lig = vl_lig + 1;
						}
					}

					/* si aucune destination n'est a decouper, sauter la 3e ligne apres la 2e destination */
					if ( (vl_nb_lib_tdp_longs == 0)  &&  (vl_k == 1) ) {
						vl_lig = vl_lig + 1;
					}
				}

				/* Mettre en majuscules le texte du message */
				aneu_ligne_majuscules (ll_lignes[0]);
				aneu_ligne_majuscules (ll_lignes[1]);
				aneu_ligne_majuscules (ll_lignes[2]);
				aneu_ligne_majuscules (ll_lignes[3]);
				aneu_ligne_majuscules (ll_lignes[4]);


				XZST_03EcritureTrace (XZSTC_INFO, "PMVA %s au neutre: Dest TdP %d/%d/%d/%d //%s/%s/%s/%s/%s//",
										tg_ListePMVA[vl_i].NomEqt,
										vl_TdP_PMVA.Dest1_Proche.Numero,
										vl_TdP_PMVA.Dest1_Loin.Numero,
										vl_TdP_PMVA.Dest2_Proche.Numero,
										vl_TdP_PMVA.Dest2_Loin.Numero,
										ll_lignes[0], ll_lignes[1], ll_lignes[2], ll_lignes[3], ll_lignes[4]);

				if ( (vl_Eqt != vg_pmva_ignore)  &&  ((vg_pmva_gere == 0) || (vg_pmva_gere == vl_Eqt))  &&
					(vg_pmv_gere == 0) )				/* ignorer les PMVA si seul un PMV est g�r� */
				{
					/* Tenter la mise au neutre du panneau si possible */
					vl_cr = XZAC840_Mise_Au_Neutre_PMVA (vl_Eqt,
														vg_ConfigNeutr.NumeroSite,
														vl_Fin_Action,
														ll_lignes[0], ll_lignes[1], ll_lignes[2], ll_lignes[3], ll_lignes[4],
														(XDY_Mot) vl_TdP_PMVA.Dest1_Proche.Numero,
														(XDY_Mot) vl_TdP_PMVA.Dest1_Loin.Numero,
														(XDY_Mot) vl_TdP_PMVA.Dest2_Proche.Numero,
														(XDY_Mot) vl_TdP_PMVA.Dest2_Loin.Numero,
														&vl_Num_Action,
														&vl_Type_Neutre,1);
					XZST_03EcritureTrace (XZSTC_INFO, "Mise au neutre: CR=%d, Action en cours=%d, Type neutre=%d",
											(int) vl_cr, (int) vl_Num_Action, (int) vl_Type_Neutre);
				}
			}
		}
	}

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

static int aneu_traitement_cyclique ()

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
*	Effectue les traitement suivants :
*	   - Consulte et met a jour l'etat des equipements PMV[A] ;
*	   - Calcule les temps de parcours pour les destinations configurees des PMV[A] ;
*	   - Commande l'affichage programme sur les equipements PMV[A] inoccupes.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cyc.c,v 1.10 2020/11/26 14:26:00 gesconf Exp $ : aneu_traitement_cyclique" ;

	int	vl_cr = XDC_OK;
	int	i = 0;


	/*A
	**        Reinitialiser l'etat des panneaux
	** (au cas ou ils ne seraient pas extraits de la base)
	*/

	for (i = 0;  i < vg_Nbr_PMV;  i++)
	{
		tg_Liste_PMV[i].EtatEqt = 0;
	}

	for (i = 0;  i < vg_Nb_PMVA;  i++)
	{
		tg_ListePMVA[i].EtatEqt = 0;
	}


	/*A
	** Lire en base et mettre a jour l'etat des equipements PMV et PMVA
	*/

	vl_cr = XZAT01_Liste_Eqt_Dispo ( (XDY_TypeEqt) XDC_EQT_PMV,
	                                    (XDY_District) vg_ConfigNeutr.NumeroSite,
	                                    (XDY_Etat_Eqt) XDC_EQT_HS,
	                                    (XDY_Etat_Eqt) XDC_EQT_NON_HS,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Horodate) XZSMC_HORODATE_NUL,
	                                    aneu_MAJ_etat_PMV );

	vl_cr = XZAT01_Liste_Eqt_Dispo ( (XDY_TypeEqt) XDC_EQT_PMVA,
	                                    (XDY_District) vg_ConfigNeutr.NumeroSite,
	                                    (XDY_Etat_Eqt) XDC_EQT_HS,
	                                    (XDY_Etat_Eqt) XDC_EQT_NON_HS,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Horodate) XZSMC_HORODATE_NUL,
	                                    aneu_MAJ_etat_PMVA );


	/*A
	** Mettre au neutre les equipements PMV et PMVA qui peuvent l'etre
	*/

	aneu_PMV_Au_Neutre ();

	aneu_PMVA_Au_Neutre ();

	return (XDC_OK);
}
