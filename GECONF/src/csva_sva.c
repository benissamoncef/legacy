/*E*/
/*Fichier : $Id: csva_sva.c,v 1.6 2016/05/31 18:38:14 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2016/05/31 18:38:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE SVA * FICHIER csva_sva.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module MSVA de TCSVA contenant le callback de sauvegarde archivage
* de a base histo (DCG-002/0.1).
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* mercier	07 Mar 1996	: Ajout msg adm lors de svg et correction orthographe v 1.2
* niepceron	08 Mar 1996	: Modif appel xzag08 en passant CSVAV_NOMMACHINE a la place de vl_machine v1.3
* JPL		24/05/2016	: Gestion nom lecteur DAT, lu en fichier de config. (DEM 1165)  1.5
* JPL		31/05/2016	: Utilisation fonction commune pour lire en fichier de config.  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "csva_sva.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: csva_sva.c,v 1.6 2016/05/31 18:38:14 devgfi Exp $ : csva_sva" ;

static XDY_Nom		CSVAV_NOM_PERIPH_SVG = CSVAC_PERIPH_SVG_DEFAUT;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	csva_sauvebd_cb est declenchee sur reception du message RTWks XDM_CSV_ARC.
*	Ce CallBack va effectuer une sauvegarde des bases sur bande et vide le journal
* des transactions,
* previent l administrateur de l etat de la sauvegarde.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void csva_sauvebd_cb (	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_CSV_ARC.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: csva_sva.c,v 1.6 2016/05/31 18:38:14 devgfi Exp $ : csva_sauvebd_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Entier 		vl_Mois 	= 0;		/* type RTwks : int4*/
	XDY_Entier 		vl_Annee	= 0;		/* type RTwks : int4*/
	XDY_Entier 		vl_MoisSVG 	= 0;		
	XDY_Entier 		vl_AnneeSVG	= 0;
	
	XDY_Entier 		vl_TapeID	= 0;
	char			vl_TapeIDChar[8]= "";
	
	int			vl_retcode    	= XDC_OK;	/* code de retour 	*/
	XDY_NomMachine  	vl_NomMachine 	= "";
	char			vl_Msg[500] 	= "";
	XZCAT_ComposantCAT 	vl_CompCAT	;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : csva_sauvebd_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CSV_ARC
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Mois,T_IPC_FT_INT4, &vl_Annee, NULL))
	{
		XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : csva_sauvebd_cb : TipcMsgRead : csva_sauvebd_cb a recu l ordre de sauvegarde du mois %d de l annee %d",
				vl_Mois, 
				vl_Annee);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_sauvebd_cb Sortie");
		return;	 
	}
	
	/*strcpy(vl_NomMachine,CSVAV_NOMMACHINE.machine);*/
	
	if (( vl_Mois >12) || (vl_Mois < 1))
	{
		sprintf(vl_Msg,"Le mois à archiver n'est pas valide : %ld ",vl_Mois);
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : Le mois recu n est pas correct : %ld",vl_Mois);	
		return;
	}
	if( vl_Annee < 1994 ) 
	{
		sprintf(vl_Msg,"L'annee à archiver %ld, ne peut etre antecedente a 1994.",vl_Annee);
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : %s",vl_Msg);	
		return;
	}
	/*A
	** Transfert du mois a archiver dans la base de sauvegarde/restauration (svg)
	** si ce n est deja fait (variables globales).
	*/
	if ((vl_retcode = cgca_LectMoisAnneeBD ( &vl_MoisSVG, &vl_AnneeSVG))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : %s",vl_Msg);
	}
	
        /*A
        ** Message a l'administrateur systeme du debut de la sauvegarde */

        sprintf(vl_Msg,"Sauvegarde du mois %ld de %ld en cours.", vl_Mois, vl_Annee);
        XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);

	if ( (vl_Mois != vl_MoisSVG) || (vl_Annee != vl_AnneeSVG))
	{
		if ((vl_retcode = XZAG01_Transfert_Mois (vl_Mois,vl_Annee))!=XDC_OK)
		{
			sprintf(vl_Msg,"Le mois %d de l'annee %d n'a pu etre transféré vers la base sauvegarde/restauration",vl_Mois,vl_Annee);
			XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : %s",vl_Msg);	
			return;
		}
		else
		{
			sprintf(vl_Msg,"Le mois %d de l'annee %d a pu etre transféré vers la base sauvegarde/restauration",vl_Mois,vl_Annee);
			XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
			XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : csva_sauvebd_cb : %s",vl_Msg);
			/*B
			** Memorisation du contenu de la base SVG
			*/ 
			cgca_EcritMoisAnneeBD (vl_Mois , vl_Annee);
		}
	}

/*A
	** Reconstitution de l identificateur de la bande
	*/
	vl_TapeID = (vl_Annee * 100) + vl_Mois ;
	sprintf(vl_TapeIDChar,"%d",vl_TapeID);
	sprintf(vl_Msg,"L'ID de la cassette générée sera %ld ",vl_TapeID);
	XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : csva_sauvebd_cb : %s",vl_Msg);
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);

	/*A
	** Lecture en configuration du nom du lecteur de sauvegarde
	*/
	{
		char		*vl_nom_F_Devices = XDC_PATH_DEQ "/" XDF_CONF_PERIPH_SYSTEME;
		XDY_Nom		vl_nom_lecteur = "";

		if ( (XDF01_ValeurNom (vl_nom_F_Devices, "CSVAC_DEVICE_SAUVEGARDE", vl_nom_lecteur, NULL) == XDC_OK) &&
		                       strcmp (vl_nom_lecteur, "") != 0 )
		{
			strcpy (CSVAV_NOM_PERIPH_SVG, vl_nom_lecteur);
			XZST_03EcritureTrace(XZSTC_INFO, "TCSVA : Lecteur de sauvegarde = %s", CSVAV_NOM_PERIPH_SVG);
		} else {
			strcpy (CSVAV_NOM_PERIPH_SVG, CSVAC_PERIPH_SVG_DEFAUT);
			XZST_03EcritureTrace(XZSTC_WARNING, "TCSVA : Aucun lecteur defini, defaut %s utilise", CSVAV_NOM_PERIPH_SVG);
		}
	}

	/*A
	** Apres constitution du num de bande pour le mois archive
	** On effectue la sauvegarde sur le DAT
	*/		
	if ((vl_retcode = XZAG08_Sauvegarder_Base(CSVAC_SITE_SAUVEGARDE,
							CSVAV_NOM_PERIPH_SVG,
							vl_TapeIDChar,
							CSVAC_SVG_GLOB,
							CSVAC_BASE_SVG,
							CSVAC_FIC_TRACE_SVG,
							CSVAC_CAPACITE_DAT,
							CSVAC_NOMFIC_DAT,
							vl_NomMachine))!=XDC_OK)
	{
		sprintf(vl_Msg,"L'archivage du mois %d de l'annee %d de la base HISTORIQUE a echoué",
					vl_Mois,
					vl_Annee);
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);	
		/*XZST_10ArchiverMsgSyst(XZSTC_WARNING,vl_Msg);*/
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : XZAG08_Sauvegarder_Base : %s",vl_Msg);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csva_sauvebd_cb :  Sortie");
		return;
	}
	else
	{
		sprintf(vl_Msg,"L'archivage du mois %d de l'annee %d de la base HISTORIQUE sur la bande %s s'est bien deroulé",
					vl_Mois,
					vl_Annee,
					vl_TapeIDChar);
		/*XZST_10ArchiverMsgSyst(XZSTC_INFO,vl_Msg);*/
		XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : %s",vl_Msg);
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	}
	/*A
	** Si l archivage s est bien deroule : on met a jour le catalogue des archives
	*/
	vl_CompCAT.BandeID = vl_TapeID;
	vl_CompCAT.NumCopie= 0;
	
	if ((vl_retcode = cgca_EcritCat(vl_CompCAT))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_sauvebd_cb : cgca_EcritCat a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csva_sauvebd_cb :  Sortie");
		return;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : La mise a jour du catalogue des archives s est bien deroulee.");
		sprintf(vl_Msg,"La mise à jour du catalogue des archives a été effectuée");
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	}
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csva_sauvebd_cb : Fin d execution");
	return;			
}

