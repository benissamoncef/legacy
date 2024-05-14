/*E*/
/*  Fichier : 	$Id: xzao71.c,v 1.3 1994/12/15 14:57:24 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/12/15 14:57:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao71.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       22 Sep 1994     : Creation
* GGY	02/05/23	:	Ajout colonne TypePnc et FichierPnc (DEM-473)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao71.c,v 1.3 1994/12/15 14:57:24 volcic Exp $ : xzao71" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO71SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO71_Ajouter_Type_Picto(	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfTypPicto	va_TypePicto_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomType_in
* XDY_Octet		va_NombreCaracteres_in
* XDY_NomFich	va_FichierPanneau_in
* XDY_Entier	va_Type_pnc_in
* XDY_Nom		va_Fichier_pnc_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un type picto en base
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
* INTERFACE :
*
* Insertion ou modification dans la table TYP_PCT (TYPE PICTO)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao71.c,v 1.3 1994/12/15 14:57:24 volcic Exp $ : XZAO71_Ajouter_TypePicto" ;
	
	CS_CHAR *rpc_name = XZAOC_XZAO71_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO71_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_Nom		vl_NomType;
   	XDY_Octet	vl_NombreCaracteres;
   	XDY_NomFich	vl_FichierPanneau;
	XDY_Octet	vl_TypePnc;
	XDY_NomFich	vl_FichierPnc;

	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomType, va_TypePicto_in.NomType );
	vl_NombreCaracteres = va_TypePicto_in.NombreCaracteres;
	strcpy( vl_FichierPanneau, va_TypePicto_in.FichierPanneau );
	vl_TypePnc = va_TypePicto_in.TypePnc;
	strcpy( vl_FichierPnc, va_TypePicto_in.FichierPnc );
	
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO71_Ajouter_TypePicto : TypePicto=%s\tNombreCaracteres=%d\tFichierPanneau=%s\tTypePnc=%d\tFichierPnc=%s.\n",
			va_TypePicto_in.NomType,
			va_TypePicto_in.NombreCaracteres,
			va_TypePicto_in.FichierPanneau,
			va_TypePicto_in.TypePnc,
			va_TypePicto_in.FichierPnc );
			
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO71_Ajouter_TypePicto : vl_NomType=%s\tvl_NombreCaracteres=%d\tvl_FichierPanneau=%s\tvl_TypePnc=%d\tvl_FichierPnc=%s.\n",
			vl_NomType,
			vl_NombreCaracteres,
			vl_FichierPanneau,
			vl_TypePnc,
			vl_FichierPnc);		
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomType,"@va_NomType_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_NombreCaracteres,"@va_NombreCaracteres_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_tinyint_input(tinyint) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	

	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_FichierPanneau,"@va_FichierPanneau_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_TypePnc,"@va_TypePnc_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_tinyint_input(CS_TINYINT) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_FichierPnc,"@va_FichierPnc_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: asql_ctparam_char_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;


	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: asql_ctparam_int_output() a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

		XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO71_Ajouter_TypePicto : vl_NomType=%s\tvl_NombreCaracteres=%d\tvl_FichierPanneau=%s\tvl_TypePnc=%d\tvl_FichierPnc=%s.\n",
			vl_NomType,
			vl_NombreCaracteres,
			vl_FichierPanneau,
			vl_TypePnc,
			vl_FichierPnc);	

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd);
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO71_Ajouter_TypePicto CS_PARAM_RESULT : cmd=%s", cmd);
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao71 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao71 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao71 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO71_Ajouter_TypePicto retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao71: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}

	
	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO71_Ajouter_TypePicto retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




