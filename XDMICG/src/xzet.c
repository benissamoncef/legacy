/*E*/
/*Fichier : $Id: xzet.c,v 1.6 2008/06/16 11:40:44 devgtie Exp $      Release : $Revision: 1.6 $        Date : $Date: 2008/06/16 11:40:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZET  * FICHIER xzet.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface equipement Tunnel, Echangeur, 
*   via la gtc Nice >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Mismer.D	version 1.2	14 Nov 1994	: Remplacement des bouchons par le code
* Mismer.D	version 1.3	29 Nov 1994	:
* Fontaine.C	version 1.4	24 Mai 1995	: Reprise entete, historique
* AAZ       30/08/2007 DEM 662 ajout SIG, ECL, VEN, ENR, INC
*
------------------------------------------------------*/

/* fichiers inclus */
#include "xzet.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzet.c,v 1.6 2008/06/16 11:40:44 devgtie Exp $ : xzet" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation tunnel.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET01_Commande_SIGT (	XZETT_CMD_SIG_NIC	va_Commande_SIG_NIC,
				int			va_NoCMD )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_AFF	Numero d'affichage invalide
*   XZEXC_ERR_EQT	Numero de tunnel incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation tunnel
*   de la fenetre de pilotage Tunnel GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SIGT en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
static char *version = "$Id: xzet.c,v 1.6 2008/06/16 11:40:44 devgtie Exp $ : XZET01_Commande_SIGT " ;
XDY_Datagroup	pl_NomDG;
T_INT2		vl_Etat[5];      
   
    /*A Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: XZET01_Commande_SIGT avec NumTube: %d NumAction: %d",
     				va_Commande_SIG_NIC.NumTE, va_NoCMD );
 	
   /*A Envoi du message XDM_ECMD_SIGT en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
      			  XDM_IdentMsg(XDM_ECMD_SIGT),
  			  XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIC.NumPan),XZETC_MAX_PANNEAU,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIC.NumCmd),XZETC_MAX_PANNEAU,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_SIGT non effectue */    	
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SIGT non effectue.");
    
      /*A Trace sortie fonction XZET01_Commande_SIGT avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET01_Commande_SIGT avec XDC_NOK");
 	
      return ( XDC_NOK );
         				
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SIGT effectue avec succes */    	
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SIGT effectue avec succes");
   
   /*A 	Trace sortie fonction XZET01_Commande_SIGT avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET01_Commande_SIGT avec XDC_OK");

   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation echangeur.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET02_Commande_SIGE (	XZETT_CMD_SIG_NIC	va_Commande_SIG_NIC,
				int			va_NoCMD )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_AFF	Numero d'affichage invalide
*   XZEXC_ERR_EQT	Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation echangeur
*   de la fenetre de pilotage Echangeur GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SIGE en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
static char *version = "$Id: xzet.c,v 1.6 2008/06/16 11:40:44 devgtie Exp $ : XZET02_Commande_SIGE " ;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: XZET02_Commande_SIGE avec NumEch: %d NumPF: %d Rang: %d NumCmd: %d NumAction: %d",
     				va_Commande_SIG_NIC.NumTE, va_NoCMD );
 	
   /*A Envoi du message XDM_ECMD_SIGE en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
      			  XDM_IdentMsg(XDM_ECMD_SIGE),
  			  XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIC.NumPan),XZETC_MAX_PANNEAU,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIC.NumCmd),XZETC_MAX_PANNEAU,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_SIGE non effectue */    	
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SIGE non effectue.");
    
      /*A Trace sortie  */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET02_Commande_SIGE avec XDC_NOK");
 	
      return ( XDC_NOK );
         				
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SIGE effectue avec succes */    	
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SIGE effectue avec succes");
   
   /*A 	Trace sortie */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET02_Commande_SIGE avec XDC_OK");

   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence tunnel 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET03_Commande_SEQT (	XZETT_CMD_SEQ_NIC	va_Commande_SIG_NIC,
				int			va_NoCMD )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_SEQ	Numero de sequence invalide
*   XZEXC_ERR_EQT	Numero de tunnel incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence tunnel
*   de la fenetre de pilotage tunnel GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQT en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
static char *version = "$Id: xzet.c,v 1.6 2008/06/16 11:40:44 devgtie Exp $ : XZET03_Commande_SEQT " ;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: XZET03_Commande_SEQT avec NumTube: %d NumSeq: %d NumAction: %d",
     				va_Commande_SIG_NIC.NumTE,
       		 		va_Commande_SIG_NIC.NumSEQ,
 				va_NoCMD );
 	
   /*A Envoi du message XDM_ECMD_SEQT en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
      			  XDM_IdentMsg(XDM_ECMD_SEQT),
  			  XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumTE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumSEQ,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_SEQT non effectue */    	
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQT non effectue.");
    
      /*A Trace sortie  */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET03_Commande_SEQT avec XDC_NOK");
 	
      return ( XDC_NOK );
         				
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SEQT effectue avec succes */    	
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQT effectue avec succes");
   
   /*A 	Trace sortie */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET03_Commande_SEQT avec XDC_OK");

   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET04_Commande_SEQE (	XZETT_CMD_SEQ_NIC	va_Commande_SIG_NIC,
				int			va_NoCMD )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIC	Commande a effectuee
*   va_NoCMD		Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_SEQ	Numero de sequence invalide
*   XZEXC_ERR_EQT	Numero de echangeur incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence echangeur
*   de la fenetre de pilotage echangeur GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQE en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
static char *version = "$Id: xzet.c,v 1.6 2008/06/16 11:40:44 devgtie Exp $ : XZET03_Commande_SEQT " ;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: XZET03_Commande_SEQT avec NumEch: %d NumSeq: %d NumAction: %d",
     				va_Commande_SIG_NIC.NumTE,
       		 		va_Commande_SIG_NIC.NumSEQ,
 				va_NoCMD );
 	
   /*A Envoi du message XDM_ECMD_SEQE en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
      			  XDM_IdentMsg(XDM_ECMD_SEQE),
  			  XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumTE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIC.NumSEQ,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_SEQE non effectue */    	
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQE non effectue.");
    
      /*A Trace sortie  */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET03_Commande_SEQT avec XDC_NOK");
 	
      return ( XDC_NOK );
         				
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SEQE effectue avec succes */    	
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQE effectue avec succes");
   
   /*A 	Trace sortie */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET03_Commande_SEQT avec XDC_OK");

   return ( XDC_OK );   

}

/* AAZ */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation du domaine Signalisation.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET05_Commande_SIG    ( XZETT_CMD_SIG_NIX  va_Commande_SIG_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine Signalisation incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation du domaine Signalisation
*   de la fenetre de pilotage equipement du domaine Signalisation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SIG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 30/08/2007 : XZET05_Commande_SIG " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
    /*A Trace des parametres en entree */
    /*
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET05_Commande_SIG avec Eqt Signalisation: %d NumAction: %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD );
   */
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET05_Commande_SIG avec Eqt Signalisation: %d NumAction: %d Numseq %d NumTypeEqt[1] %d NumEqt[1] %d NumCmd[1] %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD,
                         va_Commande_SIG_NIX.NumSeq,
                         va_Commande_SIG_NIX.NumTypeEqt[0],
                         va_Commande_SIG_NIX.NumEqt[0],
                         va_Commande_SIG_NIX.NumCmd[0] );

   /*A Envoi du message XDM_ECMD_SIG en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_SIG),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumTypeEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumCmd),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_SIG non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SIG non effectue.");
    
      /*A Trace sortie fonction XZET05_Commande_SIG avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET05_Commande_SIG avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_SIG effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SIG effectue avec succes");
   
   /*A     Trace sortie fonction XZET05_Commande_SIG avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET05_Commande_SIG avec XDC_OK");

   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation du domaine Eclairage.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET06_Commande_ECL    ( XZETT_CMD_SIG_NIX  va_Commande_SIG_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine Eclairage incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation du domaine Eclairage
*   de la fenetre de pilotage equipement du domaine Eclairage GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_ECL en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 30/08/2007 : XZET06_Commande_ECL " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
    /*A Trace des parametres en entree */
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET06_Commande_ECL avec Eqt Eclairage: %d NumAction: %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD );
     
   /*A Envoi du message XDM_ECMD_ECL en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_ECL),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumTypeEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumCmd),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_ECL non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_ECL non effectue.");
    
      /*A Trace sortie fonction XZET06_Commande_ECL avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET06_Commande_ECL avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_ECL effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_ECL effectue avec succes");
   
   /*A     Trace sortie fonction XZET06_Commande_ECL avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET06_Commande_ECL avec XDC_OK");

   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation du domaine Ventilation.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET07_Commande_VEN    ( XZETT_CMD_SIG_NIX  va_Commande_SIG_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine Ventilation incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation du domaine Ventilation
*   de la fenetre de pilotage equipement du domaine Ventilation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_VEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 30/08/2007 : XZET07_Commande_VEN " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
    /*A Trace des parametres en entree */
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET07_Commande_VEN avec Eqt Ventilation: %d NumAction: %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD );
     
   /*A Envoi du message XDM_ECMD_VEN en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_VEN),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumTypeEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumCmd),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_VEN non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_VEN non effectue.");
    
      /*A Trace sortie fonction XZET07_Commande_VEN avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET07_Commande_VEN avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_VEN effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_VEN effectue avec succes");
   
   /*A     Trace sortie fonction XZET07_Commande_VEN avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET07_Commande_VEN avec XDC_OK");

   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation du domaine Energie.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET08_Commande_ENR ( XZETT_CMD_SIG_NIX  va_Commande_SIG_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine Energie incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation du domaine Energie
*   de la fenetre de pilotage equipement du domaine Energie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_ENR en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 30/08/2007 : XZET08_Commande_ENR " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
    /*A Trace des parametres en entree */
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET08_Commande_ENR avec Eqt Energie: %d NumAction: %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD );
     
   /*A Envoi du message XDM_ECMD_ENR en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_ENR),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumTypeEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumCmd),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_ENR non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_ENR non effectue.");
    
      /*A Trace sortie fonction XZET08_Commande_ENR avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET08_Commande_ENR avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_ENR effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_ENR effectue avec succes");
   
   /*A     Trace sortie fonction XZET08_Commande_ENR avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET08_Commande_ENR avec XDC_OK");

   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande signalisation du domaine Protection incendie.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET09_Commande_INC    ( XZETT_CMD_SIG_NIX  va_Commande_SIG_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine Protection incendie incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation du domaine Protection incendie
*   de la fenetre de pilotage equipement du domaine Protection incendie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_INC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 30/08/2007 : XZET09_Commande_INC " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
    /*A Trace des parametres en entree */
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET09_Commande_INC avec Eqt Protection incendie: %d NumAction: %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD );
     
   /*A Envoi du message XDM_ECMD_INC en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_INC),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumTypeEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumCmd),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_INC non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_INC non effectue.");
    
      /*A Trace sortie fonction XZET09_Commande_INC avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET09_Commande_INC avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_INC effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_INC effectue avec succes");
   
   /*A     Trace sortie fonction XZET09_Commande_INC avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET09_Commande_INC avec XDC_OK");

   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande domaine.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET10_Commande_Domaine ( XZETT_CMD_DOM_NIX  va_Commande_DOM_NIX,
                              int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine Protection incendie incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande signalisation du domaine Protection incendie
*   de la fenetre de pilotage equipement du domaine Protection incendie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_INC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 27/09/2007 : XZET10_Commande_Domaine " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
    /*A Trace des parametres en entree */
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET10_Commande_Domaine avec domaine : %d TypeOuvrage : %d IdOuvrage : %d Mode : %d NumCmd : %d",
                         va_Commande_DOM_NIX.Domaine,
                         va_Commande_DOM_NIX.TypeOuvrage,
                         va_Commande_DOM_NIX.IdOuvrage,
                         va_Commande_DOM_NIX.Mode,
                         va_NoCMD );
     
   /*A Envoi du message XDM_ECMD_DOMAINE en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_DOMAINE),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_DOM_NIX.Domaine,
                          T_IPC_FT_INT4, va_Commande_DOM_NIX.TypeOuvrage,
                          T_IPC_FT_INT4, va_Commande_DOM_NIX.IdOuvrage,
                          T_IPC_FT_INT4, va_Commande_DOM_NIX.Mode,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_DOMAINE non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_DOMAINE non effectue.");
    
      /*A Trace sortie fonction XZET10_Commande_Domaine avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET10_Commande_Domaine avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_DOMAINE effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_DOMAINE effectue avec succes");
   
   /*A     Trace sortie fonction XZET10_Commande_DOMAINE avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET10_Commande_DOMAINE avec XDC_OK");

   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande exploitation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET11_Commande_EXP    ( XZETT_CMD_SIG_NIX  va_Commande_SIG_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SIG_NIX    Commande a effectuee
*   va_NoCMD               Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_AFF    Numero d'affichage invalide
*   XZEXC_ERR_EQT    Numero de domaine incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande exploitation
* FONCTION 
*   Construction du message XDM_ECMD_EXP en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char      *version = "@(#)xzet.c    1.1 30/08/2007 : XZET11_Commande_VEN " ;
    XDY_Datagroup    pl_NomDG;
    T_INT2           vl_Etat[5];      
   
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET11_Commande_EXP avec Eqt Exploitation: %d NumAction: %d Numseq %d NumTypeEqt[1] %d NumEqt[1] %d NumCmd[1] %d",
                         va_Commande_SIG_NIX.NumTE,
                         va_NoCMD,
                         va_Commande_SIG_NIX.NumSeq,
                         va_Commande_SIG_NIX.NumTypeEqt[0],
                         va_Commande_SIG_NIX.NumEqt[0],
                         va_Commande_SIG_NIX.NumCmd[0] );

   /*A Envoi du message XDM_ECMD_SIG en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                          XDM_IdentMsg(XDM_ECMD_EXP),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumTE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumTypeEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumEqt),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT2_ARRAY, &(va_Commande_SIG_NIX.NumCmd),XZETC_MAX_UNITAIRE,
                          T_IPC_FT_INT4, va_Commande_SIG_NIX.NumSeq,
                          T_IPC_FT_INT4, va_NoCMD,
                          NULL))
   {
      /*A Trace envoi message XDM_ECMD_VEN non effectue */        
      XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_EXP on effectue.");
    
      /*A Trace sortie fonction XZET07_Commande_VEN avec XDC_NOK */   
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET07_Commande_EXP avec XDC_NOK");
     
      return ( XDC_NOK );
                         
   }
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_VEN effectue avec succes */        
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_EXP effectue avec succes");
   
   /*A     Trace sortie fonction XZET07_Commande_VEN avec XDC_OK */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET07_Commande_EXP avec XDC_OK");

   return ( XDC_OK );   

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence equipement domaine Signalisation (tube)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET40_Commande_SEQSIG  ( XZETT_CMD_SEQ_NIX  va_Commande_SEQ_NIX,
                              int                va_NoCMD )

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX  Commande a effectuee
*   va_NoCMD             Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero eqt incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence eqt doamine Signalisation
*   de la fenetre de pilotage eqt domaine Signalisation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQSIG en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char *version = "@(#)xzet.c    1.1 30/08/2007 : XZET40_Commande_SEQSIG " ;

    XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET40_Commande_SEQSIG avec Eqt Signalisation: %d NumSeq: %d NumAction: %d",
                          va_Commande_SEQ_NIX.NumDom,
                          va_Commande_SEQ_NIX.NumSeq,
                          va_NoCMD );
     
    /*A Envoi du message XDM_ECMD_SEQSIG en fournissant les valeur recues  */
   
    if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                           XDM_IdentMsg(XDM_ECMD_SEQSIG),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.TypeOuvrage,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumDom,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumSeq,
                           T_IPC_FT_INT4, va_NoCMD,
                           NULL))
    {
        /*A Trace envoi message XDM_ECMD_SEQSIG non effectue */        
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQSIG non effectue.");
    
        /*A Trace sortie  */   
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET40_Commande_SEQSIG avec XDC_NOK");
     
        return ( XDC_NOK );
                         
    }
   
    /*A Envoi du message */
   
    TipcSrvFlush();
   
    /*A Trace envoi message XDM_ECMD_SEQSIG effectue avec succes */        
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQSIG effectue avec succes");
   
    /*A     Trace sortie */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET40_Commande_SEQSIG avec XDC_OK");

    return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence equipement domaine Eclairage 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET41_Commande_SEQECL ( XZETT_CMD_SEQ_NIX  va_Commande_SEQ_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX  Commande a effectuee
*   va_NoCMD             Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero eqt incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence eqt Eclairage
*   de la fenetre de pilotage eqt domaine Eclairage GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQECL en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char *version = "@(#)xzet.c    1.1 30/08/2007 : XZET41_Commande_SEQECL " ;

    XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET41_Commande_SEQECL avec Eqt Eclairage: %d NumSeq: %d NumAction: %d",
                          va_Commande_SEQ_NIX.NumDom,
                          va_Commande_SEQ_NIX.NumSeq,
                          va_NoCMD );
     
    /*A Envoi du message XDM_ECMD_SEQECL en fournissant les valeur recues  */
   
    if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                           XDM_IdentMsg(XDM_ECMD_SEQECL),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.TypeOuvrage,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumDom,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumSeq,
                           T_IPC_FT_INT4, va_NoCMD,
                           NULL))
    {
        /*A Trace envoi message XDM_ECMD_SEQECL non effectue */        
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQECL non effectue.");
    
        /*A Trace sortie  */   
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET41_Commande_SEQECL avec XDC_NOK");
     
        return ( XDC_NOK );
                         
    }
   
    /*A Envoi du message */
   
    TipcSrvFlush();
   
    /*A Trace envoi message XDM_ECMD_SEQECL effectue avec succes */        
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQECL effectue avec succes");
   
    /*A     Trace sortie */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET41_Commande_SEQECL avec XDC_OK");

    return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence equipement domaine Ventilation 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET42_Commande_SEQVEN ( XZETT_CMD_SEQ_NIX  va_Commande_SEQ_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX  Commande a effectuee
*   va_NoCMD             Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero eqt incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence eqt Ventilation
*   de la fenetre de pilotage eqt domaine Ventilation GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQVEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char *version = "@(#)xzet.c    1.1 30/08/2007 : XZET42_Commande_SEQVEN " ;

    XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET42_Commande_SEQVEN avec Eqt Ventilation: %d NumSeq: %d NumAction: %d",
                          va_Commande_SEQ_NIX.NumDom,
                          va_Commande_SEQ_NIX.NumSeq,
                          va_NoCMD );
     
    /*A Envoi du message XDM_ECMD_SEQVEN en fournissant les valeur recues  */
   
    if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                           XDM_IdentMsg(XDM_ECMD_SEQVEN),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.TypeOuvrage,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumDom,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumSeq,
                           T_IPC_FT_INT4, va_NoCMD,
                           NULL))
    {
        /*A Trace envoi message XDM_ECMD_SEQVEN non effectue */        
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQVEN non effectue.");
    
        /*A Trace sortie  */   
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET42_Commande_SEQVEN avec XDC_NOK");
     
        return ( XDC_NOK );
                         
    }
   
    /*A Envoi du message */
   
    TipcSrvFlush();
   
    /*A Trace envoi message XDM_ECMD_SEQVEN effectue avec succes */        
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQVEN effectue avec succes");
   
    /*A     Trace sortie */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET42_Commande_SEQVEN avec XDC_OK");

    return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence equipement domaine Energie 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET43_Commande_SEQENR ( XZETT_CMD_SEQ_NIX  va_Commande_SEQ_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX  Commande a effectuee
*   va_NoCMD             Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero eqt incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence eqt Energie
*   de la fenetre de pilotage eqt domaine Energie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQENR en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char *version = "@(#)xzet.c    1.1 30/08/2007 : XZET43_Commande_SEQENR " ;

    XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET43_Commande_SEQENR avec Eqt Energie: %d NumSeq: %d NumAction: %d",
                          va_Commande_SEQ_NIX.NumDom,
                          va_Commande_SEQ_NIX.NumSeq,
                          va_NoCMD );
     
    /*A Envoi du message XDM_ECMD_SEQENR en fournissant les valeur recues  */
   
    if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                           XDM_IdentMsg(XDM_ECMD_SEQENR),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.TypeOuvrage,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumDom,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumSeq,
                           T_IPC_FT_INT4, va_NoCMD,
                           NULL))
    {
        /*A Trace envoi message XDM_ECMD_SEQENR non effectue */        
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQENR non effectue.");
    
        /*A Trace sortie  */   
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET43_Commande_SEQENR avec XDC_NOK");
     
        return ( XDC_NOK );
                         
    }
   
    /*A Envoi du message */
   
    TipcSrvFlush();
   
    /*A Trace envoi message XDM_ECMD_SEQENR effectue avec succes */        
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQENR effectue avec succes");
   
    /*A     Trace sortie */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET43_Commande_SEQENR avec XDC_OK");

    return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence equipement domaine Protection incendie 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET44_Commande_SEQINC ( XZETT_CMD_SEQ_NIX  va_Commande_SEQ_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX  Commande a effectuee
*   va_NoCMD             Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero eqt incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence eqt Protection incendie
*   de la fenetre de pilotage eqt domaine Protection incendie GTC de Nice. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQINC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char *version = "@(#)xzet.c    1.1 30/08/2007 : XZET44_Commande_SEQINC " ;

    XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET44_Commande_SEQINC avec Eqt Protection incendie: %d NumSeq: %d NumAction: %d",
                          va_Commande_SEQ_NIX.NumDom,
                          va_Commande_SEQ_NIX.NumSeq,
                          va_NoCMD );
     
    /*A Envoi du message XDM_ECMD_SEQINC en fournissant les valeur recues  */
   
    if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                           XDM_IdentMsg(XDM_ECMD_SEQINC),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.TypeOuvrage,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumDom,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumSeq,
                           T_IPC_FT_INT4, va_NoCMD,
                           NULL))
    {
        /*A Trace envoi message XDM_ECMD_SEQINC non effectue */        
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQINC non effectue.");
    
        /*A Trace sortie  */   
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET44_Commande_SEQINC avec XDC_NOK");
     
        return ( XDC_NOK );
                         
    }
   
    /*A Envoi du message */
   
    TipcSrvFlush();
   
    /*A Trace envoi message XDM_ECMD_SEQINC effectue avec succes */        
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XDM_ECMD_SEQINC effectue avec succes");
   
    /*A     Trace sortie */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET44_Commande_SEQINC avec XDC_OK");

    return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande sequence equipement exploitation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZET45_Commande_SEQEXP ( XZETT_CMD_SEQ_NIX  va_Commande_SEQ_NIX,
                             int                va_NoCMD )        

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SEQ_NIX  Commande a effectuee
*   va_NoCMD             Numero de la commande
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK           succes
*   XDC_NOK          echec 
*   XZEXC_ERR_SEQ    Numero de sequence invalide
*   XZEXC_ERR_EQT    Numero eqt incorrect
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation commande sequence eqt exploitation
*
* FONCTION 
*   Construction du message XDM_ECMD_SEQEXP en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_GTC_NICE .
*
------------------------------------------------------*/
{
    static char *version = "@(#)xzet.c    1.1 30/08/2007 : XZET45_Commande_SEQEXP " ;

    XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZET45_Commande_SEQEXP avec Eqt %d NumSeq: %d NumAction: %d",
                          va_Commande_SEQ_NIX.NumDom,
                          va_Commande_SEQ_NIX.NumSeq,
                          va_NoCMD );
     
    /*A Envoi du message XDM_ECMD_SEQSIG en fournissant les valeur recues  */
   
    if (!TipcSrvMsgWrite ( XDG_ECMD_GTC_NICE,
                           XDM_IdentMsg(XDM_ECMD_SEQEXP),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.TypeOuvrage,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumDom,
                           T_IPC_FT_INT4, va_Commande_SEQ_NIX.NumSeq,
                           T_IPC_FT_INT4, va_NoCMD,
                           NULL))
    {
        /*A Trace envoi message XDM_ECMD_SEQSIG non effectue */        
        XZST_03EcritureTrace(XZSTC_WARNING ,"Envoi message XDM_ECMD_SEQEXP non effectue.");
    
        /*A Trace sortie  */   
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: XZET45_Commande_SEQEXP avec XDC_NOK");
     
        return ( XDC_NOK );
                         
    }
   
    /*A Envoi du message */
   
    TipcSrvFlush();
   
    /*A Trace envoi message XDM_ECMD_SEQSIG effectue avec succes */        
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message XZET45_Commande_SEQEXP effectue avec succes");
   
    /*A     Trace sortie */
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZET45_Commande_SEQEXP avec XDC_OK");

    return ( XDC_OK );    
}


/* AAZ */

