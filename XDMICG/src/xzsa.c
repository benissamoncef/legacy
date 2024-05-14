/*E*/
/* Fichier : $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZADMIN         * FICHIER xzsa.c
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
*
* T.Milleville  30 Aou 1994  : Creation         
*
*  T.Milleville  29 Dec 1994      : Ajout de la fonction
*                                       XZSA_29ListeEntiteSite V 18.
*
*  T.Milleville  6 Fev 1994      : Modification de la fonction D'interface
*					XZSA_03..    V1.9
*  T.Milleville  9 Avr 1995      : Ajout du traitement dans la fonction
*	XZSA_11.. permettant de recuperer un Shell Script et 
*	creation de la fonction XZSA_14ListeShellScript V1.10
*  T.Milleville  2 Mai 1995      : Ajout d'un module d'inetrface
*	XZSA_15TacheTempMachine V1.10 
*  T.Milleville  25 Jul 1995      : Ajout de commentaires V1.11
*  T.Milleville  6 Oct 1995      : Ajout d'une fonction d'interface 
*	permettant de recuperer le type de l'entite (XZSA_26...)  V1.12
*  T.Milleville  26 Oct 1995      : Modification XZSA_03 pour recuperer le fait 
*	que la tache est PER ou PRE si l'argument p_args est non NULL V1.13
*  P.Niepceron	 19 Fev 1995	: Ajout de l'appel XZSC_InitDescNoyau dans XZSA_ActualiserAdmin v1.14
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.15
*******************************************************/

/* fichiers inclus */
#include <unistd.h>

#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include <xdg.h>
#include <xdm.h>

#include <errno.h>

#include <xzsc.h> 
#include <xzst.h>
#include <xzsm.h> 
#include <xzsc.h> 
#include <xzsx.h>
#include <xzsa.h>
#include <xzss.h>

/* declaration de variable locales */
static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA";

/* Indicateur d'initialisation */
int 				XZSAV_InitOk;

/* Listes globales d'administration */
static XZSAT_ListeMachine*	XZSAV_ListeMachine = NULL;
static XZSAT_ListeMachine*     	pg_MachineCourante;

static XZSAT_ListeSite*		XZSAV_ListeSite = NULL;
static XZSAT_ListeSite*		pg_SiteCourante;

static XZSAT_ListeSSysteme*	XZSAV_ListeSSysteme = NULL;
static XZSAT_ListeSSysteme*    	pg_SSystemeCourante;

static XZSAT_ListeTache*      	XZSAV_ListeTache = NULL;
static XZSAT_ListeTache*     	pg_TacheCourante;

static XZSAT_ListeEntite*	XZSAV_ListeEntite = NULL;
static XZSAT_ListeEntite*     	pg_EntiteCourante;


static XZSXT_Entite*		pg_grappeEntite = NULL;

XZSCT_NomMachine		XZSAV_NomSDActif = "";
XZSCT_NomMachine		XZSAV_NomMachineLocale = "";
char				XZSAV_NomAbsoluFicAdm [XDC_NOM_FIC_SIZE] = "";

/* Declaration de fonctions internes */

extern int XZSA_ActualiseMachine (XZSXT_Entite* pa_entite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_ActualiseMachine";
  register int 		vl_ValRet = XDC_OK;
  XZSAT_ListeMachine*	pl_machine;

	XZST_03EcritureTrace (XZSTC_FONCTION,
		" XZSA_ActualiseMachine : IN \n");



  if ((pl_machine = (XZSAT_ListeMachine*) malloc (sizeof (XZSAT_ListeMachine))) != NULL)
    {
      /* Allocation de l'espace necessaire */
      if ((pl_machine->machine = (XZSXT_ListeMachine*) malloc (sizeof (XZSXT_ListeMachine))) != NULL)
	  {
	    memcpy (pl_machine->machine, 
		(XZSXT_ListeMachine*) pa_entite->Data, 
		sizeof (XZSXT_ListeMachine));

	    pl_machine->suivant = NULL;

	    /* Insertion dans la liste */
	    if (XZSAV_ListeMachine == NULL)
	      {
		XZSAV_ListeMachine = pl_machine;
	      }
	    else
	      {
		pg_MachineCourante->suivant = pl_machine;
	      }
	    pg_MachineCourante = pl_machine;
	  }

	XZST_03EcritureTrace (XZSTC_FONCTION,"Insertion machine %s --\
		Type %d -- Mach Imp %s -- Site %s \n",
		pl_machine->machine->NomMachine,
		pl_machine->machine->TypeMachine,
		pl_machine->machine->NomMachineImp,
		pl_machine->machine->NomSite);
    }
  else
    {
      vl_ValRet = XDC_NOK;
    }
  
	XZST_03EcritureTrace (XZSTC_FONCTION,
		" XZSA_ActualiseMachine : OUT -- Code Retour %d  \n",
			vl_ValRet);

  return (vl_ValRet);
}
      

extern int XZSA_ActualiseTache (XZSXT_Entite* pa_entite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_ActualiseTache";
  register int 		vl_ValRet = XDC_OK;
  XZSAT_ListeTache*	pl_tache;

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"XZSA_ActualiseTache : IN \n");		

  if ((pl_tache = malloc (sizeof (XZSAT_ListeTache))) != NULL)
    {
      /* Allocation de l'espace necessaire */
      if ((pl_tache->tache = malloc (sizeof (XZSXT_ListeTache))) != NULL)
	  {
	    memcpy (pl_tache->tache, (XZSXT_ListeTache*) pa_entite->Data, sizeof (XZSXT_ListeTache));
	    pl_tache->suivant = NULL;
	    
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"Tache memorisee -- Nom Tache %s -- Nom SSyst %s \
		Type mach %d -- Tache Temp %d \n",
		pl_tache->tache->NomTache,
		pl_tache->tache->NomSSysteme,
		pl_tache->tache->TypeMachine,
		pl_tache->tache->Temporaire );


	    /* Insertion dans la liste */
	    if (XZSAV_ListeTache == NULL)
	      {
		XZSAV_ListeTache = pl_tache;
	      }
	    else
	      {
		pg_TacheCourante->suivant = pl_tache;
	      }
	    pg_TacheCourante = pl_tache;
	  }
    }
  else
    {
      vl_ValRet = XDC_NOK;
    }
  
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"XZSA_ActualiseTache : OUT -- Code Retour %d \n",
		vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_ActualiseTache */
      

extern int XZSA_ActualiseSSysteme (XZSXT_Entite* pa_entite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_ActualiseSSysteme";
  register int 		vl_ValRet = XDC_OK;
  XZSAT_ListeSSysteme*	pl_ssysteme;
 
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"XZSA_ActualiseSSysteme : IN \n");		

  if ((pl_ssysteme = malloc (sizeof (XZSAT_ListeSSysteme))) != NULL)
    {
      /* Allocation de l'espace necessaire */
      if ((pl_ssysteme->ssysteme = malloc (sizeof (XZSXT_ListeSSysteme))) != NULL)
	{
	  memcpy (pl_ssysteme->ssysteme, (XZSXT_ListeSSysteme*) pa_entite->Data, sizeof (XZSXT_ListeSSysteme));
	  pl_ssysteme->suivant = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"SSyst enrgeistre -- Nom SSyst %s -- Type Mach %d \n",
		pl_ssysteme->ssysteme->NomSSysteme,
		pl_ssysteme->ssysteme->TypeMachine);

	  /* Insertion dans la liste */
	  if (XZSAV_ListeSSysteme == NULL)
	    {
	      XZSAV_ListeSSysteme = pl_ssysteme;
	    }
	  else
	    {
	      pg_SSystemeCourante->suivant = pl_ssysteme;
	    }
	  pg_SSystemeCourante = pl_ssysteme;
	}
    }
  else
    {
      vl_ValRet = XDC_NOK;
    }
  
	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"XZSA_ActualiseSSysteme : OUT -- Code retour %d \n",
		vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_ActualiseSSysteme */ 
      

/*X*/
extern int XZSA_ActualiseSite (XZSXT_Entite* pa_entite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_ActualiseSite";
  register int 		vl_ValRet = XDC_OK;
  XZSAT_ListeSite*	pl_site;

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"XZSA_ActualiseSite : IN \n");

  if ((pl_site = malloc (sizeof (XZSAT_ListeSite))) != NULL)
    {
      /* Allocation de l'espace necessaire */
      if ((pl_site->site = malloc (sizeof (XZSXT_ListeSite))) != NULL)
	{
	  memcpy (pl_site->site, (XZSXT_ListeSite*) pa_entite->Data, sizeof (XZSXT_ListeSite));
	  pl_site->suivant = NULL;
	  
	  /* Insertion dans la liste */
	  if (XZSAV_ListeSite == NULL)
	    {
	      XZSAV_ListeSite = pl_site;
	    }
	  else
	    {
	      pg_SiteCourante->suivant = pl_site;
	    }
	  pg_SiteCourante = pl_site;
	}
    }
  else
    {
      vl_ValRet = XDC_NOK;
    }

	XZST_03EcritureTrace(XZSTC_FONCTION, 
		"XZSA_ActualiseSite : OUT -- Code retour %d \n",
		vl_ValRet);
  
  return (vl_ValRet);

}	/* Fin XZSA_ActualiseSite */
     



/*X*/
extern int XZSA_ActualiseEntite (XZSXT_Entite* pa_entite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_ActualiseEntite";
  register int 		vl_ValRet = XDC_OK;
  XZSAT_ListeEntite*	pl_entite;

  if ((pl_entite = malloc (sizeof (XZSAT_ListeEntite))) != NULL)
    {
      /* Allocation de l'espace necessaire */
      if ((pl_entite->entite = malloc (sizeof (XZSXT_ListeEntite))) != NULL)
	{
	  memcpy (pl_entite->entite, (XZSXT_ListeEntite*) pa_entite->Data, sizeof (XZSXT_ListeEntite));
	  pl_entite->suivant = NULL;
	  
	  /* Insertion dans la liste */
	  if (XZSAV_ListeEntite == NULL)
	    {
	      XZSAV_ListeEntite = pl_entite;
	    }
	  else
	    {
	      pg_EntiteCourante->suivant = pl_entite;
	    }
	  pg_EntiteCourante = pl_entite;
	}
    }
  else
    {
      vl_ValRet = XDC_NOK;
    }
  
  return (vl_ValRet);
}
      

/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine	: arguments de callback
*
* Fonction ActualiserAdmin
*
* Mecanismes
******************************************************/

extern int XZSA_ActualiserAdmin (
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg)
{
  /* variables locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_ActualiserAdmin";
  register int 			vl_ValRet = XDC_OK;
  XZSXT_Entite*			pl_entite;



	  XZST_03EcritureTrace (XZSTC_FONCTION,
		"XZSA_ActualiserAdmin  : IN  \n");

   /* Init de variable globales  modif 1.14 */
   vl_ValRet = XZSC_InitDescNoyau();


  /* Liberation des ressources */
  if ((vl_ValRet = XZSA_21LibereRessourcesAdmin ()) != XDC_OK)
    {
      switch (vl_ValRet)
	{
	case XZSAC_FIC_INV:
	  XZST_03EcritureTrace (XZSTC_WARNING, 
		"Callback XZSA_ActualiserAdmin: erreur de destruction de fichier");
	  break;

	case XZSAC_INI_INV:
	  XZST_03EcritureTrace (XZSTC_WARNING, 
		"Callback XZSA_ActualiserAdmin: erreur d'initilisation administration");
	  break;
	}
    }


  /* Ouverture et recuperation de la grappe totale */
  else if (XZSX_LitFichier (XZSAV_NomAbsoluFicAdm) != XDC_OK)
    {
      vl_ValRet = XZSAC_FIC_INV;
      XZST_03EcritureTrace (XZSTC_WARNING, 
		   "Callback XZSA_ActualiserAdmin: XZSXLitFichier, erreur");
    }
  /* Recuperation de la grappe des entites contenues dans le fichier */
  /* Initialisation de pg_grappeEntite */
  else if (XZSX_RecupereGrappe (&pg_grappeEntite) != XDC_OK)
    {
      vl_ValRet = XDC_NOK;
      XZST_03EcritureTrace (XZSTC_WARNING, 
	"Callback XZSA_ActualiserAdmin: XZSX_RecupereGrappe, erreur");
    }

  /* Decoupage en grappe specifique par type */
  else 
    {
      /* pour toutes les entitees de la grappe */
      for (pl_entite = pg_grappeEntite ; pl_entite != NULL; 
					pl_entite = pl_entite->Suivant)
	{
	  switch (pl_entite->Type)
	    {
	    case XZSXC_COMMENTAIRE:
	    case XZSXC_JOURNAL:
	    case XZSXC_INFOTACHE:
	    case XZSXC_INFOMACHINE:
	    case XZSXC_ACTION:
	      /* entites ne servant pas a la gestion des listes */
	      break;

	    case XZSXC_MACHINE:
	      XZSA_ActualiseMachine (pl_entite);		  
	      break;

	    case XZSXC_TACHE:
	      XZSA_ActualiseTache (pl_entite);
	      break;
	      
	    case XZSXC_SS:
	      XZSA_ActualiseSSysteme (pl_entite);
	      break;

	    case XZSXC_INFOSITE:
	      XZSA_ActualiseSite (pl_entite);
	      break;

	    case XZSXC_ENTITE:
	      XZSA_ActualiseEntite (pl_entite);
	      break;

	    default:
	      XZST_03EcritureTrace (XZSTC_WARNING, 
		    "XZSAActualiserAdmin : type %d non reconnu dans le fichier d'admin.",pl_entite->Type);
	      vl_ValRet = XDC_NOK;
	      break;
	    }
	}
      XZSAV_InitOk = XDC_VRAI;
      XZSX_OublieFichier ();
    }
	  XZST_03EcritureTrace (XZSTC_FONCTION,
		"XZSA_ActualiserAdmin  : OUT  -- Code Retour %d \n",
			vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_ActualiserAdmin */


/* definition de fonctions externes */


/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          verifier l'existence dans le noyan de
*                 supervision
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et le nom de la machine est      
* recherche dans le tableau XZSAV_ListeMachine
*
* Codes retour
* XDC_OK	ok.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_05ExisterMachine (
	XZSCT_NomMachine	va_NomMachine)

{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_05ExisterMachine";
  XZSAT_ListeMachine*		pl_Machine;
  register int			vl_ValRet = XZSAC_NOM_INV;

	XZST_03EcritureTrace (XZSTC_DEBUG1,
		"XZSA_05ExisterMachine IN : machine : %s \n", va_NomMachine );

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      for (pl_Machine = XZSAV_ListeMachine; 
	   (pl_Machine != NULL) && (vl_ValRet != XDC_OK); 
	   pl_Machine = pl_Machine->suivant)
	{
	XZST_03EcritureTrace (XZSTC_DEBUG2,"Existence mach %s \n",
			pl_Machine->machine->NomMachine);

	  if (!strcmp (pl_Machine->machine->NomMachine, va_NomMachine))
	    {
	      vl_ValRet = XDC_OK;
	    }
	}
    }
	  
	XZST_03EcritureTrace (XZSTC_DEBUG1,
		"XZSA_05ExisterMachine OUT : Code Retour : %d \n", 
		vl_ValRet );

  return (vl_ValRet);

}	/* Fin XZSA_05ExisterMachine */


/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomSysteme	: Nom du sous-systme dont on veut 
* 	          verifier l'existence dans le noyan de
*                 supervision
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et le nom du sous-systeme est    
* recherche dans le tableau XZSAV_ListeSSysteme
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOS_INV	Nom du sous-systeme inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_06ExisterSSysteme	(XZSCT_NomSSysteme	va_NomSSysteme)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_06ExisterSSysteme";
  XZSAT_ListeSSysteme*		pl_SSysteme;
  register int			vl_ValRet = XZSAC_NOM_INV;
  

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      for (pl_SSysteme = XZSAV_ListeSSysteme; 
	   (pl_SSysteme != NULL) && (vl_ValRet != XDC_OK); 
	   pl_SSysteme = pl_SSysteme->suivant)
	{
	  if (!strcmp (pl_SSysteme->ssysteme->NomSSysteme, va_NomSSysteme))
	    {
	      vl_ValRet = XDC_OK;
	    }
	}
    }
  return (vl_ValRet);
}


/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomTache	: Nom de la tache dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et le nom de la tache est    
* recherche dans le tableau XZSAV_ListeTache
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOT_INV	Nom de la tache inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_07ExisterTache		(XZSCT_NomTache		va_NomTache)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_07ExisterTache";
  XZSAT_ListeTache*			pl_Tache;
  register int			vl_ValRet = XZSAC_NOT_INV;
  

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      for (pl_Tache = XZSAV_ListeTache; 
	   (pl_Tache != NULL) && (vl_ValRet != XDC_OK); 
	   pl_Tache = pl_Tache->suivant)
	{
	  if (!strcmp (pl_Tache->tache->NomTache, va_NomTache))
	    {
	      vl_ValRet = XDC_OK;
	    }
	}
    }
	  
  return (vl_ValRet);
}


/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomSite   	: Nom du site de la machine dont on   
*                 veut verifier l'existence dans le
*	       	  noyau de supervision
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et le nom du site est      
* recherche dans le tableau XZSAV_ListeSite   
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOS_INV	Nom du site inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_22ExisterSite(XZSCT_NomSite   va_NomSite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_22ExisterSite";
  register int			vl_ValRet = XZSAC_NOS_INV;
  XZSAT_ListeSite*		pl_elementSite;
  
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      /* Recuperation du Nom de la Site */
      for (pl_elementSite = XZSAV_ListeSite; 
	   (pl_elementSite != NULL) && (vl_ValRet != XDC_OK); 
	   pl_elementSite = pl_elementSite->suivant)
	{
	  if (!strcmp (pl_elementSite->site->NomSite, va_NomSite))
	    {
	      vl_ValRet = XDC_OK;
	    }
	}
    }
  return (vl_ValRet);
}


/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine	: Nom de l'entite dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
* TypeEntite	: Type de l'entite dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et le nom et le type de l'entite
* sont recherches dans le tableau XZSAV_ListeEntite 
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOE_INV	Nom de l'entite inconnu.  
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_25ExisterEntite		(XZSCT_NomEntite 	va_NomEntite, 
					 int			va_TypeEntite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_25ExisterEntite";
  register int			vl_ValRet = XZSAC_NOE_INV;
  XZSAT_ListeEntite*		pl_elementEntite;
  
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      /* Recuperation du Nom de la Entite */
      for (pl_elementEntite = XZSAV_ListeEntite; 
	   (pl_elementEntite != NULL) && (vl_ValRet != XDC_OK); 
	   pl_elementEntite = pl_elementEntite->suivant)
	{
	  if ((!strcmp (pl_elementEntite->entite->NomEntite, va_NomEntite)) && 
	      (pl_elementEntite->entite->TypeEntite == va_TypeEntite))
	    {
	      vl_ValRet = XDC_OK;
	    }
	}
    }
  return (vl_ValRet);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer le type de l'entite
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSA_26TypeEntite(
	XZSCT_NomEntite 	va_NomEntite,
	int	*		pa_TypeEntite)

/*
* ARGUMENTS EN ENTREE :
*   va_NomEntite : Nom de l'entite
*
*
* ARGUMENTS EN SORTIE :
*   pa_TypeEntite :  Type de l'entite
*
*
* CODE RETOUR : 
* 	XDC_OK	ok.
* 	XZSAC_NOE_INV	Nom de l'entite inconnu.  
* 	XZSAC_INI_INV	Erreur initialisation administration
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Parcourt la liste des entites et retourne le type
*	correspondant au nom passe en argument
*
------------------------------------------------------*/
{
	static char *version="$Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ : XZSA_26TypeEntite " ;
	XZSAT_ListeEntite * 		pl_elementEntite	= NULL;
  	register int			vl_ValRet = XZSAC_NOM_INV;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_26TypeEntite : IN \
	-- Nom Entite  %s  \n",va_NomEntite);


  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      /* Recuperation du Nom de la machine  a partir du nom de la Machine */
      for (pl_elementEntite = XZSAV_ListeEntite; 
	   (pl_elementEntite != (XZSAT_ListeEntite*) NULL) 
		&& (vl_ValRet != XDC_OK); 
	   pl_elementEntite = pl_elementEntite->suivant)
	{
	  if (!strcmp (pl_elementEntite->entite->NomEntite, va_NomEntite))
	    {
	      /* Recuperation du type de la machine */
	      *pa_TypeEntite = pl_elementEntite->entite->TypeEntite;
	      vl_ValRet = XDC_OK;
	    }
	}
    }

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_26TypeEntite : OUT \
	-- Type Entite %d -- Code Retour %d \n",*pa_TypeEntite, vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_26TypeEntite */



/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Fonction
* Cette fonction verifie que le module est initialise
* (XZSAV_InitOk = 1).
* Pour chaque element du tableau XZAV_ListeMachine
* - Application de la fonction utilisateur passee en
* argument.
* Fpour
*
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN)(XZSCT_NomMachine)	NomMachine,
*		(IN)(int)		NumOrdre,
*               (OUT)(void *)           Args)
*
* Cette fonction retourne aussi le nombre d'elements
* du tableau reellement traites.
*
* Codes retour
* XDC_OK	ok.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_01ListeMachine	(int 	(*pa_FonctionUtil)(),
				 int	*pa_NbElem,
				 void   *pa_Args)
{
  /* Variables locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_01ListeMachine";
  register int 			vl_ValRet = XDC_OK;
  XZSAT_ListeMachine*		pl_listeMachine;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_01ListeMachine : IN \n");

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      *pa_NbElem = 0;
      for (pl_listeMachine = XZSAV_ListeMachine; 
	   pl_listeMachine != NULL; 
	   pl_listeMachine = pl_listeMachine->suivant)
	{
	  if (pa_FonctionUtil != NULL)
	  {
	     (*pa_FonctionUtil) (pl_listeMachine->machine->NomMachine, *pa_NbElem, pa_Args);
	  }
	  ++(*pa_NbElem);
	}
    } 
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_01ListeMachine : IN \n");

  return (vl_ValRet);


}	/* Fin XZSA_01ListeMachine */



/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZADMIN
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomMachine	: Nom de la machine dont on veut 
 * 	          connaitre les sous-systemes
 * FonctionUtil	: Pointeur sur fonction utilisateur.
 * NbElem	: Nombre d'elements de la liste.
 * Args          : Pointeur sur structure utilisateur.
 *
 * Fonction
 * Cette fonction verifie que le module est initialise 
 * (XZSAV_InitOk = 1) et l'existence de la machine est
 * teste avec la primitive XZSA_05.
 * Appel de la primitive XZSA_17 pour recuperer le type
 * de la machine.
 * Pour chaque element du tableau XZAV_ListeSSysteme
 * pour le type de la machine donne
 *   - Application de la fonction utilisateur passee en
 *     argument.
 * Fpour
 *
 * L'appel de la fonction utilisateur est le suivant :
 * FonctionUtil ((IN)(XZSCT_NomSSysteme)	NomSSysteme,
 *		(IN)(int)		NumOrdre,
 *              (OUT)(void *)           Args)
 * Cette fonction retourne aussi le nombre d'elements
 * du tableau reellement traites.
 * Codes retour
 * XDC_OK	ok.
 * XZSAC_NOM_INV	Nom de la machine inconnu.
 * XZSAC_TYP_INV	Type de la machine invalide
 * XZSAC_INI_INV	Erreur initialisation administration
 *               (utiliser la fonction XZSA_20).
 *
 * Mecanismes
 * Pointeur de fonction.
 *
 ******************************************************/
	  
extern int XZSA_02ListeSSysteme (
	XZSCT_NomMachine	va_NomMachine,
	void			(*pa_FonctionUtil)(),
	int			*pa_NbElem,
	void               	*pa_Args)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_02ListeSSysteme";
  register int 			vl_ValRet = XDC_OK;
  XZSAT_ListeSSysteme*		pl_listeSSysteme;
  int 				vl_typeMachine;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_02ListeSSysteme : IN \
	Nom Mac %s \n",va_NomMachine);

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* test de l'existence de la machine */
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOM_INV;
    }
  /* Recuperation du type de la machine */
  else if (XZSA_17TypeMachine (va_NomMachine, &vl_typeMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_TYP_INV;
    }
  else
    {
      *pa_NbElem = 0;

      for (pl_listeSSysteme = XZSAV_ListeSSysteme; 
	   pl_listeSSysteme != NULL; 
	   pl_listeSSysteme = pl_listeSSysteme->suivant)
	{
	  if ( pl_listeSSysteme->ssysteme->TypeMachine == vl_typeMachine)
	    {
	      if (pa_FonctionUtil != NULL)
	      {
	         (*pa_FonctionUtil) (pl_listeSSysteme->ssysteme->NomSSysteme, *pa_NbElem, pa_Args);
              }
	      ++(*pa_NbElem);
	    }
	}
    } 
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,"sXZSA_02ListeSSysteme : OUT \
	Nb Elts %d -- Code Retour %d \n",*pa_NbElem,vl_ValRet);

  return (vl_ValRet);
}

/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Fonction
* Cette fonction verifie que le module est initialise
* (XZSAV_InitOk = 1).
* Pour chaque element du tableau XZAV_ListeEntite 
* - Application de la fonction utilisateur passee en
* argument.
* Fpour
*
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN)(XZSCT_NomEntite)	NomEntite,
*		(IN)(int)		TypeEntite)
*		(IN)(int)		NumOrdre,
*               (OUT)(void *)           Args)
*
* Cette fonction retourne aussi le nombre d'elements
* du tableau reellement traites.
*
* Codes retour
* XDC_OK	ok.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_24ListeEntite (
	void	(*pa_FonctionUtil)(),
	int	*pa_NbElem,
	void	*pa_Args)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_24ListeEntite";
  register int 		vl_ValRet 		= XDC_OK;
  XZSAT_ListeEntite*	pl_listeEntite		= NULL;


	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_24ListeEntite : IN \n");

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      *pa_NbElem = 0;

      for (pl_listeEntite = XZSAV_ListeEntite; 
	   pl_listeEntite != NULL; 
	   pl_listeEntite = pl_listeEntite->suivant)
	{
	  if (pa_FonctionUtil != NULL)
	  {
	     (*pa_FonctionUtil) (pl_listeEntite->entite->NomEntite, 
	   		         pl_listeEntite->entite->TypeEntite, 
			         *pa_NbElem, 
			         pa_Args);
          }
	  ++(*pa_NbElem);
	  XZST_03EcritureTrace (XZSTC_FONCTION, "XZSA24_ListeEntite, \
		Application de la fonction user surl'entite  %s",
				pl_listeEntite->entite->NomEntite);
	}
    } 
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_24ListeEntite : OUT \
		-- Code retour %d -- Nb Elts %d \n",
		vl_ValRet,*pa_NbElem);

  return (vl_ValRet);

}	/* Fin XZSA_24ListeEntite */



/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomEntite	: Nom de l'entite 
* TypeEntite	: Type de l'entite XZSAC_TYPE_PERI,
*		  XZSAC_TYPE_INTE ou XZSAC_TYPE_CZSE.
* NomSite	: Nom du site de l'entite.
*
* Fonction
* Le nom de l'entite est verifie par la primitive
* XZSA_25.
* Pour chaque element du tableau XZAV_ListeEntite
*    - Comparaison du NomEntite et du TypeEntite avec
*      l'entite demandeuse.
*    - Si egalite
*       - Memorisation du numero d'ordre du site
*         correspondant au nom et au type de l'entite
*         donnee.
*    Fsi
* Fpour
* Recuperation du nom du site dans le tableau
* XZAV_ListeSite
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOE_INV	Nom de l'entite inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
* 		(utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/
 
extern int XZSA_28SiteEntite	(XZSCT_NomEntite	va_NomEntite,
				 int			va_TypeEntite,
				 XZSCT_NomSite  	va_NomSite)  
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_28SiteEntite";
  register int 			vl_ValRet = XDC_OK;
  XZSAT_ListeEntite*		pl_listeEntite;


  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* Verification du site */
  else if (XZSA_25ExisterEntite (va_NomEntite, va_TypeEntite) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOE_INV;
    }
  else
    {
      /* recherche du site a partir de l'entite et du type */
      for (pl_listeEntite = XZSAV_ListeEntite; pl_listeEntite != NULL; pl_listeEntite = pl_listeEntite->suivant)
	{
	  if ((!strcmp (pl_listeEntite->entite->NomEntite, va_NomEntite)) && (pl_listeEntite->entite->TypeEntite == va_TypeEntite))
	    {
	      strcpy (va_NomSite, pl_listeEntite->entite->NomSite);
	      XZST_03EcritureTrace (XZSTC_INFO, "XZSA28SiteEntite, Site %s trouve pour l'entite %s", 
				    pl_listeEntite->entite->NomSite,
				    pl_listeEntite->entite->NomEntite, va_NomEntite);
	    }
	}
    } 
  
  return (vl_ValRet);

}


/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* FonctionUtil  : Pointeur sur fonction utilisateur.
* NbElem        : Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Fonction
* Cette fonction verifie que le module est initialise
* (XZSAV_InitOk = 1).
* Pour chaque element du tableau XZAV_ListeSite    
* - Application de la fonction utilisateur passee en
* argument.
* Fpour
*
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN)(XZSCT_NomSite)	NomSite,
*               (IN)(int)		NumOrdre,
*               (OUT)(void *)           Args)
*
* Cette fonction retourne aussi le nombre d'elements
* du tableau reellement traites.
*
* Codes retour
* XDC_OK      ok.
* XZSAC_INI_INV Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_18ListeSite	(void     (*pa_FonctionUtil)(),
				 int      *pa_NbElem,
				 void     *pa_Args)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_18ListeSite";
  register int 			vl_ValRet = XDC_OK;
  XZSAT_ListeSite*		pl_listeSite	= NULL;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_18ListeSite : IN \n");

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else
    {
      *pa_NbElem = 0;

      for (pl_listeSite = XZSAV_ListeSite; pl_listeSite != NULL; 
				pl_listeSite = pl_listeSite->suivant)
	{
	  if (pa_FonctionUtil != NULL)
	  {
	     (*pa_FonctionUtil) (pl_listeSite->site->NomSite, 
						*pa_NbElem, pa_Args);
          }
	  ++(*pa_NbElem);
	  XZST_03EcritureTrace (XZSTC_INFO, "XZSA_18ListeSite, \
		Application de la fonction user sur le Site  %s",
				pl_listeSite->site->NomSite);
	}
    } 
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_18ListeSite : OUT \n");

  return (vl_ValRet);

}	/* Fin XZSA_18ListeSite */

/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZADMIN
 ******************************************************
 * ALGORITHME :
 *
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les taches
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et l'existence de la machine est
* testee avec la primitive XZSA_05.
* Appel de la primitive XZSA_17 pour recuperer le type
* de la machine.
* Pour chaque element du tableau XZAV_ListeSSysteme
* pour le type de la machine donne
*   - Memorisation du numero d'ordre du sous-systeme
*     trouve
*   - Pour chaque element du tableau XZAV_ListeTache
*     pour le numero de sous-systeme donne
*      - Application de la fonction utilisateur passee
*        en argument.
*     Fpour
* Fpour
*
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN)(XZSCT_NomTache)	NomTache,
*		(IN)(int)		NumOrdre,
*               (OUT)(void *)           Args)
* Cette fonction retourne aussi le nombre d'elements
* du tableau reellement traites.
* Codes retour
* XDC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_TYP_INV	Type de la machine invalide
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_03ListeTacheMachine 	(
	XZSCT_NomMachine	va_NomMachine,
	void			(*pa_FonctionUtil)(),
	int			*pa_NbElem,
	void                   *pa_Args)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_03ListeTacheMachine";
  register int 			vl_ValRet 		= XDC_OK;
  XZSAT_ListeSSysteme*		pl_listeSSysteme	= NULL;
  XZSAT_ListeTache*		pl_listeTache		= NULL;
  int				vl_typeMachine		= 0;
  XZSXT_Tache   		vl_strucTac;


	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSA_03ListeTacheMachine IN : \
	Nom Mach : %s *(int*)(pa_Args)=%d\n",va_NomMachine,*(int*)(pa_Args));

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* test de l'existence de la machine */
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOM_INV;
    }
  /* Recuperation du type de la machine */
  else if (XZSA_17TypeMachine (va_NomMachine, &vl_typeMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_TYP_INV;
    }
  else
    {
      	*pa_NbElem = 0;
      
	XZST_03EcritureTrace(XZSTC_DEBUG1,"Recherche des taches definies pour cette machine");
	for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; 
					pl_listeTache = pl_listeTache->suivant)
	{
		  if (pl_listeTache->tache->TypeMachine == vl_typeMachine )
		    {

#if 0
			/*A Verification que la tache est bien definie pour 
			*	cette machine  et que ce n'est pas un Shell Script */
			if (*(int*)(pa_Args) == XDC_VRAI) 
			{	
				vl_ValRet = XZSX_02TacheLecture (
					va_NomMachine,
					pl_listeTache->tache->NomTache,
					XDC_NSUP_PATHOPER,
					&vl_strucTac );
				if ( (vl_ValRet == XDC_OK) &&
				(pl_listeTache->tache->Temporaire != XZSAC_TYPE_TSHE) )
				{
					/*B Tache definie sur cette machine */

		      		XZST_03EcritureTrace(XZSTC_DEBUG1,
		      			"Appel Fonction Utile Nom Tache %s Temp %d  \n",
		      			pl_listeTache->tache->NomTache,
		      			pl_listeTache->tache->Temporaire);
	
	                         if (pa_FonctionUtil != NULL)
	     			 {
					/*B Execution Fonction Utilisateur */
		        		(*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, pl_listeTache->tache->Temporaire);
			   	 }
				/*B Incremente le nombre d'elements */
		  		++(*pa_NbElem);

				}
				else
				{
					XZST_03EcritureTrace(XZSTC_FONCTION,
					"Memorisation Tache Non presente \
				dans NSUP_ADMINIS et non NSUP_NomMachine \n");

					/*B Tache presente dans le fichier NSUP_ADMINIS
					*	mais pas definie pour cette machine */
					/*B Execution Fonction Utilisateur */
					if (pa_FonctionUtil != NULL)
		        		(*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, XZSAC_TYPE_TPRE);
				}
			}
			else
			{
	                         if (pa_FonctionUtil != NULL)
	     			 {
					/*B Fonction Utilisateur */
		         		(*pa_FonctionUtil) 
						(pl_listeTache->tache->NomTache, 
						*pa_NbElem, pa_Args);
				  }
		  
				/*B Incremente le nombre d'elements */
				++(*pa_NbElem);
			}
#endif
#if 1
			/*A Verification que la tache est bien definie pour 
			*	cette machine  et que ce n'est pas un Shell Script */
			if (*(int*)(pa_Args) == XDC_VRAI) 
			{	
				vl_ValRet = XZSX_02TacheLecture (
					va_NomMachine,
					pl_listeTache->tache->NomTache,
					XDC_NSUP_PATHOPER,
					&vl_strucTac );
				if ( (vl_ValRet == XDC_OK) &&
				(pl_listeTache->tache->Temporaire != XZSAC_TYPE_TSHE) )
				{
					/*B Tache definie sur cette machine */

		      		XZST_03EcritureTrace(XZSTC_DEBUG1,
		      			"Appel Fonction Utile Nom Tache %s Temp %d  \n",
		      			pl_listeTache->tache->NomTache,
		      			pl_listeTache->tache->Temporaire);
	
	                         if (pa_FonctionUtil != NULL)
	     			 {
					/*B Execution Fonction Utilisateur */
		        		(*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, pl_listeTache->tache->Temporaire);
			   	 }
				/*B Incremente le nombre d'elements */
		  		++(*pa_NbElem);

				}
				else
				{
/*					XZST_03EcritureTrace(XZSTC_FONCTION,
					"Memorisation Tache Non presente \
				dans NSUP_ADMINIS et non NSUP_NomMachine \n");
*/
					/*B Tache presente dans le fichier NSUP_ADMINIS
					*	mais pas definie pour cette machine */
					/*B Execution Fonction Utilisateur */
					if (pa_FonctionUtil != NULL)
		        		(*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, XZSAC_TYPE_TPRE);
				}
			}
			else
			{
				/* Modif V1.13 */
				if (pa_Args != NULL)
				{
					vl_ValRet = XZSX_02TacheLecture (
						va_NomMachine, pl_listeTache->tache->NomTache,
						XDC_NSUP_PATHOPER, &vl_strucTac );
					if ( (vl_ValRet == XDC_OK) &&
						(pl_listeTache->tache->Temporaire != XZSAC_TYPE_TSHE) )
					{
	                         		if (pa_FonctionUtil != NULL)
	     			 		{
							/*B Fonction Utilisateur */
		         				(*pa_FonctionUtil) 
								(pl_listeTache->tache->NomTache, *pa_NbElem, pa_Args);
				  		}
					}
				}
				else
				{
	                         	if (pa_FonctionUtil != NULL)
	     			 	{
						/*B Fonction Utilisateur */
		         			(*pa_FonctionUtil) 
							(pl_listeTache->tache->NomTache, *pa_NbElem, pa_Args);
				  	}
				}
		  
				/*B Incremente le nombre d'elements */
				++(*pa_NbElem);
			}
#endif
                      }
	}
    } 
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,	
		"XZSA_03ListeTacheMachine OUT : \
	Code Retour %d -- Nb Elts %d \n",vl_ValRet , *pa_NbElem);

  return (vl_ValRet);
}



/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine	: Nom de la machine dont on veut 
* 	          connaitre les taches
* NomSSysteme	: Nom du sous-systeme dont on veut 
*		  connaitre les taches
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste.
* Args          : Pointeur sur structure utilisateur.
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et l'existence de la machine est
* testee avec la primitive XZSA_05 et XZSA_06.
* Appel de la primitive XZSA_17 pour recuperer le type
* de la machine.
* Pour chaque element du tableau XZAV_ListeSSysteme
*   - Memorisation du numero d'ordre du sous-systeme
*     correspondant au nom du sous-systeme et au type
*     de la machine 
* Fpour
* Pour chaque element du tableau XZAV_ListeTache
* pour le numero de sous-systeme donne
*   - Application de la fonction utilisateur passee
*     en argument.
* Fpour
*
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN)(XZSCT_NomTache)	NomTache,
*		(IN)(int)		NumOrdre,
*               (OUT)(void *)           Args)
* Cette fonction retourne aussi le nombre d'elements
* du tableau reellement traites.
* Codes retour
* XDC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_TYP_INV	Type de la machine invalide
* XZSAC_NOS_INV Nom de sous-systeme inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZSA_04ListeTacheMachineSSyst(
	XZSCT_NomMachine	va_NomMachine,
	XZSCT_NomSSysteme	va_NomSSysteme,
	void			(*pa_FonctionUtil)(),
	int			*pa_NbElem,
	void              	*pa_Args) 

{
  /* Variables locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_04ListeTacheMachineSSyst";
  register int 			vl_ValRet 	= XDC_OK;
  XZSAT_ListeSSysteme*		pl_listeSSysteme= NULL;
  XZSAT_ListeTache*		pl_listeTache	= NULL;
  int				vl_typeMachine	= 0;

	XZST_03EcritureTrace(XZSTC_INTERFACE,	
		"XZSA_04ListeTacheMachineSSyst  : IN  \
	-- Nom Mach %s -- Nom SSyst %s \n",va_NomMachine,
	va_NomSSysteme);

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* test de l'existence de la machine */
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOM_INV;
    }
  /* Recuperation du type de la machine */
  else if (XZSA_17TypeMachine (va_NomMachine, &vl_typeMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_TYP_INV;
    }
  else
    {
      *pa_NbElem = 0;

      for (pl_listeSSysteme = XZSAV_ListeSSysteme; 
		pl_listeSSysteme != NULL; 
		pl_listeSSysteme = pl_listeSSysteme->suivant)
	{
	  if ( (pl_listeSSysteme->ssysteme->TypeMachine == vl_typeMachine ) 		&& 
	      (!(strcmp (pl_listeSSysteme->ssysteme->NomSSysteme, va_NomSSysteme))))
	    {
	      for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; pl_listeTache = pl_listeTache->suivant)
		{
		  if (!(strcmp (pl_listeTache->tache->NomSSysteme, pl_listeSSysteme->ssysteme->NomSSysteme)) &&
		      (pl_listeTache->tache->TypeMachine == vl_typeMachine) )
		    {
	              if (pa_FonctionUtil != NULL)
		      {
		        (*pa_FonctionUtil) (pl_listeTache->tache->NomTache, *pa_NbElem, pa_Args);
		      }
		      ++(*pa_NbElem);
		    }
		}
	    }
	}
    } 
	XZST_03EcritureTrace(XZSTC_INTERFACE,	
		"XZSA_04ListeTacheMachineSSyst  : OUT  \
		Code Reour %d -- Nb elets %d \n",
		vl_ValRet,*pa_NbElem);

  return (vl_ValRet);

}	/* Fin XZSA_04ListeTacheMachineSSyst */




/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $ 
****************************************************** 
* STERIA *  PROJET MIGRAZUR 
****************************************************** 
* MODULE XZADMIN 
****************************************************** 
* ALGORITHME : 
* 
* Arguments 
* NomSite	: Nom du site dont on veut connaitre
*	 	  les machines.
* FonctionUtil	: Pointeur sur fonction utilisateur.
* NbElem	: Nombre d'elements de la liste. 
* Args          : Pointeur sur structure utilisateur.
*
* Fonction 
* Cette fonction verifie que le module est initialise
* (XZSAV_InitOk = 1) et l'existence du site est teste
* avec la primitive XZSA_22.
* Recherche du numero d'ordre du site dans le tableau
* XZAV_ListeSite.
* Pour chaque element du tableau XZAV_ListeMachine 
* pour le numero d'ordre du site donne
*   - Application de la fonction utilisateur passee
*     en argument.
* Fpour
*
* L'appel de la fonction utilisateur est le suivant : 
* FonctionUtil ((IN)(XZSCT_NomMachine)  NomMachine)
*               (IN)(int)               NumOrdre,
*               (OUT)(void *)           Args)
* 
* Cette fonction retourne aussi le nombre d'elements 
* du tableau reellement traites. 
* 
* Codes retour   
* XDC_OK      ok. 
* XZSAC_NOA_INV Nom du site inconnu.
* XZSAC_INI_INV Erreur initialisation administration 
*               (utiliser la fonction XZSA_20).   
* 
* Mecanismes 
* Pointeur de fonction.  
******************************************************/ 
 
extern int XZSA_19ListeMachineSite(
	XZSCT_NomSite	va_NomSite,
	int		(*pa_FonctionUtil)(),
	int		*pa_NbElem,
	void           *pa_Args)
{ 
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_19ListeMachineSite";
  register int 			vl_ValRet = XDC_OK;
  XZSAT_ListeMachine*		pl_listeMachine;


  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* test de l'existence du site */
  else if (XZSA_22ExisterSite (va_NomSite) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOA_INV;
    }
  else
    {
      *pa_NbElem = 0;

      for (pl_listeMachine = XZSAV_ListeMachine; 
	   pl_listeMachine != NULL; 
	   pl_listeMachine = pl_listeMachine->suivant)
	{
	  if  ( strcmp( pl_listeMachine->machine->NomSite, va_NomSite ) == 0 )
	  {
	     if (pa_FonctionUtil != NULL)
	     {
	       (*pa_FonctionUtil) (pl_listeMachine->machine->NomMachine, 
	         		        *pa_NbElem, 
	         		        pa_Args);
             }
	     ++(*pa_NbElem);
	     XZST_03EcritureTrace (XZSTC_INFO, 
				"XZSA19ListeMachineSite, Application de la fonction user sur la machine %s du Site %s",
				pl_listeMachine->machine->NomMachine,
				pl_listeMachine->machine->NomSite);
	   }		
	}
    } 
  
  return (vl_ValRet);


}	/* Fin XZSA_19ListeMachineSite */


  
/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomTache	: Nom de la tache dont on veut 
* 	          verifier l'existence dans le noyau de
*                 supervision
* NomSSysteme	: Nom du sous-systeme.
*
* Fonction
* Cette fonction verifie que le module est initialise 
* (XZSAV_InitOk = 1) et l'existence de la tache est    
* verifie par la primitive XZSA_07
*
* Pour chaque element du tableau XZAV_ListeTache
*    - Comparaison avec le nom de la t\142che
*    - Si egalite avec NomTache demandeur
*       - Memorisation du numero d'ordre du
*         sous-systeme correspondant au nom de la
*         tache donnee.
*      Fsi
* Fpour
* Recuperation du nom du sous-systeme dans le tableau
* XZAV_ListeSSysteme
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOT_INV	Nom de la tache inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_08TacheNomSSysteme	(XZSCT_NomTache		va_NomTache,
					 XZSCT_NomSSysteme	va_NomSSysteme)
{
  /* Variables locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_08TacheNomSSysteme";
  register int			vl_ValRet = XZSAC_NOT_INV;
  XZSAT_ListeTache*		pl_elementTache;
  int				vl_trouve = XDC_FAUX;

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSA_08TacheNomSSysteme : IN   -- Nom Tache %s \
		-- Nom SSysteme %s \n",va_NomTache, va_NomSSysteme );


  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else if (XZSA_07ExisterTache (va_NomTache) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOT_INV;
    }
  else
    {
      /* Recuperation du No du Ss Systeme a partir du nom de la Tache */
      for (pl_elementTache = XZSAV_ListeTache; 
	   (pl_elementTache != (XZSAT_ListeTache*) NULL) && (vl_trouve != XDC_VRAI); 
	   pl_elementTache = pl_elementTache->suivant)
	{
	  if (!strcmp (pl_elementTache->tache->NomTache, va_NomTache))
	    {
	      /* Recuperation du nom du sous-systeme */
	      memcpy (va_NomSSysteme, pl_elementTache->tache->NomSSysteme, sizeof (XZSCT_NomSSysteme));
	      vl_trouve = XDC_VRAI;
	      vl_ValRet = XDC_OK;
	    }
	}
    }

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSA_08TacheNomSSysteme : OUT  -- Code Retour %d \n",
		vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_08TacheNomSSysteme */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de savoir si la tache specifiee est temporaire 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSA_11TacheTemporaire(
	XZSCT_NomTache	va_NomTache)

/*
* ARGUMENTS EN ENTREE :
*   va_NomTache : Nom de la tache dont on veut connaitre
*			sa specificite
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK		ok
*   XZSAC_TYPE_TPER	Tache Permanente
*   XZSAC_TYPE_TSHE	Shell Script
*   XZSAC_NOT_INV	Nom de la tache inconnu
*   XZSAC_INI_INV	Erreur initialisation administration
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*	Teste si l'initialisation est correcte, puis
*	dans le tableau XZSAV_ListeTache verifie si la 
*	tache specifiee existe et si elle est temporaire
*
------------------------------------------------------*/
{
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_11TacheTemporaire";
  XZSAT_ListeTache*		pl_listeTache;
  int				vl_ValRet	= XDC_OK;
  int				vl_TypeMachine	= 0;


	XZST_03EcritureTrace( XZSTC_INTERFACE,
		" XZSA_11TacheTemporaire : IN  Nom Tache %s \n", va_NomTache); 

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
  {
	XZST_03EcritureTrace( XZSTC_WARNING,
		" Pb Initialisation non effectuee \n");

      vl_ValRet = XZSAC_INI_INV;
  }
  /* test de l'existence de la Tache */
  else if (XZSA_07ExisterTache (va_NomTache) != XDC_OK)
  { 
	XZST_03EcritureTrace( XZSTC_WARNING,
		" Pb Tache Inexistante \n");

      vl_ValRet = XZSAC_NOT_INV;
  }
  /* test de l'existence de la Tache */
  else if (XZSC_05TypeMachine(&vl_TypeMachine) != XDC_OK)
  { 
	XZST_03EcritureTrace( XZSTC_WARNING,
		" Pb Type Tache indefini  \n");

      vl_ValRet = XZSAC_NOT_INV;
  }
  else
  {
	for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; 
				pl_listeTache = pl_listeTache->suivant)
	{
		XZST_03EcritureTrace( XZSTC_DEBUG2,
			"Parcours liste tache %s -- Type mach %d \n",
			pl_listeTache->tache->NomTache,
			pl_listeTache->tache->TypeMachine);
		  if ( (!strcmp((char*)pl_listeTache->tache->NomTache,
						(char*)va_NomTache))
		    && (pl_listeTache->tache->TypeMachine == 
						vl_TypeMachine) )
			
		  {
	              if (pl_listeTache->tache->Temporaire == 
							XZSAC_TYPE_TTMP)
		      {
			XZST_03EcritureTrace( XZSTC_DEBUG2,
				"Tache Temporaire %s --  %d \n",
				pl_listeTache->tache->NomTache,
				pl_listeTache->tache->Temporaire);
			vl_ValRet = XDC_OK;
			break;
		      }
		      else if  (pl_listeTache->tache->Temporaire ==
						      XZSAC_TYPE_TPER)
		      {
			XZST_03EcritureTrace( XZSTC_DEBUG2,
				"Tache Permanente %s --  %d \n",
				pl_listeTache->tache->NomTache,
				pl_listeTache->tache->Temporaire);
			vl_ValRet = XZSAC_TYPE_TPER;
			break;
		      }
		      else if  (pl_listeTache->tache->Temporaire ==
						      XZSAC_TYPE_TSHE)
		      {
			XZST_03EcritureTrace( XZSTC_DEBUG2,
				"Tache SHELL %s --  %d \n",
				pl_listeTache->tache->NomTache,
				pl_listeTache->tache->Temporaire);
			vl_ValRet = XZSAC_TYPE_TSHE;
			break;
		      }
		 }
	}
   } 


	XZST_03EcritureTrace( XZSTC_INTERFACE,
		" XZSA_11TacheTemporaire : OUT   Code Retour %d \n",vl_ValRet);

  return (vl_ValRet);


}	/* Fin XZSA_TacheTemporaire */



/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer la liste des taches temporaires
*	associees a une Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSA_12ListeTacheTemporaire(
	XZSCT_NomMachine	va_NomMachine,
	void			(*pa_FonctionUtil)(),
	int			*pa_NbElem,
	void           		*pa_Args)


/*
* ARGUMENTS EN ENTREE :
*   va_NomMachine :  Nom de la machine
*   (*pa_FonctionUtil)() :  fonction utilisateur a appliquer
*		pour les taches temporaires trouvees
*   pa_Args : Arguments pour la fonction utilisateur 
*
*
* ARGUMENTS EN SORTIE :
*	pa_NbElem : Nombre de tache temporaire 
*
*
* CODE RETOUR : 
*   XDC_OK		ok
*   XZSAC_NOM_INV	Nom de la machine inconnue
*   XZSAC_INI_INV	Erreur initialisation administration
*   XZSAC_TYP_INV	Type de la Machine Invalide
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Verifie si l'initialisation est correcte, puis verifie 
*	l'existence de la machine, recupere le type de la machine 
*	et pour chaque element de la listye des Sous-Systeme
*	et si la tache est temporaire, on applique la 
*	fonction utilisateur .
*
------------------------------------------------------*/
{

  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_12ListeTacheTemporaire";
  register int 			vl_ValeurRetour = XDC_OK;
  XZSAT_ListeTache*		pl_listeTache;
  int				vl_typeMachine;

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValeurRetour = XZSAC_INI_INV;
    }
  /* test de l'existence de la machine */
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValeurRetour = XZSAC_NOM_INV;
    }
  /* Recuperation du type de la machine */
  else if (XZSA_17TypeMachine (va_NomMachine, &vl_typeMachine) != XDC_OK)
    {
      vl_ValeurRetour = XZSAC_TYP_INV;
    }
  else
    {
      	*pa_NbElem = 0;
      
	for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; 
					pl_listeTache = pl_listeTache->suivant)
	{
		  if (pl_listeTache->tache->TypeMachine == vl_typeMachine )
		  {
			if (pl_listeTache->tache->Temporaire == XZSAC_TYPE_TTMP)
			{
	              	   if (pa_FonctionUtil != NULL)
		      	   {
		               (*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, pa_Args);
                           }
		           ++(*pa_NbElem);
		        }	  
		    }
	}
    } 
  return (vl_ValeurRetour);


}	/* XZSA_ListeTacheTemporaire */


/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer la liste des taches permanentes
*	associees a une Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSA_13ListeTachePermanente(
	XZSCT_NomMachine	va_NomMachine,
	void			(*pa_FonctionUtil)(),
	int			*pa_NbElem,
	void           		*pa_Args)


/*
* ARGUMENTS EN ENTREE :
*   va_NomMachine :  Nom de la machine
*   (*pa_FonctionUtil)() :  fonction utilisateur a appliquer
*		pour les taches temporaires trouvees
*   pa_Args : Arguments pour la fonction utilisateur 
*
*
* ARGUMENTS EN SORTIE :
*	pa_NbElem : Nombre de tache temporaire 
*
*
* CODE RETOUR : 
*   XDC_OK		ok
*   XZSAC_NOM_INV	Nom de la machine inconnue
*   XZSAC_INI_INV	Erreur initialisation administration
*   XZSAC_TYP_INV	Type de la Machine Invalide
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Verifie si l'initialisation est correcte, puis verifie 
*	l'existence de la machine, recupere le type de la machine 
*	et pour chaque element de la liste des taches 
*	et si la tache est permanente, on applique la 
*	fonction utilisateur .
*
------------------------------------------------------*/
{

  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_13ListeTachePermanente";
  register int 			vl_ValeurRetour = XDC_OK;
  XZSAT_ListeTache*		pl_listeTache	= NULL;
  int				vl_typeMachine	=	0;

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValeurRetour = XZSAC_INI_INV;
    }
  /* test de l'existence de la machine */
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValeurRetour = XZSAC_NOM_INV;
    }
  /* Recuperation du type de la machine */
  else if (XZSA_17TypeMachine (va_NomMachine, &vl_typeMachine) != XDC_OK)
    {
      vl_ValeurRetour = XZSAC_TYP_INV;
    }
  else
    {
      	*pa_NbElem = 0;
      
	for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; 
					pl_listeTache = pl_listeTache->suivant)
	{
		  if (pl_listeTache->tache->TypeMachine == vl_typeMachine )
		  {
			if (pl_listeTache->tache->Temporaire == XZSAC_TYPE_TPER)
			{
	              	   if (pa_FonctionUtil != NULL)
		      	   {
		               (*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, pa_Args);
                           }
		           ++(*pa_NbElem);
		        }	  
		    }
	}
    } 
  return (vl_ValeurRetour);


}	/* XZSA_ListeTachepermanente */




/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer la liste des Sheeel Scripts
*	associees a une Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSA_14ListeShellScript(
	XZSCT_NomMachine	va_NomMachine,
	void			(*pa_FonctionUtil)(),
	int			*pa_NbElem,
	void           		*pa_Args)


/*
* ARGUMENTS EN ENTREE :
*   va_NomMachine :  Nom de la machine
*   (*pa_FonctionUtil)() :  fonction utilisateur a appliquer
*		pour les taches temporaires trouvees
*   pa_Args : Arguments pour la fonction utilisateur 
*
*
* ARGUMENTS EN SORTIE :
*	pa_NbElem : Nombre de shell Scripts
*
*
* CODE RETOUR : 
*   XDC_OK		ok
*   XZSAC_NOM_INV	Nom de la machine inconnue
*   XZSAC_INI_INV	Erreur initialisation administration
*   XZSAC_TYP_INV	Type de la Machine Invalide
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Verifie si l'initialisation est correcte, puis verifie 
*	l'existence de la machine, recupere le type de la machine 
*	et pour chaque element de la liste des taches 
*	et si la tache est un Shell Script, on applique la 
*	fonction utilisateur .
*
------------------------------------------------------*/
{

  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_14ListeShellScript";
  register int 			vl_ValeurRetour = XDC_OK;
  XZSAT_ListeTache*		pl_listeTache	= NULL;
  int				vl_typeMachine	=	0;

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValeurRetour = XZSAC_INI_INV;
    }
  /* test de l'existence de la machine */
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValeurRetour = XZSAC_NOM_INV;
    }
  /* Recuperation du type de la machine */
  else if (XZSA_17TypeMachine (va_NomMachine, &vl_typeMachine) != XDC_OK)
    {
      vl_ValeurRetour = XZSAC_TYP_INV;
    }
  else
    {
      	*pa_NbElem = 0;
      
	for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; 
					pl_listeTache = pl_listeTache->suivant)
	{
		  if (pl_listeTache->tache->TypeMachine == vl_typeMachine )
		  {
			if (pl_listeTache->tache->Temporaire == XZSAC_TYPE_TSHE)
			{
	              	   if (pa_FonctionUtil != NULL)
		      	   {
		               (*pa_FonctionUtil) (pl_listeTache->tache->NomTache, 
						*pa_NbElem, pa_Args);
                           }
		           ++(*pa_NbElem);
		        }	  
		    }
	}
    } 
  return (vl_ValeurRetour);


}	/* XZSA_14ListeShellScript */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de savoir si la tache associee a la machine
*	specifiee est temporaire 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSA_15TacheTempMachine(
	XZSCT_NomMachine	va_NomMachine,
	XZSCT_NomTache		va_NomTache)

/*
* ARGUMENTS EN ENTREE :
*   va_Nommachine : Nom de la machine 
*   va_NomTache : Nom de la tache dont on veut connaitre
*			sa specificite
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK		ok
*   XZSAC_TYPE_TPER	Tache Permanente
*   XZSAC_TYPE_TSHE	Shell Script
*   XZSAC_NOT_INV	Nom de la tache inconnu
*   XZSAC_INI_INV	Erreur initialisation administration
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*	Teste si l'initialisation est correcte, puis
*	dans le tableau XZSAV_ListeTache verifie si la 
*	tache specifiee existe et si elle est temporaire
*
------------------------------------------------------*/
{
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_15TacheTempMachine";
  XZSAT_ListeTache*		pl_listeTache	= NULL;
  int				vl_ValRet	= XDC_OK;
  int				vl_TypeMachine	= 0;


	XZST_03EcritureTrace( XZSTC_INTERFACE,
		" XZSA_15TacheTemporaireMachine : IN  \
		-- Nom Machine %s -- Nom Tache %s \n", 
		va_NomMachine, va_NomTache); 

  /* Teste si XZSA_20 a deja ete appelee */
  if (XZSAV_InitOk != XDC_VRAI)
  {
	XZST_03EcritureTrace( XZSTC_WARNING,
		" Pb Initialisation non effectuee \n");

      vl_ValRet = XZSAC_INI_INV;
  }
  /* test de l'existence de la Tache */
  else if (XZSA_17TypeMachine(va_NomMachine,&vl_TypeMachine) != XDC_OK)
  { 
	XZST_03EcritureTrace( XZSTC_WARNING,
		" Pb Type Tache indefini  \n");

      vl_ValRet = XZSAC_NOT_INV;
  }
  else
  {
	for (pl_listeTache = XZSAV_ListeTache; pl_listeTache != NULL; 
				pl_listeTache = pl_listeTache->suivant)
	{
		XZST_03EcritureTrace( XZSTC_DEBUG2,
			"Parcours liste tache %s -- Type mach %d \n",
			pl_listeTache->tache->NomTache,
			pl_listeTache->tache->TypeMachine);
		  if ( (!strcmp((char*)pl_listeTache->tache->NomTache,
						(char*)va_NomTache))
		    && (pl_listeTache->tache->TypeMachine == 
						vl_TypeMachine) )
			
		  {
	              if (pl_listeTache->tache->Temporaire == 
							XZSAC_TYPE_TTMP)
		      {
			XZST_03EcritureTrace( XZSTC_DEBUG2,
				"Tache Temporaire %s --  %d \n",
				pl_listeTache->tache->NomTache,
				pl_listeTache->tache->Temporaire);
			vl_ValRet = XDC_OK;
			break;
		      }
		      else if  (pl_listeTache->tache->Temporaire ==
						      XZSAC_TYPE_TPER)
		      {
			XZST_03EcritureTrace( XZSTC_DEBUG2,
				"Tache Permanente %s --  %d \n",
				pl_listeTache->tache->NomTache,
				pl_listeTache->tache->Temporaire);
			vl_ValRet = XZSAC_TYPE_TPER;
			break;
		      }
		      else if  (pl_listeTache->tache->Temporaire ==
						      XZSAC_TYPE_TSHE)
		      {
			XZST_03EcritureTrace( XZSTC_DEBUG2,
				"Tache SHELL %s --  %d \n",
				pl_listeTache->tache->NomTache,
				pl_listeTache->tache->Temporaire);
			vl_ValRet = XZSAC_TYPE_TSHE;
			break;
		      }
		 }
	}
   } 


	XZST_03EcritureTrace( XZSTC_INTERFACE,
		" XZSA_15TacheTemporaireMachine : OUT   Code Retour %d \n",vl_ValRet);

  return (vl_ValRet);


}	/* Fin XZSA_15TacheTemporaireMachine */


/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine 	: Nom de la machine dont on veut
*                 connaitre le type.
* TypeMachine   : Type de la machine XZSAC_TYPEM_ST,
*                 XZSAC_TYPEM_SD ou XZSAC_TYPEM_SC.
*
* Fonction
* Le nom de la machine est verifie par la primitive 
* XZSA_05.
* Pour chaque element du tableau XZAV_ListeMachine
*   - Comparaison du NomMachine avec la machine demandeur.
*   - Si egalite
*      - Memorisation du type de la machine
*        correspondant au nom de la machine donne.
*   Fsi
* Fpour
*
* Codes retour
* XDC_OK      ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_17TypeMachine		(XZSCT_NomMachine	va_NomMachine,
					 int			*pa_TypeMachine)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_17TypeMachine";
  register int			vl_ValRet = XZSAC_NOM_INV;
  XZSAT_ListeMachine*		pl_elementMachine;
 

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_17TypeMachine : IN \
	-- Nom Mach %s  \n",va_NomMachine);


  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOM_INV;
    }
  else
    {
      /* Recuperation du Nom de la machine  a partir du nom de la Machine */
      for (pl_elementMachine = XZSAV_ListeMachine; 
	   (pl_elementMachine != (XZSAT_ListeMachine*) NULL) 
		&& (vl_ValRet != XDC_OK); 
	   pl_elementMachine = pl_elementMachine->suivant)
	{
	  if (!strcmp (pl_elementMachine->machine->NomMachine, va_NomMachine))
	    {
	      /* Recuperation du type de la machine */
	      *pa_TypeMachine = pl_elementMachine->machine->TypeMachine;
	      vl_ValRet = XDC_OK;
	    }
	}
    }

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_17TypeMachine : OUT \
	-- Type Mach %d -- Code Retour %d \n",*pa_TypeMachine,
	vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_17TypeMachine  */


/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomSite   	: Nom de la machine dont on veut 
*                 connaitre le type.
* NomSite   	: Nom du site de la machine.
*
* Fonction
* Le nom de la machine est verifie par la primitive
* XZSA_05.
* Pour chaque element du tableau XZAV_ListeMachine
*    - Comparaison du NomMachine avec la machine 
*      demandeur.
*    - Recuperation du nom du site dans le tableau
* XZAV_ListeSite.
*
* Codes retour
* XDC_OK	ok.
* XZSAC_NOM_INV	Nom de la machine inconnu.
* XZSAC_INI_INV	Erreur initialisation administration
*               (utiliser la fonction XZSA_20).
*
* Mecanismes
******************************************************/

extern int XZSA_23SiteMachine	(
	XZSCT_NomMachine	va_NomMachine,
	XZSCT_NomSite   	va_NomSite)
{
  /* Variables Locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_23SiteMachine";
  register int			vl_ValRet = XZSAC_NOM_INV;
  XZSAT_ListeMachine*		pl_elementMachine;
  int				vl_trouve = XDC_FAUX;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_23SiteMachine : IN \
		-- Nom Mach %s  \n",va_NomMachine);

  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  else if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOM_INV;
    }
  else
    {
      /* Recuperation du No du Ss Systeme a partir du nom de la Tache */
      for (pl_elementMachine = XZSAV_ListeMachine; 
	   (pl_elementMachine != (XZSAT_ListeMachine*) NULL) && (vl_trouve != XDC_VRAI); 
	   pl_elementMachine = pl_elementMachine->suivant)
	{
	  if (!strcmp (pl_elementMachine->machine->NomMachine, va_NomMachine))
	    {
	      /* Recuperation du nom du sous-systeme */
	      memcpy (va_NomSite, pl_elementMachine->machine->NomSite, sizeof (XZSCT_NomSite));
	      vl_trouve = XDC_VRAI;
	      vl_ValRet = XDC_OK;
	    }
	}
    }
	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_23SiteMachine : OUT \
		-- Nom Site %s -- Code Retour %d  \n",va_NomSite,vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSA_23SiteMachine */


/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN
******************************************************
* ALGORITHME : 
*
* Arguments
* 
* Fonction  
* Cette fonction verifie que l'indicateur XZSAV_InitOk
* est positionne a 0.
* Elle recupere le fichier d'administration du noyau
* de supervision (XDC_NSUP_PATHOPER/XDF_NSUP_ADMINIS) sur le serveur de
* donnees actif (primitive XZSE_19) et le transfere (XZSS_07)
* dans le meme repertoire sur la machine locale
* (primitive XZSC_07). 
* Puis elle s'abonne au datagroup XDG_NSUPDIFA. 
* Appel de la  fonction sx_adm_lecture pour lire le
* nom des fichiers liste, ainsi que les nombres
* d'elements maximal et reel. 
* 
* La fonction arme ensuite le callback
* XZSA_ActualiserAdmin sur reception du message 
* XDM_TSA_ADMI.
* 
* Positionnement de l'indicateur XZSAV_InitOk a 1.
*
* Description des fonctions internes :
*
* XZSA_ActualiserAdmin(	T_IPC_CONN			va_Cnx,
*			T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
*			T_CB_ARG			pa_Arg)
* Les arguments sont specifiques a un callback IPC. 
* Le callback libere les ressources d'administration
* (primitive XZSA_21) puis recupere les fichiers 
* d'administration du noyau de supervision sur le 
* serveur de donnees actif (primitive XZSE_19) et le 
* transfere dans le repertoire XDC_NSUP_PATHOPER de la machine
* locale (primitive XZSS_07). La fonction initialise
* alors les tables de donnees necessaires a
* l'administration systeme avec la fonction 
* XZSA_ActualiserListe.
*
* Codes retour 
* XDC_OK      ok.
* XZSAC_FIC_INV Erreur sur transfert des fichiers
*		d'administration du noyau de supervision
* XZSAC_INI_INV Erreur initialisation administration
*               deja effectuee.                 
*
* Mecanismes
* RTworks: RTS1 et RTS3.
******************************************************/
 
extern int XZSA_20InitAdmin	()
{
  /* Variables locales */
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_20InitAdmin";
  register int			vl_ValRet = XDC_OK,
  				cr;
  XZSXT_Entite*			pl_entiteCourante;
  XZSCT_Datagroup      		pl_NomDatagroup;
  char 				vl_nomFicSD [XDC_NOM_FIC_SIZE] = "";
 
	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSA_20InitAdmin IN \n");


  /* Verification de l'indicateur d'initialisation */
  if (XZSAV_InitOk != XDC_FAUX)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* Recuperation du nom du serveur actif */
  else if (XZSE_19NomSDActif (XZSAV_NomSDActif) != XDC_OK)
    {
      vl_ValRet = XDC_NOK;
    }  
  /* Recuperation du nom de la machine locale */
  else if (XZSC_07NomMachine (XZSAV_NomMachineLocale) != XDC_OK)
    {
      vl_ValRet = XDC_NOK;
    }
  /* Abonnement au datagroup XDG_NSUPDIFA_NomMachine */
  else if (!XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPDIFA,XZSAV_NomMachineLocale) )
    {
      vl_ValRet = XDC_NOK;
    }
  else if (!XDG_JoindreDG (pl_NomDatagroup))
   {
      vl_ValRet = XDC_NOK;
    }
  /* Armement du callback XZSA_ActualiserAdmin sur reception du message XDM_TSA_ADMIN */
  else if (!TipcSrvProcessCbCreate (XDM_IdentMsg (XDM_TSA_ADMI), 
			    (T_IPC_CONN_PROCESS_CB_FUNC) XZSA_ActualiserAdmin,
				    (T_CB_ARG) NULL))
    {
      vl_ValRet = XDC_NOK;
    }


  /* construction du nom absolu du fichier d'administration NSUPADMINIS */
  else if (sprintf (XZSAV_NomAbsoluFicAdm, "%s/%s", 
			XDC_NSUP_PATHOPER,  XDF_NSUP_ADMINIS) < 0)
    {
      vl_ValRet = XDC_NOK;
    }
    /** Verification si le fichier NSUPADMINIS existe **/
    else if (access (XZSAV_NomAbsoluFicAdm, F_OK) == 0)
    {
      /* Positionnement de XZSAV_InitOK a VRAI dans la fonction XZSA_Actualiser */
      XZSA_ActualiserAdmin (0, 0, 0);
    }
  /* Transfert de ce fichier dans le directory 
  *				XDC_NSUP_PATHOPER de la machine locale */
  else if (XZSS_07TransfertFichier (XZSAV_NomSDActif,  XDF_NSUP_ADMINIS, 
				XDC_NSUP_PATHOPER, XZSAV_NomMachineLocale, 
				XDF_NSUP_ADMINIS, XDC_NSUP_PATHOPER ) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_FATAL, 
		  "XZSA_InitAdmin: erreur transfert fichier %s/%s de %s sur %s",
			    XDC_NSUP_PATHOPER,
			    XDF_NSUP_ADMINIS,
			    XZSAV_NomSDActif,
			    XZSAV_NomMachineLocale);
      vl_ValRet = XDC_NOK;
    } 


  else 
    {
      /* Positionnement de XZSAV_InitOK a VRAI dans la fonction XZSA_Actualiser */
      XZSA_ActualiserAdmin (0, 0, 0);
    }
  
	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSA_20InitAdmin OUT : Code Retour \n", vl_ValRet);
      
  return (vl_ValRet);

}	/* fin XZSA_20InitAdmin **/

/*X*/
/* Fichier : 	      Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:55 $ 
****************************************************** 
* STERIA *  PROJET MIGRAZUR 
****************************************************** 
* MODULE XZADMIN 
****************************************************** 
* ALGORITHME : 
* 
* Arguments 
*
* Fonction 
* Cette fonction verifie que l'indicateur XZSAV_InitOk 
* est positionne a 1 et libere les ressources memoire 
* de l'administration par l'instruction  'free'. Les 
* quatre tables XZSAV_Listexxx sont liberees de cette 
* facon et les fichiers d'administration du noyau de 
* supervision sont detruits du repertoire "/tmp" de 
* la machine locale (primitive XZSS_06). 
* Positionnement de l'indicateur XZSAV_InitOk a 0.
* 
* Codes retour   
* XDC_OK      ok. 
* XZSAC_FIC_INV	Erreur sur destruction du fichier
* 		d'administration du noyau de
*		supervision.
* XZSAC_INI_INV Erreur initialisation administration 
*               (utiliser la fonction XZSA_20).   
* 
* Mecanismes 
* Pointeur de fonction.  
******************************************************/ 
 
extern int XZSA_21LibereRessourcesAdmin ()
{ 
  static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_21LibereRessourcesAdmin";
  XZSAT_ListeMachine *pl_machine, *pl_exMach;
  XZSAT_ListeTache *pl_tache, *pl_exTach;
  XZSAT_ListeSite *pl_site, *pl_exSite;
  XZSAT_ListeSSysteme *pl_ssysteme, *pl_exSSyst;
  XZSAT_ListeEntite *pl_entite, *pl_exEntit;
  XZSXT_Entite* pl_general, *pl_exGen;
  int 	vl_ValRet	= XDC_OK;


	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSA_21LibereRessourcesAdmin IN \n");

  if (XZSAV_ListeMachine != NULL) 
    {
      pl_machine = XZSAV_ListeMachine;
      while (pl_machine != NULL)
	{
	  pl_exMach = pl_machine;
	  pl_machine = pl_machine->suivant;
	  free (pl_exMach->machine);
	  free (pl_exMach);
	}
	/*B On remet le pointeur de debut de liste a NULL */
	XZSAV_ListeMachine = NULL;
    }
	
  if (XZSAV_ListeTache != NULL) 
    {
      pl_tache = XZSAV_ListeTache;
      while (pl_tache != NULL)
	{
	  pl_exTach = pl_tache;
	  pl_tache = pl_tache->suivant;
	  free (pl_exTach->tache);
	  free (pl_exTach);
	}
	/*B On remet le pointeur de debut de liste a NULL */
	XZSAV_ListeTache = NULL;
    }

  if (XZSAV_ListeSite != NULL) 
    {
      pl_site = XZSAV_ListeSite;
      while (pl_site != NULL)
	{
	  pl_exSite = pl_site;
	  pl_site = pl_site->suivant;
	  free (pl_exSite->site);
	  free (pl_exSite);
	}
	/*B On remet le pointeur de debut de liste a NULL */
	XZSAV_ListeSite = NULL;
    }

  if (XZSAV_ListeSSysteme != NULL) 
    {
      pl_ssysteme = XZSAV_ListeSSysteme;
      while (pl_ssysteme != NULL)
	{
	  pl_exSSyst = pl_ssysteme;
	  pl_ssysteme = pl_ssysteme->suivant;
	  free (pl_exSSyst->ssysteme);
	  free (pl_exSSyst);
	}
	/*B On remet le pointeur de debut de liste a NULL */
	XZSAV_ListeSSysteme = NULL;
    }

  if (XZSAV_ListeEntite != NULL) 
    {
      pl_entite = XZSAV_ListeEntite;
      while (pl_entite != NULL)
	{
	  pl_exEntit = pl_entite;
	  pl_entite = pl_entite->suivant;
	  free (pl_exEntit->entite);
	  free (pl_exEntit);
	}
	/*B On remet le pointeur de debut de liste a NULL */
	XZSAV_ListeEntite = NULL;
    }

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSA_21LibereRessourcesAdmin OUT : Code Retour %d \n", 
			vl_ValRet);

  return(vl_ValRet); 

}	/* Fin XZSA_21LibereRessourcesAdmin */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'executer une fonction utile passee en argument
*	en parcourant la liste des Entites d'un Site
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZSA_29ListeEntiteSite(
	XZSCT_NomSite	va_NomSite,
	int		(*pa_FonctionUtil)(),
	int		*pa_NbElem,
	void           *pa_Args)

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite : Nom du Site
*   *pa_FonctionUtil : Fonction Utilisateur a executer
*   pa_Args : Arguments de la fonction Utile
*   
*
* ARGUMENTS EN SORTIE :
*   pa_NbElem : Nombre d'entite sur le Site
*
*
* CODE RETOUR : 
* XDC_OK      ok. 
* XZSAC_NOA_INV Nom du site inconnu.
* XZSAC_INI_INV Erreur initialisation administration 
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
* Cette fonction verifie que le module est initialise
* (XZSAV_InitOk = 1) et l'existence du site est teste
* avec la primitive XZSA_22.
* Recherche du numero d'ordre du site dans le tableau
* XZAV_ListeSite.
* Pour chaque element du tableau XZAV_ListeEntite 
* pour le numero d'ordre du site donne
*   - Application de la fonction utilisateur passee
*     en argument.
* Fpour
*
* L'appel de la fonction utilisateur est le suivant : 
* FonctionUtil ((IN)(XZSCT_NomMachine)  NomMachine)
*               (IN)(int)               NumOrdre,
*               (OUT)(void *)           Args)
* 
* Cette fonction retourne aussi le nombre d'elements 
* du tableau reellement traites. 
* 
*
------------------------------------------------------*/
{
  	static char*			version=" $Id: xzsa.c,v 1.16 2020/11/03 17:57:55 pc2dpdy Exp $ :  XZSA_29ListeEntiteSite";
  	register int 			vl_ValRet = XDC_OK;
  	XZSAT_ListeEntite*		pl_listeEntite;



	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_29ListeEntiteSite : IN \
		-- Site  %s \n",va_NomSite);


  if (XZSAV_InitOk != XDC_VRAI)
    {
      vl_ValRet = XZSAC_INI_INV;
    }
  /* test de l'existence du site */
  else if (XZSA_22ExisterSite (va_NomSite) != XDC_OK)
    {
      vl_ValRet = XZSAC_NOA_INV;
    }
  else
    {
      *pa_NbElem = 0;

      for (pl_listeEntite = XZSAV_ListeEntite; 
	   pl_listeEntite != NULL; 
	   pl_listeEntite = pl_listeEntite->suivant)
	{
	  if  ( strcmp( pl_listeEntite->entite->NomSite, va_NomSite ) == 0 )
	  {
	     if (pa_FonctionUtil != NULL)
	     {
	       (*pa_FonctionUtil) (pl_listeEntite->entite->NomEntite, 
	         		        *pa_NbElem, 
	         		        pa_Args);
             }
	     ++(*pa_NbElem);
	     XZST_03EcritureTrace (XZSTC_INFO, 
		"XZSA29ListeEntiteSite, Application de la fonction user sur la machine %s du Site %s",
				pl_listeEntite->entite->NomEntite,
				pl_listeEntite->entite->NomSite);
	   }		
	}
    } 

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSA_29ListeEntiteSite : OUT \
		-- Code Retour %d \n",vl_ValRet);

  	return (vl_ValRet);

}	/* Fin XZSA_29ListeEntiteSite */



/* fin de fichier */
