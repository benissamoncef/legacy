/*E*/
/* Fichier : $Id: xzat01.c,v 1.3 2021/06/15 17:25:27 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2021/06/15 17:25:27 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat01.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Liste d'equipements et de leur etat actuel.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/02/13 : Creation (pour DEM 1061)  1.1
* JPL	25/02/13 : Conversion des parametres en types Sybase avant execution  1.2
----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"
#include "xzat01.h"

static char *version = "$Id: xzat01.c,v 1.3 2021/06/15 17:25:27 devgfi Exp $ : xzat01.c" ;

/* Declaration de variables internes */


/* Declaration de fonctions internes */


/* Definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne la liste des equipements d'un district et d'un type donnes,
*  ayant une disponibilite correspondant aux criteres indiques.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAT01_Liste_Eqt_Dispo (	XDY_TypeEqt	va_Type,
				XDY_District	va_District,
				XDY_Etat_Eqt	va_AjoutDispo,
				XDY_Etat_Eqt	va_AjoutNonDispo,
				XDY_Etat_Eqt	va_RetraitDispo,
				XDY_Etat_Eqt	va_RetraitNonDispo,
				XDY_Horodate	va_Horodate,
				XDY_FonctionInt	va_FonctionUtilisateur )

/*
------------------------------------------------------------------------------
*
* PARAMETRES EN ENTREE :
*
*  XDY_TypeEqt		va_Type
*  XDY_District		va_District
*  XDY_Etat_Eqt		va_AjoutDispo
*  XDY_Etat_Eqt		va_AjoutNonDispo
*  XDY_Etat_Eqt		va_RetraitDispo
*  XDY_Etat_Eqt		va_RetraitNonDispo
*  XDY_Horodate		va_Horodate
*  XDY_FonctionInt	va_FonctionUtilisateur
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
*  Liste des equipements avec leur etat et date de changement d'etat.
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
* Lit en base les equipements et appelle la fonction utilisateur pour chacun.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzat01.c,v 1.3 2021/06/15 17:25:27 devgfi Exp $ : XZAT01_Liste_Eqt_Dispo" ;

	CS_CHAR *rpc_name = XZATC_XZAT01_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	/*int 		num_Tab_donnees2 = XZATC_XZAT01_NB_PARM_RETURN;*/
       	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne = NULL;
	int		i;
	XDY_Entier	SPstatus;
	int		vl_cr;

	/*A
	** Definition des variables locales
	*/

	CS_TINYINT	vl_Type_in;
	CS_TINYINT	vl_District_in;
	CS_INT		vl_AjoutDispo_in;
	CS_INT		vl_AjoutNonDispo_in;
	CS_INT		vl_RetraitDispo_in;
	CS_INT		vl_RetraitNonDispo_in;
	CS_DATETIME	vl_Horodate_in;

	XZATT_EtatEqt	vl_EtatEqt;


	XZST_03EcritureTrace( XZSTC_INTERFACE, "IN : XZAT01_Liste_Eqt_Dispo : Type=%d, Secteur=%d, criteres %d/%d/%d/%d",
	                                          va_Type, va_District, va_AjoutDispo, va_AjoutNonDispo,
	                                          va_RetraitDispo, va_RetraitNonDispo );

	/*A
	** Verification de la validite des parametres d'entree
	*/

	if ( va_FonctionUtilisateur == NULL )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "va_FonctionUtilisateur est nulle");
		return (XDC_NOK);
	}


	/*A
	** Conversion des dates en entree du format Unix au format Sybase
	*/

	if ((vl_cr = asql_date_Ux2Sybase (va_Horodate, &vl_Horodate_in)) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "xzat01: asql_date_Ux2Sybase() a echoue");
		return (XDC_NOK);
	}

	vl_Type_in		= va_Type;
	vl_District_in		= va_District;
	vl_AjoutDispo_in	= va_AjoutDispo;
	vl_AjoutNonDispo_in	= va_AjoutNonDispo;
	vl_RetraitDispo_in	= va_RetraitDispo;
	vl_RetraitNonDispo_in	= va_RetraitNonDispo;


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzat01: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &vl_Type_in, "@va_Type_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_tinyint_input (Type) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_tinyint_input (cmd, (CS_TINYINT *) &vl_District_in, "@va_District_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_tinyint_input (District) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_AjoutDispo_in, "@va_AjoutDispo_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_int_input (AjoutDispo) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_AjoutNonDispo_in, "@va_AjoutNonDispo_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_int_input (AjoutNonDispo) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_RetraitDispo_in, "@va_RetraitDispo_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_int_input (RetraitDispo) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_int_input (cmd, (CS_INT *) &vl_RetraitNonDispo_in, "@va_RetraitNonDispo_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_int_input (RetraitNonDispo) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

        if ((retcode = asql_ctparam_date_input (cmd, (CS_DATETIME *) &vl_Horodate_in, "@va_Horodate_in")) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "xzat01: asql_ctparam_date_input (Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat01: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzat01: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzat01 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat01 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzat01 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne 1");
 						   asql_clean_cmd(connection, cmd);
 						   asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzat01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
			 			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzat01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);
		}
	}


	/*B
	** Appel de la fonction utilisateur pour chaque ligne retournee par la procedure stockee
	*/

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
		vl_EtatEqt.TypeEqt = * (XDY_TypeEqt *) vl_Tab_Ligne[i][0].pt_value;
		vl_EtatEqt.NumeroEqt = * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
		XDY_Copie_Sans_Espaces_De_Fin ( vl_EtatEqt.NomEqt, (char *) vl_Tab_Ligne[i][2].pt_value );
		vl_EtatEqt.NumAuto = * (XDY_Autoroute *) vl_Tab_Ligne[i][3].pt_value;
		vl_EtatEqt.PR = * (XDY_PR *) vl_Tab_Ligne[i][4].pt_value;
		vl_EtatEqt.Sens = * (XDY_Sens *) vl_Tab_Ligne[i][5].pt_value;
		vl_EtatEqt.EtatEqt = * (XDY_Etat_Eqt *) vl_Tab_Ligne[i][6].pt_value;

		if ((vl_cr = asql_date_Sybase2Ux (* (CS_DATETIME *) vl_Tab_Ligne[i][7].pt_value, &vl_EtatEqt.DateEtat)) != XDC_OK)
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "asql_date_Sybase2Ux () a echoue");
		}

		vl_EtatEqt.SiteGestion = * (XDY_District *) vl_Tab_Ligne[i][8].pt_value;

		vl_cr = (*va_FonctionUtilisateur) ( vl_Nb_Ligne_Lue, i, vl_EtatEqt );
		if ( vl_cr != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "appel de va_FonctionUtilisateur s'est mal deroule");
			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
			asql_clean_cmd(connection, cmd);
			return (XDC_NOK);
		}
	}


	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	*/

	retcode =  asql_Free_Tab_Ligne_Results (vl_Nb_Ligne_Lue, vl_Nb_Col, &vl_Tab_Ligne);
	if (retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAT01_Liste_Eqt_Dispo retourne %d", retcode);
	asql_clean_cmd(connection, cmd);

	return (XDC_OK);
}
