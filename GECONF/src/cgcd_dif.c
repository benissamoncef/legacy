/*E*/
/*Fichier : $Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $      Release : $Revision: 1.17 $        Date : $Date: 2016/07/13 22:50:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MDIF * FICHIER cgcd_dif.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de diffusion de tcgcd gerant le configuration.
* (DCG-002/0.1 page 35). 
*************************
* SERVICE RENDU : 
*	cdif_cb : Callback de la tache TCGCD effectuant la diffusion.
*************************
*	cdif_FctUtil_Gest_ligne_bl permet de traiter 
*  une ligne du Bon de diffusion conformement a la fonctionalite
*  de diffusion des fichiers par tcgcd.x (page35 du DCG-002/0.1).
*************************
*	cdif_Arret_Demarrage_SousSyst appelle l interface de supervision
*  permettant d arret un sous-système avant diffusion et formate
*  les messages d erreurs pour l ihm de l administrateur
*************************
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	13 Dec 1994	: Creation
*************
* MERCIER	17 Janv 1995	: Ajout d un argument a XZSP_05		V 1.3
*************
* MERCIER O.	28 Jan	1995	: Gestion pour GESIHMEXE des fichiers	V 1.4
* *.d *.elo *.ax dans la diffusion
* Arret provisoire de la demande des taches d un ss systeme au noyau
* pour GESIHM.
*************
* MERCIER O.	16 Mars	1995	: Amelioration presentation des	messagesV 1.5
* 	pour l administrateur systeme. Decommante l arret et redemarrage 
* 	des sous-systèmes par le noyau de Supervision.
* MERCIER	Ajout de cdif_Arret_Demarrage_SousSyst V1.6
*************
* NIECPERON     08 Jun 1995     : Ajout du traitement associe aux fichiers inf V1.7
*************
* NIECPERON     14 Jun 1995     : filtrage du type de machine pour diff de BDC V1.8
*************
* NIECPERON     16 Jun 1995     : Ajout d'une attente de 10 sec aprés arret d'un ss-systeme ( 30 sec pour EQUEXT) v1.9
*************
* NIECPERON     20 Jun 1995     : Ajout du changt de proprietaire pour tscmd et tshor v1.10
*************
* NIECPERON     21 Jun 1995     : Ajout du stickybit pour tscmd et tshor +
				  vl_line passe de 80 a 500 pour eviter le core v1.11
				  Ajout d'un msg de fin de diffusion
*************
* NIECPERON     29 Jan 1996     : Ajout du ssysteme DIE v1.12
*************
* NIECPERON     05 Jan 1996     : Ajout du ssysteme MOD v1.13
*************
* NIECPERON     01 Fev 1996	: Correction des accents des msg systeme v1.14
* NIEPCERON     21 Oct 1996     : Ajout des ssysteme OPER UTIL et NTFM v1.15
* NIEPCERON     27 Jan 1999	: Ajout du ssyteme STRA 1.16 dem/1728
* JPL		13/07/2016	: Suppression de warnings de compilation mineurs  1.17
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_dif.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $ : cgcd_dif" ;

/* declaration de fonctions internes */

int cdif_Arret_Demarrage_SousSyst (XZSCT_NomMachine, XZSCT_NomSSysteme , int);


/* definition de fonctions externes */


/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*	cdif_cb : Callback de la tache TCGCD effectuant la diffusion.
*	Pour chaque occurence diffusable contenue dans le BL, on verifie
* que l on peut effectuer la diffusion si c est OK, on l effectue vers la 
* machine cible sinon on informe l administrateur du probleme rencontre
* via la xzia08.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cdif_cb(		T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG )

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback RTServer.
* ARGUMENTS EN SORTIE :
*   Aucun
* CODE RETOUR : 
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $ : cdif_cb";
	int 	vl_retcode	= XDC_OK;
	char 	vl_mesgerr[80]	= "";
	char 	vl_Msg[500]	= "";
	int 	vl_Nb_ligne_BL	= 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cdif_cb est declenche");
	
	
	/*A
	** On declenche la lecture du BL avec la fonction utilisateur qui
	** pour chaque occurence du BL va effectuer la diffusion si 
	** le tableau de checklist devant le type de fichier autorise a le faire.
	*/
	if((vl_retcode=xzcg02_Lecture_BL ( cdif_FctUtil_Gest_ligne_bl  , &vl_Nb_ligne_BL))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzcg02_Lecture_BL a retourne le code d erreur : %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INFO,"La diffusion a échoué");
		/*B
		** En cas de prob d execution de la fonction utilisateur, on previent 
		** l administrateur.
		*/
		sprintf(vl_Msg,"Erreur de traitement sur le fichier BL %s pour la diffusion",XDF_BON_LIVRAISON);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_cb : Sortie en erreur.");
		return;
	}


	sprintf(vl_Msg," " );
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	sprintf(vl_Msg,"Diffusion Terminée.");
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	sprintf(vl_Msg," " );
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cdif_cb : Sortie");
	return;
	
}  
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*	cdif_FctUtil_Gest_ligne_bl permet de traiter 
*  une ligne du Bon de diffusion conformement a la fonctionalite
*  de diffusion des fichiers par tcgcd.x (page35 du DCG-002/0.1).
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cdif_FctUtil_Gest_ligne_bl(XZCGT_ComposantBL va_LigneBL,int *pa_retcode_out )

/*
* ARGUMENTS EN ENTREE :
*   	va_LigneBL
* ARGUMENTS EN SORTIE :
*   	pa_retcode_out : va contenir le status du traitement de l enregistrement 
* 	du BL en cours de traitement.
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*	Les types de fichiers diffusables sont :
*	XZCGC_TYPEF_SCNT
*	XZCGC_TYPEF_SNAV
*	XZCGC_TYPEF_LPIC
*	XZCGC_TYPEF_MPMV
*	XZCGC_TYPEF_EQAL
*	XZCGC_TYPEF_SYNP
*	XZCGC_TYPEF_RSAD
*	XZCGC_TYPEF_FAID
*       XZCGC_TYPEF_FINF
*	XZCGC_TYPEF_PIGN
*	XZCGC_TYPEF_DIE
*       XZCGC_TYPEF_MOD
*	XZCGC_TYPEF_MFBO
*	XZCGC_TYPEF_EXEC_BASEDD 
*	XZCGC_TYPEF_EXEC_EQUEXT 
*	XZCGC_TYPEF_EXEC_GECONF 
*	XZCGC_TYPEF_EXEC_GESIHM 
*	XZCGC_TYPEF_EXEC_MAINTE 
*	XZCGC_TYPEF_EXEC_SAIDEC 
*	XZCGC_TYPEF_EXEC_SIMTST 
*	XZCGC_TYPEF_EXEC_SUPERV 
*	XZCGC_TYPEF_EXEC_XDMICG (si il y a lieu)
*	XZCGC_TYPEF_GLOBAL
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $ : cdif_FctUtil_Gest_ligne_bl";
	int 	vl_retcode	= XDC_OK;
	char 	vl_mesgerr[80]	= "";
	
	int	vl_numerr	= 0;
	char 	vl_Msg[1000]	= "";
	char	vl_commande[500] = "";
	char	vl_line[500]	 = "";
	FILE	*vl_ShellD;
	XZSCT_NomSite 		vl_NomSite_MachineDest;
	XZSPT_ComposantVL       vl_Enr_VersLogi;
	XZIAT_destinataire	vl_NomMachineAprevenir;
	int			vl_NbArg ;
	void                *	pl_Arg[5];	
	
	XDY_NomMachine		vl_NomMachineDest	= "";
	XDY_NomMachine		vl_NomMachineSour	= "";
	XZSCT_NomSSysteme  	vl_NomSSysteme		= "";	
	char			vl_NomRepSource[300]	= "";
	char			vl_NomRepDest[300]	= "";
	int			vl_Status		= 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cdif_FctUtil_Gest_ligne_bl  : debut d execution");

	/*A
	**	Type de fichier a diffuser vide ? 
	*/
	if (va_LigneBL.TypeFic == NULL)
	{
		sprintf(vl_Msg,"DIFFUSION : erreur type de fichier (1 ere colonne) vide");
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
		return(XDC_NOK);
	}
	/*A
	**  Comparaison de va_LigneBL.TypeFic avec les types connus.
	*/
	/*A
	** Diffusion des fichiers des seuils de niveau de trafic.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SCNT))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_SCNT_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_SCNT_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_SCNT_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers des scenarios NAV.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SNAV))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_SNAV_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_SNAV_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_SNAV_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");
		}	
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		 
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers de listes des Pictogrammes.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_LPIC))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_LPIC_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_LPIC_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_LPIC_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}	
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers des messages PMV.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MPMV))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_MPMV_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_MPMV_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_MPMV_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers des equations d alertes.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EQAL))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EQAL_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EQAL_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_EQAL_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers des synoptiques.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_SYNP))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_SYNP_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_SYNP_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_SYNP_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}

		strcpy(vl_NomMachineAprevenir.machine,va_LigneBL.NomMachine);
		XZIA_06AffMsgSyst(vl_NomMachineAprevenir,"De nouveaux synoptiques sont disponibles sur votre station");
		XZIA_06AffMsgSyst(vl_NomMachineAprevenir,"L arret de la tache est necessaire pour la prise en compte");
		sprintf(vl_Msg,"Le message suivant : %s a été envoye à l'utilisateur de la machine %s",
				"Message de l administrateur : De nouveaux synoptiques sont disponibles sur votre station : Arrêt de la tache est nécessaire pour la prise en compte",
				va_LigneBL.NomMachine);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");
		
		
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers des regles du sad.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_RSAD))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_RSAD_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_RSAD_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_RSAD_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		/*A
		** Si la diffusion des fichiers s est bien deroule : on effectue une
		** demande de prise en compte des nouvelles regles du SAD.
		*/
		/*B
		** Demande de prise en compte des fic BDC differant suivant le type de machine (Dev ou Serveur)
		*/
		if ( strncmp( va_LigneBL.NomMachine, CDIFC_DEBUT_NOM_SERV, CDIFC_LONG_DEBUT_NOM_SERV) == NULL )
		{
		   if((vl_retcode=XZSA_23SiteMachine(va_LigneBL.NomMachine,vl_NomSite_MachineDest))!=XDC_OK)
		   {
			sprintf(vl_Msg,"%s : XZSA_23SiteMachine : La recuperation du site pour la machine %s a échoué : necessaire a la prise en compte des nouvelles regles du SAD.",
						CDIFC_DIFFUSION,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg);
			return(XDC_NOK); 
		   }
		}
		else
		{
			strcpy(vl_NomSite_MachineDest , XZDDC_DIFF_SITE_DEV);
		}
		
		if((vl_retcode=XZDD01_PrendreEnCompteBDC(vl_NomSite_MachineDest))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : XZDD01_PrendreEnCompteBDC : la prise en compte des nouvelles regles du SAD pour la machine %s a échoué.",
						CDIFC_DIFFUSION,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg);
			return(XDC_NOK); 
		}
		else
		{
			sprintf(vl_Msg,"Prise en compte des nouvelles regles du SAD de la machine %s demandee.",va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers d aides.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FAID))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_FAID_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_FAID_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_FAID_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	/*A
	** Diffusion des fichiers d inf.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FINF))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_FINF_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_FINF_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_FINF_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	
	/*A
	** Diffusion des fichiers de synoptiques IGN.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_PIGN))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_PIGN_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_PIGN_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_PIGN_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	
	/*A
	** Diffusion des fichiers de synoptiques DIE.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DIE))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_DIE_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_DIE_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_DIE_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}

	/*A
	** Diffusion des fichiers de synoptiques MOD.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MOD))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_MOD_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_MOD_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_MOD_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}

	/*A
	** Diffusion des fichiers de synoptiques OPER.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_OPER))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_OPER_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_OPER_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_OPER_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}

	/*A
	** Diffusion des fichiers de synoptiques UTIL.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_UTIL))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_UTIL_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_UTIL_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_UTIL_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}

	/*A
	** Diffusion des fichiers de synoptiques NTFM.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_NTFM))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_NTFM_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_NTFM_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_NTFM_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}


	/*A
	** Diffusion des fichiers de STRA.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_STRA))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_STRA_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_STRA_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_STRA_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}

	/*A
	** Diffusion des fichiers de macros boites a outils.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_MFBO))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_MFBO_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_MFBO_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_MFBO_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	
	/*A
	** Diffusion des fichiers par defaut
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_DEFT))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_DEFT_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_DEFT_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_DEFT_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
	}
	
	/*A
	** Diffusion des fichiers d administration
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_FADM))
	{
		if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_FADM_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_FADM_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : %s de type %s a été livré dans %s de la machine %s",
				CDIFC_DIFFUSION,
				va_LigneBL.NomFichier,
                                va_LigneBL.TypeFic,
				CDIFC_DIFF_FADM_OPERA,
				va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,"  ");	
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		/*A
		** Demande de prise en compte par le Noyau de Supervison des nouveaux fichiers d administration
		*/
		if((vl_retcode = XZSP_05MajFichierAdm(va_LigneBL.NomMachine))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Prise en compte du fichier %s d administration aprés diffusion sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Prise en compte des nouveaux fichiers d administrations aprés diffusion sur %s a réussi",
					CDIFC_DIFFUSION,
					va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
		}
	}
		
	/*A
	** Diffusion des executables du sous-système BASEDD
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_BASEDD))
	{
		/*A
		** Avant de diffuser un executable : il faut l arreter.
		** On arrete le sous systeme si la diffusion est globale,
		** sinon on n arrete que la tache.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_BASEDD,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				return(XDC_NOK);
			}
			
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_BASEDD_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_BASEDD_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_BASEDD,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_BASEDD,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_BASEDD,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_BASEDD,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_BASEDD,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret et diffusion d une seule tache */
			/* Arrêt de la tache*/
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			/* Diffusion de la tache*/
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_BASEDD_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_BASEDD_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_BASEDD);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion d executable(s) : il faut le(s) redemarrer.
		*/		
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			
			if((vl_retcode = cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
									XDC_BASEDD,
									XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Relancement d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
		}
	}
	/*A
	** Diffusion des executables du sous-système EQUEXT.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_EQUEXT))
	{
		
		/*A
		** Avant de diffuser un executable ou d un sous-système : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_EQUEXT,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_EQUEXT_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_EQUEXT_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_EQUEXT,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_EQUEXT,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_EQUEXT,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_EQUEXT,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_EQUEXT,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_EQUEXT_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_EQUEXT_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_EQUEXT);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_EQUEXT,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Relancement d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion d un ou des executables du sous-système GECONF.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_GECONF))
	{
		
		/*A
		** Avant de diffuser un executable : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			sprintf(vl_Msg,"Demande d arret des taches du sous-système %s de la machine %s",
						XDC_GECONF,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			 
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_GECONF,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_GECONF_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_GECONF_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_GECONF,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_GECONF,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_GECONF,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_GECONF,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_GECONF,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
		
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_GECONF_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_GECONF_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_GECONF);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_GECONF,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion des executables du sous-système GESIHM.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_GESIHM))
	{
		/*A
		** Avant de diffuser d un executable ou du sous-système : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
		
			/*! Livraison des fichiers de macros Applix*/
			sprintf(vl_commande,"%s/cgcd_ApDif.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_GECONF_SOURCE, 
		 			va_LigneBL.NomMachine,  
		 			CDIFC_DIFF_EXEC_GECONF_OPERA);
		 			
			sprintf(vl_Msg,"Demande de %s des macros applix contenu dans %s sur %s de la machine %s",
					"DIFFUSION",
					CDIFC_DIFF_EXEC_GECONF_SOURCE,
					CDIFC_DIFF_EXEC_GECONF_OPERA,
					va_LigneBL.NomMachine);
					
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			vl_Status = XDC_OK;
			if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
			{
				while (fgets(vl_line,499,vl_ShellD)!=NULL)
				{
					/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
					if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
					vl_Status = XDC_OK;
					else
					{
						if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
						sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
						XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
						vl_Status = XDC_NOK;
					}
				}
				if (vl_Status == XDC_NOK)
				{
					sprintf(vl_Msg,"%s : Toutes les macros Applix du rep %s n ont pas été livrées sur %s : une erreur a ete detectee",
						"DIFFUSION",
						CDIFC_DIFF_EXEC_GECONF_SOURCE,
						va_LigneBL.NomMachine);
						
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					sprintf(vl_Msg,"%s : Verifiez les droits des fichiers ou du repertoire %s de %s",
						"DIFFUSION",
						CDIFC_DIFF_EXEC_GECONF_OPERA,
						va_LigneBL.NomMachine);
				}
				else
				{
					sprintf(vl_Msg,"%s : Toutes les macros Applix du rep %s ont été livrées sur %s .",
						"DIFFUSION",
						CDIFC_DIFF_EXEC_GECONF_SOURCE,
						va_LigneBL.NomMachine);
				}
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			}
			else
			{
				sprintf(vl_Msg,"%s : les macros Applix du rep %s n ont pas été livrées sur %s : l'éxécution de la commande %s a échoué",
					"DIFFUSION",
					CDIFC_DIFF_EXEC_GECONF_SOURCE,
					va_LigneBL.NomMachine,
					vl_commande);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier : Probleme d execution de la commande %s",vl_commande);	
			}
			pclose(vl_ShellD);
			
			
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_GESIHM,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_GESIHM_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_GESIHM_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_GESIHM,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_GESIHM,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_GESIHM,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_GESIHM,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_GESIHM,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_GESIHM_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_GESIHM_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_GESIHM);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion de executable ou d un sous-système: il faut le redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_GESIHM,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion des executables du sous-système MAINTE.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_MAINTE))
	{
		/*A
		** Avant de diffuser d un executable ou du sous-système : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,	
							XDC_MAINTE,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_MAINTE_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_MAINTE_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_MAINTE,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_MAINTE,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_MAINTE,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_MAINTE,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_MAINTE,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret et diffusion d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_MAINTE_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_MAINTE_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_MAINTE);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,	
							XDC_MAINTE,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion des executables du sous-système SAIDEC.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SAIDEC))
	{
		
		/*A
		** Avant de diffuser un executable : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_SAIDEC,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_SAIDEC_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_SAIDEC_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_SAIDEC,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_SAIDEC,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_SAIDEC,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_SAIDEC,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_SAIDEC,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_SAIDEC_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_SAIDEC_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s  de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_SAIDEC);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_SAIDEC,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion des executables du sous-système SIMTST.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SIMTST))
	{
		
		/*A
		** Avant de diffuser un executable : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_SIMTST,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			 
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_SIMTST_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_SIMTST_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_SIMTST,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_SIMTST,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_SIMTST,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_SIMTST,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_SIMTST,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_SIMTST_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_SIMTST_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
						XDC_SIMTST,
						XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion des executables du sous-système SUPERV.
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_SUPERV))
	{
		
		/*A
		** Avant de diffuser un executable : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
						XDC_SUPERV,
						XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			 
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_SUPERV_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_SUPERV_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_SUPERV,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_SUPERV,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_SUPERV,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_SUPERV,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_SUPERV,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_SUPERV_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_SUPERV_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_SUPERV,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion des executables de XDMICG (au cas ou).
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_EXEC_XDMICG))
	{
		
		/*A
		** Avant de diffuser un executable : il faut l arreter
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_XDMICG,
							XZSPC_ARRET))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
			strcpy(vl_NomRepSource,CDIFC_DIFF_EXEC_XDMICG_SOURCE);
			strcpy(vl_NomRepDest,CDIFC_DIFF_EXEC_XDMICG_OPERA);
			strcpy(vl_NomMachineSour,XDC_MACHINE_GECONF);
			strcpy(vl_NomMachineDest,va_LigneBL.NomMachine);
			pl_Arg[0] = (void *)vl_NomRepSource;
			pl_Arg[1] = (void *)vl_NomMachineSour;
			pl_Arg[2] = (void *)vl_NomRepDest;
			pl_Arg[3] = (void *)vl_NomMachineDest;
			pl_Arg[4] = NULL;
			/*A
			** On va effectuer la diffusion des taches definies
			** dans les fichiers d administrations pour ce sous-système 
			** sur la machine definie
			*/
			if((vl_retcode=XZSA_04ListeTacheMachineSSyst(	va_LigneBL.NomMachine,
									XDC_XDMICG,
									(void (*)())cdif_Diff_Taches,
									&vl_NbArg,
									(void **)pl_Arg))!=XDC_OK)
			{
				switch (vl_retcode)
				{
					case XZSAC_NOM_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Machine inconnue",
							XDC_XDMICG,va_LigneBL.NomMachine);
						break;
					case XZSAC_TYP_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Type de la Machine inconnu",
							XDC_XDMICG,va_LigneBL.NomMachine);
						break;
					case XZSAC_NOS_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Nom du sous-système inconnu",
							XDC_XDMICG,va_LigneBL.NomMachine);
						break;
					case XZSAC_INI_INV:
						XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Diffusion du sous-système %s vers %s a échoué : Erreur d initialisation administration",
							XDC_XDMICG,va_LigneBL.NomMachine);
						break;
					default:
						break;
				}	
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);			
			}
		}
		else
		{/* Arret d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_ARRET))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Arrêt de %s avant diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		
			if((vl_retcode=cliv_Liv_Diff_Fichier(	CDIFC_DIFFUSION,
					va_LigneBL.TypeFic,
					va_LigneBL.NomFichier,
					CDIFC_DIFF_EXEC_XDMICG_SOURCE,
					XDC_MACHINE_GECONF,
					CDIFC_DIFF_EXEC_XDMICG_OPERA,
					va_LigneBL.NomMachine))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué ");
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
				return(XDC_NOK);
			}
		}
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NomMachine,va_LigneBL.NomMachine);
		strcpy(vl_Enr_VersLogi.NomFichier,va_LigneBL.NomFichier);
		strcpy(vl_Enr_VersLogi.TypeFic,va_LigneBL.TypeFic);
		strcpy(vl_Enr_VersLogi.NumVersion,va_LigneBL.NumVersion);
		XZSM_11ConversionHorodate(va_LigneBL.Horodate,&(vl_Enr_VersLogi.Horodate));
		/*A
		** Mise à jour des versions logicielles aprés une diffusion
		*/
		if((vl_retcode=XZSP_09EcritVersionLog(vl_Enr_VersLogi))!=XDC_OK)
		{
			sprintf(vl_Msg,"%s : Mise à jour aprés diffusion des versions logicielles pour le fichier %s de type %s sur la machine %s a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier,
						va_LigneBL.TypeFic,
						va_LigneBL.NomMachine);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING,"%s",vl_Msg); 
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_FctUtil_Gest_ligne_bl : Sortie en erreur.");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"%s : Mise à jour de la version logiciel du sous-système %s aprés diffusion : OK",
						CDIFC_DIFFUSION,
						XDC_XDMICG);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Apres la diffusion de  executable : il faut la redemarrer.
		*/
		if (!strcmp(va_LigneBL.NomFichier,XZCGC_NOMF_GLOBAL))
		{
			if((vl_retcode=cdif_Arret_Demarrage_SousSyst (va_LigneBL.NomMachine,
							XDC_XDMICG,
							XZSPC_MARCHE))!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué");
				return(XDC_NOK);
			}
		}
		else
		{/* Demarrage d une seule tache */
			if((vl_retcode=XZSP_03CmdTache( va_LigneBL.NomMachine,
						va_LigneBL.NomFichier,
						XZSPC_MARCHE))!=XDC_OK)
			{
				sprintf(vl_Msg,"%s : Relancement de %s aprés diffusion a échoué",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
				return(XDC_NOK);
			}
			else
			{
				sprintf(vl_Msg,"%s : Redemarrage de %s aprés diffusion a réussi",
						CDIFC_DIFFUSION,
						va_LigneBL.NomFichier);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_INFO,"cliv_Liv_Diff_Fichier a réussi : %s",vl_Msg);
			}
		}
	}
	/*A
	** Diffusion globale des repertoires de l application migrazur
	*/
	if(!strcmp(va_LigneBL.TypeFic,XZCGC_TYPEF_GLOBAL))
	{
		/*A
		** Diffusion du repertoire des SCNT
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_SCNT_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_SCNT_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur système : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}	

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		
		/*A
		** Diffusion du repertoire des SNAV
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_SNAV_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_SNAV_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur système : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des LPIC
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_LPIC_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_LPIC_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur système : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		
		/*A
		** Diffusion du repertoire des MPMV
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_MPMV_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_MPMV_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		
		/*A
		** Diffusion du repertoire des EQAL
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EQAL_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EQAL_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des SYNP
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_SYNP_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_SYNP_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des RSAD
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_RSAD_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_RSAD_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des FAID
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_FAID_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_FAID_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
	
	       /*A
	       ** Diffusion du repertoire des FINF
	       */
	       sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
				       XDC_NSUP_PATHEXEC,
				       CDIFC_DIFF_FINF_SOURCE,
				       XDC_MACHINE_GECONF,
				       CDIFC_DIFF_FINF_OPERA);
                if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n'))
                                vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0';
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
                                }
                        }
                }
                else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
                }


		/*A
		** Diffusion du repertoire des PIGN
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_PIGN_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_PIGN_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des MFBO
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_MFBO_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_MFBO_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des fichiers par defaut
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_DEFT_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_DEFT_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		
		
		/*A
		** Diffusion du repertoire des EXEC_BASEDD
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_BASEDD_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_BASEDD_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}

		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
		}
		/*A
		** Diffusion du repertoire des EXEC_EQUEXT
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_EQUEXT_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_EQUEXT_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des EXEC_GECONF
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_GECONF_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_GECONF_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des EXEC_GESIHM
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_GESIHM_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_GESIHM_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des EXEC_MAINTE
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_MAINTE_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_MAINTE_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des EXEC_SAIDEC
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_SAIDEC_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_SAIDEC_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
	
		/*A
		** Diffusion du repertoire des EXEC_SIMTST
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_SIMTST_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_SIMTST_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des EXEC_SUPERV
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_SUPERV_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_SUPERV_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
		/*A
		** Diffusion du repertoire des EXEC_XDMICG (si il y a lieu).
		*/
		sprintf(vl_commande,"%s/cgcd_mtrf.sh %s %s %s",
					XDC_NSUP_PATHEXEC,
		 			CDIFC_DIFF_EXEC_XDMICG_SOURCE, 
		 			XDC_MACHINE_GECONF,  
		 			CDIFC_DIFF_EXEC_XDMICG_OPERA);
		if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
		{
			while (fgets(vl_line,499,vl_ShellD)!=NULL)
			{
				/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
				if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
				vl_Status = XDC_OK;
				else
				{
					if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
					sprintf(vl_Msg,"Erreur systeme : %s",vl_line);
					XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
					vl_Status = XDC_NOK;
				}
			}
		}
		else
		{
			sprintf(vl_Msg,"%s : la commande %s n'a pu s'éxécuter",CDIFC_DIFFUSION,vl_commande);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cdif_FctUtil_Gest_ligne_bl : Sortie ");
	return(XDC_OK);
	
} 
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*	cdif_Diff_Taches permet de transferer les taches d un sous systeme
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cdif_Diff_Taches	(	XZSCT_NomTache  va_NomTache,
        			int             va_NumOrdre,
        			void	**	pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   	va_NomTache : nom de la tache connue dans superv
*	va_NumOrdre : contient le numero d ordre de la tache dans superv
*	
*	pa_Arg doit contenir 
*	va_DirSource: contient le repertoire ou on doit copier la tache
*	va_MachSource: contient le nom de la machine a partir de la quelle on copie la tache
*	va_DirDest  : contient le nom du repertoire destinataire
*	va_MachDest : contient le nom de la machine vers la quelle on copie la tache
* ARGUMENTS EN SORTIE :
*   	
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
*   
*
* FONCTION 
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $ : cdif_Diff_Taches";
	int	vl_retcode	= XDC_OK;
	char 	vl_Msg[500]	= "";
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cdif_Diff_Taches  : debut d execution");
	if (pa_Arg== NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cdif_Diff_Taches a recu des arguments NULL : on ne peut commander le transfert");
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_Diff_Taches : Sortie en erreur.");
		return(XDC_NOK);
	}
	sprintf(vl_Msg,"Demande de %s de %s sur %s dans %s",
			CLIVC_DIFFUSION,
			va_NomTache,
			(char *) pa_Arg[3],
			(char *) pa_Arg[2]);
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	if((vl_retcode=XZSS_07TransfertFichier (pa_Arg[1],
						va_NomTache,
						pa_Arg[0],
						pa_Arg[3],
						va_NomTache,
						pa_Arg[2]))!=XDC_OK)
	{
		/*B
		** Previent l administrateur du prob survenu.
		*/
		cliv_Inform_Admin_Probl(	CLIVC_DIFFUSION,
						va_NomTache,
						pa_Arg[0],
						pa_Arg[1],
						va_NomTache,
						pa_Arg[2],
						pa_Arg[3],
						vl_retcode);
			
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cdif_Diff_Taches : Sortie en erreur.");
		return(XDC_NOK);
	}
	sprintf(vl_Msg,"%s : %s a été diffusé sur %s dans %s",
			CLIVC_DIFFUSION,
			va_NomTache,
			(char *) pa_Arg[3],
			(char *) pa_Arg[2]);
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
	
	/*A Si la tache transferee est tshor.x ou tscmd.x alors on lui met root comme proprietaire */
/*	if (  ( strcmp( va_NomTache , "tshor.x" ) == NULL )
	   || ( strcmp( va_NomTache , "tscmd.x" )  == NULL ) )
	   {
	   cdif_ChangeProp( pa_Arg[3] , XDC_PATHEXE , va_NomTache , "root" );
	   }
*/	   
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : cdif_Diff_Taches  : fin d execution");
	return(XDC_OK);
}
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*	cdif_Arret_Demarrage_SousSyst : permet l arret ou le demarrage 
* d un sous-système avant diffusion.
* Envoie a l administrateur le message constitue en fonction du retour de la
* fonction de SUPERVISION permettant l arret du sous-système.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cdif_Arret_Demarrage_SousSyst	(XZSCT_NomMachine 	va_NomMachine ,
					 XZSCT_NomSSysteme 	va_NomSSSysteme ,
					 int 			va_Action )

/*
* ARGUMENTS EN ENTREE :
*
* va_NomMachine 	contient le nom de la machine 
* va_NomSSSysteme 	contient le nom du sous-système de la machine sur lequel on execute l action
* va_Action		contient le type d action a executer sur le sous-système de la machine.
*
* ARGUMENTS EN SORTIE :
*   	
* CODE RETOUR : 
*	XDC_OK
*	XDC_NOK
* CONDITION D'UTILISATION
*   
*
* FONCTION 
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $ : cdif_Arret_Demarrage_SousSyst";
	int 	vl_retcode	= XDC_OK;
	char	vl_Msg[500]	= "";
	char 	vl_ActionChar[30]= "";
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cdif_Arret_Demarrage_SousSyst : debut d execution");
	if ( va_Action == XZSPC_ARRET )
	{
		strcpy(vl_ActionChar,"Arret");
	}
	else
	{
		if ( va_Action == XZSPC_MARCHE ) strcpy(vl_ActionChar,"Redemarrage");
		else
		{
			vl_retcode	= XDC_NOK;
			XZST_03EcritureTrace(XZSTC_WARNING," cdif_Arret_Demarrage_SousSyst a recu une action %d invalide",va_Action);	
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cdif_Arret_Demarrage_SousSyst : Sortie en erreur avec %d",vl_retcode);
			return(vl_retcode);
		}	
	}
	if((vl_retcode = XZSP_02CmdSSyst (va_NomMachine,
					  va_NomSSSysteme,
					  va_Action))!=XDC_OK)
	{
		switch (vl_retcode) 
		{
			case XZSPC_NOM_INV :
				sprintf(vl_Msg,"%s : Machine %s non valide : %s du sous-système %s avant diffusion a échoué",
				   		CDIFC_DIFFUSION,
						va_NomMachine,
						vl_ActionChar,
						va_NomSSSysteme);
				break;

			case XZSPC_NOS_INV :
				sprintf(vl_Msg,"%s : Sous-systeme %s non valide sur %s : %s du sous-système %s avant diffusion a échoué",

						CDIFC_DIFFUSION,
						va_NomSSSysteme,
						va_NomMachine,
						vl_ActionChar,
						va_NomSSSysteme);
				break;
			default:
						sprintf(vl_Msg,"%s : %s du sous-système %s avant diffusion a échoué",
						CDIFC_DIFFUSION,
						vl_ActionChar,
						va_NomSSSysteme);
		}
		/*A
		** Envoie a l administrateur le message constitue en fonction du retour de la
		** fonction de SUPERVISION permettant l arret du sous-système.
		*/
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier a échoué : %s",vl_Msg);
		vl_retcode = XDC_NOK;
	}
	else
	{
		sprintf(vl_Msg,"%s : %s du sous-système %s : OK",
				CDIFC_DIFFUSION,
				vl_ActionChar,
				va_NomSSSysteme);
				
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		
		/*B Si on arret le sous systeme EQUEXT alors on attend 30 sec pour etre sur que tout est arrete sino 5 sec */
		if ( va_Action == XZSPC_ARRET )   
		   {
		   if (strcmp( va_NomSSSysteme , XDC_EQUEXT ) == NULL)
		   	sleep(30);
		   else
		   	sleep(20);
		   sprintf(vl_Msg," ...");
				
		   XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		   }
		
		vl_retcode = XDC_OK;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : cdif_Arret_SousSyst : Fin d execution : retourne %d",vl_retcode);
	return(vl_retcode);
}


/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Change le propritaire d'un fichier
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cdif_ChangeProp(	char * va_Machine_Dest,
			char * va_Rep_Dest,
			char * va_NomFichier,
			char * va_Proprietaire)
/*
* ARGUMENTS EN ENTREE : 	
* ARGUMENTS EN SORTIE :
* CODE RETOUR : 
*	XDC_OK si le type de fichier est correct 
*	sinon XDC_NOK
* CONDITION D'UTILISATION   
* FONCTION 
------------------------------------------------------*/
{
	static char *version 		= "$Id: cgcd_dif.c,v 1.17 2016/07/13 22:50:35 devgfi Exp $ : cdif_ChangeProp ";
	int	vl_retcode		= XDC_OK;
	int	vl_numerr		= 0;
	char 	vl_mesgerr[80]		= "";
	char 	vl_line	[500]		= "";
	char 	vl_commande[500]	= "";
	char 	vl_Msg[500]		= "";
	int	vl_Etat			= 0;
	FILE *	vl_ShellD		= NULL;
	int	pl_Etat			= XDC_OK;
	int	vl_Status		= XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN   : cdif_ChangeProp a recu : \n Nom du Fichier %s  rep Destination %s \nsur la machine %s \n proprietaire %s",
				va_NomFichier,
				va_Rep_Dest,
				va_Machine_Dest,
				va_Proprietaire );
	
	/*A commande de modification de proprietaire */
		
	sprintf(vl_commande,"remsh %s ""chmod 777 %s/%s; chown %s %s/%s; chmod +s %s/%s""",
		 			va_Machine_Dest,  
		 			va_Rep_Dest,
		 			va_NomFichier,
		 			va_Proprietaire,
		 			va_Rep_Dest,
		 			va_NomFichier,
		 			va_Rep_Dest,
		 			va_NomFichier);
	vl_Status = XDC_OK;
	if ((vl_ShellD = popen (vl_commande, "r")) != NULL)
	{
		while (fgets(vl_line,499,vl_ShellD)!=NULL)
		{
			/* On enleve le retour chariot pou le transformer en fin de chaine*/
				
			if ( (!strcmp(vl_line,""))||(strlen(vl_line) <=1)||(vl_line[0]=='\0')||(vl_line[0]=='\n')) 
			vl_Status = XDC_OK;
			else
			{
				if (strlen(vl_line) >0) vl_line [strlen(vl_line)-1] = '\0'; 
				sprintf(vl_Msg,"Erreur système : %s",vl_line);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				vl_Status = XDC_NOK;
			}
		}
			
	}
	else
	{
		sprintf(vl_Msg,"%s : le changement de propritaire pour  %s n'a pas éffectué sur %s : l'éxécution de la commande %s a échoué",
			CLIVC_DIFFUSION,
			va_NomFichier,
			va_Machine_Dest,
			vl_commande);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE," ");
		XZST_03EcritureTrace(XZSTC_WARNING,"cdif_ChangeProp : Probleme d execution de la commande %s",vl_commande);	
	}
	pclose(vl_ShellD);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cdif_ChangeProp sortie avec %d",vl_Status );
	return(vl_Status);	
}
