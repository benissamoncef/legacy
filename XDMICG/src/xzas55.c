/*E*/
/*Fichier : $Id: xzas55.c,v 1.4 2008/11/17 11:05:18 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2008/11/17 11:05:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzas55.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tasts
* permet d'extraire la liste des stations a exporter
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 836
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas55.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas55.c,v 1.4 2008/11/17 11:05:18 pc2dpdy Exp $ : xzas55" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS55_Lect_RADT_A_Exporter : recupere les stations RADT
* a exporter
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS55_Lect_RADT_A_Exporter (XDY_FonctionInt pa_FonctionUtilisateur_in,
				      XDY_Entier        *va_Resultat_out)
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_Station_in
* XDY_Entier		va_sens_in
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
* Extraire les donnees RADT pour la station souhaitee
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzas55.c,v 1.4 2008/11/17 11:05:18 pc2dpdy Exp $ : XZAS55_Lect_RADT_A_Exporter" ;
	
	CS_CHAR 	*pl_rpc_name = XZAS55C_RPC_NAME;
	
	CS_CONNECTION   *pl_connection=XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
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

	XDY_Mot		vl_numero;
	char		*vl_nom;
	XDY_Octet	vl_sens;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS55_Lect_RADT_A_Exporter : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzas55: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS55 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS55 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAS55_Lect_RADT_A_Exporter retourne 1 : %d", (int) vl_SPstatus);
 						   	asql_clean_cmd (pl_connection,pl_cmd);
							return (XDC_NOK);
						}
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55_Lect_RADT_A_Exporter retourne %d", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );
				}
	}

        /*A
        ** Appel de la fonction utilisateur pour chaque FMC
        */

        *va_Resultat_out = 0;

        for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
        {
           (*va_Resultat_out) ++;

           /*B
           * recup des parametres de retour
           */

           vl_numero                    = * (XDY_Mot *) vl_Tab_Ligne[i][0].pt_value;
           vl_sens                      = * (XDY_Octet *) vl_Tab_Ligne[i][1].pt_value;
           vl_nom                      	= * (XDY_Nom *) vl_Tab_Ligne[i][2].pt_value;
          
           /*B
           ** Appel de la fonction utilisateur FonctionUtilisateur
           */
           if ( pa_FonctionUtilisateur_in == NULL)
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"XZAS55 : pa_FonctionUtilisateur_in est nulle");
              asql_clean_cmd(pl_connection, pl_cmd);
              return (XDC_NOK);
           }
           else
           {
              if ( ( retcode = (*pa_FonctionUtilisateur_in) ( vl_numero,
                                                                vl_nom,
                                                                vl_sens,
								vl_Nb_Ligne_Lue,i,
                                                                *va_Resultat_out ) ) != XDC_OK )
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

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS55 retourne %d", retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);
}

