/*E*/
/*Fichier : $Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2016/12/08 16:23:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MVF * FICHIER cgcd_msr.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  	Fichier appartenant au module MVF contenant les fonctions
*  modificationSGBDreseau. 
**********************
*	cmvf_FctUti_MSR_XZAO50_51 : Fct utilisateur passee a XZAO50,
*  qui appelle XZAO51 avec le nom de la base sur la quelle XZAO51 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO52_53 : Fct utilisateur passee a XZAO52,
*  qui appelle XZAO53 avec le nom de la base sur la quelle XZAO53 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO54_55 : Fct utilisateur passee a XZAO54,
*  qui appelle XZAO55 avec le nom de la base sur la quelle XZAO55 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO56_57 : Fct utilisateur passee a XZAO56,
*  qui appelle XZAO57 avec le nom de la base sur la quelle XZAO57 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO58_59 : Fct utilisateur passee a XZAO58
*  qui appelle XZAO59 avec le nom de la base sur la quelle XZAO59 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO60_61 : Fct utilisateur passee a XZAO60,
*  qui appelle XZAO61 avec le nom de la base sur la quelle XZAO61 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO83_84 : Fct utilisateur passee a xzao83_Lire_Config_Echangeur_Station ,
*  qui appelle XZAO84_Ajouter_Echangeur avec le nom de la base sur la quelle XZAO84_Ajouter_Echangeur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO83_84 : Fct utilisateur passee a xzao83_Lire_Config_Echangeur_Station ,
*  qui appelle XZAO84_Ajouter_Echangeur avec le nom de la base sur la quelle XZAO84_Ajouter_Echangeur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*	cmvf_FctUti_MSR_XZAO85_86 : Fct utilisateur passee a xzao85_Lire_Config_Air ,
*  qui appelle XZAO86_Ajouter_Aire avec le nom de la base sur la quelle XZAO86_Ajouter_Aire 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO87_88 : Fct utilisateur passee a xzao87_Lire_Config_Peage ,
*  qui appelle XZAO88_Ajouter_Peage avec le nom de la base sur la quelle XZAO88_Ajouter_Peage 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
**********************
*
*	cmvf_FctUti_MSR_XZAO87_88 : Fct utilisateur passee a xzao87_Lire_Config_Peage ,
*  qui appelle XZAO88_Ajouter_Peage avec le nom de la base sur la quelle XZAO88_Ajouter_Peage 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO89_90 : Fct utilisateur passee a xzao89_Lire_Config_PS ,
*  qui appelle XZAO90_Ajouter_PS avec le nom de la base sur la quelle XZAO90_Ajouter_PS 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO91_92 : Fct utilisateur passee a  xzao91_Lire_Config_Reseau,
*  qui appelle XZAO92_Ajouter_Reseau avec le nom de la base sur la quelle XZAO92_Ajouter_Reseau 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO93_94 : Fct utilisateur passee a  xzao91_Lire_Config_Reseau,
*  qui appelle XZAO94_Ajouter_Ville avec le nom de la base sur la quelle XZAO94_Ajouter_Ville 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO120_121 : Fct utilisateur passee a xzao120_Lire_Config_Panneau_Police ,
*  qui appelle XZAO121_Ajouter_Panneau_Police avec le nom de la base sur la quelle XZAO121_Ajouter_Panneau_Police 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO145_146 : Fct utilisateur passee a XZAO145 ,
*  qui appelle XZAO146_Ajouter_ITPC avec le nom de la base sur la quelle XZAO146_Ajouter_ITPC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO150_151 : Fct utilisateur passee a XZAO150 ,
*  qui appelle XZAO151_Ajouter_Portail avec le nom de la base sur la quelle XZAO151_Ajouter_Portail 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO405_406 : Fct utilisateur passee a  xzao406_Lire_Config_Destination_TDP,
*  qui appelle XZAO405_Ajouter_Destination_TDP avec le nom de la base sur la quelle XZAO405_Ajouter_Destination_TDP 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************
*	cmvf_FctUti_MSR_XZAO649_650 : Fct utilisateur passee a xzao649_Lire_Config_Viaduc ,
*  qui appelle XZAO650_Ajouter_Viaduc avec le nom de la base sur la quelle XZAO650_Ajouter_Viaduc 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
**********************


------------------------------------------------------
* HISTORIQUE :
*
* mercier	18 Oct 1994	: Creation
*****************
* MERCIER	21 Dec 1994	: Modification de l appel V 1.3
*  de xzia_08
* TORREGROSSA	30 Oct 1995	: Modification de l'interface d'appel cmvf_FctUti_MSR_XZAO50_51 (A57)V 1.6
* NIEPCERON	07 Jun 1996	: Suppression de l'affichage des messages de trait. des lignes v1.7 (968)
* MISMER	10 Fev 1997	: Ajout des fonction temps de parcours v1.8 (DEM/1395)
* NIEPCERON	13 Jan 1998	: modif intergest XZAO51 v1.9 dem/1724
* VR	28/11/11 : Ajout recherche_dest_pmv (DEM/1014)
* VR	05/06/12 : Ajout cmvf_FctUti_MSR_XZAO466 (DEM/1014 PMA)
* JPL	13/07/16 : Migration architecture ia64 : compatibilite des types d'arguments  1.12
* LCL	15/11/16 : Ajout cmvf_FctUti_MSR_XZAO649_650 (LOT23) 	1.13
* ABE 	02/08/22 : Creation NIC et IS DEM-SAE403
* GGY	16/02/24 : Ajout des batiments (DEM-483)
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_msr.h"
#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cgcd_msr" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO50_51 : Fct utilisateur passee a XZAO50,
*  qui appelle XZAO51 avec le nom de la base sur la quelle XZAO51 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSR_XZAO50_51(XDY_NomAuto     va_Autoroute_in, XDY_Mot va_Inversee_in, XDY_Octet va_Escota_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Autoroute_in contient l autoroute a rajouter
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO51_Ajouter_Autoroute	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO50_51";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO50_51");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO51_Ajouter_Autoroute(vl_NomBase,va_Autoroute_in, va_Inversee_in, va_Escota_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO50_51 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO51_Ajouter_Autoroute sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO51_Ajouter_Autoroute");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_AUTOROUTE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO50_51 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO50_51 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO52_53 : Fct utilisateur passee a XZAO52,
*  qui appelle XZAO53 avec le nom de la base sur la quelle XZAO53 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSR_XZAO52_53(XZAOT_ConfDistrict     va_District_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_District_in contient de district a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO53_Ajouter_District	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO52_53";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg [255];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO52_53");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO53_Ajouter_District(vl_NomBase,va_District_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO52_53 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO53_Ajouter_District sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO53_Ajouter_District");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DISTRICT,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO52_53 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO52_53 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO54_55 : Fct utilisateur passee a xzao54_Lire_Config_Portion ,
*  qui appelle XZAO55_Ajouter_Portion avec le nom de la base sur la quelle XZAO55_Ajouter_Portion va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO54_55(XZAOT_ConfPortion       va_Portion_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Portion_in contient la portion a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO55_Ajouter_Portion	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO54_55";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO54_55");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO55_Ajouter_Portion(vl_NomBase,va_Portion_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO54_55 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO55_Ajouter_Portion sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO55_Ajouter_Portion");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PORTION,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO54_55 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO54_55 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO56_57 : Fct utilisateur passee a xzao56_Lire_Config_Noeud ,
*  qui appelle XZAO57_Ajouter_Noeud avec le nom de la base sur la quelle XZAO57_Ajouter_Noeud va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO56_57(XZAOT_ConfNoeud       va_Noeud_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Noeud_in contient le noeud a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO57_Ajouter_Noeud	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO56_57";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO56_57");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO57_Ajouter_Noeud(vl_NomBase,va_Noeud_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO56_57 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO57_Ajouter_Noeud sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO57_Ajouter_Noeud");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_NOEUD,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO56_57 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO56_57 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO58_59: Fct utilisateur passee a xzao58_Lire_Config_Segment ,
*  qui appelle XZAO59_Ajouter_Segment avec le nom de la base sur la quelle XZAO59_Ajouter_Segment 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO58_59(XZAOT_ConfSegment       va_Segment_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Segment_in contient le segment a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO59_Ajouter_Segment	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO58_59";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO58_59");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO59_Ajouter_Segment(vl_NomBase,va_Segment_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO58_59 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO59_Ajouter_Segment sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO59_Ajouter_Segment");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEGMENT,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO58_59: cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO58_59: sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO60_61 : Fct utilisateur passee a xzao60_Lire_Config_Pente ,
*  qui appelle XZAO61_Ajouter_Pente avec le nom de la base sur la quelle XZAO61_Ajouter_Pente 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO60_61(XZAOT_ConfPente         va_Pente_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Pente_in contient la pente a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO61_Ajouter_Pente	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO60_61";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO60_61");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO61_Ajouter_Pente(vl_NomBase,va_Pente_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO60_61 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO61_Ajouter_Pente sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO61_Ajouter_Pente");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PENTE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
			/* On previent l IHM de l administrateur*/
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO60_61 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO60_61 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO83_84 : Fct utilisateur passee a xzao83_Lire_Config_Echangeur_Station ,
*  qui appelle XZAO84_Ajouter_Echangeur avec le nom de la base sur la quelle XZAO84_Ajouter_Echangeur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO83_84( XZAOT_ConfEchang         va_Echangeur_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Echangeur_in contient l echangeur a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO84_Ajouter_Echangeur	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO83_84";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO83_84");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO84_Ajouter_Echangeur(vl_NomBase,va_Echangeur_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO83_84 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO84_Ajouter_Echangeur sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO84_Ajouter_Echangeur");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ECHANGEUR,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO83_84 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO83_84 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO85_86 : Fct utilisateur passee a xzao85_Lire_Config_Air ,
*  qui appelle XZAO86_Ajouter_Aire avec le nom de la base sur la quelle XZAO86_Ajouter_Aire 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO85_86(  XZAOT_ConfAire          va_Aire_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Aire_in contient l aire a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO86_Ajouter_Aire	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO85_86";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO85_86");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO86_Ajouter_Aire(vl_NomBase,va_Aire_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO85_86 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO86_Ajouter_Aire sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO86_Ajouter_Aire");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_AIRE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO85_86 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO85_86 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO87_88 : Fct utilisateur passee a xzao87_Lire_Config_Peage ,
*  qui appelle XZAO88_Ajouter_Peage avec le nom de la base sur la quelle XZAO88_Ajouter_Peage 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO87_88(XZAOT_ConfPeage         va_Peage_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Peage_in contient l aire a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO88_Ajouter_Peage	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO87_88";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO87_88");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO88_Ajouter_Peage(vl_NomBase,va_Peage_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO87_88 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO88_Ajouter_Peage sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO88_Ajouter_Peage");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PEAGE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO87_88 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO87_88 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO89_90 : Fct utilisateur passee a xzao89_Lire_Config_PS ,
*  qui appelle XZAO90_Ajouter_PS avec le nom de la base sur la quelle XZAO90_Ajouter_PS 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO89_90(XZAOT_ConfPS    va_PS_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_PS_in contient le Passage Supperieur a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO90_Ajouter_PS	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO89_90";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO89_90");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO90_Ajouter_PS(vl_NomBase,va_PS_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO89_90 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO90_Ajouter_PS sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO90_Ajouter_PS");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PS,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO89_90 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO89_90 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO91_92 : Fct utilisateur passee a  xzao91_Lire_Config_Reseau,
*  qui appelle XZAO92_Ajouter_Reseau avec le nom de la base sur la quelle XZAO92_Ajouter_Reseau 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO91_92(XZAOT_ConfReseau        va_Reseau_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Reseau_in contient le Passage Supperieur a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO92_Ajouter_Reseau	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO91_92";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO91_92");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO92_Ajouter_Reseau(vl_NomBase,va_Reseau_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO91_92 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO92_Ajouter_Reseau sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO92_Ajouter_Reseau");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_RESEAU,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO91_92 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO91_92 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO93_94 : Fct utilisateur passee a  xzao91_Lire_Config_Reseau,
*  qui appelle XZAO94_Ajouter_Ville avec le nom de la base sur la quelle XZAO94_Ajouter_Ville 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO93_94(XZAOT_ConfVille         va_Ville_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Ville_in contient la ville a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO94_Ajouter_Ville	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO93_94";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO93_94");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO94_Ajouter_Ville(vl_NomBase,va_Ville_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO93_94 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO94_Ajouter_Ville sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO94_Ajouter_Ville");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_VILLE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO93_94 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO93_94 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO120_121 : Fct utilisateur passee a xzao120_Lire_Config_Panneau_Police ,
*  qui appelle XZAO121_Ajouter_Panneau_Police avec le nom de la base sur la quelle XZAO121_Ajouter_Panneau_Police 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO120_121(XZAOT_ConfPnPol va_PnPol_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_PnPol_in contient le panneau de police a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO121_Ajouter_Panneau_Police	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO120_121";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO120_121");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO121_Ajouter_Panneau_Police(vl_NomBase,va_PnPol_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO120_121 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO121_Ajouter_Panneau_Police sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO121_Ajouter_Panneau_Police");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PAN_POLICE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO120_121 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO120_121 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO145_146 : Fct utilisateur passee a XZAO145 ,
*  qui appelle XZAO146_Ajouter_ITPC avec le nom de la base sur la quelle XZAO146_Ajouter_ITPC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO145_146(XZAOT_ConfITPC  va_ITPC_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_ITPC_in contient l interuption de terre-plein central ITPC a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO146_Ajouter_ITPC	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO145_146";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO145_146");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO146_Ajouter_ITPC(vl_NomBase,va_ITPC_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO145_146 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO146_Ajouter_ITPC sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO146_Ajouter_ITPC");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ITPC,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO145_146 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO145_146 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO150_151 : Fct utilisateur passee a XZAO150 ,
*  qui appelle XZAO151_Ajouter_Portail avec le nom de la base sur la quelle XZAO151_Ajouter_Portail 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO150_151(XZAOT_ConfPortail       va_Portail_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Portail_in contient le portail a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO151_Ajouter_Portail	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO150_151";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO150_151");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO151_Ajouter_Portail(vl_NomBase,va_Portail_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO150_151 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO151_Ajouter_Portail sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO151_Ajouter_Portail");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PORTAIL,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO150_151 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO150_151 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO405_406 : Fct utilisateur passee a xzao406_Lire_Config_Destination_TDP ,
*  qui appelle XZAO405_Ajouter_Destination_TDP avec le nom de la base sur la quelle XZAO405_Ajouter_Destination_TDP 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO405_406(XZAOT_ConfDestTDP       va_Destination_TDP_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Destination_TDP_in contient la destination a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO405_Ajouter_Destination_TDP	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO405_406";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO405_406");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO405_Ajouter_Destination_TDP(vl_NomBase,va_Destination_TDP_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO405_406 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO405_Ajouter_Destination_TDP sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO405_Ajouter_Destination_TDP");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DTP XDF_CONF_PORTAIL,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO405_406 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO405_406 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO461 : Fct utilisateur passee a XZAO461_Ajouter_Destination_TDP ,
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO461(XZAOT_ConfDestPMV       va_PMV_Destination_TDP_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_PMV_Destination_TDP_in contient la destination a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO461_Ajouter_Destination_TDP	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO461";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO461");
	
	XZST_03EcritureTrace( XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO461 : Autoroute = %s\tPR = %d\tSens = %d\tNumDestination: %s\tType: %d\tinhibition:%d\tNom ZDP=%s\tinit:%d\n",
											va_PMV_Destination_TDP_in.Autoroute,
											va_PMV_Destination_TDP_in.PR,
											va_PMV_Destination_TDP_in.Sens,
											va_PMV_Destination_TDP_in.NomDest,
											va_PMV_Destination_TDP_in.TypeDest,
											va_PMV_Destination_TDP_in.Inhibition,
											va_PMV_Destination_TDP_in.Nom_ZDP,
											va_PMV_Destination_TDP_in.Init);


	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	XZST_03EcritureTrace(XZSTC_WARNING,"dest.init %d",va_PMV_Destination_TDP_in.Init);

	if ((vl_retcode =XZAO461_Ajouter_Destination_TDP(vl_NomBase,va_PMV_Destination_TDP_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO461 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO461_Ajouter_Destination_TDP sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO461_Ajouter_Destination_TDP");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DTP XDF_CONF_PORTAIL,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO461 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO461 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*	cmvf_FctUti_MSR_XZAO461 : Fct utilisateur passee a XZAO461_Ajouter_Destination_TDP ,
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO466(XZAOT_ConfDestPMA       va_PMA_Destination_TDP_in, XDY_Entier     va_Resultat_out)
/*
* ARGUMENTS EN ENTREE : va_PMV_Destination_TDP_in contient la destination a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours
*			de traitement.
*
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR :
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION
*	Doit appeler XZAO461_Ajouter_Destination_TDP
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO466";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[800]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[800]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;

	XDY_ResConf vl_Resultat_out;

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO466");

	XZST_03EcritureTrace( XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO466 : NomDest = %s\tAutoroute = %s\tPR = %d\tSens = %d\tType: %d\tSensDest:%d\tNom ZDP=%s\tInhibition : %d\tInit:%d\n",
			va_PMA_Destination_TDP_in.NomDest,
			va_PMA_Destination_TDP_in.Autoroute,
			va_PMA_Destination_TDP_in.PR,
			va_PMA_Destination_TDP_in.Sens,
			va_PMA_Destination_TDP_in.TypeDest,
			va_PMA_Destination_TDP_in.SensDest,
			va_PMA_Destination_TDP_in.ZDP,
			va_PMA_Destination_TDP_in.Inhibition,
			va_PMA_Destination_TDP_in.Init);

	strcpy( vl_NomBase,CGCDG_Base_Courante);

	XZST_03EcritureTrace(XZSTC_WARNING,"dest.init %d",va_PMA_Destination_TDP_in.Init);

	if ((vl_retcode =XZAO466_Ajouter_Destination_TDP_PMA(vl_NomBase,va_PMA_Destination_TDP_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO466 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO466_Ajouter_Destination_TDP_PMA sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO466_Ajouter_Destination_TDP_PMA");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DTP XDF_CONF_PORTAIL,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO466 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
		vl_retcode = vl_retcode_out;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO466 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO649_650 : Fct utilisateur passee a xzao649_Lire_Config_Viaduc ,
*  qui appelle XZAO650_Ajouter_Viaduc avec le nom de la base sur la quelle XZAO650_Ajouter_Viaduc 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO649_650(  XZAOT_ConfViaduc          va_Viaduc_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE : va_Viaduc_in contient le viaduc a rajouter en base
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO650_Ajouter_Viaduc	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO649_650";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO649_650");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO650_Ajouter_Viaduc(vl_NomBase,va_Viaduc_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO649_650 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO650_Ajouter_Viaduc sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO650_Ajouter_Viaduc");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_VIADUC,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO649_650 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO649_650 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO506_507 : Fct utilisateur passee a XZAO506,
*  qui appelle XZAO51 avec le nom de la base sur la quelle XZAO51 va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSR_XZAO506_507(XZAOT_ConfNIC_IS va_NIC_IS_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  
*
* XZAOT_ConfNIC_IS va_NIC_IS_in contient les donnees de la niche ou de l'issue de secour a rajouter
*			 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO507_Ajouter_NIC_IS
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_msr.c,v 1.13 2016/12/08 16:23:46 pc2dpdy Exp $ : cmvf_FctUti_MSR_XZAO506_507";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500]="";			/* contient le mesg a tracer*/
	char  	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO506_507");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);
	
	if ((vl_retcode =XZAO507_Ajouter_NIC_IS(vl_NomBase, va_NIC_IS_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO506_507 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO507_Ajouter_NIC_IS sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO507_Ajouter_NIC_IS");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_AUTOROUTE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO506_507 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO506_507 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}


/*
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSR_XZAO511_510 : Fct utilisateur passee a xzao511_Lire_Config_Bat ,
*  qui appelle XZAO510_Ajouter_Batiment avec le nom de la base sur la quelle XZAO510_Ajouter_Batiment
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MSR_XZAO511_510	(XZAOT_ConfBat va_Bat_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*
*	va_Bat_in			contient le batiment a ajouter en base
*			
* ARGUMENTS EN SORTIE :
*
*	va_Resultat_out		contient le numero de ligne du fichier qui est en cours 
*						de traitement.
*
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO510_Ajouter_Batiment	
* 	
------------------------------------------------------*/
{
	char	vl_NomBase[4]	;
	int		vl_retcode 	= XDC_OK;
	int		vl_retcode_out 	= XDC_OK;
	char	pl_Msg[500]="";				/* contient le mesg a tracer*/
	char	pl_Msg2[500]="";			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSR_XZAO511_510");
	
	strcpy (vl_NomBase, CGCDG_Base_Courante);
	
	if ((vl_retcode = XZAO510_Ajouter_Batiment (vl_NomBase, va_Bat_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO511_510 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/* Test sur parametres de sortie de XZAO510_Ajouter_Batiment sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO510_Ajouter_Batiment");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_BATIMENT,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/		
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSR_XZAO511_510 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}	
		vl_retcode = vl_retcode_out;		
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSR_XZAO511_510 : sortie avec %d",vl_retcode_out);

	return(vl_retcode_out);
}
