/*E*/
/*Fichier : $Id: xzao21.c,v 1.4 1995/01/17 16:42:09 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/01/17 16:42:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao21.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       17 Nov 1994     : Creation
* volcic       24 Nov 1994     : Ajout parametre District (v1.2)
* C.T.		13 Jan 1995	: Ajout de la date en entree (V1.3)
*                                 non teste
* B.G.		17 Jan 1995	: Ajout de la date en entree XDC_OK(V1.4)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao21.c,v 1.4 1995/01/17 16:42:09 gaborit Exp $ : xzao21.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO21SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO21_Rech_Echangeur (	XDY_Autoroute	va_Autoroute_in,
				XDY_PR		va_PR_in,
				XDY_Octet	va_Sens_in,
				XDY_Mot		*pa_Ech_out,
				XDY_District	*pa_District_out,
				XDY_Horodate	va_Horodate_in)

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		Autoroute_In
* XDY_PR		PR_In
* XDY_Sens		Sens_In
* XDY8Hordate		Horodate_In	: date apres les arguments de sortie
*
* PARAMETRES EN SORTIE :	
*
* XDY_NumEqt		NumEch
* XDY_District		District
*
* VALEUR RENDUE :
*
* Recherche le premier echangeur en amont et verifie qu'il possede une
* signalisation non inhibee
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Select dans les tables EQT_GEN (EQUIPEMENT) et EQT_ECH (DEMI-ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao21.c,v 1.4 1995/01/17 16:42:09 gaborit Exp $ : xzao21_Rech_Echangeur" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO21_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      vl_datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	CS_DATETIME	vl_Horodate;
	int 		vl_num_pl_Tab_donnees2 = XZAOC_XZAO21_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	vl_SPstatus;
	XDY_Entier	vl_Tunnel;
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO21_Rech_Echangeur : Autoroute = %d\tPR = %d\tSens = %d\t Horodate = %f\n",
			va_Autoroute_in,
			va_PR_in,
			va_Sens_in,
			va_Horodate_in);
			
			
	/*
	 *A conversion de la date au format sybase
	 */
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao15: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d",vl_retcode);
		return (XDC_NOK);
	}
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	pl_Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_pl_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_ctparam_tinyint_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in,"@va_PR_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	pl_Tab_donnees2[0].pt_value = (CS_VOID *)pa_Ech_out;
	
	if ((vl_retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)pa_Ech_out,"@va_Ech_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_ctparam_smallint_output(Ech) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	
	pl_Tab_donnees2[1].pt_value = (CS_VOID *)pa_District_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)pa_District_out,"@va_District_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_ctparam_tinyint_output(District) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

		
	if ((vl_retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: asql_ctparam_date_input(datetime) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d",vl_retcode);
		printf("error de ct_param pour va_Horodate_in = %f\n",va_Horodate_in);
		asql_clean_cmd (connection,cmd);
		free(pl_Tab_donnees2);
		return (XDC_NOK);
	}	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	        return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						vl_retcode = asql_fetch_data(cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao21: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (cmd , vl_num_pl_Tab_donnees2, pl_Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao21 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao21 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao21 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne 1");
 						   asql_clean_cmd(connection, cmd);
	        				   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
						asql_clean_cmd(connection, cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	        			return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao21: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	        			return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO21_Rech_Echangeur retourne %d", vl_retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




