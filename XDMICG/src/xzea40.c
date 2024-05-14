/*E*/
/*  Fichier : $Id: xzea40.c,v 1.4 2020/11/03 17:57:46 pc2dpdy Exp $        Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 17:57:46 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzea40.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JPL	07/12/10 : Migration architecture HP ia64 (DEM 961) : format champs de 'printf' ou 'XDG_EncodeDG2'  1.3
* JMG 10/05/20 EMS 1.4
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		23/11/22	: Suppression abonnement callback XDM_ETAT_PAL. On conserve uniquement XDM_ETAT_PAL_EMS SAE-311
* LCL	24/01/23	: Correction longueur variable sur reception message XDM_ETAT_BAF
* LCL	03/02/23	: Correction decodage message modules sur reception message XDM_ETAT_BAF_EMS
* LCL		02/06/23	: Correctif parametre vl_machine sur appel pg_FonctUtil_XZEA_XX
------------------------------------------------------ */

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzea40.c,v 1.4 2020/11/03 17:57:46 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2020/11/03 17:57:46 $ : xzea40" ;
XDY_Fonction		pg_FonctUtil_XZEA_40 = NULL;
T_CB			vg_CB_XZEA40_EMS         = NULL;
T_CB			vg_CB_XZEA40         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_BAF ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ETAT_BAF 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzea40.c,v 1.4 2020/11/03 17:57:46 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2020/11/03 17:57:46 $ : XZEA_Etat_BAF" ;
   XZEAT_Etat_BAF	vl_Etat;
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero = 0;
   T_INT2		vl_Type = 0;
   T_STR		*pl_Modules;
   T_INT4		vl_Taille = 0;
#else
   XDY_Mot		vl_Numero = 0;
   XDY_Mot		vl_Type = 0;
   XDY_Nom		pl_Modules[XDC_Lon_Nb_Mod_BAF];
   XDY_Entier		vl_Taille = 0;
#endif
   XDY_NomMachine vl_machine="";
   
   
   /* Recuperation des parametres du message XDM_ETAT_BAF */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_INT2,        &vl_Type,
               T_IPC_FT_STR_ARRAY,  &pl_Modules, &vl_Taille,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_BAF dans XZEA_Etat_BAF");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_WARNING, "JMG  type:%d no bra:%d nb modules:%d", vl_Type, vl_Numero, vl_Taille );
      vl_Etat.Etat.Numero	= (XDY_Eqt)vl_Numero;
      for ( vl_Index = 0;(vl_Index < vl_Taille); vl_Index++ )
{
XZST_03EcritureTrace ( XZSTC_WARNING, "JMG  pl_Modules[%d]=%s",vl_Index,pl_Modules[vl_Index]);
          strcpy(vl_Etat.Etat.Modules[vl_Index] , pl_Modules[vl_Index]);
}         
      /*A Renseignement de la structure XZEAT_Etat_BAF */
      vl_Etat.TypeEqt		= vl_Type;

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
	  	XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_BAF : probleme dans recuperation machine destinataire du message");
	  }
	       
      XZST_03EcritureTrace ( XZSTC_WARNING, "CLUCHAGUE numero = %d",vl_Etat.Etat.Numero);
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_BAF en argument */
      pg_FonctUtil_XZEA_40 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_BAF %d", vl_Resultat );
   }

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_BAF_EMS ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ETAT_BAF 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzea40.c,v 1.4 2020/11/03 17:57:46 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2020/11/03 17:57:46 $ : XZEA_Etat_BAF" ;
   XZEAT_Etat_BAF	vl_Etat;
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero = 0;
   T_INT2		vl_Type = 0;
   T_STR		*vl_StrModules;
   T_INT4		vl_Taille = 0;
#else
   XDY_Mot		vl_Numero = 0;
   XDY_Mot		vl_Type = 0;
   char			vl_StrModules[XDC_Lon_Nb_Mod_BAF*4];
   XDY_Entier		vl_Taille = 0;
#endif
   char			*vl_ptr;
   XDY_NomMachine vl_machine="";
   
  
   /* Recuperation des parametres du message XDM_ETAT_BAF_EMS */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,   &vl_Numero,
               T_IPC_FT_INT2,   &vl_Type,
               T_IPC_FT_STR,  	&vl_StrModules,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_BAF_EMS dans XZEA_Etat_BAF_EMS");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_WARNING, "EMS  type:%d no bra:%d modules:%s", vl_Type, vl_Numero, vl_StrModules );
      vl_Etat.Etat.Numero	= (XDY_Eqt)vl_Numero;
      vl_ptr=strtok(vl_StrModules,"|");
      vl_Index = 0;
      while (vl_ptr!=NULL)
      {
			strcpy(vl_Etat.Etat.Modules[vl_Index++] , vl_ptr);
			vl_ptr=strtok(NULL,"|");
      }
          
      /*A Renseignement de la structure XZEAT_Etat_BAF */
      vl_Etat.TypeEqt		= vl_Type;
	       
      XZST_03EcritureTrace ( XZSTC_WARNING, "EMS numero = %d Mod 0 = %s Mod 1 = %s Mod 2 = %s Mod 3 = %s Mod 4 = %s Mod 5 = %s",vl_Etat.Etat.Numero,
	  																															vl_Etat.Etat.Modules[0],
																																vl_Etat.Etat.Modules[1],
																																vl_Etat.Etat.Modules[2],
																																vl_Etat.Etat.Modules[3],
																																vl_Etat.Etat.Modules[4],
																																vl_Etat.Etat.Modules[5]);
	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_BAF : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_BAF en argument */
      pg_FonctUtil_XZEA_40 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_BAF_EMS %d", vl_Resultat );
   }

}


/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA40_Abt_Etat_BAF (		XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite		: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des BAF d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "$Id: xzea40.c,v 1.4 2020/11/03 17:57:46 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2020/11/03 17:57:46 $ : XZEA40_Abt_Etat_BAF" ;
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA40_Abt_Etat_BAF Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_40 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_BAF */
	if ( vg_CB_XZEA40_EMS == NULL )
	{
	   if ( ( vg_CB_XZEA40_EMS = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_BAF_EMS),
							      	XZEA_Etat_BAF_EMS,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_BAF_EMS." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Armement Callback sur XDM_ETAT_BAF_EMS." );
	   }
	}
// Suppression de l'abonnement au message format rtserver pour Magistra : tous les LT emettent des messages ems
//	if ( vg_CB_XZEA40 == NULL )
//	{
//	   if ( ( vg_CB_XZEA40 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_BAF),
//							      	XZEA_Etat_BAF,
//								(T_CB_ARG)NULL ) ) == NULL ) 
//	   {	
//	      /*B Ecriture Trace */
//	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_BAF." );
//	      vl_ValRet = XDC_NOK;
//	   }
//	   else
//	   {	
//	      /*B Ecriture Trace */
//	      XZST_03EcritureTrace ( XZSTC_WARNING, "Armement Callback sur XDM_ETAT_BAF." );
//	   }
//	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_BAF<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_BAF, pa_NomSite );
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pl_NomDG );
	   }
	}
	
	/*A Abonnement au Datagroup DGInit passe en parametre */
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
   	if ( ( !vl_EtatDg ) && ( pa_DGInit != NULL ) )
   	{
   	   if ( !XDG_JoindreDG( pa_DGInit ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pa_DGInit );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pa_DGInit );
	   }
	}
	   /* Envoi du message de demande d'init XDM_EInit_IHM_BAF */
           /*A Construction du DatGroup XDG_EInit_IHM_BAF_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_BAF, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_BAF en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_BAF),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_BAF non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_BAF.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_BAF */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_BAF sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA40 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_BAF_EMS */
	   if ( TutCbDestroy ( vg_CB_XZEA40_EMS ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_BAF_EMS ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_BAF_EMS ." );
	      vg_CB_XZEA40_EMS = NULL;
	   }
           /*A  Desarmement Callback sur Reception XDM_ETAT_BAF */
	   if ( TutCbDestroy ( vg_CB_XZEA40 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_BAF ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_BAF ." );
	      vg_CB_XZEA40 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_BAF<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s%s", XDG_ETAT_BAF, pa_NomSite );
   	/*A Disjoindre ce datagroup s'il est deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( vl_EtatDg )
   	{
   	   if ( !XDG_QuitterDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pl_NomDG );
	   }
	}
	
	/*A Desabonnement au Datagroup DGInit passe en parametre */
   	/*A Disjoindre ce datagroup s'il est deja joint */
   	TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
   	if ( ( vl_EtatDg ) && ( pa_DGInit != NULL ) )
   	{
   	   if ( !XDG_QuitterDG( pa_DGInit ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pa_DGInit );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pa_DGInit );
	   }
	}
     }
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA40_Abt_Etat_BAF Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


