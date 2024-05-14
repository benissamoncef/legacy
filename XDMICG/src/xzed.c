/*E*/
/* Fichier : @(#)xzed.c	1.6      Release : 1.6        Date : 11/13/95
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZED  * FICHIER xzed.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface equipement DAI >
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	17 Oct 1994	: Creation
* Nagiel.E	version 1.2	16 Nov 1994	:
* Mismer.D	version 1.3	28 Dec 1994	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Fontaine.C	version 1.5	24 Mai 1995	: Reprise historique
* Volcic.F	version 1.6	13 Nov 1995	: Modif envoi tableau XZED01
* JMG		version 1.7	0$:05/20 : EMS
* LCL		20/12/21 : MOVIS P2 ems SAE-312
* GGY		20/11/23 : Ajout XZED03_Demande_Enregistrement_DAI (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzed.h"


/* definitions de constantes */

/* definitions de types locaux */

XDY_Fonction	pg_FonctUtil_XZED_02	= NULL;
T_CB			vg_CB_XZED02			= NULL;
T_CB			vg_CB_XZED03			= NULL;

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzed.c	1.6 11/13/95 : xzed" ;
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

 	int XZED01_Config_Voies_DAI (	XZEDT_Config_Voies  va_Config_Voies,
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
*   Permet de configurer les DAI 
*
* FONCTION 
*   Envoie le message XDM_EConfig_Voies_DAI sur le 
*   datagroup XDG_EConf_Voie_DAI_<NomMachine>
*
------------------------------------------------------*/
{
static char *version = "@(#)xzed.c	1.6 11/13/95 : XZED01_Config_Voies_DAI" ;
XDY_Datagroup	pl_NomDG;
T_INT2		vl_Etat[5];
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZED01_Config_Voies_DAI avec %d %d %d %d %d %d %d %d %s"
 	,va_Config_Voies.NoEqtDAI
 	,va_Config_Voies.NoEqtAnaly
 	,va_Config_Voies.NoAnaly
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
   
   /*A Construction du DatGroup XDG_ECCONF_VOIE_DAI_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECONF_VOIE_DAI, strtok(pa_NomMachine," ") ) ;
   	
   /*A Envoi du message XDM_EConf_Voie_DAI en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_EConf_Voie_DAI)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2,va_Config_Voies.NoEqtDAI
 			  ,T_IPC_FT_INT2,va_Config_Voies.NoEqtAnaly
 			  ,T_IPC_FT_INT2,va_Config_Voies.NoAnaly
 			  ,T_IPC_FT_INT2_ARRAY,vl_Etat,5
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_EConfig_Voies_DAI non effectue */    	
         	XZST_03EcritureTrace(XZSTC_INFO ,"Envoi message XDM_EConfig_Voies_DAI non effectue");
    
		/*A Trace sortie fonction XZED01_Config_Voies_DAI avec XDC_NOK */   
                XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT: XZED01_Config_Voies_DAI avec XDC_NOK");
		return ( XDC_NOK );
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();

   /*A Trace envoi message XZED01_Config_Voies_DAI effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XZED01_Config_Voies_DAI effectue avec succes");
    
   /*A 	Trace sortie fonction XZED01_Config_Voies_DAI avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE ,"OUT: XZED01_Config_Voies_DAI avec XDC_OK");
 	
   return ( XDC_OK );   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	XZED_EMes_DAI ( 
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
*  XDM_EMES_DAI 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
static char *version = "@(#)xzed.c	1.6 11/13/95 :XZED_EMes_DAI " ;

   XZEDT_EMes_DAI	vl_Mes;
#ifndef _TIBCO_EMS
   T_REAL8              vl_Horodate;
   T_INT2		vl_Numero, vl_NumAnalyseur, vl_Sens, vl_Chaussee;
   T_INT2		*pl_Vitesse, *pl_Longueur;
   T_INT4		vl_SizeVit = 0, vl_SizeLon = 0;
#else
   XDY_Horodate         vl_Horodate;
   XDY_Mot		vl_Numero, vl_NumAnalyseur, vl_Sens, vl_Chaussee;
   XDY_Mot		pl_Vitesse[10], pl_Longueur[10];
   XDY_Entier		vl_SizeVit = 0, vl_SizeLon = 0;
#endif
   int			vl_Resultat = XDC_OK,
   			vl_Index = 0;
   
		  
   /* Recuperation des parametres du message XDM_EMES_DAI */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8, &vl_Horodate,
	       T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_INT2,        &vl_NumAnalyseur,
               T_IPC_FT_INT2,        &vl_Sens,
               T_IPC_FT_INT2,        &vl_Chaussee,
               T_IPC_FT_INT2_ARRAY,  &pl_Vitesse, &vl_SizeVit,
               T_IPC_FT_INT2_ARRAY,  &pl_Longueur, &vl_SizeLon,
               T_IPC_FT_INT2,        &vl_Mes.TO,
               T_IPC_FT_INT2,        &vl_Mes.VitesseMoyenne,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_DAI dans XZED_EMes_DAI ");
   }
   else
   { 
   vl_Mes.Horodate=vl_Horodate;
   XZST_03EcritureTrace ( XZSTC_FONCTION, 
      "IN  : XZED_EMes_DAI  Horodate:%lf Numero:%d No d'analyseur:%d Sens:%d Chaussee:%d TO:%d Vit Moy:%d.",
     				vl_Mes.Horodate, vl_Numero, vl_NumAnalyseur, vl_Sens, vl_Chaussee, 
     				vl_Mes.TO, vl_Mes.VitesseMoyenne );

      /*A Renseignement de la structure XZEDT_EMes_DAI */
      vl_Mes.Numero		= vl_Numero;
      vl_Mes.NumAnalyseur	= vl_NumAnalyseur;
      vl_Mes.Sens		= vl_Sens;
      vl_Mes.Chaussee		= vl_Chaussee;
#ifndef _TIBCO_EMS
      for ( vl_Index = 0;
            (vl_Index < vl_SizeVit) && (vl_Index < sizeof(vl_Mes.VitesseVoie)/sizeof(vl_Mes.VitesseVoie[0]));
            vl_Mes.VitesseVoie[vl_Index] = *pl_Vitesse, vl_Index++, pl_Vitesse++);
      for ( vl_Index = 0;
            (vl_Index < vl_SizeLon) && (vl_Index < sizeof(vl_Mes.LongueurVoie)/sizeof(vl_Mes.LongueurVoie[0]));
            vl_Mes.LongueurVoie[vl_Index] = *pl_Longueur, vl_Index++, pl_Longueur++);
#else
      for ( vl_Index = 0;
            (vl_Index < vl_SizeVit) && (vl_Index < sizeof(vl_Mes.VitesseVoie)/sizeof(vl_Mes.VitesseVoie[0]));
            vl_Index++)
            vl_Mes.VitesseVoie[vl_Index] = pl_Vitesse[vl_Index];
      for ( vl_Index = 0;
            (vl_Index < vl_SizeLon) && (vl_Index < sizeof(vl_Mes.LongueurVoie)/sizeof(vl_Mes.LongueurVoie[0]));
            vl_Index++)
            vl_Mes.LongueurVoie[vl_Index] = pl_Longueur[vl_Index];
#endif
	       
      /*A Appel de la fonction utilsateur avec la structure XZEDT_EMes_DAI en argument */
      pg_FonctUtil_XZED_02 ( vl_Mes, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZED_EMes_DAI %d", vl_Resultat );
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

int	XZED02_Demande_Init_DAI (	XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					XDY_Eqt		va_NumEqt,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Eqt	va_NumEqt		: Numero d'analyseur
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
*   Ce service est demande pour recevoir les mesures DAI d'un analysuer d'un site
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups correspondant
*   Envoi du message demande d'init
*
------------------------------------------------------*/
{
static char *version = "@(#)xzed.c	1.6 11/13/95 : XZED02_Demande_Init_DAI" ;
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

      XZST_03EcritureTrace ( XZSTC_FONCTION, 
      "IN  : XZED02_Demande_Init_DAI  DGInit:%s Fonct:%ld NomSite:%s.",
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite,
     		va_NumEqt );
     		
      /* Memorisation en globale du nom de la fonction utilisateur */		
      pg_FonctUtil_XZED_02 = pa_FonctUtilisateur;
     
      /*A  Armement Callback sur Reception XDM_EMES_DAI */
      if ( vg_CB_XZED02 == NULL )
      {
	 if ( ( vg_CB_XZED02 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_DAI),
							XZED_EMes_DAI,
							(T_CB_ARG)NULL ) ) == NULL ) 
	 {	
	    /*B Ecriture Trace */
	    XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_DAI." );
	    vl_ValRet = XDC_NOK;
	 }
	 else
	 {	
	    /*B Ecriture Trace */
	    XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_DAI." );
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
	   
      /* Envoi du message de demande d'init XDM_EInit_IHM_DAI */
      /*A Construction du DatGroup XDG_EInit_IHM_DAI_N<NumEqt> */
      sprintf ( pl_NomDG, "%s_N%d", XDG_EInit_IHM_DAI, va_NumEqt );
   	
      /*A Envoi du message XDM_EInit_IHM_DAI en fournissant les valeur recues  */
   
      if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  	XDM_IdentMsg(XDM_EInit_IHM_DAI),
  			  	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_STR,  pa_DGInit,
                          	T_IPC_FT_INT2, va_NumEqt,
                          	NULL))
      {
  	 /*A Trace envoi message XDM_EInit_IHM_DAI non effectue */    	
          XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_DAI.");
      }
      else
      {
  	  /*A Trace envoi message XDM_EInit_IHM_DAI */    	
          XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_DAI sur %s.", pl_NomDG );
      }
     
      XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZED02_Demande_Init_DAI Retour:%d.", vl_ValRet );
      *pa_Resultat = vl_ValRet;
      return ( vl_ValRet );				
				
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	XZED03_Demande_Enregistrement_DAI (	XDY_Eqt			vl_NumEqt,
										XDY_Nom			vl_cmd,
										XDY_LongEntier	vl_Duree,
										int *			pa_Resultat )
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Eqt			vl_NumEqt		: Numero de l'equipement
* XDY_Nom			vl_cmd			: Peut valoir 'D' ou 'A' : demarrage / arret de l'enregistrement
* XDY_LongEntier	vl_Duree		: Duree de l'enregistrement en secondes (<3600)
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour declencher l'enregistrement de la video
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups correspondant
*   Envoi du message demande d'enregistrement video
*
------------------------------------------------------*/
{
	XDY_Datagroup	pl_NomDG;
	int 			vl_ValRet = XDC_OK;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZED03_Demande_Enregistrement_DAI vl_NumEqt:%d vl_cmd=%s vl_Duree:%d.", vl_NumEqt, vl_cmd, vl_Duree);

	/* Armement Callback sur Reception XDM_RECORD_DAI */
	if ( vg_CB_XZED03 == NULL )
	{
		if ( ( vg_CB_XZED03 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_RECORD_DAI),
														XZED_EMes_DAI,
														(T_CB_ARG)NULL ) ) == NULL) 
		{	
			XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_RECORD_DAI.");
			vl_ValRet = XDC_NOK;
		}
		else
		{	
			XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_RECORD_DAI.");
		}
	}

	/* Envoi du message de demande d'init XDM_RECORD_DAI */
	/* Construction du DatGroup XDG_ECMD_DAI<NumEqt> */
	sprintf ( pl_NomDG, "%s_N%d", XDG_ECMD_DAI, vl_NumEqt );

	/* Envoi du message XDM_RECORD_DAI en fournissant les valeur recues  */
	if (!TipcSrvMsgWrite ( 	pl_NomDG,
							XDM_IdentMsg(XDM_RECORD_DAI),
							XDM_FLG_SRVMSGWRITE,
							T_IPC_FT_INT2,	vl_NumEqt,
							T_IPC_FT_STR,	vl_cmd,
							T_IPC_FT_INT2,	vl_Duree,
							NULL))
	{
		/* Trace envoi message XDM_RECORD_DAI non effectue */    	
		XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_RECORD_DAI.");
	}
	else
	{
		/* Trace envoi message XDM_RECORD_DAI */    	
		XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_RECORD_DAI sur %s.", pl_NomDG);
	}

	XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZED03_Demande_Enregistrement_DAI Retour:%d.", vl_ValRet);
	*pa_Resultat = vl_ValRet;
	return ( vl_ValRet );				
}