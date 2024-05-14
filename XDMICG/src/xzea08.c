/*E*/
/*Fichier : @(#)xzea08.c	1.8      Release : 1.8        Date : 09/09/96
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea08.c
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
* Mismer.D	version 1.3	26 Oct 1994	: Ajout static version
* Mismer.D	version 1.4	20 Dec 1994	: Mise a jour char msg RTWORKS
* Mismer.D	version 1.5	28 Jan 1995	:
* Fontaine.C	version 1.6	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.7	29 Nov 1995	: Correction condition fermeture datagroup d'init
* Mismer.D	version 1.8	29 Aou 1996	: Ajout ligne direction pour PMV 4 lignes (DEM/1162)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea08.c	1.8 09/09/96 : xzea08" ;
XDY_Fonction		pg_FonctUtil_XZEA_08 = NULL;
T_CB			vg_CB_XZEA08         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_PMV ( 
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
*  XDM_ETAT_PMV 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea08.c	1.8 09/09/96 : XZEA_Etat_PMV ";
   XZEAT_Etat_PMV	vl_Etat;
#ifndef _TIBCO_EMS
  T_STR		vl_Texte1, vl_Texte2, vl_Texte3, vl_Texte4,
   			vl_Alternat1, vl_Alternat2, vl_Alternat3, vl_Alternat4;
   T_INT2		vl_Numero, 
   			vl_Affichage1, 
   			vl_Affichage2, 
   			vl_Affichage3,
   			vl_Affichage4;
   T_CHAR		vl_Flash,
   			vl_Clign1,
   			vl_Clign2,
   			vl_Clign3,
   			vl_Clign4;
#else
  XDY_Ligne_PMV		vl_Texte1, vl_Texte2, vl_Texte3, vl_Texte4,
   			vl_Alternat1, vl_Alternat2, vl_Alternat3, vl_Alternat4;
   XDY_Mot		vl_Numero, 
   			vl_Affichage1, 
   			vl_Affichage2, 
   			vl_Affichage3,
   			vl_Affichage4;
   char		vl_Flash,
   			vl_Clign1,
   			vl_Clign2,
   			vl_Clign3,
   			vl_Clign4;
#endif
 
  int			vl_Resultat = XDC_OK;
  XDY_NomMachine vl_machine="";
   
		  
   /* Recuperation des parametres du message XDM_ETAT_PMV */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_STR,         &vl_Texte1,
               T_IPC_FT_INT2,        &vl_Affichage1,
               T_IPC_FT_STR,         &vl_Alternat1,
               T_IPC_FT_CHAR,        &vl_Clign1,
               T_IPC_FT_STR,         &vl_Texte2,
               T_IPC_FT_INT2,        &vl_Affichage2,
               T_IPC_FT_STR,         &vl_Alternat2,
               T_IPC_FT_CHAR,        &vl_Clign2,
               T_IPC_FT_STR,         &vl_Texte3,
               T_IPC_FT_INT2,        &vl_Affichage3,
               T_IPC_FT_STR,         &vl_Alternat3,
               T_IPC_FT_CHAR,        &vl_Clign3,
               T_IPC_FT_STR,         &vl_Texte4,
               T_IPC_FT_INT2,        &vl_Affichage4,
               T_IPC_FT_STR,         &vl_Alternat4,
               T_IPC_FT_CHAR,        &vl_Clign4,
               T_IPC_FT_CHAR,        &vl_Flash,
	       NULL) ) 
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_PMV dans XZEA_Etat_PMV ");
   }
   else
   {	       
      XZST_03EcritureTrace ( XZSTC_WARNING, 
      "IN  : XZEA_Etat_PMV  Numero:%d texte1:%s Alterna1:%s Clign1:%d Affichage1:%d \n\
      texte2:%s Alterna2:%s Clign2:%d Affichage2:%d \ntexte3:%s Alterna3:%s Clign3:%d Affichage3:%d \n\
      texte4:%s Alterna4:%s Clign4:%d Affichage4:%d Flash:%d.",vl_Numero,
     				vl_Texte1, vl_Alternat1, vl_Clign1, vl_Affichage1,
     				vl_Texte2, vl_Alternat2, vl_Clign2, vl_Affichage2,
     				vl_Texte3, vl_Alternat3, vl_Clign3, vl_Affichage3,
     				vl_Texte4, vl_Alternat4, vl_Clign4, vl_Affichage4,
     				vl_Flash );

      /*A Renseignement de la structure XZEAT_Etat_PMV */
      vl_Etat.Numero		= vl_Numero;
      strcpy ( vl_Etat.Texte1, vl_Texte1 );
      vl_Etat.Affichage1	= vl_Affichage1;
      strcpy ( vl_Etat.Alternat1, vl_Alternat1 );
      vl_Etat.Clign1		= vl_Clign1;
      strcpy ( vl_Etat.Texte2, vl_Texte2 );
      vl_Etat.Affichage2	= vl_Affichage2;
      strcpy ( vl_Etat.Alternat2, vl_Alternat2 );
      vl_Etat.Clign2		= vl_Clign2;
      strcpy ( vl_Etat.Texte3, vl_Texte3 );
      vl_Etat.Affichage3	= vl_Affichage3;
      strcpy ( vl_Etat.Alternat3, vl_Alternat3 );
      vl_Etat.Clign3		= vl_Clign3;
      strcpy ( vl_Etat.Texte4, vl_Texte4 );
      vl_Etat.Affichage4	= vl_Affichage4;
      strcpy ( vl_Etat.Alternat4, vl_Alternat4 );
      vl_Etat.Clign4		= vl_Clign4;
      vl_Etat.Flash		= vl_Flash;

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_PMV : probleme dans recuperation machine destinataire du message");
	  }
	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_PMV en argument */
      pg_FonctUtil_XZEA_08 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_PMV %d", vl_Resultat );
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

int	XZEA08_Abt_Etat_PMV (		XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des PMV d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA08_Abt_Etat_PMV Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_08 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_PMV */
	if ( vg_CB_XZEA08 == NULL )
	{
	   if ( ( vg_CB_XZEA08 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_PMV),
							      	XZEA_Etat_PMV,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_PMV." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_PMV." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_PMV<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PMV, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_PMV */
           /*A Construction du DatGroup XDG_EInit_IHM_PMV_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_PMV, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_PMV en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_PMV),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_PMV non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PMV.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_PMV */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PMV sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA08 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_PMV */
	   if ( TutCbDestroy ( vg_CB_XZEA08 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_PMV ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_PMV ." );
	      vg_CB_XZEA08 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_PMV<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PMV, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA08_Abt_Etat_PMV Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


