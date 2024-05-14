/*E*/
/*Fichier : @(#)xzea04.c	1.7      Release : 1.7        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea04.c
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
* Mismer.D	version 1.4	08 Nov 1994	: Suppression arg NumEqt pour XZEA04
* Mismer.D	version 1.5	28 Jan 1995	:
* Fontaine.C	version 1.6	23 Mai 1995	: Reprise entete
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

static char *version = "@(#)xzea04.c	1.7 12/13/95 : xzea04" ;
XDY_Fonction		pg_FonctUtil_XZEA_04 = NULL;
T_CB			vg_CB_XZEA04         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_EMes_6mn_Chaussee ( 
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
*  XDM_EMES_6mn_Chaussee 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea04.c	1.7 12/13/95 :  XZEA_EMes_6mn_Chaussee ";
   XZEAT_EMes_RADT	vl_Mes;
#ifdef _TIBCO_EMS
   XDY_Horodate		vl_Horodate;
   XDY_Mot		vl_Numero, vl_Periode, vl_Sens, vl_Voie,
   			vl_MesQ, vl_MesV, vl_MesTO, vl_MesPL,
   			vl_MesQ_Ind, vl_MesV_Ind, vl_MesTO_Ind, vl_MesPL_Ind;
#else
   T_REAL8		vl_Horodate;
   T_INT2		vl_Numero, vl_Periode, vl_Sens, vl_Voie,
   			vl_MesQ, vl_MesV, vl_MesTO, vl_MesPL,
   			vl_MesQ_Ind, vl_MesV_Ind, vl_MesTO_Ind, vl_MesPL_Ind;
#endif
   int			vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_EMES_6mn_Chaussee */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8, &vl_Horodate,
               T_IPC_FT_INT2,  &vl_Numero,
               T_IPC_FT_INT2,  &vl_Periode,
               T_IPC_FT_INT2,  &vl_Sens,
               T_IPC_FT_INT2,  &vl_Voie,
               T_IPC_FT_INT2,  &vl_MesQ,
               T_IPC_FT_INT2,  &vl_MesQ_Ind,
               T_IPC_FT_INT2,  &vl_MesV,
               T_IPC_FT_INT2,  &vl_MesV_Ind,
               T_IPC_FT_INT2,  &vl_MesTO,
               T_IPC_FT_INT2,  &vl_MesTO_Ind,
               T_IPC_FT_INT2,  &vl_MesPL,
               T_IPC_FT_INT2,  &vl_MesPL_Ind,
	       NULL); 
	       
   vl_Mes.Horodate = vl_Horodate;

   XZST_03EcritureTrace ( XZSTC_FONCTION, 
   "IN  : XZEA_EMes_6mn_Chaussee  Horodate:%lf Numero:%d Periode:%d Sens:%d Voie:%d Q:%d V:%d TO:%d PL:%d.",
     				vl_Mes.Horodate, vl_Numero, vl_Periode, vl_Sens, vl_Voie, 
     				vl_MesQ,  vl_MesV, vl_MesTO,  vl_MesPL );

   /*A Renseignement de la structure XZEAT_EMes_RADT */
   vl_Mes.Numero		= vl_Numero;
   vl_Mes.Periode		= vl_Periode;
   vl_Mes.Sens			= vl_Sens;
   vl_Mes.Voie			= vl_Voie;
   vl_Mes.Mesures.ValQ.Val	= vl_MesQ;
   vl_Mes.Mesures.ValQ.Indic	= vl_MesQ_Ind;
   vl_Mes.Mesures.ValV.Val	= vl_MesV;
   vl_Mes.Mesures.ValV.Indic	= vl_MesV_Ind;
   vl_Mes.Mesures.ValTO.Val	= vl_MesTO;
   vl_Mes.Mesures.ValTO.Indic	= vl_MesTO_Ind;
   vl_Mes.Mesures.ValPL.Val	= vl_MesPL;
   vl_Mes.Mesures.ValPL.Indic	= vl_MesPL_Ind;
	       
   /*A Appel de la fonction utilsateur avec la structure XZEAT_EMes_RADT en argument */
   pg_FonctUtil_XZEA_04 ( vl_Mes, &vl_Resultat );
   XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_EMes_6mn_Chaussee %d", vl_Resultat );
}
/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA04_Abt_Mesures_6mn_Chaussee (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux mesures 6mn  
*   par chaussee d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea04.c	1.7 12/13/95 : XZEA04_Abt_Mesures_6mn_Chaussee ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA04_Abt_Mesures_6mn_Chaussee Abt:%d DGInit:%s Fonct:%ld NomSite:%s .",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite);
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_04 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_EMES_6mn_Chaussee */
	if ( vg_CB_XZEA04 == NULL )
	{
	   if ( ( vg_CB_XZEA04 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_6mn_Chaussee),
							      	XZEA_EMes_6mn_Chaussee,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_6mn_Chaussee ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_6mn_Chaussee ." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EMES_RADT_6MN_<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_RADT_6MN, pa_NomSite );
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
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA04 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_EMES_6mn_Chaussee */
	   if ( TutCbDestroy ( vg_CB_XZEA04 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_EMES_6mn_Chaussee ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_EMES_6mn_Chaussee ." );
	      vg_CB_XZEA04 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EMES_RADT_6MN_<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_RADT_6MN, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA04_Abt_Mesures_6mn_Chaussee Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


