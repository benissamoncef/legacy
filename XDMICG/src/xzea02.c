/*E*/
/*  Fichier : $Id: xzea02.c,v 1.13 2020/11/03 17:57:28 pc2dpdy Exp $        Release : $Revision: 1.13 $        Date : $Date: 2020/11/03 17:57:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea02.c
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
* Mismer.D	version 1.4	28 Jan 1995	:
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.6	29 Nov 1995	: Correction condition fermeture datagroup d'init
* Mismer.D	version 1.7	22 Jan 1998	: Modif raf syno (DEM/1543)
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : format champs de 'printf' ou 'XDG_EncodeDG2'  1.11
* JPL		19/12/11 : Desabonnement du datagroup auquel l'abonnement a ete fait (DEM 997)  1.12
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzsa.h"
#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzea02.c,v 1.13 2020/11/03 17:57:28 pc2dpdy Exp $ $Revision: 1.13 $ $Date: 2020/11/03 17:57:28 $ : xzea02" ;
XDY_Fonction		pg_FonctUtil_XZEA_02 = NULL;
XDY_NomSite		vg_NomSite_XZEA_02 = "";
T_CB			vg_CB_XZEA02         = NULL;
int			vg_PC2_LT_XZEA_02 = 0;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_EMes_6gl_Chaussee ( 
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
*  XDM_EMES_6gl_Chaussee 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzea02.c,v 1.13 2020/11/03 17:57:28 pc2dpdy Exp $ $Revision: 1.13 $ $Date: 2020/11/03 17:57:28 $ : XZEA_EMes_6gl_Chaussee" ;
   XZEAT_EMes_RADT	vl_Mes;
   int			vl_Resultat = XDC_OK;
#ifndef _TIBCO_EMS
   T_STR		vl_Site;
   T_INT2		vl_Numero, vl_Periode, vl_Sens, vl_Voie,
   			vl_MesQ, vl_MesV, vl_MesTO, vl_MesPL,
   			vl_MesQ_Ind, vl_MesV_Ind, vl_MesTO_Ind, vl_MesPL_Ind;
#else
   XDY_NomSite		vl_Site;
   XDY_Mot		vl_Numero, vl_Periode, vl_Sens, vl_Voie,
   			vl_MesQ, vl_MesV, vl_MesTO, vl_MesPL,
   			vl_MesQ_Ind, vl_MesV_Ind, vl_MesTO_Ind, vl_MesPL_Ind;
#endif
   
		  
   /* Recuperation des parametres du message ES */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8, &vl_Mes.Horodate,
               T_IPC_FT_STR,   &vl_Site,
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
	       
   XZST_03EcritureTrace ( XZSTC_FONCTION, 
   "IN  : XZEA_EMes_6gl_Chaussee  Horodate:%lf Site:%s Numero:%d Periode:%d Sens:%d Voie:%d Q:%d V:%d TO:%d PL:%d.",
     				vl_Mes.Horodate, vl_Site, vl_Numero, vl_Periode, vl_Sens, vl_Voie, 
     				vl_MesQ,  vl_MesV, vl_MesTO,  vl_MesPL );

   if ( !strcmp(vl_Site, vg_NomSite_XZEA_02) && (vl_Periode == 6) && (vg_PC2_LT_XZEA_02 == XDC_FAUX) )
   {
  	XZST_03EcritureTrace ( XZSTC_FONCTION, "XZEA_EMes_6gl_Chaussee Raf 6 en local.");
   }
   else
   {
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
      pg_FonctUtil_XZEA_02 ( vl_Mes, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_EMes_6gl_Chaussee %d", vl_Resultat );
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

int	XZEA02_Abt_Mesures_6gl_Chaussee (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux mesures 6mn glissantes 
*   par chaussee d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "$Id: xzea02.c,v 1.13 2020/11/03 17:57:28 pc2dpdy Exp $ $Revision: 1.13 $ $Date: 2020/11/03 17:57:28 $ : XZEA02_Abt_Mesures_6gl_Chaussee" ;
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
int		vl_Type = 0;
XDY_NomMachine	vl_NomMachine = "";

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA02_Abt_Mesures_6gl_Chaussee Abt:%d DGInit:%s Fonct:%ld NomSite:%s .",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_02 = pa_FonctUtilisateur;
     strcpy ( vg_NomSite_XZEA_02, pa_NomSite );
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	sprintf ( vl_NomMachine, "SD%s1", pa_NomSite );
        XZSA_17TypeMachine ( vl_NomMachine, &vl_Type );
	if ( vl_Type == XZSCC_TYPEM_PC2LT )
	{
		vg_PC2_LT_XZEA_02 = XDC_VRAI;
	}
	else
	{
		vg_PC2_LT_XZEA_02 = XDC_FAUX;
	}



	/*A  Armement Callback sur Reception XDM_EMES_6gl_Chaussee */
	if ( vg_CB_XZEA02 == NULL )
	{
	   if ( ( vg_CB_XZEA02 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
							      	XZEA_EMes_6gl_Chaussee,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_6gl_Chaussee ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_6gl_Chaussee ." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EMES_RADT_6GL_R_<Site> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_RADT_6GL_R, pa_NomSite );
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
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( XDG_EMES_RADT_6GL_R, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( XDG_EMES_RADT_6GL_R ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", XDG_EMES_RADT_6GL_R );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", XDG_EMES_RADT_6GL_R );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_6gl_Chaussee */
           /*A Construction du DatGroup XDG_EInit_IHM_RADT_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_RADT, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_6gl_Chaussee en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_6gl_Chaussee),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		T_IPC_FT_INT2, XDC_EQTALL,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_6gl_Chaussee non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_6gl_Chaussee.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_6gl_Chaussee */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_6gl_Chaussee sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA02 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_EMES_6gl_Chaussee */
	   if ( TutCbDestroy ( vg_CB_XZEA02 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_EMES_6gl_Chaussee ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_EMES_6gl_Chaussee ." );
	      vg_CB_XZEA02 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EMES_RADT_6GL_R_<Site> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_RADT_6GL_R, pa_NomSite );
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
   	TipcSrvSubjectGetSubscribe ( XDG_EMES_RADT_6GL_R, &vl_EtatDg );
   	if ( vl_EtatDg )
   	{
   	   if ( !XDG_QuitterDG( XDG_EMES_RADT_6GL_R ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", XDG_EMES_RADT_6GL_R );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", XDG_EMES_RADT_6GL_R );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA02_Abt_Mesures_6gl_Chaussee Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


