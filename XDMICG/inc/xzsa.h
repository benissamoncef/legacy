/*E*/
/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $	      Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN  * FICHIER xzsa.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   - Diffusion modification dynamique administration
*     systeme
*   - Consultation de l'administration systeme
*
* Reference DCG-021 chapitre 3.1
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  13 Sep 1994      : Creation
*
*  T.Milleville  29 Dec 1994      : Ajout de la fonction
*					XZSA_29ListeEntiteSite V 1.5
*  
*  T.Milleville  6 Feb 1994      : Ajout de la constante
*					XZSAC_TYPE_TPRE	 	V 1.6
*  T.Milleville  9 Avr 1995      : Ajout de la constante
*					XZSAC_TYPE_TSHE	V
*  T.Milleville  2 Mai 1995      : Ajout d'un module d'interface 
*	XZSA_15TacheTempMachine V1.8
*  T.Milleville  6 Oct 1995      : Ajout constante pour preciser
*	le type de l'imprimante + fonction pour rechercher le type
*	de l'entite XZSA_26TypeEntite V1.10
*  B. Noel	19 Fev 1996	: Ajout de marqueurs pour filtre Applix (DEM91)		V1.11
****
*  P. Niepceron 22 Aou 1996	: Ajout du type de Machine PC simplifie v1.12
*  P. Niepceron 16 Dec 1997	: Ajout du type de machine PC 2		v1.13
*  P. Niepceron 22 Dec 1997	: Ajout du type de machine STA		v1.14
*  P. Niepceron 01 Jul 1998	: Ajout XZSAC_TYPEM_HIS			v1.15
*  JPL		30/11/10	: Nom correct de XZSA06			1.18
*******************************************************/

/******************************************************************************
**                                                                           **
** Ce fichier definit des constantes communes a tous les sous-systemes,      **
** y compris le sous-systeme IHM developpe avec Applix.                      **
** Des commentaires de la forme "Non Applix - debut" et "Non Applix - fin"   **
** delimitent par la suite les zones dont la syntaxe n'est pas reconnue par  **
** l'interpreteur Applix de maniere a pouvoir les filtrer simplement.        **
**                                                                           **
******************************************************************************/

/* Non Applix - debut */

#ifndef xzsa
#define xzsa

/* fichiers inclus */
#include <xdf.h>
#include <xzsx.h>
#include <xzsc.h> 


/*definitions de constantes exportees */

/* codes d'erreur */
#define XZSAC_NOM_INV	1        		/* nom machine invalide */
#define XZSAC_NOS_INV	2		        /* nom sous-systeme invalide */
#define XZSAC_NOT_INV	3    			/* nom tache invalide */
#define XZSAC_NOA_INV	4    			/* nom site invalide */
#define XZSAC_NOE_INV	5    			/* nom entite invalide */
#define XZSAC_FIC_INV	6    			/* operations sur fichier invalide */
#define XZSAC_INI_INV	7    			/* initialisation incorrecte */
#define XZSAC_TYP_INV	8    			/* type de la machine */

/* Types de machine */
#define XZSAC_TYPEM_ST	XZSCC_TYPEM_ST  	/* Station de Travail */
#define XZSAC_TYPEM_SD	XZSCC_TYPEM_SD  	/* Serveur de Donnees */
#define XZSAC_TYPEM_SC	XZSCC_TYPEM_SC  	/* Serveur de Communication */
#define XZSAC_TYPEM_LT	XZSCC_TYPEM_LT  	/* Local Technique */
#define XZSAC_TYPEM_SP	XZSCC_TYPEM_SP  	/* Serveur de Protocols */
#define XZSAC_TYPEM_PCS XZSCC_TYPEM_PCS		/* Poste Operateur PC simplifié */
#define XZSAC_TYPEM_PC2  XZSCC_TYPEM_PC2	/* Serveur Niveau 2 */
#define XZSAC_TYPEM_PC2LT XZSCC_TYPEM_PC2LT	/* Serveur Niveau 2 sans LT */
#define XZSAC_TYPEM_STA XZSCC_TYPEM_STA		/* Station de Travail d'analyse */
#define XZSAC_TYPEM_HIS XZSCC_TYPEM_HIS		/* Serveur Historique */
#define XZSAC_NB_TYPEM	10  			/*  Nombre de types de machine */

/* Types d'entite */
#define XZSAC_TYPE_PERI	1    			/* peripherique */
#define XZSAC_TYPE_INTE	2			/* interface */
#define XZSAC_TYPE_TTMP	3    			/* tache temporaire */
#define XZSAC_TYPE_TPER	4    			/* tache Permanante */
#define XZSAC_TYPE_TPRE	5    			/* tache Presente dans le 
						*  fichier NSUP_ADMINIS et pas
						*  dans le NUP_NomMachine */
#define XZSAC_TYPE_TSHE	6    			/* Shell Script */

/* Non Applix - fin */

#define XZSAC_TYPE_IMP_NB	10    		/* Imprimante NOIR et BLANC */
#define XZSAC_TYPE_IMP_CL	11    		/* Imprimante COULEUR */

/* Non Applix - debut */

/* definitions de types exportes */

typedef struct S_XZSAT_ListeMachine
{
  XZSXT_ListeMachine*	machine;
  struct S_XZSAT_ListeMachine*	suivant;
} XZSAT_ListeMachine;

typedef struct S_XZSAT_ListeTache
{
  XZSXT_ListeTache*	tache;
  struct S_XZSAT_ListeTache*	suivant;
} XZSAT_ListeTache;

typedef struct S_XZSAT_ListeSSysteme
{
  XZSXT_ListeSSysteme*	ssysteme;
  struct S_XZSAT_ListeSSysteme*	suivant;
} XZSAT_ListeSSysteme;

typedef struct S_XZSAT_ListeSite
{
  XZSXT_ListeSite*	site;
  struct S_XZSAT_ListeSite*	suivant;
} XZSAT_ListeSite;

typedef struct S_XZSAT_ListeEntite
{
  XZSXT_ListeEntite*	entite;
  struct S_XZSAT_ListeEntite*	suivant;
} XZSAT_ListeEntite;




/* declaration des donnees globales */
/* declaration des fonctions externes */
extern int XZSA_ActualiseMachine (XZSXT_Entite*);
extern int XZSA_ActualiseSite (XZSXT_Entite*);
extern int XZSA_ActualiseSSysteme (XZSXT_Entite*);
extern int XZSA_ActualiseTache (XZSXT_Entite*);
extern int XZSA_ActualiseEntite (XZSXT_Entite*);

extern int XZSA_ActualiserAdmin (T_IPC_CONN			va_Cnx,
				 T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
				 T_CB_ARG			pa_Arg);

/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSA_01ListeMachine(pa_FonctionUtil, pa_NbElem) 
*
* Arguments
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK      ok.
* XZSAC_INI_INV	Erreur initialisation administration
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_01ListeMachine(int 	(*pa_FonctionUtil)(),
			       int 	*pa_NbElem,
			       void     *pa_Args);

/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSA_02ListeSSysteme(va_NomMachine,
*		       pa_FonctionUtil, 
*		       pa_NbElem)
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les sous-systemes
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_02ListeSSysteme(XZSCT_NomMachine 	va_NomMachine,
				void 			(*pa_FonctionUtil)(),
				int 			*pa_NbElem,
				void                    *pa_Args);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_03ListeTacheMachine(va_NomMachine,
*			   pa_FonctionUtil,
*			   pa_NbElem) 
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les taches
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_03ListeTacheMachine(XZSCT_NomMachine 	va_NomMachine,
				    void 		(*pa_FonctionUtil)(),
			            int 		*pa_NbElem,
				    void                *pa_Args); 

/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_04ListeTacheMachineSSyst(va_NomMachine, 
*				va_NomSSysteme,
*			        pa_FonctionUtil, 
*				pa_NbElem)
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les taches
* NomSSysteme	: Nom du sous-systeme dont on veut 
		  connaitre les taches
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_NOS_INV Nom de sous-systeme inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
*
******************************************************/

extern int XZSA_04ListeTacheMachineSSyst( 
                          XZSCT_NomMachine 	va_NomMachine,
                          XZSCT_NomSSysteme 	va_NomSSysteme,
			  void 			(*pa_FonctionUtil)(),
                          int 			*pa_NbElem,
			  void                  *pa_Args); 


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_05ExisterMachine(va_NomMachine)
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
*
******************************************************/

extern int XZSA_05ExisterMachine(XZSCT_NomMachine va_NomMachine);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_06ExisterSSysteme(va_NomSsysteme)
*
* Arguments
* NomSysteme	: Nom du sous-systme dont on veut 
* 	          verifier l'existence dans le noyan de
*                 supervision
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOS_INV	Nom du sous-systeme inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
*
******************************************************/

extern int XZSA_06ExisterSSysteme(XZSCT_NomSSysteme va_NomSsysteme);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_07ExisterTache(va_NomTache)
*
* Arguments
* NomTache	: Nom de la tache dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOT_INV	Nom de la tache inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
*
******************************************************/

extern int XZSA_07ExisterTache(XZSCT_NomTache va_NomTache);






/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_08TacheNomSSysteme(va_NomTache,
*			  va_NomSSyteme)
*
* Arguments
* NomTache	: Nom de la tache dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
* NomSSysteme	: Nom du sous-systeme.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOT_INV	Nom de la tache inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
*
******************************************************/

extern int XZSA_08TacheNomSSysteme(XZSCT_NomTache 	va_NomTache,
			     	   XZSCT_NomSSysteme 	va_NomSSysteme);




/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_11TacheTemporaire(va_NomTache)
*
* Arguments
* NomTache		: Nom de la Tache
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_TYPE_TPER     Tache Permanente
* XZSAC_NOT_INV	Nom de la Tache inconnue.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_11TacheTemporaire(XZSCT_NomTache va_NomTache);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_12ListeTachetemporaire(va_NomMachine,
*			   pa_FonctionUtil,
*			   pa_NbElem) 
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les taches
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_TYP_INV	Type de la machine invalide
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_12ListeTacheTemporaire(
	XZSCT_NomMachine 	va_NomMachine,
    	void 			(*pa_FonctionUtil)(),
        int 			*pa_NbElem,
	void                	*pa_Args); 


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_13ListeTachePermanente(va_NomMachine,
*			   pa_FonctionUtil,
*			   pa_NbElem) 
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les taches
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_TYP_INV	Type de la machine invalide
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_13ListeTachePermanente(
	XZSCT_NomMachine 	va_NomMachine,
    	void 			(*pa_FonctionUtil)(),
        int 			*pa_NbElem,
	void                	*pa_Args); 


extern int XZSA_14ListeShellScript(
	XZSCT_NomMachine 	va_NomMachine,
    	void 			(*pa_FonctionUtil)(),
        int 			*pa_NbElem,
	void                	*pa_Args); 

extern int XZSA_15TacheTempMachine(
	XZSCT_NomMachine        va_NomMachine,
	XZSCT_NomTache 		va_NomTache);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_17Type Machine(va_NomMachine, pa_TypeMachine)
*
* Arguments
* NomMachine 	: Nom de la machine dont on veut
*                 connaitre le type.
* TypeMachine   : Type de la machine XZSAC_TYPEM_ST,
*                 XZSAC_TYPEM_SD ou XZSAC_TYPEM_SC.
*
* Codes retour
* XZSCC_OK      ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/




extern int XZSA_17TypeMachine(XZSCT_NomMachine	va_NomMachine,
                              int 		*pa_TypeMachine);



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_18ListeSite(pa_FonctionUtil, pa_NbElem)
*
* Arguments
* FonctionUtil  : Pointeur sur fonction utilisateur.
* NbElem        : Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK      ok.
* XZSAC_INI_INV Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_18ListeSite(void 	(*pa_FonctionUtil)(),
                            int		*pa_NbElem,
			    void        *pa_Args);



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $ 
****************************************************** 
* STERIA *  PROJET MIGRAZUR 
****************************************************** 
* MODULE XZADMIN 
****************************************************** 
* SEQUENCE D'APPEL : 
* XZSA_19ListeMachineSite(va_NomSite, 
*		   pa_FonctionUtil, 
*		   pa_NbElem)
* 
* Arguments 
* NomSite	: Nom du site dont on veut connaitre
*	 	  les machines.
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur. 
*
* Codes retour   
* XZSCC_OK      ok. 
* XZSAC_NOA_INV Nom du site inconnu.
* XZSAC_INI_INV Erreur initialisation administration 
*               (utiliser la fonction XZSA_20).   
* 
* Mecanismes 
* Pointeur de fonction.  
******************************************************/ 
 
extern int XZSA_19ListeMachineSite(XZSCT_NomSite 	va_NomSite,
                            int 		(*pa_FonctionUtil)(),
                            int 		*pa_NbElem,
                            void                *pa_Args);
  


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL : 
* XZSA_20InitAdmin()
*
* Arguments
* 
* Codes retour 
* XZSCC_OK      ok.
* XZSAC_FIC_INV Erreur sur transfert des fichiers
*		d'administration du noyau de supervision
* XZSAC_INI_INV Erreur initialisation administration
*               deja effectuee.                 
*
* Mecanismes
* RTworks: RTS1 et RTS3.
******************************************************/
 
extern int XZSA_20InitAdmin();



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $ 
****************************************************** 
* STERIA *  PROJET MIGRAZUR 
****************************************************** 
* MODULE XZADMIN 
****************************************************** 
* SEQUENCE D'APPEL : 
* XZSA_21LibereRessourcesAdmin() 
* 
* Arguments 
*
* Codes retour   
* XZSCC_OK      ok. 
* XZSAC_FIC_INV	Erreur sur destruction du fichier
* 		d'administration du noyau de
*		supervision.
* XZSAC_INI_INV Erreur initialisation administration 
*               (utiliser la fonction XZSA_20).   
* 
* Mecanismes 
* Pointeur de fonction.  
******************************************************/ 
 
extern int XZSA_21LibereRessourcesAdmin();




/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_22ExisterSite(va_NomSite)
*
* Arguments
* NomSite   	: Nom du site de la machine dont on   
*                 veut verifier l'existence dans le
*	       	  noyau de supervision
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOS_INV	Nom du site inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_22ExisterSite(XZSCT_NomSite	va_NomSite);



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_21SiteMachine(va_NomMachine, va_NomSite)
*
* Arguments
* NomSite   	: Nom de la machine dont on veut 
*                 connaitre le type.
* NomSite   	: Nom du site de la machine.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_23SiteMachine(XZSCT_NomMachine 	va_NomMachine,
			      XZSCT_NomSite 	va_NomSite);



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_24ListeEntite(pa_FonctionUtil, pa_NbElem)
*
* Arguments
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_24ListeEntite(void 	(*pa_FonctionUtil)(),
                              int 	*pa_NbElem,
			      void      *pa_Args);



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_25ExisterEntite(va_NomEntite, va_TypeEntite)
* Arguments
* va_NomEntite	: Nom de l'entite dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
* va_TypeEntite	: Type de l'entite dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de l'entite inconnu.  
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
*
******************************************************/

extern int XZSA_25ExisterEntite(XZSCT_NomEntite va_NomEntite,
                            	int		va_TypeEntite);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_26TypeEntite(va_NomEntite, pa_TypeEntite)
* Arguments
* va_NomEntite	: Nom de l'entite dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
* pa_TypeEntite	: Type de l'entite que l'on veut recuperer
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOM_INV	Nom de l'entite inconnu.  
* XZSAC_INI_INV	Erreur initialisation administration
*
* Mecanismes
*
******************************************************/
extern int XZSA_26TypeEntite(XZSCT_NomEntite 	va_NomEntite,
                            int			*pa_TypeEntite);


/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSA_28SiteEntite(va_NomEntite, 
*		    va_TypeEntite, 
*		    va_NomSite)
*
* Arguments
* NomEntite	: Nom de l'entite a rajouter.
* TypeEntite	: Type de l'entite XZSAC_TYPE_PERI,
*		  XZSAC_TYPE_INTE ou XZSAC_TYPE_CZSE.
* NomSite	: Nom du site de l'entite.
*
* Codes retour
* XZSCC_OK	ok.
* XZSAC_NOE_INV	Nom de l'entite inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
* 		(utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/
 
extern int XZSA_28SiteEntite(XZSCT_NomEntite 	va_NomEntite,
                             int 		va_TypeEntite,
			     XZSCT_NomSite 	va_NomSite);



/* Fichier : $Id: xzsa.h,v 1.18 2010/11/30 17:53:28 gesconf Exp $		Release : $Revision: 1.18 $        Date : $Date: 2010/11/30 17:53:28 $ 
****************************************************** 
* STERIA *  PROJET MIGRAZUR 
****************************************************** 
* MODULE XZADMIN 
****************************************************** 
* SEQUENCE D'APPEL : 
* XZSA_29ListeEntiteSite(va_NomSite, 
*		   pa_FonctionUtil, 
*		   pa_NbElem)
* 
* Arguments 
* NomSite	: Nom du site dont on veut connaitre
*	 	  les machines.
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur. 
*
* Codes retour   
* XZSCC_OK      ok. 
* XZSAC_NOA_INV Nom du site inconnu.
* XZSAC_INI_INV Erreur initialisation administration 
*               (utiliser la fonction XZSA_20).   
* 
* Mecanismes 
* Pointeur de fonction.  
******************************************************/ 
 
extern int XZSA_29ListeEntiteSite(XZSCT_NomSite 	va_NomSite,
                            int 		(*pa_FonctionUtil)(),
                            int 		*pa_NbElem,
                            void                *pa_Args);
  


#endif

/* Non Applix - fin */

/* fin de fichier */
