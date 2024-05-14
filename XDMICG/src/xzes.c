/*E*/
/*Fichier : $Id: xzes.c,v 1.3 1995/05/24 11:31:51 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/24 11:31:51 $
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZES  * FICHIER xzes.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le mode d'interface SATIR
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	29 Dec 1994	: Creation
* Gabert.P	version 1.2	05 Jan 1995	:
* Fontaine.C	version 1.3	24 Mai 1995	: Reprise historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzes.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzes.c,v 1.3 1995/05/24 11:31:51 cornu Exp $ : xzes" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*---------------------------------
* SERVICE RENDU : 
*  Demande de creation d'un fichier
*
-----------------------------------
* SEQUENCE D'APPEL :
*/

 	int XZES01_Creer_Fichier_SATIR ( XZEST_FIC_SAT	va_Fichier_SAT,
					 XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Fichier_SAT	Info premettant de retrouver l'evenement a creer
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation d'un evenement
*
* FONCTION 
*   Construction du message XDM_ECMD_FIC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SATIR<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "$Id: xzes.c,v 1.3 1995/05/24 11:31:51 cornu Exp $ : XZES01_Creer_Fichier_SATIR" ;
XDY_Datagroup	pl_NomDG;
    
  /*A Construction du DatGroup XDG_ECMD_SATIR_<NomMachine> */
   
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SATIR, strtok ( pa_NomMachine, " ") ) ;
   	
  /*A Envoi du message XDM_ECMD_FIC en fournissant les valeur recues  */

  if ( !TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_FIC)
  			  ,XDM_FLG_SRVMSGWRITE

                          ,T_IPC_FT_INT4, 	va_Fichier_SAT.NoEvt

                          ,T_IPC_FT_INT2,	va_Fichier_SAT.CleEvt
 
                          ,T_IPC_FT_REAL8,	va_Fichier_SAT.Horodate

                          ,NULL))
  {
    /*A Trace envoi message XDM_ECMD_FIC non effectue */    	
      	
    XZST_03EcritureTrace(XZSTC_INFO
		   ,"Envoi message XDM_ECMD_FIC non effectue \n");
    
    return(	XDC_NOK );		
  }
  else
  {
    /*A RAZ Buffer envoi du message */
   
    TipcSrvFlush();

    /*A Trace envoi message XDM_ECMD_FIC effectue avec succes */    	
      	
    XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_FIC effectue avec succes sur %s\n",pl_NomDG);
                   
    return( XDC_OK );		
  }

}

