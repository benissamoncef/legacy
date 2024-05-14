/*E*/
/*Fichier : $Id: xzao136.c,v 1.5 2020/11/03 17:57:09 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 17:57:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao136.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       11 Oct 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.2)
* volcic       29 Nov 1995     : Modif allocation Tab_donees2 (v1.3)
* Niepceron	27 jan 2009	: L'arg en output est remplac� par un select & Renommage de la proc en XZAO136 v1.4 DEM800
* LCL		10/03/20	: Correction pour passage sous Linux 64 MOVIS DEM-SAE93 v1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao136.c,v 1.5 2020/11/03 17:57:09 pc2dpdy Exp $ : xzao136.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO136SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO136_Lire_Suppleant (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt	va_NomMachine_in,
				XDY_NomEqt 	va_Suppleant_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_NomEqt	va_NomMachine_in
*
* PARAMETRES EN SORTIE :
*
* XDY_NomEqt	va_Suppleant_out
*
* VALEUR RENDUE :
*
*  Lire la machine suppleante concernee 
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
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao136.c,v 1.5 2020/11/03 17:57:09 pc2dpdy Exp $ : XZAO136_Lire_Suppleant" ;

	CS_CHAR *rpc_name = XZAOC_XZAO136_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO136_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAOC_XZAO136_NB_PARM_RETURN];
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i,j;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO136_Lire_Suppleant : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO136_Lire_Suppleant : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );
			
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
/*	Tab_donnees2[0].pt_value = (CS_VOID *)va_Suppleant_out;
	
	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Suppleant_out,"@va_Suppleant_out")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"xzao136: asql_ctparam_char_output(Suppleant) a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	return (XDC_NOK);
	}
*/	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
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
						retcode = asql_fetch_row_result (cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao136: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
					        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                                                XZST_03EcritureTrace(XZSTC_INFO,"xzao136 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao136 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao136 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne 1");
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
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					XZST_03EcritureTrace(XZSTC_INFO,"xzao136 : ct_result retourne CS_MSG_RESULT l id du message est : %d.", msg_id);
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
					XZST_03EcritureTrace(XZSTC_INFO,"xzao136 : CS_CMD_DONE");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao136: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

        /*A
        ** Appel de la fonction utilisateur pour chaque FMC
        */


        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
        {

           /*B
           * recup des parametres de retour
           */

           strncpy ( va_Suppleant_out, (char *) vl_Tab_Ligne[i][0].pt_value,10 );
           va_Suppleant_out[10]='\0';
		   for (j=0;j<=10;j++)
		   {
//			   XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO136_Lire_Suppleant va_Suppleant_out[%d]=%d.", j,va_Suppleant_out[j]);
				if (va_Suppleant_out[j]==(char)32)
				{
//					XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO136_Lire_Suppleant - REPLACE");
					va_Suppleant_out[j]='\0';
					j=10;
				}
//				else XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO136_Lire_Suppleant - NO REPLACE");
		   }


        }
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO136_Lire_Suppleant retourne %s.", va_Suppleant_out);
 	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO136_Lire_Suppleant retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




