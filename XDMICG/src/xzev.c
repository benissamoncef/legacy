/*E*/
/*Fichier : $Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2017/02/28 14:45:01 $
--------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
--------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
--------------------------------------------------------------------------------
* MODULE XZEV  * FICHIER xzev.c
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module d'interface de la video
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	27 Oct 1994	: Creation
* Guilhou	version 1.2	09 Jan 1995	:
* Mismer.D	version 1.3	25 Jan 1995	:
* Mismer.D	version 1.4	29 Mar 1995	:
* Volcic.F	version 1.5	23 Mai 1995	:
* Fontaine.C	version 1.6	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.7	24 Avr 1997	: Commande magneto EQT2 (DEM/1427)
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.8
* PNI		19/05/11 : ajout XZEVC_JOYSTICK_P03  1.9
* JPL		25/10/16 : Ids positions selon capacites cameras ; XZEV02: joystick devient position (DEM 1189)  1.10
------------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzev.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ : xzev" ;

/* declaration de fonctions internes */
int XZEV_Envoyer_message(XDY_NomSite, int, int, int, int, int, int, int, int);


/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affectation canal moniteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEV01_Affecte_Canal_Moniteur(XDY_NomSite 	va_nomSite,
                                  int 		va_NumCanal,
                                  int 		va_NumMoniteur,
                                  int 		va_NumLT)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumCanal : no du canal a affecter
* va_NumMoniteur :no du moniteur sur lequel affecter l'image
* va_NumLT : no du local technique
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
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ :XZEV01_Affecte_Canal_Moniteur " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEV01_Affecte_Canal_Moniteur avec %s %d %d %d",
        va_nomSite,
        va_NumCanal,
        va_NumMoniteur,
        va_NumLT);
 	
   /*A Envoi du message XDM_ECMD_VIDEO */
   if (XZEV_Envoyer_message (va_nomSite,
                        XDC_AFFECT_IMAG,
                        0,
                        va_NumMoniteur,
                        0,0,0,
                        va_NumCanal,
                        va_NumLT) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV01_Affecte_Canal_Moniteur avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZED01_Config_Voies_DAI avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEV01_Affecte_Canal_Moniteur avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'envoi message 
*
------------------------------------------------------*/
int XZEV_Envoyer_message(XDY_NomSite 	va_nomSite,
                     int 		va_codecmd,
                     int 		va_noimage,
                     int 		va_nomoniteur,
                     int 		va_noposition,
                     int 		va_nojoystick,
                     int 		va_nomagneto,
                     int 		va_nocanal,
                     int 		va_noltvideo)


/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site 
* va_codecmd : no de commande
* va_noimage : no image
* va_NumMoniteur : no du moniteur 
* va_noposition : no de position
* va_nojoystick : no joystick
* va_nomagneto : no de magnetoscope
* va_NumCanal : no du canal 
* va_NumLT : no du local technique
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
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
XDY_Datagroup	pl_NomDG;
XDY_NomMachine	pl_NomMachine;

   /*A Construction du DatGroup XDG_ECMD_video_<nomSite> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_VIDEO, va_nomSite ) ;
   
   /*A Envoi du message XDM_ECMD_VIDEO en fournissant les valeur recues  */

   /*A Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
   } 
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_VIDEO)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT4,va_codecmd
 			  ,T_IPC_FT_INT4,va_noimage
 			  ,T_IPC_FT_INT4,va_nomoniteur
 			  ,T_IPC_FT_INT4,va_noposition
 			  ,T_IPC_FT_INT4,va_nojoystick
 			  ,T_IPC_FT_INT4,va_nomagneto
 			  ,T_IPC_FT_INT4,va_nocanal
 			  ,T_IPC_FT_INT4,va_noltvideo
 			  ,T_IPC_FT_STR, pl_NomMachine
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_VIDEO non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_INFO
 		   ,"Envoi message XDM_ECMD_VIDEO non effectue \n");
    
         	return(	XDC_NOK );		
	}
        else
        {
                /*A Envoi du message */
   
                TipcSrvFlush();

                /*A Trace envoi message XDM_ECMD_VIDEO effectue avec succes */    	
      	
                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_VIDEO effectue avec succes sur %s\n",pl_NomDG);
                   
         	return(	XDC_OK );		
        }
 
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affectation camera a un moniteur et avec positionnement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEV02_Affecte_Image_Monit_Joy(XDY_NomSite 	va_nomSite,
                                  int 		va_NumImage,
                                  int 		va_NumMoniteur,
                                  int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumImage : no image camera a affecter
* va_NumMoniteur :no du moniteur sur lequel affecter l'image
* va_PO : Numero de position de camera
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEVC_ERR_POSITION  position camera invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ :XZEV02_Affecte_Image_Monit_Joy " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEV02_Affecte_Image_Monit_Joy avec %s %d %d %d",
        va_nomSite,
        va_NumImage,
        va_NumMoniteur,
        va_PO);
 
   /*A Controle du no de position */
   if ( (va_PO != XZEVC_POS_AUCUNE)  &&  ((va_PO < XDC_LES_2_SENS ) || (va_PO > XDC_ENTREE_SENS_2)) )
   {
      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV02_Affecte_Image_Monit_Joy avec XZEVC_ERR_POSITION");
 	
      return ( XZEVC_ERR_POSITION );
   }

   /*A Envoi du message XDM_ECMD_VIDEO */
   if (XZEV_Envoyer_message (va_nomSite,
                        XDC_AFFECT_IMAG_JOYSTICK,
                        va_NumImage,va_NumMoniteur,
                        va_PO,
                        0,
                        0,0,0) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV02_Affecte_Image_Monit_Joy avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEV02_Affecte_Image_Monit_Joy avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de positionnement camera 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEV03_Positionnement(XDY_NomSite 	        va_nomSite,
                                  int 		va_NumImage,
                                  int 		va_NumMoniteur,
                                  int 		va_IdPosition)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumImage : no image camera a affecter
* va_NumMoniteur :no du moniteur sur lequel affecter l'image
* va_IdPosition : Numero de position de camera
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEVC_ERR_POSITION  position camera invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ :XZEV03_Positionnement " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEV03_Positionnement avec %s %d %d %d",
        va_nomSite,
        va_NumImage,
        va_NumMoniteur,
        va_IdPosition);

   /*A Controle de la position */
   if ( (va_IdPosition != XZEVC_POS_AUCUNE)  &&  ((va_IdPosition < XDC_LES_2_SENS ) || (va_IdPosition > XDC_ENTREE_SENS_2)) )
   {
      /*A Trace sortie fonction XZEV03_Positionnement avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV03_Positionnement avec XZEVC_ERR_POSITION");
 	
      return ( XZEVC_ERR_POSITION );
   }
 	
   /*A Envoi du message XDM_ECMD_VIDEO */
   if (XZEV_Envoyer_message (va_nomSite,
                        XDC_CMD_POS,
                        va_NumImage,va_NumMoniteur,va_IdPosition,
                        0,0,0,0) == XDC_NOK )
   {
      /*A Trace sortie fonction XZEV03_Positionnement avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV03_Positionnement avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEV03_Positionnement avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEV03_Positionnement avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de memorisation positionnement camera 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEV04_Memorise_Position(XDY_NomSite 	va_nomSite,
                             int 		va_NumImage,
                             int 		va_IdPosition)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumImage : no image camera a affecter
* va_IdPosition : Numero de position de camera
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEVC_ERR_POSITION  position camera invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ :XZEV04_Memorise_Position " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEV04_Memorise_Position avec %s %d %d",
        va_nomSite,
        va_NumImage,
        va_IdPosition);

   /*A Controle de la position */
   if ( (va_IdPosition != XZEVC_POS_AUCUNE)  &&  ((va_IdPosition < XDC_LES_2_SENS ) || (va_IdPosition > XDC_ENTREE_SENS_2)) )
   {
      /*A Trace sortie fonction XZEV04_Memorise_Position avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV04_Memorise_Position avec XZEVC_ERR_POSITION");
 	
      return ( XZEVC_ERR_POSITION );
   }
 	
   /*A Envoi du message XDM_ECMD_VIDEO */
   if (XZEV_Envoyer_message (va_nomSite,
                        XDC_MEM_POS,
                        va_NumImage,0,va_IdPosition,
                        0,0,0,0) == XDC_NOK )
   {
      /*A Trace sortie fonction XZEV04_Memorise_Position avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV04_Memorise_Position avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEV04_Memorise_Position avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEV04_Memorise_Position avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'instinction de moniteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEV05_Eteint_Moniteur(XDY_NomSite 	va_nomSite,
                           int 		va_NumMoniteur)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumMoniteur :no du moniteur a eteindre
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
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ :XZEV05_Eteint_Moniteur " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEV05_Eteint_Moniteur avec %s %d",
        va_nomSite,
        va_NumMoniteur);

   /*A Envoi du message XDM_ECMD_VIDEO */
   if (XZEV_Envoyer_message (va_nomSite,
                        XDC_ETEINT_MONIT,
                        0,va_NumMoniteur,0,
                        0,0,0,0) == XDC_NOK )
   {
      /*A Trace sortie fonction XZEV05_Eteint_Moniteur avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV05_Eteint_Moniteur avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEV05_Eteint_Moniteur avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEV05_Eteint_Moniteur avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Commande magnetoscope
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEV06_Commande_Magneto(XDY_NomSite 	va_nomSite,
                             int 		va_TypeCmd,
                             int 		va_NumImage,
                             int 		va_NumMagneto,
                             int 		va_NumMoniteur )

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_TypeCmd : Type de commande (Enregistre,arret,retour,etc..)
* va_NumImage : no image camera a affecter
* va_NumMagneto : Numero de magneto sur lequel enregistrer l'image
* va_NumMoniteur : no moniteur pour sortie magneto
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
*   Envoie le message XDM_ECMD_VIDEO sur le 
*   datagroup XDG_ECMD_video_<nomSite>
*
------------------------------------------------------*/
{
   static char *version = "$Id: xzev.c,v 1.10 2017/02/28 14:45:01 devgfi Exp $ :XZEV06_Commande_Magneto " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEV06_Commande_Magneto avec %s %d %d %d",
        va_nomSite,
        va_TypeCmd,
        va_NumImage,
        va_NumMagneto);

   /*A Envoi du message XDM_ECMD_VIDEO */
   if (XZEV_Envoyer_message (va_nomSite,
                        va_TypeCmd,
                        va_NumImage,va_NumMoniteur,0,
                        0,va_NumMagneto,0,0) == XDC_NOK )
   {
      /*A Trace sortie fonction XZEV06_Commande_Magneto avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEV06_Commande_Magneto avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEV06_Commande_Magneto avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEV06_Commande_Magneto avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}
