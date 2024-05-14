/*E*/
/*Fichier : $Id: eprv_tra.c,v 1.2 2013/08/28 09:14:44 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2013/08/28 09:14:44 $
------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPRV * FICHIER eprv_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de traitement des trames recues de la tache TEPRV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	20/12/11		: Creation (DEM/1016)
* JMG	26/06/13		: correction L0
* ABE	31/05/21		: PRV sur IP DEM-SAE283 1.2
* ABE	19/10/21		: Ajout trace debug prod PRV sur IP DEM-SAE283 1.3
* ABE	19/10/21		: Changement test adrRGS vers Noeqt pour type de trame XZEXC_TRAME_ETAT_AFF_PRV DEM-SAE283 1.5
* ABE	25/10/21		: Fix push adr RGS dans va param recherce eqt pour ex_ret_lcr DEM-SAE283 1.7
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "eprv_tra.h"

/* definitions de constantes */

#define CM_SEPARATEUR_LIGNE "\r\n"
#define CM_DEBUT_LIGNE "K AM=1."
#define CM_LEC_NUM_LIGNE CM_DEBUT_LIGNE "%d"
#define CM_DEBUT_CONTRASTE "EC="
#define CM_LEC_CONTRASTE CM_DEBUT_CONTRASTE "%c"
#define CM_DEBUT_CLIGNO "CL="
#define CM_LEC_CLIGNO CM_DEBUT_CLIGNO "%s"
#define CM_DEBUT_AFF "AF="
#define CM_LEC_AFF CM_DEBUT_AFF "%s"
#define CM_LIGNE_UNE 1
#define CM_LIGNE_DEUX 2
#define CM_LIGNE_TROIS 3

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

int affecte_etat_prv(char *, XDY_Etat_PRV *, char *);

char *trim(char *);
/* definition de fonctions externes */
extern void supprime_blancs_fin(char *);

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
	static char *version = "@(#)eprv_tra.c	1.38 05/31/00      :recherche_eqt ";

	EPRV_DONNEES_PRV *pl_ListePRV;
	int vl_typeeqt = 0;
	char *vl_status[32];

	/*A Recherche du prv concerne */
	for (pl_ListePRV = pg_debutPRV; pl_ListePRV != NULL; pl_ListePRV = pl_ListePRV->Suivant)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "RGS:%s IP:%s va_adr:%s",
							 pl_ListePRV->Config.AdresseRGS, pl_ListePRV->Config.AdresseIP, va_adrrgs);

		if (((!strcmp(pl_ListePRV->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePRV->Config.AdresseRGS, va_adrrgs))) ||
			((strcmp(pl_ListePRV->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePRV->Config.AdresseIP, va_adrrgs))))
		{
			vl_typeeqt = XZECC_TYPE_EQT_PRV;
			pl_ListePRV->Etat.Numero = pl_ListePRV->Config.Numero.NoEqt;
			*va_noeqt = pl_ListePRV->Config.Numero.NoEqt;
			*va_SiteGestion = pl_ListePRV->Config.SiteGestion;
			strcpy(va_adrrgs, pl_ListePRV->Config.AdresseRGS); // changement dans la var param de adr IP vers adr RGS pour ex_ret_lcr
			if ((va_TypeTrame == XZEXC_TRAME_FEN_LCR) || (va_TypeRep != XZEXC_TRAME_NON_TRANSM))
				pl_ListePRV->EtatTrans = EPRVC_EQT_LIBRE;
			else
				pl_ListePRV->EtatTrans = EPRVC_EQT_PANNE;
			break;
		}
	}

	return (vl_typeeqt);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  envoi message RTwork etat d'affichage PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ep_envoi_affichage_PRV(XDY_Etat_PRV va_EtatPRV,
							XDY_Octet va_NumEqt,
							XDY_District va_site)
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
	static char *version = "@(#)eprv_tra.c	1.38 05/31/00      : ep_envoi_affichage_PRV ";

	XZSCT_NomTache vl_NomTache = "";
	XDY_NomMachine vl_NomMachine = "";
	XDY_Datagroup pl_DG;

	/*A Construction du datagroup */

	XDG_EncodeDG2(pl_DG, XDG_ETAT_PRV, XZSC23_Nom_From_Numero(va_site));

	/*A Les affichages sont differents -> envoi du message XDM_ETAT_PRV */
	if (!TipcSrvMsgWrite(pl_DG,
						 XDM_IdentMsg(XDM_ETAT_PRV),
						 XDM_FLG_SRVMSGWRITE,
						 T_IPC_FT_INT2, va_NumEqt,
						 T_IPC_FT_STR, va_EtatPRV.Bandeau,
						 T_IPC_FT_STR, va_EtatPRV.Vitesse,
						 NULL))

	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ep_envoi_affichage_PRV: Envoi message XDM_ETAT_PRV non effectue \n");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ep_envoi_affichage_PRV: Envoi message XDM_ETAT_PRV effectue sur %s\n", pl_DG);
		XZST_03EcritureTrace(XZSTC_INFO, "ep_envoi_affichage_PRV: num : %d \n vitesse : %s \n bandeau : %s \n noCMD : %d",
							 va_NumEqt,
							 va_EtatPRV.Vitesse,
							 va_EtatPRV.Bandeau,
							 va_EtatPRV.NoCMD);
	}
	TipcSrvFlush();

	/* Mise jour compteur de marche equipement */
	XZSC_06NomTache(vl_NomTache);
	XZSC_07NomMachine(vl_NomMachine);
	if (!strcmp(va_EtatPRV.Vitesse, XDC_PICTO_PASPICTO))
	{
		XZMA01_Maj_duree_marche(va_NumEqt,
								XDC_EQT_PRV,
								XDC_FAUX,
								vl_NomTache,
								vl_NomMachine);
	}
	else
	{
		XZMA01_Maj_duree_marche(va_NumEqt,
								XDC_EQT_PRV,
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
	static char *version = "@(#)eprv_tra.c	1.38 05/31/00      :em_traitement_trame ";

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
	XDY_Etat_PRV vl_etatPRV = {0, "", XDC_PICTO_PASPICTO, 0},
				 vl_etat2PRV = {0, "", XDC_PICTO_PASPICTO, 0},
				 vl_EtatDefautPRV = {0, "", XDC_PICTO_PASPICTO, 0};
	EPRV_DONNEES_PRV *pl_ListePRV;
	char vl_cligno[15];
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
			return;

		if (vl_param != 10)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Lecture message socket impossible mes=%s", vl_Msg);
			return;
		}

		/*A Conversion horodate */
		sprintf(pl_Horodatedecompo, "%s %s", vl_horod1, vl_horod2);

		XZSM_13ConversionHorodateSec(pl_Horodatedecompo, &vl_horodate);

		XZST_03EcritureTrace(XZSTC_WARNING, "%s %s %s %s %d %d %d %s %d %d", vl_entete1,
							 vl_entete2,
							 vl_horod1,
							 vl_horod2,
							 vl_nbnonrep,
							 vl_nberrcrc,
							 vl_nocmd,
							 vl_adrrgs,
							 vl_typetrame,
							 vl_typ_rep);

		/*A Recherche l'equipement qui emet une reponse */
		if ((vl_typeeqt = recherche_eqt(vl_adrrgs, vl_typ_rep, vl_typetrame, &vl_noeqt, &vl_sitegestion)) == 0)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Prv ou Picto Non trouve en memoire mes=%s", vl_Msg);
			return;
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
		pl_trame_ptr = strstr(vl_Msg, XZEXC_SEP_TRAME) + 4;
		strcpy(pl_trame, strtok(pl_trame_ptr, XZEXC_FIN_MSG_SOCK));
		pl_trame[strlen(pl_trame) - 1] = '\0';

		if (pl_trame == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Lecture message socket impossible mes=%s", vl_Msg);
			return;
		}

		/*A Recherche du type de trame */
		switch (vl_typetrame)
		{
		/*A C'est une trame de reponse pour la fenetre LCR */
		case XZEXC_TRAME_FEN_LCR:
		{

			if (vl_typeeqt == XZECC_TYPE_EQT_PRV)
			{
				ex_ret_LCR(pl_trame, vl_adrrgs, &pg_DebutListePRVFLcr, vl_typ_rep);
			}

			break;
		}
		/*A C'est une trame de reponse status temps reel */
		case XZEXC_TRAME_STAT_TR:
		{
			/*A Recherche du pmv concerne */
			for (pl_ListePRV = pg_debutPRV; pl_ListePRV != NULL;
				 pl_ListePRV = pl_ListePRV->Suivant)
			{
				if (!strcmp(pl_ListePRV->Config.AdresseRGS, vl_adrrgs))
				{
					/*A Si le status TR est ko -> demande du status complet */
					if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM)
						return;

					if ((((*pl_trame != '@') && (*(pl_trame + 1) != '@')) && (pl_ListePRV->StatusTR == XDC_OK)) ||
						(((*pl_trame == '@') || (*(pl_trame + 1) == '@')) && (pl_ListePRV->StatusTR != XDC_OK)))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "Chgt StusTR %c;%c.EnvoyerTrame ST", (*pl_trame), *(pl_trame + 1));

						if (!strcmp(pl_ListePRV->Config.AdresseIP, XZECC_IP_VIDE))
						{
							if (EnvoyerTrame("ST\0",
											 XZEXC_TRAME_STAT_COMP,
											 vl_adrrgs,
											 XZEXC_NOCMD,
											 XZEXC_PRIORITEFAIBLE,
											 va_socket) == XDC_NOK)
							{
								XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV");
								return;
							}
						}
						else
						{
							if (EnvoyerTrameIP("ST\0",
											   XZEXC_TRAME_STAT_COMP,
											   pl_ListePRV->Config.AdresseIP,
											   pl_ListePRV->Config.Port,
											   vl_adrrgs,
											   XZEXC_NOCMD,
											   XZEXC_PRIORITEFAIBLE,
											   va_socket) == XDC_NOK)
							{
								XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV");
								return;
							}
						}
						pl_ListePRV->EtatTrans = EPRVC_EQT_OCCUP;
					}

					pl_ListePRV->StatusTR = ((*pl_trame == '@') || (*(pl_trame + 1)) == '@') ? XDC_OK : XDC_NOK;
					if (pl_ListePRV->StatusTR == XDC_NOK)
						XZST_03EcritureTrace(XZSTC_WARNING, "Le statut du PRV passe a XDC_NOK:%c;%c.", (*pl_trame), *(pl_trame + 1));
					break;
				}
			}
			break;
		}
		/*A C'est une trame de reponse etat d'affichage PRV */
		case XZEXC_TRAME_ETAT_AFF_PRV:
		{
			/*B Recherche du nombre de caracteres du PRV traite */
			for (pl_ListePRV = pg_debutPRV; pl_ListePRV != NULL;
				 pl_ListePRV = pl_ListePRV->Suivant)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "adrrgs =%s Prv AdresseRGS=%s	AdresseIP=%s	PortIP=%d", vl_adrrgs, pl_ListePRV->Config.AdresseRGS, pl_ListePRV->Config.AdresseIP, pl_ListePRV->Config.Port);
				if (pl_ListePRV->Config.Numero.NoEqt == vl_noeqt)
				/*if (!strcmp(pl_ListePRV->Config.AdresseRGS,vl_adrrgs))*/
				{
					break;
				}
			}
			if (pl_ListePRV == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, " Prv Non trouve en memoire mes=%s", vl_Msg);
				return;
			}

			XZST_03EcritureTrace(XZSTC_WARNING, " ici");
			if (vl_typ_rep == XZEXC_REP_BLC_FIN)
			{
				strcat(pl_ListePRV->Trame, pl_trame);
			}
			else if (vl_typ_rep == XZEXC_REP_BLC_INT)
			{
				strcpy(pl_ListePRV->Trame, pl_trame);
				break;
			}

			/*A Envoi d'une demande ST a l'init */
			if (vl_nocmd == XZEXC_CMD_INIT)
			{
				if (!strcmp(pl_ListePRV->Config.AdresseIP, XZECC_IP_VIDE))
				{
					if (EnvoyerTrame("ST\0",
									 XZEXC_TRAME_STAT_COMP,
									 vl_adrrgs,
									 XZEXC_NOCMD,
									 XZEXC_PRIORITEFAIBLE,
									 va_socket) == XDC_NOK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV");
						return;
					}
				}
				else
				{
					if (EnvoyerTrameIP("ST\0",
									   XZEXC_TRAME_STAT_COMP,
									   pl_ListePRV->Config.AdresseIP,
									   pl_ListePRV->Config.Port,
									   vl_adrrgs,
									   XZEXC_NOCMD,
									   XZEXC_PRIORITEFAIBLE,
									   va_socket) == XDC_NOK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV");
						return;
					}
				}
				pl_ListePRV->EtatTrans = EPRVC_EQT_OCCUP;
			}

			if ((vl_nocmd != XZEXC_NOCMD) && (vl_nocmd != XZEXC_CMD_INIT))
				ex_ecriture_trace_cmd(XDC_EQT_PRV,
									  pl_ListePRV->Config.Numero.NoEqt,
									  NULL,
									  pl_trame,
									  vl_nocmd);

			/* Lecture vl_horodate */
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);

			/*B on initialise les variables avec l'etat commande et l'etat courant */
			/*B ceci va permetre la comparaison */
			memcpy(&vl_etatPRV, &pl_ListePRV->Etat, sizeof(XDY_Etat_PRV));
			memcpy(&vl_etat2PRV, &pl_ListePRV->Etat_Commande, sizeof(XDY_Etat_PRV));

			/*B On recupere l'etat affiche sur le prv */
			vl_retour = affecte_etat_prv(pl_ListePRV->Trame, &vl_etatPRV, &vl_contraste);
			if (vl_retour == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, " Trame incomplete mes=%s", vl_Msg);
				if (vl_nocmd > XZEXC_NOCMD)
				{
					/* envoi de CR NOK */
					xzat520_CompteRendu_PRV(vl_nocmd, vl_horodate, XDC_NOK, pl_ListePRV->Config.SiteGestion);
					ex_ecriture_trace_cmd(XDC_EQT_PRV,
										  pl_ListePRV->Config.Numero.NoEqt,
										  NULL,
										  pl_trame,
										  vl_nocmd);
				}
				if (vl_typ_rep == XZEXC_REP_BLC_FIN)
					strcpy(pl_ListePRV->Trame, "");
				return;
			}

			vl_retour = affecte_etat_prv(pl_ListePRV->Trame, &vl_etat2PRV, &vl_contraste);

			/*B COMPARAISON ENTRE ETAT COURANT ET ETAT PRECEDENT */
			strcpy(pl_ListePRV->Etat_Commande.Vitesse, trim(pl_ListePRV->Etat_Commande.Vitesse));
			strcpy(pl_ListePRV->Etat_Commande.Bandeau, trim(pl_ListePRV->Etat_Commande.Bandeau));
			strcpy(vl_etatPRV.Vitesse, trim(vl_etatPRV.Vitesse));
			strcpy(vl_etatPRV.Bandeau, trim(vl_etatPRV.Bandeau));
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd deb:%s.", pl_ListePRV->Etat_Commande.Vitesse);
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd deb:%s.", pl_ListePRV->Etat_Commande.Bandeau);
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd:%s.", vl_etatPRV.Vitesse);
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd:%s.", vl_etatPRV.Bandeau);
			if ((memcmp(&vl_etatPRV, &pl_ListePRV->Etat, sizeof(XDY_Etat_PRV))) /* &&
				   !(pl_ListePRV->Etat_Service & XDC_EQT_HORS_SRV)*/
			)
			{
				/*B Copie du nouvel etat d'affichage pmv ds l'etat courant */
				memcpy(&pl_ListePRV->Etat, &vl_etatPRV, sizeof(XDY_Etat_PRV));

				/*A Envoi de l'etat d'affichage du PRV */
				ep_envoi_affichage_PRV(vl_etatPRV, pl_ListePRV->Config.Numero.NoEqt,
									   pl_ListePRV->Config.SiteGestion);
			}

			/*B COMPARAISON ENTRE ETAT COURANT ET ETAT COMMANDE */
			strcpy(pl_ListePRV->Etat_Commande.Vitesse, trim(pl_ListePRV->Etat_Commande.Vitesse));
			strcpy(pl_ListePRV->Etat_Commande.Bandeau, trim(pl_ListePRV->Etat_Commande.Bandeau));
			strcpy(vl_etat2PRV.Vitesse, trim(vl_etatPRV.Vitesse));
			strcpy(vl_etat2PRV.Bandeau, trim(vl_etatPRV.Bandeau));
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd deb:%s.", pl_ListePRV->Etat_Commande.Vitesse);
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd deb:%s.", pl_ListePRV->Etat_Commande.Bandeau);
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd:%s.", vl_etat2PRV.Vitesse);
			XZST_03EcritureTrace(XZSTC_WARNING, "etat cmd:%s.", vl_etat2PRV.Bandeau);
			if (((strcmp(pl_ListePRV->Etat_Commande.Vitesse, vl_etat2PRV.Vitesse)) ||
				 (strcmp(pl_ListePRV->Etat_Commande.Bandeau, vl_etat2PRV.Bandeau))) &&
				(pl_ListePRV->NumDerCmd == vl_nocmd))
			{
				/* filtrage de l'alerte si eqt hs ou inactif */
				if (/*!(pl_ListePRV->Etat_Service) &&*/
					(vl_nocmd != XZEXC_NOCMD) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					(vl_nocmd != XZEXC_CMD_HORS_BASE))
				{
					xzat520_CompteRendu_PRV(vl_nocmd, vl_horodate, XDC_NOK, pl_ListePRV->Config.SiteGestion);
					XZST_03EcritureTrace(XZSTC_DEBUG1, "/* envoi de CR NOK */");
					/*if (pl_ListePRV->NumCmd != 0) {*/
					ex_env_alerte(vl_horodate,
								  XDC_EQT_PRV,
								  pl_ListePRV->Config.Numero.NoEqt,
								  XZAAC_AFF_DIF,
								  vg_Mode_Fonct,
								  pl_ListePRV->Config.SiteGestion,
								  "affichage PRV",
								  NULL);
					XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF2 prv <> etat comm -> CR OK pour cmd:%d", vl_nocmd);
					memcpy(&pl_ListePRV->Etat_Commande, &vl_etat2PRV, sizeof(XDY_Etat_PRV));
					/*}*/
				}

				if (!(pl_ListePRV->Etat_Service) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					(pl_ListePRV->NumCmd <= XZEXC_NOCMD))
				{
					if (pl_ListePRV->NumCmd != 0)
					{
						ex_env_alerte(vl_horodate,
									  XDC_EQT_PRV,
									  pl_ListePRV->Config.Numero.NoEqt,
									  XZAAC_AFF_DIF,
									  vg_Mode_Fonct,
									  pl_ListePRV->Config.SiteGestion,
									  "affichage PRV",
									  NULL);
						memcpy(&pl_ListePRV->Etat_Commande, &vl_etat2PRV, sizeof(XDY_Etat_PRV));
					}
				}
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "OK?:%d,cmd:%d", pl_ListePRV->Etat_Service, vl_nocmd);
				if (/*!(pl_ListePRV->Etat_Service) &&*/
					(vl_nocmd != XZEXC_NOCMD) &&
					(vl_nocmd != XZEXC_CMD_INIT) &&
					(vl_nocmd != XZEXC_CMD_HORS_BASE))
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1, "/* c'est pareil */");
					/* envoi de CR OK */
					xzat520_CompteRendu_PRV(vl_nocmd, vl_horodate, XDC_OK, pl_ListePRV->Config.SiteGestion);
					XZST_03EcritureTrace(XZSTC_WARNING, "Etat AFF3 prv = etat comm -> CR OK pour cmd:%d", vl_nocmd);
					if (vl_nocmd < XZEXC_NOCMD)
						pl_ListePRV->NumCmd = XZEXC_NOCMD;
					else
						pl_ListePRV->NumCmd = vl_nocmd;
				}
			}

			/*B si le Contraste a change -> envoi du message XDM_ETAT_Contraste  */
			if (((vl_contraste == EPRVC_CONTRASTE) && (pl_ListePRV->Contraste == XDC_CONTRASTENORMAL)) ||
				((vl_contraste != EPRVC_CONTRASTE) && (pl_ListePRV->Contraste == XDC_CONTRASTEBRILLANT)))
			{
				pl_ListePRV->Contraste = (vl_contraste == EPRVC_CONTRASTE) ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL;
				/* Envoi XDM_Etat_Contraste  */
				/* Construction du datagroup */
				if (!(pl_ListePRV->Etat_Service & XDC_EQT_HORS_SRV))
				{
					XDG_EncodeDG2(pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite);
					if (!TipcSrvMsgWrite(pl_DG, XDM_IdentMsg(XDM_ETAT_Contraste), XDM_FLG_SRVMSGWRITE, T_IPC_FT_INT2, XDC_EQT_PRV, T_IPC_FT_INT2, pl_ListePRV->Config.Numero.NoEqt, T_IPC_FT_INT2, pl_ListePRV->Contraste, NULL))
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
				!(pl_ListePRV->Etat_Service) &&
				(pl_ListePRV->ContrasteCmd = !pl_ListePRV->Contraste))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Appel ex_env_alerte");
				ex_env_alerte(vl_horodate,
							  XDC_EQT_PRV,
							  pl_ListePRV->Config.Numero.NoEqt,
							  XZAAC_AFF_DIF,
							  vg_Mode_Fonct,
							  pl_ListePRV->Config.SiteGestion,
							  "Contraste PRV",
							  NULL);
			}
			if (vl_nocmd == XZEXC_CMD_INIT)
			{
				memcpy(&pl_ListePRV->Etat, &vl_etatPRV, sizeof(XDY_Etat_PRV));
				memcpy(&pl_ListePRV->Etat_Commande, &vl_etatPRV, sizeof(XDY_Etat_PRV));
			}
			if (vl_typ_rep == XZEXC_REP_BLC_FIN)
				strcpy(pl_ListePRV->Trame, "");
			break;
		}
		/* c'est une trame de status complet */
		case XZEXC_TRAME_TEST_COM:
		case XZEXC_TRAME_STAT_COMP:
		{

			/*A Recherche du pmv concernes */
			for (pl_ListePRV = pg_debutPRV; pl_ListePRV != NULL;
				 pl_ListePRV = pl_ListePRV->Suivant)
			{
				if (!strcmp(pl_ListePRV->Config.AdresseRGS, vl_adrrgs))
				{
					if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM)
					{
						vl_EtatDefautPRV.Numero = pl_ListePRV->Etat.Numero;
						/*A Si perte de communication avec l'equipement alors envoi d'un message "blanc" */
						if ((vl_typetrame == XZEXC_TRAME_TEST_COM) &&
							(memcmp(&vl_EtatDefautPRV, &pl_ListePRV->Etat, sizeof(XDY_Etat_PRV))))
						{
							memcpy(&pl_ListePRV->Etat, &vl_EtatDefautPRV, sizeof(XDY_Etat_PRV));
							ep_envoi_affichage_PRV(pl_ListePRV->Etat, pl_ListePRV->Config.Numero.NoEqt,
												   pl_ListePRV->Config.SiteGestion);
						}
						return;
					}
					else
					{
						if (vl_typetrame == XZEXC_TRAME_TEST_COM)
							memcpy(&pl_ListePRV->Etat, &vl_EtatDefautPRV, sizeof(XDY_Etat_PRV));
					}
					break;
				}
			}
			XZST_03EcritureTrace(XZSTC_INFO, "ST:%s.", pl_trame);
			/* Decodage de la trame de status */
			ex_decode_status(pl_trame,
							 vl_tabstatus,
							 13,
							 vl_adrrgs,
							 vl_horodate,
							 vl_typeeqt,
							 pl_ListePRV->Config.Numero.NoEqt,
							 vg_NomMachine,
							 pl_ListePRV->Config.SiteGestion);
			/*!! On sait jamais si un jour , on traite de nouveau le ERI
						   ex_traitERI_PRV(vl_tabstatus[7],vl_horodate,vl_typeeqt,pl_ListePRV->Config.Numero.NoEqt,vg_NomMachine, pl_ListePRV->Config.SiteGestion);
			*/

			/*A Traitement des alerte connexion locale */
			if ((vl_tabstatus[3][0] != '0') && (vl_tabstatus[3][0] != ' ') && (vl_tabstatus[3][0] != '\0'))
				vl_Alerte = XZAAC_CLE;
			if ((vl_tabstatus[11][0] != '0') && (vl_tabstatus[11][0] != ' ') && (vl_tabstatus[11][0] != '\0'))
				vl_Alerte = XZAAC_CLL;
			if ((vl_tabstatus[12][0] != '0') && (vl_tabstatus[12][0] != ' ') && (vl_tabstatus[12][0] != '\0'))
				vl_Alerte = XZAAC_CLE;
			if ((vl_Alerte != 0) && !(pl_ListePRV->Etat_Service) && (pl_ListePRV->Mode == XZEXC_MODE_NORMAL))
				ex_env_alerte(vl_horodate,
							  vl_typeeqt,
							  pl_ListePRV->Config.Numero.NoEqt,
							  vl_Alerte,
							  vg_Mode_Fonct,
							  pl_ListePRV->Config.SiteGestion,
							  (vl_Alerte == XZAAC_CLL) ? vl_tabstatus[11] : (vl_tabstatus[12][0] != ' ') ? vl_tabstatus[12]
																										 : vl_tabstatus[3],
							  NULL);
			if (vl_Alerte == 0)
				pl_ListePRV->Mode = XZEXC_MODE_NORMAL;
			else
				pl_ListePRV->Mode = XZEXC_MODE_CONNECTION_LOCALE;

			XZST_03EcritureTrace(XZSTC_DEBUG1, " Decodage status PRV");
		}
		}
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int affecte_etat_prv(char *pa_trame,
					 XDY_Etat_PRV *pa_etatPRV,
					 char *pa_contraste)
/*
* ARGUMENTS EN ENTREE :

*  *pa_trame : trame en entree
*  *pa_etatPicto : etat de l'affichage du PRV
*  *pa_ListeTypePRV : donnees type PRV
*  *pa_contraste : extraction de contraste
*  va_NbLigne    : nombre de ligne du PRV
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*  *pa_etatPRV : etat de l'affichage du picto
*
* CONDITION D'UTILISATION
*
* FONCTION
*  extrait les infos et les affecte dans pa_etatPRV
*
------------------------------------------------------*/
{
	int vl_param;
	char vl_TrameTrav[512];
	char *pl_Trame[12] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	char vl_cligno[15];
	char vl_cligno2[15];
	char vl_aff[60];
	int vl_flash;
	char vl_flashC;
	char *vl_Mot;
	char vl_Texte[32], vl_Alter[32];
	char vl_Contraste;
	int vl_Index = 1,
		vl_Aff = 0,
		vl_Clig = 0,
		vl_Ligne = 0;

	static char *version = "@(#)eprv_tra.c	1.38 05/31/00      :affecte_etat_pmv ";

XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 1 pa_trame=[%s]", pa_trame);
	/* Decoupage de la trame en ligne */
	strcpy(vl_TrameTrav, pa_trame);
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 1 vl_TrameTrav=[%s]", vl_TrameTrav);
	pl_Trame[vl_Index++] = strtok(vl_TrameTrav, CM_SEPARATEUR_LIGNE);
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 1 pl_Trame[%d]=[%s]", vl_Index-1,pl_Trame[vl_Index-1]);
	while ((pl_Trame[vl_Index++] = strtok(NULL, CM_SEPARATEUR_LIGNE)) != NULL)
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 1 pl_Trame[%d]=[%s]", vl_Index-1,pl_Trame[vl_Index-1]);
		;

XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 2");
	for (vl_Index = 1; vl_Index < 7; vl_Index++)
	{
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 3");
		if ((pl_Trame[vl_Index] == NULL) || ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_LIGNE)) == NULL))
			break;
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 4");
		/* Extraction du numero de ligne  */
		if (sscanf(vl_Mot, CM_LEC_NUM_LIGNE, &vl_Ligne) != 1)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Syntaxe etat affichage PRV incorrecte %s", vl_Mot);
			return (XDC_NOK);
		}

XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 5");
		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_CLIGNO)) != NULL)
		{
			if (sscanf(vl_Mot, CM_LEC_CLIGNO, vl_cligno) != 1)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "clignotement 1er ligne incorrecte");
				return (XDC_NOK);
			}
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 6");
			sprintf(vl_cligno2, "CL=%s", vl_cligno);
			if (!strcmp(vl_cligno2, vg_clignolent))
			{
				vl_Clig = XDC_PMV_CLIGNOLENT;
			}
			else
			{
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 7");
				if (!strcmp(vl_cligno2, vg_clignorapide))
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
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 8");
			vl_Clig = XDC_PMV_PASCLIGNO;
		}
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 9");

		if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_CONTRASTE)) != NULL)
		{
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 10");
			sscanf(vl_Mot, CM_LEC_CONTRASTE, &vl_Contraste);
		}

XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 11");
		switch (vl_Ligne)
		{
		case CM_LIGNE_TROIS:
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 12");
			XZST_03EcritureTrace(XZSTC_DEBUG1, "pl_Trame[vl_Index] %s", pl_Trame[vl_Index]);
			XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_Mot %s // CM_DEBUT_AFF = %s", vl_Mot, CM_DEBUT_AFF);
			if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_AFF)) != NULL)
				sscanf(vl_Mot, CM_LEC_AFF, vl_Texte);
			XZST_03EcritureTrace(XZSTC_WARNING, "Mot:%s ", vl_Mot);
			if (*vl_Texte == '0')
				strcpy(pa_etatPRV->Vitesse, "L0" /*XDC_PICTO_PASPICTO*/);
			else
				strcpy(pa_etatPRV->Vitesse, vl_Texte);
			break;
		case CM_LIGNE_DEUX:
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 13");
			XZST_03EcritureTrace(XZSTC_DEBUG1, "pl_Trame[vl_Index] %s", pl_Trame[vl_Index]);
			XZST_03EcritureTrace(XZSTC_DEBUG1, "vl_Mot %s // CM_DEBUT_AFF = %s", vl_Mot, CM_DEBUT_AFF);
			if ((vl_Mot = strstr(pl_Trame[vl_Index], CM_DEBUT_AFF)) != NULL)
			{
				if ((strncmp(vl_Mot + 3, "\"\"", 2)) && ((vl_Mot = strtok(vl_Mot, "\"")) != NULL))
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1, "Mot:%s ", vl_Mot);
					strcpy(vl_Texte, ((vl_Mot = strtok(NULL, "\"")) == NULL) ? "" : vl_Mot);
				}
			}
			supprime_blancs_fin(vl_Texte);
			strcpy(pa_etatPRV->Bandeau, vl_Texte);
			XZST_03EcritureTrace(XZSTC_WARNING, "BANDEAU %s - %s",
								 pa_etatPRV->Bandeau, vl_Texte);
			*pa_contraste = vl_Contraste;
			break;
		default:
XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 14");
			XZST_03EcritureTrace(XZSTC_WARNING, "Numero de ligne affichage PRV incorrecte %d", vl_Ligne);
			return (XDC_NOK);
			break;
		} /*fin du switch sur vl_ligne*/
	}	  /*fin du for*/

XZST_03EcritureTrace(XZSTC_DEBUG1,"affecte_etat_prv : 15");
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
