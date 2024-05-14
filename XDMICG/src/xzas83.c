/*E*/
/*Fichier : @(#)xzas83.c	1.1     Release : 1.1        Date :a 04/19/01 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzas83.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache taweb
* permet d'extraire la liste des mesures RDT pour TDPASF
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	20/10/04	creation 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas83.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzas83.c	1.3 12/03/97 : xzas83" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS83_Lire_Parcours (XDY_FonctionInt pa_FonctionUtilisateur_in)
/*
*
* PARAMETRES EN ENTREE :
*
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
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
*
* MECANISMES :
*
------------------------------------------------------*/

{
	CS_CHAR 	*pl_rpc_name = XZAS83C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	int		vl_num_Tab_donnees2 = XZAS83C_NB_PARM_RETURN;
	tg_Colonne_donnees2 Tab_donnees2[XZAS83C_NB_PARM_RETURN];
	tg_row_result 	vl_Tab_Ligne;
	int		vl_Nb_Ligne_Lue = 0;
	int		vl_Nb_Col = 0;
	int		i;

	XDY_Entier	vl_SPstatus;
	CS_DATETIME     vl_Horodate;
	XDY_Horodate	vl_HorodateFin;
	
	int 		vl_indice;
	XDY_Booleen	vl_Bool;
	int		retcode;

	char vl_identifiant[100];
	char	vl_a_diffuser[5];
	int	vl_temps;


	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS83_Lire_RDT : Debut d execution");

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result (pl_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzas83: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS83 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS83 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAS83_Lire_RDT retourne 1 : %d", (int) vl_SPstatus);
 						   	asql_clean_cmd (pl_connection,pl_cmd);
							return (XDC_NOK);
						}
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pl_cmd , 
							vl_num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,
								"XZAS83 : PARAMETER RESULTS\n");
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT retourne %d", vl_retcode);
						asql_clean_cmd (pl_connection,pl_cmd);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83_Lire_RDT retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
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
	   strcpy(vl_identifiant, (char *)vl_Tab_Ligne[i][0].pt_value);
	   strcpy (vl_a_diffuser, (char *) vl_Tab_Ligne[i][1].pt_value);
	   vl_temps 		= * (XDY_Entier *) vl_Tab_Ligne[i][2].pt_value;

	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83: id %s, a diffuser %s, temps %d",
					vl_identifiant,
					vl_a_diffuser,
					vl_temps);
           /*B
           ** Appel de la fonction utilisateur FonctionUtilisateur
           */
           if ( pa_FonctionUtilisateur_in == NULL)
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"XZAS83 : pa_FonctionUtilisateur_in est nulle");
              asql_clean_cmd(pl_connection, pl_cmd);
              return (XDC_NOK);
           }
           else
           {
              if ( ( retcode = (*pa_FonctionUtilisateur_in) ( vl_identifiant,
								vl_a_diffuser[0],
								vl_temps) ) != XDC_OK )
              {
                 XZST_03EcritureTrace(XZSTC_WARNING,"appel de pa_FonctionUtilisateur_in s'est mal deroule");
                 asql_clean_cmd(pl_connection, pl_cmd);
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
           asql_clean_cmd(pl_connection, pl_cmd);
           return (XDC_NOK);
        }


        /*A
        ** Retourne le resultat et les parametres de sorties renseignes
        */

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS83 retourne %d", retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);
}

