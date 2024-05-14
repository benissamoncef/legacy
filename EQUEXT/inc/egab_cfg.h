/*E*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18	creation GABEB 1.1
------------------------------------------------------ */

#ifndef egab_cfg
#define egab_cfg

/* fichiers inclus */
#include "xzst.h"
#include "xdy.h"
#include "xdc.h"
#include "xzao.h"
#include "xzao301.h"
#include "xzex.h"
#include "ex_mlcr.h"

/* definitions de constantes exportees */
#define C_PAS_BOUCHON	1
#define C_BOUCHON_BRETELLE	2
#define C_BOUCHON_BOUT_BRETELLE	3
#define C_BOUCHON_BRETELLE_SC	4

#define C_CAPTEUR_DEBUT_SENS1	1 
#define C_CAPTEUR_FIN_SENS1	3	/*arrivee de la bretelle sur la SC*/
#define C_CAPTEUR_DEBUT_SENS2	2
#define C_CAPTEUR_FIN_SENS2	4	 /*arrivee de la bretelle sur la SC*/

/* definitions de constantes exportees */

typedef struct Liste_Generique {
	void *element;
	struct Liste_Generique *suivant;
} T_Liste_Generique;

#define Liste_Generique_Suivant(p) \
 if (p!=NULL) p=p->suivant

#define Liste_Generique_Inserer_En_Tete(nouveau, tete) \
		nouveau->suivant = tete; tete = nouveau

typedef struct Bouchon {
        XDY_Entier numeroQ;
        XDY_Octet cleQ;
        XDY_Entier numeroT;
        XDY_Octet cleT;
        XDY_Autoroute autorouteQ;
        XDY_Autoroute autorouteT;
        XDY_PR prQ;
        XDY_PR prT;
        XDY_Octet ptCarQ;
        XDY_Octet ptCarT;
        XDY_Sens sensQ;
        XDY_Sens sensT;
	XDY_Entier numero_cause;
	XDY_Octet cle_cause;
} T_Bouchon;
typedef struct {
	XZAOT_CAP config;
	XDY_Octet	etat;
	XDY_Octet	Etat_Service;
	XDY_Octet	detection3;
	XDY_Octet	detection1;
	XDY_Octet	detection4;
	XDY_Octet	detection2;
	int                  Socket;	
	int                  DernAlerte;
} T_CAB;
#define EGABC_DETECTION_HS	0
#define EGABC_DETECTION		1
#define EGABC_NO_DETECTION	2

typedef struct {
	T_Bouchon bouchon;
	XDY_TexteAlerte alerte_envoyee;
	XDY_Entier sous_type_alerte_envoyee;
	XDY_Octet       reponse_operateur;
} T_Alerte;
typedef struct  {
	XZAOT301_Echangeur	echangeur;
	T_CAB	*cab;
	XDY_Octet       actif;
	XDY_Octet	detection_sud;
	XDY_Octet	detection_nord;
	XDY_Octet 	contexte;
	XDY_Booleen	creation_only;
	T_Bouchon *bouchon;
	T_Liste_Generique       *ListeAlertes;
} T_ECH;


typedef struct S_T_CAB {
	T_CAB		cab;
	struct S_T_CAB 	*suivant;
} T_Liste_CAB;

T_Liste_CAB *pg_CAB;

typedef struct S_T_ECH {
	T_ECH ech;
	struct S_T_ECH *suivant;
} T_Liste_ECH;


typedef struct MAILLON_LISTE_CHAINEE {
        T_Bouchon evt;
        struct MAILLON_LISTE_CHAINEE *suivant;
} T_LISTE_CHAINEE;

extern int                      vg_SockTIM;
extern XZEXT_MASK *            vg_SockMask ;
extern	int vg_SockTEMIP;
extern int vg_SockTEEIP;
extern T_INT4                  vg_SockRT;

T_Liste_Generique       *ListeAlertes;

/* definition de macro exportees */

/* declarations de variables globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  initialisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cfg_init();

/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR : 
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION 
*   Chargement de la liste des zones puis de la liste des PRV
*
------------------------------------------------------*/


#endif
