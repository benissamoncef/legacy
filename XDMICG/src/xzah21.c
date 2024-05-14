/*E*/
/*Fichier : $Id: xzah21.c,v 1.2 1995/05/22 16:26:24 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 16:26:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER xzah08.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	XZAH21 : Envoie a EQUEXT les voies radt ou dai inversees
* 
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	10 jan 1995	: Creation
* gaborit-mercier 22/05/95	: modif type analyseur (1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah21.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah21.c,v 1.2 1995/05/22 16:26:24 mercier Exp $ : xzah21" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*	XZAH21 : Envoie a EQUEXT les voies radt ou dai inversees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH21_VoiesDaiRadt(XDY_NomMachine	va_NomMachine_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_NomMachine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
*
* 
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* INTERFACE :
*
*
* MECANISMES :
*
------------------------------------------------------*/

{

	CS_CHAR *rpc_name = XZAH21C_XZAH21_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i =0;
	int		j =0;
	XDY_Entier	vl_SPstatus;
	XDY_Entier	vl_Num;
	/*A
	** Definition des variables locales a passer a XZED01
	*/	
	XDY_NomMachine 	pl_NomMachine; 
        XZEDT_Config_Voies  vl_Config_Voies; 
	
	/*A
	** Definition des variables locales a passer a XZER01
	*/	
        XZERT_Config_Voies  vl_Config_Radt; 
		
	static char *version = "$Id: xzah21.c,v 1.2 1995/05/22 16:26:24 mercier Exp $ : XZAH21" ;	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s \n",version);			
	
	/*A
	** Initialisation de la commande				
	*/
	
	vl_Num=0;

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_cmd_alloc() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_command() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_char_input(pl_cmd,(CS_CHAR *)va_NomMachine_in,"@va_LocalTechnique_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"%s: asql_ctparam_char_input(va_NomMachine_in) a echoue",version);
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
	   asql_clean_cmd (pl_connection,pl_cmd);
	   return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_send() a echoue",version);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/
	while ((vl_retcode = ct_results(pl_cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)res_type)
				{
					case  CS_ROW_RESULT:
						vl_Num++;
						vl_retcode = asql_fetch_row_result(pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"%s: ROW RESULTS nb lignes:%d select %d",version,vl_Nb_Ligne_Lue,vl_Num);
						if (vl_Num==1) /* premier select */
						{
						   for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						   {
						   	vl_Config_Voies.NoEqtAnaly   = *(XDY_Eqt  *) vl_Tab_Ligne[i][0].pt_value;
						   	vl_Config_Voies.NoAnaly      = (XDY_Mot)(*(XDY_Octet *) vl_Tab_Ligne[i][1].pt_value);
						   	vl_Config_Voies.NoEqtDAI     = *(XDY_Eqt  *) vl_Tab_Ligne[i][2].pt_value;
						   	vl_Config_Voies.EtatVoie[0]  = *(XDY_Voie *) vl_Tab_Ligne[i][3].pt_value;
						   	vl_Config_Voies.EtatVoie[1]  = *(XDY_Voie *) vl_Tab_Ligne[i][4].pt_value;
						   	vl_Config_Voies.EtatVoie[2]  = *(XDY_Voie *) vl_Tab_Ligne[i][5].pt_value;
						   	vl_Config_Voies.EtatVoie[3]  = *(XDY_Voie *) vl_Tab_Ligne[i][6].pt_value;
						   	vl_Config_Voies.EtatVoie[4]  = *(XDY_Voie *) vl_Tab_Ligne[i][7].pt_value;
						   	strcpy (pl_NomMachine,vl_Tab_Ligne[i][8].pt_value);
						   	
						   	vl_retcode=XZED01_Config_Voies_DAI(vl_Config_Voies,pl_NomMachine);
						   	if (vl_retcode!=XDC_OK)
						   	   XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZED01 retourne XDC_NOK",version);
						   }
						   vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

                                                   if (vl_retcode != XDC_OK)
                                                   {
                                                        XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
                                                        asql_clean_cmd(pl_connection, pl_cmd);
                                                        return (XDC_NOK);
                                                   }
						}
						else /* deuxieme select */
						{
						   for (i = 0; i < vl_Nb_Ligne_Lue; i++ )
						   {
						   	vl_Config_Radt.NoEqtRADT    = *(XDY_Eqt  *) vl_Tab_Ligne[i][0].pt_value;
						   	vl_Config_Radt.Sens         = *(XDY_Sens *) vl_Tab_Ligne[i][1].pt_value;
						   	vl_Config_Radt.EtatVoie[0]  = *(XDY_Voie *) vl_Tab_Ligne[i][2].pt_value;
						   	vl_Config_Radt.EtatVoie[1]  = *(XDY_Voie *) vl_Tab_Ligne[i][3].pt_value;
						   	vl_Config_Radt.EtatVoie[2]  = *(XDY_Voie *) vl_Tab_Ligne[i][4].pt_value;
						   	vl_Config_Radt.EtatVoie[3]  = *(XDY_Voie *) vl_Tab_Ligne[i][5].pt_value;
						   	vl_Config_Radt.EtatVoie[4]  = *(XDY_Voie *) vl_Tab_Ligne[i][6].pt_value;
						   	strcpy (pl_NomMachine,vl_Tab_Ligne[i][7].pt_value);
						   	
						   	vl_retcode=XZER01_Config_Voies_RADT (vl_Config_Radt,pl_NomMachine);
						   	if (vl_retcode!=XDC_OK)
						   	   XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZER01 retourne XDC_NOK",version);
						   }
						   vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

                                                   if (vl_retcode != XDC_OK)
                                                   {
                                                        XZST_03EcritureTrace(XZSTC_WARNING,"%s : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer la memoire",version);
                                                        asql_clean_cmd(pl_connection, pl_cmd);
                                                        return (XDC_NOK);
                                                   }
						}
						break;

					case  CS_PARAM_RESULT:
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_STATUS_RESULT = %ld",version, vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"%s : LE RESULTAT de STATUS DE LA PROCEDURE STOCKEE = %ld",version, vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s retourne 1",version);
 						   asql_clean_cmd (pl_connection,pl_cmd);
 						   return (XDC_NOK);
						}
						break;
				}				
				break;
				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_res_info(msgtype) a echoue",version);
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne CS_CMD_FAIL.",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"%s: ct_results retourne un resultat de type inattendu",version);
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return (XDC_NOK);
				}
	}

	
	
 		
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
	asql_clean_cmd (pl_connection,pl_cmd);	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT %s retourne %d",version, vl_retcode);
 	return(XDC_OK);
}

