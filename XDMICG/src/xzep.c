/*E*/
/*Fichier : @(#)xzep.c	1.18      Release : 1.18        Date : 03/26/07
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEP  * FICHIER xzep.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le mode d'interface pmv et picto
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Nagiel.E	version 1.2	26 Sep 1994	:
* Nagiel.E	version 1.3	04 Oct 1994	:
* Nagiel.E	version 1.4	04 Oct 1994	:
* Nagiel.E	version 1.5	04 Oct 1994	:
* Nagiel.E	version 1.6	10 Oct 1994	: Ajout de XZEP02 XZEP03
* 				  		  MAJ de XZEP01
* Nagiel.E	version 1.7	27 Oct 1994	:
* Nagiel.E	version 1.8	16 Nov 1994	:
* Nagiel.E	version 1.9	16 Nov 1994	:
* Mismer.D	version 1.10 	28 Dec 1994	:
* Mismer.D	version 1.11	10 Fev 1995	:
* Fontaine.C	version 1.12	24 Mai 1995	:
* Mismer	version 1.13    29 Aou 1996	: modif des messages XDM_ECMD_PMV, Picto (num action sur INT4) DEM/1172 v1.162
* Mismer.D	version 1.14	29 Aou 1996	: Ajout ligne direction pour PMV 4 lignes (DEM/1162)
* Mismer.D	version 1.15	25 Mar 1997	: Ajout commande affichage TDP (DEM/1417)
* JMG		version 1.16&17	13/09/05	: ajout PMVA
* VR		version 1.18	04/11/11 : Ajout DestinationNoeud XDM_ETDP_PMV (DEM/1014)
* VR		version 1.20	25/05/12 : regulation DEM/1016
* VR		version 1.19	05/06/12 : Ajout DTP PMVA (DEM/1014 PMA)
* JMG		version 1.20 	25/01/13 : espace commande vide PMV
* JMG		version 1.23	16/04/16 : ajout SAGA DEM1191
* PNI		version 1.24	02/08/17 : Transmission de l'�tat flash DEM 1233
* CGR		version 1.25	07/12/20 : Ajout IMU DEM-SAE155
* ABE		version 1.26	06/04/21 : Ajout PAU SONO DEM-SAE244
* ABE		version 1.29	08/06/21 : Fix xdy_autoroute vers xdy_nomautoroutePAU SONO DEM-SAE244
* ...		 ...
* JPL		version 1.39	09/06/21 : Traces des modes de clignotement et flash.
* GGY    	02/05/23    :  Suppression du force du Flash à XDC_PMV_PASFLASH (DEM-473)
* GGY		13/11/23	: Ajout PIC (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzep.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzep.c	1.18 03/26/07 : xzep" ;
static XDY_Booleen vm_exisequ = 0;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage sur PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 	int XZEP01_Commande_PMV (	XZEPT_CMD_PMV	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine)		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
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
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "@(#)xzep.c	1.18 03/26/07 : XZEP01_Commande_PMV" ;
XDY_Datagroup	pl_NomDG;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP01_Commande_PMV avec %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %d %d %s"
        ,va_Commande_PMV.Numero
        ,va_Commande_PMV.Texte1
        ,va_Commande_PMV.Affichage1
        ,va_Commande_PMV.Alternat1
        ,va_Commande_PMV.Clign1
        ,va_Commande_PMV.Texte2
        ,va_Commande_PMV.Affichage2
        ,va_Commande_PMV.Alternat2
        ,va_Commande_PMV.Clign2
        ,va_Commande_PMV.Texte3
        ,va_Commande_PMV.Affichage3
        ,va_Commande_PMV.Alternat3
        ,va_Commande_PMV.Clign3
        ,va_Commande_PMV.Texte4
        ,va_Commande_PMV.Affichage4
        ,va_Commande_PMV.Alternat4
        ,va_Commande_PMV.Clign4
        ,va_Commande_PMV.Flash
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_PMV_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMV, strtok ( pa_NomMachine, " ") ) ;
   	
   /*A Envoi du message XDM_ECMD_PMV en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_PMV)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Flash
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMV non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_PMV non effectue");
    
		/*A Trace sortie fonction XZEP01_Commande_PMV avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP01_Commande_PMV avec XDC_NOK");
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();

   /*A Trace envoi message XDM_ECMD_PMV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
      ,"Envoi message XDM_ECMD_PMV effectue avec succes sur DG: <%s>", pl_NomDG );
    
   
   /*A 	Envoi commande a TETDP */
   /*A Construction du DatGroup XDG_ETDP_PMV_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ETDP_PMV, strtok ( pa_NomMachine, " ") ) ;
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ETDP_PMV)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Flash
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,T_IPC_FT_INT4, va_Commande_PMV.Destination
                          ,T_IPC_FT_INT4, va_Commande_PMV.DestinationAlternat
			  ,T_IPC_FT_INT4, va_Commande_PMV.DestinationNoeud
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMV non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ECMD_PMV non effectue");
	}

   /*A 	Trace sortie fonction XZEP01_Commande_PMV avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP01_Commande_PMV avec XDC_OK");
 	
   return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage sur Picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEP02_Commande_Picto (	XZEPT_CMD_PICTO	va_Commande_PICTO,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_Picto	Commande a effectuee
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
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_Picto en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_Picto_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "@(#)xzep.c	1.18 03/26/07 : XZEP02_Commande_Picto" ;
 XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP01_Commande_PICTO avec %d %s %s %d %d %s"
        ,va_Commande_PICTO.Numero
        ,va_Commande_PICTO.Texte
        ,va_Commande_PICTO.Picto
        ,va_Commande_PICTO.Clign
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECONFIG_EQPT_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PICTO, strtok ( pa_NomMachine, " ") ) ;
   	
   /*A Envoi du message XDM_ECMD_PICTO en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_PICTO)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PICTO.Numero
                          ,T_IPC_FT_STR,  va_Commande_PICTO.Texte
                          ,T_IPC_FT_STR,  va_Commande_PICTO.Picto
                          ,T_IPC_FT_CHAR, va_Commande_PICTO.Clign
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PICTO non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_PICTO non effectue \n");
    
		/*A Trace sortie fonction XZEP01_Commande_PICTO avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEP01_Commande_PICTO avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_PICTO effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_PICTO effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEP01_Commande_PICTO avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP01_Commande_PICTO avec XDC_OK");
 	
   return ( XDC_OK );    

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande rafraichissement affichage TDP sur PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 int XZEP03_Commande_Raf_TDP_PMV (	XZEPT_CMD_PMV	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
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
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur changement duree afficha temps de parcours (TETDP). 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "@(#)xzep.c	1.18 03/26/07 : XZEP03_Commande_Raf_TDP_PMV" ;
XDY_Datagroup	pl_NomDG;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP03_Commande_Raf_TDP_PMV avec %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %d %d %s"
        ,va_Commande_PMV.Numero
        ,va_Commande_PMV.Texte1
        ,va_Commande_PMV.Affichage1
        ,va_Commande_PMV.Alternat1
        ,va_Commande_PMV.Clign1
        ,va_Commande_PMV.Texte2
        ,va_Commande_PMV.Affichage2
        ,va_Commande_PMV.Alternat2
        ,va_Commande_PMV.Clign2
        ,va_Commande_PMV.Texte3
        ,va_Commande_PMV.Affichage3
        ,va_Commande_PMV.Alternat3
        ,va_Commande_PMV.Clign3
        ,va_Commande_PMV.Texte4
        ,va_Commande_PMV.Affichage4
        ,va_Commande_PMV.Alternat4
        ,va_Commande_PMV.Clign4
        ,va_Commande_PMV.Flash
 	,va_NoCMD
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_PMV_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMV, strtok ( pa_NomMachine, " ") ) ;
   	
   /*A Envoi du message XDM_ECMD_PMV en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_PMV)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_CHAR, XDC_PMV_PASCLIGNO
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Flash
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMV non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_PMV non effectue");
    
		/*A Trace sortie fonction XZEP03_Commande_Raf_TDP_PMV avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP03_Commande_Raf_TDP_PMV avec XDC_NOK");
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();

   /*A Trace envoi message XDM_ECMD_PMV effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
      ,"Envoi message XDM_ECMD_PMV effectue avec succes sur DG: <%s>", pl_NomDG );
    
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP03_Commande_Raf_TDP_PMV avec XDC_OK");
 	
   return ( XDC_OK );   
}


/*PMVA DEBUT*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage sur PMVA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 	int XZEP04_Commande_PMVA (	XZEPT_CMD_PMVA	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
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
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMVA. 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMVA en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMVA_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "@(#)xzep.c	1.18 1.18 03/26/07 : XZEP04_Commande_PMVA" ;
XDY_Datagroup	pl_NomDG;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP04_Commande_PMVA avec %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %d %d %d %d %d %d %s"
        ,va_Commande_PMV.Numero
        ,va_Commande_PMV.Texte1
        ,va_Commande_PMV.Affichage1
        ,va_Commande_PMV.Alternat1
        ,va_Commande_PMV.Clign1
        ,va_Commande_PMV.Texte2
        ,va_Commande_PMV.Affichage2
        ,va_Commande_PMV.Alternat2
        ,va_Commande_PMV.Clign2
        ,va_Commande_PMV.Texte3
        ,va_Commande_PMV.Affichage3
        ,va_Commande_PMV.Alternat3
        ,va_Commande_PMV.Clign3
        ,va_Commande_PMV.Texte4
        ,va_Commande_PMV.Affichage4
        ,va_Commande_PMV.Alternat4
        ,va_Commande_PMV.Clign4
        ,va_Commande_PMV.Texte5
        ,va_Commande_PMV.Affichage5
        ,va_Commande_PMV.Alternat5
        ,va_Commande_PMV.Clign5
        ,va_Commande_PMV.Flash
		,va_NoCMD
		,va_Commande_PMV.Dest1_proche
		,va_Commande_PMV.Dest1_loin
		,va_Commande_PMV.Dest2_proche
		,va_Commande_PMV.Dest2_loin
		,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_PMVA_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMVA, strtok ( pa_NomMachine, " ") ) ;
   	
   /*A Envoi du message XDM_ECMD_PMVA en fournissant les valeur recues  */
   
   if (strlen(va_Commande_PMV.Texte1)==0) strcpy(va_Commande_PMV.Texte1,"               ");
   if (strlen(va_Commande_PMV.Texte2)==0) strcpy(va_Commande_PMV.Texte2,"               ");
   if (strlen(va_Commande_PMV.Texte3)==0) strcpy(va_Commande_PMV.Texte3,"               ");
   if (strlen(va_Commande_PMV.Texte4)==0) strcpy(va_Commande_PMV.Texte4,"               ");
   if (strlen(va_Commande_PMV.Texte5)==0) strcpy(va_Commande_PMV.Texte5,"               ");
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_PMVA)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage5
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Flash
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest1_proche
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest1_loin
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest2_proche
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest2_loin
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMVA non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_PMVA non effectue");
    
		/*A Trace sortie fonction XZEP04_Commande_PMVA avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP04_Commande_PMVA avec XDC_NOK");
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();

   /*A Trace envoi message XDM_ECMD_PMVA effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
      ,"Envoi message XDM_ECMD_PMVA effectue avec succes sur DG: <%s>", pl_NomDG );
    
   
   /*A 	Envoi commande a TETDP */
   /*A Construction du DatGroup XDG_ETDP_PMVA_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ETDP_PMVA, strtok ( pa_NomMachine, " ") ) ;
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ETDP_PMVA)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage5
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat5
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Clign5
                          ,T_IPC_FT_CHAR, va_Commande_PMV.Flash
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,T_IPC_FT_INT2,va_Commande_PMV.Dest1_proche
                          ,T_IPC_FT_INT2,va_Commande_PMV.Dest1_loin
                          ,T_IPC_FT_INT2,va_Commande_PMV.Dest2_proche
                          ,T_IPC_FT_INT2,va_Commande_PMV.Dest2_loin
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMV non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETDP_PMVA non effectue");
	}

   /*A 	Trace sortie fonction XZEP04_Commande_PMVA avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP04_Commande_PMVA avec XDC_OK");
 	
   return ( XDC_OK );   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande rafraichissement affichage TDP sur PMVA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 int XZEP05_Commande_Raf_TDP_PMVA (	XZEPT_CMD_PMVA	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
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
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur changement duree afficha temps de parcours (TETDP). 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "@(#)xzep.c	1.18 1.18 03/26/07 : XZEP05_Commande_Raf_TDP_PMVA" ;
XDY_Datagroup	pl_NomDG;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP05_Commande_Raf_TDP_PMVA avec %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %d %d %d %d %d %d %s"
        ,va_Commande_PMV.Numero
        ,va_Commande_PMV.Texte1
        ,va_Commande_PMV.Affichage1
        ,va_Commande_PMV.Alternat1
        ,va_Commande_PMV.Clign1
        ,va_Commande_PMV.Texte2
        ,va_Commande_PMV.Affichage2
        ,va_Commande_PMV.Alternat2
        ,va_Commande_PMV.Clign2
        ,va_Commande_PMV.Texte3
        ,va_Commande_PMV.Affichage3
        ,va_Commande_PMV.Alternat3
        ,va_Commande_PMV.Clign3
        ,va_Commande_PMV.Texte4
        ,va_Commande_PMV.Affichage4
        ,va_Commande_PMV.Alternat4
        ,va_Commande_PMV.Clign4
        ,va_Commande_PMV.Texte5
        ,va_Commande_PMV.Affichage5
        ,va_Commande_PMV.Alternat5
        ,va_Commande_PMV.Clign5
        ,va_Commande_PMV.Flash
 	,va_NoCMD
		,va_Commande_PMV.Dest1_proche
		,va_Commande_PMV.Dest1_loin
		,va_Commande_PMV.Dest2_proche
		,va_Commande_PMV.Dest2_loin
 	,pa_NomMachine);
 	

   /*A Construction du DatGroup XDG_ECMD_PMV_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PMVA, strtok ( pa_NomMachine, " ") ) ;
   	
   /*A Envoi du message XDM_ECMD_PMV en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_PMVA)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  !strcmp(va_Commande_PMV.Texte1,"") ? " " : va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign1
                          ,T_IPC_FT_STR,  !strcmp(va_Commande_PMV.Texte2,"") ? " " : va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign2
                          ,T_IPC_FT_STR,  !strcmp(va_Commande_PMV.Texte3,"") ? " " : va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign3
                          ,T_IPC_FT_STR,  !strcmp(va_Commande_PMV.Texte4,"") ? " " : va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign4
                          ,T_IPC_FT_STR,  !strcmp(va_Commande_PMV.Texte5,"") ? " " : va_Commande_PMV.Texte5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage5
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clign5
                          ,T_IPC_FT_INT2, XDC_PMV_PASFLASH
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest1_proche
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest1_loin
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest2_proche
                          ,T_IPC_FT_INT2, va_Commande_PMV.Dest2_loin
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMVA non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_PMVA non effectue");
    
		/*A Trace sortie fonction XZEP05_Commande_Raf_TDP_PMVA avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP05_Commande_Raf_TDP_PMVA avec XDC_NOK");
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();

   /*A Trace envoi message XDM_ECMD_PMVA effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
      ,"Envoi message XDM_ECMD_PMVA effectue avec succes sur DG: <%s>", pl_NomDG );
    
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP05_Commande_Raf_TDP_PMVA avec XDC_OK");
 	
   return ( XDC_OK );   
}

/*PMVA FIN*/

	/*X*/
	/*------------------------------------------------------
	* SERVICE RENDU :
	*  Demande de commande PRV
	*
	------------------------------------------------------
	* SEQUENCE D'APPEL :
	*/

	int XZEP06_Commande_PRV (	XZEPT_CMD_PRV	va_Commande_PRV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine)

	/*
	* ARGUMENTS EN ENTREE :
	*   va_Commande_PRV	Commande a effectuee
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
	*   Doit etre invoquee sur validation de la fenetre de pilotage PRV.
	*
	* FONCTION
	*   Construction du message XDM_ECMD_PRV en specifiant les valeurs recues.
	*   Envoi du message sur le DataGroup XDG_ECMD_PRV_<NomMachine> du site concerne
	*
	------------------------------------------------------*/
	{
	 XDY_Datagroup	     		pl_NomDG;

	    /*A Trace des parametres en entree */

	   XZST_03EcritureTrace(XZSTC_INTERFACE
	 	,"IN: XZEP06_Commande_PRV avec %d \n %s \n %s \n %d \n %s"
	        ,va_Commande_PRV.Numero
	        ,va_Commande_PRV.Bandeau
	        ,va_Commande_PRV.Vitesse
	        ,va_NoCMD
			,pa_NomMachine);


	   /*A Construction du DataGroup XDG_ECMD_PRV_<NomMachine> */

	   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PRV, strtok(pa_NomMachine," ") ) ;

	   /*A Envoi du message XDM_ECMD_PRV en fournissant les valeur recues  */

	   if (!TipcSrvMsgWrite (  pl_NomDG
	      						,XDM_IdentMsg(XDM_ECMD_PRV)
	      						,XDM_FLG_SRVMSGWRITE
	      						,T_IPC_FT_INT2, va_Commande_PRV.Numero
	      						,T_IPC_FT_STR,  va_Commande_PRV.Bandeau
	      						,T_IPC_FT_STR, va_Commande_PRV.Vitesse
	      						,T_IPC_FT_INT4, va_NoCMD
	      						,T_IPC_FT_STR, pa_NomMachine
	      						,NULL))
	      	{
	  		/*A Trace envoi message XDM_ECMD_PRV non effectue */

	         	XZST_03EcritureTrace(XZSTC_WARNING
	 		   ,"Envoi message XDM_ECMD_PRV non effectue \n");

			/*A Trace sortie fonction XZEN12_Commande_PRV avec XDC_NOK */

	                XZST_03EcritureTrace(XZSTC_INTERFACE
	                   ,"OUT: XZEP06_Commande_PRV avec XDC_NOK");

			return ( XDC_NOK );

	      	}

	   /*A Envoi du message */

	   TipcSrvFlush();

	   /*A Trace envoi message XDM_ECMD_NAV effectue avec succes */

	   XZST_03EcritureTrace(XZSTC_INFO
	 	   ,"Envoi message XDM_ECMD_PRV effectue avec succes \n");

	   /*A 	Trace sortie fonction XZEP06_Commande_PRV avec XDC_OK */

	   XZST_03EcritureTrace(XZSTC_INTERFACE
	      ,"OUT: XZEP06_Commande_PRV avec XDC_OK");

	   return ( XDC_OK );

	}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU
*  Demande d'affichage surEQT SAGA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 	int XZEP104_Commande_Eqt_SAGA(	XZEPT_CMD_EQT_SAGA	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
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
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMVA. 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMVA en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMVA_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
static char *version = "@(#)xzep.c	1.18 1.18 03/26/07 : XZEP04_Commande_PMVA" ;
XDY_Datagroup	pl_NomDG;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP104_Commande_EQT_SAGA ");
 	

   /*A Construction du DatGroup XDG_ECMD_PMVA_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_EQT_SAGA, strtok ( pa_NomMachine, " ") ) ;
   	
   /*A Envoi du message XDM_ECMD_EQT_SAGA en fournissant les valeur recues  */
   
   if (strlen(va_Commande_PMV.Texte1)==0) strcpy(va_Commande_PMV.Texte1,"               ");
   if (strlen(va_Commande_PMV.Texte2)==0) strcpy(va_Commande_PMV.Texte2,"               ");
   if (strlen(va_Commande_PMV.Texte3)==0) strcpy(va_Commande_PMV.Texte3,"               ");
   if (strlen(va_Commande_PMV.Texte4)==0) strcpy(va_Commande_PMV.Texte4,"               ");
   if (strlen(va_Commande_PMV.Texte5)==0) strcpy(va_Commande_PMV.Texte5,"               ");
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_EQT_SAGA)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Commande_PMV.Numero
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte1
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat1
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte2
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat2
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte3
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat3
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte4
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat4
                          ,T_IPC_FT_STR,  va_Commande_PMV.Texte5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Affichage5
                          ,T_IPC_FT_STR,  va_Commande_PMV.Alternat5
                          ,T_IPC_FT_INT2, va_Commande_PMV.Clignotement
                          ,T_IPC_FT_INT2, va_Commande_PMV.Luminosite
			  ,T_IPC_FT_STR, va_Commande_PMV.Picto
			  ,T_IPC_FT_STR, va_Commande_PMV.Bandeau
			  ,T_IPC_FT_INT2, va_Commande_PMV.Barriere
			  ,T_IPC_FT_INT4, va_Commande_PMV.Scenario
                          ,T_IPC_FT_INT4, va_NoCMD
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_PMVA non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_EQT_SAGA non effectue");
    
		/*A Trace sortie fonction XZEP104_Commande_EQT_SAGA avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP104_Commande_EQT_SAGA avec XDC_NOK");
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();

   /*A Trace envoi message XDM_ECMD_PMVA effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
      ,"Envoi message XDM_ECMD_EQT_SAGA effectue avec succes sur DG: <%s>", pl_NomDG );
    
   
   /*A 	Trace sortie fonction XZEP04_Commande_PMVA avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP104_Commande_EQT_SAGA avec XDC_OK");
 	
   return ( XDC_OK );   
}





	/*X*/
	/*------------------------------------------------------
	* SERVICE RENDU :
	*  Demande de commande CFE
	*
	------------------------------------------------------
	* SEQUENCE D'APPEL :
	*/

	int XZEP07_Commande_CFE (	XZEPT_CMD_CFE	va_Commande_CFE,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine)

	/*
	* ARGUMENTS EN ENTREE :
	*   va_Commande_PRV	Commande a effectuee
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
	*   Doit etre invoquee sur validation de la fenetre de pilotage PRV.
	*
	* FONCTION
	*   Construction du message XDM_ECMD_PRV en specifiant les valeurs recues.
	*   Envoi du message sur le DataGroup XDG_ECMD_PRV_<NomMachine> du site concerne
	*
	------------------------------------------------------*/
	{
	 XDY_Datagroup	     		pl_NomDG;

	    /*A Trace des parametres en entree */

	   XZST_03EcritureTrace(XZSTC_INTERFACE
	 	,"IN: XZEP07_Commande_CFE avec %d %d %d %s"
	        ,va_Commande_CFE.Numero
	        ,va_Commande_CFE.NoPlanDeFeux
	        ,va_NoCMD
			,pa_NomMachine);


	   /*A Construction du DataGroup XDG_ECMD_PRV_<NomMachine> */

	   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_CFE, strtok(pa_NomMachine," ") ) ;

	   /*A Envoi du message XDM_ECMD_PRV en fournissant les valeur recues  */

	   if (!TipcSrvMsgWrite (  pl_NomDG
	      						,XDM_IdentMsg(XDM_ECMD_CFE)
	      						,XDM_FLG_SRVMSGWRITE
	      						,T_IPC_FT_INT2, va_Commande_CFE.Numero
	      						,T_IPC_FT_INT2,  va_Commande_CFE.NoPlanDeFeux
	      						,T_IPC_FT_INT4, va_NoCMD
	      						,T_IPC_FT_STR, pa_NomMachine
	      						,NULL))
	      	{
	  		/*A Trace envoi message XDM_ECMD_PRV non effectue */

	         	XZST_03EcritureTrace(XZSTC_WARNING
	 		   ,"Envoi message XDM_ECMD_CFE non effectue \n");

	                XZST_03EcritureTrace(XZSTC_INTERFACE
	                   ,"OUT: XZEP07_Commande_CFE avec XDC_NOK");

			return ( XDC_NOK );

	      	}

	   /*A Envoi du message */

	   TipcSrvFlush();

	   /*A Trace envoi message XDM_ECMD_NAV effectue avec succes */

	   XZST_03EcritureTrace(XZSTC_INFO
	 	   ,"Envoi message XDM_ECMD_CFE effectue avec succes \n");

	   /*A 	Trace sortie fonction XZEP06_Commande_PRV avec XDC_OK */

	   XZST_03EcritureTrace(XZSTC_INTERFACE
	      ,"OUT: XZEP07_Commande_CFE avec XDC_OK");

	   return ( XDC_OK );

	}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de commande IMU
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEP08_Commande_IMU (       XZEPT_CMD_IMU   va_Commande_IMU,
				int             va_NoCMD,
				XDY_NomMachine  pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_IMU     Commande a effectuee
*   va_NoCMD            Numero de la commande
*   va_NomMachine       nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage IMU.
*
* FONCTION
*   Construction du message XDM_ECMD_IMU en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_IMU_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
	XDY_Datagroup                  pl_NomDG;

	/*A Trace des parametres en entree */

	XZST_03EcritureTrace(XZSTC_INTERFACE
		,"IN: XZEP08_Commande_IMU avec %d %s %d %d %d %d %s"
		,va_Commande_IMU.Numero
		,va_Commande_IMU.ChaineIMU
		,va_Commande_IMU.Intervalle
		,va_Commande_IMU.Cycles
		,va_Commande_IMU.Duree
		,va_NoCMD
		,pa_NomMachine);


	/*A Construction du DataGroup XDG_ECMD_IMU_<NomMachine> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_IMU, strtok(pa_NomMachine," ") ) ;

	/*A Envoi du message XDM_ECMD_IMU en fournissant les valeur recues  */

	if (!TipcSrvMsgWrite (  pl_NomDG
				,XDM_IdentMsg(XDM_ECMD_IMU)
				,XDM_FLG_SRVMSGWRITE
				,T_IPC_FT_INT2, va_Commande_IMU.Numero
				,T_IPC_FT_STR,  va_Commande_IMU.ChaineIMU
				,T_IPC_FT_INT4, va_Commande_IMU.Intervalle
				,T_IPC_FT_INT4, va_Commande_IMU.Cycles
				,T_IPC_FT_INT4, va_Commande_IMU.Duree
				,T_IPC_FT_INT4, va_NoCMD
				,T_IPC_FT_STR, pa_NomMachine
				,NULL))
		{
			/*A Trace envoi message XDM_ECMD_IMU non effectue */

			XZST_03EcritureTrace(XZSTC_WARNING
				,"Envoi message XDM_ECMD_IMU non effectue \n");

			XZST_03EcritureTrace(XZSTC_INTERFACE
				,"OUT: XZEP08_Commande_IMU avec XDC_NOK");

			return ( XDC_NOK );

		}

		/*A Envoi du message */

		TipcSrvFlush();

		/*A Trace envoi message XDM_ECMD_IMU effectue avec succes */

		XZST_03EcritureTrace(XZSTC_INFO
			,"Envoi message XDM_ECMD_IMU effectue avec succes \n");

		/*A  Trace sortie fonction XZEP08_Commande_IMU avec XDC_OK */

		XZST_03EcritureTrace(XZSTC_INTERFACE
			,"OUT: XZEP08_Commande_IMU avec XDC_OK");

		return ( XDC_OK );

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de commande SONO
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEP09_Commande_RAU_SONO (  XZEPT_CMD_SONO  va_Commande_SONO,
				int             va_NoCMD,
				XDY_NomMachine  pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SONO     Commande a effectuee
*   va_NoCMD            Numero de la commande
*   va_NomMachine       nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*   XZEXC_ERR_EQT       Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage SONO.
*
* FONCTION
*   Construction du message XDM_ECMD_SONO en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SONO_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
	XDY_Datagroup                  pl_NomDG;

	/*A Trace des parametres en entree */

	XZST_03EcritureTrace(XZSTC_INTERFACE
		,"IN : XZEP09_Commande_RAU_SONO avec %s %s %d %d %d %d %s"
		,va_Commande_SONO.Message
		,va_Commande_SONO.Autoroute
		,va_Commande_SONO.Sens
		,va_Commande_SONO.PR_debut
		,va_Commande_SONO.PR_fin
		,va_Commande_SONO.NoAction
		,pa_NomMachine);

	/*A Construction du DataGroup XDG_ECMD_SONO_<NomMachine> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_SONO, strtok(pa_NomMachine," ") ) ;

	/*A Envoi du message XDM_ECMD_SONO en fournissant les valeur recues  */

	if (!TipcSrvMsgWrite (  pl_NomDG
				,XDM_IdentMsg(XDM_ECMD_SONO)
				,XDM_FLG_SRVMSGWRITE
				,T_IPC_FT_INT4, va_Commande_SONO.Sens
				,T_IPC_FT_INT4, va_Commande_SONO.PR_debut
				,T_IPC_FT_INT4, va_Commande_SONO.PR_fin
				,T_IPC_FT_INT4, va_Commande_SONO.NoAction
				,T_IPC_FT_STR,  va_Commande_SONO.Message
				,T_IPC_FT_STR,  va_Commande_SONO.Autoroute
				,T_IPC_FT_STR,  pa_NomMachine
				,NULL))
		{
			/*A Trace envoi message XDM_ECMD_SONO non effectue */

			XZST_03EcritureTrace(XZSTC_WARNING
				,"Envoi message XDM_ECMD_SONO non effectue \n");

			XZST_03EcritureTrace(XZSTC_INTERFACE
				,"OUT: XZEP09_Commande_RAU_SONO avec XDC_NOK");

			return ( XDC_NOK );

		}

		/*A Envoi du message */

		TipcSrvFlush();

		/*A Trace envoi message XDM_ECMD_SONO effectue avec succes */

		XZST_03EcritureTrace(XZSTC_INFO
			,"Envoi message XDM_ECMD_SONO effectue avec succes \n");

		/*A  Trace sortie fonction XZEP09_Commande_RAU_SONO avec XDC_OK */

		XZST_03EcritureTrace(XZSTC_INTERFACE
			,"OUT: XZEP09_Commande_RAU_SONO avec XDC_OK");

		return ( XDC_OK );

}


/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de commande PIC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEP10_Commande_PIC (	XZEPT_CMD_PIC	va_Commande_PIC,
							int				va_NoCMD,
							XDY_NomMachine	pa_NomMachine)
/*
* ARGUMENTS EN ENTREE :
*	va_Commande_Pic	Commande a effectuee
*	va_NoCMD		Numero de la commande
*	va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*	aucun
*
* CODE RETOUR :
*	XDC_OK		succes
*	XDC_NOK		echec
*	XZEXC_ERR_EQT	Numero d'equipement invalide
*	
* CONDITION D'UTILISATION
*	Doit etre invoquee sur validation de la fenetre de pilotage PIC.
*
* FONCTION
*	Construction du message XDM_ECMD_PIC en specifiant les valeurs recues.
*	Envoi du message sur le DataGroup XDG_ECMD_PIC_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
	XDY_Datagroup	     		pl_NomDG;

    /* Trace des parametres en entree */
  	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN: XZEP10_Commande_PIC avec Numero=(%d)\t Picto=(%s)\t Bandeau=(%s)\t Flash=(%d)\t NoCmd=(%d)\t NomMachine=(%s)",
							va_Commande_PIC.Numero,
							va_Commande_PIC.Picto,
							va_Commande_PIC.Bandeau,
							va_Commande_PIC.Flash,
							va_NoCMD,
							pa_NomMachine);

   /* Construction du DataGroup XDG_ECMD_PIC_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PIC, strtok(pa_NomMachine," ") ) ;

   /* Envoi du message XDM_ECMD_PIC en fournissant les valeur recues  */

   if (!TipcSrvMsgWrite (	pl_NomDG,
   							XDM_IdentMsg(XDM_ECMD_PIC),
							XDM_FLG_SRVMSGWRITE,
							T_IPC_FT_INT2, va_Commande_PIC.Numero,
							T_IPC_FT_STR,  va_Commande_PIC.Picto,
							T_IPC_FT_STR, va_Commande_PIC.Bandeau,
							T_IPC_FT_STR, va_Commande_PIC.Flash,
							T_IPC_FT_INT4, va_NoCMD,
							T_IPC_FT_STR, pa_NomMachine,
							NULL))
	{
		/* Trace envoi message XDM_ECMD_PIC non effectue */
	 	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ECMD_PIC non effectue \n");

		/* Trace sortie fonction XZEP10_Commande_PIC avec XDC_NOK */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP10_Commande_PIC avec XDC_NOK");
		
		return ( XDC_NOK );
	}

	/* Envoi du message */
	TipcSrvFlush();

	/* Trace envoi message XDM_ECMD_NAV effectue avec succes */
	XZST_03EcritureTrace(XZSTC_INFO, "Envoi message XDM_ECMD_PIC effectue avec succes \n");

	/* Trace sortie fonction XZEP10_Commande_PIC avec XDC_OK */
	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEP10_Commande_PIC avec XDC_OK");

	return ( XDC_OK );
}
