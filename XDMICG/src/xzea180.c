/*E*/
/*Fichier : 	@(#)xzea0180.c	1.2 Release : 1.2 Date : 03/26/07
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea0180.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea0180.c	1.2 1.2 03/26/07 : xzea0180" ;
XDY_Fonction		pg_FonctUtil_XZEA_0180 = NULL;
T_CB			vg_CB_XZEA0180         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_Eqt_SAGA ( 
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
   static char *version = "@(#)xzea0180.c	1.8 09/09/96 : XZEA_Etat_PMV ";
   XZEAT_Etat_Eqt_SAGA	vl_Etat;
   		
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero, 
   			vl_Affichage1, 
   			vl_Affichage2, 
   			vl_Affichage3,
   			vl_Affichage4,
   			vl_Affichage5,
			vl_Clign,vl_Lumino,vl_barriere;
   T_STR		vl_Texte1, vl_Texte2, vl_Texte3, vl_Texte4, vl_Texte5,vl_picto,
			vl_bandeau,
   			vl_Alternat1, vl_Alternat2, vl_Alternat3, vl_Alternat4,vl_Alternat5;
   T_INT4		vl_scenario;
   T_INT4		vl_typesaga;	
#else
   XDY_Mot		vl_Numero, 
   			vl_Affichage1, 
   			vl_Affichage2, 
   			vl_Affichage3,
   			vl_Affichage4,
   			vl_Affichage5,
			vl_Clign,vl_Lumino,vl_barriere;
   XDY_Ligne_PMV		vl_Texte1, vl_Texte2, vl_Texte3, vl_Texte4, vl_Texte5,vl_picto,
			vl_bandeau,
   			vl_Alternat1, vl_Alternat2, vl_Alternat3, vl_Alternat4,vl_Alternat5;
   XDY_Entier		vl_scenario;
   XDY_Entier		vl_typesaga;	
#endif
   int			vl_Resultat = XDC_OK;
   XDY_NomMachine vl_machine="";

   
		  
   /* Recuperation des parametres du message XDM_ETAT_EQT_SAGA */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_STR,         &vl_Texte1,
               T_IPC_FT_INT2,        &vl_Affichage1,
               T_IPC_FT_STR,         &vl_Alternat1,
               T_IPC_FT_STR,         &vl_Texte2,
               T_IPC_FT_INT2,        &vl_Affichage2,
               T_IPC_FT_STR,         &vl_Alternat2,
               T_IPC_FT_STR,         &vl_Texte3,
               T_IPC_FT_INT2,        &vl_Affichage3,
               T_IPC_FT_STR,         &vl_Alternat3,
               T_IPC_FT_STR,         &vl_Texte4,
               T_IPC_FT_INT2,        &vl_Affichage4,
               T_IPC_FT_STR,         &vl_Alternat4,
               T_IPC_FT_STR,         &vl_Texte5,
               T_IPC_FT_INT2,        &vl_Affichage5,
               T_IPC_FT_STR,         &vl_Alternat5,
	       T_IPC_FT_INT2,		&vl_Clign,
	       T_IPC_FT_INT2,		&vl_Lumino,
	       T_IPC_FT_STR,	&vl_picto,
	       T_IPC_FT_STR,	&vl_bandeau,
	       T_IPC_FT_INT2,	&vl_barriere,
	       T_IPC_FT_INT4,	&vl_scenario,
	       T_IPC_FT_INT4,   &vl_typesaga,
	       NULL) ) 
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_EQT_SAGA dans XZEA_Etat_EQT_SAGA ");
   }
   else
   {	       
      XZST_03EcritureTrace ( XZSTC_DEBUG1, 
      "IN  : XZEA_Etat_EQT_SAGA  Numero:%d typeSAGA�:%d texte1:%s Alterna1:%s Affichage1:%d \n\
      texte2:%s Alterna2:%s Affichage2:%d \ntexte3:%s Alterna3:%s Affichage3:%d \n\
      texte4:%s Alterna4:%s Affichage4:%d \ntexte5:%s Alterna5:%s Affichage5:%d\n. ",vl_Numero,
				vl_typesaga,
     				vl_Texte1, vl_Alternat1,  vl_Affichage1,
     				vl_Texte2, vl_Alternat2,  vl_Affichage2,
     				vl_Texte3, vl_Alternat3,  vl_Affichage3,
     				vl_Texte4, vl_Alternat4, vl_Affichage4,
     				vl_Texte5, vl_Alternat5,  vl_Affichage5);
      XZST_03EcritureTrace ( XZSTC_DEBUG1,
      "IN  : XZEA_Etat_EQT_SAGA cligno %d\n, lumino %d\n,picto %s\n, bandeau %s\n, barriere %d\n, scenario %d",
				vl_Clign,vl_Lumino,vl_picto,vl_bandeau,vl_barriere,vl_scenario);
      /*A Renseignement de la structure XZEAT_Etat_Eqt_SAGA */
      vl_Etat.Numero		= vl_Numero;
      vl_Etat.TypeSAGA		= vl_typesaga;
      strcpy ( vl_Etat.Texte1, vl_Texte1 );
      vl_Etat.Affichage1	= vl_Affichage1;
      strcpy ( vl_Etat.Alternat1, vl_Alternat1 );
      strcpy ( vl_Etat.Texte2, vl_Texte2 );
      vl_Etat.Affichage2	= vl_Affichage2;
      strcpy ( vl_Etat.Alternat2, vl_Alternat2 );
      strcpy ( vl_Etat.Texte3, vl_Texte3 );
      vl_Etat.Affichage3	= vl_Affichage3;
      strcpy ( vl_Etat.Alternat3, vl_Alternat3 );
      strcpy ( vl_Etat.Texte4, vl_Texte4 );
      vl_Etat.Affichage4	= vl_Affichage4;
      strcpy ( vl_Etat.Alternat4, vl_Alternat4 );
      vl_Etat.Affichage5	= vl_Affichage5;
      strcpy ( vl_Etat.Alternat5, vl_Alternat5 );
      strcpy ( vl_Etat.Texte5, vl_Texte5 );
      vl_Etat.Clignotement = vl_Clign;
      vl_Etat.Luminosite = vl_Lumino;
      strcpy ( vl_Etat.Picto, vl_picto );
      strcpy ( vl_Etat.Bandeau, vl_bandeau );
      vl_Etat.Barriere = vl_barriere;
      vl_Etat.Scenario = vl_scenario;

      if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_Eqt_SAGA : probleme dans recuperation machine destinataire du message");
	  }

	  /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_PMV en argument */
      pg_FonctUtil_XZEA_0180 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_Eqt_SAGA %d", vl_Resultat );
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

int	XZEA180_Abt_Etat_Eqt_SAGA (		XDY_Booleen	va_Abonnement,
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

     XZST_03EcritureTrace ( XZSTC_INTERFACE ,
     "IN  : XZEA0180_Abt_Etat_Eqt_SAGA Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_0180 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_PMVA */
	if ( vg_CB_XZEA0180 == NULL )
	{
	   if ( ( vg_CB_XZEA0180 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_EQT_SAGA),
							      	XZEA_Etat_Eqt_SAGA,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_EQT_SAGA." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_EQT_SAGA." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_PMV<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_EQT_SAGA, pa_NomSite );
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
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_EQT_SAGA, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_PMVA en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_EQT_SAGA),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_PMV non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_EQT_SAGA.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_PMV */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PMVA sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA0180 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_EQT_SAGA */
	   if ( TutCbDestroy ( vg_CB_XZEA0180 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_PMV ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_PMV ." );
	      vg_CB_XZEA0180 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_PMV<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_EQT_SAGA, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA0180_Abt_Etat_PMV Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


