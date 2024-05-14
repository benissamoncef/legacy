/*E*/
/*  Fichier : $Id: xzao4001.c,v 1.1 2017/10/06 19:42:51 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/10/06 19:42:51 $
--------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao4001.c
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		26/09/17 : Creation d'apres XZAO400 dont elle constitue la suite (DEM 1236)  1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao4001.c,v 1.1 2017/10/06 19:42:51 devgfi Exp $ : xzao4001.c" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO4001SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO4001_Liste_Zones_De_Parcours (	XDY_Basedd	va_Basedd_in,
					XDY_District	va_Site_in,
					XDY_FonctionInt pa_FonctionUtilisateurZDP_in,
					XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_District		va_Site_in
* XDY_FonctionInt	pa_FonctionUtilisateurZDP_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des zones de parcours
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
* Pour chaque zone de parcours appel de la fonction utilisateur avec XDY_ZDP
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzao4001.c,v 1.1 2017/10/06 19:42:51 devgfi Exp $ : XZAO4001_Liste_Zones_De_Parcours" ;

	CS_CHAR *rpc_name = XZAOC_XZAO4001_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	tg_row_result 	vl_Tab_Ligne;
	int		i = 0;
	XDY_Entier	SPstatus;
	char vl_nom[100];

	/*A
	** Definition des variables locales
	*/

	XDY_ZDP	vl_Eqt;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO4001_Liste_Zones_De_Parcours : Basedd = %s",
			va_Basedd_in );


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: asql_ctparam_tinyint_input(Site) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao4001: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao4001 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao4001 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao4001 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao4001: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao4001: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao4001: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}


	/*A
	** Appel de la fonction utilisateur pour chaque ZDP
	*/

	*va_Resultat_out = 0;

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;

	   /*B
           ** Remplissage de la structure XDY_ZDP correspondant a la ligne renvoyee par la procedure
           */

           vl_Eqt.Numero 		= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
           vl_Eqt.Site			= * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
           strcpy(vl_Eqt.Libelle, (char *) vl_Tab_Ligne[i][2].pt_value);
           vl_Eqt.Autoroute		= * (XDY_Autoroute *) vl_Tab_Ligne[i][3].pt_value;
           strcpy(vl_Eqt.NomAutoroute, strtok((char *) vl_Tab_Ligne[i][4].pt_value, " "));
           vl_Eqt.Maitre 		= * (XDY_Eqt *) vl_Tab_Ligne[i][5].pt_value;
           vl_Eqt.PR_Debut		= * (XDY_PR *) vl_Tab_Ligne[i][6].pt_value;
           vl_Eqt.PR_Fin		= * (XDY_PR *) vl_Tab_Ligne[i][7].pt_value;
           vl_Eqt.Sens			= * (XDY_Sens *) vl_Tab_Ligne[i][8].pt_value;
           vl_Eqt.TDP_Normal		= * (XDY_Mot *) vl_Tab_Ligne[i][9].pt_value;
           vl_Eqt.TDP_Max		= * (XDY_Mot *) vl_Tab_Ligne[i][10].pt_value;
           vl_Eqt.TDP_Courant		= vl_Eqt.TDP_Normal;
           vl_Eqt.Seuil_Bas		= * (XDY_Mot *) vl_Tab_Ligne[i][11].pt_value;
           vl_Eqt.Seuil_Haut		= * (XDY_Mot *) vl_Tab_Ligne[i][12].pt_value;
           vl_Eqt.Type_Zone		= * (XDY_Booleen *) vl_Tab_Ligne[i][13].pt_value;
           vl_Eqt.Code_Zone		= * (XDY_Mot *) vl_Tab_Ligne[i][14].pt_value;
           strcpy(vl_nom, (char *) vl_Tab_Ligne[i][15].pt_value);

           /*B
	   ** Appel de la fonction utilisateur pour cette ZDP
	   */

	   if ( pa_FonctionUtilisateurZDP_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurZDP_in est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurZDP_in) ( vl_Nb_Ligne_Lue, i,
					vl_Eqt, vl_nom, *va_Resultat_out ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurZDP_in s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
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
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO4001_Liste_Zones_De_Parcours retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
