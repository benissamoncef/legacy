/*E*/
/*Fichier :  @(#)xzao561.h	1.3      Release : 1.3      Date : 20/02/12
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao561.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	20/03/12	: creation (DEM/1016)
------------------------------------------------------*/

#ifndef xzao561
#define xzao561

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO561_RPC_NAME "XZAO561"

#define XZAOC_XZAO561_NB_PARM_RETURN 1 

/* definitions de types exportes */

typedef struct {

	XDY_Eqt Numero;
	char	Libelle[80];
	XDY_Eqt Type_horaire_trafic;
	XDY_Eqt Type_condition_horaire;
	XDY_Horodate Horodate_debut;
	XDY_Horodate Horodate_fin;
	XDY_Eqt Heure_debut;
	XDY_Eqt Minute_debut;
	XDY_Eqt Heure_fin;
	XDY_Eqt Minute_fin;
	XDY_Eqt Cond_alternative;
	XDY_Entier Seuil;
	XDY_Eqt Donnee_trafic;
	XDY_Octet Operateur;
	XDY_Eqt Scenario;
	XDY_Eqt Conjonction1;
	XDY_Eqt Conjonction2;
	XDY_Eqt Conjonction3;
	XDY_Eqt Conjonction4;
	XDY_Station Station;
	XDY_Sens	Sens;
	XDY_Eqt Type_jour;
	XDY_Octet	Condition_simple;
	XDY_Eqt Zone;
} XZAOT_Condition_Scenarios;


/* delaration de fonctions */
extern int XZAO561_Conditions_Scenarios(XDY_Basedd,
					XDY_Entier ,
					XDY_Entier,
					 XDY_FonctionInt ,
					 XDY_Entier*
					);



#endif
