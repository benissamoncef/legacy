/*E*/
/*  Fichier : $Id: eqtx_don.h,v 1.8 2017/12/21 15:32:33 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2017/12/21 15:32:33 $
------------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------------------------------------------
* MODULE TEQTX * FICHIER eqtx_don.h
------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees des fichiers de la tache TEQTX
*
------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	30 Oct 1996	: Creation
* Orengo.A	version 1.2	06 Nov 1996	: Modification du nom des equipements
* Mismer.D	version 1.3	13 Jan 1997	: Modification protocole (DEM/1393)
* cluchague	version 1.4	03 Jan 2000	: plus de 10 station meteo (NB_EQT_MAX_TEQTX)
* cluchague	version 1.5	03 Jan 2000	: retour en arriere                                
* JMG   31/10/07        : ajout site de gestion 1.6 
* JPL	25/02/11 : Migration architecture HP ia64 (DEM 975) : declaration fonction 'lancement_timer_eqtx'  1.7
----------------------------------------------------------------------------------------*/

#ifndef EQTX_DON
#define EQTX_DON


/* Declaration des defines */
/* ----------------------- */

#define		CLE_CYCLE_TIMER_QTX_1H			"Cle_CYCLE_TIMER_QTX_1h"
#define		EQTXC_REVEIL_1H			 	(60*60)
#define		EQTXC_REVEIL_1MN			60

#define		CLE_CYCLE_TIMER_QTX_LIGNE_ESCLAVE	"Cle_CYCLE_TIMER_QTX_LIGNE_ESCLAVE_"
#define		EQTXC_REVEIL_LIGNE_ESCLAVE		(60*60+8*60)

#define		CLE_SYNCHRO_HEURE_QTX			"Cle_SYNCHRO_HEURE_QTX"

#define		CLE_DEMANDE_HEURE_X_DONNEES_QTX		"Cle_DEMANDE_HEURE_DONNEES_QTX_"
#define		EQTXC_DEMANDE_HEURE_X_DONNEES_QTX	":50:00"		

#define		CLE_CYCLE_TIMER_QTX_FENETRE_LCR		"Cle_CYCLE_TIMER_QTX_FENETRE_LCR"

#define		NOM_FICHIER_EQTX_RECEPTION_DONNEES_METEO "EQTX_reception_DONNEES_METEO"
#define		NOM_FICHIER_EQTX_HISTORIQUE_DONNEES	"EQTX_historique_donnees"

#define		FORMAT_FICHIER_RECEPTION_DONNEES_METEO	"\
Fichier Indicateur de reception des donnees Meteo \n\
Donnees en date du %02d %02d %d\n"

#define TAILLE_LIGNE_FICHIER_CONFIG_ALARME_ALERTE	300
#define NB_MAX_ALARME_ALERTE_FICHIER_CONFIG		100

#define NB_EQT_MAX_TEQTX				20

/* definitions de types exportes */
/* ----------------------------- */

typedef enum
{
	Non_valide,
	Valide
}
QTX_Validation;

typedef struct
{
	int				Numero_Alerte;
	int				Numero_Alarme;
	XDY_TexteAlerte			Texte_Alerte;
}
EQTX_ALARMES_ALERTES_SYSTQTX;
		
typedef struct
{
	QTX_Validation			ChampValide;
	QTX_Validation			LSMaitreValide;
	XDY_Eqt				Numero;
	XDY_TypeEqt			TypeEqt;
   	XDY_NomST			NomST;
   	XDY_Mot				NoLSMaitre;
   	XDY_Mot				NoLSEsclave;
   	XDY_Rgs				AdresseRGS;
   	char				LibelleNomEqt[31];
   	int				SocketMaitre;
   	int				SocketEsclave;
	int				Etat_Service;
	int				code_systeme_gere;
	EQTX_ALARMES_ALERTES_SYSTQTX	*ptr_alarmes_alertes;
	XDY_District         SiteGestion;
}
EQTX_DONNEES_EQT_SYSTQTX;
		
typedef struct
{
	XDY_NomMachine			NomMachine;
	XDY_NomSite			NomSite;
	int				Mode_Fonct;
	EQTX_DONNEES_EQT_SYSTQTX	DonneesEqt[NB_EQT_MAX_TEQTX];
}
EQTX_DONNEES_SYSTQTX;
		
/* Declaration des prototypes des fonctions externes */
/* ------------------------------------------------- */


extern char *em_retourne_nom_fichier_envoi_qtx( int );
extern char *em_retourne_nom_fichier_historique_qtx( int, char *);
extern int em_lecture_repertoire( char *, int (*)(char *));

extern int lancement_timer_eqtx( int, char *);

#endif
