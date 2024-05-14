/*E*/
/*Fichier : $Id: xzir.h,v 1.7 2011/04/04 15:31:25 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2011/04/04 15:31:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE XZIR * FICHIER XZIR.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module rassemble toutes les communications a destination
* commune Dataviews et Applix.
*       Ω2.5.2. du tome 2.3. de la Conception G≈n≈rale
*
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	28 sept 1994	: Creation			V1.1
* Guilhou  13 oct 1994	: Ajout interfaces XZIR01 et 02	V1.2
* Niepceron  28 oct 1994	: Modification de XZIR01_Creer_Alerte en
                 XZIR01_Alerte et suppr de XZIR02_Supprimer_Alerte V1.3
*************************
* MERCIER O.	28 Oct 1994	: Modification		V1.3
*		type du 1er argument de XZIR01.
* Volcic F.	01 Aou 1995	: Ajout XZIR02_Alerte_PAU_GN	V1.5
* Torregrossa  09 Jan 1998 : Modif interface de XZIR01_Alerte (1532) (1.6)
* JMG	18/03/11 : ajout XZIR11_Alerte_Localisee 1.7
------------------------------------------------------ */

#ifndef XZIR
#define XZIR

/* fichiers inclus */
#include        "xdc.h"
#include        "xdy.h"
#include        "xdg.h"
#include        "xdm.h"

#include        "xzst.h"
#include        "xzic.h"

/* definitions de constantes exportees */

#define	XZIRC_INHIBE		2
#define	XZIRC_DESACTIVE		4
#define	XZIRC_CRITIQUE		8
#define	XZIRC_HS		1
#define	XZIRC_MINEURE		16
#define	XZIRC_MAJEURE		32	
#define XZIRC_ERR_PAR		3
#define XZIRC_ERR_EXE		5

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Permet de mettre » jour l'≈tat de disponibilit≈ d'un ≈quipement.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int     XZIR03_Modif_Indispo(XDY_Eqt va_NumEqt, XDY_TypeEqt va_TypeEqt,XDY_Octet va_Etat
	  );

/*
* ARGUMENTS EN ENTREE :
*
*       va_NumEqt       Num≈ro de l'≈quipement
*       va_TypeEqt      Type de l'≈quipement
*       va_Etat         Etat de disponibilit≈ de l'≈quipement. Correspond » un OU logique entre :
*                                       XZIRC_INHIBE
*                                       XZIRC_DESACTIVE
*                                       XZIRC_CRITIQUE
*                                       XZIRC_HS
*                                       XZIRC_MINEURE
*                                       XZIRC_MAJEURE
*
* ARGUMENTS EN SORTIE :
*
*       Aucun
*
* CODE RETOUR :
*
*       XDC_NOK         : Erreur d'ex≈cution de la fonction
*       XDC_OK          : Ex≈cution correcte
*       XZIRC_ERR_PAR   : Erreur dans les param…tres d'entr≈e
*
* CONDITION D'UTILISATION
*
*       Primitive appel≈e d…s qu'un changement d'≈tat » lieu.
------------------------------------------------------*/





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* permet de creer ou supprimer une alerte dans la liste des alertes et
* sur le synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZIR01_Alerte(
                      XDY_Entier 	va_numAlerte,
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
                      XDY_Octet       	va_siteAlerte
          );

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
*
* CONDITION D'UTILISATION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* permet de creer une alerte PAU GN
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZIR02_Alerte_PAU_GN(
                      XDY_TypeAlerte 	va_typeAlerte,
                      XDY_TypeEqt 	va_typeEqt,
                      XDY_Eqt 		va_numEqt);

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
*
* CONDITION D'UTILISATION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* permet de creer ou supprimer une alerte dans la liste des alertes et
* sur le synoptique
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
          );


#endif
