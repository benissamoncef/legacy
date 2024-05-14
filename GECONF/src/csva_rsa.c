/*E*/
/*Fichier : $Id: csva_rsa.c,v 1.3 2016/05/31 18:38:06 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2016/05/31 18:38:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE RSA * FICHIER csva_rsa.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
*	Module MRSA de TCSVA contenant le callback permettant
*  de restaurer une base archiver sur bande dans la base 
*  sauvegarde/restauration.
* DCG-002/0.1
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* JPL		24/05/2016	: Gestion nom lecteur DAT, lu en fichier de config. (DEM 1165)  1.2
* JPL		31/05/2016	: Utilisation fonction commune pour lire en fichier de config.  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "csva_rsa.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: csva_rsa.c,v 1.3 2016/05/31 18:38:06 devgfi Exp $ : csva_rsa" ;

static XDY_Nom		CRSAV_NOM_PERIPH_RESTAUR = CRSAC_PERIPH_RSTR_DEFAUT;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	crsa_restarc_cb est declenchee sur reception du message RTWks XDM_CSV_RES.
*	Ce CallBack va effectuer une restauration des bases du mois precise a partir de
* la bande dans la base de sauvegarde/restauration.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void crsa_restarc_cb (	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_CSV_RES.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: csva_rsa.c,v 1.3 2016/05/31 18:38:06 devgfi Exp $ : crsa_restarc_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Entier 		vl_Mois 	= 0;		/* type RTwks : int4*/
	XDY_Entier 		vl_Annee	= 0;		/* type RTwks : int4*/
	XDY_Entier 		vl_TapeID	= 0;
	int			vl_retcode    	= XDC_OK;	/* code de retour 	*/
	XDY_NomMachine  	vl_NomMachine 	= "";
	char			vl_Msg[500] 	= "";	
	char			vl_TapeIDChar[8]= "";
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : crsa_restarc_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CSV_RES
	*/ 
	if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4, &vl_Mois,T_IPC_FT_INT4, &vl_Annee, NULL))
	{
		XZST_03EcritureTrace(XZSTC_INFO,"TCSVA : crsa_restarc_cb a recu l ordre de sauvegarde du mois %d de l annee %d = %d",
				vl_Mois, 
				vl_Annee);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : crsa_restarc_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : crsa_restarc_cb Sortie");
		return;	 
	}
	vl_TapeID = (vl_Annee * 100) + vl_Mois ;
	sprintf(vl_TapeIDChar,"%d",vl_TapeID);
	
	XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : crsa_restarc_cb : l ID de la cassette a restaurer est %ld ",vl_TapeID);


	/*A
	** Lecture en configuration du nom du lecteur de sauvegarde
	*/
	{
		char		*vl_nom_F_Devices = XDC_PATH_DEQ "/" XDF_CONF_PERIPH_SYSTEME;
		XDY_Nom		vl_nom_lecteur = "";

		if ( (XDF01_ValeurNom (vl_nom_F_Devices, "CRSAC_DEVICE_RESTAURATION", vl_nom_lecteur, NULL) == XDC_OK) &&
		      strcmp (vl_nom_lecteur, "") != 0 )
		{
			strcpy (CRSAV_NOM_PERIPH_RESTAUR, vl_nom_lecteur);
			XZST_03EcritureTrace(XZSTC_INFO, "TCSVA : Lecteur de sauvegarde = %s", CRSAV_NOM_PERIPH_RESTAUR);
		} else {
			strcpy (CRSAV_NOM_PERIPH_RESTAUR, CRSAC_PERIPH_RSTR_DEFAUT);
			XZST_03EcritureTrace(XZSTC_WARNING, "TCSVA : Aucun lecteur defini, defaut %s utilise", CRSAV_NOM_PERIPH_RESTAUR);
		}
	}


	if ((vl_retcode = XZAG09_Restaurer_Base(	CRSAC_SITE_SAUVEGARDE,
							CRSAV_NOM_PERIPH_RESTAUR,
							vl_TapeIDChar,
							CRSAC_BASE_SVG,
							CRSAC_FIC_TRACE_REST,
							CRSAC_FIC_TRACE_REST_TEMPO,
							CRSAC_NOMFIC_DAT,
							vl_NomMachine))!=XDC_OK)
	{
		sprintf(vl_Msg,"La restauration du mois %d de l annee %d archive a partir de la bande %s, a echoue"
					,vl_Mois,vl_Annee,vl_TapeIDChar);
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : crsa_restarc_cb : XZAG09_Restaurer_Base : %s",vl_Msg);	
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : crsa_restarc_cb :  Sortie");
		return;
	}
	else
	{
		/*XZST_10ArchiverMsgSyst(XZSTC_INFO,"La Restauration de l archive du mois %d de l annee %d dans la Base SVG, s est bien deroulee",vl_Mois,vl_Annee);*/
		XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : La Restauration de l archive du mois %d de l annee %d dans la Base SVG, s est bien deroulee",vl_Mois,vl_Annee);
		sprintf(vl_Msg,"La Restauration de l archive du mois %d de l annee %d dans la Base SVG, s est bien deroulee.",vl_Mois,vl_Annee);
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : crsa_restarc_cb : Fin d execution");
	return;			
}
