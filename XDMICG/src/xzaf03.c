/*E*/
/* Fichier : $Id: xzaf03.c,v 1.2 1995/01/17 11:59:40 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/17 11:59:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaf03.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       13/01/95     : Creation
* C.T.       17/01/95     : Modif + test (V 1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf03.h"

static char *version = "$Id: xzaf03.c,v 1.2 1995/01/17 11:59:40 torregrossa Exp $ XZAF03_Animer_Neut_Voies";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Rechercher les evenements et les actions clos en cours a
*  l'heure specifiee pour mettre a jour les tables de formation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAF03_Animer_Neut_Voies (	XDY_FonctionInt	va_FonctionUtilisateur)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate		va_Horodate
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAF02SP
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzaf03.c,v 1.2 1995/01/17 11:59:40 torregrossa Exp $ XZAF03_Animer_Neut_Voies";
	
	CS_CHAR 	*vl_RpcName =XZAFC_XZAF03_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	int		vl_Status;
	int		i;
	tg_row_result	vl_Tab_Ligne;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	XDY_Entier	SPstatus;
	XDY_Evt		vl_Evenement;
	XDY_Act		vl_Action;
	XDY_Entier	vl_Translation;
	XDY_Sens	vl_Sens;
	XDY_Voie	vl_Voie1;
	XDY_Voie	vl_NVoie1;
	XDY_Voie	vl_Voie2;
	XDY_Voie	vl_NVoie2;
	XDY_Voie	vl_Voie3;
	XDY_Voie	vl_NVoie3;
	XDY_Voie	vl_Voie4;
	XDY_Voie	vl_NVoie4;
	XDY_Voie	vl_Voie5;
	XDY_Voie	vl_NVoie5;
	XDY_Voie	vl_Voie6;
	XDY_Voie	vl_NVoie6;
	XDY_Voie	vl_Voie7;
	XDY_Voie	vl_NVoie7;
	XDY_Voie	vl_Voie8;
	XDY_Voie	vl_NVoie8;
	XDY_Voie	vl_Voie9;
	XDY_Voie	vl_NVoie9;
	XDY_Voie	vl_Voie10;
	XDY_Voie	vl_NVoie10;
	XDY_Mot		vl_Portion;
	XDY_Booleen	vl_DebutFin;
	
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAF03_Animer_Neut_Voies ");
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_Retcode = ct_command(vl_Cmd, CS_RPC_CMD, vl_RpcName, 
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}
        
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_Retcode = ct_results(vl_Cmd, &vl_ResType)) == CS_SUCCEED)
	{
		switch ((int)vl_ResType)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_ResType)
				{
					case  CS_ROW_RESULT:
						vl_Retcode = asql_fetch_row_result(vl_Cmd, &vl_Nb_Ligne_Lue, &vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf03: ROW RESULTS nb lignes:%d", vl_Nb_Ligne_Lue);
						for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						{
							vl_Evenement.numero = *(XDY_Entier *)vl_Tab_Ligne[i][0].pt_value;
							vl_Evenement.cle = *(XDY_Octet *)vl_Tab_Ligne[i][1].pt_value;
							vl_DebutFin = *(XDY_Booleen *)vl_Tab_Ligne[i][2].pt_value;
							vl_Portion = *(XDY_Mot *)vl_Tab_Ligne[i][3].pt_value;
							vl_Translation = *(XDY_Entier *)vl_Tab_Ligne[i][4].pt_value;
							vl_Voie1 = *(XDY_Voie *)vl_Tab_Ligne[i][5].pt_value;
							vl_NVoie1 = *(XDY_Voie *)vl_Tab_Ligne[i][6].pt_value;
							vl_Voie2 = *(XDY_Voie *)vl_Tab_Ligne[i][7].pt_value;
							vl_NVoie2 = *(XDY_Voie *)vl_Tab_Ligne[i][8].pt_value;
							vl_Voie3 = *(XDY_Voie *)vl_Tab_Ligne[i][9].pt_value;
							vl_NVoie3 = *(XDY_Voie *)vl_Tab_Ligne[i][10].pt_value;
							vl_Voie4 = *(XDY_Voie *)vl_Tab_Ligne[i][11].pt_value;
							vl_NVoie4 = *(XDY_Voie *)vl_Tab_Ligne[i][12].pt_value;
							vl_Voie5 = *(XDY_Voie *)vl_Tab_Ligne[i][13].pt_value;
							vl_NVoie5 = *(XDY_Voie *)vl_Tab_Ligne[i][14].pt_value;
							vl_Voie6 = *(XDY_Voie *)vl_Tab_Ligne[i][15].pt_value;
							vl_NVoie6 = *(XDY_Voie *)vl_Tab_Ligne[i][16].pt_value;
							vl_Voie7 = *(XDY_Voie *)vl_Tab_Ligne[i][17].pt_value;
							vl_NVoie7 = *(XDY_Voie *)vl_Tab_Ligne[i][18].pt_value;
							vl_Voie8 = *(XDY_Voie *)vl_Tab_Ligne[i][19].pt_value;
							vl_NVoie8 = *(XDY_Voie *)vl_Tab_Ligne[i][20].pt_value;
							vl_Voie9 = *(XDY_Voie *)vl_Tab_Ligne[i][21].pt_value;
							vl_NVoie9 = *(XDY_Voie *)vl_Tab_Ligne[i][22].pt_value;
							vl_Voie10 = *(XDY_Voie *)vl_Tab_Ligne[i][23].pt_value;
							vl_NVoie10 = *(XDY_Voie *)vl_Tab_Ligne[i][24].pt_value;

							if (va_FonctionUtilisateur == NULL)
							{
								XZST_03EcritureTrace(XZSTC_WARNING,
										"%s : Fonction utilisateur retourne XDC_NOK", version);
								asql_clean_cmd(vl_Connection, vl_Cmd);
								return XDC_NOK;
							}
							vl_Status = (*va_FonctionUtilisateur)(vl_Evenement,
									vl_DebutFin, vl_Portion, 
									vl_Translation, 
									vl_Voie1, vl_NVoie1,
									vl_Voie2, vl_NVoie2,
									vl_Voie3, vl_NVoie3,
									vl_Voie4, vl_NVoie4,
									vl_Voie5, vl_NVoie5,
									vl_Voie6, vl_NVoie6,
									vl_Voie7, vl_NVoie7,
									vl_Voie8, vl_NVoie8,
									vl_Voie9, vl_NVoie9,
									vl_Voie10, vl_NVoie10);
							if (vl_Status != XDC_OK)
								XZST_03EcritureTrace(XZSTC_WARNING, "xzaf03 : fonction utilisateur retourne %d", vl_Status);
						}
						vl_Retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,
									vl_Nb_Col,&vl_Tab_Ligne);
						if (vl_Retcode != XDC_OK)
						{
							XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
							asql_clean_cmd(vl_Connection, vl_Cmd);
							return XDC_NOK;
						}
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf03 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf03 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaf03 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne 1");
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
						   return (XDC_NOK);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED && vl_ResType != CS_ROW_RESULT)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: asql_fetch_data* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_Retcode = ct_res_info(vl_Cmd, CS_MSGTYPE,(CS_VOID *)&vl_MsgId, CS_UNUSED, NULL);
					if (vl_Retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", vl_Retcode);
						asql_clean_cmd(vl_Connection, vl_Cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaf03: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAF03_Animer_Neut_Voies retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAF03_Animer_Neut_Voies \n");
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);

	return(XDC_OK);
}

