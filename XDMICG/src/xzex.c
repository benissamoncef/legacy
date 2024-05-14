/*E*/
/* Fichier : $Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $     Release : $Revision: 1.29 $      Date :  $Date: 2020/11/03 17:57:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEX  * FICHIER xzex.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des commandes communes
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Nagiel.E	version 1.2	07 Oct 1994	:
* Nagiel.E	version 1.3	11 Oct 1994	:
* Mismer.D	version 1.4	28 Oct 1994	:
* Nagiel.E	version 1.5	16 Nov 1994	:
* Nagiel.E	version 1.6	07 Dec 1994	: Ajout XZEX01 et xzex02
* Mismer.D	version 1.7	05 Jan 1995	:
* Mismer.D	version 1.8	25 Jan 1995	:
* Mismer.D	version 1.9	25 Jan 1995	:
* Mismer.D	version 1.10	06 Fev 1995	:
* Mismer.D	version 1.11	23 Mai 1995	: Ajout type Panneau pour XZEX03, Reprise historique
* Mismer.D	version 1.12	21 Jun 1995	: Correction duplicate symbol 
* Mismer.D	version 1.13	07 Aug 1995	: Ajout XZEX04_Annul_TimeOut
* Volcic.F	version 1.14	21 Aug 1995	: Ajout XZEX05_Acknoledge_Alarme
* Volcic.F	version 1.15	04 Sep 1995	: Ajout param Horodate dans XZEX05_Acknoledge_Alarme
* Volcic.F	version 1.16	17 Oct 1995	: Modif XZEX05_Acknoledge_Alarme
* Mismer.D	version 1.17	12 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.18	10 Fev 1997	: Ajout type TDP (DEM/1395)
* Mismer.D	version 1.19	24 Fev 1997	: Ajout type TDP Suite (DEM/1396)
* JMG		version 1.21	13/09/05	: ajout PMVA BAF BAD       
* JMG		version 1.22	28/09/05	: ajout PMVA BAF BAD       
* JBL		Mai 2008					: DEM 664 Ajout GTC renovee
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : retour C.R. OK pour fonction XZEX04_Annul_TimeOut  1.25
* JMG		04/07/12 : ajout PRV DEM/1016
* CGR		27/07/20 : Ajout IMU 1.29 DEM-SAE155
* ABK		18/08/23 : Ajout du cas pour le nouveau type Picto (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */

#include <xdc.h>
#include <xdy.h>
#include <xzst.h>
#include <xzec.h>

#include "xzex.h"

/* definitions de constantes */
#define	CM_TIMEOUT_LCR	45

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $ : xzex" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX01_Demande_LCR( int	 	va_NoTypEqt,
                        XDY_Eqt 	va_NoEqt,
                        char *		va_FichierLCR,
                        char *		va_FichierResultat,
                        int		va_Periode,
                        XDY_Horodate	va_HrDebut,
                        XDY_Horodate	va_HrFin,
                        XDY_NomMachine	va_NomMachine,
                        XDY_Fonction	va_FonctionTimeout )

/*
* ARGUMENTS EN ENTREE :
*
* va_NoTypEqt : No du type d'eqt
* va_NoEqt : No d'equipement
* va_FichierLCR : Fichier contenant les commandes LCR
* va_FichierResultat : Fichier resultat LCR
* va_Periode : Periode d'emission des messages
* va_HrDebut : Heure debut d'emission
* va_HrFin : Heure fin d'emission
* va_NomMachine : Nom Machine
* va_FonctionTimeout : Fonction appelee sur timeout
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
*   Envoie le message XDM_ELCR sur le 
*   datagroup XDG_ECPT_LCR_<NomMachine>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $ : XZEX01_Demande_LCR" ;
 XDY_Datagroup		pl_NomDG;
 XDY_NomMachine		vl_NomMachine;
 char			vl_typeqt[5];
 struct itimerval	vl_Prog;
 
   /*A interdire les interruptions */
  signal(SIGALRM,SIG_IGN);

   /*A Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEX01_Demande_LCR avec %d %d %s %s %d %lf %lf %s",
			va_NoTypEqt,
			va_NoEqt,
			va_FichierLCR,
			va_FichierResultat,
			va_Periode,
			va_HrDebut,
			va_HrFin,
			va_NomMachine);
 
   /*A Construction du DatGroup XDG_ECMD_<TypeEqt>_<NomMachine> */
   
   XZEX03_nom_eqt(va_NoTypEqt,vl_typeqt);
   
   sprintf ( pl_NomDG, "ECMD_%s_%s",vl_typeqt,va_NomMachine ) ;
   
   XZSC_07NomMachine(vl_NomMachine);

   /*A Envoi du message XDM_ELCR */

   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ELCR)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2 ,va_NoTypEqt
 			  ,T_IPC_FT_INT2 ,va_NoEqt
 			  ,T_IPC_FT_STR  ,va_FichierLCR
 			  ,T_IPC_FT_STR  ,va_FichierResultat
 			  ,T_IPC_FT_INT2 ,va_Periode
 			  ,T_IPC_FT_REAL8,va_HrDebut
 			  ,T_IPC_FT_REAL8,va_HrFin
 			  ,T_IPC_FT_STR  ,vl_NomMachine
 			  ,NULL))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEX01_Demande_LCR avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEX01_Demande_LCR avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEX01_Demande_LCR avec XDC_OK : %s",pl_NomDG);
      
      /* Armement du time_out */
      vl_Prog.it_value.tv_sec     = CM_TIMEOUT_LCR;
      vl_Prog.it_value.tv_usec    = 0;
      vl_Prog.it_interval.tv_sec  = 0;
      vl_Prog.it_interval.tv_usec = 0;

      /*A armement du reveil */
      setitimer ( ITIMER_REAL, &vl_Prog, (struct itimerval *)NULL );
   
      /*A retablissement du signal d'alarme */
      signal( SIGALRM, va_FonctionTimeout );
   
      
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX02_Retour_LCR(  XDY_Mot 	va_CrLCR,
                        XDY_NomMachine	va_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*
* va_CrLCR : Compte rendu de traitement
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
*   Envoie le message XDM_ECPT_LCR sur le 
*   datagroup XDG_ECPT_LCR_<NomMachine>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $ : XZEX02_Retour_LCR" ;
XDY_Datagroup	pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEX02_Retour_LCR avec %d",
			va_CrLCR);
 
   /*A Construction du DatGroup XDG_ECPT_LCR_<NomMachine> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECPT_LCR, va_NomMachine ) ;
   
   /*A Envoi du message XDM_ECPT_LCR */

   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECPT_LCR)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2 ,va_CrLCR
 			  ,NULL))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEX02_Retour_LCRR avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEX02_Retour_LCR avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEX02_Retour_LCR avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction retournant le nom de l'equipement correspondant
*  au type d'equipement fournit en entree
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX03_nom_eqt ( int	va_TypeEqt , char	*pa_NomEqt )

/*
* ARGUMENTS EN ENTREE :
*  va_TypeEqt		: Type d'equipement
*
*
* ARGUMENTS EN SORTIE :
*  paNomEqt		: Nom de l'equipement
*
*
* CODE RETOUR : 
*  XDC_OK		: Succes
*  XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
static char *version = "$Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $ : XZEX03_nom_eqt ";
int	vl_ValRet = XDC_OK;

	/*A Selon le type d'equipement recopie du nom de l'quipement dans l'argument de sortie */
	switch ( va_TypeEqt )
	{
	case 	XZECC_TYPE_EQT_RAU	: strcpy ( pa_NomEqt, XZECC_NOM_RAU );
					  break;
	case 	XDC_EQT_PAU		: strcpy ( pa_NomEqt, XZECC_NOM_RAU );
					  break;
	case 	XDC_EQT_GEN_LCR		: strcpy ( pa_NomEqt, XZECC_NOM_QTX );
					  break;
	case 	XDC_EQT_TDP		: strcpy ( pa_NomEqt, XZECC_NOM_TDP );
					  break;
	case 	XDC_EQT_ZDP		: strcpy ( pa_NomEqt, XZECC_NOM_TDP );
					  break;
	case	XZECC_TYPE_EQT_TEL	: strcpy ( pa_NomEqt, XZECC_NOM_TEL );
					  break;
	case	XZECC_TYPE_EQT_OPE	: strcpy ( pa_NomEqt, XZECC_NOM_OPE );
					  break;
	case	XZECC_TYPE_EQT_VID	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
					  break;
	case	XZECC_TYPE_EQT_MAGN	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
					  break;
	case	XZECC_TYPE_EQT_NIC	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
					  break;
	case	XDC_EQT_ECH		: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
					  break;
	case	XDC_EQT_TUB		: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
					  break;
	case	XDC_EQT_CPT		: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
					  break;
	case	XDC_EQT_PAN		: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
					  break;
	case	XZECC_TYPE_EQT_SAT	: strcpy ( pa_NomEqt, XZECC_NOM_SAT );
					  break;
	case	XZECC_TYPE_EQT_RDT	: strcpy ( pa_NomEqt, XZECC_NOM_RDT );
					  break;
	case	XZECC_TYPE_EQT_DAI	: strcpy ( pa_NomEqt, XZECC_NOM_DAI );
					  break;
	case	XZECC_TYPE_EQT_ANA	: strcpy ( pa_NomEqt, XZECC_NOM_DAI );
					  break;
	case	XZECC_TYPE_EQT_PMV	: strcpy ( pa_NomEqt, XZECC_NOM_PMV );
					  break;
	case	XZECC_TYPE_EQT_PICTO	: strcpy ( pa_NomEqt, XZECC_NOM_PICTO );
					  break;
	case	XZECC_TYPE_EQT_NAV	: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
					  break;
	case	XDC_EQT_FAC		: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
					  break;
	case	XDC_EQT_FAU		: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
					  break;
	case	XDC_EQT_MOD		: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
					  break;
	case	XZECC_TYPE_EQT_PAL	: strcpy ( pa_NomEqt, XZECC_NOM_PAL );
					  break;
	case	XZECC_TYPE_EQT_GTC	: strcpy ( pa_NomEqt, XZECC_NOM_GTC );
					  break;
	case	XDC_EQT_TOR		: strcpy ( pa_NomEqt, XZECC_NOM_GTC );
					  break;
	case    XZECC_TYPE_EQT_PMVA  : strcpy ( pa_NomEqt, XZECC_NOM_PMVA);
					break;
	case    XZECC_TYPE_EQT_BAF  : strcpy ( pa_NomEqt, XZECC_NOM_BAF);
					break;
	case    XZECC_TYPE_EQT_BAD  : strcpy ( pa_NomEqt, XZECC_NOM_BAD);
					break;
	case	XDC_EQT_MODBAD		: strcpy ( pa_NomEqt, XZECC_NOM_BAD );
					  break;
	case	XDC_EQT_MODBAF		: strcpy ( pa_NomEqt, XZECC_NOM_BAF );
					  break;
	case 	XDC_EQT_PRV		: strcpy (pa_NomEqt, XZECC_NOM_PRV);
					break;
	case 	XDC_EQT_PIC		: strcpy (pa_NomEqt, XZECC_NOM_PIC);
					break;
	case 	XDC_EQT_CAP	: strcpy (pa_NomEqt, XZECC_NOM_CAP);
				break;
	case XDC_EQT_CFE	: strcpy(pa_NomEqt, XZECC_NOM_CFE);
			break;
	case	XDC_EQT_IMU	: strcpy ( pa_NomEqt, XZECC_NOM_IMU );
				  break;
	default				:
		if (va_TypeEqt >= XDC_DEBUT_TYPEQT_GTCRENOVEE)  /* GTC renovee */
			strcpy ( pa_NomEqt, XZECC_NOM_NIC );	
		else
			strcpy ( pa_NomEqt, XZECC_NOM_QTX );
		break;
	}
	
	return ( vl_ValRet );
      
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Annulation Timeout demande LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX04_Annul_TimeOut ( void )

/*
* ARGUMENTS EN ENTREE :
*
* aucun
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
*   Annule le timeout demande LCR
*
------------------------------------------------------*/
{
static char *version = "$Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $ : XZEX04_Annul_TimeOut" ;
struct itimerval	vl_Prog;
   
   /*A interdire les interruptions */
   signal(SIGALRM,SIG_IGN);

   /*A Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE, "IN: XZEX04_Annul_TimeOut" );
   
   /* Armement du time_out */
   vl_Prog.it_value.tv_sec     = 0;
   vl_Prog.it_value.tv_usec    = 0;
   vl_Prog.it_interval.tv_sec  = 0;
   vl_Prog.it_interval.tv_usec = 0;

   /*A armement du reveil */
   setitimer ( ITIMER_REAL, &vl_Prog, (struct itimerval *)NULL );
   
   /*A Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEX04_Annul_TimeOut" );
   return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Acknoledge de l'envoi d'une alarme a la Base de Donnees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX05_Acknoledge_Alarme (  XDY_Horodate	va_Horodate_in,
				XDY_TypeEqt     va_TypeEqt_in,
                                XDY_Eqt         va_Eqt_in,
                                XDY_TypeAlarme  va_TypeAlarme_in,
                                XDY_Booleen     va_Etat_in,
                                XDY_NomMachine	va_NomMachine_in )

/*
* ARGUMENTS EN ENTREE :
*
* aucun
*
* ARGUMENTS EN SORTIE :
*   
*	va_Horodate_in
*       va_TypeEqt_in
*       va_Eqt_in
*       va_TypeAlarme_in
*       va_Etat_in
*	va_NomMachine_in
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Acknoledge de l'envoi d'une alarme a la Base de Donnees
*
------------------------------------------------------*/
{
static char *version = "$Id: xzex.c,v 1.29 2020/11/03 17:57:53 pc2dpdy Exp $ : XZEX05_Acknoledge_Alarme" ;

XDY_Datagroup	pl_NomDG = "";
XDY_Texte	pl_TypeEqt = "";
   
   /*A Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE, "IN: %s : Horodate = %lf\tType = %d\tNumEqt = %d\tTypeAla = %d\tEtat = %d\tMachine = %s",
					version,
					va_Horodate_in,
					va_TypeEqt_in,
					va_Eqt_in,
					va_TypeAlarme_in,
					va_Etat_in,
					va_NomMachine_in );
   
   
   /*A Construction du nom du type de l'equipement */
   if ( XZEX03_nom_eqt ( va_TypeEqt_in , pl_TypeEqt ) != XDC_OK )
   {
      XZST_03EcritureTrace(XZSTC_WARNING ,"%s : XZEX03_nom_eqt retourne NOK", version);
      XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: %s avec XDC_NOK", version);
      return (XDC_NOK);
   }
   
   /*A Construction du DataGroup XDG_ACKNOLEDGE_ALA_<NomMachine>_<TypeEqt> */
   sprintf ( pl_NomDG, "%s_%s_%s", XDG_ACKNOLEDGE_ALA, va_NomMachine_in, pl_TypeEqt ) ;
   	
   /*A Envoi du message XDM_Acknoledge_Ala en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_Acknoledge_Ala)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_REAL8,va_Horodate_in
 			  ,T_IPC_FT_INT2,va_TypeEqt_in
 			  ,T_IPC_FT_INT2,va_Eqt_in
 			  ,T_IPC_FT_INT2,va_TypeAlarme_in
 			  ,T_IPC_FT_INT2,va_Etat_in
                          ,NULL))
      	{
  		/*A Trace envoi messageXDM_Acknoledge  non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING ,"%s : Envoi message XDM_Acknoledge_Ala non effectue", version);
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: %s avec XDC_NOK", version);
		return ( XDC_NOK );
	}
   
   /*A Envoi du message */
   TipcSrvFlush();

   XZST_03EcritureTrace(XZSTC_INFO,"%s : Envoi message XDM_Acknoledge_Ala effectue avec succes", version);
   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: %s retourne OK", version );
   return ( XDC_OK );   
}
