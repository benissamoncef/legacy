/*E*/
/*Fichier : $Id: xzir.c,v 1.13 2011/04/04 15:33:07 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2011/04/04 15:33:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE XZIR * FICHIER xzir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module rassemble toutes les communications a destination 
* commune Dataviews et Applix.
*	Ω2.5.2. du tome 2.3. de la Conception G≈n≈rale
* 
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	28 sept 1994	: Creation			V1.1
**************************
* Guilhou  13 sept 1994	: Ajout XZIR01 et 02		V1.2
**************************
* Niepceron  28 oct 1994	: Modification de XZIR01_Creer_Alerte en
*                XZIR01_Alerte et suppr de XZIR02_Supprimer_Alerte V1.3
**************************
* Mercier	28 Oct 1994	: Modification 		V1.3
*		type du premier champ de XZIR01
**************************
* Lecrivain  24 Nov 1994	: Ecriture de la fonction XZIR01_Alerte V1.5,1.6
**************************
gaborit 29 dec 1994 : tentative ecriture de traces correctes (1.7)
guilhou 30 dec 1994 : parametre Creation dans XZIR01 (1.8)
volcic  01 Aou 1995 : Ajout XZIR02_Alerte_PAU_GN (1.9)
volcic  01 Aou 1995 : Modif XZIR02_Alerte_PAU_GN (1.10)
torregrossa  09 Jan 1998 : Modif interface de XZIR01_Alerte (1532) (1.11)
JMG 12 dec 2007 : RAU au CI DEM716 1.12
JMG	21/03/11 : ajout XZIR11_Creer_Alerte_localisee 1.13
------------------------------------------------------*/

/* fichiers inclus */

#include "xzir.h"
#include "xzsm.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzir.c,v 1.13 2011/04/04 15:33:07 pc2dpdy Exp $ : xzir" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* permet de creer une alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIR01_Alerte(
                      XDY_Entier	va_numAlerte,
                      XDY_Horodate 	va_date, 
                      XDY_TypeAlerte 	va_typeAlerte,
                      XDY_NomType 	va_nomType,
                      XDY_TypeEqt 	va_typeEqt,
                      XDY_Eqt 		va_numEqt,
                      XDY_NomEqt        va_nom_Eqt,
                      XDY_TexteAlerte 	va_texte,
                      XDY_Autoroute 	va_autoroute, 
                      XDY_PR 		va_pr,
                      XDY_Sens 		va_sens,
                      XDY_Booleen       va_creation,
		      XDY_Octet		va_siteAlerte
                       )
/*
* ARGUMENTS EN ENTREE :
* va_numAlerte		: numero de l'alerte
* va_date		: horodate de debut de l'alerte
* va_typeAlerte		: type
* va_nomType		: nom du type de l'alerte
* va_typeEqt		: type de l'equipement
* va_numEqt		: numero de l'equipement
* va_numEqt		: nom    de l'equipement
* va_texte		: texte de l'alerte
* va_autoroute		: autoroute
* va_pr			: pr
* va_sens		: sens
* va_creation		: Creation  : XDC_OUI ou XDC_NON
* va_siteAlerte		: site de l'alerte
*
* ARGUMENTS EN SORTIE :
* aucun
*
* CODE RETOUR : 
*       XDC_NOK         : Erreur d'ex≈cution de la fonction
*       XDC_OK          : Ex≈cution correcte
*       XZIRC_ERR_PAR   : Erreur dans les param…tres d'entr≈e
*       XZIRC_ERR_EXE   : Erreur d'execution dans la fonction
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzir.c,v 1.13 2011/04/04 15:33:07 pc2dpdy Exp $ : XZIR01_Alerte" ;
XDY_NomSite	vl_nomSite;
XZSCT_Datagroup	vl_DG;
XZSMT_Horodate	vl_chaine_date;
int		vl_taille_msg;
char		*pl_str;

/*A Recuperation du nom du site local */
if (XZSC_04NomSite(vl_nomSite) != XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation nom site a echoue\n");
	return (XDC_NOK);
}

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG, XDG_ICREER_ALERTE, vl_nomSite);

/*A Construction du message */
XZSM_11ConversionHorodate (va_date, (XZSMT_Horodate *) vl_chaine_date);

XZST_03EcritureTrace (XZSTC_INTERFACE,
			"IN %s \ndatagroup %s, alerte %d, date %s, type %d-%s, equip. %d, numero %d-%s, texte \"%s\", autoroute %d, PR %d, sens %d\n, creation %d\n sit %d\n",
			version,vl_DG,
			(int) va_numAlerte,
			vl_chaine_date, 
			(int) va_typeAlerte,
			va_nomType,
			(int) va_typeEqt,
			(int) va_numEqt,
			va_nom_Eqt,
			va_texte,
			(int) va_autoroute, 
			(int) va_pr,
			(int) va_sens,
			(int) va_creation,
			(int) va_siteAlerte);

/*A Allocation d'un espace suffisant pour les arguments chaines et entiers */
vl_taille_msg=strlen(vl_chaine_date) + strlen(va_nomType) + strlen(va_nom_Eqt) +
		strlen(va_texte) + (7 * 11) + 12 ;
if ((pl_str = (char *) malloc (vl_taille_msg)) == NULL) {
	XZST_03EcritureTrace(XZSTC_WARNING, "Allocation %d octets impossible\n",
				vl_taille_msg);
	return (XDC_NOK);
}

sprintf(pl_str, "%d|%s|%d|%s|%d|%d|%s|%s|%d|%d|%d|%d|%d",
	(int) va_numAlerte,
	vl_chaine_date, 
	(int) va_typeAlerte,
	va_nomType,
	(int) va_typeEqt,
	(int) va_numEqt,
	va_nom_Eqt,
	va_texte,
	(int) va_autoroute, 
	(int) va_pr,
	(int) va_sens,
	(int) va_creation,
	(int) va_siteAlerte);

if (!TipcSrvMsgWrite(vl_DG,
	XDM_IdentMsg(XDM_ICREER_ALERTE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,
	T_IPC_FT_INT4, COM_CANAL_MTLL,
	T_IPC_FT_STR, pl_str,
	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message de creation d'alerte"); 
  free(pl_str);
  return(XZIRC_ERR_EXE);
}

TipcSrvFlush();
free(pl_str);
XZST_03EcritureTrace (XZSTC_INTERFACE,"OUT %s",version);
return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* permet de creer une alerte pau GN
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIR02_Alerte_PAU_GN(
                      XDY_TypeAlerte 	va_typeAlerte,
                      XDY_TypeEqt 	va_typeEqt,
                      XDY_Eqt 		va_numEqt
                       )
/*
* ARGUMENTS EN ENTREE :
* va_typeAlerte		: type
* va_typeEqt		: type de l'equipement
* va_numEqt		: numero de l'equipement
*
* ARGUMENTS EN SORTIE :
* aucun
*
* CODE RETOUR : 
*       XDC_NOK         : Erreur d'ex≈cution de la fonction
*       XDC_OK          : Ex≈cution correcte
*       XZIRC_ERR_PAR   : Erreur dans les param…tres d'entr≈e
*       XZIRC_ERR_EXE   : Erreur d'execution dans la fonction
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzir.c,v 1.13 2011/04/04 15:33:07 pc2dpdy Exp $ : XZIR02_Alerte_PAU_GN" ;
XDY_NomSite	vl_nomSite;
XZSCT_Datagroup	vl_DG;
XZSMT_Horodate	vl_chaine_date;
int		vl_taille_msg;
char		*pl_str;

/*A Recuperation du nom du site local */
if (XZSC_04NomSite(vl_nomSite) != XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation nom site a echoue\n");
	return (XDC_NOK);
}

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG, XDG_IAX_TEXTE, vl_nomSite);

/*A Construction du message */
XZSM_11ConversionHorodate (0.0, (XZSMT_Horodate *) vl_chaine_date);

XZST_03EcritureTrace (XZSTC_WARNING,
			"IN %s \ndatagroup %s, type %d, equip. %d, numero %d",
			version,vl_DG,
			(int) va_typeAlerte,
			(int) va_typeEqt,
			(int) va_numEqt);

/*A Allocation d'un espace suffisant pour les arguments chaines et entiers */
vl_taille_msg=sizeof(XZSMT_Horodate) + sizeof(XDY_NomType) + sizeof(XDY_NomEqt) +
		sizeof(XDY_TexteAlerte) + (7 * 11) + 12 ;
if ((pl_str = (char *) malloc (vl_taille_msg)) == NULL) {
	XZST_03EcritureTrace(XZSTC_WARNING, "Allocation %d octets impossible\n",
				vl_taille_msg);
	return (XDC_NOK);
}

/*sprintf(pl_str, "0|%s|%d|PAU_GN|%d|%d|PAU|RIEN|0|0|0|0",*/
sprintf(pl_str, "0|%s|%d|PAU_GN|%d|%d|PAU|%s|0|0|0|0",
	vl_chaine_date, 
	(int) va_typeAlerte,
	(int) va_typeEqt,
	(int) va_numEqt,
	vl_nomSite);

if (!TipcSrvMsgWrite(vl_DG,
	XDM_IdentMsg(XDM_ICREER_ALERTE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,
	T_IPC_FT_INT4, COM_CANAL_MTMT_COMM_GN,
	T_IPC_FT_STR, pl_str,
	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message de creation d'alerte PAU GN"); 
  free(pl_str);
  return(XZIRC_ERR_EXE);
}

/*on envoie aussi le message au CI*/
XDG_EncodeDG2(vl_DG, XDG_IAX_TEXTE, "CI");
if (!TipcSrvMsgWrite(vl_DG,
	XDM_IdentMsg(XDM_ICREER_ALERTE),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_INT4, COM_SENDPOKE,
	T_IPC_FT_INT4, COM_CANAL_MTMT_COMM_GN,
	T_IPC_FT_STR, pl_str,
	NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message de creation d'alerte PAU GN");
  free(pl_str);
  return(XZIRC_ERR_EXE);
}

TipcSrvFlush();
free(pl_str);
XZST_03EcritureTrace (XZSTC_INTERFACE,"OUT %s",version);
return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
* Permet de mettre » jour l'≈tat de disponibilit≈ d'un ≈quipement.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZIR03_Modif_Indispo(XDY_Eqt va_NumEqt, XDY_TypeEqt va_TypeEqt,XDY_Octet va_Etat
          )

/*
* ARGUMENTS EN ENTREE :
*
*	va_NumEqt	Num≈ro de l'≈quipement
*	va_TypeEqt	Type de l'≈quipement
*	va_Etat		Etat de disponibilit≈ de l'≈quipement. Correspond » un OU logique entre :
*					XZIRC_INHIBE
*					XZIRC_DESACTIVE
*					XZIRC_CRITIQUE
*					XZIRC_HS
*					XZIRC_MINEURE
*					XZIRC_MAJEURE
*
* ARGUMENTS EN SORTIE :
* 
* 	Aucun
*
* CODE RETOUR : 
*
*	XDC_NOK 	: Erreur d'ex≈cution de la fonction
*	XDC_OK  	: Ex≈cution correcte
*	XZIRC_ERR_PAR	: Erreur dans les param…tres d'entr≈e
*
* CONDITION D'UTILISATION
*
*	Primitive appel≈e d…s qu'un changement d'≈tat » lieu.
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *version = "$Id: xzir.c,v 1.13 2011/04/04 15:33:07 pc2dpdy Exp $ : XZIR03_Modif_Indispo" ;
	XZST_03EcritureTrace (XZSTC_INTERFACE,"IN %s",version);
	/*! je ne fait strictement rien ! */
	XZST_03EcritureTrace (XZSTC_INTERFACE,"OUT %s",version);
	return (XDC_OK);
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* permet de creer une alerte localisee
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int XZIR11_Alerte_Localisee(
                      XDY_Entier        va_numAlerte,
                      XDY_Horodate      va_date,
                      XDY_TypeAlerte    va_typeAlerte,
                      XDY_NomType       va_nomType,
                      XDY_TypeEqt       va_typeEqt,
                      XDY_Eqt           va_numEqt,
                      XDY_NomEqt        va_nom_Eqt,
                      XDY_TexteAlerte   va_texte,
                      XDY_Autoroute     va_autoroute,
                      XDY_PR            va_pr,
                      XDY_Sens          va_sens,
                      XDY_Booleen       va_creation,
                      XDY_Octet         va_siteAlerte,
                      XDY_Mot           va_portion,
                      XDY_Entier        va_translation
                       )

/*
* ARGUMENTS EN ENTREE :
* va_numAlerte          : numero de l'alerte
* va_date               : horodate de debut de l'alerte
* va_typeAlerte         : type
* va_nomType            : nom du type de l'alerte
* va_typeEqt            : type de l'equipement
* va_numEqt             : numero de l'equipement
* va_numEqt             : nom    de l'equipement
* va_texte              : texte de l'alerte
* va_autoroute          : autoroute
* va_pr                 : pr
* va_sens               : sens
* va_creation           : Creation  : XDC_OUI ou XDC_NON
* va_siteAlerte         : site de l'alerte
*
* ARGUMENTS EN SORTIE :
* aucun
*
* CODE RETOUR :
*       XDC_NOK         : Erreur d'ex–cution de la fonction
*       XDC_OK          : Ex–cution correcte
*       XZIRC_ERR_PAR   : Erreur dans les param‹tres d'entr–e
*       XZIRC_ERR_EXE   : Erreur d'execution dans la fonction
*
------------------------------------------------------*/
{
/*A Definition des variables locales */
static char *version = "$Id: xzir.c,v 1.13 2011/04/04 15:33:07 pc2dpdy Exp $ : XZIR11_Alerte_localisee" ;
XDY_NomSite     vl_nomSite;
XZSCT_Datagroup vl_DG;
XZSMT_Horodate  vl_chaine_date;
int             vl_taille_msg;
char            *pl_str;

/*A Recuperation du nom du site local */
if (XZSC_04NomSite(vl_nomSite) != XDC_OK) {
        XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation nom site a echoue\n");
        return (XDC_NOK);
}

/*A Construction du datagroup d'emission */
XDG_EncodeDG2(vl_DG, XDG_ICREER_ALERTE, vl_nomSite);

/*A Construction du message */
XZSM_11ConversionHorodate (va_date, (XZSMT_Horodate *) vl_chaine_date);

XZST_03EcritureTrace (XZSTC_WARNING,
                        "IN %s \ndatagroup %s, alerte %d, date %s, type %d-%s, equip. %d, numero %d-%s, texte \"%s\", autoroute %d, PR %d, sens %d\n, creation %d\n sit %d\n portion %d\n translation %d\n",
                        version,vl_DG,
                        (int) va_numAlerte,
                        vl_chaine_date,
                        (int) va_typeAlerte,
                        va_nomType,
                        (int) va_typeEqt,
                        (int) va_numEqt,
                        va_nom_Eqt,
                        va_texte,
                        (int) va_autoroute,
                        (int) va_pr,
                        (int) va_sens,
                        (int) va_creation,
                        (int) va_siteAlerte,
                        (int) va_portion,
                        (int) va_translation);

/*A Allocation d'un espace suffisant pour les arguments chaines et entiers */
vl_taille_msg=strlen(vl_chaine_date) + strlen(va_nomType) + strlen(va_nom_Eqt) +
                strlen(va_texte) + (7 * 11) + 12 +50 ;
if ((pl_str = (char *) malloc (vl_taille_msg)) == NULL) {
        XZST_03EcritureTrace(XZSTC_WARNING, "Allocation %d octets impossible\n",
                                vl_taille_msg);
        return (XDC_NOK);
}

sprintf(pl_str, "%d|%s|%d|%s|%d|%d|%s|%s|%d|%d|%d|%d|%d|%d|%d",
        (int) va_numAlerte,
        vl_chaine_date,
        (int) va_typeAlerte,
        va_nomType,
        (int) va_typeEqt,
        (int) va_numEqt,
        va_nom_Eqt,
        va_texte,
        (int) va_autoroute,
        (int) va_pr,
        (int) va_sens,
        (int) va_creation,
        (int) va_siteAlerte,
        (int) va_portion,
        (int) va_translation);

if (!TipcSrvMsgWrite(vl_DG,
        XDM_IdentMsg(XDM_ICREER_ALERTE_LOCALISEE),
        XDM_FLG_SRVMSGWRITE,
        T_IPC_FT_INT4, COM_SENDPOKE,
        T_IPC_FT_INT4, COM_CANAL_MTLL,
        T_IPC_FT_STR, pl_str,
        NULL))
{
  XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer le message de creation d'alerte");
  free(pl_str);
  return(XZIRC_ERR_EXE);
}
TipcSrvFlush();
free(pl_str);
XZST_03EcritureTrace (XZSTC_INTERFACE,"OUT %s",version);
return (XDC_OK);
}

