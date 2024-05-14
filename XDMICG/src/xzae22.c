/*E*/
/*  Fichier : 	$Id: xzae22.c,v 1.6 1994/12/19 16:13:54 volcic Exp $      Release : $Revision: 1.6 $        Date : $Date: 1994/12/19 16:13:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae22.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       08 Sep 1994     : Creation
* volcic       11 Oct 1994     : Modification recuperation select SP (v 1.4)
* volcic       12 Oct 1994     : Tests sur Row_Results (v 1.5)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae22.h"

static char *version = "$Id: xzae22.c,v 1.6 1994/12/19 16:13:54 volcic Exp $ : xzae22" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit les informations generiques d'un accident
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


int XZAE22_Decrire_Accident_PA (	XDY_Horodate		va_Horodate,
					XDY_Evt			va_NumEvt,
					XDY_FonctionInt		pa_FonctionUtilisateurMatDang,
					XDY_FonctionInt		pa_FonctionUtil,
					XZAET_Accident 		*va_Description,
					XZAET_ComplAcc 		*va_ComplementAccident)
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate		va_Horodate
* XDY_Evt		va_NumEvt
* XDY_Fonction		pa_FonctionUtilisateurMatDang	
*
* ARGUMENTS EN SORTIE :
*
* XZAET_Accident 	va_Description
* XZAET_ComplAcc 	va_ComplementAccident	
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
* Appelle XZAE41SP
* Positionnement des indicateurs de presence bebe, d'animaux, de personnes agees.
* Calcul du nombre total de personnes des bus impliques dans l'accident
* Pour chaque vehicules impliques dans l'accident, contenant une matiere dangereuse,
* appelle la fonction :
*       FonctionUtilisateurMatDang avec la structure XDY_MatDang
*
* MECANISMES :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzae22.c,v 1.6 1994/12/19 16:13:54 volcic Exp $ : E22_Decrire_Accident_PA" ;

	CS_CHAR *rpc_name =XZAEC_XZAE22_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      vl_retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE22_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne = 0;
	int		i;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XZAET_Evenement et XZAET_Voies
	*/
	
	XDY_Octet	va_VL_out = 0;
	XDY_Octet	va_PL_out = 0;
	XDY_Octet	va_Remorque_out = 0;
	XDY_Octet	va_Car_out = 0;
	XDY_Octet	va_Moto_out = 0;
	XDY_Octet	va_BlesseLeg_out = 0;
	XDY_Octet	va_BlesseGra_out = 0;
	XDY_Octet	va_Mort_out = 0;
	XDY_Booleen	va_VehEscota_out = 0;
	XDY_Booleen	va_MatDang_out = 0;
	XDY_Booleen	va_HomEscota_out = 0;
	XDY_Booleen	va_Personalite_out = 0;
	XDY_Booleen	va_DegatsDom_out = 0;
	
	XDY_Octet	va_PassagersBus_out = 0;
	
	XDY_MatDang	va_CodeMat;
	
	XZAET_AccVeh	vl_Vehicule;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	XDY_Entier	va_NumEvt_in;
	XDY_Octet	va_CleEvt_in;
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	
	CS_DATETIME	va_Horodate_in;
	
	
	/*A
	** Conversion des dates en entree du format Unix au format Sybase
	*/
	
	if ((vl_retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in))!= XDC_OK)
        {

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
		return (XDC_NOK);
	}  
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in = va_NumEvt.numero;
	va_CleEvt_in = va_NumEvt.cle;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE22_Decrire_Accident_PA : va_Horodate=%d va_NumEvt_in=%d va_CleEvt_in.cle=%d",va_Horodate_in.dttime,va_NumEvt_in,va_CleEvt_in);
	
	
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&va_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_int_input(NumEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_input(CleEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)&va_VL_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_VL_out,"@va_VL_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(VL) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[1].pt_value = (CS_VOID *)&va_PL_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_PL_out,"@va_PL_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(PL) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[2].pt_value = (CS_VOID *)&va_Remorque_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Remorque_out,"@va_Remorque_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(Remorque) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)&va_Car_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Car_out,"@va_Car_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(Car) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[4].pt_value = (CS_VOID *)&va_Moto_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Moto_out,"@va_Moto_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(Moto) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[5].pt_value = (CS_VOID *)&va_BlesseLeg_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_BlesseLeg_out,"@va_BlesseLeg_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(BlesseLeg) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[6].pt_value = (CS_VOID *)&va_BlesseGra_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_BlesseGra_out,"@va_BlesseGra_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(BlesseGra) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[7].pt_value = (CS_VOID *)&va_Mort_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Mort_out,"@va_Mort_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_tinyint_output(Mort) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	Tab_donnees2[8].pt_value = (CS_VOID *)&va_VehEscota_out;
	
	if ((vl_retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_VehEscota_out,"@va_VehEscota_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_bit_output(VehEscota) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[9].pt_value = (CS_VOID *)&va_MatDang_out;
	
	if ((vl_retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_MatDang_out,"@va_MatDang_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_bit_output(MatDang) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[10].pt_value = (CS_VOID *)&va_HomEscota_out;
	
	if ((vl_retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_HomEscota_out,"@va_HomEscota_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_bit_output(HomEscota) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[11].pt_value = (CS_VOID *)&va_Personalite_out;
	
	if ((vl_retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_Personalite_out,"@va_Personalite_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_bit_output(Personalite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[12].pt_value = (CS_VOID *)&va_DegatsDom_out;
	
	if ((vl_retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_DegatsDom_out,"@va_DegatsDom_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_bit_output(DegatsDom) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
	   	return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae22: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae22 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae22 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae22 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
						   free (Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_fetch_* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
						asql_clean_cmd(connection, cmd);
						free(Tab_donnees2);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
					free(Tab_donnees2);
					return (XDC_NOK);
				}
	}
	


	/*A
	** Affectation des structures parametres de sortie de XZAET_Accident
	*/
	
	va_Description->VL		= va_VL_out;
	va_Description->PL		= va_PL_out;
	va_Description->Remorque	= va_Remorque_out;
	va_Description->Car	  	= va_Car_out;
	va_Description->Moto	  	= va_Moto_out;
	va_Description->BlesseLeg	= va_BlesseLeg_out;
	va_Description->BlesseGra	= va_BlesseGra_out;
	va_Description->Mort	  	= va_Mort_out;
	va_Description->VehEscota	= va_VehEscota_out;
	va_Description->MatDang 	= va_MatDang_out;
	va_Description->HomEscota	= va_HomEscota_out;
	va_Description->Personnalite 	= va_Personalite_out;
	va_Description->DegatsDom	= va_DegatsDom_out;
	
	
	/*A
	** Appel de la fonction utilisateur avec XZAET_Vehicule
	*/
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           
           vl_Vehicule.Evt 		= * (XDY_Evt *) vl_Tab_Ligne[i][0].pt_value;
           vl_Vehicule.NbPassagers	= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
           vl_Vehicule.Bebe		= * (XDY_Booleen *) vl_Tab_Ligne[i][2].pt_value;
           vl_Vehicule.PersAgees	= * (XDY_Booleen *) vl_Tab_Ligne[i][3].pt_value;
           vl_Vehicule.Animaux		= * (XDY_Booleen *) vl_Tab_Ligne[i][4].pt_value;
           vl_Vehicule.VehEscota	= * (XDY_Booleen *) vl_Tab_Ligne[i][5].pt_value;
           vl_Vehicule.VolCharge	= * (XDY_Octet *) vl_Tab_Ligne[i][6].pt_value;
           strcpy ( vl_Vehicule.MatDang, (char *) vl_Tab_Ligne[i][7].pt_value );
           vl_Vehicule.Depannage	= * (XDY_Entier *) vl_Tab_Ligne[i][8].pt_value;
           vl_Vehicule.SiteDepannage	= * (XDY_Octet *) vl_Tab_Ligne[i][9].pt_value;
           strcpy ( vl_Vehicule.Commentaire, (char *) vl_Tab_Ligne[i][10].pt_value );
           vl_Vehicule.NumVehicule	= * (XDY_Entier *) vl_Tab_Ligne[i][11].pt_value;
           
           
           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurListe
	   */
	  
	   if ( pa_FonctionUtil == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: pa_FonctionUtilisateurListe est nulle");
	      asql_clean_cmd(connection, cmd);
	      free(Tab_donnees2);
	      return ( XDC_NOK );
	   }
	   else
	   {
	      if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_Vehicule ) ) != XDC_OK )
              {    
                 XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: appel de FonctionUtilisateur s'est mal deroule");
                 asql_clean_cmd(connection, cmd);
	         free(Tab_donnees2);
	         return ( XDC_NOK );
              }
	   }
	}
	
	
	/*A
	** Positionnement des indicateurs de presence de bebes, d'animaux, de personnes agees
	*/
	
	/*B
	** Indicateur presence bebe
	*/
	
	va_ComplementAccident->Bebe = XDC_FAUX;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	  if ( * (XDY_Booleen *) vl_Tab_Ligne[i][2].pt_value == XDC_VRAI )
	   {
	      va_ComplementAccident->Bebe = XDC_VRAI;
	      break;
	   }
	}
	
	
	/*B
	** Indicateur presence personnes agees
	*/
	
	va_ComplementAccident->PersAgee = XDC_FAUX;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	   if ( * (XDY_Booleen *) vl_Tab_Ligne[i][3].pt_value == XDC_VRAI )
	   {
	      va_ComplementAccident->PersAgee = XDC_VRAI;
	      break;
	   }
	}
	
	
	/*B
	** Indicateur presence animaux
	*/
	
	va_ComplementAccident->Animaux = XDC_FAUX;

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	   if ( * (XDY_Booleen *) vl_Tab_Ligne[i][4].pt_value == XDC_VRAI )
	   {
	      va_ComplementAccident->Animaux = XDC_VRAI;
	      break;
	   }
	}
	 
	   
	/*A
	** Calcul du nombre total de personnes des bus impliques
	*/
	
	va_PassagersBus_out = 0;
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
	   if ( * (XDY_Octet *) vl_Tab_Ligne[i][0].pt_value == XZAEC_VehiculeCar)
	      va_PassagersBus_out =  va_PassagersBus_out + * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value; 
	}
	
	va_ComplementAccident->PassagersBus = va_PassagersBus_out;
 	
 	
 	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurMatDang pour
	** chaque vehicule contenant une matiere dangereuse
	*/
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           if (strcspn((char *) vl_Tab_Ligne[i][7].pt_value, NULL) != NULL)
           {           
      	      strcpy(va_CodeMat, (char *) vl_Tab_Ligne[i][7].pt_value); 

	      /*B
	      ** Appel de la fonction utilisateur FonctionUtilisateurMatDang
	      */
	  
	      if ( pa_FonctionUtilisateurMatDang == NULL)
	      {
		 XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: pa_FonctionUtilisateurMatDang est nulle");
	         asql_clean_cmd(connection, cmd);
		 free(Tab_donnees2);
		 return ( XDC_NOK );
              }
	      else
	      {
	         if ( (vl_retcode = (*pa_FonctionUtilisateurMatDang) ( va_CodeMat ) ) != XDC_OK )
                 {    
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: appel de FonctionUtilisateur s'est mal deroule");
                    asql_clean_cmd(connection, cmd);
		    free(Tab_donnees2);
		    return ( XDC_NOK );
                 }
	      }	
	   }
	}
	
	
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	vl_retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	if (vl_retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return(vl_retcode);
	}
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
 	asql_clean_cmd(connection, cmd);
	free(Tab_donnees2);
	return(XDC_OK);
}
