/*E*/
/*Fichier : $Id: xzao123.c,v 1.7 2021/06/15 17:25:19 devgfi Exp $      $Revision: 1.7 $        $Date: 2021/06/15 17:25:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao123.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       	31 Mai 1995     : Creation
* volcic        29 Nov 1995     : Modif allocation Tab_donees2 (v1.2)
* JPL		05/02/2013	: Fonction interne et deux publiques (remplace xzao316)  1.5
* JPL		22/02/2013	: Retour NOK si parametre d'entree invalide  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao123.c,v 1.7 2021/06/15 17:25:19 devgfi Exp $ : xzao123.c" ;

/* Declaration de variables internes */

static	XZAOT_Autoroute		vm_Auto_Nulle = { 0, "" };

static	XZAOT_Autoroute *	tm_Autoroutes = NULL;

static	XDY_Entier		vm_Nb_Autoroutes = 0;


/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonction interne d'interface avec la base de donnees.
*  Lit la liste des autoroutes dans la base specifiee,
*  et conserve la liste retournee en memoire locale au module.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static	int XZAO123_Lire_Autoroutes (	XDY_Basedd	va_Basedd_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
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
* Lit en base la liste des autoroutes et la copie en local.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao123.c,v 1.7 2021/06/15 17:25:19 devgfi Exp $ : XZAO123_Lire_Autoroutes" ;

	CS_CHAR *rpc_name = XZAOC_XZAO123_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	/*int 		num_Tab_donnees2 = XZAOC_XZAO123_NB_PARM_RETURN;*/
       	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier	SPstatus;

	/*A
	** Definition des variables locales
	*/

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO123_Lire_Autoroutes : Basedd = %s", va_Basedd_in );


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
		return (XDC_NOK);
	}




	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: asql_ctparam_char_input(Basedd) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
 

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao123: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao123 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao123 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao123 : STATUS RESULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
			 		asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
					return (XDC_NOK);
				}
	}


	/*B
	** Recuperation des donnees retournees par la procedure stockee
	*/

	vm_Nb_Autoroutes = vl_Nb_Ligne_Lue;

	if (vl_Nb_Ligne_Lue == 0)
	{
		tm_Autoroutes = &vm_Auto_Nulle;
	}
	else
	{
		/*A
		** Allocation de la memoire pour conserver la liste d'autoroutes
		*/

		tm_Autoroutes = (XZAOT_Autoroute *) calloc ((size_t) vl_Nb_Ligne_Lue, sizeof(XZAOT_Autoroute));
		if (tm_Autoroutes == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzao123: allocation memoire a echoue");
			tm_Autoroutes = &vm_Auto_Nulle;
			vm_Nb_Autoroutes = 0;

			asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
			asql_clean_cmd(connection, cmd);
			return (XDC_NOK);
		}

		/*B
		** Copie de la liste retournee par la procedure stockee
		*/

		for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
		{
			tm_Autoroutes[i].numero	= * (XDY_Autoroute *) vl_Tab_Ligne[i][0].pt_value;
			XDY_Copie_Sans_Espaces_De_Fin ( tm_Autoroutes[i].nom, (char *) vl_Tab_Ligne[i][1].pt_value );
			tm_Autoroutes[i].inversee = * (XDY_Mot *) vl_Tab_Ligne[i][2].pt_value;
			tm_Autoroutes[i].escota = * (XDY_Octet *) vl_Tab_Ligne[i][3].pt_value;
			tm_Autoroutes[i].debut = * (XDY_PR *) vl_Tab_Ligne[i][4].pt_value;
			tm_Autoroutes[i].fin = * (XDY_PR *) vl_Tab_Ligne[i][5].pt_value;
			tm_Autoroutes[i].debut_concession = * (XDY_PR *) vl_Tab_Ligne[i][6].pt_value;
			tm_Autoroutes[i].fin_concession = * (XDY_PR *) vl_Tab_Ligne[i][7].pt_value;
		}
	}


	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	*/

	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

	if (retcode != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Lire_Autoroutes retourne %d", retcode);
	asql_clean_cmd(connection, cmd);

	return (XDC_OK);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Lit en base de donnees la liste des autoroutes.
*  Puis appelle la fonction utilisateur pour chaque autoroute.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO123_Liste_Autoroute (	XDY_Basedd	va_Basedd_in,
				XDY_FonctionInt va_FonctionUtilisateur_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
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
* Pour chaque autoroute appel de la fonction utilisateur.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao123.c,v 1.7 2021/06/15 17:25:19 devgfi Exp $ : XZAO123_Liste_Autoroute" ;

	int		retcode;
	int		i;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO123_Liste_Autoroute : Basedd = %s", va_Basedd_in );


	/*A
	** Test des parametres d'entree
	*/

	if ( va_FonctionUtilisateur_in == NULL )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "va_FonctionUtilisateur_in est nulle");
		return (XDC_NOK);
	}


	/*A
	** Lecture des donnees dans la base indiquee
	*/

	retcode = XZAO123_Lire_Autoroutes (va_Basedd_in);
	if (retcode != XDC_OK)
	{
		return (XDC_NOK);
	}



	/*A
	** Appel de la fonction utilisateur pour chaque autoroute
	*/

	for ( i = 0; i < vm_Nb_Autoroutes; i++ )
	{
		retcode = (*va_FonctionUtilisateur_in) ( tm_Autoroutes[i].numero, tm_Autoroutes[i].nom );
		if ( retcode != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "appel de va_FonctionUtilisateur_in s'est mal deroule");
			free (tm_Autoroutes);
			tm_Autoroutes = NULL;
			vm_Nb_Autoroutes = 0;
			return (XDC_NOK);
		}
	}


	/*A
	** Desallocation du tableau de donnees renvoye par la SP
	*/

	free (tm_Autoroutes);
	tm_Autoroutes = NULL;
	vm_Nb_Autoroutes = 0;


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Liste_Autoroute retourne %d", retcode);
	return (XDC_OK);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Lit en base de donnees la liste des autoroutes.
*  Puis appelle la fonction utilisateur une fois avec la liste d'autoroutes.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO123_Liste_Complete_Autoroutes (	XDY_Basedd	va_Basedd_in,
					XDY_FonctionInt va_FonctionUtilisateur_in )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
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
* Appel de la fonction utilisateur avec la table d'autoroutes.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao123.c,v 1.7 2021/06/15 17:25:19 devgfi Exp $ : XZAO123_Liste_Complete_Autoroutes" ;

	int		retcode;
	int		i;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO123_Liste_Complete_Autoroutes : Basedd = %s",
			va_Basedd_in );


	/*A
	** Test des parametres d'entree
	*/

	if ( va_FonctionUtilisateur_in == NULL )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "va_FonctionUtilisateur_in est nulle");
		return (XDC_NOK);
	}


	/*A
	** Lecture des donnees dans la base indiquee
	*/

	retcode = XZAO123_Lire_Autoroutes (va_Basedd_in);
	if (retcode != XDC_OK)
	{
		return (XDC_NOK);
	}



	/*A
	** Appel de la fonction utilisateur avec la liste d'autoroutes
	*/

	retcode = (*va_FonctionUtilisateur_in) ( tm_Autoroutes, (XDY_Entier) vm_Nb_Autoroutes );
	if ( retcode != XDC_OK )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "appel de va_FonctionUtilisateur_in s'est mal deroule");
		free (tm_Autoroutes);
		tm_Autoroutes = NULL;
		vm_Nb_Autoroutes = 0;
		return (XDC_NOK);
	}


	/*A
	** Desallocation du tableau de donnees renvoye par la SP
	*/

	free (tm_Autoroutes);
	tm_Autoroutes = NULL;
	vm_Nb_Autoroutes = 0;


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO123_Liste_Complete_Autoroutes retourne %d", retcode);
	return (XDC_OK);
}
