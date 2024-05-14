/*E*/
/*Fichier : @(#)xzea06.c	1.7      Release : 1.7        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea06.c
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
* Fontaine.C	version 1.6	23 Mai 1995	: Reprise entete
* Mismer.D	version 1.7	29 Nov 1995	: Correction condition fermeture datagroup d'init
* JMG		version 1.8	10/05/2020	: EMS
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea06.c	1.7 12/13/95 : xzea06" ;
XDY_Fonction		pg_FonctUtil_XZEA_06 = NULL;
T_CB			vg_CB_XZEA06         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_EMes_DAI ( 
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
*  XDM_EMES_DAI 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea06.c	1.7 12/13/95 : XZEA_EMes_DAI ";
   XZEAT_EMes_DAI	vl_Mes;
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero, vl_NumAnalyseur, vl_Sens, vl_Chaussee;
   T_INT2	*pl_Vitesse, *pl_Longueur;
   T_INT4		vl_SizeVit = 0, vl_SizeLon = 0;
#else
   XDY_Mot		vl_Numero, vl_NumAnalyseur, vl_Sens, vl_Chaussee;
   XDY_Mot 		pl_Vitesse[10], pl_Longueur[10];
   XDY_Entier		vl_SizeVit = 0, vl_SizeLon = 0;
#endif
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
   
		  
   /* Recuperation des parametres du message XDM_EMES_DAI */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8,       &vl_Mes.Horodate,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_INT2,        &vl_NumAnalyseur,
               T_IPC_FT_INT2,        &vl_Sens,
               T_IPC_FT_INT2,        &vl_Chaussee,
               T_IPC_FT_INT2_ARRAY,  &pl_Vitesse, &vl_SizeVit,
               T_IPC_FT_INT2_ARRAY,  &pl_Longueur, &vl_SizeLon,
               T_IPC_FT_INT2,        &vl_Mes.TO,
               T_IPC_FT_INT2,        &vl_Mes.VitesseMoyenne,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_DAI dans XZEA_EMes_DAI ");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_FONCTION, 
      "IN  : XZEA_EMes_DAI  Horodate:%lf Numero:%d No d'analyseur:%d Sens:%d Chaussee:%d TO:%d Vit Moy:%d.",
     				vl_Mes.Horodate, vl_Numero, vl_NumAnalyseur, vl_Sens, vl_Chaussee, 
     				vl_Mes.TO, vl_Mes.VitesseMoyenne );

      /*A Renseignement de la structure XZEAT_EMes_DAI */
      vl_Mes.Numero		= vl_Numero;
      vl_Mes.NumAnalyseur	= vl_NumAnalyseur;
      vl_Mes.Sens		= vl_Sens;
      vl_Mes.Chaussee		= vl_Chaussee;
#ifndef _TIBCO_EMS
      for ( vl_Index = 0;
            (vl_Index < vl_SizeVit) && (vl_Index < sizeof(vl_Mes.VitesseVoie)/sizeof(vl_Mes.VitesseVoie[0]));
            vl_Mes.VitesseVoie[vl_Index] = *pl_Vitesse, vl_Index++, pl_Vitesse++);
      for ( vl_Index = 0;
            (vl_Index < vl_SizeLon) && (vl_Index < sizeof(vl_Mes.LongueurVoie)/sizeof(vl_Mes.LongueurVoie[0]));
            vl_Mes.LongueurVoie[vl_Index] = *pl_Longueur, vl_Index++, pl_Longueur++);
#else
      for ( vl_Index = 0;
            (vl_Index < vl_SizeVit) && (vl_Index < sizeof(vl_Mes.VitesseVoie)/sizeof(vl_Mes.VitesseVoie[0]));
            vl_Index++)
      	     vl_Mes.VitesseVoie[vl_Index] = pl_Vitesse[vl_Index] ;
      for ( vl_Index = 0;
            (vl_Index < vl_SizeLon) && (vl_Index < sizeof(vl_Mes.LongueurVoie)/sizeof(vl_Mes.LongueurVoie[0]));
            vl_Index++)
            vl_Mes.LongueurVoie[vl_Index] = pl_Longueur[vl_Index];
#endif
	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_EMes_DAI en argument */
      pg_FonctUtil_XZEA_06 ( vl_Mes, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_EMes_DAI %d", vl_Resultat );
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

int	XZEA06_Abt_Mesures_DAI (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux mesures DAI d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea06.c	1.7 12/13/95 : XZEA06_Abt_Mesures_DAI ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA06_Abt_Mesures_DAI Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_06 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_EMES_DAI */
	if ( vg_CB_XZEA06 == NULL )
	{
	   if ( ( vg_CB_XZEA06 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_DAI),
							      	XZEA_EMes_DAI,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_DAI." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_DAI." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EMES_DAI<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_DAI, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_DAI */
           /*A Construction du DatGroup XDG_EInit_IHM_DAI_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_DAI, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_DAI en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_DAI),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_DAI non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_DAI.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_DAI */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_DAI sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA06 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_EMES_DAI */
	   if ( TutCbDestroy ( vg_CB_XZEA06 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_EMES_DAI ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_EMES_DAI ." );
	      vg_CB_XZEA06 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EMES_DAI<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_DAI, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA06_Abt_Mesures_DAI Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


