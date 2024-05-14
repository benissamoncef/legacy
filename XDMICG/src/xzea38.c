/*E*/
/*  Fichier : $Id: xzea38.c,v 1.5 2020/11/03 17:57:45 pc2dpdy Exp $        Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 17:57:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea38.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	16 Jan 1998	: Creation
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : format champs de 'printf' ou 'XDG_EncodeDG2'  1.4
* JMG		10/05/20 : EMS 1.2
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		23/11/22	: Suppression abonnement callback XDM_ETAT_PAL. On conserve uniquement XDM_ETAT_PAL_EMS SAE-311
* LCL		12/06/23	: Ajout traces dans XZEA_Etat_PAL_EMS
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzea38.c,v 1.5 2020/11/03 17:57:45 pc2dpdy Exp $ $Revision: 1.5 $ $Date: 2020/11/03 17:57:45 $ : xzea38" ;

XDY_Fonction		pg_FonctUtil_XZEA_38 = NULL;
T_CB			vg_CB_XZEA38         = NULL;
T_CB			vg_CB_XZEA38_EMS         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_PAL ( 
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
*  XDM_ETAT_PAL 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzea38.c,v 1.5 2020/11/03 17:57:45 pc2dpdy Exp $ $Revision: 1.5 $ $Date: 2020/11/03 17:57:45 $ : XZEA_Etat_PAL" ;
   XZEAT_Etat_PAL	vl_Etat;
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
#ifndef _TIBCO_EMS
   T_STR		*pl_Modules;
   T_INT2		vl_Numero = 0;
   T_INT4		vl_Taille = 0;
   T_INT2               vl_Type = 0; 
#else
   XDY_Nom		pl_Modules[XDC_Lon_Nb_Mod_Pal];
   short		vl_Numero = 0;
   int			vl_Taille = 0;
   short                vl_Type = 0; 
#endif
   XDY_NomMachine vl_machine="";


   /* Recuperation des parametres du message XDM_ETAT_PAL */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Numero,
	       T_IPC_FT_INT2,        &vl_Type,
               T_IPC_FT_STR_ARRAY,  &pl_Modules, &vl_Taille,
	       NULL) )
   {
	      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_PAL dans XZEA_Etat_PAL");
   }
   else
   { 

   /* Recuperation des parametres du message XDM_ETAT_PAL */
   
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEA_Etat_PAL  Numero:%d ", vl_Numero );      
      /*A Renseignement de la structure XZEAT_Etat_PAL */
      vl_Etat.Numero	= vl_Numero;
	       
      for ( vl_Index = 0;(vl_Index < vl_Taille); vl_Index++ )
{
        strcpy(vl_Etat.Modules[vl_Index] , pl_Modules[vl_Index]);
XZST_03EcritureTrace ( XZSTC_FONCTION, "XZEA_Etat_PAL vl_Etat.Modules[%d]=[%s] pl_Modules[%d]=[%s]",vl_Index,vl_Etat.Modules[vl_Index],vl_Index,pl_Modules[vl_Index]);
}          

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_PAL : probleme dans recuperation machine destinataire du message");
	  }
 
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_PAL en argument */
      pg_FonctUtil_XZEA_38 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_PAL %d", vl_Resultat );
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

void	XZEA_Etat_PAL_EMS ( 
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
*  XDM_ETAT_PAL 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzea38.c,v 1.5 2020/11/03 17:57:45 pc2dpdy Exp $ $Revision: 1.5 $ $Date: 2020/11/03 17:57:45 $ : XZEA_Etat_PAL_EMS" ;
   XZEAT_Etat_PAL	vl_Etat;
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
#ifndef _TIBCO_EMS
   T_STR		pl_Modules;
   T_INT2		vl_Numero = 0;
   T_INT2               vl_Type = 0; 
   T_STR                *vl_StrModules;
#else
   XDY_Nom		pl_Modules[XDC_Lon_Nb_Mod_Pal];
   short		vl_Numero = 0;
   short                vl_Type = 0; 
   char                 vl_StrModules[1024];
#endif

   char                 *vl_ptr;
   XDY_NomMachine vl_machine="";
   
   /* Recuperation des parametres du message XDM_ETAT_PAL */
   if ( !TipcMsgRead(pa_Data -> msg,
               		T_IPC_FT_INT2,	&vl_Numero,
	       			T_IPC_FT_INT2,	&vl_Type,
               		T_IPC_FT_STR,	&vl_StrModules,
	       			NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_PAL_EMS dans XZEA_Etat_PAL_EMS");
   }
   else
   { 
   
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEA_Etat_PAL_EMS  Numero:%d Type:%d Modules:%s", vl_Numero, vl_Type, vl_StrModules );      
      /*A Renseignement de la structure XZEAT_Etat_PAL */
      for (vl_Index=0;vl_Index<XDC_Lon_Nb_Mod_Pal;vl_Index++)
    		strcpy(vl_Etat.Modules[vl_Index++] , "");

      vl_Etat.Numero	= vl_Numero;
      vl_ptr=strtok((char*)vl_StrModules,"|");
      vl_Index = 0;
      while (vl_ptr!=NULL)
      {
            strcpy(vl_Etat.Modules[vl_Index++] , vl_ptr);
      		XZST_03EcritureTrace ( XZSTC_DEBUG1, "vl_Etat.Modules[%d] = [%s]",vl_Index-1, vl_Etat.Modules[vl_Index-1]);
            vl_ptr=strtok(NULL,"|");
      }
//      for ( vl_Index = 0;(vl_Index < vl_Taille); vl_Index++ )
//        strcpy(vl_Etat.Modules[vl_Index] , pl_Modules[vl_Index]);
          
	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_PAL_EMS : probleme dans recuperation machine destinataire du message");
	  }
       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_PAL en argument */
      pg_FonctUtil_XZEA_38 ( vl_Etat, &vl_Resultat,vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_PAL_EMS %d", vl_Resultat );
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

int	XZEA38_Abt_Etat_PAL (		XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des PAL d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "$Id: xzea38.c,v 1.5 2020/11/03 17:57:45 pc2dpdy Exp $ $Revision: 1.5 $ $Date: 2020/11/03 17:57:45 $ : XZEA38_Abt_Etat_PAL" ;
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA38_Abt_Etat_PAL Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_38 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_PAL */
	if ( vg_CB_XZEA38_EMS == NULL )
	{
	   if ( ( vg_CB_XZEA38_EMS = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_PAL_EMS),
							      	XZEA_Etat_PAL_EMS,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_PAL_EMS." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_PAL_EMS." );
	   }
	}

// Suppression de l'abonnement au message format rtserver pour Magistra : tous les LT emettent des messages ems
//	/*A  Armement Callback sur Reception XDM_ETAT_PAL */
//	if ( vg_CB_XZEA38 == NULL )
//	{
//	   if ( ( vg_CB_XZEA38 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_PAL),
//							      	XZEA_Etat_PAL,
//								(T_CB_ARG)NULL ) ) == NULL ) 
//	   {	
//	      /*B Ecriture Trace */
//	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_PAL." );
//	      vl_ValRet = XDC_NOK;
//	   }
//	   else
//	   {	
//	      /*B Ecriture Trace */
//	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_PAL." );
//	   }
//	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_PAL<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PAL, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_PAL */
           /*A Construction du DatGroup XDG_EInit_IHM_PAL_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_PAL, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_PAL en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_PAL),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_PAL non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PAL.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_PAL */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PAL sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA38 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_PAL */
	   if ( TutCbDestroy ( vg_CB_XZEA38_EMS ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_PAL_EMS ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_PAL_EMS ." );
	      vg_CB_XZEA38 = NULL;
	   }
           /*A  Desarmement Callback sur Reception XDM_ETAT_PAL */
	   if ( TutCbDestroy ( vg_CB_XZEA38 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_PAL ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_PAL ." );
	      vg_CB_XZEA38 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_PAL<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s%s", XDG_ETAT_PAL, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA38_Abt_Etat_PAL Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


