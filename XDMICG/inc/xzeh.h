/*E*/
/* Fichier : $Id: xzeh.h,v 1.9 2016/05/31 16:36:57 devgfi Exp $      Release : $Revision: 1.9 $        Date : $Date: 2016/05/31 16:36:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEH  * FICHIER xzeh.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzeh.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	11 Aug 1994	: Creation
* Mismer.D	version 1.2	06 Dec 1994	:
* Volcic.F	version 1.3	01 Mar 1995	:
* Milleville.T	version 1.4	08 Mar 1995	: Modification du Nom de la RS232 
* Fontaine.C	version 1.5	24 Mai 1995	: Reprise entete, historique
* Niepceron	version 1.6	14 Jan 1998	: Ajout des nom de la RS232 pour les autre machine HP
* Cluchague	version 1.7	04 Sep 2002	: modif tty pour RS232_800 et ajout RS232_8x6 (REPERE1) 
* Niepceron	version 1.8	30 Mars 2009	: ajout tty pour RS232 ia64 DEM875
* JPL		version 1.9	31 Mai 2016	: Remplacement devices RS232 par un unique defaut (DEM 1167)
------------------------------------------------------ */

#ifndef XZEH
#define XZEH

/* fichiers inclus */

#include <xzss.h>
#include <xzsm.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>

/* definitions de constantes exportees */

#define XZEHC_NOM_RS232_DEFAUT	"/dev/tty1p0"


#define XZEHC_RS232_D9600 	XZSSC_RS_9600
#define XZEHC_RS232_D4800 	XZSSC_RS_4800
#define XZEHC_RS232_INFO7 	7
#define XZEHC_RS232_INFO8 	8
#define XZEHC_RS232_STOP1 	1
#define XZEHC_RS232_STOP2 	2
#define XZEHC_RS232_PAIR 	0
#define XZEHC_RS232_IMPAIR 	1
#define XZEHC_RS232_NOPAR 	2


#define	XZEHC_FIN_HORODATE "\002"
#define	XZEHC_TAILLE_HORODATE 23

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Ouverture de la ligne serie connecte au Centrale horaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEH01_Ouverture_Horloge ( 	char         *pa_Device, 
			       		unsigned int va_Vitesse,
			       		unsigned int va_NbitInfo,
			       		unsigned int va_NbitStop,
			       		unsigned int va_Parite);

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   pa_Horodate		Horodate donn≈e par la centrale horaire
*
* CODE RETOUR : 
* XDC_OK 		Succ≈s
* XZEHC_DEV_CFN		Configuration du device 
*			incorrecte.
*
* CONDITION D'UTILISATION
*   Est appelee une fois a l'initialisation afin de pouvoir lire l'horodate ulterieurement
*
* FONCTION 
*   Controle des parametre d'entree.
*   Ouverture et configuration de la liaison serie par appel de la primitive XZSS08
*   Memorisation du descripteur de fichier dans XZEHV_FD_HORLOGE
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Lecture sur la ligne serie connecte au Centrale horaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEH02_Lecture_Horloge ( char         *pa_Horodate );

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   pa_Horodate		Horodate donn≈e par la centrale horaire
*
* CODE RETOUR : 
* XDC_OK 		Succ≈s
* XZEHC_DEV_CFN		Configuration du device 
*			incorrecte.
*
* CONDITION D'UTILISATION
*   Demande de lecture de l'horodate de la centrale 
*   horaire li≈e au PC d'un site ou du CI
*
* FONCTION 
*   R≈ception de l'horadate en r≈ponse par appel » la
*   primitive de lecture liaison s≈rie XZSS10.
*   Retour de la r≈ponse dans pa_Horodate.
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fermeture la ligne serie connectee au Centrale horaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEH03_Fermeture_Horloge ( void );

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
* XDC_OK 		Succ≈s
*
* CONDITION D'UTILISATION
*   Est appelee un fois, des qu'il n'est plus necessaire de lire l'horodate
*
* FONCTION 
*
*   Fermeture de la liaison serie par appel a la primitive XZSS09, en specifiant
*   le descripteur de fichier XZEHV_FD_HORLOGE
------------------------------------------------------*/

#endif
