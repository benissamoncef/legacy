/*E*/
/*  Fichier : @(#)xzao225.c	1.1        Release : 1.1        Date : 06/29/07
-------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao225.c
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	29/06/07 : Creation  1.1
-----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao225.c	1.1 06/29/07 : xzao225.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO225SP et pour chaque ligne retournée par la procédure,
*   appelle la fonction utilisateur indiquée.
*
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO225_Liste_Cantons_Par_Portion (	XDY_Basedd	va_Basedd_in,
					XDY_Nom		va_NomTroncon_in,
					XDY_Mot		va_NumeroPortion_in,
					XDY_FonctionInt va_FonctionUtilisateurCanton_in,
					XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	va_NumeroPortion_in
* XDY_Fonction  va_FonctionUtilisateurCanton_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des cantons d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
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
* Pour chaque canton appel de la fonction utilisateur avec XZAOT_Canton
*
* MECANISMES :
*
-----------------------------------------------------------------------------*/

{
	static char *version = "@(#)xzao225.c	1.1 06/29/07 : XZAO225_Liste_Cantons_Par_Portion.c" ;

	CS_CHAR *rpc_name = XZAOC_XZAO225_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO225_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;


	/*A
	** Definition des variables locales
	*/

	XZAOT_Canton	vl_Canton;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO225_Liste_Cantons_Par_Portion : Basedd = %s\tTroncon = %s\tPortion = %d\tFonction = %ld\n",
			va_Basedd_in,
			va_NomTroncon_in,
			va_NumeroPortion_in,
			va_FonctionUtilisateurCanton_in );


	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/

	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2) * num_Tab_donnees2);


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
		free(Tab_donnees2);
		return (XDC_NOK);
	}




	/*A
	** Envoi de la commande RPC pour cette procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d'I/O
	** a faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomTroncon_in,"@va_NomTroncon_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: asql_ctparam_char_input(NomTroncon) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumeroPortion_in,"@va_NumeroPortion_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: asql_ctparam_smallint_input(NumeroPortion) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}



	/*A
	** Envoi de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao225: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao225 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao225 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao225 : STATUS REUSULT PROCEDURE STOCKEE = %ld", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne 1");
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: ct_res_info(msgtype) a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
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
				** Le serveur SQL a detecte une erreur en executant notre commande.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: ct_results retourne CS_CMD_FAIL.");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				free(Tab_donnees2);
				return (XDC_NOK);

			default:
				/*
				** Il s est passe quelque chose d inattendu.
				*/
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao225: ct_results retourne un resultat de type inattendu");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
				asql_clean_cmd(connection, cmd);
				free(Tab_donnees2);
				return (XDC_NOK);
			}
	}


	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurCanton pour chaque Canton
	** Affectation des parametres de sortie
	*/

	va_Resultat_out = 0;

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
		va_Resultat_out ++;

		/*B
		** Remplissage de la structure XZAOT_Canton coorrespondant a la ligne resultat
		** du select de la procedure stockee XZAO225
		*/

		vl_Canton.NumeroAutoroute	= * (XDY_Autoroute *) vl_Tab_Ligne[i][0].pt_value;
		vl_Canton.PR_Debut		= * (XDY_PR *) vl_Tab_Ligne[i][1].pt_value;
		vl_Canton.Sens			= * (XDY_Sens *) vl_Tab_Ligne[i][2].pt_value;

		/*B
		** Appel de la fonction utilisateur FonctionUtilisateurCanton
		*/

		if ( va_FonctionUtilisateurCanton_in == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzao225 : va_FonctionUtilisateurCanton_in est nulle");
			asql_clean_cmd(connection, cmd);
			free(Tab_donnees2);
			return (XDC_NOK);
		}
		else
		{
			if ( ( retcode = (*va_FonctionUtilisateurCanton_in) ( vl_Canton ) ) != XDC_OK )
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao225 : appel de va_FonctionUtilisateurCanton_in s'est mal deroule");
				asql_clean_cmd(connection, cmd);
				free(Tab_donnees2);
				return (XDC_NOK);
			}
		}
	}

	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/

	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

	if (retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao225 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO225_Liste_Cantons_Par_Portion retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}
