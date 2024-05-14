/*E*/
/*Fichier : $Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2021/07/19 16:05:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MUTI * FICHIER cuti_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de TCGCD contenant les utilitaires de la tache.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	18 Jan 1995	: Creation
* Niepceron	01 Mar 1996	: Correction accents v1.2
* Niepceron	18 Avr 1996	: Suppression du message Changement de Base Courante demandé de CFT a CFT v1.3 (966)
* Niepceron	07 Jun 1996	: Correction pour éviter le message Changement de Base Courante demandé de CFG à CFG v1.4 (966)
* JPL		13/07/16	: Suppression de warnings de compilation mineurs  1.5
* JMG		19/07/21	: EMS 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_liv.h"

#include "cgcd_util.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_util" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cuti_Recup_Horodate : Fonction permettant de recuperer l horodate courante
*  sous forme de chaine : utilisee pour initialiser
*  les Callbacks de la tache TCGCD.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cuti_Recup_Horodate(XZSMT_Horodate pa_Horodate)
/*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*	pa_Horodate	recoit l horodate en caractere.
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	on retourne XDC_NOK apres avoir trace l erreur.
*
* CONDITION D'UTILISATION
*   Connexion au RTserver doit etre initialisee.
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_Recup_Horodate" ;
	int vl_ValRet 		= XDC_OK;
	
	int			vl_JourSemaine;	/* variable necessaire pour lire l horodate
						** mais qui n est pas exploitee
						*/
						
	double			vl_NbSeconde;	/* va contenir l horodate renvoyee par SUPERV*/
	XZSMT_Horodate		pl_Horodate;	/* va contenir l horodate en caractere*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cuti_Recup_Horodate est appelee");
	/*A
	** Lecture de l Horodate
	*/
	if ((vl_ValRet =XZSM_07LireHorodate(CUTIC_TYPE_SYSTEM,&vl_JourSemaine,&vl_NbSeconde))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : XZSM_07LireHorodate a echoue");
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : FIN de TCGCD");
		return(XDC_NOK);
	}
	/*A
	** Conversion en caractere de l Horodate
	*/
	if ((vl_ValRet =XZSM_11ConversionHorodate(vl_NbSeconde,&pl_Horodate))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : XZSM_11ConversionHorodate a echoue");
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : FIN de TCGCD");
		return(XDC_NOK);
	}
	
	strcpy(pa_Horodate,pl_Horodate);
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cuti_Recup_Horodate sortie");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* cuti_BilanConf_Trace  permet d ecrire dans le fichier bilan config le message passe en argument
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cuti_BilanConf_Trace(char * pa_Msg	) 
/*
* ARGUMENTS EN ENTREE :  pa_Msg contient le message a hostoriser dans le fichier de bilan config.	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_BilanConf_Trace";
	char pl_MsgAdm[255];	/*contient un message pour l administrateur en cas de prob sur le fichier bilan config*/
	
	if (!fprintf(CGCDG_Fich_BilanConfig,"%s\n",pa_Msg))
      	{
      		/*feof(CGCDG_Fich_BilanConfig);*/
      		
      		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pa_Msg); /* On previent l IHM de l administrateur */
      		sprintf(pl_MsgAdm,"Erreur d'écriture dans le fichier Bilan Config du message %s",   "" );
      		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_MsgAdm); /* On previent l IHM de l administrateur */
         	XZST_03EcritureTrace(XZSTC_FATAL,"TCGCD : erreur écriture dans le fichier Bilan Config du msg %s",pa_Msg);
      		return(XDC_NOK);
      	}
      	else
      	{
         	fflush(CGCDG_Fich_BilanConfig);
      	}
      	return(XDC_OK);

}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cuti_Chgmt_Base_Courante permet de changer de base courante.
*  Pour le passage de la Base courante a CFT, il n y a pas de verification.
*  Pour passer la Base courante a CFG, il faut que le tableau de checklist soit
*  Valider : pour tous les types de Verif, le flag doit etre positionne a XDC_OK.
*  On envoie un message a l IHM de l administrateur pour l informer de toute demande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cuti_Chgmt_Base_Courante(char * pa_NomBase) 
/*
* ARGUMENTS EN ENTREE :  pa_NomBase contient le nom de la base 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si le changement a ete autorise
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
	
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_Chgmt_Base_Courante";
	char 	pl_MsgAdm[255]	= "";	
	int 	vl_retcode	= XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cuti_Chgmt_Base_Courante a recu une demande pour passer a %s",pa_NomBase);
	
	if ( !strcmp(pa_NomBase,XDC_BASE_CFT))
	{
		if ( strcmp(CGCDG_Base_Courante,XDC_BASE_CFT) )
		   sprintf(pl_MsgAdm,"TCGCD : Changement de Base Courante demandé de %s à %s : OK",CGCDG_Base_Courante,XDC_BASE_CFT);
		else
		   sprintf(pl_MsgAdm,"");
		strcpy(CGCDG_Base_Courante,XDC_BASE_CFT);
		vl_retcode = XDC_OK;
		XZST_03EcritureTrace(XZSTC_INFO,"cuti_Chgmt_Base_Courante : Base courante = %s",CGCDG_Base_Courante);
  	}    	
  	else
  	{
  		if ( !strcmp(pa_NomBase,XDC_BASE_CFG))
  		{
  			/*!
  			** Verification de l etat des Verif pour permettre de passer dans CFG.
  			*/
  			if((cliv_ConsultGlobale_Tab_Checklist() == XDC_OK)||(CGCDV_CHECKLIST == XDC_NOK))
  			{
				if ( strcmp(CGCDG_Base_Courante,XDC_BASE_CFG) )
  					sprintf(pl_MsgAdm,"TCGCD : Changement de Base Courante demandé de %s a %s : OK ",
 						CGCDG_Base_Courante,XDC_BASE_CFG);
				else
					sprintf(pl_MsgAdm,"");
  				strcpy(CGCDG_Base_Courante,XDC_BASE_CFG);
 				XZST_03EcritureTrace(XZSTC_INFO,"cuti_Chgmt_Base_Courante : Base courante = %s",CGCDG_Base_Courante);
  				vl_retcode = XDC_OK;
  			}
  			else
  			{
  				sprintf(pl_MsgAdm,"TCGCD : Changement de Base Courante demandé de %s a %s : Refusé ",
 						CGCDG_Base_Courante,pa_NomBase);
 				strcpy(CGCDG_Base_Courante,XDC_BASE_CFT);
 				XZST_03EcritureTrace(XZSTC_INFO,"cuti_Chgmt_Base_Courante : Base courante = %s",CGCDG_Base_Courante);
  				vl_retcode = XDC_NOK;
  			}
  		} 
  		else
  		{
  			/*A
  			** Si on demande de passer a une base inconnue (autre que CFG ou CFT)
  			** on sort en FATAL.
  			*/
  			XZST_03EcritureTrace(XZSTC_INFO,"cuti_Chgmt_Base_Courante : Base courante = %s",CGCDG_Base_Courante);
  			sprintf(pl_MsgAdm,"Changement de Base Courante demandé de %s a %s impossible : %s n'est pas une base reconnue par le système",
 				CGCDG_Base_Courante,pa_NomBase,pa_NomBase);
 				
 			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_MsgAdm); /* On previent l IHM de l administrateur */	
  			
  			XZST_03EcritureTrace(XZSTC_FATAL,"cuti_Chgmt_Base_Courante a recu une demande de changement de base de %s vers %s qui est inconnue",
  					CGCDG_Base_Courante,pa_NomBase);		
  			vl_retcode = XDC_NOK;
  		}	
      	}
      	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_MsgAdm); /* On previent l IHM de l administrateur */
      	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cuti_Chgmt_Base_Courante");
	return(vl_retcode);
	
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cuti_CanalComInit_cb est declenchee sur reception du message RTWks XDM_CGD_CANAL.
*  Permet de positionne la Globale CGCDV_NOMMACHINE vers la quelle on envoie les messgae 
*  d administration (via xzia08).
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cuti_CanalComInit_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :   aucun
* CODE RETOUR :  aucun
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_CGD_CANAL.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_CanalComInit_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int			vl_retcode    = XDC_OK;		/* code de retour 	*/
#ifndef _TIBCO_EMS
	char         	*	vl_NomMachine = "";		/* type RTwks : str	*/
#else
	XDY_NomMachine     	vl_NomMachine = "";		/* type RTwks : str	*/
#endif	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cuti_CanalComInit_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CGD_CANAL
	*/ 
	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_STR, 
			&vl_NomMachine, 
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cuti_CanalComInit_cb a recu NomMachine = %s",vl_NomMachine);	
	}
	else
	{
		/*B 
		** SI Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cuti_CanalComInit_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cuti_CanalComInit_cb Sortie");
		return;	 
	}
	strcpy(CGCDV_NOMMACHINE.machine,vl_NomMachine);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cuti_CanalComInit_cb a positionne le canal Applix de communication (XZIA08) a %s",CGCDV_NOMMACHINE.machine);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cuti_CanalComInit_cb : Fin d execution");
	return;			
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cuti_InitChecklist_cb est declenchee sur reception du message RTWks XDM_CGD_CHECK.
*  Permet de positionne la Globale CGCDV_CHECKLIST permettant d activer 
*  toutes les verifications checklist de TCGCD.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cuti_InitChecklist_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :   aucun
* CODE RETOUR :  aucun
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_CGD_CHECK.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_InitChecklist_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int			vl_retcode    = XDC_OK;		/* code de retour 	*/
	int			vl_Activation = XDC_NOK;	/* type RTwks : int4	*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cuti_InitChecklist_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CGD_CHECK
	*/ 
	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_INT4, 
			&vl_Activation, 
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cuti_InitChecklist_cb a recu Activation = %d",vl_Activation);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cuti_InitChecklist_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cuti_InitChecklist_cb Sortie");
		return;	 
	}
	if ((vl_Activation != XDC_NOK)&&(vl_Activation != XDC_OK))
	{
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : cuti_InitChecklist_cb a recue une valeur d activation de checklist incorrecte : Echec");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cuti_InitChecklist_cb Sortie en erreur");
		return;
	
	}
	CGCDV_CHECKLIST = vl_Activation;
	XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cuti_InitChecklist_cb a positionne la Verification de Checklist a %d",CGCDV_CHECKLIST);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cuti_InitChecklist_cb : Fin d execution");
	return;			
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cuti_ChgmtBaseCour_cb permet de changer de base courante.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cuti_ChgmtBaseCour_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :   aucun
* CODE RETOUR :  aucun
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_CGD_CHECK.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_util.c,v 1.6 2021/07/19 16:05:08 pc2dpdy Exp $ : cuti_ChgmtBaseCour_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int			vl_retcode    = XDC_OK;		/* code de retour 	*/
#ifndef _TIBCO_EMS
	char			*vl_BaseCourante="";
#else
	XDY_Nom			vl_BaseCourante="";
#endif	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cuti_ChgmtBaseCour_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CGD_CHGB
	*/ 
	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_STR, 
			&vl_BaseCourante, 
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cuti_ChgmtBaseCour_cb a recu la nom de base suivant :%s ",vl_BaseCourante);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cuti_ChgmtBaseCour_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cuti_ChgmtBaseCour_cb Sortie");
		return;	 
	}
	if ((vl_retcode = cuti_Chgmt_Base_Courante(vl_BaseCourante)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cuti_ChgmtBaseCour_cb : cuti_Chgmt_Base_Courante n a pas reussi a changer de Base");
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cuti_ChgmtBaseCour_cb : Fin d execution");
	return;			
}

