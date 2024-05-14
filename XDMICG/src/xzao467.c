/*E*/
/* Fichier : $Id: xzao467.c,v 1.1 2012/11/08 09:52:11 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/11/08 09:52:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao467.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05/06/12	: creation (DEM/1014 PMA)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao467.h"

static char *version = "@(#)xzao467.c	1.2 1.2 15/11/11 : xzao467.c" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO467SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO467_Lire_Liste_PMA (	XDY_Basedd	va_Basedd_in,
				XDY_NomEqt  	va_NomMachine_in,
				XDY_FonctionInt pa_FonctionUtilisateurPMA_in,
				XDY_Entier 	*va_Resultat_out )

/*
A
*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in
* XDY_Machine	va_NomMachine_in
* XDY_Fonction  pa_FonctionUtilisateurPMA_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des PMVA de la Machine
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
* Pour chaque PMVA appel de la fonction utilisateur avec XZAOT_PMA
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzao467.c	1.3 15/11/11 : XZAO467_Lire_Liste_PMA" ;

	CS_CHAR *rpc_name = XZAOC_XZAO467_RPC_NAME;
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
	int		i;
	XDY_Entier	SPstatus;

	/*A
	** Definition des variables locales
	*/

	XZAOT_PMA_DEST_TDP	vl_PMA;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO467_Lire_Liste_PMA : Basedd = %s\tMachine = %s",
			va_Basedd_in,
			va_NomMachine_in );


	/*A
	** Initialisation de la commande
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}


	/*A
	** Initialisation de la structure pour chacun des parametres d I/O
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)

	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_NomMachine_in,"@va_NomMachine_in"))!= CS_SUCCEED)

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: asql_ctparam_char_input(NomMachine) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}



	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao467: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzao467 : PARAMETER RESULTS");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : CS_STATUS_RESULT = %d", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao117 : STATUS REUSULT PROCEDURE STOCKEE = %d", SPstatus);

						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO117_Lire_SystemeRAU retourne 1");
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao467: ct_results retourne CS_CMD_SUCCEED.");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao467: ct_results retourne CS_CMD_DONE.");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao467: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}


	/*A
	** Appel de la fonction utilisateur FonctionUtilisateurPMA pour
	** chaque type de Picto
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,"lignes lues ");

	*va_Resultat_out = 0;

	XZST_03EcritureTrace(XZSTC_WARNING,"lignes lues %d",vl_Nb_Ligne_Lue);

	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
           (*va_Resultat_out) ++;

	   /*B
           ** Remplissage de la structure XZAOT_PMA correspondant a la ligne resultat
           ** du select de la procedure stockee XZAO467
           */

           vl_PMA.NumeroPMA 		= * (XDY_Eqt *) vl_Tab_Ligne[i][0].pt_value;
		   vl_PMA.Num_Destination_un_proche 	= * (XDY_Eqt *) vl_Tab_Ligne[i][1].pt_value;
           strcpy(vl_PMA.Nom_Destination_un_proche, (char *) vl_Tab_Ligne[i][2].pt_value);
		   vl_PMA.Num_Destination_un_lointaine 	= * (XDY_Eqt *) vl_Tab_Ligne[i][3].pt_value;
		   strcpy(vl_PMA.Nom_Destination_un_lointaine, (char *) vl_Tab_Ligne[i][4].pt_value);
		   vl_PMA.Num_Destination_deux_proche 	= * (XDY_Eqt *) vl_Tab_Ligne[i][5].pt_value;
		   strcpy(vl_PMA.Nom_Destination_deux_proche, (char *) vl_Tab_Ligne[i][6].pt_value);
		   vl_PMA.Num_Destination_deux_lointaine 	= * (XDY_Eqt *) vl_Tab_Ligne[i][7].pt_value;
		   strcpy(vl_PMA.Nom_Destination_deux_lointaine, (char *) vl_Tab_Ligne[i][8].pt_value);


           /*B
	   ** Appel de la fonction utilisateur FonctionUtilisateurPMA
	   */

	   if ( pa_FonctionUtilisateurPMA_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"pa_FonctionUtilisateurPMA est nulle");
	      asql_clean_cmd(connection, cmd);
	      return (XDC_NOK);
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtilisateurPMA_in) ( vl_PMA, *va_Resultat_out ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateurPMA s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	         return (XDC_NOK);
	      }
	   }

	}




	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/
	if (vl_Nb_Ligne_Lue>0) {
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);

	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	}

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/

 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO467_Lire_Liste_PMA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




