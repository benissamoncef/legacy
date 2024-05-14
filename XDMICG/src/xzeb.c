/*E*/
/* Fichier : $Id: xzeb.c,v 1.2 2004/05/06 14:23:00 gesconf Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2004/05/06 14:23:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEB  * FICHIER xzeb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la pal
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Jan 1998	: Creation
* JMG		version 1.2	27/04/04	cmd par scenario
------------------------------------------------------*/

/* fichiers inclus */
#include <xdy.h>
#include <xdc.h>

#include "xzeb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzeb.c,v 1.2 2004/05/06 14:23:00 gesconf Exp $ : xzeb" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande PAL
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEB01_Commande_PAL (	XZEBT_CMD_PAL	va_Commande_PAL,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PAL	Commande a effectuee
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
*   Doit etre invoquee sur validation de la fenetre de pilotage PAL. 
*
* FONCTION 
*   Construction du message XDM_ECMD_PAL en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PAL_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
 XDY_Datagroup	     		pl_NomDG;
 int				vl_Index = 0;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEB01_Commande_PAL avec eqt=%d  scenario(%s) cmd=%d %s"
        ,va_Commande_PAL.Numero
        ,va_Commande_PAL.Scenario
 	,va_NoCMD
 	,pa_NomMachine);
 	
   /*A Construction du DatGroup XDG_ECMD_PAL_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PAL, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_ECMD_PAL en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_PAL)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PAL.Numero
                          ,T_IPC_FT_STR,  va_Commande_PAL.Scenario
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PAL non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_PAL non effectue \n");
    
		/*A Trace sortie fonction XZEB01_Commande_PAL avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEB01_Commande_PAL avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_PAL effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_PAL effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEB01_Commande_PAL avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEB01_Commande_PAL avec XDC_OK");
 	
   return ( XDC_OK );   

}


