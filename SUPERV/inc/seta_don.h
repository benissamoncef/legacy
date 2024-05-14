/*E*/
/* Fichier : $Id: seta_don.h,v 1.3 1994/10/24 17:19:38 milleville Exp $	$Revision: 1.3 $        Date : $Date: 1994/10/24 17:19:38 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_DON * FICHIER seta_don.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier SETA_DON.h >
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  14 Oct 1994	: Creation
*******************************************************/
#ifndef SETA_DON
#define SETA_DON


/* fichiers inclus */
#include "xzsc.h"
#include "xzst.h"


/* definitions de constantes */
#define SET_PRESENCE 0
#define SET_ABSENCE_COURTE 1
#define SET_ABSENCE_LONGUE 2

/* definitions des types globaux */

   /* etat d'un sous systeme */
typedef struct {
                  XZSCT_NomMachine  NomMachine;
                  XZSCT_NomSSysteme NomSSysteme;
		  int               Etat;
               } SET_ETAT_SSYS;

   /* etat d'une tache */
typedef struct {
                  XZSCT_NomMachine  NomMachine;
                  XZSCT_NomTache    NomTache;
                  XZSTT_NiveauTrace NivTrace;
                  int               VisuTrace;
                  int               Etat;
               } SET_ETAT_TACH;

   /* etat d'une machine */
typedef struct {
                  XZSCT_NomMachine  NomMachine;
		  XZSCT_NomSite     NomSite;
		  int               TypeMachine;
                  int               Etat;
                  int               EtatModeIns;
                  int               EtatModeRec;
		  int               TypeSurveillance;
		  long		    HeureVie;
		  int 		    NbAbsenceVie;
		  int		    EtatAbsence;
               } SET_ETAT_MACH;

   /* etat d'une entite */
typedef struct {
                  XZSCT_NomEntite   NomEntite;
                  int               TypeEntite;
                  int               Etat;
               } SET_ETAT_ENTI;

   /* surveiilance d'un serveur */
typedef struct {
                  XZSCT_NomMachine  NomMachine;
                  int               NbClient;
               } SET_ETAT_SERV;

#ifndef SETA_DON_c

/* definition des variables globales */

extern XZSCT_NomSite    SEP_NOM_SITE;        /* nom du site de la machine locale */
extern XZSCT_NomMachine SEP_NOM_MACHINE;     /* nom de la machine locale */
extern int              SEV_TYPE_MACHINE;    /* type de la machine locale */
extern XZSCT_NomMachine SEP_MACHINE_SECOURS; /* nom de la machine de secours */
extern XZSCT_NomTache   SEP_NOM_TACHE;       /* nom de la tache */
extern XZSCT_NomMachine SEP_NOM_SERVEUR1, SEP_NOM_SERVEUR2;


   /* pointeurs sur les tableaux d'etat des elements a surveiller */
extern SET_ETAT_SSYS *SEP_ETAT_SSYS; /* etat sous systeme */
extern SET_ETAT_TACH *SEP_ETAT_TACH; /* etat tache */
extern SET_ETAT_MACH *SEP_ETAT_MACH; /* etat machine */
extern SET_ETAT_ENTI *SEP_ETAT_ENTI; /* etat entite */

   /* nombre d'elements a surveiller par la tache */
extern int SEV_NB_MACH; /* nombre de machines */
extern int SEV_NB_SSYS; /* nombre de sous-systemes */
extern int SEV_NB_TACH; /* nombre de taches */
extern int SEV_NB_ENTI; /* nombre d'entites */

	/* Flag de Redondance Interne */
extern int REDONDANCE_INTERNE;


/* definitions des fonction externes */

int seta_rechmach(SET_ETAT_MACH *pa_EtatMachine,
		int va_TailleTab,
	        XZSCT_NomMachine pa_NomMachine,
		int *pa_Indice);

int seta_rechssys(SET_ETAT_SSYS *pa_EtatSSysteme,
	        int va_TailleTab,
	        XZSCT_NomMachine pa_NomMachine,
	        XZSCT_NomSSysteme pa_NomSSysteme,
	        int *pa_Indice);

int seta_rechtach(SET_ETAT_TACH *pa_EtatTache,
	        int va_TailleTab,
	        XZSCT_NomMachine pa_NomMachine,
	        XZSCT_NomTache pa_NomTache,
	        int *pa_Indice);

int seta_rechenti(SET_ETAT_ENTI *pa_EtatEntite,
    	        int va_TailleTab,
	        XZSCT_NomEntite pa_NomEntite,
	        int va_TypeEntite,
	        int *pa_Indice);


#endif


#endif

