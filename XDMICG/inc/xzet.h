/*E*/
/* Fichier : $Id: xzet.h,v 1.8 2020/05/27 11:28:53 devgtie Exp $      Release : $Revision: 1.8 $        Date : $Date: 2020/05/27 11:28:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZET  * FICHIER XZET.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzet.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Mercier.O	version 1.2	14 Nov 1994	:
* Mismer.D	version 1.3	28 Nov 1994	:
* Fontaine.C	version 1.4	24 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.5	06 Mai 1997	: Modif pour alerte monaco (DEM/1421)
* AAZ       version 1.6 20 juin 2007 : DEM 662 ajout XZETC_MAX_UNITAIRE, 
*                                            XZETC_NB_ETAT_UNITAIRE
*                                            XDC_GTC_EQUIPEMENT
*                                            XDC_GTC_DOMAINE
* ABE		version 1.4	10 fev 2020	: XZETC_MAX_UNITAIRE passé à 60 pour ouvrage Toulon
------------------------------------------------------ */

#ifndef XZET
#define XZET

/* fichiers inclus */
#include <xdy.h>
#include <xdc.h>
#include "xzst.h"
#include "xdm.h"

/* definitions de constantes exportees */
#define XZETC_MAX_PANNEAU 	30
#define XZETC_NB_ETAT_PAN 	3
/* AAZ ajout XZETC_MAX_UNITAIRE, XZETC_NB_ETAT_UNITAIRE */
#define XZETC_MAX_UNITAIRE 	60
#define XZETC_NB_ETAT_UNITAIRE 	3
/* AAZ */
#define XZETC_MAX_BIT_PAN	5
#define XZETC_NB_MOT_ITOR	4
#define XZETC_NB_MOT_GABARIT	1
#define XZETC_NB_MOT_VITESSE	1
#define XZETC_NB_MOT_NICHE	2
#define XZETC_BIT_0		1
#define XZETC_BIT_1		2
#define XZETC_BIT_2		4
#define XZETC_MASQUE_PAN	0x001f
#define XZETC_TIMEOUT_DISC	5

#define XZETC_CMD_PAN		1
#define XZETC_CMD_TUBE		2
#define XZETC_CMD_ECH		3
/* AAZ */
#define XZETC_CMD_UNITAIRE  4
#define XZETC_CMD_DOMAINE	5
#define XZETC_CMD_SIG   	6
#define XZETC_CMD_ECL   	7
#define XZETC_CMD_VEN	    8
#define XZETC_CMD_ENR   	9
#define XZETC_CMD_INC   	10
#define XZETC_CMD_EXP   	11
#define XZETC_CMD_SYS   	12
#define XZETC_CMD_LTN   	13
/* AAZ */

#define XZETC_ADRESSE_FLAG		2570
#define XZETC_TAILLE_FLAG		16
#define XZETC_FLAG_MARCHE_DEGRADEE	0
#define XZETC_FLAG_BASCULEMENT		1
#define XZETC_FLAG_TELEALARME		2
#define XZETC_FLAG_SIGNALISATION 	3
#define XZETC_FLAG_ITOR			4
#define XZETC_FLAG_ETOR			5
#define XZETC_FLAG_SEQ_TUBE		6
#define XZETC_FLAG_SEQ_ECH		7
#define XZETC_FLAG_TELECOMMANDE		8
#define XZETC_FLAG_ALERTE		11

#define XZETC_TELEALARME	XDC_GTC_TELEALARME
#define XZETC_ETOR		XDC_GTC_ETOR
#define XZETC_PANNEAU		XDC_GTC_PANNEAU
#define XZETC_DISCORDANCE	XDC_GTC_DISCORDANCE
#define XZETC_SEQUENCE		XDC_GTC_SEQUENCE
#define XZETC_CAPTEUR		XDC_GTC_CAPTEUR
#define XZETC_ALERTE		XDC_EQT_GAB
/* AAZ */
#define XZETC_EQUIPEMENT	XDC_EQT_EQT
#define XZETC_DOMAINE	    XDC_EQT_DOMAINE
/* AAZ */

/* definitions de types exportes */

/* Commande signalisation GTC de Nice */
typedef struct	{
	XDY_Mot		NumTE;			   /* Numero Eqt Tube ou Echangeur */
	XDY_Mot		NumPan[XZETC_MAX_PANNEAU]; /* Numeros Eqt panneau */
	XDY_Mot		NumCmd[XZETC_MAX_PANNEAU]; /* Numeros de commande d'affichage */
	XDY_Octet	NumSeq;			   /* Numero de sequence associee */
}	XZETT_CMD_SIG_NIC;		/* Commande signalisation GTC de Nice */


/* Commande signalisation GTC de Nice */
typedef struct	{
	XDY_Mot		NumTE;			   /* Numero Eqt Tube ou Echangeur */
	XDY_Mot		NumTypeEqt[XZETC_MAX_UNITAIRE]; /* Numeros Type Eqt */
	XDY_Mot		NumEqt[XZETC_MAX_UNITAIRE]; /* Numeros Eqt */
	XDY_Mot		NumCmd[XZETC_MAX_UNITAIRE]; /* Numeros de commande d'affichage */
	XDY_Octet	NumSeq;			   /* Numero de sequence associee */
}	XZETT_CMD_SIG_NIX;		/* Commande signalisation GTC de Nice */

/* Commande domaine GTC de Nice */
typedef struct	{
	XDY_Mot		Domaine;
	XDY_Mot		TypeOuvrage;
	XDY_Mot		IdOuvrage;
	XDY_Mot		Mode;
}	XZETT_CMD_DOM_NIX;		/* Commande domaine GTC de Nice */

/* Commande sequence GTC de Nice */
typedef struct	{
	XDY_Mot		NumTE;		/* Numero de Tube ou d'Echangeur */
	XDY_Octet	NumSEQ;		/* numero de sequence */
}	XZETT_CMD_SEQ_NIC;		/* Commande sequence GTC de Nice */

/* Commande sequence GTC de Nice */
typedef struct	{
	XDY_Octet	TypeOuvrage;
	XDY_Mot		NumDom;		/* Numero du domaine */
	XDY_Octet	NumSeq;		/* numero de sequence */
}	XZETT_CMD_SEQ_NIX;		/* Commande sequence GTC de Nice */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation tunnel.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZET01_Commande_SIGT (	XZETT_CMD_SIG_NIC	va_Commande_SIG_NIC,
					int			va_NoCMD );		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_AFF	Numero d'affichage invalide
*   XZEXC_ERR_EQT	Numero de tunnel incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation tunnel
*   de la fenetre de pilotage Tunnel GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SIGT en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SIGT .
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation echangeur.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZET02_Commande_SIGE (	XZETT_CMD_SIG_NIC	va_Commande_SIG_NIC,
					int			va_NoCMD );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_AFF	Numero d'affichage invalide
*   XZEXC_ERR_EQT	Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation echangeur
*   de la fenetre de pilotage Echangeur GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SIGE en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SIGE .
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence tunnel 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZET03_Commande_SEQT (	XZETT_CMD_SEQ_NIC	va_Commande_SIG_NIC,
					int			va_NoCMD );		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_SEQ	Numero de sequence invalide
*   XZEXC_ERR_EQT	Numero de tunnel incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence tunnel
*   de la fenetre de pilotage tunnel GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQT en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQT .
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZET04_Commande_SEQE (	XZETT_CMD_SEQ_NIC	va_Commande_SIG_NIC,
					int			va_NoCMD );		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_SEQ	Numero de sequence invalide
*   XZEXC_ERR_EQT	Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence echangeur
*   de la fenetre de pilotage echangeur GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQE en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQE .
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande unitaire du domaine Signalisation.
*
--------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET05_Commande_SIG    ( XZETT_CMD_SIG_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );    

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande unitaire du domaine Signalisation
*   de la fenetre de pilotage Signalisation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SIG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SIG .
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande unitaire du domaine Eclairage.
*
--------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET06_Commande_ECL    ( XZETT_CMD_SIG_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );    

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande unitaire du domaine Eclairage
*   de la fenetre de pilotage Eclairage GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_ECL en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_ECL .
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande unitaire du domaine Ventilation.
*
--------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET07_Commande_VEN    ( XZETT_CMD_SIG_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );    

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande unitaire du domaine Ventilation
*   de la fenetre de pilotage Ventilation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_VEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_VEN .
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande unitaire du domaine Energie.
*
--------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET08_Commande_ENR    ( XZETT_CMD_SIG_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );    

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande unitaire du domaine Energie
*   de la fenetre de pilotage Energie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_ENR en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_ENR .
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande unitaire du domaine Protection incendie.
*
--------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET09_Commande_INC    ( XZETT_CMD_SIG_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );    

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande unitaire du domaine Protection incendie
*   de la fenetre de pilotage Protection incendie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_INC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_INC .
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande domaine.
*
--------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET10_Commande_Domaine ( XZETT_CMD_DOM_NIX va_Commande_Domaine_NIX,
                                     int               va_NoCMD );    

									 
extern int XZET11_Commande_EXP    ( XZETT_CMD_SIG_NIX va_Commande_SIG_NIX,
int               va_NoCMD );    

/*

* ARGUMENTS EN ENTREE :
*   va_Commande_Domaine_NIX  Commande a effectuee
*   va_NoCMD                 Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande domaine
*   de la fenetre de pilotage domaine GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_DOMAINE en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_DOMAINE .
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence domaine Signalisation 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET40_Commande_SEQSIG ( XZETT_CMD_SEQ_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero de l'equipement du domaine Signalisation incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence domaine Signalisation
*   de la fenetre de pilotage domaine Signalisation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQSIG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQSIG .
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence domaine Eclairage 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET41_Commande_SEQECL ( XZETT_CMD_SEQ_NIX  va_Commande_SIG_NIX,
                                    int                va_NoCMD );        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero de l'equipement du domaine Eclairage incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence domaine Eclairage
*   de la fenetre de pilotage domaine Eclairage GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQECL en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQECL .
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence domaine Ventilation 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET42_Commande_SEQVEN ( XZETT_CMD_SEQ_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero de l'equipement du domaine Ventilation incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence domaine Ventilation
*   de la fenetre de pilotage domaine Ventilation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQVEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQVEN .
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence domaine Energie 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET43_Commande_SEQENR ( XZETT_CMD_SEQ_NIX va_Commande_SIG_NIX,
                                    int               va_NoCMD );

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero de l'equipement du domaine Energie incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence domaine Energie
*   de la fenetre de pilotage domaine Energie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQENR en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQENR .
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence domaine Protection incendie 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZET44_Commande_SEQINC ( XZETT_CMD_SEQ_NIX  va_Commande_SIG_NIX,
                                    int                va_NoCMD );        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero de l'equipement du domaine Protection incendie incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence domaine Protection incendie
*   de la fenetre de pilotage domaine Protection incendie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQINC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SEQINC .
*
------------------------------------------------------*/


extern int XZET45_Commande_SEQEXP ( XZETT_CMD_SEQ_NIX  va_Commande_SIG_NIX,
                                    int                va_NoCMD );        


#endif
