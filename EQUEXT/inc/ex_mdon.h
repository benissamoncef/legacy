/*E*/
/* Fichier : $Id: ex_mdon.h,v 1.13 2018/02/13 18:00:09 devgfi Exp $      Release : $Revision: 1.13 $        Date : $Date: 2018/02/13 18:00:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mdon.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier des donnees communes au
*  Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	06 Sep 1994	: Creation
* Mismer.D	version 1.2	02 Nov 1994	:
* Volcic.F	version 1.3	19 Jan 1995	:
* Volcic.F	version 1.4	19 Jan 1995	:
* Volcic.F	version 1.5	19 Jan 1995	:
* Mismer.D	version 1.6	17 Mar 1995	:
* Fontaine.C	version 1.7	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.9	17 Aou 1995	: Modif ex_cp_heure_RADT
* Mismer.D	version 1.10	20 Oct 1995	: Ajout fonction ex_lire_serveur
* Mismer.D	version 1.11	08 Fev 1995	: Suppression fonction ex_presence_machine
* Mismer.D      version 1.12    12 Jun 1996     : Ajout fonction ex_lire_idpassw (DEM/1135)
* JPL		version 1.13	13/02/18	: Migration Linux 64 bits (DEM 1274) : EX_FONCT_INSERER: initialisation pointeur
------------------------------------------------------ */

#ifndef ex_mdon
#define ex_mdon

/* fichiers inclus */
#include <sys/types.h>
#include <sys/un.h>
#include <sys/time.h>

#include "xdc.h"
#include "xdy.h"
#include "xdf.h"
#include "xzst.h"
#include "xzss.h"
#include "xzex.h"
#include "xzec.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Insere dans une liste de structure de donnees, une nouvelle structure.
* Les structures de donnees sont classees dans l'ordre du numero d'equipement.
* Si la taille en argument d'entree est nulle : 
*   - mise a jour complete de la structure
* Sinon : mise a jour uniquement de la config ( pas l'etat )
* 
*------------------------------------------------------
* SEQUENCE D'APPEL :
*/
#define EX_DECLA_INSERER(STRUCT_DONNEES,NOM_FONCT_INSERER)	\
extern int    NOM_FONCT_INSERER  (	STRUCT_DONNEES *pa_Enr, int va_Taille );

#define EX_FONCT_INSERER(STRUCT_DONNEES,NOM_FONCT_INSERER,DEBUT,ORDRE,SUIV,CONFIG)	\
int    NOM_FONCT_INSERER  (	STRUCT_DONNEES *pa_Enr, int va_Taille )\
{\
	STRUCT_DONNEES *pl_NewEnr, *pl_Enr, *pl_ExEnr;\
	int	vl_existe = XDC_NOK;\
	void *	vl_PtrSuiv = NULL;\
   for ( pl_Enr = pl_ExEnr = DEBUT;\
         pl_Enr != NULL;\
         pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->SUIV )\
   {\
       if ( pa_Enr->ORDRE < pl_Enr->ORDRE )\
          break;\
       if ( pa_Enr->ORDRE == pl_Enr->ORDRE )\
       {\
          vl_existe = XDC_OK;\
          vl_PtrSuiv = pl_Enr->SUIV;\
          break;\
       }\
   }\
   if ( vl_existe == XDC_OK )\
      pl_NewEnr = pl_Enr;\
   else\
      if ( ( pl_NewEnr = (STRUCT_DONNEES *) malloc ( sizeof(STRUCT_DONNEES) ) ) == NULL)\
         return ( XDC_NOK );\
      else\
         memset ( pl_NewEnr, 0, sizeof(STRUCT_DONNEES) );\
   if ( va_Taille != 0 )\
      memcpy ( &pl_NewEnr->CONFIG, &pa_Enr->CONFIG, va_Taille );\
   else\
   {\
      memcpy ( pl_NewEnr, pa_Enr, sizeof(STRUCT_DONNEES) );\
      pl_NewEnr->SUIV = vl_PtrSuiv;\
   }\
   if ( vl_existe != XDC_OK )\
   {\
      if ( pl_Enr == DEBUT )\
      {\
          pl_NewEnr->SUIV = DEBUT;\
          DEBUT           = pl_NewEnr;\
      }\
      else\
      {\
         pl_NewEnr->SUIV = pl_ExEnr->SUIV;\
         pl_ExEnr->SUIV  = pl_NewEnr;\
      }\
   }\
   return ( XDC_OK );\
} 






/******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Dans une liste de structure de donnees, detruit une structure 
* donnee selon le numero d'equipement.
*
******************************************************
* SEQUENCE D'APPEL :
*/
#define EX_DECLA_SUPPRIMER(NOM_FONCT_SUPPRIMER)	\
extern void    NOM_FONCT_SUPPRIMER  (	int va_NumEqt );

#define EX_FONCT_SUPPRIMER(STRUCT_DONNEES,NOM_FONCT_SUPPRIMER,DEBUT,ORDRE,SUIV)	\
void    NOM_FONCT_SUPPRIMER  (	int va_NumEqt )\
{\
STRUCT_DONNEES *pl_EltsSup, *pl_ExSup;\
   for ( pl_ExSup = pl_EltsSup = DEBUT;\
         pl_EltsSup != NULL;\
         pl_EltsSup = pl_EltsSup->SUIV )\
   {\
      if ( pl_EltsSup->ORDRE == va_NumEqt )\
      {\
         if ( ( pl_ExSup == DEBUT ) && ( pl_ExSup == pl_EltsSup ) )\
         {\
	    DEBUT = pl_EltsSup->SUIV;\
         }\
         else\
         {\
	    pl_ExSup->SUIV = pl_EltsSup->SUIV;\
         }\
         free(pl_EltsSup);\
         break;\
      }\
      else\
         pl_ExSup = pl_EltsSup;\
   }\
}


#define EX_ATTEINDRE_ELT(ELTS,NUM_EQT,DEBUT,ORDRE,SUIV)	\
   for ( ELTS = DEBUT;\
         ELTS != NULL;\
         ELTS = ELTS->SUIV )\
   {\
      if ( ELTS->ORDRE == NUM_EQT )\
         break;\
   }
   




/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appel≈e pour lire une ligne dans un fichier ASCII
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ex_LireLigneFich ( 	int 	va_Fd,
					char	*pa_Ligne,
					int	va_NbCar );


/*
* ARGUMENTS EN ENTREE :
*  va_Fd		File descripteur du fichier
*  va_NbCar		Taille maxi de la ligne
*
* ARGUMENTS EN SORTIE :
*  pa_Ligne		Contenu de la ligne lu
*
* CODE RETOUR : 
*  int			Nombre caratere retourner
*  XZEXC_EOF		Fin du fichier atteindre
*  XDC_NOK		echec lecture ligne
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction retournant le nom de l'equipement correspondant
*  au type d'equipement fournit en entree
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_nom_eqt ( int	va_TypeEqt , char	*pa_NomEqt );

/*
* ARGUMENTS EN ENTREE :
*  va_TypeEqt		: Type d'equipement
*
*
* ARGUMENTS EN SORTIE :
*  paNomEqt		: Nom de l'equipement
*
*
* CODE RETOUR : 
*  XDC_OK		: Succes
*  XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   
*
------------------------------------------------------*/

#endif



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Lignes_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_lire_suppleant	( XDY_NomMachine *pa_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* XDY_NomMachine *pa_NomMachine
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/


extern int ex_lire_serveur	( XDY_NomMachine * );
extern int ex_lire_idpassw      ( XDY_NomMachine, char * );
extern int ex_lire_cligno	( XDY_NomMachine, char * , char * );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ex_cp_heure_RADT ( 	XDY_NomMachine, XDY_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine destinatrice
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Test si une machine est accessible sur le reseau
*
------------------------------------------------------*/
