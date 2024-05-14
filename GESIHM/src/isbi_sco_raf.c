/*E*/
/*Fichier : $Id: isbi_sco_raf.c,v 1.2 2012/05/29 10:09:29 pc2dpdy Exp $      Release : $Revision: 1.2 $       Date : $Date: 2012/05/29 10:09:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSOP * FICHIER MSOP_ech.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Mise a jour des courbe : Programmation timer sur fonction de 
*  raf des donnees.
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	16 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco_raf.h"
#include "isbi_sco.h"
/* definitions de constantes */

#define CM_DELAI   72
#define CM_DELAI_6mn   360

#define CM_TAILLE_NOM 256


/* definitions de types locaux */

typedef char tm_nom_var[CM_TAILLE_NOM];


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isyn_sco_raf.c	1.11 09/17/07 : isyn_sco_raf" ;

static T_CB vm_rafcallback[CM_NB_MAX_FENETRES]={NULL,NULL,NULL};

static int vm_ident_msgraf[CM_NB_MAX_FENETRES]={ XDM_IRAF_COURBE0, XDM_IRAF_COURBE1, XDM_IRAF_COURBE2 };

extern XZSCT_NomSite vm_NomSite;
extern XZSCT_NomMachine vm_NomMachine;
extern int  vm_TypeMachine;
extern int  vm_multizone;
extern  tm_caracteristique_courbe vm_caracteristique_courbe[CM_NB_MAX_FENETRES];

extern int		vm_slot_courant_mz;


/* declaration de fonctions internes */



/* definition de fonctions externes */
extern XDY_Octet MSCO_Type_Courbe();
//extern ADDRESS MSCO_Maj_dsv();

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    Callback demandant le raf des donnees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Rafraichir(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_raf.c	1.11 09/17/07 : MSCO_Rafraichir" ;
   XDY_Octet vl_fenetre;
   XDY_Octet vl_type_courbe;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :MSCO_Rafraichir" );
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" );
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" );

   vl_fenetre = (XDY_Octet) pa_ARG;
   vl_type_courbe = MSCO_Type_Courbe(vl_fenetre);

   XZST_03EcritureTrace(   XZSTC_WARNING, "$$ FENETRE = %d  $$", vl_fenetre);
   XZST_03EcritureTrace(   XZSTC_WARNING, "$$ COURBE  = %d  $$", vl_type_courbe);

   /*A Appel de la fonction de raf des courbes */
//   if ( vl_fenetre == CM_FENETRE_LARGE )
//      {
//      if (vm_multizone == XDC_OUI)
//	MSCO_raf_courbe_tdp_mz(vl_fenetre);
//      else
//      	MSCO_raf_courbe_tdp(vl_fenetre);
//      }
//   else
//   if ( vl_type_courbe == CM_COURBE_STRA )
//      {
//      MSCO_raf_courbe_strada(vl_fenetre);
//      }
//   else
   if ( vl_type_courbe == CM_COURBE_FT_REF )
      {
//      MSCO_raf_courbe_ft_ref(vl_fenetre);
      }
   else
      {
      if (( vm_TypeMachine == XZSCC_TYPEM_PCS ) || ( vm_caracteristique_courbe[vl_fenetre-1].periode == XDC_NON ))
         MSCO03_raf_courbe_temps6mn(vl_fenetre);
      else
         MSCO03_raf_courbe_temps(vl_fenetre);
      }
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" );
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" );
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :MSCO_Rafraichir");
   return ;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Lancement_raf_courbe(
          va_fenetre)
          
          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*  va_fenetre : fenetre d'affichage
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_raf.c	1.11 09/17/07 : MSCO_Lancement_raf_courbe" ;
   int i=0;
   XZSMT_CleProg vl_cle;
   XZSCT_Datagroup vl_Datagroup ;
   int vl_JourSemaine;
   double vl_HorodateSec; 
   int vl_delai=0.0;
   T_BOOL vl_etat;
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Lancement_raf_courbe" );
   
   
         XZST_03EcritureTrace( XZSTC_WARNING, "Periode = %d" , (int) vm_caracteristique_courbe[va_fenetre-1].periode );
         
      

   /*A  Armement Callback sur Reception XDM_IRAF_COURBE */
   if ( (vm_rafcallback[va_fenetre-1] = TipcSrvProcessCbCreate(XDM_IdentMsg( vm_ident_msgraf[va_fenetre-1] ),
				MSCO_Rafraichir,(T_CB_ARG) va_fenetre )) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "CHI Callback sur %d non declenchee. \n " ,vm_ident_msgraf[va_fenetre-1] );
      return XDC_NOK;
      }	


   sprintf( vl_cle , "courbe_%s%d" , vm_NomMachine , va_fenetre );
   
   XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_COURBE , vm_NomMachine );
   
   /*A Abonnement un datagroup pour le callback si ce n'est deja fait */
   TipcSrvDgGetRecv( vl_Datagroup , &vl_etat );
   if ( vl_etat == FALSE )
      if (! XDG_JoindreDG(vl_Datagroup) )
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }	
   
   
   /*A Lecture horodate courante systeme */
   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
   /*A Demande de programmation timer */
   if ( va_fenetre == CM_FENETRE_LARGE )
      vl_delai=CM_DELAI_6mn;
   else
      {
      if (( vm_TypeMachine == XZSCC_TYPEM_PCS ) || ( vm_caracteristique_courbe[va_fenetre-1].periode == XDC_NON ) || ( vm_caracteristique_courbe[va_fenetre-1].periode == CM_COURBE_STRA) )
         vl_delai=CM_DELAI_6mn;
      else
         vl_delai=CM_DELAI;
      }
      
   XZST_03EcritureTrace( XZSTC_WARNING, "DELAI %d",vl_delai);
   if ( XZSM_01DemanderProgrammation(    vl_HorodateSec+vl_delai,
					 vl_delai, 
					 vl_Datagroup, 
			                 vm_ident_msgraf[va_fenetre-1], 
					 vl_cle,
					 NULL) != XDC_OK )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Programmation sur %s non declenchee. \n " ,vm_ident_msgraf[va_fenetre-1]);
       return XDC_NOK;
       }	
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Lancement_raf_courbe" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Arret_raf_courbe(
		va_fenetre)
          
          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*    pa_nom_ech : nom de l'echangeur
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_raf.c	1.11 09/17/07 : MSCO_Arret_raf_courbe" ;
   int i=0;
   XZSMT_CleProg vl_cle;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Arret_raf_courbe" );
   
   /*A liberation de la table des pourcentage */   
   sprintf( vl_cle , "courbe_%s%d" , vm_NomMachine , va_fenetre);
   /*A  DesArmement Callback sur Reception XDM_IRAF_COURBE */
XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Arret_raf_courbe - desarmer CB fen %d = (%o)",va_fenetre, vm_rafcallback[va_fenetre -1] );
	if (vm_rafcallback[va_fenetre -1] != NULL){
	   if ( TutCbDestroy( vm_rafcallback[va_fenetre -1] ) == FALSE )
		  {
		  /* Ecriture Trace */
		  XZST_03EcritureTrace( XZSTC_WARNING,
					   "Desarmement de la Callback sur %d non effectuee. \n " ,vm_ident_msgraf[va_fenetre -1]);
		  return XDC_NOK;
		  }
	}else
	{
		  XZST_03EcritureTrace( XZSTC_WARNING,
					   "Desarmement de la Callback sur %d Impossible, le CB n'existe pas. \n " ,vm_ident_msgraf[va_fenetre -1]);
		  return XDC_NOK;

	}
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Arret_raf_courbe - annuler Programmation timer" );
   /*A Demande de programmation timer */
   if ( XZSM_02AnnulerProgrammation(vl_cle,NULL,NULL) != XDC_OK )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Annulation de la Programmation sur %s non effectuee. \n " ,vm_ident_msgraf[va_fenetre -1]);
       return XDC_NOK;
       }	
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Arret_raf_courbe" );
   return XDC_OK;
}
