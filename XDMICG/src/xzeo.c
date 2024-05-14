/*E*/
/* Fichier : $Id: xzeo.c,v 1.5 1995/05/24 11:31:47 cornu Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/05/24 11:31:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEO  * FICHIER xzeo.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module d'interface messagerie operateur
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	version 1.1	29 Nov 1994	: Creation
* Nouaille.M	version 1.2 	27 Dec 1994	: Modif envoi message 
* Nouaille.M	version 1.3	04 Jan 1995	: Modif format des traces
* Nouaille.M	version 1.4	04 Jan 1995	:
* Fontaine.C	version 1.5	24 Mai 1995	: Reprise historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzeo.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: xzeo.c,v 1.5 1995/05/24 11:31:47 cornu Exp $ : xzeo" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande appel numero telephone messagerie operateur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEO01_Appel_Op_Num(XDY_NomSite 	va_nomSite,
			XDY_Tel		va_NumOp,
                        XDY_Tel		va_NumTel)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumOp :   no du poste operateur demandeur
* va_NumTel :  no de telephone a appeler
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_OPER sur le 
*   datagroup XDG_ECMD_OPER_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzeo.c,v 1.5 1995/05/24 11:31:47 cornu Exp $ :XZEO01_Appel_Op_Num " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEO01_Appel_Op_Num avec %s %s %s ",
        va_nomSite,
        va_NumOp,
        va_NumTel);
 
   
 		
   /*A Envoi du message XDM_ECMD_OPER */
   if (Envoyer_Mess_Oper (va_nomSite,
                        XDC_APPEL_NUM,
                        va_NumOp,
                        va_NumTel) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEO01_Appel_Op_Num avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEO01_Appel_Op_Num avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEO01_Appel_Op_Num avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande appel pour messagerie alphanumerique operateur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEO02_Appel_Op_AlphaNum(	XDY_NomSite 		va_nomSite,
				XDY_Tel			va_NumOp,
                        	XDY_Msg_ES80		va_Message)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumOp :   no du poste operateur demandeur
* va_Message : message a envoyer
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_OPER sur le 
*   datagroup XDG_ECMD_OPER_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzeo.c,v 1.5 1995/05/24 11:31:47 cornu Exp $ :XZEO02_Appel_Op_AlphaNum " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEO02_Appel_Op_AlphaNum avec %s %s %s ",
        va_nomSite,
        va_NumOp,
        va_Message);
 
   
 		
   /*A Envoi du message XDM_ECMD_OPER */
   if (Envoyer_Mess_Oper (va_nomSite,
                        XDC_APPEL_ALPHA,
                        va_NumOp,
                        va_Message) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEO02_Appel_Op_AlphaNum avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEO02_Appel_Op_AlphaNum avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEO02_Appel_Op_AlphaNum avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'envoi message 
*
------------------------------------------------------*/
 int Envoyer_Mess_Oper(XDY_NomSite 		va_nomSite,
                    int 			va_codecmd,
                    XDY_Tel			va_NumOp,
                    XDY_Msg_ES80		va_Message )
                    

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site 
* va_codecmd : no de commande
* va_NumOp :  no operateur
* va_Message :  message
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_OPER sur le 
*   datagroup XDG_ECMD_OPER_<nomSite>
*
------------------------------------------------------*/
{
XDY_Datagroup	pl_NomDG;

   /*A Construction du DatGroup XDG_ECMD_OPER_<nomSite> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_OPER, va_nomSite ) ;
   
   /*A Envoi du message XDM_ECMD_OPER en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_OPER)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT4,va_codecmd
 			  ,T_IPC_FT_STR, va_NumOp
 			  ,T_IPC_FT_STR, va_Message
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_OPER non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_INFO
		   ,"Envoi message XDM_ECMD_OPER non effectue \n");
    
         	return(	XDC_NOK );		
	}
        else
        {
                /*A RAZ Buffer envoi du message */
   
                TipcSrvFlush();

                /*A Trace envoi message XDM_ECMD_OPER effectue avec succes */    	
      	
                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_OPER effectue avec succes sur %s\n",pl_NomDG);
                   
         	return(	XDC_OK );		
        }
 
}
