/*E*/
/*  Fichier : 	$Id: xzao70.c,v 1.4 2018/10/25 10:49:07 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/10/25 10:49:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao70.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       22 Sep 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.2)
* JMG		15/08/18 : ajout picto suppression SAGA DEM1306 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao70.c,v 1.4 2018/10/25 10:49:07 pc2dpdy Exp $ : xzao70.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO70SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO70_Ajouter_Type_PMV(	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfTypPMV	va_TypePMV_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomType_in
* XDY_Octet	va_NombreLignes_in	
* XDY_Octet	va_NombreCaracteres_in
* XDY_Booleen	va_Flash_in
* XDY_Booleen	va_Temperature_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un type de PMV en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table TYP_PMV (TYPE PMV)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao70.c,v 1.4 2018/10/25 10:49:07 pc2dpdy Exp $ : XZAO70_Ajouter_TypePMV" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO70_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO70_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Nom		vl_NomType;
   	XDY_Octet	vl_NombreLignes;
   	XDY_Octet	vl_NombreCaracteres;
   	XDY_Octet	vl_Picto;
   	XDY_Booleen	vl_Flash;
   	XDY_Booleen	vl_Temperature;
   	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomType, va_TypePMV_in.NomType );
	vl_NombreLignes    = va_TypePMV_in.NombreLignes;
	vl_NombreCaracteres = va_TypePMV_in.NombreCaracteres;
	vl_Flash	    = va_TypePMV_in.Flash;
	vl_Temperature 	    = va_TypePMV_in.Temperature;
	vl_Picto 	    = va_TypePMV_in.Picto;
	
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO70_Ajouter_TypePMV : TypePMV = %s\tNombreLignes = %d\tNombreCaracteres = %d\tFlash = %d\tTemperature = %d\tPicto = %d\n",
			va_TypePMV_in.NomType,
			va_TypePMV_in.NombreLignes,
			va_TypePMV_in.NombreCaracteres,
			va_TypePMV_in.Flash,
			va_TypePMV_in.Temperature,
			vl_Picto);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomType,"@va_NomType_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_NombreLignes,"@va_NombreLignes_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao51: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_NombreCaracteres,"@va_NombreCaracteres_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao51: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Flash,"@va_Flash_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao51: asql_ctparam_bit_input(bit) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Temperature,"@va_Temperature_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao51: asql_ctparam_bit_input(bit) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Picto,"@va_Picto_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao51: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
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
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzao70: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao70 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao70 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao70 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO70Ajouter_TypePMV retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
						   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"xzao70: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO70_Ajouter_TypePMV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




