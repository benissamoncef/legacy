/*E*/
/*Fichier : $Id: xzen.c,v 1.7 2007/03/26 10:56:08 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2007/03/26 10:56:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEN  * FICHIER xzen.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la nav
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Nagiel.E	version 1.2	16 Nov 1994	: Maj
* Mismer.D	version 1.3	28 Dec 1994	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Fontaine.C	version 1.5	24 Mai 1995	:
* JMG		version 1.6	28/09/05 : ajout cmd BAF et BAD
------------------------------------------------------*/

/* fichiers inclus */
#include <xdy.h>
#include <xdc.h>

#include "xzen.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzen.c	1.1 08/25/94 : xzen" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande NAV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN01_Commande_NAV (	XZENT_CMD_NAV	va_Commande_NAV,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_NAV	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_NAV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_NAV_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN01_Commande_NAV avec %d %d %s %d %d %s"
        ,va_Commande_NAV.NoEqt
        ,va_Commande_NAV.Module
        ,va_Commande_NAV.Symb
        ,va_Commande_NAV.Flash
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_NAV_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_NAV, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_NAV en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_NAV)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_NAV.NoEqt
                          ,T_IPC_FT_INT2, va_Commande_NAV.Module
                          ,T_IPC_FT_STR,  va_Commande_NAV.Symb
                          ,T_IPC_FT_INT2, va_Commande_NAV.Flash
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PICTO non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_NAV non effectue \n");
    
		/*A Trace sortie fonction XZEN01_Commande_NAV avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN01_Commande_NAV avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_NAV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_NAV effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN01_Commande_NAV avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN01_Commande_NAV avec XDC_OK");
 	
   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN02_Commande_SCEN (	XZENT_CMD_SCEN	va_Commande_SCEN,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_<NomMachine> du site concerne
*
------------------------------------------------------*/
{

 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN02_Commande_SCEN  avec %d %s %d %s"
        ,va_Commande_SCEN.NoEqt
        ,va_Commande_SCEN.Scenar
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_SCEN_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SCEN, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_SCEN en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_SCEN)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_SCEN.NoEqt
                          ,T_IPC_FT_STR,  va_Commande_SCEN.Scenar
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_SCEN non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_SCEN non effectue \n");
    
		/*A Trace sortie fonction XZEN02_Commande_SCEN avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN02_Commande_SCEN avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SCEN effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_SCEN effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN02_Commande_SCEN avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN02_Commande_SCEN avec XDC_OK");
 	
   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par fichier scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN03_Config_Matrice_Test (XZENT_CMD_MAT_NAV	va_Commande_MAT,
				int			va_NoCMD,
				XDY_NomMachine		pa_NomMachine )	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN_FICH	Commande a effectuee
*   va_NoCMD			Numero de la commande
*   va_NomMachine		Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN_FICH en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_FICH_<NomMachine> 
*   du site concerne
*
------------------------------------------------------*/
{
 XDY_Datagroup	     		pl_NomDG;
 
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN03_Config_Matrice_Test  avec %d %s (params) %d %s"
        ,va_Commande_MAT.NoEqt
        ,va_Commande_MAT.Matrice
 	,va_NoCMD
 	,pa_NomMachine);
 	
   /*A Construction du DatGroup XDG_ECMD_MAT_NAV_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_MAT_NAV, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_MAT_NAV en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_MAT_NAV)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_MAT.NoEqt
                          ,T_IPC_FT_STR,  va_Commande_MAT.Matrice
                          ,T_IPC_FT_INT2_ARRAY,  va_Commande_MAT.ListeParam , XZENC_Nb_Params_Matrice
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_MAT_NAV non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_MAT_NAV non effectue \n");
    
		/*A Trace sortie fonction XZEN03_Config_Matrice_Test avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN03_Config_Matrice_Test avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_MAT_NAV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_MAT_NAV effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN03_Config_Matrice_Test avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN03_Config_Matrice_Test avec XDC_OK");
 	

   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN04_Arret_SCEN (	XDY_Eqt		va_NoEqt,
			int		va_NoCMD,
			XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_<NomMachine> du site concerne
*
------------------------------------------------------*/
{

 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN04_Arret_SCEN  avec %d %d %s"
 	,va_NoEqt
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_ARRET_SCEN_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_ARRET_SCEN, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_ARRET_SCEN en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_ARRET_SCEN)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_NoEqt
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_ARRET_SCEN non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_ARRET_SCEN non effectue \n");
    
		/*A Trace sortie fonction XZEN04_Arret_SCEN avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN04_Arret_SCENXZEN02_Commande_SCEN avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_ARRET_SCEN effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_ARRET_SCEN effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN04_Arret_SCEN avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN04_Arret_SCEN avec XDC_OK");
 	
   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande BAF
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN10_Commande_BAF (	XZENT_CMD_BAF	va_Commande_BAF,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_NAV	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage BAF. 
*
* FONCTION 
*   Construction du message XDM_ECMD_BAD en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_BAF_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
 XDY_Datagroup	     		pl_NomDG;
  char	vl_str[10];

    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_WARNING
 	,"IN: XZEN10_Commande_BAF avec %d %d %d %d %d %s"
        ,va_Commande_BAF.NoEqt
        ,va_Commande_BAF.Module
        ,va_Commande_BAF.Message
        ,va_Commande_BAF.Cligno
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_BAF_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_BAF, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_NAV en fournissant les valeur recues  */
   
   sprintf(vl_str,"%d",va_Commande_BAF.Message);
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_BAF)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_BAF.NoEqt
                          ,T_IPC_FT_INT2, va_Commande_BAF.Module
                          ,T_IPC_FT_STR,  vl_str
                          ,T_IPC_FT_INT2, va_Commande_BAF.Cligno
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PICTO non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_BAF non effectue sur %s\n",pl_NomDG);
    
		/*A Trace sortie fonction XZEN10_Commande_BAF avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN10_Commande_BAF avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_NAV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_BAF effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN10_Commande_BAF avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN10_Commande_BAF avec XDC_OK");
 	
   return ( XDC_OK );   

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande BAD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN11_Commande_BAD (	XZENT_CMD_BAD	va_Commande_BAD,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_NAV	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage BAF. 
*
* FONCTION 
*   Construction du message XDM_ECMD_BAD en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_BAF_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN11_Commande_BAD avec %d %d %s %d %d %s"
        ,va_Commande_BAD.NoEqt
        ,va_Commande_BAD.Module
        ,va_Commande_BAD.Message
        ,va_Commande_BAD.Cligno
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_BAD_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_BAD, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_NAV en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_BAD)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_BAD.NoEqt
                          ,T_IPC_FT_INT2, va_Commande_BAD.Module
                          ,T_IPC_FT_STR,  va_Commande_BAD.Message
                          ,T_IPC_FT_INT2, va_Commande_BAD.Cligno
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_BAD non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_BAD non effectue \n");
    
		/*A Trace sortie fonction XZEN11_Commande_BAD avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN11_Commande_BAD avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_NAV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_BAD effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN10_Commande_BAF avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN11_Commande_BAD avec XDC_OK");
 	
   return ( XDC_OK );   

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN13_Commande_SCEN_BAD (	XZENT_CMD_SCEN_BAD	va_Commande_SCEN,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_<NomMachine> du site concerne
*
------------------------------------------------------*/
{

 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN13_Commande_SCEN_BAD  avec %d %s %d %s"
        ,va_Commande_SCEN.NoEqt
        ,va_Commande_SCEN.Scenar
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_SCEN_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SCEN_BAD, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_SCEN en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_SCEN)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_SCEN.NoEqt
                          ,T_IPC_FT_STR,  va_Commande_SCEN.Scenar
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_SCEN non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_SCEN non effectue \n");
    
		/*A Trace sortie fonction XZEN02_Commande_SCEN avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN13_Commande_SCEN_BAD avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SCEN effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_SCEN effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN02_Commande_SCEN avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN13_Commande_SCEN_BAD avec XDC_OK");
 	
   return ( XDC_OK );   

}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande BAD par scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN15_Arret_SCEN_BAD (	XDY_Eqt		va_NoEqt,
			int		va_NoCMD,
			XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_<NomMachine> du site concerne
*
------------------------------------------------------*/
{

 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN14_Arret_SCEN_BAD  avec %d %d %s"
 	,va_NoEqt
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_ARRET_SCEN_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_ARRET_SCEN_BAD, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_ARRET_SCEN en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_ARRET_SCEN)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_NoEqt
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_ARRET_SCEN non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_ARRET_SCEN non effectue \n");
    
		/*A Trace sortie fonction XZEN04_Arret_SCEN avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN14_Arret_SCEN_BAD avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_ARRET_SCEN effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_ARRET_SCEN effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN04_Arret_SCEN avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN14_Arret_SCEN_BAD avec XDC_OK");
 	
   return ( XDC_OK );   

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande BAF globale
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEN16_Commande_BAF_globale (	XZENT_CMD_BAF	va_Commande_BAF[6],
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_NAV	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage BAF. 
*
* FONCTION 
*   Construction du message XDM_ECMD_BAD en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_BAF_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
 XDY_Datagroup	     		pl_NomDG;
  char	vl_str1[10];
  char	vl_str2[10];
  char	vl_str3[10];
  char	vl_str4[10];
  char	vl_str5[10];
  char	vl_str6[10];

    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEN16_Commande_BAF_globale avec %d %d %d %d %d %s"
        ,va_Commande_BAF[0].NoEqt
        ,va_Commande_BAF[0].Module
        ,va_Commande_BAF[0].Message
        ,va_Commande_BAF[0].Cligno
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_BAF_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_BAF, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_NAV en fournissant les valeur recues  */
   
   sprintf(vl_str1,"%d",va_Commande_BAF[0].Message);
   sprintf(vl_str2,"%d",va_Commande_BAF[1].Message);
   sprintf(vl_str3,"%d",va_Commande_BAF[2].Message);
   sprintf(vl_str4,"%d",va_Commande_BAF[3].Message);
   sprintf(vl_str5,"%d",va_Commande_BAF[4].Message);
   sprintf(vl_str6,"%d",va_Commande_BAF[5].Message);
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_BAF_GLOBALE)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_BAF[0].NoEqt
                          ,T_IPC_FT_STR,  vl_str1
                          ,T_IPC_FT_STR,  vl_str2
                          ,T_IPC_FT_STR,  vl_str3
                          ,T_IPC_FT_STR,  vl_str4
                          ,T_IPC_FT_STR,  vl_str5
                          ,T_IPC_FT_STR,  vl_str6
                          ,T_IPC_FT_INT2, va_Commande_BAF[0].Cligno
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PICTO non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_BAF_GLOBALE non effectue sur %s\n",pl_NomDG);
    
		/*A Trace sortie fonction XZEN16_Commande_BAF avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEN16_Commande_BAF_globale avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_NAV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_BAF effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEN10_Commande_BAF_globale avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEN16_Commande_BAF_globale avec XDC_OK");
 	
   return ( XDC_OK );   

}

