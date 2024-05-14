/*E*/
/*Fichier : $Id: xzia.c,v 1.18 2016/12/05 17:47:00 devgfi Exp $      Release : $Revision: 1.18 $        Date : $Date: 2016/12/05 17:47:00 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE XZIA * FICHIER xzia.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module regroupe tous les services de rafra—chissement 
* de l'IHM textuelle.
*
*
------------------------------------------------------
* HISTORIQUE :
*
* GUILHOU	29 Sep 1994 : Creation
* MERCIER	14 Oct 1994 : Correction du bouchon XZIA08 V 1.5
* Guilhou	21 dec 1994 : interface xzia06	V1.6
* Guilhou	21 dec 1994 : interface xzia08	V1.7
* Guilhou	30 dec 1994 : code xzia08 pour msg adm	V1.8
* Mercier	16 Jan 1995 : enlevement des extern (.h)	V 1.9
* Lecrivain	12 Avr 1995 : Remplacement Newtask par Sendpoke (XZIA06) V 1.11
* T.Milleville 	11 Oct 1995 : Ajout de l'horodate en debut de texte 
*	pour affichage operateur V1.13
* T.Milleville 	17 Oct 1995 : Modification code XZIA_06 et XZIA_08 V1.14
* Torregrossa	17 Jan 1996 : Remplacement Newtask par Sendpoke (XZIA08) V 1.15
* JMG		26/06/10 : linux DEM/934
* JPL		29/06/16 : Suppression traces superflues  1.17
* JPL		10/11/16 : Suppression autres traces superflues  1.18
* ABE		21/09/22 : Remplace le nome machine par le site en fonction du nom de la machine pour fonctionnement sous magistra DEM-SAE311
------------------------------------------------------*/

/* fichiers inclus */

#include "xzia.h"


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzia.c,v 1.18 2016/12/05 17:47:00 devgfi Exp $ $Revision: 1.18 $ $Date: 2016/12/05 17:47:00 $" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* affiche un libelle dans la zone d'avertissement
* de l'ecran synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZIA_06AffMsgSyst(XZIAT_destinataire va_destinataire, char *pa_libelle)

/*
* ARGUMENTS EN ENTREE :
*
* va_destinataire	destinataire du libelle	
* va_libelle 		message a afficher	
*
*
* ARGUMENTS EN SORTIE :
* Aucun
*
*
* CODE RETOUR : 
*  
* XDC_NOK	Erreur d'ex≈cution de la fonction
* XDC_OK	Bonne ex≈cution de la fonction
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	/*A Definition des variables locales */
	static char *version = "$Id: xzia.c,v 1.18 2016/12/05 17:47:00 devgfi Exp $" ;

	XZSCT_Datagroup         vl_DG	= "";          /* datagroup d'emission */
	XZSMT_Horodate  vl_heureTrace	= "";
	int             vl_jourSemaine	= 0;
	double          vl_horodateSec = 0.0;
	char 	*	pl_Libelle_Horo	= NULL;
	char 		pl_Libelle_Inter[]	= " : ";


	XZST_03EcritureTrace(XZSTC_INTERFACE ,
		"XZIA_06AffMsgSyst : IN -- Destinataire %s -- Texte %s \n",
			va_destinataire.site, pa_libelle);

 	/*A Construction du datagroup d'emission */
 	XDG_EncodeDG2(vl_DG, XDG_IAVERT_OPERATEUR, va_destinataire.site);

	/****   Lecture Heure Systeme ******/
	XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine
			      , (double *) &vl_horodateSec );

	/*  Conversion heure pour affichage trace */
	XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heureTrace );

	/* Allocation Memoire du Libelle Horoadate */
	pl_Libelle_Horo = (char *) malloc (sizeof(char) * (strlen(pa_libelle)+strlen(vl_heureTrace)+strlen(pl_Libelle_Inter)+1) );
	sprintf (pl_Libelle_Horo,"%s%s%s",vl_heureTrace,pl_Libelle_Inter,pa_libelle);

	XZST_03EcritureTrace(XZSTC_DEBUG3 ,"XZIA_06AffMsgSyst :Affichage Texte %s \n",pl_Libelle_Horo);

	if ( (*pa_libelle) != '\0' )
	{
 		TipcSrvMsgWrite(vl_DG,
		 	XDM_IdentMsg(XDM_IAX_SENDPOKE),
		 	XDM_FLG_SRVMSGWRITE,
		 	T_IPC_FT_INT4, COM_SENDPOKE,
		 	T_IPC_FT_INT4, COM_CANAL_MTAV,
		 	T_IPC_FT_STR, pl_Libelle_Horo,
		 	NULL);
 		TipcSrvFlush();
	}

	/* Liberation memoire */
	free(pl_Libelle_Horo);

	XZST_03EcritureTrace(XZSTC_INTERFACE ,
		"XZIA_06AffMsgSyst : OUT \n");

 	return (XDC_OK);

}		/* Fin XZIA_06AffMsgSyst */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer un message a afficher dans la fenetre operateur 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZIA08_AffMsgAdm(
XZIAT_destinataire va_destinataire,
char *va_texte
          )

/*
* ARGUMENTS EN ENTREE :
*   	va_destinataire : destinataire du message
*	va_texte : Texte a afficher
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
------------------------------------------------------*/
{
	/*A Definition des variables locales */
	static char *version = "$Id: xzia.c,v 1.18 2016/12/05 17:47:00 devgfi Exp $ : XZIA08_AffMsgAdm" ;

	XZSCT_Datagroup         vl_DG;          /* datagroup d'emission */
	char                    *pl_str;        /* contenu du message*/
	XZSMT_Horodate  vl_heureTrace	= "";
	int             vl_jourSemaine	= 0;
	double          vl_horodateSec = 0.0;
	char 	*	pl_Libelle_Horo	= NULL;
	char 		pl_Libelle_Inter[]	= " : ";
	char * vl_ret;

  	/*trace*/
  	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZIA08_AffMsgAdm :IN  message %s",va_texte);

 	/*A Construction du datagroup d'emission en fonction du site dans le nom de la machine pour Magistra*/
	if (vl_ret=strstr(va_destinataire.machine,XDC_NOM_SITE_CI)!=NULL)
	{
		XDG_EncodeDG2(vl_DG, XDG_IAVERT_ADMINISTRATEUR, XDC_NOM_SITE_CI);
	}
	else if (vl_ret=strstr(va_destinataire.machine,XDC_NOM_SITE_DP)!=NULL)
	{
		XDG_EncodeDG2(vl_DG, XDG_IAVERT_ADMINISTRATEUR, XDC_NOM_SITE_DP);
	}
	else if (vl_ret=strstr(va_destinataire.machine,XDC_NOM_SITE_VC)!=NULL)
	{
		XDG_EncodeDG2(vl_DG, XDG_IAVERT_ADMINISTRATEUR, XDC_NOM_SITE_VC);
	}
	
	/****   Lecture Heure Systeme ******/
	XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine
			      , (double *) &vl_horodateSec );

	/*  Conversion heure pour affichage trace */
	XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heureTrace );

	/* Allocation Memoire du Libelle Horodate */
	pl_Libelle_Horo = (char *) malloc (sizeof(char) * (strlen(va_texte)+strlen(vl_heureTrace)+strlen(pl_Libelle_Inter)+1) );
	sprintf (pl_Libelle_Horo,"%s%s%s",vl_heureTrace,pl_Libelle_Inter,va_texte);

	XZST_03EcritureTrace(XZSTC_DEBUG3 ,"XZIA08_AffMsgAdm :Affichage Texte %s \n",pl_Libelle_Horo);

	if ( (*va_texte) != '\0' && vl_ret!=NULL)
	{
 		TipcSrvMsgWrite(vl_DG,
		 	XDM_IdentMsg(XDM_IAX_NEWTASK),
		 	XDM_FLG_SRVMSGWRITE,
		 	T_IPC_FT_INT4, COM_SENDPOKE,
		 	T_IPC_FT_INT4, COM_CANAL_MTAA,
		 	T_IPC_FT_STR, pl_Libelle_Horo,
		 	NULL);
 		TipcSrvFlush();
 	}

	/* Envoie Ègalment sur le DG transmit pour migrazur et autre taches utilisatrice */
 	XDG_EncodeDG2(vl_DG, XDG_IAVERT_ADMINISTRATEUR, va_destinataire.machine);

	if ( (*va_texte) != '\0')
	{
 		TipcSrvMsgWrite(vl_DG,
		 	XDM_IdentMsg(XDM_IAX_NEWTASK),
		 	XDM_FLG_SRVMSGWRITE,
		 	T_IPC_FT_INT4, COM_SENDPOKE,
		 	T_IPC_FT_INT4, COM_CANAL_MTAA,
		 	T_IPC_FT_STR, pl_Libelle_Horo,
		 	NULL);
 		TipcSrvFlush();
 	}

	/* Liberation memoire */
	free(pl_Libelle_Horo);

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZIA08_AffMsgAdm :OUT \n");

	return (XDC_OK);
}	


