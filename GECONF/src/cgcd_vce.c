/*E*/
/*Fichier : $Id: cgcd_vce.c,v 1.10 2016/07/28 15:56:00 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2016/07/28 15:56:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MVCE * FICHIER cgcd_vce.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Module conforme a la DCG-002/0.1.
*
*	MVCE contient les callbacks cg_verification_cb et cg_majbd_cb.
*	La v�rification 
*
*********************
*
*	cvce_VerifModif_cb recoit le message RTWks XDM_CGD_VRF, recupere les informations.
*  Selon le type de verification, le callback aiguille 
*  dans les differentes fonctions de v�rification de coh�rence.
*
*********************
*
*	cvce_majbd_cb est declenchee sur reception du message RTWks XDM_CGD_DBD.
*	Ce CallBack va effectuer une sauvegarde de la Base CFG sur bande,
* previent l'administrateur apr�s la sauvegarde de reinserer la bande afin de recharger 
* la base dans CFT. Ceci doit etre realise avant d'effectuer une v�rification de coh�rence
* des fichiers de configuration de la base.
* 
*
*********************
*
*	cvce_Prerequis_ActionSurBase : Fonction qui permet de verifier
*  Si quand on effectue une v�rification sur la base (CFT) on a bien realise
*  la sauvegarde CFG-restauration CFG dans CFT ou bien si on veut realiser
*  une modification de la base CFG il faut que tous les flags de la Checklist
*  soit vierge.
*********************

------------------------------------------------------
* HISTORIQUE :
*
* MERCIER O.	13 Oct 1994	: Creation
**********************
* MERCIER O.	21 Oct 1994	: V 1.1
*				Mise a jour des entetes,
*				Archivage de toutes demandes 
*				dans le journal d administration.
**********************
* MERCIER O.	21 Oct 1994	: V 1.2
*				Refonte majeur de cvce_VerifModif_cb
**********************
* MERCIER O.	22 Dec 1994	: V 1.3
*				Creation de cvce_majbd_cb
*		26 Dec 1994	Ajout de cvce_Prerequis_ActionSurBase
**********************
* MERCIER O.  	29 Dec 1994	: V 1.4
* ajout de la v�rification Cheklist du rep /deq afin d avoir la permission 
* d'effectuer un changement de base de cft vers cfg.
**********************
* MERCIER O.	14 Jan 1995	: V 1.5
* Modification de l ordre de v�rification du SAD/BASE
* NIEPCERON	01 Mar 1996	: Correction des accents v1.6
* NIEPCERON     16 Avr 1996	: Ajout des msg du bilan � l'affichage de la fenetre syst�me 
				  suppression Repositionnement de la Base courante � CFT 	v1.8
* JPL		24/05/2016	: Nom du peripherique de sauvegarde en donnees globales (DEM 1165)  1.9
* JPL		27/07/2016	: Separation des traitements Reseau et Equipements  1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_dir.h"
#include "cgcd_vce.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_vce.c,v 1.10 2016/07/28 15:56:00 devgfi Exp $ : cgcd_vce" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cvce_VerifModif_cb recoit le message RTWks XDM_CGD_VRF, recupere les informations.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cvce_VerifModif_cb (	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_CGD_VRF.
*	On recoit le type de v�rification a effectuer.
*	Le message RTwks doit contenir un XDY_TypeFich,XDY_NomMachine.
*	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_vce.c,v 1.10 2016/07/28 15:56:00 devgfi Exp $ : cvce_VerifModif_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int 		vl_TypeVerif;		/* type RTwks : int4	*/
	int		vl_retcode;		/* code de retour 	*/
	char 		vl_Msg[500];		/* Buffer de msg destine a l'administrateur*/
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cvce_VerifModif_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CGD_VRF
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_TypeVerif, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cvce_VerifModif_cb a recu TypeVerif = %d",vl_TypeVerif);	
	}
	else
	{
		/*B 
		** SI Erreur detectee : Trace l erreur 
		*/
		XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb Sortie");
		return;	 
	}
	/*A
	** Lancement de la v�rification selon le type demande (recu par le message RTwks dans ce CB).
	*/
	switch (vl_TypeVerif)
	{
		case CVCEC_VERIF_DEFT:
			/*B
			** SI Verif DEFT :  v�rification des fichiers par defaut a effectuer.
			*/
			sprintf(vl_Msg,"V�rification DEFT demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_Verif_DEFT ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Verif_DEFT () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification DEFT : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			break;
			
			case CVCEC_VERIF_SCNT:
			/*B
			** SI Verif SCNT : v�rification des fichiers Seuil Niveau Trafic a effectuer
			*/
			sprintf(vl_Msg,"V�rification des fichiers de seuil niveau trafic demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_Verif_SCNT ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Verif_SCNT () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			
			sprintf(vl_Msg,"V�rification des fichiers de seuil niveau trafic : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*A V�rification des equations d alerte historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_SCNT,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_SCNT,XDC_NOK); 
			}
			
			case CVCEC_VERIF_SNAV:
			/*A Pas de v�rification des scenarios NAV : OK systematique */
			/*A V�rification des scenari NAV historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_SNAV,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_SNAV,XDC_NOK); 
			}
			break;
			
			case CVCEC_VERIF_EQAL:
			/*B
			** SI Verif EQAL : v�rification des fichiers des equations d alertes a effectuer
			*/
			sprintf(vl_Msg,"V�rification des fichiers des �quations d'alertes demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Verif_EQAL ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Verif_EQAL () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers des �quations d'alertes : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*A V�rification des equations d alerte historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_EQAL,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_EQAL,XDC_NOK); 
			}
			
			break;
			
			case CVCEC_VERIF_RSAD:
			/*B
			** SI Verif RSAD : v�rification des fichiers de types d astreintes a effectuer
			*/
			sprintf(vl_Msg,"V�rification des fichiers de types d'astreintes demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_ModifSGBD_RSAD ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_ModifSGBD_RSAD () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de types d'astreintes : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*A V�rification du SADT/Base courante historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_RSAD,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_RSAD,XDC_NOK); 
			}
			break;
			
			/*A
			** Les verifications de la base de donnees sont classees par famille et sont interdependantes:
			** Si on demande de verifier le reseau alors il faut verifier les machines....
			** d ou absence de break a la fin des "case" suivants.
			*/
			case CVCEC_VERIF_SGBD:
			/*B
			** SI Verif SGBD : v�rification  de tous les fichiers de config de la base a effectuer.
			*/
			
			sprintf(vl_Msg,"V�rification des fichiers de SGBD r�seaux demand�e");	
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			case CVCEC_VERIF_RESE:
			/*B
			** SI Verif RESE : V�rification du reseau demand�e.
			*/
			if ((vl_retcode = cvce_Prerequis_ActionSurBase())!=XDC_OK)
			{

				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes r�seaux demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_ModifSGBD_Reseau ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_ModifSGBD_Reseau () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes r�seaux : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			case CVCEC_VERIF_MACH:
			/*B
			** Si Verif_MACH : v�rification des fichiers de classes machines a effectuer
			*/
			if ((vl_retcode = cvce_Prerequis_ActionSurBase())!=XDC_OK)
			{
				cuti_BilanConf_Trace("Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes machines demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_Modif_SGBD_Machine ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Machine () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes machines : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*B
			** V�rification des fichiers de classes types 
			** et v�rification RASD a effectuer.
			*/
			if ((vl_retcode = cvce_Prerequis_ActionSurBase())!=XDC_OK)
			{
				cuti_BilanConf_Trace("Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes types demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Modif_SGBD_Type ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Type () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes types : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*A V�rification du reseau historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_DORA,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_DORA,XDC_NOK); 
			}

			break;
			
			case CVCEC_VERIF_EQUI:
			/*B
			** SI Verif EQUI : v�rification des fichiers de classes d equipements a effectuer.
			*/
			if ((vl_retcode = cvce_Prerequis_ActionSurBase())!=XDC_OK)
			{
				cuti_BilanConf_Trace("Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes d'�quipements demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_Modif_SGBD_Equipements ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Equipements () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes d'�quipements : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			case CVCEC_VERIF_SIHM:
			/*B
			** SI Verif SIHM :v�rification des fichiers de classes d IHM a effectuer.
			*/
			if ((vl_retcode = cvce_Prerequis_ActionSurBase())!=XDC_OK)
			{
				sprintf(vl_Msg,"Arr�t de la v�rification de CFT ou de la modification de CFG");
				cuti_BilanConf_Trace(vl_Msg);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Arr�t de la v�rification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification des fichiers de classes d'IHM demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Modif_SGBD_Ihm ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Ihm () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}					
			sprintf(vl_Msg,"V�rification des fichiers de classes d'IHM : OK");			
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*A V�rification des equipements historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_EQUI,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_EQUI,XDC_NOK); 
			}
			
			sprintf(vl_Msg,"V�rification de coh�rence du RSAD par rapport � la Base demand�e");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_ModifSGBD_RSAD ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_ModifSGBD_RSAD () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"V�rification de coh�rence du RSAD par rapport � la Base : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			/*A V�rification SAD/Base historisee dans la checklist*/
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_RSAD,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_RSAD,XDC_NOK); 
			}
			
			break;
			
			case CVCEC_VERIF_GLOBAL:
			/*B
			** SI Verif globale : Toutes les verifications sont a effectuer
			*/
			sprintf(vl_Msg,"*********  verification globale des fichiers demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			sprintf(vl_Msg,"Verification DEFT demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Verif_DEFT ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Verif_DEFT () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"Verification DEFT : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			sprintf(vl_Msg,"Verification des fichiers de seuil niveau trafic demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Verif_SCNT ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Verif_SCNT () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_SCNT,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_EQUI,XDC_NOK); 
			}
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_SNAV,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_SNAV,XDC_NOK); 
			}
			sprintf(vl_Msg,"Verification des fichiers de seuil niveau trafic : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			sprintf(vl_Msg,"Verification des fichiers des equations d'alertes demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Verif_EQAL ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Verif_EQAL () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_EQAL,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_EQUI,XDC_NOK); 
			}
			sprintf(vl_Msg,"Verification des fichiers des equations d'alertes : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cvce_Prerequis_ActionSurBase())!=XDC_OK)
			{
				cuti_BilanConf_Trace("Arret de la verification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Arret de la verification de CFT ou de la modification de CFG");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"Verification des fichiers de classes reseaux demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_ModifSGBD_Reseau ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_ModifSGBD_Reseau () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"Verification des fichiers de classes reseaux : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			sprintf(vl_Msg,"Verification des fichiers de classes machines demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Modif_SGBD_Machine ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Machine () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"Verification des fichiers de classes machines : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			sprintf(vl_Msg,"Verification des fichiers de classes types demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Modif_SGBD_Type ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Type () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			sprintf(vl_Msg,"Verification des fichiers de classes types : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_DORA,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_DORA,XDC_NOK); 
			}
			
			sprintf(vl_Msg,"Verification des fichiers de classes d'equipements demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_Modif_SGBD_Equipements ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Equipements () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			
			sprintf(vl_Msg,"Verification des fichiers de classes d'equipements : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			 
			sprintf(vl_Msg,"Verification des fichiers de classes d'IHM demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = cmvf_Modif_SGBD_Ihm ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_Modif_SGBD_Ihm () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}	
							
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_EQUI,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_EQUI,XDC_NOK); 
			}
			sprintf(vl_Msg,"Verification des fichiers de classes d'IHM : OK");		
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			sprintf(vl_Msg,"Verification des fichiers de types d'astreintes demandee");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if ((vl_retcode = cmvf_ModifSGBD_RSAD ())!=XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb : cmvf_ModifSGBD_RSAD () a echoue");
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
				return;
			}
			
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_RSAD,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_RSAD,XDC_NOK); 
			}
			
			sprintf(vl_Msg,"Verification des fichiers de types d'astreintes : OK");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_GLOBAL,XDC_OK))==XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"cvce_VerifModif_cb : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_RSAD,XDC_NOK); 
			}
			sprintf(vl_Msg,"******   v�rification globale des fichiers : OK ");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			
			break;
			
			default:
			sprintf(vl_Msg,"Demande de verification de type incorrect");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING ," TCGCD : cvce_VerifModif_cb a recu un type de verification non valide !");
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
			return;
		}/*end du switch type de v�rification */
			
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a realise la v�rification demande");
	return;			
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cvce_majbd_cb est declenchee sur reception du message RTWks XDM_CGD_DBD.
*	Ce CallBack va effectuer une sauvegarde de la Base CFG sur bande,
* previent l'administrateur apr�s la sauvegarde de reinserer la bande afin de recharger 
* la base dans CFT. Ceci doit etre realise avant d'effectuer une v�rification de coh�rence
* des fichiers de configuration de la base.
* 
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cvce_majbd_cb (	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_CGD_DBD.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_vce.c,v 1.10 2016/07/28 15:56:00 devgfi Exp $ : cvce_majbd_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int 			vl_TypeAction 	= 0;		/* type RTwks : int4	*/
	int			vl_retcode    	= XDC_OK;	/* code de retour 	*/
	XDY_NomMachine  	vl_NomMachine 	= "";
	char			vl_Msg[500] 	= "";	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cvce_majbd_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CGD_DBD
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_TypeAction, NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TCGCD : cvce_majbd_cb a recu TypeAction = %d",vl_TypeAction);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_majbd_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_majbd_cb Sortie");
		return;	 
	}
	strcpy(vl_NomMachine,CGCDV_NOMMACHINE.machine);
	/*A
	** Lancement de l action sauvegarde ou restauration avant verif selon le type demande (recu par le message RTwks dans ce CB).
	*/
	switch (vl_TypeAction)
	{
		case CVCEC_SAUVEGARDE_CFG:
			sprintf(vl_Msg,"Duplication Base : La sauvegarde de la Base CFG sur bande peut prendre du temps : Une fois la sauvegarde effectu�e : r�enclenchez la bande");
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			sprintf(vl_Msg,"Duplication Base : vous ne pouvez d�clencher la v�rification de coh�rence de la base qu'une fois la sauvegarde de CFG effectu�e ainsi que sa restauration dans CFT");
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			sprintf(vl_Msg,"Duplication Base : Traitement en cours.");
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			if ((vl_retcode = XZAG08_Sauvegarder_Base(CVCEC_SITE_SAUVEGARDE,
								CGCDV_NOM_PERIPH_SVG,
								CVCEC_NOM_VOL_SAUVEGARDE_AUTO,
								CVCEC_SVG_GLOB,
								CVCEC_BASE_CFG,
								CVCEC_FIC_TRACE_SVG,
								CVCEC_CAPACITE_DAT,
								CVCEC_NOMFIC_DAT,
								vl_NomMachine))!=XDC_OK)
			{
				sprintf(vl_Msg,"Duplication Base : La Sauvegarde de CFG sur DAT s'est mal deroul�e");
				cuti_BilanConf_Trace(vl_Msg);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				sprintf(vl_Msg,"Duplication Base : Regardez le fichier %s pour r�gler le probl�me",CVCEC_FIC_TRACE_SVG);
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_majbd_cb : XZAG08_Sauvegarder_Base de CFG a echoue");	
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_majbd_cb :  Sortie");
				return;
			}
			else
			{
				sprintf(vl_Msg,"La Sauvegarde de la base de CONFIGURATION, sur DAT (avant v�rification de coh�rence) s'est bien d�roul�e");
				cuti_BilanConf_Trace(vl_Msg);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);  /* 1.8 */
				XZST_03EcritureTrace(XZSTC_INFO ,"%s",vl_Msg);
				sprintf(vl_Msg,"Duplication Base : R�enclenchez la bande avant de demander la restauration de la base CFG dans CFT");
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			}
			break;
		case CVCEC_RESTAURATION_DANS_CFT:
			if ((vl_retcode = XZAG09_Restaurer_Base(CVCEC_SITE_SAUVEGARDE,
								CGCDV_NOM_PERIPH_SVG,
								CVCEC_NOM_VOL_SAUVEGARDE_AUTO,
								CVCEC_BASE_CFT,
								CVCEC_FIC_TRACE_REST,
								CVCEC_FIC_TRACE_SVG_REST_TEMPO,
								CVCEC_NOMFIC_DAT,
								vl_NomMachine))!=XDC_OK)
			{
				sprintf(vl_Msg,"Duplication Base : Restauration de la sauvegarde de la base de configuration dans la base de test s'est mal pass�e :");
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				sprintf(vl_Msg,"	Regarder le fichier de bilan de restauration : %s",
						CVCEC_FIC_TRACE_REST);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				sprintf(vl_Msg,"Duplication Base : Restauration de la sauvegarde de la base de configuration dans la base de test s'est mal pass�e : Regarder le fichier de bilan de restauration : %s",
						CVCEC_FIC_TRACE_REST);
					
				cuti_BilanConf_Trace(vl_Msg);
				XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_majbd_cb : XZAG09_Restaurer_Base de CFG -> CFT a echou�");	
				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_majbd_cb Sortie");	
				return;
			}
			else
			{
				sprintf(vl_Msg,"La Restauration de CFG dans CFT (avant v�rification de coh�rence) s'est bien deroul�e.");
				cuti_BilanConf_Trace(vl_Msg);
				XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg); /* 1.8 */
				XZST_03EcritureTrace(XZSTC_INFO ,"TCGCD : %s",vl_Msg);
				if ((vl_retcode = cliv_Modif_Tab_Checklist (XZCGC_TYPEF_TRANSFERT_BASE,XDC_OK))!=XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : Probl�me de positionnement de Flag Base de donnees Transf�r�e");	
				}
				cuti_Chgmt_Base_Courante (XDC_BASE_CFT);		
			}
			break;
		default:
			sprintf(vl_Msg,"Demande de rest ou sauvegarde de type inconnu");
			cuti_BilanConf_Trace(vl_Msg);
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
				
			XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_majbd_cb a recu un type d action a effectuer inconnu : autre que sauvegarde restauration !");
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_majbd_cb a echoue : Sortie");
			return;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_majbd_cb : Fin d execution");
	return;			
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU :  
*	cvce_Prerequis_ActionSurBase : Fonction qui permet de verifier
*  Si quand on effectue une v�rification sur la base (CFT) on a bien realise
*  la sauvegarde CFG-restauration CFG dans CFT ou bien si on veut realiser
*  une modification de la base CFG il faut que tous les flags de la Checklist
*  soit vierge.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cvce_Prerequis_ActionSurBase () 
/*
* ARGUMENTS EN ENTREE : aucun
*
* ARGUMENTS EN SORTIE :   aucun
*
* CODE RETOUR :  XDC_OK : On peut realiser la demande V�rification de la base
*  CFT ou Modification de la base CFG.
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_vce.c,v 1.10 2016/07/28 15:56:00 devgfi Exp $ : cvce_Prerequis_ActionSurBasecvce_majbd_cb" ;
	
	int	vl_retcode    	= XDC_OK;	/* code de retour 	*/
	int	vl_Etat		= XDC_OK;
	char	vl_Msg[500]	= "";
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cvce_Prerequis_ActionSurBase : Debut d execution");
	/*A
	** Si la Base courante est CFT, avant d'effectuer une verification
	** de coh�rence dans la base de test, on doit avoir effectuer une
	** sauvegarde de CFG et l avoir restaurer dans CFT.
	*/
	if( !strcmp(CGCDG_Base_Courante,XDC_BASE_CFT))
	{
		cliv_Consult_Tab_Checklist(XZCGC_TYPEF_TRANSFERT_BASE,&vl_Etat);
		if (vl_Etat != XDC_OK)
		{
			sprintf(vl_Msg,"Avant d'effectuer une v�rification sur la Base de donnee, il faut effectuer une sauvegarde de la Base CFG et la restaurer dans la base CFT"); 	
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			XZST_03EcritureTrace(XZSTC_MESSAGE,"TCGCD : cvce_Prerequis_ActionSurBasecvce_majbd_cb : apr�s demande de v�rification a envoy� le message suivant a l'administrateur\n : %s",vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"Autorisation pour effectuer la v�rification sur la Base de donnee CFT : OK"); 	
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			XZST_03EcritureTrace(XZSTC_MESSAGE,"TCGCD : cvce_Prerequis_ActionSurBasecvce_majbd_cb : apr�s demande de v�rification a envoy� le message suivant a l'administrateur\n : %s",vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution");
			return(XDC_OK);
		}
	}
	/*A
	** Si la Base courante est CFG, avant d'effectuer une Modification,
	** il faut que toutes les verifications de coh�rence aient ete passees
	** avec succes.
	*/
	if( !strcmp(CGCDG_Base_Courante,XDC_BASE_CFG))
	{
		cliv_Consult_Tab_Checklist(XZCGC_TYPEF_DORA,&vl_Etat);
		if (vl_Etat != XDC_OK)
		{
			sprintf(vl_Msg,"Avant d'effectuer la modification de la base CFG, il faut effectuer une sauvegarde de la Base CFG ");
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			sprintf(vl_Msg,"	et la restaurer dans la base CFT et effectuer une v�rification de coh�rence sur %s",
					XZCGC_TYPEF_DORA); 	
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			sprintf(vl_Msg,"Avant d'effectuer la modification de la base CFG, il faut effectuer une sauvegarde de la Base CFG et la restaurer dans la base CFT et effectuer une v�rification de coh�rence sur %s",
					XZCGC_TYPEF_DORA); 
			
			cuti_BilanConf_Trace(vl_Msg);
			XZST_03EcritureTrace(XZSTC_MESSAGE,"TCGCD : cvce_Prerequis_ActionSurBasecvce_majbd_cb : apr�s demande de Modification a envoy� le message suivant a l'administrateur\n : %s",vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution");
			return(XDC_NOK);
		}
		
		cliv_Consult_Tab_Checklist(XZCGC_TYPEF_EQUI,&vl_Etat);
		if (vl_Etat != XDC_OK)
		{
			sprintf(vl_Msg,"Avant d'effectuer la modification de la base CFG,\n il faut effectuer une sauvegarde de la Base CFG et la restaurer dans la base CFT et effectuer une v�rification de coh�rence sur %s",
					XZCGC_TYPEF_EQUI); 	
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			XZST_03EcritureTrace(XZSTC_MESSAGE,"TCGCD : cvce_Prerequis_ActionSurBasecvce_majbd_cb : apr�s demande de Modification a envoy� le message suivant a l'administrateur\n : %s",vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution");
			return(XDC_NOK);
		}
		
		cliv_Consult_Tab_Checklist(XZCGC_TYPEF_DORA,&vl_Etat);
		if (vl_Etat != XDC_OK)
		{
			sprintf(vl_Msg,"Avant d'effectuer la modification de la base CFG, il faut effectuer une sauvegarde de la Base CFG et la restaurer dans la base CFT et effectuer une v�rification de coh�rence sur %s",
					XZCGC_TYPEF_DORA); 	
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			XZST_03EcritureTrace(XZSTC_MESSAGE,"TCGCD : cvce_Prerequis_ActionSurBasecvce_majbd_cb : apr�s demande de Modification a envoy� le message suivant a l'administrateur\n : %s",vl_Msg);
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution");
			return(XDC_NOK);
		}
		else
		{
			sprintf(vl_Msg,"Autorisation pour effectuer la Modification sur la Base de donn�e CFG : OK. Traitement en cours"); 	
			XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Msg);
			cuti_BilanConf_Trace(vl_Msg);
			XZST_03EcritureTrace(XZSTC_MESSAGE,"TCGCD : cvce_Prerequis_ActionSurBasecvce_majbd_cb : apr�s demande de Modification a envoy� le message suivant a l'administrateur\n : %s",vl_Msg);
		}
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution");
		return(XDC_OK);
	}
	else
	XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cvce_Prerequis_ActionSurBase a d�tect� un �tat non prevu au niveau des actions a effectuer au niveau de la base");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cvce_Prerequis_ActionSurBase : Fin d execution : Echec");
	return(XDC_NOK);
}
