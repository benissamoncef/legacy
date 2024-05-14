/*E*/
/*Fichier : @(#)xzea27.c	1.7      Release : 1.7        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea27.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Sep 1994	: Creation
* Mismer.D	version 1.2	14 Oct 1994	:
* Mismer.D	version 1.3	14 Oct 1994	:
* Mismer.D	version 1.4	26 Oct 1994	: Ajout static version
* Mismer.D	version 1.5	28 Jan 1995	:
* Fontaine.C	version 1.6	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.7	29 Nov 1995	: Correction condition fermeture datagroup d'init
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea27.c	1.7 12/13/95 : xzea27" ;
XDY_Fonction		pg_FonctUtil_XZEA_27 = NULL;
T_CB			vg_CB_XZEA27         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_EMes_Classe_DIV ( 
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
*  XDM_EMES_Classe_DIV 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea27.c	1.7 12/13/95 : XZEA_EMes_Classe_DIV ";
   XZEAT_EMes_Classe_DIV	vl_Mes;
#ifndef _TIBCO_EMS
   T_REAL8              vl_Horodate;
   T_INT2			vl_Numero, vl_Sens, vl_DIVcl1, vl_DIVcl2, vl_DIVcl3, vl_DIVcl4, vl_DIVcl5;
#else
   XDY_Horodate         vl_Horodate;
   XDY_Mot			vl_Numero, vl_Sens, vl_DIVcl1, vl_DIVcl2, vl_DIVcl3, vl_DIVcl4, vl_DIVcl5;
#endif
   int				vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_EMES_Classe_DIV */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8, &vl_Horodate,
               T_IPC_FT_INT2,  &vl_Numero,
               T_IPC_FT_INT2,  &vl_Sens,
               T_IPC_FT_INT2,  &vl_DIVcl1,
               T_IPC_FT_INT2,  &vl_DIVcl2,
               T_IPC_FT_INT2,  &vl_DIVcl3,
               T_IPC_FT_INT2,  &vl_DIVcl4,
               T_IPC_FT_INT2,  &vl_DIVcl5,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_Classe_DIV dans XZEA_EMes_Classe_DIV ");
   }
   else
   {	       

      vl_Mes.Horodate=vl_Horodate;
      XZST_03EcritureTrace ( XZSTC_FONCTION, 
      "IN  : XZEA_EMes_Classe_DIV  Horodate:%lf Numero:%d Sens:%d DIV Cl1:%d DIV Cl2:%d DIV Cl3:%d DIV Cl4:%d DIV Cl5:%d.",
     				   vl_Mes.Horodate, vl_Numero,  vl_Sens,  
     				   vl_DIVcl1, vl_DIVcl2, vl_DIVcl3, vl_DIVcl4, vl_DIVcl5 );

      /*A Renseignement de la structure XZEAT_EMes_Classe_DIV */
      vl_Mes.Numero		= vl_Numero;
      vl_Mes.Sens		= vl_Sens;
      vl_Mes.DIVcl1		= vl_DIVcl1;
      vl_Mes.DIVcl2		= vl_DIVcl2;
      vl_Mes.DIVcl3		= vl_DIVcl3;
      vl_Mes.DIVcl4		= vl_DIVcl4;
      vl_Mes.DIVcl5		= vl_DIVcl5;
	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_EMes_Classe_DIV en argument */
      pg_FonctUtil_XZEA_27 ( vl_Mes, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_EMes_Classe_DIV %d", vl_Resultat );
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

int	XZEA27_Abt_Classe_DIV (		XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					XDY_Mot		va_Numero,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite		: Nom du site demande
* XDY_Mot	va_Numero		: Numero d'equipement
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures de debit par classe de DIV   
*   par voie d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea27.c	1.7 12/13/95 : XZEA27_Abt_Classe_DIV ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA27_Abt_Classe_DIV Abt:%d DGInit:%s Fonct:%ld NomSite:%s Numero:%d.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite,
     		va_Numero );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_27 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_EMES_Classe_DIV */
	if ( vg_CB_XZEA27 == NULL )
	{
	   if ( ( vg_CB_XZEA27 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_Classe_DIV),
							      	XZEA_EMes_Classe_DIV,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_Classe_DIV ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_Classe_DIV ." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EMES_CLASSE_DIV<Numero> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%d", XDG_EMES_CLASSE_DIV,va_Numero );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_Classe_DIV */
           /*A Construction du DatGroup XDG_EInit_IHM_RADT_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_RADT, pa_NomSite );
	   
           /*A Envoi du message XDM_EInit_IHM_Classe_DIV en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_Classe_DIV),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		T_IPC_FT_INT2, va_Numero,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_Classe_DIV non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_Classe_DIV");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_Classe_DIV */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_Classe_DIV sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA27 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_EMES_Classe_DIV */
	   if ( TutCbDestroy ( vg_CB_XZEA27 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_EMES_Classe_DIV ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_EMES_Classe_DIV ." );
	      vg_CB_XZEA27 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EMES_CLASSE_DIV<Numero> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%d", XDG_EMES_CLASSE_DIV, va_Numero );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA27_Abt_Classe_DIV Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


