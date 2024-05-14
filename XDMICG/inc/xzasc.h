/*E Fichier : $Id: xzasc.h,v 1.3 2001/10/30 18:30:32 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2001/10/30 18:30:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME _SS_SYSTEME_
------------------------------------------------------
* MODULE XZAE * FICHIER xzasc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
* C.T.  27/10/94	: Creation
* PN	11/08/99	: ajout constante lib sens RDS v1.2 dem/1767
------------------------------------------------------ */


/* fichiers inclus */

/* definitions de constantes exportees */

/* code d'erreur pour les mesures de la courbe f(t) */

#define XZASC_RADT_PAS_MES	3	/* pas de mesure pour le RADT */

#define XZASC_LIB_P_1	"Aix -> Italie"
#define XZASC_LIB_P_2	"A8 -> Monaco"
#define XZASC_LIB_P_3	"Marseille -> Toulon"
#define XZASC_LIB_P_4	"Marseille -> A52"
#define XZASC_LIB_P_5	"Aix -> Les Alpes"
#define XZASC_LIB_P_6	"Aix -> Aubagne"
#define XZASC_LIB_P_7	"A52 -> Auriol"
#define XZASC_LIB_P_8	"Le Luc -> Toulon"

#define XZASC_LIB_N_1	"Italie -> Aix"
#define XZASC_LIB_N_2	"Monaco -> A8"
#define XZASC_LIB_N_3	"Toulon -> Marseille"
#define XZASC_LIB_N_4	"A52 -> Marseille"
#define XZASC_LIB_N_5	"Les Alpes -> Aix"
#define XZASC_LIB_N_6	"Aubagne -> Aix"
#define XZASC_LIB_N_7	"Auriol -> A52"
#define XZASC_LIB_N_8	"Toulon -> Le Luc"
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

