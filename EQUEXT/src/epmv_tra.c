/*E*/
/* Fichier : $Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $        Release : $Revision: 1.52 $        Date : $Date: 2019/05/21 11:04:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de traitement des trames recues de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	06 Oct 1994	: Creation
* Nagiel.E	version 1.2	23 Nov 1994	:
* Nagiel.E	version 1.3	23 Nov 1994	:
* Nagiel.E	version 1.4	29 Nov 1994	:
* Nagiel.E	version 1.5	16 Dec 1994	:
* Mismer.D	version 1.6	23 Jan 1995	:
* Mismer.D	version 1.7	30 Jan 1995	:
* Mismer.D	version 1.8	31 Jan 1995	:
* Mismer.D	version 1.9	09 Fev 1995	:
* Mismer.D	version 1.10	21 Fev 1995	:
* Mismer.D 	version 1.11	08 Mar 1995	:
* Mismer.D	version 1.12	17 Mar 1995	:
* Mismer.D	version 1.13	09 Avr 1995	:
* Volcic.F	version 1.14	14 Avr 1995	: Modif traitement alerte affichage PMV et Picto
* Volcic.F	version 1.15	14 Avr 1995	: Modif traitement alerte affichage PMV et Picto
* Mismer.D	version 1.16	21 Avr 1995	:
* Mismer.D	version 1.17	21 Avr 1995	:
* Mismer.D	version 1.18	21 Avr 1995	:
* Volcic.F	version 1.19	24 Avr 1995	:
* Volcic.F	version 1.20	26 Avr 1995	:
* Volcic.F	version 1.21	02 Mai 1995	: Modif traitement recup etat Picto
* Fontaine.C	version 1.22	18 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.23	18 Mai 1995	: Modif Envoi etat PMV a l'init
* Mismer.D	version 1.24	15 Jun 1995	: Envoi etat PMV apres coupure liaison PMV
* Volcic.F	version 1.25	26 Jul 1995	: Modif  action qualite
* Mismer.D	version 1.26	16 Aou 1995	: Ajout trace fichier cmd et compte-rendu
* Mismer.D	version 1.27	28 Sep 1995	: Ajout alerte passage en local
* Mismer.D	version 1.28	23 Oct 1995	: Ajout d'un ST a l'init
* Mismer.D	version 1.29	29 Nov 1995	: Modif pour connection en mode local du PMV (TRM)
* Mismer.D	version 1.30	15 Fev 1996	: MOdif pour rafrichiessement etat picto (DEM/925)
* Mismer.D	version 1.31	30 Mai 1996	: Ajout appel fonction compteur de marche XZMA01 (DEM/643)
* Mismer.D      version 1.32    29 Aou 1996     : Ajout ligne direction pour PMV 4 lignes (DEM/1162)
* Mismer.D      version 1.33    18 Sep 1996     : Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.34	10 Fev 1997	: Modif TDP (decodage status )(DEM/1395)
* Mismer.D	version 1.35	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D      version 1.36    10 Mar 1998     : Modif pour PMV PICTO avec la meme RGS (DEM/1618)
* Mismer.D      version 1.37    30 Mai 2000     : Modif recherche PMV avant Picto
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	08/06/10	; linux DEM934
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.41
* JPL	22/03/11 : Migration architecture HP ia64 (DEM 975) : retour de C.R. OK ou NOK (em_traitement_trame)  1.42
* JPL	22/03/11 : Migration architecture HP ia64 (DEM 975) : Message suite a trame status: determiner le type PMV  1.43
* JMG	25/02/13 : ajout trim 1.44
* JPL	14/10/15 : Etat PMV : prise en compte du retour 'Neutre 3 points' (DEM 1135 complement)  1.45
* JPL	09/11/15 : Etat PMV : Sur retour 'Neutre 3 points', conserver le TEXTE (DEM 1135 complement)  1.46
* PNI	28/08/17 : correctif lecture cligno DEM1233 1.47
* JPL	07/12/17 : Trame de reponse d'etat d'affichage PMV : gestion reponse PMV sur blocs multiples (DEM 1258)  1.48
* JPL	15/12/17 : Trame de reponse d'etat d'affichage PMV : gestion reponse Picto sur blocs multiples (DEM 1258)  1.49
* JMG	14/08/18 : PMV 2 lignes suppression SAGA DEM1306 1.50
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* PNI	31/03/2022 : Ajout alerte en cas de pilotage correctif
* LCL	25/10/2022 : JIRA 423 plantage tepmv POLTDP100 Ajout traces
* JPL	30/06/23 : Ecarts negliges entre etat commande / courant des Pictos pour maj. Temps de parcours en bouchon (SAE-460) 
* GGY	16/06/2023	:	Ajout EnvoyerTrameIP et correction panonceau (DEM472)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"
#include "xzma.h"

#include "epmv_tra.h"

/* definitions de constantes */

#define CM_SEPARATEUR_LIGNE "\r\n"
#define CM_DEBUT_LIGNE "K AM="
#define CM_DEBUT_LIGNE_7 "AM="
#define CM_LEC_NUM_LIGNE CM_DEBUT_LIGNE "%d"
#define CM_LEC_NUM_LIGNE_7 CM_DEBUT_LIGNE_7 "%d.0"
#define CM_DEBUT_CONTRASTE "EC="
#define CM_LEC_CONTRASTE CM_DEBUT_CONTRASTE "%c"
#define CM_DEBUT_CLIGNO "CL="
#define CM_LEC_CLIGNO CM_DEBUT_CLIGNO "%s"
#define CM_DEBUT_AFF "AF="
#define CM_LEC_AFF CM_DEBUT_AFF "%s"
#define CM_LIGNE_UNE 1
#define CM_LIGNE_DEUX 2
#define CM_LIGNE_TROIS 3
#define CM_LIGNE_QUATRE 4
#define CM_LIGNE_CINQ 5
#define CM_LIGNE_SIX 6
#define CM_LIGNE_SEPT 7

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
char *trim(char *);

/* declaration de fonctions internes */
int affecte_etat_pmv(char *, XDY_Etat_PMV *, EPMV_DONNEES_TYPE_PMV *, char *, int, XDY_Octet);
int affecte_etat_picto(char *, XDY_Etat_Picto *, char *, XDY_TypePicto );

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  envoi un msg d'alarme
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int recherche_eqt(char va_adrrgs[10],
				  XDY_Mot va_TypeRep,
				  XDY_Mot va_TypeTrame,
				  XDY_Mot *va_noeqt,
				  XDY_District *va_SiteGestion)
/*
* ARGUMENTS EN ENTREE :
*
* va_horodate : horodate de l'eqt
* va_type_err : 1 non reponse    2 err crc
* va_nb_err : nb err de l'eqt
* va_adrrgs : adresse rgs de l'eqt
*
* ARGUMENTS EN SORTIE :
*
* va_noeqt : no eqt traite
*
* CODE RETOUR :
*
* XDC_VRAI : picto
* XDC_FAUX : pmv
* 3 : eqt non trouve
* CONDITION D'UTILISATION
*
* FONCTION
*  Envoi un msg d'alarme suivant le type d'eqt et le type de l'alarme
*
------------------------------------------------------*/
{
	static char *version = "$Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $      :recherche_eqt ";

	EPMV_DONNEES_PMV *pl_ListePMV;
	EPMV_DONNEES_Picto *pl_ListePicto;
	int vl_typeeqt = 0;
	char *vl_status[32];

	/*A Recherche du pmv concerne */
	for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
		 pl_ListePMV = pl_ListePMV->Suivant)
	{
		if (
			((!strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePMV->Config.AdresseRGS, va_adrrgs))) ||
			((strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePMV->Config.AdresseIP, va_adrrgs))))
		{
			/* Sauvegarde donnees pmv */
			vl_typeeqt = XZECC_TYPE_EQT_PMV;
			pl_ListePMV->Etat.Numero = pl_ListePMV->Config.Numero.NoEqt;
			*va_noeqt = pl_ListePMV->Config.Numero.NoEqt;
			*va_SiteGestion = pl_ListePMV->Config.SiteGestion;
			strcpy(va_adrrgs, pl_ListePMV->Config.AdresseRGS);
			if ((va_TypeTrame == XZEXC_TRAME_FEN_LCR) || (va_TypeRep != XZEXC_TRAME_NON_TRANSM))
				pl_ListePMV->EtatTrans = EPMVC_EQT_LIBRE;
			else
				pl_ListePMV->EtatTrans = EPMVC_EQT_PANNE;
			break;
		}
	}

	/*A Si ce n'est pas un pmv */
	if (pl_ListePMV == NULL)
	{
		/*A Recherche du picto concerne */
		for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL; pl_ListePicto = pl_ListePicto->Suivant)
		{
			if (!strcmp(pl_ListePicto->Config.AdresseRGS, va_adrrgs))
			{
				/* Sauvegarde donnees picto */
				vl_typeeqt = XZECC_TYPE_EQT_PICTO;
				pl_ListePicto->Etat.Numero = pl_ListePicto->Config.Numero.NoEqt;
				*va_noeqt = pl_ListePicto->Config.Numero.NoEqt;
				*va_SiteGestion = pl_ListePicto->Config.SiteGestion;
				if ((va_TypeTrame == XZEXC_TRAME_FEN_LCR) || (va_TypeRep != XZEXC_TRAME_NON_TRANSM))
					pl_ListePicto->EtatTrans = EPMVC_EQT_LIBRE;
				else
					pl_ListePicto->EtatTrans = EPMVC_EQT_PANNE;
				break;
			}
		}
	}

	return (vl_typeeqt);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  envoi message RTwork etat d'affichage PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ep_envoi_affichage_PMV(XDY_Etat_PMV va_EtatPMV,
							int va_NumEqt,
							int va_NbLigne,
							XDY_District va_Site)
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
	static char *version = "$Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $      : ep_envoi_affichage_PMV ";

	XZSCT_NomTache vl_NomTache = "";
	XDY_NomMachine vl_NomMachine = "";
	XDY_Datagroup pl_DG;

	/*A Construction du datagroup */

	XDG_EncodeDG2(pl_DG, XDG_ETAT_PMV, XZSC23_Nom_From_Numero(va_Site));

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PMV */
	if (!TipcSrvMsgWrite(pl_DG,
						 XDM_IdentMsg(XDM_ETAT_PMV),
						 XDM_FLG_SRVMSGWRITE,
						 T_IPC_FT_INT2, va_NumEqt,
						 T_IPC_FT_STR, va_EtatPMV.Texte1,
						 T_IPC_FT_INT2, va_EtatPMV.Affichage1,
						 T_IPC_FT_STR, va_EtatPMV.Alternat1,
						 T_IPC_FT_CHAR, va_EtatPMV.Clign1,
						 T_IPC_FT_STR, va_EtatPMV.Texte2,
						 T_IPC_FT_INT2, (va_NbLigne >= 2) ? va_EtatPMV.Affichage2 : XDC_PMV_NON_EXIST,
						 T_IPC_FT_STR, va_EtatPMV.Alternat2,
						 T_IPC_FT_CHAR, va_EtatPMV.Clign2,
						 T_IPC_FT_STR, va_EtatPMV.Texte3,
						 T_IPC_FT_INT2, (va_NbLigne >= 3) ? va_EtatPMV.Affichage3 : XDC_PMV_NON_EXIST,
						 T_IPC_FT_STR, va_EtatPMV.Alternat3,
						 T_IPC_FT_CHAR, va_EtatPMV.Clign3,
						 T_IPC_FT_STR, va_EtatPMV.Texte4,
						 T_IPC_FT_INT2, (va_NbLigne == EPMVC_PMV_4LIGNES) ? va_EtatPMV.Affichage4 : XDC_PMV_NON_EXIST,
						 T_IPC_FT_STR, va_EtatPMV.Alternat4,
						 T_IPC_FT_CHAR, va_EtatPMV.Clign4,
						 T_IPC_FT_CHAR, (va_EtatPMV.Flash == XDC_PMV_FLASH) ? XDC_VRAI : XDC_FAUX,
						 NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PMV non effectue \n");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PMV effectue va_EtatPMV.Flash=%d.\n", va_EtatPMV.Flash);
	}
	/* Mise jour compteur de marche equipement */
	XZSC_06NomTache(vl_NomTache);
	XZSC_07NomMachine(vl_NomMachine);
	if ((va_EtatPMV.Texte1[0] == '\0') && (va_EtatPMV.Texte2[0] == '\0') &&
		(va_EtatPMV.Texte3[0] == '\0') && (va_EtatPMV.Texte4[0] == '\0'))
	{
		XZMA01_Maj_duree_marche(va_NumEqt,
								XDC_EQT_PMV,
								XDC_FAUX,
								vl_NomTache,
								vl_NomMachine);
	}
	else
	{
		XZMA01_Maj_duree_marche(va_NumEqt,
								XDC_EQT_PMV,
								XDC_VRAI,
								vl_NomTache,
								vl_NomMachine);
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  envoi message RTwork etat d'affichage Picto
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ep_envoi_affichage_Picto(XDY_Etat_Picto va_EtatPicto,
							  int va_NumEqt,
							  XDY_District va_Site)
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
	static char *version = "$Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $      : ep_envoi_affichage_Picto ";

	XZSCT_NomTache vl_NomTache = "";
	XDY_NomMachine vl_NomMachine = "";
	XDY_Datagroup pl_DG;

	/*A Construction du datagroup */
	XDG_EncodeDG2(pl_DG, XDG_ETAT_PICTO, XZSC23_Nom_From_Numero(va_Site));

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PICTO */
	if (!TipcSrvMsgWrite(pl_DG,
						 XDM_IdentMsg(XDM_ETAT_PICTO),
						 XDM_FLG_SRVMSGWRITE,
						 T_IPC_FT_INT2, va_NumEqt,
						 T_IPC_FT_STR, va_EtatPicto.Texte,
						 T_IPC_FT_STR, va_EtatPicto.Picto,
						 T_IPC_FT_INT2, va_EtatPicto.Clign,
						 NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Picto non effectue.");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Picto effectue.");
	}

	/* Mise jour compteur de marche equipement */
	XZSC_06NomTache(vl_NomTache);
	XZSC_07NomMachine(vl_NomMachine);
	if (!strcmp(va_EtatPicto.Picto, XDC_PICTO_PASPICTO))
	{
		XZMA01_Maj_duree_marche(va_NumEqt,
								XDC_EQT_PCT,
								XDC_FAUX,
								vl_NomTache,
								vl_NomMachine);
	}
	else
	{
		XZMA01_Maj_duree_marche(va_NumEqt,
								XDC_EQT_PCT,
								XDC_VRAI,
								vl_NomTache,
								vl_NomMachine);
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Analyse de la trame >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame(int va_socket)

/*
* ARGUMENTS EN ENTREE :
*
* va_socket : no socket sur laquelle le message est arrive
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*  analyse la trame et execute les actions correspondantes
*
------------------------------------------------------*/
{
	static char *version = "$Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $      :em_traitement_trame ";

	int vl_IndCnx = 0,
		vl_LgMsg = 0;
	char *pl_trame_ptr = NULL;
	char vl_entete1[10],
		vl_entete2[10],
		vl_adrrgs[10];
	int vl_typetrame;
	int vl_typ_rep;
	XZEXT_MSG_SOCKET vl_Msg, pl_trame;
	int vl_param;
	int vl_nbnonrep,
		vl_nberrcrc;
	XDY_Etat_PMV vl_etatPMV = {0, "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
							   XZEXC_TEXTE_PMV_NON_REP, XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
							   "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
							   "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
							   XDC_PMV_PASFLASH},
				 vl_etat2PMV = {0, "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
								XZEXC_TEXTE_PMV_NON_REP, XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
								"", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
								"", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
								XDC_PMV_PASFLASH},
				 vl_EtatDefautPMV = {0, "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
									 XZEXC_TEXTE_PMV_NON_REP, XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
									 "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
									 "", XDC_PMV_TEXTE, XDC_PMV_PASCLIGNO, "",
									 XDC_PMV_PASFLASH};
	XDY_Etat_Picto vl_etatPicto = {0, "", XDC_PICTO_PASPICTO, XDC_PICTO_PASCLIGNO, 0},
				   vl_etat2Picto = {0, "", XDC_PICTO_PASPICTO, XDC_PICTO_PASCLIGNO, 0},
				   vl_EtatDefautPicto = {0, "", XDC_PICTO_PASPICTO, XDC_PICTO_PASCLIGNO, 0};
	EPMV_DONNEES_PMV *pl_ListePMV;
	EPMV_DONNEES_TYPE_PMV *pl_ListeTypePMV;
	EPMV_DONNEES_Picto *pl_ListePicto;
	EPMV_DONNEES_TYPE_Picto *pl_ListeTypePicto;
	char vl_cligno[32];
	char vl_contraste;
	XDY_Datagroup pl_DG;
	int vl_js;
	XDY_Horodate vl_horodate;
	int vl_nocmd;
	char vl_horod1[20],
		vl_horod2[9];
	XZSMT_Horodate pl_Horodatedecompo;
	char *vl_status[32];
	XDY_Mot vl_noeqt;
	int vl_valret, vl_retour;
	int vl_typeeqt;
	XZEXT_TEXTE_STATUS vl_tabstatus[16];
	int vl_Alerte = 0;
	char vl_NbErr[10] = "";
	XDY_District vl_sitegestion;

	/*A
	 *  Lecture du message recu dans la socket appelante
	 */

	if (ex_lire_msg(va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx) != XDC_OK)
	{
		/* Ecriture trace Erreur lecture message socket. */
		XZST_03EcritureTrace(XZSTC_WARNING, " Erreur lecture message socket. ");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_INFO, " -------------tra mes=%s", vl_Msg);
		/*A Scan du message provenant de la socket */
		vl_param = sscanf(vl_Msg, "%s %s %s %s %d %d %d %s %d %d", vl_entete1,
						  vl_entete2,
						  vl_horod1,
						  vl_horod2,
						  &vl_nbnonrep,
						  &vl_nberrcrc,
						  &vl_nocmd,
						  vl_adrrgs,
						  &vl_typetrame,
						  &vl_typ_rep);

		/*A Retourne a l'appelant si simulation */
		if (vg_Simulation)
			return (XDC_OK);

		if (vl_param != 10)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Lecture message socket impossible mes=%s", vl_Msg);
			return (XDC_NOK);
		}

		/*A Conversion horodate */
		sprintf(pl_Horodatedecompo, "%s %s", vl_horod1, vl_horod2);

		XZSM_13ConversionHorodateSec(pl_Horodatedecompo, &vl_horodate);

		/*A Recherche l'equipement qui emet une reponse */
		if ((vl_typeeqt = recherche_eqt(vl_adrrgs, vl_typ_rep, vl_typetrame, &vl_noeqt, &vl_sitegestion)) == 0)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Pmv ou Picto Non trouve en memoire mes=%s", vl_Msg);
			return (XDC_NOK);
		}

		/*A Traitement des alarmes transmission avec l'equipement */
		ex_traite_alarme_com(vg_NomMachine,
							 vl_typeeqt,
							 vl_noeqt,
							 vl_typ_rep,
							 vl_nbnonrep,
							 vl_nberrcrc,
							 vl_typetrame,
							 vg_Mode_Fonct,
							 vl_sitegestion);

		/*A On se positionne sur le debut de la trame */
		pl_trame_ptr = strstr(vl_Msg, XZEXC_SEP_TRAME);
		if (pl_trame_ptr == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Lecture message socket impossible mes=%s", vl_Msg);
			return (XDC_NOK);
		}

		strcpy(pl_trame, strtok(pl_trame_ptr + 4, XZEXC_FIN_MSG_SOCK));
		pl_trame[strlen(pl_trame) - 1] = '\0';

		/*A Recherche du type de trame */
		switch (vl_typetrame)
		{
		/*A C'est une trame de reponse pour la fenetre LCR */
		case XZEXC_TRAME_FEN_LCR:
		{
			/* Appel de ex_mrcr suvant le type d'eqt */
			if (vl_typeeqt == XZECC_TYPE_EQT_PICTO)
			{
				/* Recherche du picto concerne */
				for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL; pl_ListePicto = pl_ListePicto->Suivant)
				{
					if (!strcmp(pl_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
					{
						ex_ret_LCR(pl_trame, vl_adrrgs, &pg_DebutListePictoFLcr, vl_typ_rep);
					}
					else
					{
						ex_ret_LCR_IP(pl_trame, vl_adrrgs, &pg_DebutListePictoFLcr, vl_typ_rep);
					}
				}
			}

			if (vl_typeeqt == XZECC_TYPE_EQT_PMV)
			{
				/* Recherche du pmv concerne */
				for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL; pl_ListePMV = pl_ListePMV->Suivant)
				{
					if (!strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
					{
						ex_ret_LCR(pl_trame, vl_adrrgs, &pg_DebutListePMVFLcr, vl_typ_rep);
					}
					else
					{
						ex_ret_LCR_IP(pl_trame, vl_adrrgs, &pg_DebutListePMVFLcr, vl_typ_rep);
					}
				}
			}
			break;
		}

		/*A C'est une trame de reponse status temps reel */
		case XZEXC_TRAME_STAT_TR:
		{
			if (vl_typeeqt == XZECC_TYPE_EQT_PICTO)
			{

				/*A Recherche du picto concerne */
				for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL; pl_ListePicto = pl_ListePicto->Suivant)
				{
					if (!strcmp(pl_ListePicto->Config.AdresseRGS, vl_adrrgs))
					{
						if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM)
							return (XDC_OK);

						/*A Si le status TR est ko -> demande du status complet */
						if ((((*pl_trame != '@') && (*(pl_trame + 1) != '@')) && (pl_ListePicto->StatusTR == XDC_OK)) ||
							(((*pl_trame == '@') || (*(pl_trame + 1) == '@')) && (pl_ListePicto->StatusTR != XDC_OK)))
						{
							if (!strcmp(pl_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
								EnvoyerTrame("ST\0",
											 XZEXC_TRAME_STAT_COMP,
											 vl_adrrgs,
											 XZEXC_NOCMD,
											 XZEXC_PRIORITEFAIBLE,
											 va_socket);
							else
												EnvoyerTrameIP("ST\0",
													XZEXC_TRAME_STAT_COMP,
													pl_ListePicto->Config.AdresseIP,
													pl_ListePicto->Config.Port,
													vl_adrrgs,
													XZEXC_NOCMD,
													XZEXC_PRIORITEFAIBLE,
													va_socket);
							pl_ListePicto->EtatTrans = EPMVC_EQT_OCCUP;
						}
						if (!strcmp(pl_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
							pl_ListePicto->StatusTR = ((*pl_trame == '@') || (*(pl_trame + 1)) == '@') ? XDC_OK : XDC_NOK;
						break;
					}
				}
			}
			else
			{
				/*A Recherche du pmv concerne */
				for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
					 pl_ListePMV = pl_ListePMV->Suivant)
				{
					if (!strcmp(pl_ListePMV->Config.AdresseRGS, vl_adrrgs))
					{
						/*A Si le status TR est ko -> demande du status complet */
						if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM)
							return (XDC_OK);

						if ((((*pl_trame != '@') && (*(pl_trame + 1) != '@')) && (pl_ListePMV->StatusTR == XDC_OK)) ||
							(((*pl_trame == '@') || (*(pl_trame + 1) == '@')) && (pl_ListePMV->StatusTR != XDC_OK)))
						{
							if (!strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
								EnvoyerTrame("ST\0",
											 XZEXC_TRAME_STAT_COMP,
											 vl_adrrgs,
											 XZEXC_NOCMD,
											 XZEXC_PRIORITEFAIBLE,
											 va_socket);
							else
								EnvoyerTrameIP("ST\0",
											   XZEXC_TRAME_STAT_COMP,
											   pl_ListePMV->Config.AdresseIP,
											   pl_ListePMV->Config.Port,
											   vl_adrrgs,
											   XZEXC_NOCMD,
											   XZEXC_PRIORITEFAIBLE,
											   va_socket);
							pl_ListePMV->EtatTrans = EPMVC_EQT_OCCUP;
						}
						pl_ListePMV->StatusTR = ((*pl_trame == '@') || (*(pl_trame + 1)) == '@') ? XDC_OK : XDC_NOK;
						break;
					}
				}
			}
			break;
		}

		/*A C'est une trame de reponse etat d'affichage PMV */
		case XZEXC_TRAME_ETAT_AFF_PMV:
		{
			/*B Recherche du nombre de caracteres du PMV traite */
			for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
				 pl_ListePMV = pl_ListePMV->Suivant)
			{
				if (!strcmp(pl_ListePMV->Config.AdresseRGS, vl_adrrgs))
				{
					break;
				}
			}
			if (pl_ListePMV == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, " Pmv Non trouve en memoire mes=%s", vl_Msg);
				return (XDC_NOK);
			}

			for (pl_ListeTypePMV = pg_debutTypePMV; pl_ListeTypePMV != NULL;
				 pl_ListeTypePMV = pl_ListeTypePMV->Suivant)
			{
				if (pl_ListeTypePMV->TypePMV == pl_ListePMV->Config.TypePMV)
				{
					break;
				}
			}
			if (pl_ListeTypePMV == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, " Type Pmv Non trouve en memoire mes=%s", vl_Msg);
				return (XDC_NOK);
			}

			/* JIRA 423 plantage tepmv POLTDP100 Ajout traces */
			XZST_03EcritureTrace(XZSTC_INFO, " --> CONCATENATION strlen(pl_ListePMV->Trame)=%d strlen(pl_trame)=%d", strlen(pl_ListePMV->Trame), strlen(pl_trame));
			/*B Ajouter le bloc en fin de la trame construite */
			strcat(pl_ListePMV->Trame, pl_trame);
			XZST_03EcritureTrace(XZSTC_INFO, " --> APRES CONCATENATION strlen(pl_ListePMV->Trame)=%d", strlen(pl_ListePMV->Trame));

			if (vl_typ_rep == XZEXC_REP_BLC_INT)
			{
				break;
			}
			else if (vl_typ_rep == XZEXC_REP_BLC_FIN)
			{
				/*A Envoi d'une demande ST a l'init */
				if (vl_nocmd == XZEXC_CMD_INIT)
				{
					if (!strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE))
						EnvoyerTrame("ST\0",
									 XZEXC_TRAME_STAT_COMP,
									 vl_adrrgs,
									 XZEXC_NOCMD,
									 XZEXC_PRIORITEFAIBLE,
									 va_socket);
					else
						EnvoyerTrameIP("ST\0",
									   XZEXC_TRAME_STAT_COMP,
									   pl_ListePMV->Config.AdresseIP,
									   pl_ListePMV->Config.Port,
									   vl_adrrgs,
									   XZEXC_NOCMD,
									   XZEXC_PRIORITEFAIBLE,
									   va_socket);
					pl_ListePMV->EtatTrans = EPMVC_EQT_OCCUP;
				}

				if ((vl_nocmd != XZEXC_NOCMD) && (vl_nocmd != XZEXC_CMD_INIT))
					ex_ecriture_trace_cmd(XDC_EQT_PMV,
										  pl_ListePMV->Config.Numero.NoEqt,
										  NULL,
										  pl_ListePMV->Trame,
										  vl_nocmd);

				/* Lecture vl_horodate */
				XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);

				/*B on initialise les variables avec l'etat commande et l'etat courant */
				/*B ceci va permetre la comparaison */
				memcpy(&vl_etatPMV, &pl_ListePMV->Etat, sizeof(XDY_Etat_PMV));
				memcpy(&vl_etat2PMV, &pl_ListePMV->Etat_Commande, sizeof(XDY_Etat_PMV));

				/*B On recupere l'etat affiche sur le pmv */
				vl_retour = affecte_etat_pmv(pl_ListePMV->Trame, &vl_etatPMV, pl_ListeTypePMV, &vl_contraste, pl_ListeTypePMV->NbLigne, pl_ListePMV->Config.TypeLCR);
				if (vl_retour == XDC_NOK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, " Trame incomplete mes=%s", vl_Msg);
					if (vl_nocmd > XZEXC_NOCMD)
					{
						/* envoi de CR NOK */
						xzat25_CompteRendu_PMV(vl_nocmd, vl_horodate, XDC_NOK, pl_ListePMV->Config.SiteGestion);
						ex_ecriture_trace_cmd(XDC_EQT_PMV,
											  pl_ListePMV->Config.Numero.NoEqt,
											  NULL,
											  pl_ListePMV->Trame,
											  vl_nocmd);

						XZEZ03_CompteRendu_TDP(vl_nocmd, XDC_NOK, pl_ListePMV->Config.Numero.NoEqt, vg_NomMachine);
						XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF pmv <> etat comm -> CR NOK pour cmd:%d", vl_nocmd);
					}
					strcpy(pl_ListePMV->Trame, "");
					return (XDC_NOK);
				}

				vl_retour = affecte_etat_pmv(pl_ListePMV->Trame, &vl_etat2PMV, pl_ListeTypePMV, &vl_contraste, pl_ListeTypePMV->NbLigne, pl_ListePMV->Config.TypeLCR);

				/*B COMPARAISON ENTRE ETAT COURANT ET ETAT PRECEDENT */
				/*JMG     if ( (memcmp(&vl_etatPMV,&pl_ListePMV->Etat,sizeof(XDY_Etat_PMV))) &&
						  !(pl_ListePMV->Etat_Service & XDC_EQT_HORS_SRV))
					 {*/
				/*B Copie du nouvel etat d'affichage pmv ds l'etat courant */
				memcpy(&pl_ListePMV->Etat, &vl_etatPMV, sizeof(XDY_Etat_PMV));

				/*A Envoi de l'etat d'affichage du PMV */
				ep_envoi_affichage_PMV(vl_etatPMV, pl_ListePMV->Config.Numero.NoEqt, pl_ListeTypePMV->NbLigne,
									   pl_ListePMV->Config.SiteGestion);
				/*  }*/

				/*B COMPARAISON ENTRE ETAT COURANT ET ETAT COMMANDE */
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Texte1, pl_ListePMV->Etat_Commande.Texte1);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Texte2, pl_ListePMV->Etat_Commande.Texte2);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Texte3, pl_ListePMV->Etat_Commande.Texte3);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Texte4, pl_ListePMV->Etat_Commande.Texte4);

				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Alternat1, pl_ListePMV->Etat_Commande.Alternat1);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Alternat2, pl_ListePMV->Etat_Commande.Alternat2);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Alternat3, pl_ListePMV->Etat_Commande.Alternat3);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%s//%s.", vl_etat2PMV.Alternat4, pl_ListePMV->Etat_Commande.Alternat4);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Clign1, pl_ListePMV->Etat_Commande.Clign1);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Clign2, pl_ListePMV->Etat_Commande.Clign2);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Clign3, pl_ListePMV->Etat_Commande.Clign3);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Clign4, pl_ListePMV->Etat_Commande.Clign4);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Affichage1, pl_ListePMV->Etat_Commande.Affichage1);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Affichage2, pl_ListePMV->Etat_Commande.Affichage2);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Affichage3, pl_ListePMV->Etat_Commande.Affichage3);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Affichage4, pl_ListePMV->Etat_Commande.Affichage4);

				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Flash, pl_ListePMV->Etat_Commande.Flash);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Destination, pl_ListePMV->Etat_Commande.Destination);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.DestinationAlternat, pl_ListePMV->Etat_Commande.DestinationAlternat);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.DepassSeuil, pl_ListePMV->Etat_Commande.DepassSeuil);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.DestinationNoeud, pl_ListePMV->Etat_Commande.DestinationNoeud);
				XZST_03EcritureTrace(XZSTC_WARNING, ".%d//%d.", vl_etat2PMV.Numero, pl_ListePMV->Etat_Commande.Numero);


				if (memcmp(&vl_etat2PMV, &pl_ListePMV->Etat_Commande, sizeof(XDY_Etat_PMV)) &&
					(pl_ListePMV->NumDerCmd == vl_nocmd))
				{
					/* c'est different */
					/* filtrage de l'alerte si eqt hs ou inactif */
					if (!(pl_ListePMV->Etat_Service) &&
						(vl_nocmd != XZEXC_NOCMD) &&
						(vl_nocmd != XZEXC_CMD_INIT) &&
						(vl_nocmd != XZEXC_CMD_HORS_BASE))
					{
						/* envoi de CR NOK */
						xzat25_CompteRendu_PMV(vl_nocmd, vl_horodate, XDC_OK, pl_ListePMV->Config.SiteGestion);
						XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF pmv <> etat comm MAIS CR OK pour cmd:%d vl_nocmd=%d", pl_ListePMV->NumCmd, vl_nocmd);
						memcpy(&pl_ListePMV->Etat_Commande, &vl_etat2PMV, sizeof(XDY_Etat_PMV));
					}
					if (vl_nocmd != XZEXC_CMD_INIT)
					{
						XZEZ03_CompteRendu_TDP(vl_nocmd, XDC_NOK, pl_ListePMV->Config.Numero.NoEqt, vg_NomMachine);

					}

					if (!(pl_ListePMV->Etat_Service) &&
						(vl_nocmd != XZEXC_CMD_INIT) &&
						(pl_ListePMV->NumCmd != XZEXC_NOCMD))
					/*(pl_ListePMV->NumCmd <= XZEXC_NOCMD)*/
					{
						/* static int no_alerte_aff_pmv = 1; trace_alerte(no_alerte_aff_pmv++, &vl_etat2PMV, &pl_ListePMV->Etat_Commande); */
						ex_env_alerte(vl_horodate,
									  XDC_EQT_PMV,
									  pl_ListePMV->Config.Numero.NoEqt,
									  XZAAC_AFF_DIF,
									  vg_Mode_Fonct,
									  pl_ListePMV->Config.SiteGestion,
									  "affichage PMV",
									  NULL);
						memcpy(&pl_ListePMV->Etat_Commande, &vl_etat2PMV, sizeof(XDY_Etat_PMV));
					}
				}
				else
				{
					if (!(pl_ListePMV->Etat_Service) &&
						(vl_nocmd != XZEXC_NOCMD) &&
						(vl_nocmd != XZEXC_CMD_INIT) &&
						(vl_nocmd != XZEXC_CMD_HORS_BASE))
					{
						/* c'est pareil */
						/* envoi de CR OK */
						xzat25_CompteRendu_PMV(vl_nocmd, vl_horodate, XDC_OK, pl_ListePMV->Config.SiteGestion);
						XZEZ03_CompteRendu_TDP(vl_nocmd, XDC_OK, pl_ListePMV->Config.Numero.NoEqt, vg_NomMachine);
						XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF pmv = etat comm -> CR OK pour cmd:%d", vl_nocmd);
						if (vl_nocmd < XZEXC_NOCMD)
							pl_ListePMV->NumCmd = XZEXC_NOCMD;
						else
							pl_ListePMV->NumCmd = vl_nocmd;
					}
				}

				/*B si le Contraste a change -> envoi du message XDM_ETAT_Contraste  */
				if (((vl_contraste == EPMVC_CONTRASTE) && (pl_ListePMV->Contraste == XDC_CONTRASTENORMAL)) ||
					((vl_contraste != EPMVC_CONTRASTE) && (pl_ListePMV->Contraste == XDC_CONTRASTEBRILLANT)))
				{
					pl_ListePMV->Contraste = (vl_contraste == EPMVC_CONTRASTE) ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL;
					/* Envoi XDM_Etat_Contraste  */
					/* Construction du datagroup */
					if (!(pl_ListePMV->Etat_Service & XDC_EQT_HORS_SRV))
					{
						XDG_EncodeDG2(pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite);
						if (!TipcSrvMsgWrite(pl_DG, XDM_IdentMsg(XDM_ETAT_Contraste), XDM_FLG_SRVMSGWRITE, T_IPC_FT_INT2, XDC_EQT_PMV, T_IPC_FT_INT2, pl_ListePMV->Config.Numero.NoEqt, T_IPC_FT_INT2, pl_ListePMV->Contraste, NULL))
						{
							/* Trace envoi message XDM_ETAT_Contraste non effectue */
							XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste non effectue \n");
						}
						else
						{
							/* Trace envoi message XDM_ETAT_Contraste effectue */
							XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste effectue \n");
						}
					}
				}

				if ((vl_nocmd == XZEXC_CMD_HORS_BASE) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					!(pl_ListePMV->Etat_Service) &&
					(pl_ListePMV->ContrasteCmd = !pl_ListePMV->Contraste))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Appel ex_env_alerte");
					ex_env_alerte(vl_horodate,
								  XDC_EQT_PMV,
								  pl_ListePMV->Config.Numero.NoEqt,
								  XZAAC_AFF_DIF,
								  vg_Mode_Fonct,
								  pl_ListePMV->Config.SiteGestion,
								  "Contraste PMV",
								  NULL);
				}
				if (vl_nocmd == XZEXC_CMD_INIT)
				{
					memcpy(&pl_ListePMV->Etat, &vl_etatPMV, sizeof(XDY_Etat_PMV));
					memcpy(&pl_ListePMV->Etat_Commande, &vl_etatPMV, sizeof(XDY_Etat_PMV));
				}

				/*B Soumettre la trame complete a l'analyse de reponse d'etat Picto, et la reinitialiser */
				strcpy(pl_trame, pl_ListePMV->Trame);
				strcpy(pl_ListePMV->Trame, "");
			}

			if (pl_ListePMV->Config.Eqt_PMV_Picto == XDC_FAUX)
				break;
			XZST_03EcritureTrace(XZSTC_WARNING, "pl_ListeTypePMV->Picto %d", pl_ListeTypePMV->Picto);
			if (pl_ListeTypePMV->Picto == 0)
				break;
		}

		/*A C'est une trame de reponse etat d'affichage Picto */
		case XZEXC_TRAME_ETAT_AFF_PICTO:
		{
			/*B Recherche du nombre de caracteres du Picto traite */
			for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL;
				 pl_ListePicto = pl_ListePicto->Suivant)
			{
				if (!strcmp(pl_ListePicto->Config.AdresseRGS, vl_adrrgs))
				{
					break;
				}
			}
			if (pl_ListePicto == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, " Picto Non trouve en memoire mes=%s", vl_Msg);
				return (XDC_NOK);
			}

			for (pl_ListeTypePicto = pg_debutTypePicto; pl_ListeTypePicto != NULL;
				 pl_ListeTypePicto = pl_ListeTypePicto->Suivant)
			{
				if (pl_ListeTypePicto->TypePicto == pl_ListePicto->Config.TypePicto)
				{
					break;
				}
			}
			if (pl_ListeTypePicto == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, " Type Picto Non trouve en memoire mes=%s", vl_Msg);
				return (XDC_NOK);
			}

			/*A Envoi d'une demande ST a l'init */
			if (vl_nocmd == XZEXC_CMD_INIT)
			{
				if (!strcmp(pl_ListePicto->Config.AdresseIP, XZECC_IP_VIDE))
					EnvoyerTrame("ST\0",
								 XZEXC_TRAME_STAT_COMP,
								 vl_adrrgs,
								 XZEXC_NOCMD,
								 XZEXC_PRIORITEFAIBLE,
								 va_socket);
				else
					EnvoyerTrameIP("ST\0",
								   XZEXC_TRAME_STAT_COMP,
								   pl_ListePicto->Config.AdresseIP,
								   pl_ListePicto->Config.Port,
								   vl_adrrgs,
								   XZEXC_NOCMD,
								   XZEXC_PRIORITEFAIBLE,
								   va_socket);
				pl_ListePicto->EtatTrans = EPMVC_EQT_OCCUP;
			}

			if ((vl_nocmd != XZEXC_NOCMD) && (vl_nocmd != XZEXC_CMD_INIT))
				ex_ecriture_trace_cmd(XDC_EQT_PCT,
									  pl_ListePicto->Config.Numero.NoEqt,
									  NULL,
									  pl_trame,
									  vl_nocmd);

			/*B on initialise les variables avec l'etat commande et l'etat courant */
			/*B ceci va permetre la comparaison */
			memcpy(&vl_etatPicto, &pl_ListePicto->Etat, sizeof(XDY_Etat_Picto));
			memcpy(&vl_etat2Picto, &pl_ListePicto->Etat_Commande, sizeof(XDY_Etat_Picto));

			/*B On recupere l'etat affiche sur le picto */

			vl_retour = affecte_etat_picto(pl_trame, &vl_etatPicto, &vl_contraste, pl_ListePicto->Config.TypePicto);
			if (vl_retour == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "affecte_etat_picto 1 KO = %d", vl_retour);
				return (XDC_NOK);
			}
			vl_retour = affecte_etat_picto(pl_trame, &vl_etat2Picto, &vl_contraste, pl_ListePicto->Config.TypePicto);
			if (vl_retour == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "affecte_etat_picto 2 KO = %d", vl_retour);
				return (XDC_NOK);
			}

			/*B AJUSTEMENT LONGUEUR PANONCEAU DANS ETAT PANNEAU A PARTIR DE LA LONGUEUR DANS LA COMMANDE */
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.Texte=[%s]", vl_etat2Picto.Texte);
			vl_etat2Picto.Texte[strlen(&pl_ListePicto->Etat_Commande.Texte)]='\0';
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.Texte=[%s]", vl_etat2Picto.Texte);

			/*B COMPARAISON ENTRE ETAT COURANT ET ETAT PRECEDENT */
			if (memcmp(&vl_etatPicto, &pl_ListePicto->Etat, sizeof(XDY_Etat_Picto)))
			{
				/*B Copie du nouvel etat d'affichage picto ds l'etat courant */
				memcpy(&pl_ListePicto->Etat, &vl_etatPicto, sizeof(XDY_Etat_Picto));


				/*A Envoi du message d'etat d'affichage du Picto */
				/*	       if ( !(pl_ListePicto->Etat_Service & XDC_EQT_HORS_SRV) )*/
				ep_envoi_affichage_Picto(vl_etatPicto, pl_ListePicto->Config.Numero.NoEqt,
										 pl_ListePicto->Config.SiteGestion);
			}

			/* Lecture vl_horodate */
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);

			/* COMPARAISON ENTRE ETAT COURANT ET ETAT COMMANDE */
			XZST_03EcritureTrace(XZSTC_WARNING, "vl_etat2Picto.Clign=[%d]", vl_etat2Picto.Clign);
			XZST_03EcritureTrace(XZSTC_WARNING, "vl_etat2Picto.NoCMD=[%d]", vl_etat2Picto.NoCMD);
			XZST_03EcritureTrace(XZSTC_WARNING, "vl_etat2Picto.Numero=[%d]", vl_etat2Picto.Numero);
			XZST_03EcritureTrace(XZSTC_WARNING, "vl_etat2Picto.Picto=[%s]", vl_etat2Picto.Picto);
			XZST_03EcritureTrace(XZSTC_WARNING, "vl_etat2Picto.Texte=[%s]", vl_etat2Picto.Texte);
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.Clign=[%d]", pl_ListePicto->Etat_Commande.Clign);
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.NoCMD=[%d]", pl_ListePicto->Etat_Commande.NoCMD);
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.Numero=[%d]", pl_ListePicto->Etat_Commande.Numero);
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.Picto=[%s]", &pl_ListePicto->Etat_Commande.Picto);
			XZST_03EcritureTrace(XZSTC_WARNING, "Etat_Commande.Texte=[%s]", &pl_ListePicto->Etat_Commande.Texte);
			if (memcmp(&vl_etat2Picto, &pl_ListePicto->Etat_Commande, sizeof(XDY_Etat_Picto)) &&
				(pl_ListePicto->NumDerCmd == vl_nocmd))
			{
				/* c'est different */
				/* filtrage de l'alerte si eqt hs ou inactif */
				if (!(pl_ListePicto->Etat_Service) &&
					(vl_nocmd != XZEXC_NOCMD) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					(vl_nocmd != XZEXC_CMD_HORS_BASE))
				{
					/* envoi de CR NOK */
					xzat25_CompteRendu_PMV(vl_nocmd, vl_horodate, XDC_OK, pl_ListePicto->Config.SiteGestion);
					XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF picto <> etat comm MAIS CR OK pour cmd:%d vl_nocmd=%d", pl_ListePicto->NumCmd, vl_nocmd);
				//	xzat25_CompteRendu_PMV(vl_nocmd, vl_horodate, XDC_NOK, pl_ListePicto->Config.SiteGestion);
				//	XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF picto <> etat comm -> CR NOK pour cmd:%d", pl_ListePicto->NumCmd);
				}
				if (!(pl_ListePicto->Etat_Service) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					(pl_ListePicto->NumCmd <= XZEXC_NOCMD))
				{
					ex_env_alerte(vl_horodate,
								  XDC_EQT_PCT,
								  pl_ListePicto->Config.Numero.NoEqt,
								  XZAAC_AFF_DIF,
								  vg_Mode_Fonct,
								  pl_ListePicto->Config.SiteGestion,
								  "affichage PICTO",
								  NULL);
					memcpy(&pl_ListePicto->Etat_Commande, &vl_etat2Picto, sizeof(XDY_Etat_Picto));
				}
			}
			else
			{
				/* Si c'est une commande */
				if (!(pl_ListePicto->Etat_Service) &&
					(vl_nocmd != XZEXC_NOCMD) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					(vl_nocmd != XZEXC_CMD_HORS_BASE))
				{
					/* c'est pareil */
					/* envoi de CR OK */
					xzat25_CompteRendu_PMV(vl_nocmd, vl_horodate, XDC_OK, pl_ListePicto->Config.SiteGestion);
					XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF picto = etat comm -> CR OK pour cmd:%d", vl_nocmd);
					memcpy(&pl_ListePicto->Etat_Commande, &vl_etatPicto, sizeof(XDY_Etat_Picto));
					if (vl_nocmd < XZEXC_NOCMD)
						pl_ListePicto->NumCmd = XZEXC_NOCMD;
					else
						pl_ListePicto->NumCmd = vl_nocmd;
				}
			}

			/* Si le contraste a change -> envoi du message contraste */
			if (((vl_contraste == EPMVC_CONTRASTE) && (pl_ListePicto->Contraste == XDC_CONTRASTENORMAL)) ||
				((vl_contraste != EPMVC_CONTRASTE) && (pl_ListePicto->Contraste == XDC_CONTRASTEBRILLANT)))
			{
				pl_ListePicto->Contraste = (vl_contraste == EPMVC_CONTRASTE) ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL;
				/* Envoi XDM_Etat_Contraste  */
				/* Construction du datagroup */
				if (!(pl_ListePicto->Etat_Service & XDC_EQT_HORS_SRV))
				{
					XDG_EncodeDG2(pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite);

					if (!TipcSrvMsgWrite(pl_DG, XDM_IdentMsg(XDM_ETAT_Contraste), XDM_FLG_SRVMSGWRITE, T_IPC_FT_INT2, XDC_EQT_PCT, T_IPC_FT_INT2, pl_ListePicto->Config.Numero.NoEqt, T_IPC_FT_INT2, pl_ListePicto->Contraste, NULL))
					{
						/* Trace envoi message XDM_ETAT_Contraste non effectue */
						XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste non effectue \n");
					}
					else
					{
						/* Trace envoi message XDM_ETAT_Contraste effectue */
						XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste effectue \n");
					}
				}
			}

			if ((vl_nocmd == XZEXC_CMD_HORS_BASE) &&
				!(pl_ListePicto->Etat_Service) &&
				(pl_ListePicto->ContrasteCmd = !pl_ListePicto->Contraste))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Appel ex_env_alerte sur commande contraste");
				ex_env_alerte(vl_horodate,
							  XDC_EQT_PCT,
							  pl_ListePicto->Config.Numero.NoEqt,
							  XZAAC_AFF_DIF,
							  vg_Mode_Fonct,
							  pl_ListePicto->Config.SiteGestion,
							  "Contraste PICTO",
							  NULL);
			}

			if (vl_nocmd == XZEXC_CMD_INIT)
			{
				memcpy(&pl_ListePicto->Etat, &vl_etatPicto, sizeof(XDY_Etat_Picto));
				memcpy(&pl_ListePicto->Etat_Commande, &vl_etatPicto, sizeof(XDY_Etat_Picto));
			}
			break;
		}

		/* c'est une trame de status complet */
		case XZEXC_TRAME_TEST_COM:
		case XZEXC_TRAME_STAT_COMP:
		{
			if (vl_typeeqt == XZECC_TYPE_EQT_PICTO)
			{
				/*A Recherche du picto concerne */
				for (pl_ListePicto = pg_debutPicto; pl_ListePicto != NULL; pl_ListePicto = pl_ListePicto->Suivant)
				{
					if (!strcmp(pl_ListePicto->Config.AdresseRGS, vl_adrrgs))
					{
						if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM)
						{
							/*A Si perte de communication avec l'equipement alors envoi d'un message "blanc" */
							vl_EtatDefautPicto.Numero = pl_ListePicto->Etat.Numero;

							if ((vl_typetrame == XZEXC_TRAME_TEST_COM) &&
								(memcmp(&vl_EtatDefautPicto, &pl_ListePicto->Etat, sizeof(XDY_Etat_Picto))))
							{
								memcpy(&pl_ListePicto->Etat, &vl_EtatDefautPicto, sizeof(XDY_Etat_Picto));
								ep_envoi_affichage_Picto(pl_ListePicto->Etat, pl_ListePicto->Config.Numero.NoEqt,
														 pl_ListePicto->Config.SiteGestion);
							}

							return (XDC_OK);
						}
						else
						{
							if (vl_typetrame == XZEXC_TRAME_TEST_COM)
								memcpy(&pl_ListePicto->Etat, &vl_EtatDefautPicto, sizeof(XDY_Etat_Picto));
						}
						break;
					}
				}
			}
			else
			{
				/*A Recherche du pmv concerne */
				for (pl_ListePMV = pg_debutPMV; pl_ListePMV != NULL;
					 pl_ListePMV = pl_ListePMV->Suivant)
				{
					if (!strcmp(pl_ListePMV->Config.AdresseRGS, vl_adrrgs))
					{
						if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM)
						{
							vl_EtatDefautPMV.Numero = pl_ListePMV->Etat.Numero;
							/*A Si perte de communication avec l'equipement alors envoi d'un message "blanc" */
							if ((vl_typetrame == XZEXC_TRAME_TEST_COM) &&
								(memcmp(&vl_EtatDefautPMV, &pl_ListePMV->Etat, sizeof(XDY_Etat_PMV))))
							{
								memcpy(&pl_ListePMV->Etat, &vl_EtatDefautPMV, sizeof(XDY_Etat_PMV));

								/*B Recherche du type de PMV traite */
								for (pl_ListeTypePMV = pg_debutTypePMV; pl_ListeTypePMV != NULL;
									 pl_ListeTypePMV = pl_ListeTypePMV->Suivant)
								{
									if (pl_ListeTypePMV->TypePMV == pl_ListePMV->Config.TypePMV)
									{
										break;
									}
								}
								if (pl_ListeTypePMV == NULL)
								{
									XZST_03EcritureTrace(XZSTC_WARNING, " Type Pmv Non trouve en memoire mes=%s", vl_Msg);
									return (XDC_NOK);
								}
								ep_envoi_affichage_PMV(pl_ListePMV->Etat, pl_ListePMV->Config.Numero.NoEqt, pl_ListeTypePMV->NbLigne, pl_ListePMV->Config.SiteGestion);
							}
							return (XDC_OK);
						}
						else
						{
							if (vl_typetrame == XZEXC_TRAME_TEST_COM)
								memcpy(&pl_ListePMV->Etat, &vl_EtatDefautPMV, sizeof(XDY_Etat_PMV));
						}
						break;
					}
				}
			}

			/* Decodage de la trame de status */
			if (vl_typeeqt == XZECC_TYPE_EQT_PICTO)
			{
				/* Decodage de la trame de status */
				ex_decode_status(pl_trame,
								 vl_tabstatus,
								 13,
								 vl_adrrgs,
								 vl_horodate,
								 vl_typeeqt,
								 pl_ListePicto->Config.Numero.NoEqt,
								 vg_NomMachine,
								 pl_ListePicto->Config.SiteGestion);
				/*!! On sait jamais si un jour , on traite de nouveau le ERI
							   ex_traitERI_PMV(vl_tabstatus[7],vl_horodate,vl_typeeqt,pl_ListePicto->Config.Numero.NoEqt,vg_NomMachine, pl_ListePicto->Config.SiteGestion);
				*/

				/*A Traitement des alerte connexion locale */
				if ((vl_tabstatus[3][0] != '0') && (vl_tabstatus[3][0] != ' '))
					vl_Alerte = XZAAC_CLE;
				if ((vl_Alerte != 0) && !(pl_ListePicto->Etat_Service) && (pl_ListePicto->Mode == XZEXC_MODE_NORMAL))
					ex_env_alerte(vl_horodate,
								  vl_typeeqt,
								  pl_ListePicto->Config.Numero.NoEqt,
								  vl_Alerte,
								  vg_Mode_Fonct,
								  pl_ListePicto->Config.SiteGestion,
								  vl_tabstatus[3],
								  NULL);
				if (vl_Alerte == 0)
					pl_ListePicto->Mode = XZEXC_MODE_NORMAL;
				else
					pl_ListePicto->Mode = XZEXC_MODE_CONNECTION_LOCALE;

				XZST_03EcritureTrace(XZSTC_DEBUG1, " Decodage status PICTO");
			}
			else
			{
				/* Decodage de la trame de status */
				ex_decode_status(pl_trame,
								 vl_tabstatus,
								 13,
								 vl_adrrgs,
								 vl_horodate,
								 vl_typeeqt,
								 pl_ListePMV->Config.Numero.NoEqt,
								 vg_NomMachine,
								 pl_ListePMV->Config.SiteGestion);
				/*!! On sait jamais si un jour , on traite de nouveau le ERI
							   ex_traitERI_PMV(vl_tabstatus[7],vl_horodate,vl_typeeqt,pl_ListePMV->Config.Numero.NoEqt,vg_NomMachine, pl_ListePMV->Config.SiteGestion);
				*/

				/*A Traitement des alerte connexion locale */
				if ((vl_tabstatus[3][0] != '0') && (vl_tabstatus[3][0] != ' ') && (vl_tabstatus[3][0] != '\0'))
					vl_Alerte = XZAAC_CLE;
				if ((vl_tabstatus[11][0] != '0') && (vl_tabstatus[11][0] != ' ') && (vl_tabstatus[11][0] != '\0'))
					vl_Alerte = XZAAC_CLL;
				if ((vl_tabstatus[12][0] != '0') && (vl_tabstatus[12][0] != ' ') && (vl_tabstatus[12][0] != '\0'))
					vl_Alerte = XZAAC_CLE;
				if ((vl_Alerte != 0) && !(pl_ListePMV->Etat_Service) && (pl_ListePMV->Mode == XZEXC_MODE_NORMAL))
					ex_env_alerte(vl_horodate,
								  vl_typeeqt,
								  pl_ListePMV->Config.Numero.NoEqt,
								  vl_Alerte,
								  vg_Mode_Fonct,
								  pl_ListePMV->Config.SiteGestion,
								  (vl_Alerte == XZAAC_CLL) ? vl_tabstatus[11] : (vl_tabstatus[12][0] != ' ') ? vl_tabstatus[12]
																											 : vl_tabstatus[3],
								  NULL);
				if (vl_Alerte == 0)
					pl_ListePMV->Mode = XZEXC_MODE_NORMAL;
				else
					pl_ListePMV->Mode = XZEXC_MODE_CONNECTION_LOCALE;

				XZST_03EcritureTrace(XZSTC_DEBUG1, " Decodage status PMV");
			}
		}
		}
	}

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  met a jour la variable pa_etatPicto
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int affecte_etat_picto(char *pa_trame,
					   XDY_Etat_Picto *pa_etatPicto,
					   char *pa_contraste,
					   XDY_TypePicto pa_typePicto)
/*
* ARGUMENTS EN ENTREE :
*  *pa_trame : trame en entree
*  *pa_etatPicto : etat de l'affichage du picto
*  *pa_contraste : extraction de contraste
*  XDY_TypePicto : type de l'equipement picto
*
* ARGUMENTS EN SORTIE :
*  *pa_etatPicto : etat de l'affichage du picto
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*  extrait les infos et les affecte dans pa_etatPicto
*
------------------------------------------------------*/
{
//	EPMV_DONNEES_TYPE_Picto *pl_ListeTypePicto;
	
	char vl_cligno[32];
	char vl_cligno2[32];
	int vl_param;
	char vl_TrameTrav[1024] = "";
	char *pl_Trame[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	char *vl_Mot;
	char vl_Texte[32];
	char vl_Contraste;
	int vl_Index = 1,
		vl_Clig = 0,
		vl_Ligne = 0;
	int vl_nb_car_picto = XDC_Lon_TextePicto;

	static char *version = "$Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $      :affecte_etat_picto ";

	/* Decoupage de la trame en ligne */
	XZST_03EcritureTrace(XZSTC_WARNING, "TRAME %s", pa_trame);
	strcpy(vl_TrameTrav, pa_trame);
	pl_Trame[vl_Index++] = strtok(vl_TrameTrav, CM_SEPARATEUR_LIGNE);
	while ((pl_Trame[vl_Index++] = strtok(NULL, CM_SEPARATEUR_LIGNE)) != NULL)
		;

	for (vl_Index = 1; vl_Index < 10; vl_Index++)
	{
		/*LINUX*/
		if ((pl_Trame[vl_Index] == NULL))
			break;

		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_LIGNE)) == NULL)
			break;

		/* Extraction du numero de ligne  */
		if (sscanf(vl_Mot, CM_LEC_NUM_LIGNE, &vl_Ligne) != 1)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Syntaxe etat affichage picto incorrecte %s", vl_Mot);
			return (XDC_NOK);
		}

		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_CLIGNO)) != NULL)
		{
			if (sscanf(vl_Mot, CM_LEC_CLIGNO, vl_cligno) != 1)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "clignotement 1er ligne incorrecte");
				return (XDC_NOK);
			}
			XZST_03EcritureTrace(XZSTC_WARNING, "clignotement picto lu : %s", vl_cligno);
			sprintf(vl_cligno2, "CL=%s", vl_cligno);
			strcpy(vl_cligno, vl_cligno2);
			/*      	         XZST_03EcritureTrace( XZSTC_WARNING, "clignotement picto relu : %s clignolent=%s ==>%d",vl_cligno,vg_clignolent,strncmp(vl_cligno,vg_clignolent,8));*/
			if (!strncmp(vl_cligno, vg_clignolent, 8))
			{
				vl_Clig = XDC_PICTO_CLIGNO;
			}
			else
			{
				vl_Clig = XDC_PICTO_PASCLIGNO;
			}
		}
		else
		{
			vl_Clig = XDC_PICTO_PASCLIGNO;
		}

		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_CONTRASTE)) != NULL)
		{
			sscanf(vl_Mot, CM_LEC_CONTRASTE, &vl_Contraste);
		}

		strcpy(vl_Texte, "");

		XZST_03EcritureTrace(XZSTC_FONCTION, "vl_Texte : %s", vl_Texte);
		XZST_03EcritureTrace(XZSTC_FONCTION, "Numero de ligne %d texte:<%s><%s>", vl_Ligne, vl_Texte, pa_trame);

		switch (vl_Ligne)
		{
		case CM_LIGNE_SIX:
			if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_AFF)) != NULL)
				sscanf(vl_Mot, CM_LEC_AFF, vl_Texte);
			if (*vl_Texte == '0')
				strcpy(pa_etatPicto->Picto, XDC_PICTO_PASPICTO);
			else
				strcpy(pa_etatPicto->Picto, vl_Texte);
			break;
		case CM_LIGNE_SEPT:
			if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_AFF)) != NULL)
			{
				if ((strncmp(vl_Mot + 3, "\"\"", 2)) && ((vl_Mot = strtok(vl_Mot, "\"")) != NULL))
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1, "Mot:%s ", vl_Mot);
					strcpy(vl_Texte, ((vl_Mot = strtok(NULL, "\"")) == NULL) ? "" : vl_Mot);
				}
//				vl_Texte[vl_nb_car_picto-1]='\0';
				strcat (vl_Texte, "            ");
				vl_Texte[XDC_Lon_TextePicto-1]='\0';
			}
			strcpy(pa_etatPicto->Texte, vl_Texte);
			pa_etatPicto->Clign = vl_Clig;
			*pa_contraste = vl_Contraste;
			break;
		case CM_LIGNE_UNE:
		case CM_LIGNE_DEUX:
		case CM_LIGNE_TROIS:
		case CM_LIGNE_QUATRE:
			break;
		default:
			XZST_03EcritureTrace(XZSTC_WARNING, "Numero de ligne affichage picto incorrecte %d", vl_Ligne);
			return (XDC_NOK);
			break;
		}
	}
	/*	strcpy(pa_etatPicto->Texte,trim(pa_etatPicto->Texte));*/
	if (strlen(pa_etatPicto->Texte) == 0)
		strcpy(pa_etatPicto->Texte, "      ");
	XZST_03EcritureTrace(XZSTC_WARNING, "picto:%s  texte:%s clign:%d lu:%c", pa_etatPicto->Picto, pa_etatPicto->Texte, pa_etatPicto->Clign, *pa_contraste);

	return (XDC_OK);
}

int detect_heure(char *va_texte)
{
	char *vl_position;

	vl_position = strstr(va_texte, ":");

	if (vl_position == NULL)
		return (XDC_FAUX);

	if ((isdigit(*(vl_position - 1))) &&
		(isdigit(*(vl_position + 1))) &&
		(isdigit(*(vl_position + 2))))
	{
		return (XDC_VRAI);
	}
	else
	{
		return (XDC_FAUX);
	}
}

int detect_neutre_3points(char *va_texte)
{
	char *vl_position;

	vl_position = strstr(va_texte, "          ...");

	if (vl_position == NULL)
		return (XDC_FAUX);

	return (XDC_VRAI);
}

int detect_temp(char *va_texte)
{
	char *vl_position;

	vl_position = strstr(va_texte, "�C");

	if (vl_position == NULL)
		return (XDC_FAUX);

	if (isdigit(*(vl_position - 1)))
	{
		return (XDC_VRAI);
	}
	else
	{
		return (XDC_FAUX);
	}
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  met a jour la variable pa_etatPicto
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int affecte_etat_pmv(char *pa_trame,
					 XDY_Etat_PMV *pa_etatPMV,
					 EPMV_DONNEES_TYPE_PMV *pa_ListeTypePMV,
					 char *va_contraste,
					 int va_NbLigne,
					 XDY_Octet va_TypeLCR)
/*
* ARGUMENTS EN ENTREE :

*  *pa_trame : trame en entree
*  *pa_etatPicto : etat de l'affichage du PMV
*  *pa_ListeTypePMV : donnees type PMV
*  *pa_contraste : extraction de contraste
*  va_NbLigne    : nombre de ligne du PMV
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*  *pa_etatPMV : etat de l'affichage du picto
*
* CONDITION D'UTILISATION
*
* FONCTION
*  extrait les infos et les affecte dans pa_etatPMV
*
------------------------------------------------------*/
{
	int vl_param;
	XZEXT_TRAME_PS_NAV vl_TrameTrav;
	char *pl_Trame[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	char vl_cligno[32];
	char vl_cligno2[32];
	char vl_aff[60];
	int vl_flash;
	char *vl_Mot;
	char vl_Texte[32], vl_Alter[32];
	char vl_buf[32];
	int vl_cr;
	char vl_Contraste;
	int vl_start;
	int vl_Index = 1,
		vl_Aff = 0,
		vl_Clig = 0,
		vl_Ligne = 0;

	static char *version = "$Id: epmv_tra.c,v 1.52 2019/05/21 11:04:28 pc2dpdy Exp $      :affecte_etat_pmv ";

	/* Decoupage de la trame en ligne */
	strcpy(vl_TrameTrav, pa_trame);
	XZST_03EcritureTrace(XZSTC_FONCTION, "trame : %s", vl_TrameTrav);

	pl_Trame[vl_Index++] = strtok(vl_TrameTrav, CM_SEPARATEUR_LIGNE);
	while ((pl_Trame[vl_Index++] = strtok(NULL, CM_SEPARATEUR_LIGNE)) != NULL)
		;

	vl_start = 1;
	if (pa_ListeTypePMV->TypePMV == 7)
		vl_start = 2;
	for (vl_Index = vl_start; vl_Index < 10; vl_Index++)
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "ligne %d = '%s'", vl_Index, pl_Trame[vl_Index]);

		if (pa_ListeTypePMV->TypePMV == 7 || va_TypeLCR == 6)
		{
			if ((pl_Trame[vl_Index] == NULL) || ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_LIGNE_7)) == NULL))
				break;
		}
		else
		{
			if ((pl_Trame[vl_Index] == NULL) || ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_LIGNE)) == NULL))
				break;
		}
		/* Extraction du numero de ligne  */
		if (pa_ListeTypePMV->TypePMV == 7 || va_TypeLCR == 6)
			vl_cr = sscanf(vl_Mot, CM_LEC_NUM_LIGNE_7, &vl_Ligne);
		else
			vl_cr = sscanf(vl_Mot, CM_LEC_NUM_LIGNE, &vl_Ligne);
		if (vl_cr != 1)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Syntaxe etat affichage PMV incorrecte %s", vl_Mot);
			return (XDC_NOK);
		}

		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_CLIGNO)) != NULL)
		{
			if (sscanf(vl_Mot, CM_LEC_CLIGNO, vl_cligno) != 1)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "clignotement 1er ligne incorrecte");
				return (XDC_NOK);
			}
			sprintf(vl_cligno2, "CL=%s", vl_cligno);
			strcpy(vl_cligno, vl_cligno2);
			if (!strcmp(vl_cligno, vg_clignolent))
			{
				vl_Clig = XDC_PMV_CLIGNOLENT;
			}
			else
			{
				if (!strcmp(vl_cligno, vg_clignorapide))
				{
					vl_Clig = XDC_PMV_CLIGNORAPIDE;
				}
				else
				{
					vl_Clig = XDC_PMV_PASCLIGNO;
				}
			}
		}
		else
		{
			vl_Clig = XDC_PMV_PASCLIGNO;
		}

		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_CONTRASTE)) != NULL)
		{
			sscanf(vl_Mot, CM_LEC_CONTRASTE, &vl_Contraste);
		}

		/* Extraction du texte et de l'alternatif eventuel  */
		/*      	   strcpy ( vl_Texte, "" );*/
		strcpy(vl_Texte, XDC_CHAINE_VIDE);
		strcpy(vl_Alter, "");


		if  ( strstr(pl_Trame[vl_Index], "\"") )
		{
			if ((vl_Mot = strtok(pl_Trame[vl_Index], "\"")) != NULL)
			{

				/*	      strcpy ( vl_Texte, ( (vl_Mot=strtok(NULL, "\"" )) == NULL ) ? "\0" : vl_Mot );*/
				strcpy(vl_Texte, ((vl_Mot = strtok(NULL, "\"")) == NULL) ? XDC_CHAINE_VIDE : vl_Mot);
				vl_Mot = strtok(NULL, "\"");

				if ((vl_Mot != NULL) && (*vl_Mot == '/')) /*LINUX*/
					strcpy(vl_Alter, ((vl_Mot = strtok(NULL, "\"")) == NULL) ? "\0" : vl_Mot);
				else
					strcpy(vl_Alter, "");

			}
		}
		else
		{
			vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_AFF);
			vl_Mot = vl_Mot+strlen (CM_DEBUT_AFF);
			sprintf (vl_Texte, "%d", atoi(vl_Mot));
		}
		vl_Aff = XDC_PMV_TEXTE;
		/* Detection de l'heure */
		if (detect_heure(vl_Texte))
		{
			vl_Aff = XDC_PMV_HEURE;
			strcpy(vl_Texte, "");
			strcpy(vl_Alter, "");
		}

		/* Detection du neutre */
		if (detect_neutre_3points(vl_Texte))
		{
			/* Conserver le texte et son type */
			/* vl_Aff = XDC_PMV_3POINTS; */
		}

		XZST_03EcritureTrace(XZSTC_FONCTION, "TYPE PMV %d", pa_ListeTypePMV->TypePMV);
		XZST_03EcritureTrace(XZSTC_FONCTION, "vl_Texte : %s", vl_Texte);
		if ((pa_ListeTypePMV->TypePMV == 4) ||
			(pa_ListeTypePMV->TypePMV == 5))
		{
			strcpy(vl_buf, vl_Texte);
			if (strlen(trim(vl_buf)) == 0)
				strcpy(vl_Texte, " ");
		}

		XZST_03EcritureTrace(XZSTC_FONCTION, "vl_Texte : %s", vl_Texte);

		XZST_03EcritureTrace(XZSTC_FONCTION, "Numero de ligne %d texte:<%s><%s>", vl_Ligne, vl_Texte, vl_Alter);

		switch (vl_Ligne)
		{
		case CM_LIGNE_UNE:
			if (!strcmp(vl_Texte,"0"))
				strcpy(vl_Texte," ");
			if (va_NbLigne != EPMVC_PMV_4LIGNES)
			{
				strcpy(pa_etatPMV->Texte1, vl_Texte);
				strcpy(pa_etatPMV->Alternat1, vl_Alter);
				pa_etatPMV->Affichage1 = vl_Aff;
				pa_etatPMV->Clign1 = vl_Clig;
			}
			else
			{
				strcpy(pa_etatPMV->Texte4, vl_Texte);
				strcpy(pa_etatPMV->Alternat4, vl_Alter);
				pa_etatPMV->Affichage4 = vl_Aff;
				pa_etatPMV->Clign4 = vl_Clig;
			}
			break;
		case CM_LIGNE_DEUX:
			if (!strcmp(vl_Texte,"0"))
				strcpy(vl_Texte," ");
			if (va_NbLigne != EPMVC_PMV_4LIGNES)
			{
				if (va_NbLigne != EPMVC_PMV_1LIGNES)
				{
					strcpy(pa_etatPMV->Texte2, vl_Texte);
					strcpy(pa_etatPMV->Alternat2, vl_Alter);
					pa_etatPMV->Affichage2 = vl_Aff;
					pa_etatPMV->Clign2 = vl_Clig;
				}
				else if (pa_ListeTypePMV->Flash == XDC_PMV_FLASHPRESENT)
				{
					if (atoi(vl_Texte) == 1)
						pa_etatPMV->Flash = XDC_PMV_FLASH;
					else
						pa_etatPMV->Flash = XDC_PMV_PASFLASH;
				}
			}
			else
			{
				strcpy(pa_etatPMV->Texte1, vl_Texte);
				strcpy(pa_etatPMV->Alternat1, vl_Alter);
				pa_etatPMV->Affichage1 = vl_Aff;
				pa_etatPMV->Clign1 = vl_Clig;
			}
			break;
		case CM_LIGNE_TROIS:
			if (!strcmp(vl_Texte,"0"))
				strcpy(vl_Texte," ");
			if (va_NbLigne != EPMVC_PMV_4LIGNES)
			{
				if (va_NbLigne != EPMVC_PMV_2LIGNES)
				{
					strcpy(pa_etatPMV->Texte3, vl_Texte);
					strcpy(pa_etatPMV->Alternat3, vl_Alter);
					pa_etatPMV->Affichage3 = vl_Aff;
					pa_etatPMV->Clign3 = vl_Clig;
					*va_contraste = vl_Contraste;
				}
				else if (pa_ListeTypePMV->Flash == XDC_PMV_FLASHPRESENT)
				{
					if (atoi(vl_Texte) == 1)
						pa_etatPMV->Flash = XDC_PMV_FLASH;
					else
						pa_etatPMV->Flash = XDC_PMV_PASFLASH;
				}
			}
			else
			{
				strcpy(pa_etatPMV->Texte2, vl_Texte);
				strcpy(pa_etatPMV->Alternat2, vl_Alter);
				pa_etatPMV->Affichage2 = vl_Aff;
				pa_etatPMV->Clign2 = vl_Clig;
			}
			break;
		case CM_LIGNE_QUATRE:
			if (va_NbLigne != EPMVC_PMV_4LIGNES)
			{
				if (pa_ListeTypePMV->Flash == XDC_PMV_FLASHPRESENT)
				{
					XZST_03EcritureTrace(XZSTC_FONCTION, "Flash : %s. atoi(Flash)=%d", vl_Texte,atoi(vl_Texte));

					if (atoi(vl_Texte) == 1)
						pa_etatPMV->Flash = XDC_PMV_FLASH; // XDC_PMV_FLASH
					else
						pa_etatPMV->Flash = XDC_PMV_PASFLASH; // XDC_PMV_PASFLASH
				}
				else
					pa_etatPMV->Flash = XDC_PMV_FLASH; // XDC_PMV_PASFLASH
			}
			else
			{
				if (!strcmp(vl_Texte,"0"))
					strcpy(vl_Texte," ");
				strcpy(pa_etatPMV->Texte3, vl_Texte);
				strcpy(pa_etatPMV->Alternat3, vl_Alter);
				pa_etatPMV->Affichage3 = vl_Aff;
				pa_etatPMV->Clign3 = vl_Clig;
				*va_contraste = vl_Contraste;
			}
			break;
		case CM_LIGNE_CINQ:
			/*A Extration eventuelle du flash */
			if ((va_NbLigne == EPMVC_PMV_4LIGNES) && (pa_ListeTypePMV->Flash == XDC_PMV_FLASHPRESENT))
			{
				if (atoi(vl_Texte) == 1)
					pa_etatPMV->Flash = XDC_PMV_FLASH; // XDC_PMV_FLASH;
				else
					pa_etatPMV->Flash = XDC_PMV_PASFLASH; // XDC_PMV_PASFLASH;
			}
			else
				pa_etatPMV->Flash = XDC_PMV_FLASH; // XDC_PMV_PASFLASH
			break;
		case CM_LIGNE_SIX:
			/*A Extration eventuelle du flash */
			if ((va_NbLigne == EPMVC_PMV_4LIGNES) && (pa_ListeTypePMV->Flash == XDC_PMV_FLASHPRESENT))
			{
				XZST_03EcritureTrace(XZSTC_FONCTION, "Flash : %s. atoi(Flash)=%d", vl_Texte,atoi(vl_Texte));

				if (atoi(vl_Texte) == 1)
					pa_etatPMV->Flash = XDC_PMV_FLASH; // XDC_PMV_FLASH;
				else
					pa_etatPMV->Flash = XDC_PMV_PASFLASH; // XDC_PMV_PASFLASH;
			}
			else
				pa_etatPMV->Flash = XDC_PMV_FLASH; // XDC_PMV_PASFLASH
			break;
		case CM_LIGNE_SEPT:
			break;
		default:
			XZST_03EcritureTrace(XZSTC_WARNING, "Numero de ligne affichage PMV incorrecte %d", vl_Ligne);
			return (XDC_NOK);
			break;
		}
	}

	if (va_NbLigne != EPMVC_PMV_4LIGNES)
	{
		strcpy(pa_etatPMV->Texte4, "");
		strcpy(pa_etatPMV->Alternat4, "");
		pa_etatPMV->Affichage4 = pa_etatPMV->Affichage1;
		pa_etatPMV->Clign4 = XDC_PMV_PASCLIGNO;
	}

	if (va_NbLigne < 3)
	{
		strcpy(pa_etatPMV->Texte3, "");
		strcpy(pa_etatPMV->Alternat3, "");
		pa_etatPMV->Affichage3 = pa_etatPMV->Affichage1;
		pa_etatPMV->Clign3 = XDC_PMV_PASCLIGNO;
	}
	if (va_NbLigne < 2)
	{
		strcpy(pa_etatPMV->Texte2, "");
		strcpy(pa_etatPMV->Alternat2, "");
		pa_etatPMV->Affichage2 = pa_etatPMV->Affichage1;
		pa_etatPMV->Clign2 = XDC_PMV_PASCLIGNO;
	}
	if ((pa_etatPMV->Affichage1 == XDC_PMV_3POINTS) ||
		(pa_etatPMV->Affichage2 == XDC_PMV_3POINTS) ||
		(pa_etatPMV->Affichage3 == XDC_PMV_3POINTS) ||
		(pa_etatPMV->Affichage4 == XDC_PMV_3POINTS))
	{
		pa_etatPMV->Affichage1 = XDC_PMV_3POINTS;
		pa_etatPMV->Affichage2 = XDC_PMV_3POINTS;
		pa_etatPMV->Affichage3 = XDC_PMV_3POINTS;
		pa_etatPMV->Affichage4 = XDC_PMV_3POINTS;
	}

	if ((pa_etatPMV->Affichage1 == XDC_PMV_HEURE) ||
		(pa_etatPMV->Affichage2 == XDC_PMV_HEURE) ||
		(pa_etatPMV->Affichage3 == XDC_PMV_HEURE) ||
		(pa_etatPMV->Affichage4 == XDC_PMV_HEURE))
	{
		pa_etatPMV->Affichage1 = XDC_PMV_HEURE;
		pa_etatPMV->Affichage2 = XDC_PMV_HEURE;
		pa_etatPMV->Affichage3 = XDC_PMV_HEURE;
		pa_etatPMV->Affichage4 = XDC_PMV_HEURE;
		strcpy(pa_etatPMV->Texte1, "");
		strcpy(pa_etatPMV->Alternat1, "");
		strcpy(pa_etatPMV->Texte2, "");
		strcpy(pa_etatPMV->Alternat2, "");
		strcpy(pa_etatPMV->Texte3, "");
		strcpy(pa_etatPMV->Alternat3, "");
		strcpy(pa_etatPMV->Texte4, "");
		strcpy(pa_etatPMV->Alternat4, "");
	}
	if ((pa_etatPMV->Affichage1 == XDC_PMV_TEMPERATURE) ||
		(pa_etatPMV->Affichage2 == XDC_PMV_TEMPERATURE) ||
		(pa_etatPMV->Affichage3 == XDC_PMV_TEMPERATURE) ||
		(pa_etatPMV->Affichage4 == XDC_PMV_TEMPERATURE))
	{
		pa_etatPMV->Affichage1 = XDC_PMV_HEURE;
		pa_etatPMV->Affichage2 = XDC_PMV_HEURE;
		pa_etatPMV->Affichage3 = XDC_PMV_HEURE;
		pa_etatPMV->Affichage4 = XDC_PMV_HEURE;
		strcpy(pa_etatPMV->Texte1, "");
		strcpy(pa_etatPMV->Alternat1, "");
		strcpy(pa_etatPMV->Texte2, "");
		strcpy(pa_etatPMV->Alternat2, "");
		strcpy(pa_etatPMV->Texte3, "");
		strcpy(pa_etatPMV->Alternat3, "");
		strcpy(pa_etatPMV->Texte4, "");
		strcpy(pa_etatPMV->Alternat4, "");
	}

	return (XDC_OK);
}

char *trim(char *s)
{
	int i = 0;
	int j = strlen(s) - 1;
	int k = 0;

	while (isspace(s[i]) && s[i] != '\0')
		i++;

	while (isspace(s[j]) && j >= 0)
		j--;

	while (i <= j)
		s[k++] = s[i++];

	s[k] = '\0';

	return s;
}

int trace_alerte(int no_alerte, XDY_Etat_PMV *etat_lu, XDY_Etat_PMV *etat_commande)
{
	char nomTrace[80];
	sprintf(nomTrace, "/produits/migrazur/appliSD/traces/etat_PMV_cmd%02d", no_alerte);
	int fTrace = open(nomTrace, O_CREAT | O_WRONLY);
	write(fTrace, (void *)etat_commande, sizeof(XDY_Etat_PMV));
	close(fTrace);

	sprintf(nomTrace, "/produits/migrazur/appliSD/traces/etat_PMV_lu%02d", no_alerte);
	fTrace = open(nomTrace, O_CREAT | O_WRONLY);
	write(fTrace, (void *)etat_lu, sizeof(XDY_Etat_PMV));
	close(fTrace);

	return (0);
}
