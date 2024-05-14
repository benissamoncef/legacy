/*E*/
/*Fichier : @(#)xzea35.c	1.1      Release : 1.1        Date : 04/07/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea35.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux 6 TdP pour synoptique >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Fev 1997	: Creation pour TDP (DEM/1396)
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"
#include "xzea35.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea35.c	1.1 04/07/97 : xzea35" ;
XDY_Fonction		pg_FonctUtil_XZEA_35 = NULL;
T_CB			vg_CB_XZEA35         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_TdP_6mn_Zone ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_PTR                           pa_ARG )
 
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
*  XDM_EMES_TDP_6MN_PAR_ZONE 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea35.c	1.1 04/07/97 : XZEA_TdP_6mn_Zone ";
   XDY_EMes_TdP_6mn	vl_MesTDP6mn;
#ifndef _TIBCO_EMS
   T_REAL8		vl_Horodate;
   T_INT2		vl_Numero;
   T_INT2		vl_TDP6mn;
   T_INT2		vl_Validite6mn;
#else
   XDY_Horodate		vl_Horodate;
   XDY_Mot		vl_Numero;
   XDY_Mot		vl_TDP6mn;
   XDY_Mot		vl_Validite6mn;
#endif
   int			vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_EMES_TDP_6MN_PAR_ZONE */
   if ( !TipcMsgRead(pa_Data -> msg,
               	     	T_IPC_FT_REAL8,	&vl_Horodate,
               		T_IPC_FT_INT2, 	&vl_Numero,
               		T_IPC_FT_INT2, 	&vl_TDP6mn,
               		T_IPC_FT_INT2, 	&vl_Validite6mn,	 
	       		NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_TDP_6MN_PAR_ZONE dans XZEA_TdP_6mn_Zone ");
   }
   else
   {
      /*A Renseignement de la structure XDY_EMes_TdP_6mn */
      vl_MesTDP6mn.Horodate =   vl_Horodate;     
      vl_MesTDP6mn.Numero   =  (XDY_Eqt)     vl_Numero;      
      vl_MesTDP6mn.Dispo    =  (XDY_Booleen) vl_Validite6mn;        
      vl_MesTDP6mn.Temps    =  (XDY_TDP)     vl_TDP6mn;       
      
      XZST_03EcritureTrace ( XZSTC_INTERFACE,"XZEA_TdP_6mn_Zone appel fonct util avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tTemps\t: %d\n\tHorodate\t: %lf",
            				vl_MesTDP6mn.Numero,
            				vl_MesTDP6mn.Dispo,
            				vl_MesTDP6mn.Temps,
            				vl_MesTDP6mn.Horodate );
      	       
      /*A Appel de la fonction utilsateur avec la structure XDY_EMes_TdP_6mn en argument */
      pg_FonctUtil_XZEA_35 ( vl_MesTDP6mn, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_TdP_6mn_Zone %d", vl_Resultat );
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

int	XZEA35_Abt_TdP_6mn_Zone (	XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Eqt		va_NumZone,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Eqt	va_NumZone		: Numero du Zone demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesure TDP d'une Zone
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea35.c	1.1 04/07/97 : XZEA35_Abt_TdP_6mn_Zone ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA35_Abt_TdP_6mn_Zone Abt:%d DGInit:%s Fonct:%ld NumZone:%d.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		va_NumZone );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_35 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_EMES_TDP_6MN_PAR_ZONE */
	if ( vg_CB_XZEA35 == NULL )
	{
	   if ( ( vg_CB_XZEA35 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
							      	XZEA_TdP_6mn_Zone,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_TDP_6MN_PAR_ZONE." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_INFO, "valeur de vg_CB_XZEA35 : %ld", vg_CB_XZEA35); 
	      XZST_03EcritureTrace ( XZSTC_INFO, "Armement Callback sur XDM_EMES_TDP_6MN_PAR_ZONE." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EMES_UGTP_6MN_<NumZone> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%d", XDG_EMES_UGTP_6MN, va_NumZone );
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

           /*A Envoi du message de demande d'init XDM_EINIT_ABT en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	XDG_EINIT_ABT_TDP,
      			  		XDM_IdentMsg(XDM_EINIT_ABT),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		T_IPC_FT_INT4, XDC_EQT_ZDP,
                          		T_IPC_FT_INT4, va_NumZone,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EINIT_ABT non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EINIT_ABT.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EINIT_ABT */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EINIT_ABT sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA35 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_EMES_TDP_6MN_PAR_ZONE */
	   if ( TutCbDestroy ( vg_CB_XZEA35 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_EMES_TDP_6MN_PAR_ZONE ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_EMES_TDP_6MN_PAR_ZONE ." );
	      vg_CB_XZEA35 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EMES_UGTP_6MN_<NumZone> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%d", XDG_EMES_UGTP_6MN, va_NumZone );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA35_Abt_TdP_6mn_Zone Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


