/*E*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSXDON * FICHIER xzsx.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSXDON 
*        - Declaration des fonctions externes
*        - Declaration des variables externes
*
*   Reference DCG-021 chapitre ??? 
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville	24 Aou 1994      	: Creation
*  T.Milleville	15 Fev 1995      	: Modification du test de presence 
*				d'une tache dans le fichier NSUP_NomMachine
*				dans la  fonction XZSX_02   V1.3
*  T.Milleville	25 Jul 1995      	: Ajout de commentaires V1.14
*  Niepceron 	12 Fev 1996	: Caste du pl_P v1.15
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : 'return(Ok)' manquants; format champs de 'scanf'  1.16
* LCL		13/07/21 : Fix memory leak liberation sur parcours de liste dans XZSX_OublieFichier 1.17
*******************************************************/

#include <errno.h>
#include <sys/stat.h>

#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include <xdg.h>
#include <xdm.h>

#include <xzsc.h> 
#include <xzst.h>
#include <xzsm.h>
#include "xzsx.h"

 XZSXT_Entite *		XZSXV_pgNSUP = (XZSXT_Entite *)NULL ;
 XZSXT_Entite *		pg_PCourant ;


 extern int XZSX_Commentaire(int,void *,char *) ;
 extern int XZSX_InfoMachine(int,void *,char *) ;
 extern int XZSX_InfoTache(int,void *,char *) ;
 extern int XZSX_Site(int,void *,char *) ;
 extern int XZSX_Machine	(int,void *,char *) ;
 extern int XZSX_Tache(int,void *,char *) ;
 extern int XZSX_SousSysteme(int,void *,char *) ;
 extern int XZSX_Entite(int,void *,char *) ;
 extern int XZSX_Journal(int,void *,char *) ;
 extern int XZSX_Timer(int,void *,char *) ;

 typedef struct
 {
	 char		Cle[20] ;
	 int 		(* Traite)(int,void *,char *) ;
	 int		Type ;
	 int		Taille ;

 } XZSXT_GesEntite ;

 XZSXT_GesEntite	tg_ActionEntite[] =
 {
   {"#", 		XZSX_Commentaire, 	XZSXC_COMMENTAIRE, 	XZSXC_LONG},
   {"InfoMachine", 	XZSX_InfoMachine, 	XZSXC_INFOMACHINE,	sizeof(XZSXT_Machine)},
   {"InfoTache",	XZSX_InfoTache,		XZSXC_INFOTACHE,	sizeof(XZSXT_Tache)},
   {"InfoSite",		XZSX_Site,		XZSXC_INFOSITE,		sizeof(XZSXT_ListeSite)},
   {"Machine",		XZSX_Machine,		XZSXC_MACHINE,		sizeof(XZSXT_ListeMachine)},
   {"Tache",		XZSX_Tache,		XZSXC_TACHE,		sizeof(XZSXT_ListeTache)},
   {"SousSysteme",	XZSX_SousSysteme,	XZSXC_SS,		sizeof(XZSXT_ListeSSysteme)},
   {"Entite",		XZSX_Entite,		XZSXC_ENTITE,		sizeof(XZSXT_ListeEntite)},
   {"Journal",		XZSX_Journal,		XZSXC_JOURNAL,		sizeof(XZSXT_TailleJourAdm)},
   {"Timer",		XZSX_Timer,		XZSXC_TIMER,		sizeof(XZSXT_ListeTimer)},
 };


static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX ";

static int vg_nb_appels=0;


/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Recupere la grappe lue dans XZSX_LitFichier
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
extern int XZSX_RecupereGrappe (XZSXT_Entite** pa_grappe)
{
  /* Donnees locales */
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_RecupereGrappe ";

  *pa_grappe = (XZSXT_Entite*) XZSXV_pgNSUP;
  return (XDC_OK);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour le pointeur de grappe
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZSX_MajGrappe(
		XZSXT_Entite* pa_grappe)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Affectation de la valeur du pointeur
*
------------------------------------------------------*/
{

  /* Donnees locales */
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_MajGrappe ";

  XZSXV_pgNSUP = pa_grappe;
  return (XDC_OK);

}	/* XZSX_MajGrappe */



 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregitrement du journal
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 int			XZSX_Journal(
 int				   va_Action,
 void *				   pa_Data,
 char *				   pa_Chaine
 )
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Journal ";
	 XZSXT_TailleJourAdm *		pl_Info = (XZSXT_TailleJourAdm *)pa_Data ;

	 /* si encodage */
	 if (va_Action == XZSXC_ENCODAGE)
	 {
		 sprintf(pa_Chaine,"%ld",pl_Info->TailleMax) ;
	 }
	 /* sinon decodage */
	 else
	 {
		 if (sscanf(pa_Chaine,"%ld",&(pl_Info->TailleMax)) == 0) return(XDC_NOK) ;
	 }
	 return(XDC_OK) ;
 }


 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregistrement entite
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 int			XZSX_Entite(
 int				   va_Action,
 void *				   pa_Data,
 char *				   pa_Chaine
 )
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Entite ";
	 XZSXT_ListeEntite *		pl_Info = (XZSXT_ListeEntite *)pa_Data ;

	 /* si encodage */
	 if (va_Action == XZSXC_ENCODAGE)
	 {
		 sprintf(pa_Chaine,"%s %d %s"
		     ,pl_Info->NomEntite
		     ,pl_Info->TypeEntite
		     ,pl_Info->NomSite) ;
	 }
	 /* sinon decodage */
	 else
	 {
		 if (sscanf(pa_Chaine,"%s %d %s"
		     ,pl_Info->NomEntite
		     ,&(pl_Info->TypeEntite)
		     ,pl_Info->NomSite) != 3) return(XDC_NOK) ;
	 }
	 return(XDC_OK) ;
 }

 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregitrement sous-systeme
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 int			XZSX_SousSysteme(
 int					va_Action,
 void *					pa_Data,
 char *					pa_Chaine
 )
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_SousSysteme ";
   XZSXT_ListeSSysteme *		pl_Info = (XZSXT_ListeSSysteme *)pa_Data ;

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
       sprintf(pa_Chaine,"%s %d"
	       ,pl_Info->NomSSysteme
	       ,pl_Info->TypeMachine) ;
     }
   /* sinon decodage */
   else
     {
       if (sscanf(pa_Chaine,"%s %d"
		  ,pl_Info->NomSSysteme
		  ,&(pl_Info->TypeMachine)) != 2) return(XDC_NOK) ;
     }
   return(XDC_OK) ;
 }



/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de coder ou decoder une struture TACHE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 int	XZSX_Tache(
	    int		va_Action,
	    void *  	pa_Data,
	    char *  	pa_Chaine)

/*
* ARGUMENTS EN ENTREE :
*   va_Action :  Codage ou Decodage 
*   pa_Data : donnees a traiter
*   pa_chaine : donnees a etourner 
*
* ARGUMENTS EN SORTIE :
*	Code de retour
*
* CODE RETOUR : 
*	XDC_OK
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Test l'action a effectuer, puis transforme
*	les donnees.
*
------------------------------------------------------*/
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Tache ";
   XZSXT_ListeTache *	pl_Info = (XZSXT_ListeTache *)pa_Data ;

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
       sprintf( pa_Chaine,"%s %s %d %d",
			 pl_Info->NomTache,
			 pl_Info->NomSSysteme,
			 pl_Info->TypeMachine,
			 pl_Info->Temporaire);
     }
   /* sinon decodage */
   else
     {
       if ( sscanf( pa_Chaine,"%s %s %d %d",
			 pl_Info->NomTache,
			 pl_Info->NomSSysteme,
			 &(pl_Info->TypeMachine),
			 &(pl_Info->Temporaire) ) != 4)
		
		return(XDC_NOK) ;
     }
   return(XDC_OK) ;

 } 	/* fin XZSX_Tache */


/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregistrement machine
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 int			XZSX_Machine (int				    va_Action,
				      void *				    pa_Data,
				      char *				    pa_Chaine)
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Machine ";
   XZSXT_ListeMachine *		pl_Info = (XZSXT_ListeMachine *)pa_Data ;

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
	       /*,pl_Info->NomMachineTra*/
       sprintf(pa_Chaine
	       ,"%s %d %s %s"
	       ,pl_Info->NomMachine
	       ,pl_Info->TypeMachine
	       ,pl_Info->NomMachineImp
	       ,pl_Info->NomSite
	       ) ;
     }
   /* sinon decodage */
   else
     {
		  /*,pl_Info->NomMachineTra*/
       if (sscanf(pa_Chaine
		  ,"%s %d %s %s"
		  ,pl_Info->NomMachine
		  ,&(pl_Info->TypeMachine)
		  ,pl_Info->NomMachineImp
		  ,pl_Info->NomSite) != 4)

	 return(XDC_NOK) ;
     }
   return(XDC_OK) ;
 }

 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregitrement site
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

 int			XZSX_Site (int				 va_Action,
				   void *			 pa_Data,
				   char *			 pa_Chaine)
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Site ";
   XZSXT_ListeSite *		pl_Info = (XZSXT_ListeSite *)pa_Data ;

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
       sprintf(pa_Chaine,"%s",pl_Info->NomSite) ;
     }
   /* sinon decodage */
   else
     {
       if (sscanf(pa_Chaine,"%s",pl_Info->NomSite) != 1)

	 return(XDC_NOK) ;
     }
   return(XDC_OK) ;
 }

 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregitrement commentaire
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 int			XZSX_Commentaire(
					  int					va_Action,
					  void *					pa_Data,
					  char *					pa_Chaine
					  )
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Commentaire ";
   if (pa_Chaine == NULL) return(XDC_NOK) ;

   if (va_Action == XZSXC_ENCODAGE)     strcpy(pa_Chaine,(char *)pa_Data) ;
   else strcpy((char *)pa_Data,pa_Chaine) ;
   return(XDC_OK) ;
 }

 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregitrement info_machine
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
int	XZSX_InfoMachine(
		int	va_Action,
		void *	pa_Data,
		char *	pa_Chaine)

 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_InfoMachine ";

	#define	vl_NbChampsMac	13	
   	XZSXT_Machine *		pl_Info = (XZSXT_Machine *)pa_Data ;

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
       sprintf(pa_Chaine
	       ,"%s %s %s %s %s %s %s %u %u %u %u %u %u"
	       ,pl_Info->NomMachine
	       ,pl_Info->NomPeriphSR
	       ,pl_Info->NomMachineImp
	       ,pl_Info->NomImprimante
	       ,pl_Info->NomImprimanteSec
	       ,pl_Info->NomSite
	       ,pl_Info->PathTrace
	       ,pl_Info->MaxTailleFicTrace
	       ,pl_Info->PeriodeTestReseau
	       ,pl_Info->TypeMachine
	       ,pl_Info->Priorite
	       ,pl_Info->TypeProfil
	       ,pl_Info->DureeAbsenceCourte
	       ) ;
     }
   /* sinon decodage */
   else
     {
       if (sscanf(pa_Chaine
		  ,"%s %s %s %s %s %s %s %u %u %u %u %u %u"
		  ,pl_Info->NomMachine
		  ,pl_Info->NomPeriphSR
		  ,pl_Info->NomMachineImp
		  ,pl_Info->NomImprimante
		  ,pl_Info->NomImprimanteSec
		  ,pl_Info->NomSite
		  ,pl_Info->PathTrace
		  ,&(pl_Info->MaxTailleFicTrace)
		  ,&(pl_Info->PeriodeTestReseau)
		  ,&(pl_Info->TypeMachine)
		  ,&(pl_Info->Priorite)
		  ,&(pl_Info->TypeProfil)
		  ,&(pl_Info->DureeAbsenceCourte)) != vl_NbChampsMac)

	 return(XDC_NOK) ;
     }
   return(XDC_OK) ;
 }

 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Encode et Decode un enregitrement info_tache
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 int			XZSX_InfoTache(
					int				      va_Action,
					void *				      pa_Data,
					char *				      pa_Chaine
 )
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_InfoTache ";
   XZSXT_Tache *		pl_Info = (XZSXT_Tache *)pa_Data ;

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
       sprintf(pa_Chaine
	       ,"%s %s %s %u"
	       ,pl_Info->NomTache
	       ,pl_Info->ListeServer
	       ,pl_Info->ListeLicense
	       ,pl_Info->TailleFicTrace
	       ) ;
     }
   /* sinon decodage */
   else
     {
       if (sscanf(pa_Chaine
		  ,"%s %s %s %u"
		  ,pl_Info->NomTache
		  ,pl_Info->ListeServer
		  ,pl_Info->ListeLicense
		  ,&(pl_Info->TailleFicTrace)
		  )
	   != 4
	   )
	 return(XDC_NOK) ;
     }
   return(XDC_OK) ;
 }

 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Cre et ajoute en fin de grappe une entite
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
 static int	XZSX_InsereEntite (int		 va_Type,
				    void*	 pa_Data)
 /*
 * PARAMETRES EN ENTREE :
 *
 *	va_Type		: Type de l'entite
 *	pa_Data		: Donnees de l'entite
 *
 * VALEUR(S) DE RETOUR : 
 *
 *	XDC_OK		: Avec succes
 *	XDC_NOK		: Plus de memoire
 *
 * CONDITION D'ERREUR :
 *
 *	Plus de memoire
 *
 ******************************************************/
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_InsereEntite ";
  XZSXT_Entite *		pl_P=NULL ;
  
  /* si l'allocation de l'espace pour une entite est correcte */
  if ((pl_P = (XZSXT_Entite *) malloc(sizeof(XZSXT_Entite))) != NULL)
    {
      /* valorise cette entite */
      pl_P->Type    = va_Type ;
      pl_P->Data    = pa_Data ;
      pl_P->Suivant = NULL ;
      
      /* insere cette entite dans la grappe */
      if (XZSXV_pgNSUP == NULL)     
	XZSXV_pgNSUP              = pl_P ;
      else 
	pg_PCourant->Suivant = pl_P ;
      
      pg_PCourant          = pl_P ;
       
      return(XDC_OK) ;
    }
   else return(XDC_NOK) ;
}



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Cree une entite soit a partir d'une chaine de
 *	caractere qu'il faudra decoder.
 *	Soit a partir d'un structure de meme type qu'il
 *	faudra copier.
 *
 *	Cette fonction cree l'entite et le lien de celle
 *	ci dans la grappe.
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

 int		XZSX_CreeEntite (char *			       pa_Cle,
				 int			       va_Action,
				 char *			       pa_A_Decoder,
				 void *			       pa_Data)

 /*
 * PARAMETRES EN ENTREE :
 *
 *	pa_Cle		: Chaine de caractere
 *
 * PARAMETRES EN SORTIE :
 *
 *
 * VALEUR(S) DE RETOUR : < commentaire sur le resultat >
 *
 * CONDITION D'ERREUR :
 *
 *
 * CONDITIONS LIMITES :
 *
 *
 ******************************************************/
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_CreeEntite ";
   int			vl_Entite ;
   void *		pl_Data ;

	if (pa_A_Decoder==NULL)
		return(XDC_OK);

   /* pour chaque entite connue */
   for (vl_Entite = 0 ; vl_Entite < sizeof(tg_ActionEntite)/sizeof(*tg_ActionEntite) ; vl_Entite += 1)
     {
       /* si l'entite est localise */
       if (!strcasecmp(pa_Cle,tg_ActionEntite[vl_Entite].Cle))
	 {
	   /* si l'allocation des espaces memoire necessaire se deroule bien */
	   if ((pl_Data = malloc(tg_ActionEntite[vl_Entite].Taille)) != NULL)
	     {
	       /* selon ce qu'il y a a faire */
	       switch(va_Action)
		 {
		   /* decodage */
		 case XZSXC_DECODAGE :

		   /* la conversion se deroule bien */
		   if ((tg_ActionEntite[vl_Entite].Traite)(XZSXC_DECODAGE,pl_Data,pa_A_Decoder) == XDC_OK)
		     {
		       /* insere ce descripteur */
		       if (XZSX_InsereEntite(tg_ActionEntite[vl_Entite].Type,pl_Data) != XDC_OK)
			 {
			   free(pl_Data) ;
			   return(XDC_NOK) ;
			 }
		     }
		   /* sinon libere la place memoire */
		   else {
		     free(pl_Data) ;
		     return(XDC_NOK) ;
		   }
		   break ;

		   /* copie enregistrement */
		 default :

		   memcpy(pl_Data,pa_Data,tg_ActionEntite[vl_Entite].Taille) ;

		   /* insere ce descripteur */
		   if (XZSX_InsereEntite(tg_ActionEntite[vl_Entite].Type,pl_Data) != XDC_OK)
		     {
		       free(pl_Data) ;
		       return(XDC_NOK) ;
		     }
		   break ;
		 }
	     }
	   /* stoppe la recherche */
	   break ;
	 }
     }
   /* retourne */
   return(vl_Entite = sizeof(tg_ActionEntite)/sizeof(*tg_ActionEntite) ? XDC_NOK:XDC_OK) ;
 }




/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int	XZSX_LitFichier (
	char *	pa_NomFichier)
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
 * CONDITIONS LIMITES :
 *
 ******************************************************/
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_LitFichier ";
   FILE *	pl_F 		= NULL;
   char *	pl_Descripteur 	= NULL;
   char *	pl_Dummy 	= NULL;
   int		vl_ValRet 	= XDC_OK;
   char 	tl_Donnee[XZSXC_LONG];

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSX_LitFichier : IN \
		-- Nom Fichier %s",pa_NomFichier);

vg_nb_appels++;

   /* si l'ouverture du fichier ne pose pas de probleme */
   if ((pl_F = fopen(pa_NomFichier,"r")) != NULL)
   {
       /* tant qu'il y a de la donnee */
       for (;fgets(tl_Donnee,sizeof(tl_Donnee),pl_F) != NULL;)
	 {
	   /* si il existe un membre descripteur d'entite */
	   if ((pl_Descripteur = strtok(tl_Donnee," \t")) != NULL)
	     {
	       if ((pl_Dummy = strrchr(pl_Descripteur,'\n')) != NULL) *pl_Dummy='\0' ;

	       /* Cree et insere l'entite */
	       XZSX_CreeEntite(pl_Descripteur,XZSXC_DECODAGE,strtok(NULL,""),NULL) ;
	     }
	 }
       /* ferme le fichier */
       fclose(pl_F) ;
   
	/* Avec succes */
   	vl_ValRet = XDC_OK;
   }
   else
   {
	if (access(pa_NomFichier,F_OK) != 0) 
     	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSX_LitFichier: -- Fichier inexistant %s\n",
					pa_NomFichier);
		vl_ValRet =  XZSXC_FIC_INV;
     	}	
     	else
     	{
		XZST_03EcritureTrace(XZSTC_WARNING,
	  	"XZSX_LitFichier: Probleme ouverture fichier %s\n ", 
								pa_NomFichier);
		vl_ValRet = XDC_NOK;
	}
   }

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSX_LitFichier : OUT \
		-- Code retour %d",vl_ValRet);
//printf("tsaplg-XZSX_LitFichier : OUT \n");
	return (vl_ValRet);

}	/* Fin XZSX_LitFichier */



/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int	XZSX_OublieFichier(void)
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_OublieFichier ";
   XZSXT_Entite *		pl_P ;
   XZSXT_Entite *		pl_ExP ;

   /*
   for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; 
	pl_ExP = pl_P,free(pl_P->Data),pl_P = pl_P->Suivant,free(pl_ExP)) ;*/

   pl_P = XZSXV_pgNSUP;
   while (pl_P != NULL) {
	pl_ExP = pl_P;
	free(pl_P->Data);
	pl_P = pl_P->Suivant;
	free(pl_ExP);
   }

   XZSXV_pgNSUP = NULL ;

   return(XDC_OK);
 }



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_EcritFichier (char * pa_Fichier)
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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_EcritFichier ";
   FILE *		pl_F ;
   XZSXT_Entite *	pl_P ;
   XZSXT_Entite *       pl_ExP ;
   char			tl_Encode[XZSXC_LONG] ;
   int			vl_I ;

   /* si ouverture OK */
   if ((pl_F = fopen(pa_Fichier,"w")) != NULL)
     {
       /* pour toutes les entites connues */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_ExP = pl_P,free(pl_P->Data),pl_P = pl_P->Suivant,free(pl_ExP))
	 {
	   /* recherche les caracteristiques de l'entite */
	   for (vl_I = 0 ; tg_ActionEntite[vl_I].Type != pl_P->Type ; vl_I++) ;

	   /* encodage des donnees */
	   tg_ActionEntite[vl_I].Traite(XZSXC_ENCODAGE,pl_P->Data,tl_Encode) ;

	   /* enregistrement dans le fichier */
	   fprintf(pl_F,"%s %s\n",tg_ActionEntite[vl_I].Cle,tl_Encode) ;
	 }
       /* plus d'entite */
       XZSXV_pgNSUP = NULL ;

       /* ferme le fichier */
       fclose(pl_F) ;

       return(XDC_OK) ;
     }
   else 
     return(XDC_NOK) ;
 }


/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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
 * XDC_NOK Nom machine invalide (fichier NSUP_NomMachine 
 *         non trouve) ou Erreur lecture fichier NSUP_NomMachine
 * XDC_OK  ok
 * XDC_FIC_INV  Fichier n'existe pas
 *
 * Mecanismes
 *
 ******************************************************/

int	XZSX_01MachineLecture (
		XZSCT_NomMachine     pa_NomMachine,
		char *		     pa_PathFichier,
		XZSXT_Machine *	     pa_Machine)
 {
   static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_01MachineLecture";

   int 			vl_ValRet = XDC_OK;
		/* PathName du fichier NSUP_NomMachine */
   char			pl_PathName[XZSXC_LONG]; 
   XZSXT_Entite *	pl_P ;

   XZST_03EcritureTrace(XZSTC_FONCTION
		,"XZSX_01MachineLecture(Machine=%s,Rep Fichier=%s,)"
			,pa_NomMachine ,pa_PathFichier) ;

   /* Construction du pathname du fichier NSUP_NomMachine*/
   sprintf(pl_PathName,"%s/%s_%s",pa_PathFichier,XDF_NSUP,pa_NomMachine);

   /* si la lecture du fichier ne s'est pas correctement deroule */
   if ( (vl_ValRet = XZSX_LitFichier(pl_PathName)) != XDC_OK)
   {
	/*if (access(pl_PathName,F_OK) != 0) */
	if (vl_ValRet == XZSXC_FIC_INV)
     	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSX_01MachineLecture: -- Fichier inexistant %s\n"						,pl_PathName);
		return (XZSXC_FIC_INV );
     	}	
     	else
     	{
		XZST_03EcritureTrace(XZSTC_WARNING,
	  	"XZSX_01MachineLecture: Probleme ouverture fichier %s\n ", 
								pl_PathName);
		return(XDC_NOK) ;
     	}
   }
   else
   {
       /* pour toutes les entites de la grappe */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	 {
	   /* si il s'agit d'une entite descripteur de machine */
	   if (pl_P->Type == XZSXC_INFOMACHINE)
	     {
	       /* si il s'agit de la machine recherchee */
	       if (!strcmp(((XZSXT_Machine *)(pl_P->Data))->NomMachine,pa_NomMachine))
		 {
		   /* recupere les infomations */
		   memcpy(pa_Machine,pl_P->Data,sizeof(*pa_Machine)) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
       /* si pas d'info machine */
       if (pl_P == NULL)
	 {
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_01MachineLecture:Le fichier %s ne possede pas de donnees",
				pl_PathName) ;
	   vl_ValRet = XDC_NOK ;
	 }
       /* oublie */
       XZSX_OublieFichier() ;
     }
   /* Bye */
   return(vl_ValRet) ;
 }



 
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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
 * XDC_NOK Nom machine invalide (fichier NSUP_NomMachine 
 *         non trouve) ou Erreur lecture fichier NSUP_NomMachine
 * XZSXC_NOM_INV Tache inexistante
 * XDC_OK  ok
 *
 * Mecanismes
 *
 ******************************************************/

 int	XZSX_02TacheLecture(
 		XZSCT_NomMachine	pa_NomMachine,
 		XZSCT_NomTache 	   	pa_NomTache,
 		char *		   	pa_PathFichier,
 		XZSXT_Tache *	   	pa_Tache)
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_02TacheLecture";

	int 		vl_ValRet = XDC_OK;
		/* PathName du fichier NSUP_NomMachine */
	char 		pl_PathName[500]	= "";      
	XZSXT_Entite *	pl_P 		= NULL;
	int		vl_IndPre	= XDC_FAUX;

	 
	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSX_02TacheLecture IN : Nom Mach : [%s] Nom Tache : [%s] Fichier : [%s]",pa_NomMachine,pa_NomTache,pa_PathFichier);

	 /* Construction du pathname du fichier NSUP_NomMachine*/
	 sprintf(pl_PathName,"%s/%s_%s",pa_PathFichier,XDF_NSUP,pa_NomMachine);

	 /* si la lecture du fichier s'est deroule sans aucun probleme */
	 if (XZSX_LitFichier(pl_PathName) == XDC_OK)
	 {
		 /* pour toutes les entites du fichier */
		 for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
		 {
			 /* si l'entite est un descripteur de tache */
			 if (pl_P->Type == XZSXC_INFOTACHE)
			 {
				 /* si ce descripteur est celui attendu */
				 if (!strcmp(((XZSXT_Tache *)(pl_P->Data))->NomTache,pa_NomTache))
				 {
				 XZST_03EcritureTrace(XZSTC_DEBUG1,"Memorisation des donnees de la tache %s",pa_NomTache);

					 /* recupere les informations tache */
					 memcpy(pa_Tache,pl_P->Data,sizeof(*pa_Tache)) ;

					 /*B Memorisation Indicateur de presence 					*	de la Tache */
					 vl_IndPre = XDC_VRAI;

					 /* stoppe la recherche */
					 break ;
				 }
			 }
		 }
		 /* si aucune information pour cette tache */
		 if ( (pl_P == NULL) || (vl_IndPre == XDC_FAUX) )
		 {
			 /* signale le */
			 XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSX_02TacheLecture : La tache %s n'a pas ete localisee",pa_NomTache) ;

			 vl_ValRet = XZSXC_NOM_INV ;
		 }
		 /* libere les ressources */
		 XZSX_OublieFichier() ;
	 }
	 else
	 {
		 vl_ValRet = XDC_NOK ;
	 }

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSX_02TacheLecture : OUT \n");

	 return(vl_ValRet);

} 	/* Fin XZSX_02TacheLecture */



/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	ERnregistre les caracteristiques d'une machine
 *	dans un fichier donne
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

int	XZSX_03Machine_Ecriture(
		char *		pa_Repertoire,
		XZSXT_Machine	va_Machine)

 /*
 * PARAMETRES EN ENTREE :
 *
 *	pa_Repertoire : Repertoire du fichier 
 *	va_Machine : Description du noyau de supervision
 *
 ******************************************************/
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_03Machine_Ecriture ";
	 char			tl_NomFichier[XZSXC_LONG] ;
	 XZSXT_Entite *		pl_P ;

	 /* construction du nom du fichier de config a creer */
	 sprintf(tl_NomFichier,"%s/%s_%s",
			pa_Repertoire,XDF_NSUP,va_Machine.NomMachine) ;

	 /* si la lecture pose probleme */
	 if (XZSX_LitFichier(tl_NomFichier) == XDC_NOK)
	 {
		XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSX_03Machine_Ecriture:XZSX_LitFichier") ;
		return(XDC_NOK) ;
	 }
	 else
	 {
		 /*  pour toutes les entites connues */
		 for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)

			 /* si l'entite courante est une information machine */
			 if (pl_P->Type == XZSXC_INFOMACHINE)
			 {
				 /* endosse les nouvelles informations */
				 memcpy((XZSXT_Machine *)(pl_P->Data),
						&va_Machine,sizeof(va_Machine)) ;

				 /* stoppe la recherche */
				 break ;
			 }
	 	/* si pas d'information machine */
	 	if (pl_P == NULL)
	 	{
			 /* cree cette entite info machine */
			 XZSX_CreeEntite("InfoMachine",
					XZSXC_COPIE,NULL,&va_Machine) ;
	 	}

	        /* Cree ou reecrit le fichier */
	        XZSX_EcritFichier(tl_NomFichier) ;

	        /* Bye */
	        return(XDC_OK) ;
	 }
 }

/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_04Tache_Ecriture(
 XZSCT_NomMachine			     va_NomMachine,
 XZSXT_Tache				     va_Tache,
 char *					     pa_Repertoire
 )

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_04Tache_Ecriture ";
	 char			vl_NomFichier[XZSXC_LONG] ;
	 XZSXT_Entite *		pl_P ;

	 /* construction du nom du fichier de config */
	 sprintf(vl_NomFichier,"%s/%s_%s",pa_Repertoire,XDF_NSUP,va_NomMachine) ;

	 /* si probleme d'ouverture du fichier de config */
	 if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
	 {
		 XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_04Tache_Ecriture:XZSX_LitFichier") ;
		 return(XDC_NOK) ;
	 }
	 else
	 {
		 /* pour toutes les entites taches connues */
		 for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
		 {
			 /* si l'entite est de type description de tache */
			 if (pl_P->Type == XZSXC_INFOTACHE)
			 {
				 /* si il y s'agit de la tache recherchee */
				 if (!strcmp(((XZSXT_Tache *)(pl_P->Data))->NomTache,va_Tache.NomTache))
				 {
					 /* ecrase les info existantes */
					 memcpy(((XZSXT_Tache *)(pl_P->Data)),&va_Tache,sizeof(va_Tache)) ;

					 /* stoppe la recherche */
					 break ;
				 }
			 }
		 }
		 /* si aucune tache de ce nom localisee */
		 if (pl_P == NULL)
		 {
			 /* cree cette entite */
			 XZSX_CreeEntite("InfoTache",XZSXC_COPIE,NULL,&va_Tache) ;
		 }

	 /* reecrit le fichier de config */
	 XZSX_EcritFichier(vl_NomFichier) ;

	 return( XDC_OK );
	 }
 }
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_05Tache_Detruire(
 XZSCT_NomMachine			     va_NomMachine,
 XZSCT_NomTache				     va_NomTache,
 char *					     pa_Repertoire
 )

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_05Tache_Detruire ";
	 char		vl_NomFichier[XZSXC_LONG] ;
	 XZSXT_Entite *	pl_P ;
	 XZSXT_Entite *	pl_ExP ;

	 /* construction du nom du fichier cible */
	 sprintf(vl_NomFichier,"%s/%s_%s",pa_Repertoire,XDF_NSUP,va_NomMachine) ;

	 /* si la lecture du fichier s'est correctement deroulee */
	 if (XZSX_LitFichier(vl_NomFichier) == XDC_OK)
	 {
		 /* pour toutes les taches connues */
		 for ( pl_P = pl_ExP = XZSXV_pgNSUP
		     ; pl_P != NULL
		     ; pl_ExP = pl_P,pl_P = pl_P->Suivant
		     )
		 {
			 /* si l'entite est une tache */
			 if (pl_P->Type == XZSXC_INFOTACHE)
			 {
				 /* si il s'agit de la tache recherchee */
				 if (!strcmp(((XZSXT_Tache *)(pl_P->Data))->NomTache,va_NomTache))
				 {
					 if (pl_P == XZSXV_pgNSUP)      {
						 pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
					 }
					 else pl_ExP->Suivant = pl_P->Suivant ;

					 /* libere la place memoire */
					 free(pl_P->Data) ;
					 free(pl_P) ;

					 /* stoppe la recherche */
					 break ;
				 }
			 }
		 }
		 /* reecrit le fichier */
		 XZSX_EcritFichier(vl_NomFichier) ;

		 /* avec succes */
		 return(XDC_OK) ;
	 }
	 /* sinon signale le */
	 else
	 {
	    XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_05Tache_Detruire : pas de fichier correspondant a la machine %s",
		     va_NomMachine) ;
	    return(XDC_NOK) ;
	 }
 }
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_06ExisteSite(
 XZSCT_NomSite			      va_NomSite,
 char *				      pa_Repertoire
 )

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
 * XDC_OK Si le nom de Site Existe
 * XDC_NOK sinon
 *
 * CONDITIONS LIMITES :
 *
 * < commentaire precisant le comportement de la fonction
 * en cas de fourniture de donnees hors du domaine de
 * validite prevu >
 *
 ******************************************************/
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_06ExisteSite ";
	 XZSXT_Entite *		pl_P ;
	 char 			tl_NomFichier[XZSXC_LONG] ;

	 XZST_03EcritureTrace(XZSTC_FONCTION,"XZSX_06ExisteSite(%s,%s)",va_NomSite,pa_Repertoire) ;

	 /* construction du nom du fichier cible */
	 sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSITE) ;

	 /* si le decodage du fichier s'est correctement deroule */
	 if (XZSX_LitFichier(tl_NomFichier) == XDC_OK)
	 {

		 /* pour toutes les entites decodees */
		 for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
		 {
			 /* si l'entite est de type site */
			 if (pl_P->Type == XZSXC_INFOSITE)
			 {
				 /* si il s'agit du site recherche */
				 if (!strcmp(((XZSXT_ListeSite *)(pl_P->Data))->NomSite,va_NomSite))
				 {
					 /* stoppe la recherche */
					 break ;
				 }
			 }
		 }
		 /* libere les ressources */
		 XZSX_OublieFichier() ;

		 /* si aucun site ne correspond a celui demande */
		 if (pl_P == NULL)
		 {
			 /* informe et abandonne */
			 XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_06ExisteSite:Le site %s n'a pas ete localise dans le fichier %s",
			     va_NomSite,tl_NomFichier) ;
			 return(XDC_NOK) ;
		 }
	 }
	 /* sinon probleme */
	 else
	 {
		 XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_06NumSite:XZSX_LitFichier(%s)",tl_NomFichier) ;
		 return(XDC_NOK) ;
	 }
	 /* avec succes */
	 return(XDC_OK) ;
 }
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int	XZSX_07Cmachine(
		XZSXT_Machine	va_Machine,
		char *	       pa_Repertoire)

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_07Cmachine ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *	pl_P ;
   XZSXT_ListeMachine	vl_Machine ;

   /* si la localisation du numero de site pose probleme */
   if (XZSX_06ExisteSite(va_Machine.NomSite,pa_Repertoire) != XDC_OK)
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_07Cmachine:XZSX_06") ;
       return(XDC_NOK) ;
     }
   /* conversion XZSXT_Machine -> XZSXT_ListeMachine */
   /*strcpy(vl_Machine.NomMachineTra,va_Machine.NomMachineTra) ;*/

   strcpy(vl_Machine.NomMachine,va_Machine.NomMachine) ;
   strcpy(vl_Machine.NomMachineImp,va_Machine.NomMachineImp) ;
   vl_Machine.TypeMachine = va_Machine.TypeMachine ;
   strcpy(vl_Machine.NomSite,va_Machine.NomSite) ;

   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LMACHINE) ;

   /* si probleme d'ouverture du fichier de config */
   if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
     {
       /* informe et abandonne */
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_04Tache_Ecriture:XZSX_LitFichier") ;
       return(XDC_NOK) ;
     }
   else
     {
       /* pour toutes les entites connues */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	 {
	   /* si l'entite est de type machine */
	   if (pl_P->Type == XZSXC_MACHINE)
	     {
	       /* si il y s'agit de la machine recherchee */
	       if (!strcmp(((XZSXT_ListeMachine *)(pl_P->Data))->NomMachine,vl_Machine.NomMachine))
		 {
		   /* ecrase les info existantes */
		   memcpy(((XZSXT_ListeMachine *)(pl_P->Data)),&vl_Machine,sizeof(vl_Machine)) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
       /* si aucune machine de ce nom localisee */
       if (pl_P == NULL)
	 {
	   /* cree cette entite */
	   XZSX_CreeEntite("Machine",XZSXC_COPIE,NULL,&vl_Machine) ;
	 }
       /* reecrit le fichier de config */
       XZSX_EcritFichier(vl_NomFichier) ;

       /* Avec succes */
       return(XDC_OK) ;
     }
 }

/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_08Dmachine(
 XZSCT_NomMachine		       va_NomMachine,
 char *				       pa_Repertoire
 )

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_08Dmachine ";
   char		vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *	pl_P ;
   XZSXT_Entite *	pl_ExP ;

   /* construction du nom du fichier cible */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LMACHINE) ;

   /* si la lecture du fichier s'est correctement deroulee */
   if (XZSX_LitFichier(vl_NomFichier) == XDC_OK)
     {
       /* pour toutes les entites connues */
       for ( pl_P = pl_ExP = XZSXV_pgNSUP
	    ; pl_P != NULL
	    ; pl_ExP = pl_P,pl_P = pl_P->Suivant
		     )
	 {
	   /* si l'entite est une Machine */
	   if (pl_P->Type == XZSXC_MACHINE)
	     {
	       /* si il s'agit de la tache recherchee */
	       if (!strcmp(((XZSXT_ListeMachine *)(pl_P->Data))->NomMachine,va_NomMachine))
		 {
		   if (pl_P == XZSXV_pgNSUP)      {
		     pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
		   }
		   else pl_ExP->Suivant = pl_P->Suivant ;

		   /* libere la place memoire */
		   free(pl_P->Data) ;
		   free(pl_P) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
       /* reecrit le fichier */
       XZSX_EcritFichier(vl_NomFichier) ;

       /* avec succes */
       return(XDC_OK) ;
     }
   /* sinon signale le */
   else
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_08Dmachine : pas de fichier correspondant a la machine %s",va_NomMachine) ;
       return(XDC_NOK) ;
     }
 }



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_09Cssysteme(
 XZSXT_ListeSSysteme			va_SSysteme,
 char *				        pa_Repertoire
 )

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_09Cssysteme ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *	pl_P ;

   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSSYSTEME) ;

   /* si probleme d'ouverture du fichier de config */
   if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_09Cssysteme:XZSX_LitFichier") ;
       return(XDC_NOK) ;
     }
   /* sinon */
   else
     {
       /* pour toutes les entites connues */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	 {
	   /* si l'entite est de type description de sous systeme */
	   if (pl_P->Type == XZSXC_SS)
	     {
	       /* si il y s'agit du sous systeme recherchee */
	       if (  !strcmp(((XZSXT_ListeSSysteme *)(pl_P->Data))->NomSSysteme,va_SSysteme.NomSSysteme)
		   &&  (((XZSXT_ListeSSysteme *)(pl_P->Data))->TypeMachine == va_SSysteme.TypeMachine)
		   )
		 {
		   /* ecrase les info existantes */
		   memcpy(((XZSXT_ListeSSysteme *)(pl_P->Data)),&va_SSysteme,sizeof(va_SSysteme)) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
       /* si aucun sous systeme de ce nom localise */
       if (pl_P == NULL)
	 {
	   /* cree cette entite */
	   XZSX_CreeEntite("SousSysteme",XZSXC_COPIE,NULL,&va_SSysteme) ;
	 }

       /* reecrit le fichier de config */
       XZSX_EcritFichier(vl_NomFichier) ;
     }

   return(XDC_OK);
 }



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int	XZSX_10Dssysteme(
 		XZSXT_ListeSSysteme	va_SSysteme,
 		char *			pa_Repertoire)

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_10Dssysteme ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *		pl_P ;
   XZSXT_Entite *		pl_ExP ;

   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSSYSTEME) ;

   /* si probleme d'ouverture du fichier de config */
   if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_09Cssysteme:XZSX_LitFichier") ;
       return(XDC_NOK) ;
     }
   /* sinon */
   else
     {

	/*A Verification qu'aucune tache est associe au Sous-Systeme
	*	que l'on veut supprimer */
	for ( pl_P = pl_ExP = XZSXV_pgNSUP ; 
			pl_P != NULL ; pl_ExP = pl_P,pl_P = pl_P->Suivant)
	{
		/* si l'entite est de type description de tache */
		if (pl_P->Type == XZSXC_TACHE)
		{
			/*B Si le Sous_systeme associe a la tache
			*	est celui que l'on veut detruire */
			if ( !strcmp(((XZSXT_ListeTache *)(pl_P->Data))->NomSSysteme,va_SSysteme.NomSSysteme))
			{
				/*B Impossible detruire Sous-systeme */
				XZST_03EcritureTrace(XZSTC_WARNING,
					"Impossible detruire SSyst %s \n",
					va_SSysteme.NomSSysteme);
				return(XDC_NOK);
			}
		}
	}


       /* pour toutes les entites connues */
       for ( pl_P = pl_ExP = XZSXV_pgNSUP ; pl_P != NULL ; pl_ExP = pl_P,pl_P = pl_P->Suivant)
	 {
	   /* si l'entite est de type description de sous systeme */
	   if (pl_P->Type == XZSXC_SS)
	     {
	       /* si il y s'agit du sous systeme recherchee */
	       if (  !strcmp(((XZSXT_ListeSSysteme *)(pl_P->Data))->NomSSysteme,va_SSysteme.NomSSysteme)
		   &&  (((XZSXT_ListeSSysteme *)(pl_P->Data))->TypeMachine == va_SSysteme.TypeMachine)
		   )
		 {
		   if (pl_P == XZSXV_pgNSUP)     pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
		   else pl_ExP->Suivant = pl_P->Suivant ;

		   /* libere la place memoire */
		   free(pl_P->Data) ;
		   free(pl_P) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }

       /* reecrit le fichier de config */
       XZSX_EcritFichier(vl_NomFichier) ;
     }

   return(XDC_OK);
 }



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int	XZSX_11ExisteSSysteme(
 		XZSCT_NomSSysteme	va_NomSSysteme,
 		int		  	va_TypeMachine,
 		char *			pa_Repertoire)

 /*
 * PARAMETRES EN ENTREE :
 * Nom du soussysteme
 * Nom machine
 *
 * PARAMETRES EN SORTIE :
 *
 *
 * VALEUR(S) DE RETOUR : < commentaire sur le resultat >
 *
 * CONDITION D'ERREUR :
 *
 * XDC_OK si il existe un enregistrement ssysteme et machine
 * XDC_NOK sinon
 *
 * CONDITIONS LIMITES :
 *
 *
 ******************************************************/
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_11ExisteSSysteme ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *		pl_P ;

   XZST_03EcritureTrace(XZSTC_FONCTION,"XZSX_11ExisteSSysteme(%s,%d,%s)",va_NomSSysteme,va_TypeMachine,pa_Repertoire) ;

   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSSYSTEME) ;

   /* si probleme d'ouverture du fichier de config */
   if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_11ExisteSSysteme:XZSX_LitFichier") ;
       return(XDC_NOK) ;
     }
   /* sinon */
   else
	 {
	   /* pour toutes les entites connues */
	   for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	     {
	       /* si l'entite est de type description de sous systeme */
	       if (pl_P->Type == XZSXC_SS)
		 {

		   /* si il y s'agit du sous systeme recherchee */
		   if (  !strcmp(((XZSXT_ListeSSysteme *)(pl_P->Data))->NomSSysteme,va_NomSSysteme)
		       && (((XZSXT_ListeSSysteme *)(pl_P->Data))->TypeMachine == va_TypeMachine)
		       )
		     {
		       /* stoppe la recherche */
		       break ;
		     }
		 }
	     }
	   /* libere les rewssources */
	   XZSX_OublieFichier() ;

	   /* si aucun sous systeme de ce nom localise */
	   if (pl_P == NULL)
	     {
	       /* Informe et abandonne */
	       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_11ExisteSSysteme:SS %s %d non localisee",va_NomSSysteme,
				    va_TypeMachine) ;
	       return(XDC_NOK) ;
	     }
	 }
   /* avec succes */
   return(XDC_OK) ;
 }



/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int	XZSX_12Ctache(
 		XZSCT_NomSSysteme	va_NomSSysteme,
 		int		        va_TypeMachine,
 		XZSCT_NomTache	     	va_NomTache,
 		int		        va_Temporaire,
 		char *		     	pa_Repertoire)

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_12Ctache ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *		pl_P ;
   XZSXT_ListeTache	vl_Tache ;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSX_12CTache(%s,%d,%s,%s)",
	va_NomSSysteme,va_TypeMachine,va_NomTache,pa_Repertoire) ;

   /* si la localisation du ssysteme pour la machine pose probleme */
   if (XZSX_11ExisteSSysteme(va_NomSSysteme,va_TypeMachine,pa_Repertoire) != XDC_OK)
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_12CTache:XZSX_11ExisteSSysteme") ;
       return(XDC_NOK) ;
     }
   /* creation XZSXT_ListeTache */
   strcpy(vl_Tache.NomTache,va_NomTache) ;
   strcpy(vl_Tache.NomSSysteme,va_NomSSysteme) ;
   vl_Tache.TypeMachine = va_TypeMachine;
   vl_Tache.Temporaire = va_Temporaire;

   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LTACHE) ;

   /* si probleme d'ouverture du fichier de config */
   if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
     {
       /* informe et abandonne */
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_12CTache:XZSX_LitFichier") ;
       return(XDC_NOK) ;
     }
   /* sinon */
   else
     {
       /* pour toutes les entites connues */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	 {
	   /* si l'entite est de type tache */
	   if (pl_P->Type == XZSXC_TACHE)
	     {
	       /* si il y s'agit de la tache recherchee */
	       if (!strcmp(((XZSXT_ListeTache *)(pl_P->Data))->NomTache,vl_Tache.NomTache)
		   && !strcmp(((XZSXT_ListeTache *)(pl_P->Data))->NomSSysteme,vl_Tache.NomSSysteme)
		   && ( ((XZSXT_ListeTache *)(pl_P->Data))->TypeMachine == vl_Tache.TypeMachine )  )
		 {
		   /* ecrase les info existantes */
		   memcpy(((XZSXT_ListeTache *)(pl_P->Data)),&vl_Tache,sizeof(vl_Tache)) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
       /* si aucune tache de ce nom localisee */
       if (pl_P == NULL)
	 {
	   /* cree cette entite */
	   XZSX_CreeEntite("Tache",XZSXC_COPIE,NULL,&vl_Tache) ;
	 }
     }
   /* reecrit le fichier de config */
   XZSX_EcritFichier(vl_NomFichier) ;

   /* Avec succes */
   return(XDC_OK) ;
 }



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int	XZSX_13Dtache(
 		XZSCT_NomSSysteme	va_NomSSysteme,
 		int		        va_TypeMachine,
 		XZSCT_NomTache	     	va_NomTache,
 		int		        va_Temporaire,
 		char *		     	pa_Repertoire)

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_13Dtache ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *		pl_P ;
   XZSXT_Entite *		pl_ExP ;
   XZSXT_ListeTache	vl_Tache ;

   XZST_03EcritureTrace(XZSTC_FONCTION
			,"XZSX_13Dtache(SS=%s,Mach=%d,Tache=%s,Rep=%s)"
			,va_NomSSysteme
			,va_TypeMachine
			,va_NomTache
			,pa_Repertoire
			) ;

   /* si la localisation du numero de ssysteme pose probleme */
   if (XZSX_11ExisteSSysteme(va_NomSSysteme,va_TypeMachine,pa_Repertoire) != XDC_OK)
     {
       /* Litte mother fucker... */
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_12CTache:XZSX_11ExisteSSysteme") ;
       return(XDC_NOK) ;
     }
   /* creation XZSXT_ListeTache */
   strcpy(vl_Tache.NomTache,va_NomTache) ;
   strcpy(vl_Tache.NomSSysteme,va_NomSSysteme) ;
   vl_Tache.TypeMachine = va_TypeMachine;
   vl_Tache.Temporaire = va_Temporaire;

   /* construction du nom du fichier cible */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LTACHE) ;

   /* si la lecture du fichier s'est correctement deroulee */
   if (XZSX_LitFichier(vl_NomFichier) == XDC_OK)
     {
       /* pour toutes les entites connues */
       for ( pl_P = pl_ExP = XZSXV_pgNSUP
	    ; pl_P != NULL
	    ; pl_ExP = pl_P,pl_P = pl_P->Suivant
	    )
	 {
	   /* si l'entite est une tache */
	   if (pl_P->Type == XZSXC_TACHE)
	     {
	       /* si il s'agit de la tache recherchee */
	       if (  !strcmp(((XZSXT_ListeTache *)(pl_P->Data))->NomTache,vl_Tache.NomTache)
		   && !strcmp(((XZSXT_ListeTache *)(pl_P->Data))->NomSSysteme,vl_Tache.NomSSysteme)
		   && ( ((XZSXT_ListeTache *)(pl_P->Data))->TypeMachine == vl_Tache.TypeMachine)
		   && ( ((XZSXT_ListeTache *)(pl_P->Data))->Temporaire ==  vl_Tache.Temporaire)
		   )
		 {
		   if (pl_P == XZSXV_pgNSUP)     pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
		   else pl_ExP->Suivant = pl_P->Suivant ;

		   /* libere la place memoire */
		   free(pl_P->Data) ;
		   free(pl_P) ;

		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
       /* reecrit le fichier */
       XZSX_EcritFichier(vl_NomFichier) ;

       /* avec succes */
       return(XDC_OK) ;
     }
   /* sinon signale le */
   else
     {
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_08Dtache : pas de fichier correspondant a la tache %s",va_NomTache) ;
       return(XDC_NOK) ;
     }
 }



/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

 int			XZSX_14Centite(
 XZSCT_NomEntite			      va_NomEntite,
 int				      va_TypeEntite,
 XZSCT_NomSite			      va_NomSite,
 char *				      pa_Repertoire
 )

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
 {
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_14Centite ";
   char			vl_NomFichier[XZSXC_LONG] ;
   XZSXT_Entite *		pl_P ;
   XZSXT_ListeEntite	vl_Entite ;

   /* si la recuperation du numero de site pose probleme */
  if (XZSX_06ExisteSite(va_NomSite,pa_Repertoire) != XDC_OK)
    {
      /* informe et abadonne */
      XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_14Centite:XZSX_06") ;
      return(XDC_NOK) ;
    }
   /* valorisation de la structure entite */
   strcpy(vl_Entite.NomEntite,va_NomEntite) ;
   vl_Entite.TypeEntite = va_TypeEntite ;
   strcpy(vl_Entite.NomSite,va_NomSite) ;
   
   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LENTITE) ;
   
   /* si probleme d'ouverture du fichier de config */
   if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
     {
       /* informe et abandonne */
       XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_14CEntite:XZSX_LitFichier") ;
       return(XDC_NOK) ;
     }
   /* sinon */
   else
     {
       /* pour toutes les entites connues */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	 {
	   /* si l'entite est de type entite */
	   if (pl_P->Type == XZSXC_ENTITE)
	     {
	       /* si il y s'agit de l' entite recherchee */
	       if (!strcmp(((XZSXT_ListeEntite *)(pl_P->Data))->NomEntite,vl_Entite.NomEntite)
		   && ((XZSXT_ListeEntite *)(pl_P->Data))->TypeEntite == vl_Entite.TypeEntite
		   )
		 {
		   /* ecrase les info existantes */
		   memcpy(((XZSXT_ListeEntite *)(pl_P->Data)),&vl_Entite,sizeof(vl_Entite)) ;
		   
		   /* stoppe la recherche */
		   break ;
		 }
	     }
	 }
		/* si aucune entite de ce nom localisee */
		if (pl_P == NULL)
		{
			/* cree cette entite */
			XZSX_CreeEntite("Entite",XZSXC_COPIE,NULL,&vl_Entite) ;
		}
	}
	/* reecrit le fichier de config */
	XZSX_EcritFichier(vl_NomFichier) ;

	/* Avec succes */
	return(XDC_OK) ;
}
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int			XZSX_15Dentite(
XZSCT_NomEntite			      va_NomEntite,
int				      va_TypeEntite,
char *				      pa_Repertoire
)

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
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_15Dentite ";
	char			vl_NomFichier[XZSXC_LONG] ;
	XZSXT_Entite *		pl_P ;
	XZSXT_Entite *		pl_ExP ;

	/* construction du nom du fichier cible */
	sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LENTITE) ;

	/* si la lecture du fichier s'est correctement deroulee */
	if (XZSX_LitFichier(vl_NomFichier) == XDC_OK)
	{
		/* pour toutes les entites connues */
		for ( pl_P = pl_ExP = XZSXV_pgNSUP
		    ; pl_P != NULL
		    ; pl_ExP = pl_P,pl_P = pl_P->Suivant
		    )
		{
			/* si l'entite est une ENTITE */
			if (pl_P->Type == XZSXC_ENTITE)
			{
				/* si il s'agit de la entite recherchee */
				if (  !strcmp(((XZSXT_ListeEntite *)(pl_P->Data))->NomEntite,va_NomEntite)
				    && ((XZSXT_ListeEntite *)(pl_P->Data))->TypeEntite == va_TypeEntite
				    )
				{
					if (pl_P == XZSXV_pgNSUP)     pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
					else pl_ExP->Suivant = pl_P->Suivant ;

					/* libere la place memoire */
					free(pl_P->Data) ;
					free(pl_P) ;

					/* stoppe la recherche */
					break ;
				}
			}
		}
		/* reecrit le fichier */
		XZSX_EcritFichier(vl_NomFichier) ;

		/* avec succes */
		return(XDC_OK) ;
	}
	/* sinon signale le */
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_15Dentite : pas de fichier correspondant a l'entite %s",va_NomEntite) ;
		return(XDC_NOK) ;
	}
}
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int			XZSX_19NbEntite(
int				       va_Type,
char *				       pa_Repertoire
)

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
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_19NbEntite ";
	char			tl_NomFichier[XZSXC_LONG];
	XZSXT_Entite *	pl_P ;
	int			vl_Nb = 0 ;

	/* Construction du pathname du fichier NSUP_NomMachine*/
	switch(va_Type)
	{
	case XZSXC_INFOSITE 	:
		sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSITE) ;
		break ;
	case XZSXC_MACHINE 	:
		sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LMACHINE) ;
		break ;
	case XZSXC_TACHE 	:
		sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LTACHE) ;
		break ;
	case XZSXC_SS  	:
		sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSSYSTEME) ;
		break ;
	case XZSXC_ENTITE 	:
		sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LENTITE) ;
		break ;
	}
	/* si la lecture du fichier s'est correctement deroule */
	if (XZSX_LitFichier(tl_NomFichier) == XDC_OK)
	{
		/* pour toutes les entites de la grappe */
		for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
		{
			/* si il s'agit d'une entite descripteur de machine */
			if (pl_P->Type == va_Type)
			{
				vl_Nb += 1 ;
			}
		}
		/* oublie */
		XZSX_OublieFichier() ;
	}
	/* Bye */
	return(vl_Nb) ;
}


/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int XZSX_24Message_Detruire( int    va_NumTexte,
char * pa_Repertoire )

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
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_24Message_Detruire ";
	char		tl_NomFichier[XZSXC_LONG] ;
	XZSXT_Entite *  pl_P ;
	XZSXT_Entite *	pl_ExP ;

	/* construction du fichier cible */
	sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_ACTMESG) ;

	/* si la lecture du fichier pose probleme */
	if (XZSX_LitFichier(tl_NomFichier) != XDC_OK)
	{
		return(XDC_NOK) ;
	}
	/* pour toutes les entites connues */
	for ( pl_P = pl_ExP = XZSXV_pgNSUP
	    ; pl_P != NULL
	    ; pl_ExP = pl_P,pl_P = pl_P->Suivant
	    )
	{
		/* si l'entite est de type entite */
		if (pl_P->Type == XZSXC_ACTION)
		{
			/* si il y s'agit de l'entite recherchee */
			if (((XZSXT_ActionMessage *)(pl_P->Data))->NumTexte == va_NumTexte)
			{
				if (pl_P == XZSXV_pgNSUP)     pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
				else pl_ExP->Suivant = pl_P->Suivant ;

				/* libere la place memoire */
				free(pl_P->Data) ;
				free(pl_P) ;

				/* stoppe la recherche */
				break ;
			}
		}
	}
	/* reecrit le fichier de config */
	XZSX_EcritFichier(tl_NomFichier) ;

	return(XDC_OK) ;
}
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xzsx
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* site.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int			XZSX_25Csite(
XZSCT_NomSite			     va_Site,
char *				     pa_Repertoire
)

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
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_25Csite ";
	char			vl_NomFichier[XZSXC_LONG] ;
	XZSXT_Entite *		pl_P ;
	XZSXT_ListeSite		vl_Site ;
	int			vl_IdSsysteme ;

	strcpy(vl_Site.NomSite,va_Site) ;

	/* construction du nom de fichier cible */
	sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSITE) ;

	/* si la lecture du fichier pose rpobleme */
	if (XZSX_LitFichier(vl_NomFichier) != XDC_OK)
	{
		/* informe et abandonne */
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_25Csite:XZSX_LitFichier") ;
		return(XDC_NOK) ;
	}
	/* pour toutes les entites connues */
	for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	{
		/* si l'entite est de type site */
		if (pl_P->Type == XZSXC_INFOSITE)
		{
			/* si il y s'agit du site recherchee */
			if (!strcmp(((XZSXT_ListeSite *)(pl_P->Data))->NomSite,vl_Site.NomSite))
			{
				/* ecrase les infos existantes */
				memcpy(((XZSXT_ListeSite *)(pl_P->Data)),&vl_Site,sizeof(vl_Site)) ;

				/* stoppe la recherche */
				break ;
			}
		}
	}
	/* si aucune site de ce nom localisee */
	if (pl_P == NULL)
	{
		/* cree cette entite */
		XZSX_CreeEntite("InfoSite",XZSXC_COPIE,NULL,&vl_Site) ;
	}
	/* reecrit le fichier de config */
	XZSX_EcritFichier(vl_NomFichier) ;

	/* Avec succes */
	return(XDC_OK) ;
}

/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int	XZSX_26Dsite(
		XZSCT_NomSite	va_Site,
		char *		pa_Repertoire)

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
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_26Dsite ";
	char			vl_NomFichier[XZSXC_LONG] ;
	XZSXT_Entite *		pl_P ;
	XZSXT_Entite *		pl_ExP ;
	XZSXT_ListeSite		vl_Site ;

	XZST_03EcritureTrace(XZSTC_INTERFACE
	    ,"XZSX_26Dsite	: IN -- Site %s-- Rep %s\n"
	    ,va_Site ,pa_Repertoire) ;

	/* creation XZSXT_ListeSite */
	strcpy(vl_Site.NomSite,va_Site) ;

	/* construction du nom du fichier cible */
	sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LSITE) ;

	/* si la lecture du fichier s'est correctement deroulee */
	if (XZSX_LitFichier(vl_NomFichier) == XDC_OK)
	{
		/*A Verification qu'aucune machine n'est associe
		*		au Site que l'on veut detruire */
		for ( pl_P = pl_ExP = XZSXV_pgNSUP
			    	; pl_P != NULL
				; pl_ExP = pl_P,pl_P = pl_P->Suivant)
	    	{
			/* si l'entite est une Machine */
			if (pl_P->Type == XZSXC_MACHINE)
			{
				/*B Si site associe correspond au Site 
				*	a detruire alors on ne detruit pas */
				if (!strcmp(((XZSXT_ListeMachine *)(pl_P->Data))->NomSite,vl_Site.NomSite))
				{
					/*B Impossible detruire Site */
					XZST_03EcritureTrace(XZSTC_WARNING,
						"Impossible detruire Site %s\n",
						vl_Site.NomSite);
					return(XDC_NOK);
				}
			}
		}

		/* pour toutes les entites connues */
		for ( pl_P = pl_ExP = XZSXV_pgNSUP
		    ; pl_P != NULL
		    ; pl_ExP = pl_P,pl_P = pl_P->Suivant
		    )
		{
			/* si l'entite est un site */
			if (pl_P->Type == XZSXC_INFOSITE)
			{
				/* si il s'agit de la site recherchee */
				if (!strcmp(((XZSXT_ListeSite *)(pl_P->Data))->NomSite,vl_Site.NomSite))
				{
					if (pl_P == XZSXV_pgNSUP)     
					  pl_ExP = XZSXV_pgNSUP = pl_P->Suivant ;
					else 
					  pl_ExP->Suivant = pl_P->Suivant ;

					/* libere la place memoire */
					free(pl_P->Data) ;
					free(pl_P) ;

					/* stoppe la recherche */
					break ;
				}
			}
		}

		/* reecrit le fichier */
		XZSX_EcritFichier(vl_NomFichier) ;

		/* avec succes */
		return(XDC_OK) ;
	}
	/* sinon signale le */
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_26Dsite : pas de fichier correspondant au site %s",va_Site) ;
		return(XDC_NOK) ;
	}
}

/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
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

int			XZSX_27ExisteMachine(
XZSCT_NomMachine		      va_NomMachine,
char *				      pa_Repertoire
)

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
{
	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_27ExisteMachine ";
	XZSXT_Entite *		pl_P ;
	char 			tl_NomFichier[XZSXC_LONG] ;

	XZST_03EcritureTrace(XZSTC_FONCTION,"XZSX_27ExisteMachine(%s,%s)",va_NomMachine,pa_Repertoire) ;

	/* construction du nom du fichier cible */
	sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_LMACHINE) ;

	/* si le decodage du fichier s'est correctement deroule */
	if (XZSX_LitFichier(tl_NomFichier) == XDC_OK)
	{

		/* pour toutes les entites decodees */
		for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
		{
			/* si l'entite est de type machine */
			if (pl_P->Type == XZSXC_MACHINE)
			{
				/* si il s'agit du site recherche */
				if (!strcmp(((XZSXT_ListeMachine *)(pl_P->Data))->NomMachine,va_NomMachine))
				{
					/* stoppe la recherche */
					break ;
				}
			}
		}
		/* libere les ressources */
		XZSX_OublieFichier() ;

		/* si aucun site ne correspond a celui demande */
		if (pl_P == NULL)
		{
			/* informe et abandonne */
			XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_06ExisteMachine:La machine %s n'a pas ete localise dans le fichier %s",
			    va_NomMachine,tl_NomFichier) ;
			return(XDC_NOK) ;
		}
	}
	/* sinon probleme */
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_06NumMachine:XZSX_LitFichier(%s)",tl_NomFichier) ;
		return(XDC_NOK) ;
	}
	/* avec succes */
	return(XDC_OK) ;
}



/*X*/
/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR 
 ******************************************************
 * MODULE XZSXDON  
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * pa_Repertoire:  repertoire ou se trouve le fichier
 *                 de config
 * pa_Taillemax  : Taille maxi du fichier circulaire du
 *  		   Journal
 *
 * Fonction
 * 
 * Codes retour
 * 
 * XDC_NOK 
 * XDC_OK  ok
 *
 * Mecanismes
 *
 ******************************************************/

 int	XZSX_29JournalLecture (
	long	     *pa_Taillemax,
	char 	     *pa_Repertoire)
 {
   static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_29JournalLecture";

   int 			vl_ValRet = XDC_OK;
   char			vl_NomFichier[XZSXC_LONG];
   XZSXT_Entite *	pl_P ;

   XZST_03EcritureTrace(XZSTC_INTERFACE
	,"XZSX_29JournalLecture : IN -- Rep repertoire %s "
			,pa_Repertoire ) ;

   /* construction du nom du fichier de config */
   sprintf(vl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_ADMINIS) ;

   /* si la lecture du fichier s'est correctement deroule */
   if (XZSX_LitFichier(vl_NomFichier) == XDC_OK)
     {
       /* pour toutes les entites de la grappe */
       for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)
	 {
	   /* si il s'agit d'une entite descripteur de journal */
	   if (pl_P->Type == XZSXC_JOURNAL)
	     {
		/* recupere les infomations */
		memcpy(pa_Taillemax,pl_P->Data,sizeof(*pa_Taillemax)) ;

		/* stoppe la recherche */
		break ;
	     }
	 }
       /* si pas d'info machine */
       if (pl_P == NULL)
	 {
	   XZST_03EcritureTrace(XZSTC_WARNING,
	        "XZSX_29JournalLecture:Le fichier %s ne possede pas de donnees",
		vl_NomFichier) ;
	   vl_ValRet = XDC_NOK ;
	 }
       /* oublie */
       XZSX_OublieFichier() ;
     }
     else
     {
	XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_29JournalLecture:XZSX_LitFichier" );
	vl_ValRet = XDC_NOK ;
     }

   XZST_03EcritureTrace(XZSTC_INTERFACE
	,"XZSX_29JournalLecture : OUT  -- Taille Max %d -- Code Retour %d \n",
	*pa_Taillemax, vl_ValRet);

   /* Bye */
   return(vl_ValRet) ;
 
}	/* Fin XZSX_29JournalLecture */


/* Fichier : $Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/07/22 09:26:30 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE xzsx
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * ecrit dans le fichier d'administration la taille max
 * du journal
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

 int	XZSX_30JournalEcriture(
	long	     va_Taillemax,
       char 	     *pa_Repertoire)

 /*
 * PARAMETRES EN ENTREE :
 *
 * pa_Repertoire:  repertoire ou se trouve le fichier
 *                 de config
 * va_Taillemax  : Taille maxi du fichier circulaire du
 *  		   Journal
 *
 ******************************************************/
 {
   	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_30JournalEcriture";
	 char			tl_NomFichier[XZSXC_LONG] ;
	 XZSXT_Entite *		pl_P ;

	 /* construction du nom du fichier de config a creer */
	 sprintf(tl_NomFichier,"%s/%s",pa_Repertoire,XDF_NSUP_ADMINIS) ;

	 /* si la lecture pose probleme */
	 if (XZSX_LitFichier(tl_NomFichier) != XDC_OK)
	 {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSX_30JournalEcriture:XZSX_LitFichier") ;
		return(XDC_NOK) ;
	 }
	 else
	 {
		 /*  pour toutes les entites connues */
		 for (pl_P = XZSXV_pgNSUP ; pl_P != NULL ; pl_P = pl_P->Suivant)

			 /* si l'entite courante est une information machine */
			 if (pl_P->Type == XZSXC_JOURNAL)
			 {
				 /* endosse les nouvelles informations */
				 memcpy((long *)(pl_P->Data),&va_Taillemax,sizeof(va_Taillemax)) ;

				 /* stoppe la recherche */
				 break ;
			 }
	 	/* si pas d'information journal */
	 	if (pl_P == NULL)
	 	{
			 /* cree cette entite */
			 XZSX_CreeEntite("Journal",XZSXC_COPIE,NULL,&va_Taillemax) ;
	 	}

	        /* reecrit le fichier */
	        XZSX_EcritFichier(tl_NomFichier) ;

	        /* Bye */
	        return(XDC_OK) ;
	 }
 }



/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de coder ou decoder une struture TIMER
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 int	XZSX_Timer(
	    int		va_Action,
	    void *  	pa_Data,
	    char *  	pa_Chaine)

/*
* ARGUMENTS EN ENTREE :
*   va_Action :  Codage ou Decodage 
*   pa_Data : donnees a traiter
*   pa_chaine : donnees a etourner 
*
* ARGUMENTS EN SORTIE :
*	Code de retour
*
* CODE RETOUR : 
*	XDC_OK
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Test l'action a effectuer, puis transforme
*	les donnees.
*
------------------------------------------------------*/
{
   	static char *version="$Id: xzsx.c,v 1.18 2021/07/22 09:26:30 pc2dpdy Exp $ : XZSX_Timer";
   	XZSXT_ListeTimer *	pl_Info = (XZSXT_ListeTimer *)pa_Data ;
	char * 			separateur_blanc = " ";

   /* si encodage */
   if (va_Action == XZSXC_ENCODAGE)
     {
       sprintf( pa_Chaine,"%f %d %d %s %s %d",
			 pl_Info->Horodate,
			 pl_Info->Delai,
			 pl_Info->IdMsg,
			 pl_Info->Dg,
			 pl_Info->Cle,
			 pl_Info->Secur );
     }
   /* sinon decodage */
   else
     {
       if (sscanf( pa_Chaine,"%lf %d %d %s %s %d",
			 &(pl_Info->Horodate),
			 &(pl_Info->Delai),
			 &(pl_Info->IdMsg),
			 pl_Info->Dg,
			 pl_Info->Cle,
			 &(pl_Info->Secur) ) != 6) 
	{
		return(XDC_NOK) ;
	}
	else
	{
		/** Recuperation de l'Horodate en utilisant la 
		* 	primitive atof car pb pour recuperer un
		* 	double avec sscanf   **/
		pl_Info->Horodate = 
			atof( (strtok(pa_Chaine,separateur_blanc)) );
	}
     }

	
   return(XDC_OK) ;

 } 	/* fin XZSX_Timer */

/* fin de fichier */
