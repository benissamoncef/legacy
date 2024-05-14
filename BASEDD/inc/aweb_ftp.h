/* Fichier : $Id: aweb_ftp.h,v 1.5 2010/11/16 19:12:22 gesconf Exp $   Release : $Revision: 1.5 $   Date : $Date: 2010/11/16 19:12:22 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : asts_export.h			*
*************************************************
* DESCRIPTION :
* module externe de la tache serveur de donnees
*************************************************
* HISTORIQUE :
*
* JMG	20/10/04	AJOUT RDT POUR TDP ASF	1.2
* JMG	15/12/04	ajout parcours 1.3
* JMG	11/10/08	refonte tagen 1.4 DEM/836
* JMG	12/10/08	libelle zdp passe a 100 1.5 DEM/836
************************************************/

#ifndef EXPORT_WEB
#define EXPORT_WEB

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzia.h"
#include "aweb_export.h"

#include "xzas81.h"
#include "xzlg.h"

/* definition des constantes */

/* d≈finition des variables globales » l'application */
extern T_Type_FMC              tg_types_fmc[];

/* definition des variables locales au module */
struct T_Troncon{
        char		 	Identifiant[10];
	int			Niveau;
	int			Etat;
        char			NomVue[50];
	int			Rouge;
	int			Vert;
	int			Bleu;
	XDY_Octet		Station1;
	XDY_Octet		Station2;
	XDY_Octet		Station3;
	XDY_Octet		Station4;
	XDY_Octet		Station5;
	XDY_Autoroute		Autoroute;
	XDY_Sens		Sens;
	XDY_PR			Debut;
	XDY_PR			Fin;
        struct T_Troncon	*Suivant;
        };

struct T_Icone{
	char	Identifiant[10];
	int	Niveau;
	char	NomVue[100];
	int	X;
	int 	Y;
	int	FacteurX;
	int	FacteurY;
	int	Rotation;
	char	Icone[30];
	char	Texte[300];
	char    Horodate[30];
	int	Type;
	struct T_Icone *Suivant;
	};

/*TDPASF*/
struct T_Comptage{
	char    Identifiant[20];
	char	Horodate[50];
	int	Numero;
	XDY_Octet	Sens;
	int	Validite;
	int	Debit;
	int	Debit_PL;
	int	Vitesse;
	int	Taux;
	struct T_Comptage *Suivant;
};

struct T_Parcours {
	char	Identifiant[100];
	char	A_Diffuser;
	int	Temps;
	struct T_Parcours *Suivant;
};

struct T_ZDP {
	char	Libelle[100];
	XDY_Autoroute	Autoroute;
	XDY_PR	Debut;
	XDY_PR	Fin;
	XDY_Sens	Sens;
	XDY_Booleen	A_Calculer;
	struct T_ZDP *Suivant;
}; 
/* declaration de fonctions internes */

/* definition de fonctions externes */
int aweb_charge_zdp();
#endif
