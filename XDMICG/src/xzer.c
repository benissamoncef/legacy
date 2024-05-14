/*E*/
/*Fichier : $Id: xzer.c,v 1.8 2021/09/14 10:03:40 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2021/09/14 10:03:40 $
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZER  * FICHIER xzer.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des commandes RADT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	19 Dec 1994	: Creation
* Volcic.F	version 1.2	16 Jan 1995	: Modif
* Volcic.F	version 1.3	16 Jan 1995	:
* Fontaine.C	version 1.4	24 Mai 1995	: Reprise historique
* Volcic.F	version 1.5	01 Aou 1995	: Ajout XZER03_Acknoledge_6min_QVTO
* Mismer.D	version 1.6	01 Aou 1995	: Correction XZER03_Acknoledge_6min_QVTO
* Volcic.F	version 1.7	13 Nov 1995	: Modif envoi tableau XZER01
* PNI		1.8		14/09/2021	: Correction traces XZER01
------------------------------------------------------*/

/* fichiers inclus */

#include "xzer.h"


/* definitions de constantes */

/* definitions de types locaux */

XDY_Fonction		pg_FonctUtil_XZER_02 = NULL;
T_CB			vg_CB_XZER02         = NULL;

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzer.c,v 1.8 2021/09/14 10:03:40 pc2dpdy Exp $ : xzer" ;

static XDY_Booleen vm_exisequ = 0;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Demande de configuration des voies
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZER01_Config_Voies_RADT (	XZERT_Config_Voies  va_Config_Voies,
				XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*   va_Config_Voies	Configuration des voies
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
*   Permet de configurer les RADT 
*
* FONCTION 
*   Envoie le message XDM_EConfig_Voies_RADT sur le 
*   datagroup XDG_EConf_Voie_RADT_<NomMachine>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzer.c,v 1.8 2021/09/14 10:03:40 pc2dpdy Exp $ : XZER01_Config_Voies_RADT" ;
XDY_Datagroup	pl_NomDG;
T_INT2		vl_Etat[5];
    
     
     XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZER01_Config_Voies_RADT avec %d %d %d %d %d %d %d %s"
 	,va_Config_Voies.NoEqtRADT
 	,va_Config_Voies.Sens
 	,va_Config_Voies.EtatVoie[0]
 	,va_Config_Voies.EtatVoie[1]
 	,va_Config_Voies.EtatVoie[2]
 	,va_Config_Voies.EtatVoie[3]
 	,va_Config_Voies.EtatVoie[4]
 	,pa_NomMachine);
 	
   vl_Etat[0]=va_Config_Voies.EtatVoie[0];
   vl_Etat[1]=va_Config_Voies.EtatVoie[1];
   vl_Etat[2]=va_Config_Voies.EtatVoie[2];
   vl_Etat[3]=va_Config_Voies.EtatVoie[3];
   vl_Etat[4]=va_Config_Voies.EtatVoie[4];
   
   /*A Construction du DatGroup XDG_ECONF_VOIE_RADT_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECONF_VOIE_RADT, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_EConf_Voie_RADT en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_EConf_Voie_RADT)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2,va_Config_Voies.NoEqtRADT
 			  ,T_IPC_FT_INT2,va_Config_Voies.Sens
 			  ,T_IPC_FT_INT2_ARRAY,vl_Etat,5
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_EConfig_Voies_RADT non effectue */    	
         	XZST_03EcritureTrace(XZSTC_INFO ,"XZER01_Config_Voies_RADT : Envoi message XDM_EConfig_Voies_RADT non effectue");
    
		/*A Trace sortie fonction XZER01_Config_Voies_RADT avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZER01_Config_Voies_RADT avec XDC_NOK");
		return ( XDC_NOK );
	}
   
   /*A Envoi du message */
   TipcSrvFlush();

   /*A Trace envoi message XZER01_Config_Voies_RADT effectue avec succes */    	
   XZST_03EcritureTrace(XZSTC_WARNING,"XZER01_Config_Voies_RADT : Envoi message XZER01_Config_Voies_RADT effectue avec succes");
   XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZER01_Config_Voies_RADT avec XDC_OK");
   return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* 
* Demande individuelle 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZER02_Demande_Individuelle (	XDY_Eqt		va_NumEqt,
					XDY_Entier	va_Demande,
					XDY_NomMachine	pa_NomMachine )		

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumEqt		numero RADT
*   va_Demande		debut ou fin
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
*   Permet de configurer les RADT 
*
* FONCTION 
*   Envoie le message XDM_EConfig_Voies_RADT sur le 
*   datagroup XDG_EDEM_Indiv_<NomMachine>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzer.c,v 1.8 2021/09/14 10:03:40 pc2dpdy Exp $ :XZER02_Demande_Individuelle";

XDY_Datagroup	pl_NomDG;
    
     XZST_03EcritureTrace(XZSTC_INTERFACE,"IN: XZER02_Demande_Individuelle avec %d %d %s",va_NumEqt
 											,va_Demande
 											,pa_NomMachine);
 	
   /*A 
   ** Construction du DatGroup XDG_EDEM_Indiv_<NomMachine> 
   */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_EDEM_INDIV, pa_NomMachine ) ;
   	
   /*A 
   ** Envoi du message XDM_EDEM_Indiv en fournissant les valeur recues  
   */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_EDEM_Indiv)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2,va_NumEqt
 			  ,T_IPC_FT_INT2,va_Demande
                          ,NULL))
      	{
  		/*A 
  		** Trace envoi message XDM_EDEM_Indiv non effectue 
  		*/    	
      	
         	XZST_03EcritureTrace(XZSTC_INFO ,"XZER02_Demande_Individuelle : Envoi message XDM_EDEM_Indiv non effectue");
    
		/*A 
		** Trace sortie fonction XZER02_Demande_Individuelle avec XDC_NOK 
		*/   

                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZER02_Demande_Individuelle avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A 
   ** Envoi du message 
   */
   
   TipcSrvFlush();

   /*A 
   ** Trace envoi message XZER02_Demande_Individuelle effectue avec succes 
   */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO,"XZER02_Demande_Individuelle :Envoi message XZER02_Demande_Individuelle effectue avec succes");
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZER02_Demande_Individuelle avec XDC_OK");
 	
   return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* 
* Acknolegde de stockage en BD des 6 min Q V TO PL 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZER03_Acknoledge_6min_QVTO (	XDY_Eqt		va_NumEqt,
					XDY_Horodate	va_Horodate )		

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumEqt		numero RADT
*   va_Horodate		Horodate de stockage en BD
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*   Permet de mettre a jour les dates d'historisation des mesures 6min RADT 
*
* FONCTION 
*   Envoie le message XDM_Acknoledge sur le 
*   datagroup XDG_ACKNOLEDGE_R<NumRADT>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzer.c,v 1.8 2021/09/14 10:03:40 pc2dpdy Exp $ :XZER03_Acknoledge_6min_QVTO";

XDY_Datagroup	pl_NomDG;
    
     XZST_03EcritureTrace(XZSTC_INTERFACE,"IN: XZER03_Acknoledge_6min_QVTO avec %d %lf",va_NumEqt
 											,va_Horodate);
 	
   /*A Construction du DatGroup XDG_ACKNOLEDGE_R<NumRADT> */
   sprintf ( pl_NomDG, "%s_R%d", XDG_ACKNOLEDGE, va_NumEqt ) ;
   	
   /*A Envoi du message XDM_EDEM_Indiv en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_Acknoledge)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2,va_NumEqt
 			  ,T_IPC_FT_REAL8,va_Horodate
                          ,NULL))
      	{
  		/*A Trace envoi messageXDM_Acknoledge  non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING ,"XZER03_Acknoledge_6min_QVTO : Envoi message XDM_Acknoledge non effectue");
    
		/*A Trace sortie fonction XZER03_Acknoledge_6min_QVTO avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZER03_Acknoledge_6min_QVTO avec XDC_NOK");
		return ( XDC_NOK );
	}
   
   /*A Envoi du message */
   TipcSrvFlush();

   /*A Trace envoi message XZER03_Acknoledge_6min_QVTO effectue avec succes */    	
   XZST_03EcritureTrace(XZSTC_INFO,"XZER03_Acknoledge_6min_QVTO :Envoi message XDM_Acknoledge effectue avec succes");
   XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZER03_Acknoledge_6min_QVTO avec XDC_OK");
   return ( XDC_OK );   
}

