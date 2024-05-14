/*E*/
/* Fichier : @(#)xzea24.c	1.6      Release : 1.6        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea24.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	21 Nov 1994	: Creation
* Mismer.D	version 1.2	02 Dec 1994	:
* Mismer.D	version 1.3	28 Jan 1995	:
* Mismer.D	version 1.4	18 Mai 1995	: Ajout numero echangeur en parametre d'entree 
* Fontaine.C	version 1.5	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.6	29 Nov 1995	: Correction condition fermeture datagroup d'init
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea24.c	1.6 12/13/95 : xzea24" ;
XDY_Fonction		pg_FonctUtil_XZEA_24 = NULL;
T_CB			vg_CB_XZEA24         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 void	XZEA_Etat_Sequence_Ech ( 
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
*  XDM_ETAT_SEQUENCE_ECH 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea24.c	1.6 12/13/95 : XZEA_Etat_Sequence_Ech ";
   XZEAT_Sequence	vl_Etat;
#ifndef _TIBCO_EMS
	T_INT4		vl_TypeEqt, vl_NumEqt, vl_SeqCourante, vl_SeqChange, vl_EtatSeq;
#else
	XDY_Entier	vl_TypeEqt, vl_NumEqt, vl_SeqCourante, vl_SeqChange, vl_EtatSeq;
#endif
   int			vl_Resultat = XDC_OK;
   XDY_NomMachine vl_machine="";

   /* Recuperation des parametres du message XDM_ETAT_SEQUENCE_ECH */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4,        &vl_TypeEqt,
               T_IPC_FT_INT4,        &vl_NumEqt,
               T_IPC_FT_INT4,        &vl_SeqCourante,
               T_IPC_FT_INT4,        &vl_SeqChange,
               T_IPC_FT_INT4,        &vl_EtatSeq,
	       NULL) ) 
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_SEQUENCE_ECH dans XZEA_Etat_Sequence_Ech ");
   }
   else
   {	       
      if ( vl_TypeEqt == XDC_EQT_ECH )
      {
         XZST_03EcritureTrace ( XZSTC_FONCTION, 
         "IN  : XZEA_Etat_Sequence_Ech TypeEqt:%d NumEqt:%d SeqCourant:%d SeqChange:%d EtatSeq:%d.",
      				    vl_TypeEqt,
      				    vl_NumEqt,
      				    vl_SeqCourante,
      				    vl_SeqChange,
      				    vl_EtatSeq );

         /*A Renseignement de la structure XZEAT_Sequence */
         vl_Etat.TypeEqt		= vl_TypeEqt;
         vl_Etat.NumEqt			= vl_NumEqt;
         vl_Etat.SeqCourante		= vl_SeqCourante;
         vl_Etat.SeqChange		= vl_SeqChange;
         vl_Etat.EtatSeq		= vl_EtatSeq;

		if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_Sequence_Ech : probleme dans recuperation machine destinataire du message");
	    }
  
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Sequence en argument */
      pg_FonctUtil_XZEA_24 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_Sequence_Ech %d", vl_Resultat );
      }
   }
}
/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats des sequences echangeur de la GTC de Nice  >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA24_Abt_Etat_Sequence_Ech (	XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					XDY_Eqt		va_NumEqt,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite		: Nom du site demande
* XDY_Eqt	va_NumEqt		: Numero d'echangeur 
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des sequences echangeurs de la GTC Nice
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea24.c	1.6 12/13/95 : XZEA24_Abt_Etat_Sequence_Ech ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA24_Abt_Etat_Sequence_Ech Abt:%d DGInit:%s Fonct:%ld NomSite:%s NumEch:%d.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite,
     		va_NumEqt );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_24 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_SEQUENCE_ECH */
	if ( vg_CB_XZEA24 == NULL )
	{
	   if ( ( vg_CB_XZEA24 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_SEQUENCE_ECH),
							      	XZEA_Etat_Sequence_Ech,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_SEQUENCE_ECH." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_SEQUENCE_ECH." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_SEQ_ECH<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_SEQ_ECH, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_NICE */
           /*A Construction du DatGroup XDG_EInit_IHM_NICE_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_NICE, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_NICE en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_NICE),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
               				T_IPC_FT_INT4, XDC_EQT_ECH,
               				T_IPC_FT_INT4, va_NumEqt,
               				T_IPC_FT_INT4, XDC_GTC_SEQUENCE,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_NICE non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_NICE.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_NICE */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_NICE sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA24 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_SEQUENCE_ECH */
	   if ( TutCbDestroy ( vg_CB_XZEA24 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_SEQUENCE_ECH ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_SEQUENCE_ECH ." );
	      vg_CB_XZEA24 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_SEQ_ECH<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_SEQ_ECH, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA24_Abt_Etat_Sequence_Ech Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


