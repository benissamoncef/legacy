/*E*/
/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $      Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSXDON * FICHIER xzsx.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSCFINI
*        - Declarations externes des fonctions
*        - Declarations externes des variables
*        - Declarations des constantes
*        - Declarations des types
*
*   Reference DCG-021 chapitre 3.7  
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville	24 Aou 1994      	: Creation
*
*  T.Milleville	11 Jan 1995      	: Ajout du Champ TypeFichier
*					dans la structure XZSXT_VersLog
*						V 1.5
*  T.Milleville	11 Jan 1995      	: Ajout du Champ TypeFichier
*					dans la structure XZSXT_VersLog
*
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Suppression declaration de fonction interne  1.9
*******************************************************/

#ifndef xzsx
#define xzsx

#include <xdf.h>
#include <xdy.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h> 

/* definitions de constantes exportees */
#define XZSXC_NOM_INV 1
#define XZSXC_FIC_INV 2

#define XZSXC_ENCODAGE			0
#define XZSXC_DECODAGE			1
#define XZSXC_COPIE			2

#define XZSXC_COMMENTAIRE		0
#define XZSXC_INFOMACHINE		1
#define XZSXC_INFOTACHE			2
#define XZSXC_INFOSITE			3
#define XZSXC_MACHINE			4
#define XZSXC_TACHE			5
#define XZSXC_SS			6
#define XZSXC_ENTITE			7
#define	XZSXC_JOURNAL			8
#define XZSXC_ACTION			9
#define XZSXC_TIMER			10

#define XZSXC_LONG 			255

#define XZSXC_FORMATENTETEJOURNAL		"%8ld %8ld\n"
#define XZSXC_TAILLEENTETEJOURNAL		18	


/* definitions de types exportes */


typedef struct 
{
  	XZSCT_NomMachine NomMachine;
        XZSCT_NomPeriph  NomPeriphSR;
	XZSCT_NomMachine NomMachineImp;
	XZSCT_NomPeriph  NomImprimante;
	XZSCT_NomPeriph  NomImprimanteSec;
	XZSCT_NomSite    NomSite;
	char             PathTrace[80];
	int              MaxTailleFicTrace;
	int              PeriodeTestReseau;
	int              TypeMachine;
	int              Priorite;
	XZSCT_TypeProfil TypeProfil;
	int              DureeAbsenceCourte;

} XZSXT_Machine;

typedef struct 
{
	XZSCT_NomTache NomTache;
	char           ListeServer[256];
	char           ListeLicense[256];
	int            TailleFicTrace;

} XZSXT_Tache;

typedef struct 
{
	unsigned char		Type ;
	void *			Data ;
	void *			Suivant ;

} XZSXT_Entite ;

typedef struct 
{
	char			NomFichierListe[20];
	int			NbMax;
	int			NbReel;

} XZSXT_ListeNombre;

typedef struct 
{
	XZSCT_NomSite		NomSite;
 
} XZSXT_ListeSite ;

	/*XZSCT_NomMachine	NomMachineTra;*/
typedef struct 
{
	XZSCT_NomMachine	NomMachine;
	int			TypeMachine;
	XZSCT_NomMachine        NomMachineImp;
	XZSCT_NomSite		NomSite;

} XZSXT_ListeMachine ;

typedef struct 
{
	XZSCT_NomSSysteme	NomSSysteme;
	int			TypeMachine; 

} XZSXT_ListeSSysteme ;

typedef struct
{
	XZSCT_NomTache       	NomTache;
	XZSCT_NomSSysteme	NomSSysteme;
	int                     TypeMachine;
	int                     Temporaire;

} XZSXT_ListeTache ;

typedef struct
{
	XZSCT_NomEntite		NomEntite;
	int			TypeEntite;
	XZSCT_NomSite		NomSite;

} XZSXT_ListeEntite;

/*typedef struct
{
	XZSCT_NomMachine        NomMachine;
	XZSCT_NomTache          NomTache;
	XZSCT_NumVersion        NumVersion;
	XZSMT_Horodate		Horodate;

} XZSXT_VersLog;*/

typedef struct
{
	long			TailleMax;
	long			NbOctetEcrit;

} XZSXT_EnteteJourAdm;

typedef struct
{
	long			TailleMax;

} XZSXT_TailleJourAdm;

typedef struct
{
	int			NumTexte;
	int			IndDiff;
	int			IndBasc;

} XZSXT_ActionMessage;

typedef struct
{
        double          Horodate ;
        int             Delai,
                        IdMsg ;
        XZSCT_Datagroup Dg ;
        XZSMT_CleProg   Cle;
        int             Secur;

} XZSXT_ListeTimer;

/* declaration des macros fonctions externes */

#define XZSXC_Decode_DesMachine_Ok	12

#define XZSXM_Decode_DesMachine(p,t)\
sscanf((p)\
      ,"%s %s %s %s %s %s %s %u %u %u %u %u %u"\
      ,(t).NomMachine\
      ,(t).NomPeriphSR\
      ,(t).NomMachineImp\
      ,(t).NomImprimante\
      ,&((t).NomImprimanteSec)\
      ,(t).NomSite\
      ,(t).PathTrace\
      ,&((t).MaxTailleFicTrace)\
      ,&((t).PeriodeTestReseau)\
      ,&((t).TypeMachine)\
      ,&((t).Priorite)\
      ,&((t).TypeProfil)\
      ,&((t).DureeAbsenceCourte)\
      )

#define XZSXC_Decode_DesTache_Ok	4

#define XZSXM_Decode_DesTache(p,t)\
sscanf((p)\
      ,"%s %s %s %u"\
      ,(t).NomTache\
      ,(t).ListeServer\
      ,(t).ListeLicense\
      ,&((t).TailleFicTrace)\
      )

#define XZSX_16Jouradm_Lecture(f,s)	lseek((f),0,SEEK_SET);read((f),(s),sizeof(*(s)))
#define XZSX_17Jouradm_Ecriture(f,s)	lseek((f),0,SEEK_SET);write((f),(s),sizeof(*(s)))


/* declaration des donnees globales */

/* declaration des fonctions externes */



/* comme son nom l'indique ... appelee par xzsa_actualiseAdmin */
extern int XZSX_RecupereGrappe (XZSXT_Entite** pa_grappe);



/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSXDON  
******************************************************
* ALGORITHME :
*
* Arguments
* pa_NomMachine : nom de la machine 
* pa_PathFichier: PathName du fichier NSUP_NomMachine
* pa_Machine    : adresse de la structure decrivant le
*                 noyau de supervision de la machine
*
* Fonction
* Retourne les caracteristiques du noyau de supervision
* d'une machine contenues dans le fichier de configuration
* NSUP_NomMachine.
* 
* Codes retour
* 
* XZSXC_NOM_INV Nom machine invalide (fichier NSUP_NomMachine 
*               non trouve)
* XZSXC_FIC_INV Erreur lecture fichier NSUP_NomMachine
* XDC_OK	ok
*
* Mecanismes
*
******************************************************/

extern int XZSX_01MachineLecture(XZSCT_NomMachine pa_NomMachine,
		                 char *pa_PathFichier,
		                 XZSXT_Machine *pa_Machine);




/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSXDON  
******************************************************
* ALGORITHME :
*
* Arguments
* pa_NomMachine : nom de la machine 
* pa_NomTache   : nom de la tache
* pa_PathFichier: PathName du fichier NSUP_NomMachine
* pa_Machine    : adresse de la structure contenant les
*                 carcacteristiques de la tache
*
* Fonction
* Retourne les caracteristiques d'une tache d'une machine 
* contenues dans le fichier de configuration NSUP_NomMachine.
* 
* Codes retour
* 
* XZSXC_NOM_INV Nom machine invalide (fichier NSUP_NomMachine 
*               non trouve)
* XZSXC_FIC_INV Erreur lecture fichier NSUP_NomMachine
* XDC_OK	ok
*
* Mecanismes
*
******************************************************/

extern int XZSX_02TacheLecture(XZSCT_NomMachine pa_NomMachine,
                 	       XZSCT_NomTache pa_NomTache,
		               char *pa_PathFichier,
		               XZSXT_Tache *pa_Tache);

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	creation du fichier de config contenant les
*	informations du noyay de supervision
*
******************************************************
* SEQUENCE D'APPEL :
*/
extern

int			XZSX_03Machine_Ecriture(
char *					       pa_Repertoire,
XZSXT_Machine				       va_Machine
					       )

;
/*
* PARAMETRES EN ENTREE :
*
*	va_Machine : Description du noyau de supervision
*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Ecriture des caracteristiques d'une tache 
*	dans un fichier donne
*
******************************************************
* SEQUENCE D'APPEL :
*/
extern

int			XZSX_04Tache_Ecriture(
XZSCT_NomMachine			     va_NomMachine,
XZSXT_Tache				     va_Tache,
char *					     pa_Repertoire
					     )

;
/*
* PARAMETRES EN ENTREE :
*
*	ca_NomMachine		: Nom de la machine 
*	va_tache		: Caracteristiques de la tache
*	pa_Repertoire		: Repertoire du fichier
*
* VALEUR DE RETOUR :
*
*	XDX_OK
*	XDC_NOK
*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Destruction des caracterisitiques d'une tache 
*	dans un fichier donne
*
******************************************************
* SEQUENCE D'APPEL :
*/
extern

int			XZSX_05Tache_Detruire(
XZSCT_NomMachine			     va_NomMachine,
XZSCT_NomTache				     va_NomTache,
char *					     pa_Repertoire
          				     )

;
/*
* PARAMETRES EN ENTREE :
*
*	va_NomMachine	: Nom de la machine
*	va_NomTache	: Nom de la tache a detruire
*	pa_Repertoire	: Repertoire du fichier cible
*
* PARAMETRES EN SORTIE :
*
*	XDC_OK
*	XDC_NOK
*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Destruction des caracterisitiques d'une tache 
*	dans un fichier donne
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_05Tache_Detruire(
XZSCT_NomMachine			     va_NomMachine,
XZSCT_NomTache				     va_NomTache,
char *					     pa_Repertoire
          				     )
;


/*
* PARAMETRES EN ENTREE :
*
*	va_NomMachine	: Nom de la machine
*	va_NomTache	: Nom de la tache a detruire
*	pa_Repertoire	: Repertoire du fichier cible
*
* PARAMETRES EN SORTIE :
*
*	XDC_OK
*	XDC_NOK
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_06ExisteSite(
XZSCT_NomSite			      va_NomSite,
char *				      pa_Repertoire
				      )
				      ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_07Cmachine(
XZSXT_Machine			       va_Machine,
char *				       pa_Repertoire
				       )
				       ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_08Dmachine(
XZSCT_NomMachine		       va_NomMachine,
char *				       pa_Repertoire
				       )
				       ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_09Cssysteme(
XZSXT_ListeSSysteme			va_SSysteme,
char *				        pa_Repertoire
					)
					;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_09Cssysteme(
XZSXT_ListeSSysteme			va_SSysteme,
char *				        pa_Repertoire
					)
					;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_10Dssysteme(
XZSXT_ListeSSysteme			va_SSysteme,
char *					pa_Repertoire
					)
					;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_11ExisteSSysteme(
XZSCT_NomSSysteme			  va_NomSSysteme,
int					  va_TypeMachine,
char *					  pa_Repertoire
					  )
					  ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int	XZSX_12Ctache(
			XZSCT_NomSSysteme	va_NomSSysteme,
			int		        va_TypeMachine,
			XZSCT_NomTache		va_NomTache,
			int                     va_Temporaire,
			char *			pa_Repertoire) ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE MODULE
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int	XZSX_13Dtache(
			XZSCT_NomSSysteme	va_NomSSysteme,
			int		     	va_TypeMachine,
			XZSCT_NomTache		va_NomTache,
			int                     va_Temporaire,
			char *			pa_Repertoire) ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*
* CONDITION D'ERREUR :
*
*
* CONDITIONS LIMITES :
*
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_14Centite(
XZSCT_NomEntite			      va_NomEntite,
int				      va_TypeEntite,
XZSCT_NomSite			      va_NomSite,
char *				      pa_Repertoire
          			      )
				      ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_15Dentite(
XZSCT_NomEntite			      va_NomEntite,
int				      va_TypeEntite,
char *				      pa_Repertoire
				      )
				      ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
*
* CONDITIONS LIMITES :
*
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_19NbEntite(
int				       va_Type,
char *				       pa_Repertoire
				       )
				       ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
* Suppression d'un message dans le fichier de config
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int XZSX_24Message_Detruire(
			  int    va_NumTexte,
			  char * pa_Repertoire
          );

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* < commentaire decrivant les differentes valeurs prises
* par le code d'erreur et leur signification >
*
* CONDITIONS LIMITES :
*
* < commentaire precisant le comportement de la fonction
* en cas de fourniture de donnees hors du domaine de
* validite prevu >
*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE MODULE
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Desctruction d'un site dans le fichier de
*	config site.
*
******************************************************
* SEQUENCE D'APPEL :
*/
extern

int			XZSX_26Dsite(
XZSCT_NomSite			     va_Site,
char *				     pa_Repertoire
				     )

;
/*
* PARAMETRES EN ENTREE :
*
* va_Site	: Nom du site a detruire
* pa_Repertoire : Repertoire du fichier cible
*
* VALEUR(S) DE RETOUR : 
*
*	XDC_OK
*	XDC_NOK
*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Mise a jour d'un site dans le fichier de config
* site.
*
******************************************************
* SEQUENCE D'APPEL :
*/
extern

int			XZSX_25Csite(
XZSCT_NomSite			     va_Site,
char *				     pa_Repertoire
				     )

;
/*
* PARAMETRES EN ENTREE :
*
*	va_Site		: Nom du site 
*	pa_Repertoire	: Repertoire du fichier a creer 
*
* VALEUR(S) DE RETOUR : 
*
*	XDC_OK
*	XDC_NOK
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern    int		XZSX_LitFichier(
char *			       pa_NomFichier
			       )
			       ;

/*
* PARAMETRES EN ENTREE :
*
* pa_NomFichier : nom du fichier de configuration avec son PATH

* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* XDC_OK 
* XDC_NOK : Erreur lecture fichier   
*
* CONDITIONS LIMITES :
*
* La fonction initialise un liste chaine en memoire globale
* dont chaque element pointe sur un structure renseignee selon 
* le type d'enregistrement du fichier de configuration
* La zone memoire alouee est pointee par pg_NSUP
* La liberation de la zone memoire est effectuee par la fonction 
* OublieFichier()
*
******************************************************/


/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* La fonction libere la zone memoire alouee par la 
* fonction XZSX_OublieFichier. 
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern 	int		XZSX_OublieFichier(void);

/*
******************************************************/

/* Fichier : $Date: 2018/02/07 19:34:21 $	      Release : $Revision: 1.9 $        Date : $Date: 2018/02/07 19:34:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Test si le nom de machine existe
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern int			XZSX_27ExisteMachine(
XZSCT_NomMachine		      va_NomMachine,
char *				      pa_Repertoire
				      )
				      ;

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* XDC_OK Si le nom de Machine existe
* XDC_NOK sinon
*
* CONDITIONS LIMITES :
*
*
******************************************************/



int   XZSX_29JournalLecture (long           *pa_Taillemax,
                             char          *pa_PathFichier);


/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* XDC_OK 
* XDC_NOK
*
* CONDITIONS LIMITES :
*
*
******************************************************/



int   XZSX_30JournalEcriture(long           va_Taillemax,
                             char          *pa_Repertoire);

#endif
