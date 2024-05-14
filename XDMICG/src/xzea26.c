/*E*/
/* Fichier : @(#)xzea26.c	1.6      Release : 1.6        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea26.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Fev 1995	: Creation
* Mismer.D	version 1.2	08 Mar 1995	: Suppression Duplicate initializer for identifier
* Mismer.D	version 1.3	09 Mar 1995	:
* Fontaine.C	version 1.4	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.5	21 Sep 1995	: Modif message de demande d'init
* Mismer.D	version 1.6	27 Nov 1995	: Correction modif message de demande d'init
* JMG		1.7		10/05/20	: EMS
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea26.c	1.6 12/13/95 : xzea26" ;
XDY_Fonction		pg_FonctUtil_XZEA_26 = NULL;
T_CB			vg_CB_XZEA26         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_Mat_NAV ( 
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
*  XDM_ETAT_MAT_NAV 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea26.c	1.6 12/13/95 : XZEA_Etat_Mat_NAV ";
   XZEAT_Etat_Mat_NAV	vl_Etat;
#ifndef _TIBCO_EMS
   T_INT2		vl_NoEqtNAV;
   T_INT2		*pl_ListeParam;
   T_STR		vl_Matrice;
   T_INT4		vl_Nb_Params_Mat = 0;
#else
   XDY_Mot		vl_NoEqtNAV;
   XDY_Mot		pl_ListeParam[XDC_Lon_Params_Matrice];
   XDY_Phrase		vl_Matrice;
   XDY_Entier		vl_Nb_Params_Mat = 0;
#endif
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
   XDY_NomMachine vl_machine="";
   
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEA_Etat_Mat_NAV " );

   /* Recuperation des parametres du message XDM_ETAT_MAT_NAV */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtNAV
                          ,T_IPC_FT_STR,  &vl_Matrice
                          ,T_IPC_FT_INT2_ARRAY,  &pl_ListeParam , &vl_Nb_Params_Mat
	                  ,NULL))
   {       
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_MAT_NAV dans XZEA_Etat_Mat_NAV");
   }
   else
   { 
#ifndef _TIBCO_EMS
      for ( vl_Index = 0;
          (vl_Index < vl_Nb_Params_Mat);
          vl_Etat.Params[vl_Index] = *pl_ListeParam, vl_Index++, pl_ListeParam++);
#else
      for ( vl_Index = 0;
          (vl_Index < vl_Nb_Params_Mat);vl_Index++)
          vl_Etat.Params[vl_Index] = pl_ListeParam[vl_Index];
#endif
          
      /*A Renseignement de la structure XZEAT_Etat_NAV */
      vl_Etat.NumEqt	= vl_NoEqtNAV;
      strcpy ( vl_Etat.Matrice, vl_Matrice );

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_Mat_NAV : probleme dans recuperation machine destinataire du message");
	  }
	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_NAV en argument */
      pg_FonctUtil_XZEA_26 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_Mat_NAV %d", vl_Resultat );
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

int	XZEA26_Abt_Etat_Mat_NAV (	XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Eqt		va_NumEqt,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Eqt	va_NumEqt		: Numero d'equipement
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats matrice des NAV d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea26.c	1.6 12/13/95 : XZEA26_Abt_Etat_Mat_NAV ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA26_Abt_Etat_Mat_NAV Abt:%d DGInit:%s Fonct:%ld Numero:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		va_NumEqt );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_26 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_MAT_NAV */
	if ( vg_CB_XZEA26 == NULL )
	{
	   if ( ( vg_CB_XZEA26 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_MAT_NAV),
							      	XZEA_Etat_Mat_NAV,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_MAT_NAV." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_MAT_NAV." );
	   }
	}
	

	/*A Abonnement au Datagroup XDG_ETAT_MAT_NAV */
	/*A Construction du nom du DataGroup */
	strcpy ( pl_NomDG, XDG_ETAT_MAT_NAV );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_MAT_NAV */
           /*A Construction du DatGroup XDG_EInit_IHM_NAV */
           strcpy ( pl_NomDG, XDG_EInit_IHM_NAV );
   	
           /*A Envoi du message XDM_EInit_IHM_MAT_NAV en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_MAT_NAV),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
					T_IPC_FT_INT2, va_NumEqt,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_MA_TNAV non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_MAT_NAV.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_MAT_NAV */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_MAT_NAV sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA26 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_MAT_NAV */
	   if ( TutCbDestroy ( vg_CB_XZEA26 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_MAT_NAV ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_MAT_NAV ." );
	      vg_CB_XZEA26 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_MAT_NAV */
	/*A Construction du nom du DataGroup */
	strcpy ( pl_NomDG, XDG_ETAT_MAT_NAV );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA26_Abt_Etat_Mat_NAV Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


