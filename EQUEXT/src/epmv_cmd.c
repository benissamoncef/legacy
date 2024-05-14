/* Fichier : epmv_cmd.c
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de traitement des commandes recues par la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Sep 1994	: Creation
* Nagiel.E	version 1.2	28 Sep 1994	: Ajout de ep_cmd_PICTO_cb
* Nagiel.E	version 1.3	10 Oct 1994	: Ajout des autres fonctions
* Mismer.D	version 1.4	23 Jan 1995	:
* Mismer.D	version 1.5	31 Jan 1995	:
* Mismer.D	version 1.6	09 Fev 1995	:
* Mismer.D	version 1.7	21 Fev 1995	:
* Mismer.D	version 1.8	08 Mar 1995	:
* Mismer.D	version 1.9	17 Mar 1995	:
* Mismer.D 	version 1.10	09 Avr 1995	:
* Volcic.F	version 1.11	19 Avr 1995	:
* Volcic.F	version 1.12	24 Avr 1995	:
* Volcic.F	version 1.13	24 Avr 1995	:
* Volcic.F	version 1.14	26 Avr 1995	:
* Volcic.F	version 1.15	27 Avr 1995 	: Suppr Tempo pour PMV SES
* Volcic.F	version 1.16	02 Mai 1995	: Modif reception cmd picto
* Mismer.D	version 1.17	03 Mai 1995	: Ajout Abt etat liaison
* Mismer.D	version 1.18	11 Mai 1995	: Ajout fonction de reconnexion aux taches protocoles
* Fontaine.C	version 1.19	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.20	30 Mai 1995	: Modif pour affichage par defaut en fichier
* Mismer.D	version 1.21	16 Aou 1995	: Ajout trace fichier cmd et compte-rendu
* Volcic.F	version 1.22	25 Sep 1995	: Modif envoi PMV Heure/Temp/Eteint
* Mismer.D	version 1.23	01 Dec 1995	: Correction pour affichage par defaut en fichier
* Mismer	version 1.24    29 aut 1996	: modif des messages XDM_ECMD_PMV, Picto (num action sur INT4) DEM/1172
* Mismer.D      version 1.25    29 Aou 1996     : Ajout ligne direction pour PMV 4 lignes (DEM/1162)
* Mismer.D      version 1.26    13 Sep 1996     : Modif pour eclairage et periode (RADT_W)
* Mismer.D	version 1.27	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.28	10 Mar 1998	: Modif pour PMV PICTO avec la meme RGS (DEM/1618)
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	06/07/10	: correction cligno (linux) DEM934
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.32
* JPL	18/03/11 : Migration HP ia64 (DEM 975) : Bug : ajout arg. site gestion a ep_envoi_affichage_{PMV,Picto}  1.33
* JMG	04/07/12 : regulation 1.34
* PNI   25/03/13 : Mise a zero de la zone mem cmd + suppression des blancs dans l'alternat 1.35 DEM 1061
* PNI	28/08/13 : suppression de l'envoi du contraste dans le cas NOCMD 1.36 DEM1081
* PNI	xx/08/15 : Eviter l'envoi d'une chaine vide par l'usage de la constante dediee  1.38
* JPL	09/11/15 : Sur commande Neutre 3 POINTS, memoriser une commande TEXTE (retour reconnu par les IHM) 1.39
* PNI	02/08/17 : Prise en compte des PMV avec flash et correctif sur clignotement DEM1233 1.40
* JMG	02/09/18 : PMV IP suppression SAGA DEM1306 1.41
* JMG	15/03/19 : correction clignotement IP 1.42
* LCL	10/05/19 : correction modules LCR PMV01850 1.43
* GR    10/07/20 : EMS 1.44
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* JPL	08/11/22 : Suppression des commandes de luminosite (contraste) non manuelles  SAE-429
* JPL	15/11/22 : Commande mode Auto sur demande Normal; fonction MAJ contraste tous eqts. pilotes  SAE-429
* JPL	29/11/22 : Restauration demande d'etat rattachee a la commande de contraste Picto 
* GGY   15/06/23    :   Ajout EnvoyerTrameIp et ex_ret_LCR_IP (DEM472)
* GGY	02/05/23 : Suppression du force du Flash à XDC_PMV_PASFLASH (DEM-473)
* GGY	06/12/23 : correction flash pour mise au neutre (DEM473)
* JPL	08/09/23 : Suppression des commandes de contraste accompagnant les commandes PMV et Picto (SAE-498)
-----------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

#include "epmv_cmd.h"
#include "epmv_cfg.h"
#include "epmv_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "epmv_cmd.c 1.49";

/* declaration de fonctions internes */
int em_demander_etat_pmv_picto (int, int, XDY_Rgs, XDY_AdresseIP, int, int);
int em_demander_etat_picto (EPMV_DONNEES_Picto *, int);
int em_envoyer_contraste_pmv(EPMV_DONNEES_PMV *, int, int);
int em_envoyer_contraste_picto(EPMV_DONNEES_Picto *, int, int, int);

void supprime_blancs_fin(char *);
static char *epmv_majuscules(char *);

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_Init_IHM_contraste_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_Contraste
*
* FONCTION
*  Pour touts les PMV et picto , envoyer l'etat  par XDM_ETAT_Contraste dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_STR pl_DGInit;
#else
	XDY_Datagroup pl_DGInit;
#endif

	EPMV_DONNEES_PMV *pl_listepmv;
	EPMV_DONNEES_Picto *pl_listepicto;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_contraste_cb ");

	/*A Recuperation des parametres du message XDM_EInit_IHM_Contraste */
	if (!TipcMsgRead(pa_Data->msg,
					 T_IPC_FT_STR, &pl_DGInit,
					 NULL))
	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Contraste non lu");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_contraste_cb");
		return;
	}

	/*A Pour chaque picto : envoi du msg XDM_ETAT_Contraste si il est en service et actif */

	for (pl_listepicto = pg_debutPicto; pl_listepicto != NULL; pl_listepicto = pl_listepicto->Suivant)
	{
		/* si l'eqt est en service */
		if (!(pl_listepicto->Etat_Service & XDC_EQT_HORS_SRV))
		{
			if (!TipcSrvMsgWrite(pl_DGInit,
								 XDM_IdentMsg(XDM_ETAT_Contraste),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_INT2, XDC_EQT_PCT,
								 T_IPC_FT_INT2, pl_listepicto->Config.Numero.NoEqt,
								 T_IPC_FT_INT2, pl_listepicto->Contraste,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste non effectue.");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_Contraste effectue .");
			}
		}
	}

	/*A Pour chaque pmv : envoi du msg XDM_ETAT_Contraste si il est en service et actif */

	for (pl_listepmv = pg_debutPMV; pl_listepmv != NULL; pl_listepmv = pl_listepmv->Suivant)
	{
		/* si l'eqt est en service et actif */
		if (!(pl_listepmv->Etat_Service & XDC_EQT_HORS_SRV))
		{
			if (!TipcSrvMsgWrite(pl_DGInit,
								 XDM_IdentMsg(XDM_ETAT_Contraste),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_INT2, XDC_EQT_PMV,
								 T_IPC_FT_INT2, pl_listepmv->Config.Numero.NoEqt,
								 T_IPC_FT_INT2, pl_listepmv->Contraste,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste non effectue.");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_Contraste effectue.");
			}
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_contraste_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM Liaison.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_Init_IHM_Liaison_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_Liaison
*
* FONCTION
*  Pour touts les PMV et picto , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_STR pl_DGInit;
#else
	XDY_Datagroup pl_DGInit;
#endif

	EPMV_DONNEES_PMV *pl_listepmv;
	EPMV_DONNEES_Picto *pl_listepicto;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");

	/*A Recuperation des parametres du message  */
	if (!TipcMsgRead(pa_Data->msg,
					 T_IPC_FT_STR, &pl_DGInit,
					 NULL))
	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Liaison non lu \n");
		XZST_10ArchiverMsgSyst(0, "ep_Init_IHM_contraste_cb : Message XDM_EInit_IHM_Liaison recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb");
		return;
	}

	/*A Pour chaque picto : envoi du msg XDM_ETAT_LIAISON  */

	for (pl_listepicto = pg_debutPicto; pl_listepicto != NULL; pl_listepicto = pl_listepicto->Suivant)
	{
		ex_env_etat_liaison(vg_NomMachine, pl_DGInit, pl_listepicto->Config.Numero.NoEqt, XDC_EQT_PCT);
	}

	/*A Pour chaque pmv : envoi du msg XDM_ETAT_LIAISON */

	for (pl_listepmv = pg_debutPMV; pl_listepmv != NULL; pl_listepmv = pl_listepmv->Suivant)
	{
		ex_env_etat_liaison(vg_NomMachine, pl_DGInit, pl_listepmv->Config.Numero.NoEqt, XDC_EQT_PMV);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_Init_IHM_PMV_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_PMV
*
* FONCTION
*  Pour touts les PMV, envoyer l'etat PMV par XDM_ETAT_PMV dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_STR pl_DGInit;
#else
	XDY_Datagroup pl_DGInit;
#endif

	EPMV_DONNEES_PMV *pl_DonCrt;
	EPMV_DONNEES_TYPE_PMV *pl_typepmv;
	int vl_NbLigne = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_PMV_cb ");

	/*A Recuperation des parametres du message XDM_EInit_IHM_PMV */
	if (!TipcMsgRead(pa_Data->msg,
					 T_IPC_FT_STR, &pl_DGInit,
					 NULL))
	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_PMV non lu.");
		XZST_10ArchiverMsgSyst(0, "ep_Init_IHM_PMV_cb : Message XDM_EInit_IHM_PMV recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PMV_cb");
		return;
	}

	/*A Pour chaque pmv : envoi du msg XDM_ETAT_PMV */

	for (pl_DonCrt = pg_debutPMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
	{
		/* si l'eqt est en service et actif */
		if (!(pl_DonCrt->Etat_Service & XDC_EQT_HORS_SRV))
		{
			/*A Recherche du type de pmv */
			for (pl_typepmv = pg_debutTypePMV; pl_typepmv != NULL; pl_typepmv = pl_typepmv->Suivant)
			{
				if (pl_DonCrt->Config.TypePMV == pl_typepmv->TypePMV)
				{
					vl_NbLigne = pl_typepmv->NbLigne;
					break;
				}
			}

			if (!TipcSrvMsgWrite(pl_DGInit,
								 XDM_IdentMsg(XDM_ETAT_PMV),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_INT2, pl_DonCrt->Config.Numero.NoEqt,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Texte1,
								 T_IPC_FT_INT2, pl_DonCrt->Etat.Affichage1,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Alternat1,
								 T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign1,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Texte2,
								 T_IPC_FT_INT2, (vl_NbLigne >= 2) ? pl_DonCrt->Etat.Affichage2 : XDC_PMV_NON_EXIST,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Alternat2,
								 T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign2,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Texte3,
								 T_IPC_FT_INT2, (vl_NbLigne >= 3) ? pl_DonCrt->Etat.Affichage3 : XDC_PMV_NON_EXIST,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Alternat3,
								 T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign3,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Texte4,
								 T_IPC_FT_INT2, (vl_NbLigne == 4) ? pl_DonCrt->Etat.Affichage4 : XDC_PMV_NON_EXIST,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Alternat4,
								 T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign4,
								 T_IPC_FT_CHAR, pl_DonCrt->Etat.Flash,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PMV non effectue.");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PMV effectue.");
			}
		}
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PMV_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM Picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_Init_IHM_Picto_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_Picto
*
* FONCTION
*  Pour touts les Picto, envoyer l'etat Picto par XDM_ETAT_Picto dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_STR pl_DGInit;
#else
	XDY_Datagroup pl_DGInit;
#endif

	T_INT2 vl_Type;
	T_INT2 vl_NoEqt;
	EPMV_DONNEES_Picto *pl_DonCrt;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Picto_cb ");

	/*A Recuperation des parametres du message XDM_EInit_IHM_Picto */
	if (!TipcMsgRead(pa_Data->msg,
					 T_IPC_FT_STR, &pl_DGInit,
					 NULL))
	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Picto non lu \n");
		XZST_10ArchiverMsgSyst(0, "ep_Init_IHM_Picto_cb : Message XDM_EInit_IHM_Picto recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Picto_cb");
		return;
	}

	/*A Pour chaque picto : envoi du msg XDM_ETAT_PICTO */

	for (pl_DonCrt = pg_debutPicto; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
	{
		/* si l'eqt est en service et actif */
		if (!(pl_DonCrt->Etat_Service & XDC_EQT_HORS_SRV))
		{
			if (!TipcSrvMsgWrite(pl_DGInit,
								 XDM_IdentMsg(XDM_ETAT_PICTO),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_INT2, pl_DonCrt->Config.Numero.NoEqt,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Texte,
								 T_IPC_FT_STR, pl_DonCrt->Etat.Picto,
								 T_IPC_FT_INT2, pl_DonCrt->Etat.Clign,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PICTO non effectue.");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_PICTO effectue. ");
			}
		}
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Picto_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande d'affichage picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_cmd_PICTO_cb(T_IPC_CONN va_Cnx,
					 T_IPC_CONN_PROCESS_CB_DATA data,
					 T_CB_ARG arg)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_PICTO
*
* FONCTION
*  Demande d'affichage picto
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_STR vl_Texte;
	T_STR vl_Picto;
	T_INT2 vl_NoEqt;
	T_CHAR vl_Clign;
	T_INT4 vl_NoCMD;
#else
	XDY_Texte vl_Texte;
	XDY_Picto vl_Picto;
	XDY_Mot vl_NoEqt;
	XDY_Octet vl_Clign;
	XDY_Entier vl_NoCMD;
#endif

	EPMV_DONNEES_Picto *pl_donneepicto;
	EPMV_DONNEES_PMV *pl_donneepmv;
	EPMV_DONNEES_TYPE_Picto *pl_typepicto;
	EPMV_DONNEES_TYPE_PMV *pl_typepmv;
	EPMV_LISTE_Picto *pl_listepicto;
	int vl_js;
	XDY_Horodate vl_horodate;
	XZEXT_Trame pl_mlcr = "";
	XZEXT_Trame pl_mlcr_lg1 = "";
	XZEXT_Trame pl_mlcr_lg2 = "";
	char *token_picto;
	int vl_ValRet = XDC_OK;
	int vl_EnvoiSTPS;
	int j = 0;

	/*A Lecture du message */

	if (!TipcMsgRead(data->msg,
					 T_IPC_FT_INT2, &vl_NoEqt,
					 T_IPC_FT_STR, &vl_Texte,
					 T_IPC_FT_STR, &vl_Picto,
					 T_IPC_FT_CHAR, &vl_Clign,
					 T_IPC_FT_INT4, &vl_NoCMD, NULL))

	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_PICTO non lu .");
		XZST_10ArchiverMsgSyst(0, "ep_cmd_PICTO_cb : Message XDM_CMD_PICTO recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PICTO_cb");
		return;
	}

	token_picto = strtok(vl_Picto, " ");

	if (token_picto != NULL)
		strcpy(vl_Picto, token_picto);
	else
		strcpy(vl_Picto, "");

	/*	supprime_blancs_fin(vl_Texte);*/
	/* Trace du message en entree de la fonction */
	XZST_03EcritureTrace(XZSTC_INFO, "IN callback ep_cmd_PICTO, msg= %d.%s.%s.%d.%d.",
						 vl_NoEqt,
						 vl_Texte,
						 vl_Picto,
						 vl_Clign,
						 vl_NoCMD);

	/*A Lecture vl_horodate en cas d'erreur */

	XZSM_07LireHorodate(XZSMC_TYPE_RTWORKS, &vl_js, &vl_horodate);

	/*A Recherche du No d'equipement dans la structure config_picto en memoire */
	for (pl_donneepicto = pg_debutPicto; pl_donneepicto != NULL;
		 pl_donneepicto = pl_donneepicto->Suivant)
	{
		/* Equipement trouve */
		XZST_03EcritureTrace(XZSTC_WARNING, "PICTO=%d", pl_donneepicto->Config.Numero.NoEqt);
		if (pl_donneepicto->Config.Numero.NoEqt == vl_NoEqt)
			break;
	}

	for (pl_donneepmv = pg_debutPMV; pl_donneepmv != NULL;
		 pl_donneepmv = pl_donneepmv->Suivant)
	{
		if (pl_donneepmv->Config.Numero.NoEqt == vl_NoEqt)
		{
			break;
		}
	}
	if (pl_donneepmv != NULL)
	{
		for (pl_typepmv = pg_debutTypePMV; pl_typepmv != NULL;
			 pl_typepmv = pl_typepmv->Suivant)
		{
			if (pl_donneepmv->Config.TypePMV == pl_typepmv->TypePMV)
				break;
		}
	}

	/* No d'equipement introuvable */
	if (pl_donneepicto == NULL)
	{
		if (pl_typepmv != NULL)
		{
			if (pl_typepmv->Picto == 0)
			{
				XZST_03EcritureTrace(XZSTC_FONCTION, "PMV sans picto");
				return;
			}
		}
		/* Trace du message */
		XZST_03EcritureTrace(XZSTC_WARNING, "Msg ep_cmd_PICTO incorrect (erreur no equipement), msg= %d %s %s %d %d \n",
							 vl_NoEqt,
							 vl_Texte,
							 vl_Picto,
							 vl_Clign,
							 vl_NoCMD);

		/* Trace : erreur equipement introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PICTO_cb : erreur equipement");
		/* Envoi du CR NOK a la base */
		/*if (vl_NoCMD != 0) xzat25_CompteRendu_PMV(vl_NoCMD,vl_horodate,XDC_NOK,0);*/
		return;
	}

	/* Si equipement HS ou Inactif -> pas d'envoi au picto */
	if (((pl_donneepicto->Etat_Service & XDC_EQT_HORS_SRV) != 0) ||
		(((pl_donneepicto->Etat_Service & XDC_EQT_INACTIF) != 0) && (vl_NoCMD != 0)) ||
		(((pl_donneepicto->Etat_Service & XDC_EQT_INHIBE) != 0) && (vl_NoCMD != 0)))
	{
		/* Trace du message */
		XZST_03EcritureTrace(XZSTC_WARNING, "Msg ep_cmd_PICTO incorrect (equipement hs ou inactif), msg= %d %s %s %d %d \n",
							 vl_NoEqt,
							 vl_Texte,
							 vl_Picto,
							 vl_Clign,
							 vl_NoCMD);

		/* Trace : erreur equipement introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PICTO_cb : erreur hs ou inactif");
		/* Envoi du CR NOK a la base */
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepicto->Config.SiteGestion);
		return;
	}

	/*A Picto valide ? */

	/* Recherche du type de picto */
	for (pl_typepicto = pg_debutTypePicto; pl_typepicto != NULL;
		 pl_typepicto = pl_typepicto->Suivant)
	{
		if (pl_donneepicto->Config.TypePicto == pl_typepicto->TypePicto)
			break;
	}

	/* Type picto introuvable */
	if (pl_typepicto == NULL)
	{
		/* Trace : erreur typepicto introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PICTO_cb : erreur typepicto");
		/* Envoi du CR NOK a la base */
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepicto->Config.SiteGestion);
		return;
	}

	/*A Si clignotement invalide -> envoi du CR NOK a la base */
	if ((vl_Clign != XDC_PICTO_PASCLIGNO) && (vl_Clign != XDC_PICTO_CLIGNO))
	{
		/* Trace du message */
		XZST_03EcritureTrace(XZSTC_WARNING, "Msg ep_cmd_PICTO incorrect (erreur clignotement), msg= %d %s %s %d %d \n",
							 vl_NoEqt,
							 vl_Texte,
							 vl_Picto,
							 vl_Clign,
							 vl_NoCMD);

		/* Trace : erreur clignotement */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PICTO_cb : erreur clignotement");
		/* Envoi du CR NOK a la base */
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepicto->Config.SiteGestion);
		return;
	}

	XZST_03EcritureTrace(XZSTC_WARNING, "Panonceau avant tronque = <<%s>> \t nbcar =%d \n", vl_Texte, pl_typepicto->NbCaracteres);

	/*A on supprime les blanc dans le message */
	XDY_Supprimer_Espaces (vl_Texte);

	/* On tronque le texte-picto si celui-ci est trop grand */
	XZST_03EcritureTrace(XZSTC_WARNING, "Panonceau avant tronque = <<%s>> \t nbcar =%d \n", vl_Texte, pl_typepicto->NbCaracteres);
	if (strlen(vl_Texte) > pl_typepicto->NbCaracteres)
		vl_Texte[pl_typepicto->NbCaracteres] = '\0';

	XZST_03EcritureTrace(XZSTC_WARNING, "Panonceau apres tronque = <<%s>> \t nbcar =%d \n", vl_Texte, pl_typepicto->NbCaracteres);

	/*A Composition de la trame LCR d'affichage du picto*/
	if (pl_typepmv->TypePMV == 7)
		sprintf(pl_mlcr_lg1, "AM=6.0 AF=%s %s DV=0",
				(!strcmp(vl_Picto, XDC_PICTO_PASPICTO)) ? "0" : vl_Picto,
				(vl_Clign == XDC_PICTO_PASCLIGNO) ? "CL=0" : vg_clignolent);
	else
		sprintf(pl_mlcr_lg1, "AM=6 AF=%s %s DV=0",
				(!strcmp(vl_Picto, XDC_PICTO_PASPICTO)) ? "0" : vl_Picto,
				(vl_Clign == XDC_PICTO_PASCLIGNO) ? "CL=0" : vg_clignolent);

	/*A Composition de la trame LCR d'affichage du texte sous le picto*/

	if (pl_typepmv->TypePMV == 7)
		sprintf(pl_mlcr_lg2, "AM=4.0 AF=\"%s\" %s DV=0",
				vl_Texte,
				(vl_Clign == XDC_PICTO_PASCLIGNO) ? "CL=0" : vg_clignolent);
	else
		sprintf(pl_mlcr_lg2, "AM=7 AF=\"%s\" %s DV=0",
				vl_Texte,
				(vl_Clign == XDC_PICTO_PASCLIGNO) ? "CL=0" : vg_clignolent);

	if (pl_typepmv->TypePMV == 7)
		sprintf(pl_mlcr, "PA %s%s %s", pl_donneepicto->Config.IdLCR, pl_mlcr_lg1, pl_mlcr_lg2);
	else
		sprintf(pl_mlcr, "P %s%s %s", pl_donneepicto->Config.IdLCR, pl_mlcr_lg1, pl_mlcr_lg2);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PICTO_cb = trame : %s", pl_mlcr);

	/* Ecriture dans fichier Trace_CMD_PMV de la commande */
	ex_ecriture_trace_cmd(XDC_EQT_PCT,
						  vl_NoEqt,
						  pl_mlcr,
						  NULL,
						  vl_NoCMD);

	/*A Envoyer trame a la tache TELMi */
	if (!vg_Simulation)
	{
		if (!strcmp(pl_donneepicto->Config.AdresseIP, XZECC_IP_VIDE))
		{
			EnvoyerTrame(pl_mlcr,
						 XZEXC_TRAME_AFF,
						 pl_donneepicto->Config.AdresseRGS,
						 XZEXC_NOCMD,
						 XZEXC_PRIORITEFAIBLE,
						 pl_donneepicto->Socket);
		}
		else
		{
			EnvoyerTrameIP(pl_mlcr,
						   XZEXC_TRAME_AFF,
						   pl_donneepicto->Config.AdresseIP,
						   pl_donneepicto->Config.Port,
						   pl_donneepicto->Config.AdresseRGS,
						   XZEXC_NOCMD,
						   XZEXC_PRIORITEFAIBLE,
						   pl_donneepicto->Socket);
		}
	}

	/*A Demande de relecture pour controle de la bonne execution et envoi contraste */
	pl_donneepicto->Etat_Commande.Clign = vl_Clign;
	pl_donneepicto->Etat_Commande.NoCMD = vl_NoCMD;

	if (vl_NoCMD != XZEXC_NOCMD)
	{
		/* NOCMD alors pas d'envoi de cmd contraste */
		/* NOCMD alors pas de demande d'etat (PS) et donc pas de risque d'incoherence entre derniere commande envoyee et retour PS tardif */
		   /* On envoie ST/PS uniquement si la commande PMV est deja passee */
		vl_EnvoiSTPS = (pl_donneepmv->NumDerCmd == pl_donneepicto->Etat_Commande.NoCMD) ? XDC_VRAI : XDC_FAUX;

		if (vl_EnvoiSTPS == XDC_VRAI)
		{
			vl_ValRet = em_demander_etat_picto (pl_donneepicto, (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE);
			if (vl_ValRet == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la demande d'etat Picto");
				if (vl_NoCMD != 0)
					xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepicto->Config.SiteGestion);
				XZST_10ArchiverMsgSyst(0, " ep_cmd_PICTO_cb: Impossible d'envoyer la demande d'etat Picto", NULL);
			}
		}

		if (vl_ValRet == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PICTO_cb");
			return;
		}
	}

	memset(&pl_donneepicto->Etat_Commande, 0, sizeof(XDY_Etat_Picto));

	/* Svg de l'etat commande */
	pl_donneepicto->Etat_Commande.Numero = vl_NoEqt;
	pl_donneepicto->NumDerCmd = (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE;
	/* Initialisation zone totale memoire de la variable (12 caracteres alors que texte peut avoir moins de caracteres) */
	strcpy(pl_donneepicto->Etat_Commande.Texte,"            ");
	strcpy(pl_donneepicto->Etat_Commande.Texte, vl_Texte);
	/* Initialisation zone totale memoire de la variable (10 caracteres alors que picto peut avoir moins de caracteres) */
	strcpy(pl_donneepicto->Etat_Commande.Picto,"         ");
	strcpy(pl_donneepicto->Etat_Commande.Picto, vl_Picto);
	pl_donneepicto->Etat_Commande.Clign = vl_Clign;
	pl_donneepicto->Etat_Commande.NoCMD = vl_NoCMD;
	XZST_03EcritureTrace(XZSTC_WARNING, "picto:%d;vl_NoCMD=%d;pl_donneepicto->NumDerCmd=%d;pl_donneepicto->Etat_Commande.NoCMD=%d;pl_donneepicto->Etat_Commande.Clig=%d;", vl_NoEqt, vl_NoCMD, pl_donneepicto->NumDerCmd, pl_donneepicto->Etat_Commande.NoCMD, pl_donneepicto->Etat_Commande.Clign);
	if (vg_Simulation)
	{
		memcpy(&pl_donneepicto->Etat, &pl_donneepicto->Etat_Commande, sizeof(XDY_Etat_Picto));
		ep_envoi_affichage_Picto(pl_donneepicto->Etat, vl_NoEqt, pl_donneepicto->Config.SiteGestion);
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_OK, pl_donneepicto->Config.SiteGestion);
	}

	/* Fin du callback */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PICTO_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande affichage PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_cmd_PMV_cb(T_IPC_CONN va_Cnx,
				   T_IPC_CONN_PROCESS_CB_DATA data,
				   T_CB_ARG arg)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_PMV
*
* FONCTION
*  Demande d'affichage PMV
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT2 vl_Numero;
	T_STR vl_RecTexte1;
	T_STR vl_RecTexte2;
	T_STR vl_RecTexte3;
	T_STR vl_RecTexte4;
	T_STR vl_RecAlternat1;
	T_STR vl_RecAlternat2;
	T_STR vl_RecAlternat3;
	T_STR vl_RecAlternat4;
	T_CHAR vl_Clign1;
	T_CHAR vl_Clign2;
	T_CHAR vl_Clign3;
	T_CHAR vl_Clign4;
	T_INT2 vl_Affichage1;
	T_INT2 vl_Affichage2;
	T_INT2 vl_Affichage3;
	T_INT2 vl_Affichage4;
	T_CHAR vl_Flash;
	T_INT4 vl_NoCMD;
#else
	XDY_Mot vl_Numero;
	XDY_Ligne_PMV vl_RecTexte1;
	XDY_Ligne_PMV vl_RecTexte2;
	XDY_Ligne_PMV vl_RecTexte3;
	XDY_Ligne_PMV vl_RecTexte4;
	XDY_Ligne_PMV vl_RecAlternat1;
	XDY_Ligne_PMV vl_RecAlternat2;
	XDY_Ligne_PMV vl_RecAlternat3;
	XDY_Ligne_PMV vl_RecAlternat4;
	XDY_Octet vl_Clign1;
	XDY_Octet vl_Clign2;
	XDY_Octet vl_Clign3;
	XDY_Octet vl_Clign4;
	XDY_Mot vl_Affichage1;
	XDY_Mot vl_Affichage2;
	XDY_Mot vl_Affichage3;
	XDY_Mot vl_Affichage4;
	XDY_Octet vl_Flash;
	XDY_Entier vl_NoCMD;
#endif

	XDY_Ligne_PMV vl_Alternat1;
	XDY_Ligne_PMV vl_Alternat2;
	XDY_Ligne_PMV vl_Alternat3;
	XDY_Ligne_PMV vl_Alternat4;
	XDY_Ligne_PMV vl_Texte1 = XDC_CHAINE_VIDE;
	XDY_Ligne_PMV vl_Texte2 = XDC_CHAINE_VIDE;
	XDY_Ligne_PMV vl_Texte3 = XDC_CHAINE_VIDE;
	XDY_Ligne_PMV vl_Texte4 = XDC_CHAINE_VIDE;
	EPMV_DONNEES_PMV *pl_donneepmv;
	EPMV_DONNEES_TYPE_PMV *pl_typepmv;
	int vl_js;
	XDY_Horodate vl_horodate;
	XZEXT_Trame pl_mlcr = "",
				vl_compstr = "";
	int vl_TypeAff = 0;
	XZEXT_Trame pl_mlcr_lg1 = "";
	XZEXT_Trame pl_mlcr_lg2 = "";
	XZEXT_Trame pl_mlcr_lg3 = "";
	XZEXT_Trame pl_mlcr_lg4 = "";
	EPMV_DONNEES_Picto *pl_donneepicto;
	int vl_EnvoiSTPS;

	/*A Lecture du message */

	if (!TipcMsgRead(data->msg, T_IPC_FT_INT2, 
					&vl_Numero, T_IPC_FT_STR, 
					&vl_RecTexte1, T_IPC_FT_INT2, 
					&vl_Affichage1, T_IPC_FT_STR, 
					&vl_RecAlternat1, T_IPC_FT_CHAR, 
					&vl_Clign1, T_IPC_FT_STR, 
					&vl_RecTexte2, T_IPC_FT_INT2, 
					&vl_Affichage2, T_IPC_FT_STR, 
					&vl_RecAlternat2, T_IPC_FT_CHAR, 
					&vl_Clign2, T_IPC_FT_STR, 
					&vl_RecTexte3, T_IPC_FT_INT2, 
					&vl_Affichage3, T_IPC_FT_STR, 
					&vl_RecAlternat3, T_IPC_FT_CHAR, 
					&vl_Clign3, T_IPC_FT_STR, 
					&vl_RecTexte4, T_IPC_FT_INT2, 
					&vl_Affichage4, T_IPC_FT_STR, 
					&vl_RecAlternat4, T_IPC_FT_CHAR, 
					&vl_Clign4, T_IPC_FT_CHAR, 
					&vl_Flash, T_IPC_FT_INT4, 
					&vl_NoCMD, NULL))

	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_PMV non lu \n");
		XZST_10ArchiverMsgSyst(0, "ep_cmd_PMV_cb : Message XDM_CMD_PMV recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PMV_cb");
		return;
	}

	strcpy(vl_Texte1, vl_RecTexte1);
	strcpy(vl_Texte2, vl_RecTexte2);
	strcpy(vl_Texte3, vl_RecTexte3);
	strcpy(vl_Texte4, vl_RecTexte4);

	supprime_blancs_fin(vl_RecTexte1);
	if (strlen(vl_RecTexte1) == 0)
		strcpy(vl_Texte1, XDC_CHAINE_VIDE);
	supprime_blancs_fin(vl_RecTexte2);
	if (strlen(vl_RecTexte2) == 0)
		strcpy(vl_Texte2, XDC_CHAINE_VIDE);
	supprime_blancs_fin(vl_RecTexte3);
	if (strlen(vl_RecTexte3) == 0)
		strcpy(vl_Texte3, XDC_CHAINE_VIDE);
	supprime_blancs_fin(vl_RecTexte4);
	if (strlen(vl_RecTexte4) == 0)
		strcpy(vl_Texte4, "");
	strcpy(vl_Alternat1, vl_RecAlternat1);
	strcpy(vl_Alternat2, vl_RecAlternat2);
	strcpy(vl_Alternat3, vl_RecAlternat3);
	strcpy(vl_Alternat4, vl_RecAlternat4);

	supprime_blancs_fin(vl_RecAlternat1);
	if (strlen(vl_RecAlternat1) == 0)
		strcpy(vl_Alternat1, "");
	supprime_blancs_fin(vl_RecAlternat2);
	if (strlen(vl_RecAlternat2) == 0)
		strcpy(vl_Alternat2, "");
	supprime_blancs_fin(vl_RecAlternat3);
	if (strlen(vl_RecAlternat3) == 0)
		strcpy(vl_Alternat3, "");
	supprime_blancs_fin(vl_RecAlternat4);
	if (strlen(vl_RecAlternat4) == 0)
		strcpy(vl_Alternat4, "");

	XZST_03EcritureTrace(XZSTC_WARNING, "0vl_Alternat1=%s.", vl_Alternat1);
	/* Trace du message en entree de la fonction */
	XZST_03EcritureTrace(XZSTC_WARNING, "IN Callback ep_cmd_PMV_cb, msg= %d T1\"%s\" %d A1\"%s\" %c T2\"%s\" %d A2\"%s\" %c T3\"%s\" %d A3\"%s\" %c T4\"%s\" %d A4\"%s\" %c %c %d",
						 vl_Numero,
						 vl_Texte1,
						 vl_Affichage1,
						 vl_Alternat1,
						 vl_Clign1,
						 vl_Texte2,
						 vl_Affichage2,
						 vl_Alternat2,
						 vl_Clign2,
						 vl_Texte3,
						 vl_Affichage3,
						 vl_Alternat3,
						 vl_Clign3,
						 vl_Texte4,
						 vl_Affichage4,
						 vl_Alternat4,
						 vl_Clign4,
						 vl_Flash,
						 vl_NoCMD);

	/*A Lecture vl_horodate en cas d'erreur */

	XZSM_07LireHorodate(XZSMC_TYPE_RTWORKS, &vl_js, &vl_horodate);

	/*A Recherche du No d'equipement dans la structure config_pmv en memoire */
	for (pl_donneepmv = pg_debutPMV; pl_donneepmv != NULL;
		 pl_donneepmv = pl_donneepmv->Suivant)
	{
		if (pl_donneepmv->Config.Numero.NoEqt == vl_Numero)
		{
			break;
		}
	}

	/* No d'equipement introuvable */
	if (pl_donneepmv == NULL)
	{
		/* Trace du message incorrect */
		XZST_03EcritureTrace(XZSTC_WARNING, "Message XDM_ECMD_PMV incorrect (erreur equipement %d) .",
							 vl_Numero);

		/* Trace : erreur equipement introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMV_cb : erreur equipement");

		/* Envoi du CR NOK a la base */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "Appel xzat25_CompteRendu_PMV avec %d %d %d", vl_NoCMD, vl_horodate, XDC_NOK);
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, 0);
		XZEZ03_CompteRendu_TDP(vl_NoCMD, XDC_NOK, vl_Numero, vg_NomMachine);
		return;
	}

	/* Si equipement HS ou Inactif -> pas d'envoi au picto */
	if (((pl_donneepmv->Etat_Service & XDC_EQT_HORS_SRV) != 0) ||
		(((pl_donneepmv->Etat_Service & XDC_EQT_INACTIF) != 0) && (vl_NoCMD != 0)) ||
		(((pl_donneepmv->Etat_Service & XDC_EQT_INHIBE) != 0) && (vl_NoCMD != 0)))
	{
		/* Trace du message incorrect */
		XZST_03EcritureTrace(XZSTC_WARNING, "Message XDM_ECMD_PMV incorrect (equipement hs ou inactif Eqt:%d) .",
							 vl_Numero);

		/* Trace : erreur equipement introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMV_cb : equipement hs ou inactif");
		/* Envoi du CR NOK a la base */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "Appel xzat25_CompteRendu_PMV avec %d %d %d", vl_NoCMD, vl_horodate, XDC_NOK);
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepmv->Config.SiteGestion);
		XZEZ03_CompteRendu_TDP(vl_NoCMD, XDC_NOK, vl_Numero, vg_NomMachine);
		return;
	}

	/*A Recherche du type de pmv */
	for (pl_typepmv = pg_debutTypePMV; pl_typepmv != NULL;
		 pl_typepmv = pl_typepmv->Suivant)
	{
		if (pl_donneepmv->Config.TypePMV == pl_typepmv->TypePMV)
			break;
	}

	/* Type pmv introuvable */
	if (pl_typepmv == NULL)
	{
		/* Trace : erreur typepmv introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMV_cb : erreur typepmv");
		/* Envoi du CR NOK a la base */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "Appel xzat25_CompteRendu_PMV avec %d %d %d", vl_NoCMD, vl_horodate, XDC_NOK);
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepmv->Config.SiteGestion);
		XZEZ03_CompteRendu_TDP(vl_NoCMD, XDC_NOK, vl_Numero, vg_NomMachine);
		return;
	}

	if (pl_typepmv->NbLigne == 1)
	{
		vl_Affichage2 = vl_Affichage1;
		vl_Affichage3 = vl_Affichage1;
		vl_Affichage4 = vl_Affichage1;
	}
	if (pl_typepmv->NbLigne == 2)
	{
		vl_Affichage3 = vl_Affichage1;
		vl_Affichage4 = vl_Affichage1;
		vl_Clign3 = XDC_PMV_PASCLIGNO;
	}
	if (pl_typepmv->NbLigne == 3)
		vl_Affichage4 = vl_Affichage1;

	if (pl_donneepmv->Config.TypeLCR == 6)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "majuscule");
		strcpy(vl_Texte1, epmv_majuscules(vl_Texte1));
		strcpy(vl_Texte2, epmv_majuscules(vl_Texte2));
		strcpy(vl_Texte3, epmv_majuscules(vl_Texte3));
		strcpy(vl_Texte4, epmv_majuscules(vl_Texte4));
		strcpy(vl_Alternat1, epmv_majuscules(vl_Alternat1));
		strcpy(vl_Alternat2, epmv_majuscules(vl_Alternat2));
		strcpy(vl_Alternat3, epmv_majuscules(vl_Alternat3));
		strcpy(vl_Alternat4, epmv_majuscules(vl_Alternat4));
	}

	/*A Si clignotement invalide -> envoi du CR NOK a la base */
	if (((vl_Clign1 != XDC_PMV_PASCLIGNO) &&
		 (vl_Clign1 != XDC_PMV_CLIGNOLENT) &&
		 (vl_Clign1 != XDC_PMV_CLIGNORAPIDE)) ||
		((vl_Clign2 != XDC_PMV_PASCLIGNO) &&
		 (vl_Clign2 != XDC_PMV_CLIGNOLENT) &&
		 (vl_Clign2 != XDC_PMV_CLIGNORAPIDE)) ||
		((vl_Clign3 != XDC_PMV_PASCLIGNO) &&
		 (vl_Clign3 != XDC_PMV_CLIGNOLENT) &&
		 (vl_Clign3 != XDC_PMV_CLIGNORAPIDE)) ||
		((vl_Clign4 != XDC_PMV_PASCLIGNO) &&
		 (vl_Clign4 != XDC_PMV_CLIGNOLENT) &&
		 (vl_Clign4 != XDC_PMV_CLIGNORAPIDE)))
	{
		/* Trace du message incorrect */
		XZST_03EcritureTrace(XZSTC_WARNING, "Message XDM_ECMD_PMV incorrect (erreur clignotement Eqt:%d Cl1:%d Cl2:%d Cl3:%d Cl4:%d) .",
							 vl_Numero,
							 vl_Clign1,
							 vl_Clign2,
							 vl_Clign3,
							 vl_Clign4);

		/* Trace : erreur clignotement */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMV_cb : erreur clignotement");
		/* Envoi du CR NOK a la base */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "Appel xzat25_CompteRendu_PMV avec %d %d %d", vl_NoCMD, vl_horodate, XDC_NOK);
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepmv->Config.SiteGestion);
		XZEZ03_CompteRendu_TDP(vl_NoCMD, XDC_NOK, vl_Numero, vg_NomMachine);
		return;
	}

	/*A Si Type d'affichage invalide -> envoi du CR NOK a la base */
	if (((vl_Affichage1 != vl_Affichage2) ||
		 (vl_Affichage1 != vl_Affichage3) ||
		 (vl_Affichage2 != vl_Affichage3) ||
		 (vl_Affichage3 != vl_Affichage4)) ||
		((vl_Affichage1 != XDC_PMV_TEXTE) &&
		 (vl_Affichage1 != XDC_PMV_ETEINDRE) &&
		 (vl_Affichage1 != XDC_PMV_DEFAUT) &&
		 (vl_Affichage1 != XDC_PMV_TEMPERATURE) &&
		 (vl_Affichage1 != XDC_PMV_HEURE) &&
		 (vl_Affichage1 != XDC_PMV_3POINTS)) ||
		((vl_Flash != XDC_PMV_FLASH) &&
		 (vl_Flash != XDC_PMV_PASFLASH)))
	{
		/* Trace du message incorrect */
		XZST_03EcritureTrace(XZSTC_WARNING, "Message XDM_ECMD_PMV incorrect (erreur affichage Eqt:%d Af1:%d Af2:%d Af3:%d Af4:%d flash:%d) .",
							 vl_Numero,
							 vl_Affichage1,
							 vl_Affichage2,
							 vl_Affichage3,
							 vl_Affichage4,
							 vl_Flash);

		/* Trace : erreur affichage */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMV_cb : erreur affichage");
		/* Envoi du CR NOK a la base */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "Appel xzat25_CompteRendu_PMV avec %d %d %d", vl_NoCMD, vl_horodate, XDC_NOK);
		if (vl_NoCMD != 0)
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_NOK, pl_donneepmv->Config.SiteGestion);
		XZEZ03_CompteRendu_TDP(vl_NoCMD, XDC_NOK, vl_Numero, vg_NomMachine);
		return;
	}

	/* On tronque les textes-pmv et les textes-alternatif-pmv si ceux-ci sont trop grand */
	if (strlen(vl_Texte1) > pl_typepmv->NbCaracteres)
		vl_Texte1[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Texte2) > pl_typepmv->NbCaracteres)
		vl_Texte2[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Texte3) > pl_typepmv->NbCaracteres)
		vl_Texte3[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Texte4) > pl_typepmv->NbCaracteres)
		vl_Texte4[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Alternat1) > pl_typepmv->NbCaracteres)
		vl_Alternat1[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Alternat2) > pl_typepmv->NbCaracteres)
		vl_Alternat2[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Alternat3) > pl_typepmv->NbCaracteres)
		vl_Alternat3[pl_typepmv->NbCaracteres] = '\0';
	if (strlen(vl_Alternat4) > pl_typepmv->NbCaracteres)
		vl_Alternat4[pl_typepmv->NbCaracteres] = '\0';

	XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PMV_cb = x vl_Affichage1 =%d", vl_Affichage1);
	/* Si demande d'affichage de l'heure 'standard' -> envoi de la trame correspondante */
	if (vl_Affichage1 == XDC_PMV_DEFAUT)
	{
		vl_TypeAff = atoi(pl_donneepmv->AffDefaut[0]);
		XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PMV_cb = Aff:%d  Def:<%s>", vl_TypeAff, pl_donneepmv->AffDefaut[0]);
		if (vl_TypeAff == XDC_PMV_3POINTS)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_3POINTS;
		}
		else if (vl_TypeAff == XDC_PMV_HEURE)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_HEURE;
			if (pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES)
			{
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=HR1 AM=3 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			}
			else if (pl_typepmv->NbLigne == 1)
			{
				sprintf(pl_mlcr, "P %sAM=1 AF=HR1 DV=0", pl_donneepmv->Config.IdLCR);
			}
			else if (pl_typepmv->NbLigne == 2)
			{
				sprintf(pl_mlcr, "P %sAM=1 AF=HR1 AM=2 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			}
			else
			{
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=0 AM=3 AF=HR1 AM=4 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			}
		}
		else if (vl_TypeAff == XDC_PMV_ETEINDRE)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_ETEINDRE;
			sprintf(pl_mlcr, "P %sAM=* AF=0", pl_donneepmv->Config.IdLCR);
		}
		else if (vl_TypeAff == XDC_PMV_TEMPERATURE)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_TEMPERATURE;
			if (pl_typepmv->NbLigne == 1)
				sprintf(pl_mlcr, "P %sAM=1 AF=TP0 DV=0", pl_donneepmv->Config.IdLCR);
			else if (pl_typepmv->NbLigne == 2)
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=TP0 DV=0", pl_donneepmv->Config.IdLCR);
			else if (pl_typepmv->NbLigne == 3)
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=TP0 AM=3 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			else
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=0 AM=3 AF=TP0 AM=4 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
		}
		else
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_TEXTE;
			strcpy(vl_Texte1, pl_donneepmv->AffDefaut[0]);
			strcpy(vl_Texte2, pl_donneepmv->AffDefaut[1]);
			strcpy(vl_Texte3, pl_donneepmv->AffDefaut[2]);
			strcpy(vl_Texte4, pl_donneepmv->AffDefaut[3]);
			strcpy(vl_Alternat1, "");
			strcpy(vl_Alternat2, "");
			strcpy(vl_Alternat3, "");
			strcpy(vl_Alternat4, "");
		}
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PMV_cb = (vl_Affichage1 =%d", vl_Affichage1);
	if ((vl_Affichage1 == XDC_PMV_ETEINDRE) ||
		(vl_Affichage1 == XDC_PMV_TEMPERATURE) ||
		(vl_Affichage1 == XDC_PMV_HEURE))
	{
		vl_Flash = pl_donneepmv->Etat.Flash;
		vl_Clign1 = vl_Clign2 = vl_Clign3 = vl_Clign4 = XDC_PMV_PASCLIGNO;

		if (vl_Affichage1 == XDC_PMV_HEURE)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_HEURE;
			if (pl_typepmv->NbLigne == 1)
				sprintf(pl_mlcr, "P %sAM=1 AF=HR1 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			else if (pl_typepmv->NbLigne == 2)
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=HR1 DV=0", pl_donneepmv->Config.IdLCR);
			else if (pl_typepmv->NbLigne == 3)
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=HR1 AM=3 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			else
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=0 AM=3 AF=HR1 AM=4 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
		}

		if (vl_Affichage1 == XDC_PMV_ETEINDRE)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_ETEINDRE;
			sprintf(pl_mlcr, "P %sAM=* AF=0", pl_donneepmv->Config.IdLCR);
		}

		if (vl_Affichage1 == XDC_PMV_TEMPERATURE)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_TEMPERATURE;
			if (pl_typepmv->NbLigne == 1)
				sprintf(pl_mlcr, "P %sAM=1 AF=TP0 DV=0", pl_donneepmv->Config.IdLCR);
			else if (pl_typepmv->NbLigne == 2)
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=TP0 DV=0", pl_donneepmv->Config.IdLCR);
			else if (pl_typepmv->NbLigne == 3)
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=TP0 AM=3 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
			else
				sprintf(pl_mlcr, "P %sAM=1 AF=0 AM=2 AF=0 AM=3 AF=TP0 AM=4 AF=0 DV=0", pl_donneepmv->Config.IdLCR);
		}

		XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PMV_cb = trame : %s", pl_mlcr);

		/*A Envoyer trame a la tache TELMi */
		if (!vg_Simulation)
		{
			if (!strcmp(pl_donneepmv->Config.AdresseIP, XZECC_IP_VIDE))
			{
				EnvoyerTrame(pl_mlcr,
							 XZEXC_TRAME_AFF,
							 pl_donneepmv->Config.AdresseRGS,
							 XZEXC_NOCMD,
							 XZEXC_PRIORITEFAIBLE,
							 pl_donneepmv->Socket);
			}
			else
			{
				EnvoyerTrameIP(pl_mlcr,
							   XZEXC_TRAME_AFF,
							   pl_donneepmv->Config.AdresseIP,
							   pl_donneepmv->Config.Port,
							   pl_donneepmv->Config.AdresseRGS,
							   XZEXC_NOCMD,
							   XZEXC_PRIORITEFAIBLE,
							   pl_donneepmv->Socket);
			}
		}
	}
	else
	{
		if (vl_Affichage1 == XDC_PMV_3POINTS)
		{
			vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = XDC_PMV_TEXTE;

			vl_Flash = pl_donneepmv->Etat.Flash;
			vl_Clign1 = vl_Clign2 = vl_Clign3 = vl_Clign4 = XDC_PMV_PASCLIGNO;

			strcpy(vl_Texte1, XDC_CHAINE_VIDE);
			strcpy(vl_Texte2, XDC_CHAINE_VIDE);
			strcpy(vl_Texte3, "            ...");
			strcpy(vl_Texte4, XDC_CHAINE_VIDE);

			strcpy(vl_Alternat1, "");
			strcpy(vl_Alternat2, "");
			strcpy(vl_Alternat3, "");
			strcpy(vl_Alternat4, "");
		}
		/*A Composition de la trame LCR d'affichage de la premiere ligne pmv*/

		/* Composition de la trame sans txt alternat*/
		memset(vl_compstr, ' ', pl_typepmv->NbCaracteres);
		vl_compstr[pl_typepmv->NbCaracteres] = '\0';
		if (!strcmp(vl_compstr, vl_Alternat1) || !strlen(vl_Alternat1))
		{
			strcpy(vl_Alternat1, "");
			if (pl_typepmv->TypePMV == 7 || pl_donneepmv->Config.TypeLCR == 6)
				sprintf(pl_mlcr_lg1, "AM=%d.0 AF=\042%s\042 %s DV=0",
						(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 1 : 2,
						vl_Texte1,
						(vl_Clign1 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign1 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																								  : vg_clignorapide);
			else
				sprintf(pl_mlcr_lg1, "AM=%d AF=\042%s\042 %s DV=0",
						(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 1 : 2,
						vl_Texte1,
						(vl_Clign1 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign1 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																								  : vg_clignorapide);
		}
		else
		{
			/* Composition de la trame avec txt alternat*/
			if (pl_typepmv->TypePMV == 7 || pl_donneepmv->Config.TypeLCR == 6)
				sprintf(pl_mlcr_lg1, "AM=%d.0 AF=\042%s\042/\042%s\042 %s DV=0",
						(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 1 : 2,
						vl_Texte1,
						vl_Alternat1,
						vg_clignolent);
			else
				sprintf(pl_mlcr_lg1, "AM=%d AF=\042%s\042/\042%s\042 %s DV=0",
						(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 1 : 2,
						vl_Texte1,
						vl_Alternat1,
						vg_clignolent);
		}

		/*A Composition de la trame LCR d'affichage de la deuxieme ligne pmv*/

		if (pl_typepmv->NbLigne >= 2)
		{
			/* Composition de la trame sans txt alternat*/
			if (!strcmp(vl_compstr, vl_Alternat2) || !strlen(vl_Alternat2))
			{
				strcpy(vl_Alternat2, "");
				if (pl_typepmv->TypePMV == 7 || pl_donneepmv->Config.TypeLCR == 6)
					sprintf(pl_mlcr_lg2, "AM=%d.0 AF=\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 2 : 3,
							vl_Texte2,
							(vl_Clign2 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign2 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																									  : vg_clignorapide);
				else
					sprintf(pl_mlcr_lg2, "AM=%d AF=\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 2 : 3,
							vl_Texte2,
							(vl_Clign2 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign2 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																									  : vg_clignorapide);
			}
			else
			{
				/* Composition de la trame avec txt alternat*/
				if (pl_typepmv->TypePMV == 7 || pl_donneepmv->Config.TypeLCR == 6)
					sprintf(pl_mlcr_lg2, "AM=%d.0 AF=\042%s\042/\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 2 : 3,
							vl_Texte2,
							vl_Alternat2,
							vg_clignolent);
				else
					sprintf(pl_mlcr_lg2, "AM=%d AF=\042%s\042/\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 2 : 3,
							vl_Texte2,
							vl_Alternat2,
							vg_clignolent);
			}
		}

		/*A Composition de la trame LCR d'affichage de la troisieme ligne pmv*/

		if (pl_typepmv->NbLigne >= 3)
		{
			/* Composition de la trame sans txt alternat*/
			if (!strcmp(vl_compstr, vl_Alternat3) || !strlen(vl_Alternat3))
			{
				strcpy(vl_Alternat3, "");
				if (pl_typepmv->TypePMV == 7 || pl_donneepmv->Config.TypeLCR == 6)
					sprintf(pl_mlcr_lg3, "AM=%d.0 AF=\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 3 : 4,
							vl_Texte3,
							(vl_Clign3 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign3 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																									  : vg_clignorapide);
				else
					sprintf(pl_mlcr_lg3, "AM=%d AF=\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 3 : 4,
							vl_Texte3,
							(vl_Clign3 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign3 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																									  : vg_clignorapide);
			}
			else
			{
				/* Composition de la trame avec txt alternat*/
				if (pl_typepmv->TypePMV == 7 || pl_donneepmv->Config.TypeLCR == 6)
					sprintf(pl_mlcr_lg3, "AM=%d.0 AF=\042%s\042/\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 3 : 4,
							vl_Texte3,
							vl_Alternat3,
							vg_clignolent);
				else
					sprintf(pl_mlcr_lg3, "AM=%d AF=\042%s\042/\042%s\042 %s DV=0",
							(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 3 : 4,
							vl_Texte3,
							vl_Alternat3,
							vg_clignolent);
			}
		}

		XZST_03EcritureTrace(XZSTC_WARNING, "2vl_Alternat1=%s.", vl_Alternat1);
		/* Si PMV 4L Composition de la trame sans txt alternat de la 4eme ligne */
		if (pl_typepmv->NbLigne == EPMVC_PMV_4LIGNES)
		{
			if (!strcmp(vl_compstr, vl_Alternat4) || !strlen(vl_Alternat4))
			{
				strcpy(vl_Alternat4, "");
				sprintf(pl_mlcr_lg4, " AM=1 AF=\042%s\042 %s DV=0",
						vl_Texte4,
						(vl_Clign4 == XDC_PMV_PASCLIGNO) ? "" : (vl_Clign4 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																								  : vg_clignorapide);
			}
			else
			{
				/* Composition de la trame avec txt alternat*/
				sprintf(pl_mlcr_lg4, " AM=1 AF=\042%s\042/\042%s\042 %s DV=0",
						vl_Texte4,
						vl_Alternat4,
						vg_clignolent);
			}
		}

		/* Si PMV avec flash, compisition de la ligne de commande */
		if (pl_typepmv->Flash == XDC_PMV_FLASHPRESENT)
		{
			if (vl_Flash == XDC_PMV_FLASH) 
				sprintf(pl_mlcr_lg4, " AM=4 AF=1 DV=0");
			else
				sprintf(pl_mlcr_lg4, " AM=4 AF=0 DV=0");
		}

		XZST_03EcritureTrace(XZSTC_WARNING, "Callback ep_cmd_PMV_cb = trame : %s", pl_mlcr);

		if (pl_typepmv->TypePMV == 7)
			sprintf(pl_mlcr, "PA %s%s %s %s %s", pl_donneepmv->Config.IdLCR, pl_mlcr_lg4, pl_mlcr_lg1, pl_mlcr_lg2, pl_mlcr_lg3);
		else
			sprintf(pl_mlcr, "P %s%s %s %s %s", pl_donneepmv->Config.IdLCR, pl_mlcr_lg4, pl_mlcr_lg1, pl_mlcr_lg2, pl_mlcr_lg3);
		XZST_03EcritureTrace(XZSTC_WARNING, "Callback ep_cmd_PMV_cb = trame : %s", pl_mlcr);

		/*A Envoyer trame a la tache TELMi */
		XZST_03EcritureTrace(XZSTC_WARNING, "3vl_Alternat1=%s.", vl_Alternat1);
		if (!vg_Simulation)
		{
			if (!strcmp(pl_donneepmv->Config.AdresseIP, XZECC_IP_VIDE))
			{	
				EnvoyerTrame(pl_mlcr,
							 XZEXC_TRAME_AFF,
							 pl_donneepmv->Config.AdresseRGS,
							 XZEXC_NOCMD,
							 XZEXC_PRIORITEFAIBLE,
							 pl_donneepmv->Socket);
			}
			else
			{
				EnvoyerTrameIP(pl_mlcr,
							   XZEXC_TRAME_AFF,
							   pl_donneepmv->Config.AdresseIP,
							   pl_donneepmv->Config.Port,
							   pl_donneepmv->Config.AdresseRGS,
							   XZEXC_NOCMD,
							   XZEXC_PRIORITEFAIBLE,
							   pl_donneepmv->Socket);
			}
		}
	}
	/* Ecriture dans fichier Trace_CMD_PMV de la commande */
	ex_ecriture_trace_cmd(XDC_EQT_PMV,
						  vl_Numero,
						  pl_mlcr,
						  NULL,
						  vl_NoCMD);

	memset(&pl_donneepmv->Etat_Commande, 0, sizeof(XDY_Etat_PMV));
	/* Mise a jour memoire etat commande */
	pl_donneepmv->Etat_Commande.Numero = vl_Numero;
	pl_donneepmv->NumDerCmd = (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE;

	if (vl_Affichage1 == XDC_PMV_ETEINDRE)
		vl_Affichage1 = XDC_PMV_TEXTE;
	if (vl_Affichage2 == XDC_PMV_ETEINDRE)
		vl_Affichage2 = XDC_PMV_TEXTE;
	if (vl_Affichage3 == XDC_PMV_ETEINDRE)
		vl_Affichage3 = XDC_PMV_TEXTE;
	if (vl_Affichage4 == XDC_PMV_ETEINDRE)
		vl_Affichage4 = XDC_PMV_TEXTE;
	strcpy(pl_donneepmv->Etat_Commande.Texte1, vl_Texte1);
	pl_donneepmv->Etat_Commande.Affichage1 = vl_Affichage1;
	strcpy(pl_donneepmv->Etat_Commande.Alternat1, vl_Alternat1);
	pl_donneepmv->Etat_Commande.Clign1 = vl_Clign1;

	strcpy(pl_donneepmv->Etat_Commande.Texte2, vl_Texte2);
	pl_donneepmv->Etat_Commande.Affichage2 = vl_Affichage2;
	strcpy(pl_donneepmv->Etat_Commande.Alternat2, vl_Alternat2);
	pl_donneepmv->Etat_Commande.Clign2 = vl_Clign2;

	strcpy(pl_donneepmv->Etat_Commande.Texte3, vl_Texte3);
	pl_donneepmv->Etat_Commande.Affichage3 = vl_Affichage3;
	strcpy(pl_donneepmv->Etat_Commande.Alternat3, vl_Alternat3);
	pl_donneepmv->Etat_Commande.Clign3 = vl_Clign3;
	pl_donneepmv->Etat_Commande.Flash = vl_Flash;

	if (pl_typepmv->NbLigne == 1)
	{
		strcpy(pl_donneepmv->Etat_Commande.Texte2, "");
		pl_donneepmv->Etat_Commande.Affichage2 = vl_Affichage2;
		strcpy(pl_donneepmv->Etat_Commande.Alternat2, "");
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign2;
		strcpy(pl_donneepmv->Etat_Commande.Texte3, "");
		pl_donneepmv->Etat_Commande.Affichage3 = vl_Affichage3;
		strcpy(pl_donneepmv->Etat_Commande.Alternat3, "");
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign3;
		strcpy(pl_donneepmv->Etat_Commande.Texte4, "");
		pl_donneepmv->Etat_Commande.Affichage4 = vl_Affichage4;
		strcpy(pl_donneepmv->Etat_Commande.Alternat4, "");
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign4;
	}
	else if (pl_typepmv->NbLigne == 2)
	{
		strcpy(pl_donneepmv->Etat_Commande.Texte3, "");
		pl_donneepmv->Etat_Commande.Affichage3 = vl_Affichage3;
		strcpy(pl_donneepmv->Etat_Commande.Alternat3, "");
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign3;
		strcpy(pl_donneepmv->Etat_Commande.Texte4, "");
		pl_donneepmv->Etat_Commande.Affichage4 = vl_Affichage4;
		strcpy(pl_donneepmv->Etat_Commande.Alternat4, "");
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign4;
	}
	else if (pl_typepmv->NbLigne == 3)
	{
		strcpy(pl_donneepmv->Etat_Commande.Texte4, "");
		pl_donneepmv->Etat_Commande.Affichage4 = vl_Affichage4;
		strcpy(pl_donneepmv->Etat_Commande.Alternat4, "");
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign4;
	}
	else
	{
		strcpy(pl_donneepmv->Etat_Commande.Texte4, vl_Texte4);
		pl_donneepmv->Etat_Commande.Affichage4 = vl_Affichage4;
		strcpy(pl_donneepmv->Etat_Commande.Alternat4, vl_Alternat4);
		pl_donneepmv->Etat_Commande.Clign4 = vl_Clign4;
	}

	/* Demande de relecture pour controle de la bonne execution */
	XZST_03EcritureTrace(XZSTC_WARNING, "Eqt_PMV_Picto %d", pl_donneepmv->Config.Eqt_PMV_Picto);
	if ((pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ||
		(pl_typepmv->Picto == 0))
	{
		if (!strcmp(pl_donneepmv->Config.AdresseIP, XZECC_IP_VIDE))
		{
			EnvoyerTrame(	"ST",
							(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
							pl_donneepmv->Config.AdresseRGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_donneepmv->Socket);
		}
		else
		{
			EnvoyerTrameIP(	"ST",
							(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
							pl_donneepmv->Config.AdresseIP,
							pl_donneepmv->Config.Port,
							pl_donneepmv->Config.AdresseRGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_donneepmv->Socket);
		}				
		if (!vg_Simulation)
		{
			if (!strcmp(pl_donneepmv->Config.AdresseIP, XZECC_IP_VIDE))
			{
				EnvoyerTrame(	"PS",
								(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
								pl_donneepmv->Config.AdresseRGS,
								vl_NoCMD,
								XZEXC_PRIORITEFAIBLE,
								pl_donneepmv->Socket);
			}
			else
			{
				EnvoyerTrameIP(	"PS",
								(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
								pl_donneepmv->Config.AdresseIP,
								pl_donneepmv->Config.Port,
								pl_donneepmv->Config.AdresseRGS,
								vl_NoCMD,
								XZEXC_PRIORITEFAIBLE,
								pl_donneepmv->Socket);
			}
		}
	}
	else
	{
		/*A Recherche du No d'equipement dans la structure config_picto en memoire */
		for (pl_donneepicto = pg_debutPicto; pl_donneepicto != NULL;
			pl_donneepicto = pl_donneepicto->Suivant)
		{
			/* Equipement trouve */
			XZST_03EcritureTrace(XZSTC_WARNING, "PICTO %d", pl_donneepicto->Config.Numero.NoEqt);
			if (pl_donneepicto->Config.Numero.NoEqt == vl_Numero)
				break;
		}
		
		if (pl_donneepicto==NULL)
			vl_EnvoiSTPS = XDC_VRAI;
		else
			vl_EnvoiSTPS = pl_donneepmv->NumDerCmd == pl_donneepicto->Etat_Commande.NoCMD ? XDC_VRAI : XDC_FAUX;
		
		if ( vl_EnvoiSTPS==XDC_VRAI )
		{
			if (!strcmp(pl_donneepmv->Config.AdresseIP, XZECC_IP_VIDE))
			{
				EnvoyerTrame(	"ST",
								(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
								pl_donneepmv->Config.AdresseRGS,
								XZEXC_NOCMD,
								XZEXC_PRIORITEFAIBLE,
								pl_donneepmv->Socket);
			}
			else
			{
				EnvoyerTrameIP(	"ST",
								(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
								pl_donneepmv->Config.AdresseIP,
								pl_donneepmv->Config.Port,
								pl_donneepmv->Config.AdresseRGS,
								XZEXC_NOCMD,
								XZEXC_PRIORITEFAIBLE,
								pl_donneepmv->Socket);				
			}
			if (!vg_Simulation)
			{
				if (!strcmp(pl_donneepmv->Config.AdresseIP, XZECC_IP_VIDE))
				{
					EnvoyerTrame(	"PS",
									(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
									pl_donneepmv->Config.AdresseRGS,
									vl_NoCMD,
									XZEXC_PRIORITEFAIBLE,
									pl_donneepmv->Socket);
				}
				else
				{
					EnvoyerTrameIP(	"PS",
									(pl_donneepmv->Config.Eqt_PMV_Picto == XDC_FAUX) ? XZEXC_TRAME_ETAT_AFF_PICTO : XZEXC_TRAME_ETAT_AFF_PMV,
									pl_donneepmv->Config.AdresseIP,
									pl_donneepmv->Config.Port,
									pl_donneepmv->Config.AdresseRGS,
									vl_NoCMD,
									XZEXC_PRIORITEFAIBLE,
									pl_donneepmv->Socket);	
				}
			}
		}

	}

	XZST_03EcritureTrace(XZSTC_WARNING, "pmv etat clign : %d %d %d %d", pl_donneepmv->Etat_Commande.Clign1, pl_donneepmv->Etat_Commande.Clign2, pl_donneepmv->Etat_Commande.Clign3, pl_donneepmv->Etat_Commande.Clign4);
	XZST_03EcritureTrace(XZSTC_INFO, "pmv:%d;vl_NoCMD=%d;pl_donneepmv->NumDerCmd=%d;A1:%s;A2:%s;", vl_Numero, vl_NoCMD, pl_donneepmv->NumDerCmd, pl_donneepmv->Etat_Commande.Alternat1, pl_donneepmv->Etat_Commande.Alternat2);
	if (vg_Simulation)
	{
		memcpy(&pl_donneepmv->Etat, &pl_donneepmv->Etat_Commande, sizeof(XDY_Etat_PMV));

		ep_envoi_affichage_PMV(pl_donneepmv->Etat, vl_Numero, pl_typepmv->NbLigne, pl_donneepmv->Config.SiteGestion);
		if (vl_NoCMD != 0)
		{
			xzat25_CompteRendu_PMV(vl_NoCMD, vl_horodate, XDC_OK, pl_donneepmv->Config.SiteGestion);
		}
		XZEZ03_CompteRendu_TDP(vl_NoCMD, XDC_OK, vl_Numero, vg_NomMachine);
	}
	/* Fin du callback */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMV_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le pmv
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int em_envoyer_contraste_pmv(EPMV_DONNEES_PMV *pa_ListePMV,
							 int va_Contraste,
							 int va_NumCmd)
/*
* ARGUMENTS EN ENTREE :
*  pa_ListePMV : pointeur sur liste pmv
*  va_Contraste : Contraste a envoyer
*
* ARGUMENTS EN SORTIE :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*  Envoi de la trame de contraste a telmi pour le pmv
*
------------------------------------------------------*/
{
	XZEXT_Trame vl_lcr = "";
	char vl_Contraste[3];
	XDY_Datagroup pl_DG;
	int vl_ValRet = XDC_OK;
	EPMV_DONNEES_TYPE_PMV *pl_typepmv;

	/*A Choix de la commande en fonction du contexte jour nuit */
	switch (va_Contraste)
	{
	case XDC_CONTRASTEBRILLANT:
		strcpy(vl_Contraste, "S");
		break;
	case XDC_CONTRASTENORMAL:
	default :
		/* SAE-429 l'etat normal est le mode Automatique, et ne depend plus du contexte Jour/Nuit */
		strcpy(vl_Contraste, "A");
		break;
	}

	/*A Recherche du type de pmv */
	for (pl_typepmv = pg_debutTypePMV; pl_typepmv != NULL;
		 pl_typepmv = pl_typepmv->Suivant)
	{
		if (pa_ListePMV->Config.TypePMV == pl_typepmv->TypePMV)
		{	
			break;
		}
	}

	/*A envoi de la trame de contraste */

	if (pl_typepmv->TypePMV == 7 || pa_ListePMV->Config.TypeLCR == 6)
		sprintf(vl_lcr, "P %sAM=%d.0 %s EC=%s", pa_ListePMV->Config.IdLCR,
				(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 1 : 2,
				(pa_ListePMV->Etat_Commande.Clign1 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign1 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																		  : vg_clignorapide,
				vl_Contraste);
	else
		sprintf(vl_lcr, "P %sAM=%d %s EC=%s", pa_ListePMV->Config.IdLCR,
				(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 1 : 2,
				(pa_ListePMV->Etat_Commande.Clign1 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign1 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																		  : vg_clignorapide,
				vl_Contraste);

	if (!vg_Simulation)
	{
		if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
		{
			EnvoyerTrame(vl_lcr,
						 XZEXC_TRAME_CONTRASTE,
						 pa_ListePMV->Config.AdresseRGS,
						 XZEXC_NOCMD,
						 XZEXC_PRIORITEFAIBLE,
						 pa_ListePMV->Socket);
		}
		else
		{
			EnvoyerTrameIP(vl_lcr,
						   XZEXC_TRAME_CONTRASTE,
						   pa_ListePMV->Config.AdresseIP,
						   pa_ListePMV->Config.Port,
						   pa_ListePMV->Config.AdresseRGS,
						   XZEXC_NOCMD,
						   XZEXC_PRIORITEFAIBLE,
						   pa_ListePMV->Socket);
		}
	}
	if (pl_typepmv->NbLigne >= 2)
	{
		if (pl_typepmv->TypePMV == 7 || pa_ListePMV->Config.TypeLCR == 6)
			sprintf(vl_lcr, "P %sAM=%d.0 %s EC=%s", pa_ListePMV->Config.IdLCR,
					(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 2 : 3,
					(pa_ListePMV->Etat_Commande.Clign2 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign2 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																			  : vg_clignorapide,
					vl_Contraste);
		else
			sprintf(vl_lcr, "P %sAM=%d %s EC=%s", pa_ListePMV->Config.IdLCR,
					(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 2 : 3,
					(pa_ListePMV->Etat_Commande.Clign2 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign2 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																			  : vg_clignorapide,
					vl_Contraste);
		if (!vg_Simulation)
		{
			if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
			{
				EnvoyerTrame(vl_lcr,
							 XZEXC_TRAME_CONTRASTE,
							 pa_ListePMV->Config.AdresseRGS,
							 XZEXC_NOCMD,
							 XZEXC_PRIORITEFAIBLE,
							 pa_ListePMV->Socket);
			}
			else
			{
				EnvoyerTrameIP(vl_lcr,
							   XZEXC_TRAME_CONTRASTE,
							   pa_ListePMV->Config.AdresseIP,
							   pa_ListePMV->Config.Port,
							   pa_ListePMV->Config.AdresseRGS,
							   XZEXC_NOCMD,
							   XZEXC_PRIORITEFAIBLE,
							   pa_ListePMV->Socket);
			}
		}
	}
	if (pl_typepmv->NbLigne >= 3)
	{
		if (pl_typepmv->TypePMV == 7 || pa_ListePMV->Config.TypeLCR == 6)
			sprintf(vl_lcr, "P %sAM=%d.0 %s EC=%s", pa_ListePMV->Config.IdLCR,
					(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 3 : 4,
					(pa_ListePMV->Etat_Commande.Clign3 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign3 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																			  : vg_clignorapide,
					vl_Contraste);
		else
			sprintf(vl_lcr, "P %sAM=%d %s EC=%s", pa_ListePMV->Config.IdLCR,
					(pl_typepmv->NbLigne != EPMVC_PMV_4LIGNES) ? 3 : 4,
					(pa_ListePMV->Etat_Commande.Clign3 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign3 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																			  : vg_clignorapide,
					vl_Contraste);
		if (!vg_Simulation)
		{
			if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
			{
				EnvoyerTrame(vl_lcr,
							 XZEXC_TRAME_CONTRASTE,
							 pa_ListePMV->Config.AdresseRGS,
							 XZEXC_NOCMD,
							 XZEXC_PRIORITEFAIBLE,
							 pa_ListePMV->Socket);
			}
			else
			{
				EnvoyerTrameIP(vl_lcr,
							   XZEXC_TRAME_CONTRASTE,
							   pa_ListePMV->Config.AdresseIP,
							   pa_ListePMV->Config.Port,
							   pa_ListePMV->Config.AdresseRGS,
							   XZEXC_NOCMD,
							   XZEXC_PRIORITEFAIBLE,
							   pa_ListePMV->Socket);
			}
		}
	}

	if (pl_typepmv->NbLigne == EPMVC_PMV_4LIGNES)
	{
		if (pl_typepmv->TypePMV == 7 || pa_ListePMV->Config.TypeLCR == 6)
			sprintf(vl_lcr, "P %sAM=%d.0 %s EC=%s", pa_ListePMV->Config.IdLCR,
					1,
					(pa_ListePMV->Etat_Commande.Clign4 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign4 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																			  : vg_clignorapide,
					vl_Contraste);
		else
			sprintf(vl_lcr, "P %sAM=%d %s EC=%s", pa_ListePMV->Config.IdLCR,
					1,
					(pa_ListePMV->Etat_Commande.Clign4 == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePMV->Etat_Commande.Clign4 == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																			  : vg_clignorapide,
					vl_Contraste);
		if (!vg_Simulation)
		{
			if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
				EnvoyerTrame(vl_lcr,
							 XZEXC_TRAME_CONTRASTE,
							 pa_ListePMV->Config.AdresseRGS,
							 XZEXC_NOCMD,
							 XZEXC_PRIORITEFAIBLE,
							 pa_ListePMV->Socket);
			else
				EnvoyerTrameIP(vl_lcr,
							   XZEXC_TRAME_CONTRASTE,
							   pa_ListePMV->Config.AdresseIP,
							   pa_ListePMV->Config.Port,
							   pa_ListePMV->Config.AdresseRGS,
							   XZEXC_NOCMD,
							   XZEXC_PRIORITEFAIBLE,
							   pa_ListePMV->Socket);
		}
	}

	if (vg_Simulation)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "if (vg_Simulation) ");
		pa_ListePMV->Contraste = va_Contraste;
		XDG_EncodeDG2(pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite);
		TipcSrvMsgWrite(pl_DG,
						XDM_IdentMsg(XDM_ETAT_Contraste),
						XDM_FLG_SRVMSGWRITE,
						T_IPC_FT_INT2, XDC_EQT_PMV,
						T_IPC_FT_INT2, pa_ListePMV->Config.Numero.NoEqt,
						T_IPC_FT_INT2, pa_ListePMV->Contraste,
						NULL);
	}

	/*A Si envoi a telmi impossible alors init connexion socket avec telmi */
	if (vl_ValRet != XDC_OK)
		ec_init_cnx();

	pa_ListePMV->ContrasteCmd = va_Contraste;
	return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi de la trame de contraste au pictogramme
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int em_envoyer_contraste_picto(EPMV_DONNEES_Picto *pa_ListePicto,
							   int va_Contraste,
							   int va_NumCmd,
							   int va_EnvoiSTPS)
/*
* ARGUMENTS EN ENTREE :
*  pl_ListePMV : pointeur sur liste pmv
*  va_Contraste : Contraste a envoyer
*
* ARGUMENTS EN SORTIE :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*   Envoi de la trame de contraste a telmi pour le pmv
*
*
------------------------------------------------------*/
{
	XZEXT_Trame vl_lcr = "";
	char vl_Contraste[3];
	XDY_Datagroup pl_DG;
	int vl_ValRet = XDC_OK;

	XZST_03EcritureTrace(XZSTC_WARNING, "Contraste FONCTION IN %d %d", pa_ListePicto->Config.Numero.NoEqt, va_Contraste);
	/*A Choix de la commande en fonction du contexte jour nuit */
	switch (va_Contraste)
	{
	case XDC_CONTRASTEBRILLANT:
		strcpy(vl_Contraste, "S");
		break;
	case XDC_CONTRASTENORMAL:
	default :
		/* SAE-429 l'etat normal est le mode Automatique, et ne depend plus du contexte Jour/Nuit */
		strcpy(vl_Contraste, "A");
		break;
	}

	/*A envoi de la trame de contraste */
	sprintf(vl_lcr, "P %sAM=6 %s EC=%s", pa_ListePicto->Config.IdLCR,
			(pa_ListePicto->Etat_Commande.Clign == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePicto->Etat_Commande.Clign == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																		: vg_clignorapide,
			vl_Contraste);
	XZST_03EcritureTrace(XZSTC_INFO, "Contraste Picto %s", vl_lcr);
	if (!vg_Simulation)
	{
		if (!strcmp(pa_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
			EnvoyerTrame(vl_lcr,
						 XZEXC_TRAME_CONTRASTE,
						 pa_ListePicto->Config.AdresseRGS,
						 XZEXC_NOCMD,
						 XZEXC_PRIORITEFAIBLE,
						 pa_ListePicto->Socket);
		else
			EnvoyerTrameIP(vl_lcr,
						   XZEXC_TRAME_CONTRASTE,
						   pa_ListePicto->Config.AdresseIP,
						   pa_ListePicto->Config.Port,
						   pa_ListePicto->Config.AdresseRGS,
						   XZEXC_NOCMD,
						   XZEXC_PRIORITEFAIBLE,
						   pa_ListePicto->Socket);
	}

	sprintf(vl_lcr, "P %sAM=7 %s EC=%s", pa_ListePicto->Config.IdLCR,
			(pa_ListePicto->Etat_Commande.Clign == XDC_PMV_PASCLIGNO) ? "" : (pa_ListePicto->Etat_Commande.Clign == XDC_PMV_CLIGNOLENT) ? vg_clignolent
																																		: vg_clignorapide,
			vl_Contraste);
	XZST_03EcritureTrace(XZSTC_INFO, "Contraste Picto %s", vl_lcr);
	if (!vg_Simulation)
	{
		if (!strcmp(pa_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
			EnvoyerTrame(vl_lcr,
						 XZEXC_TRAME_CONTRASTE,
						 pa_ListePicto->Config.AdresseRGS,
						 XZEXC_NOCMD,
						 XZEXC_PRIORITEFAIBLE,
						 pa_ListePicto->Socket);
		else
			EnvoyerTrameIP(vl_lcr,
						   XZEXC_TRAME_CONTRASTE,
						   pa_ListePicto->Config.AdresseIP,
						   pa_ListePicto->Config.Port,
						   pa_ListePicto->Config.AdresseRGS,
						   XZEXC_NOCMD,
						   XZEXC_PRIORITEFAIBLE,
						   pa_ListePicto->Socket);
	}

//	if (va_EnvoiSTPS)
//	{
//		vl_ValRet = em_demander_etat_picto ( pa_ListePicto, va_NumCmd);
//	}

	if (vg_Simulation)
	{
		pa_ListePicto->Contraste = va_Contraste;
		XDG_EncodeDG2(pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite);
		TipcSrvMsgWrite(pl_DG,
						XDM_IdentMsg(XDM_ETAT_Contraste),
						XDM_FLG_SRVMSGWRITE,
						T_IPC_FT_INT2, XDC_EQT_PCT,
						T_IPC_FT_INT2, pa_ListePicto->Config.Numero.NoEqt,
						T_IPC_FT_INT2, pa_ListePicto->Contraste,
						NULL);
	}

	pa_ListePicto->ContrasteCmd = va_Contraste;
	return (vl_ValRet);
}


/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi de demande d'etat a un PMV ou Picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int em_demander_etat_pmv_picto (int		va_TypeCmd,
				int		va_NumCmd,
				XDY_Rgs		va_AdresseRGS,
				XDY_AdresseIP	va_AdresseIP,
				int		va_Port,
				int		va_Socket)
/*
* ARGUMENTS EN ENTREE :
*  va_TypeCmd	: Type de la commande
*  va_NumCmd	: Numero de la commande en cours
*  va_AdresseRGS : Adresse RGS de l'equipement
*  va_AdresseIP	: Adresse IP de l'equipement
*  va_Port	: Numero de port de l'equipement
*  va_Socket	: Socket de liaison vers l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*   Envois successifs d'une demande de status complet
*   puis d'une demande d'etat panneau
*
------------------------------------------------------*/
{
	int	vl_ValRet = XDC_OK;

	/* envoi d'une demande de status complet */
	if (!strcmp (va_AdresseIP, XZECC_IP_VIDE))
	{
		EnvoyerTrame("ST",
				va_TypeCmd,
				va_AdresseRGS,
				XZEXC_NOCMD,
				XZEXC_PRIORITEFAIBLE,
				va_Socket);
	}
	else
	{
		EnvoyerTrameIP("ST",
				va_TypeCmd,
				va_AdresseIP,
				va_Port,
				va_AdresseRGS,
				XZEXC_NOCMD,
				XZEXC_PRIORITEFAIBLE,
				va_Socket);
	}

	/* envoi de la trame de demande d'etat d'affichage */
	if (!vg_Simulation)
	{
		if (!strcmp (va_AdresseIP, XZECC_IP_VIDE))
		{
			vl_ValRet = EnvoyerTrame("PS",
							va_TypeCmd,
							va_AdresseRGS,
							va_NumCmd,
							XZEXC_PRIORITEFAIBLE,
							va_Socket);
		}
		else
		{
			vl_ValRet = EnvoyerTrameIP("PS",
							va_TypeCmd,
							va_AdresseIP,
							va_Port,
							va_AdresseRGS,
							va_NumCmd,
							XZEXC_PRIORITEFAIBLE,
							va_Socket);
		}
	}

	return (vl_ValRet);
}


/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi de demande d'etat a un Picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_demander_etat_picto (EPMV_DONNEES_Picto	*pa_ListePicto,
				int	va_NumCmd)
/*
* ARGUMENTS EN ENTREE :
*  va_NumCmd	: Numero de la commande en cours
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*   Envoi de la demande d'etat au Picto.
*
------------------------------------------------------*/
{
	int	vl_TypeCmd;
	int	vl_ValRet = XDC_OK;

	if (pa_ListePicto->Config.Eqt_PMV_Picto == XDC_FAUX)
	{
		vl_TypeCmd = XZEXC_TRAME_ETAT_AFF_PICTO;
	}
	else {
		vl_TypeCmd = XZEXC_TRAME_ETAT_AFF_PMV;
	}

	vl_ValRet = em_demander_etat_pmv_picto (vl_TypeCmd,
						va_NumCmd,
						pa_ListePicto->Config.AdresseRGS,
						pa_ListePicto->Config.AdresseIP,
						pa_ListePicto->Config.Port,
						pa_ListePicto->Socket);

	return (vl_ValRet);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de changement de contraste .
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_contraste_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_Contraste
*
* FONCTION
*
*  suivant le type d'equipement envoi du message de contraste
*
------------------------------------------------------*/
{
#ifdef _TIBCO_EMS
	XDY_Mot va_Contraste;
	XDY_Mot va_NoEqt;
	XDY_Mot va_TypeEqt;
#else
	T_INT2 va_Contraste;
	T_INT2 va_NoEqt;
	T_INT2 va_TypeEqt;
#endif

	EPMV_DONNEES_PMV *pl_ListePMV;
	EPMV_DONNEES_Picto *pl_ListePicto;
	XZEXT_Trame vl_lcr = "";

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_contraste_cb");

	/* Recuperation des parametres du message XDM_ECMD_Contraste */
	if (!TipcMsgRead(pa_Data->msg, 
					T_IPC_FT_INT2, &va_Contraste, 
					T_IPC_FT_INT2, &va_NoEqt, 
					T_IPC_FT_INT2, &va_TypeEqt, 
					NULL))
	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_Contraste non lu \n");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb");
		return;
	}

	switch (va_TypeEqt)
	{
	case XZECC_TYPE_EQT_PMV:
		/*B pour tous les pmv envoi de la trame de contraste */
		for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL; pl_ListePMV = pl_ListePMV->Suivant)
		{
			if ((va_NoEqt == XZEPC_SURB_EQTALL) || (pl_ListePMV->Config.Numero.NoEqt == va_NoEqt))
			{
				/*A Composition de la trame contraste et envoi */
				em_envoyer_contraste_pmv(pl_ListePMV, va_Contraste, XZEXC_CMD_HORS_BASE);
			}
			if (pl_ListePMV->Config.Numero.NoEqt == va_NoEqt)
				break;
		}
		/*B pour tous les pictos envoi de la trame de contraste */
		for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL; pl_ListePicto = pl_ListePicto->Suivant)
		{
			if ((va_NoEqt == XZEPC_SURB_EQTALL) || (pl_ListePicto->Config.Numero.NoEqt == va_NoEqt))
			{
				/*A Composition de la trame contraste et envoi */
				em_envoyer_contraste_picto(pl_ListePicto, va_Contraste, XZEXC_CMD_HORS_BASE, XDC_VRAI);
				em_demander_etat_picto (pl_ListePicto, XZEXC_CMD_HORS_BASE);
			}
			if (pl_ListePicto->Config.Numero.NoEqt == va_NoEqt)
				break;
		}
		break;

	case XZECC_TYPE_EQT_PICTO:

		/*B pour tous les pictos envoi de la trame de contraste */
		for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL; pl_ListePicto = pl_ListePicto->Suivant)
		{
			if ((va_NoEqt == XZEPC_SURB_EQTALL) || (pl_ListePicto->Config.Numero.NoEqt == va_NoEqt))
			{
				/*A Composition de la trame contraste et envoi */
				if (pl_ListePicto->Config.Eqt_PMV_Picto == XDC_FAUX)
				{
					em_envoyer_contraste_picto(pl_ListePicto, va_Contraste, XZEXC_CMD_HORS_BASE, XDC_VRAI);
					em_demander_etat_picto (pl_ListePicto, XZEXC_CMD_HORS_BASE);
				}
			}
			if (pl_ListePicto->Config.Numero.NoEqt == va_NoEqt)
				break;
		}
	}

	/*A Trace sortie fonction ep_contraste_cb */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb");
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Message de controle.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_controle_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_EQPT
*
* FONCTION
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_STR va_NomMachine;
	T_INT4 va_Etat;
	T_INT4 va_TypeEqt;
	T_INT4 va_NumEqt;
#else
	XDY_NomMachine va_NomMachine;
	XDY_Entier va_Etat;
	XDY_Entier va_TypeEqt;
	XDY_Entier va_NumEqt;
#endif

	EPMV_DONNEES_PMV *pl_ListePMV;
	EPMV_DONNEES_Picto *pl_ListePicto;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_controle_cb ");

	/*A Recuperation des parametres du message XDM_ECMD_EQPT */
	if (!TipcMsgRead(pa_Data->msg, 
					T_IPC_FT_INT4, &va_Etat, 
					T_IPC_FT_STR, &va_NomMachine, 
					T_IPC_FT_INT4, &va_TypeEqt, 
					T_IPC_FT_INT4, &va_NumEqt, 
					NULL))

	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu \n");
	}

	/* Trace du message en entree de la fonction */
	XZST_03EcritureTrace(XZSTC_INFO, "callback ep_controle_cb, msg= %d %s %d %d",
						 va_Etat,
						 va_NomMachine,
						 va_TypeEqt,
						 va_NumEqt);
	return;
	{
		/*A si l'eqt est un picto */
		if (va_TypeEqt == XZECC_TYPE_EQT_PICTO)
		{
			/*B Recherche du No d'equipement dans la structure config_picto en memoire */
			for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL;
				 pl_ListePicto = pl_ListePicto->Suivant)
			{
				if (pl_ListePicto->Config.Numero.NoEqt == va_NumEqt)
				{
					if (va_Etat > XZEXC_LIMITE_ETAT_EQT)
						pl_ListePicto->Etat_Service &= va_Etat;
					else
						pl_ListePicto->Etat_Service += va_Etat;

					/*               if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
								   {
									  pl_ListePicto->Etat_Service = va_Etat;
								   }
								   if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
								   {
									  pl_ListePicto->Etat_Activite = va_Etat;
								   }*/
					break;
				}
			}
		}

		/*A si l'eqt est un pmv */
		if (va_TypeEqt == XZECC_TYPE_EQT_PMV)
		{
			/*B Recherche du No d'equipement dans la structure config_pmv en memoire */
			for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
				 pl_ListePMV = pl_ListePMV->Suivant)
			{
				if (pl_ListePMV->Config.Numero.NoEqt == va_NumEqt)
				{
					if (va_Etat > XZEXC_LIMITE_ETAT_EQT)
						pl_ListePMV->Etat_Service &= va_Etat;
					else
						pl_ListePMV->Etat_Service += va_Etat;

					/*               if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
								   {
									 pl_ListePMV->Etat_Service = va_Etat;
								   }
								   if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
								   {
									  pl_ListePMV->Etat_Activite = va_Etat;
								   }*/
					break;
				}
			}
		}
	}

	/* Trace des Differents etats service et activite */
	for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL;
		 pl_ListePicto = pl_ListePicto->Suivant)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "servpicto:%d", pl_ListePicto->Etat_Service);
	}
	for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
		 pl_ListePMV = pl_ListePMV->Suivant)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "servpmv:%d ", pl_ListePMV->Etat_Service);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_controle_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Changement de contexte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_journuit(XZEAT_JourNuit va_contexte, int va_resultat)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   vl_resultat
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
* Met a jour la variable vg_contexte
*
------------------------------------------------------*/
{
	XZEXT_Trame vl_lcr = "";
	char vl_Contraste[3];
	EPMV_DONNEES_PMV *pl_ListePMV;
	EPMV_DONNEES_Picto *pl_ListePicto;

	/*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
	if (va_contexte.JourNuit == XDC_JOUR)
		vg_Contexte = XDC_JOUR;
	else
		vg_Contexte = XDC_NUIT;

	XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d", vg_Contexte);

// SAE-429  Plus de pilotage de luminosite
//	/*B Pour tout les pictos en memoire */
//	for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL;
//		 pl_ListePicto = pl_ListePicto->Suivant)
//	{
//		/* Envoyer nouveau contraste */
//		/*if ( pl_ListePicto->Config.Eqt_PMV_Picto == XDC_FAUX ) {*/
//		em_envoyer_contraste_picto(pl_ListePicto, pl_ListePicto->Contraste, XZEXC_CMD_HORS_BASE, XDC_VRAI);
//		em_demander_etat_picto (pl_ListePicto, XZEXC_CMD_HORS_BASE);
//	}
//
//	/*B Pour tout les PMV en memoire */
//	for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
//		 pl_ListePMV = pl_ListePMV->Suivant)
//	{
//		/* Envoyer nouveau contraste */
//		em_envoyer_contraste_pmv(pl_ListePMV, pl_ListePMV->Contraste, XZEXC_CMD_HORS_BASE);
//	}
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Mise a jour du contraste de tous les PMV et Pictos pilotes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_maj_contraste_tous(int va_Contraste)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
* Envoie la consigne de contraste a tous les equipements pilotes
*
------------------------------------------------------*/
{
	EPMV_DONNEES_PMV *pl_ListePMV;
	EPMV_DONNEES_Picto *pl_ListePicto;


	/* Pour tous les PMV en memoire */
	for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
		 pl_ListePMV = pl_ListePMV->Suivant)
	{
		/* Envoyer nouveau contraste */
		em_envoyer_contraste_pmv(pl_ListePMV, va_Contraste, XZEXC_CMD_HORS_BASE);
	}

	/* Pour tous les pictos en memoire */
	for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL;
		 pl_ListePicto = pl_ListePicto->Suivant)
	{
		/* Envoyer nouveau contraste */
		/*if ( pl_ListePicto->Config.Eqt_PMV_Picto == XDC_FAUX ) {*/
		em_envoyer_contraste_picto(pl_ListePicto, va_Contraste, XZEXC_CMD_HORS_BASE, XDC_FAUX);
	}

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande d'affichage picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_CMD_LCR(T_IPC_CONN va_Cnx,
				T_IPC_CONN_PROCESS_CB_DATA data,
				T_CB_ARG arg)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ELCR
*
* FONCTION
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT2 vl_TypeEqt;
	T_INT2 vl_NoEqt;
	T_STR vl_FicCmdLcr;
	T_STR vl_FicResLcr;
	T_REAL8 vl_HrDebut;
	T_REAL8 vl_HrFin;
	T_STR vl_NomMachine;
	T_INT2 vl_Periode;
#else
	XDY_Mot vl_TypeEqt;
	XDY_Mot vl_NoEqt;
	XDY_FichEquext vl_FicCmdLcr;
	XDY_FichEquext vl_FicResLcr;
	XDY_Horodate vl_HrDebut;
	XDY_Horodate vl_HrFin;
	XDY_NomMachine vl_NomMachine;
	XDY_Mot vl_Periode;
#endif

	EPMV_DONNEES_Picto *pl_ListePicto;
	EPMV_DONNEES_PMV *pl_ListePMV;
	INFO_EQT_FEN_LCR pl_eqt, *pl_ListeEqt;
	XZEXT_MSG_SOCKET pl_MsgTimer;
	XZEXT_CLE_PROG vl_clef;

	/*A Lecture du message */

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN :ep_CMD_LCR");
	if (!TipcMsgRead(data->msg,
					 T_IPC_FT_INT2, &vl_TypeEqt,
					 T_IPC_FT_INT2, &vl_NoEqt,
					 T_IPC_FT_STR, &vl_FicCmdLcr,
					 T_IPC_FT_STR, &vl_FicResLcr,
					 T_IPC_FT_INT2, &vl_Periode,
					 T_IPC_FT_REAL8, &vl_HrDebut,
					 T_IPC_FT_REAL8, &vl_HrFin,
					 T_IPC_FT_STR, &vl_NomMachine,
					 NULL))

	{
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ELCR non lu \n");
		XZST_10ArchiverMsgSyst(0, "ep_CMD_LCR : Message XDM_ELCR(Pmv Picto) recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_CMD_LCR");
		return;
	}

	/* Trace du message en entree de la fonction */
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN ep_CMD_LCR");

	/*A Construction de la liste globale des eqt a piloter par la fenetre LCR */

	pl_eqt.TypeEqt = vl_TypeEqt;
	strcpy(pl_eqt.NomFichCmdesLCR, vl_FicCmdLcr);
	strcpy(pl_eqt.NomFichResLCR, vl_FicResLcr);
	pl_eqt.Periode = vl_Periode;
	pl_eqt.HrDebut = vl_HrDebut;
	pl_eqt.HrFin = vl_HrFin;
	strcpy(pl_eqt.NomMachine, vl_NomMachine);
	pl_eqt.NoEqt = vl_NoEqt;
	pl_eqt.Position = 0;
	pl_eqt.Suivant = NULL;

	/* Recherche si pas de commande LCR en cours sur equipement concerne */
	if ((vl_TypeEqt == XDC_EQT_PMV) &&
		(ex_test_autorisation_eqt(&pg_DebutListePMVFLcr,
								  &pl_eqt,
								  XDC_EQT_PMV) == XDC_NOK))
	{
		return;
	}
	if ((vl_TypeEqt == XDC_EQT_PCT) &&
		(ex_test_autorisation_eqt(&pg_DebutListePictoFLcr,
								  &pl_eqt,
								  XDC_EQT_PCT) == XDC_NOK))
	{
		return;
	}

	if (vl_TypeEqt == XDC_EQT_PMV)
	{
		for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
			 pl_ListePMV = pl_ListePMV->Suivant)
		{
			if ((pl_ListePMV->Config.Numero.NoEqt == vl_NoEqt) || (vl_NoEqt == XDC_EQTALL))
			{
				if (strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
				{
					ex_init_lcr_ip(&pg_DebutListePMVFLcr,
								   pl_eqt,
								   pl_ListePMV->Socket,
								   pl_ListePMV->Config.AdresseRGS,
								   pl_ListePMV->Config.Numero.NoEqt,
								   "PMV_FLcr_NoEqt:",
								   vg_SockTIM,
								   pl_ListePMV->Config.AdresseIP,
								   pl_ListePMV->Config.Port);
				}
				else
				{
					ex_init_lcr(&pg_DebutListePMVFLcr,
								pl_eqt,
								pl_ListePMV->Socket,
								pl_ListePMV->Config.AdresseRGS,
								pl_ListePMV->Config.Numero.NoEqt,
								"PMV_FLcr_NoEqt:",
								vg_SockTIM);
				}
			}
			if (pl_ListePMV->Config.Numero.NoEqt == vl_NoEqt)
				break;
		}
		if ((pl_ListePMV == NULL) && (vl_NoEqt != XDC_EQTALL))
		{
			XZEX02_Retour_LCR(XDC_NOK, vl_NomMachine);
		}
	}
	else
	{
		for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL;
			 pl_ListePicto = pl_ListePicto->Suivant)
		{
			if ((pl_ListePicto->Config.Numero.NoEqt == vl_NoEqt) || (vl_NoEqt == XDC_EQTALL))
			{
				if (strcmp(pl_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
				{
					ex_init_lcr_ip(&pg_DebutListePictoFLcr,
								   pl_eqt,
								   pl_ListePicto->Socket,
								   pl_ListePicto->Config.AdresseRGS,
								   pl_ListePicto->Config.Numero.NoEqt,
								   "Picto_FLcr_NoEqt:",
								   vg_SockTIM,
								   pl_ListePicto->Config.AdresseIP,
								   pl_ListePicto->Config.Port);
				}
				else
				{
					ex_init_lcr(&pg_DebutListePictoFLcr,
								pl_eqt,
								pl_ListePicto->Socket,
								pl_ListePicto->Config.AdresseRGS,
								pl_ListePicto->Config.Numero.NoEqt,
								"Picto_FLcr_NoEqt:",
								vg_SockTIM);
				}
			}
			if (pl_ListePicto->Config.Numero.NoEqt == vl_NoEqt)
				break;
		}
		if ((pl_ListePicto == NULL) && (vl_NoEqt != XDC_EQTALL))
		{
			XZEX02_Retour_LCR(XDC_NOK, vl_NomMachine);
		}
	}

	/* Fin du callback */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_CMD_LCR");
}

void supprime_blancs_fin(char *Ligne)

/*
* ARGUMENTS EN ENTREE :
*       T_STR           Ligne;
*
* ARGUMENTS EN SORTIE :
*       T_STR           Ligne;
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   parcours de la chaine
*
------------------------------------------------------*/
{
	int i = 0, j = 0, k = 0;

	/*A on supprime les espaces de fin de ligne */
	k = strlen(Ligne);
	for (i = 0, j = 0; i < k; i++)
	{
		if (Ligne[i] != ' ')
			break;
	}
	if (j != k - 1) /* Il n'y a pas que des blancs */
	{
		i = k;
		for (j = i - 1; j >= 0; j--)
		{
			if (Ligne[j] == ' ')
				Ligne[j] = '\0';
			else
				break;
		}
	}
}

static char *epmv_majuscules(char *Texte)
{
	int i;
	int l;

	/*A
	** on passe en majuscules SANS accent
	*/
	l = (int)strlen(Texte);
	for (i = 0; i < l; i++)
		switch ((int)Texte[i])
		{
		case -64:
			Texte[i] = 'A';
			break;
		case -56:
			Texte[i] = 'A';
			break;
		case -59:
			Texte[i] = 'E';
			break;
		case -55:
			Texte[i] = 'E';
			break;
		case -63:
			Texte[i] = 'E';
			break;
		case -47:
			Texte[i] = 'I';
			break;
		case -62:
			Texte[i] = 'O';
			break;
		case -53:
			Texte[i] = 'U';
			break;
		case -61:
			Texte[i] = 'U';
			break;
		default:
			Texte[i] = toupper(Texte[i]);
		}
	return (Texte);
}
