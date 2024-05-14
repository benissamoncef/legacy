/*E*/
/*Fichier : $Id: xzas70.c,v 1.2 2001/10/30 18:30:45 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2001/10/30 18:30:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE STRADA * FICHIER xzas70.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Appartient a BASEDD tache tasts
* permet d'extraire la liste des stations a exporter
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	05 Jan 1998	: Creation 
* Niepceron     09 Aou 1999	: Ajout de la version rds dem/1767 v1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas70.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzas70.c	1.2 09/03/99 : xzas70" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS70_Loc_RADT_A_Exporter : recupere les localisation des stations RADT
* a exporter
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS70_Loc_RADT_A_Exporter (XDY_FonctionInt pa_FonctionUtilisateur_in,
				      XDY_Entier        *va_Resultat_out)
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
* Extraire les donnees RADT pour la station souhaitee
*
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "@(#)xzas70.c	1.2 09/03/99 : XZAS70_Lect_RADT_A_Exporter" ;
	
	CS_CHAR 	*pl_rpc_name = XZAS70C_RPC_NAME;
	
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
	char		vl_nom[36];
	char		vl_sens[2];
	char		vl_table[4];
	char            vl_version[36];
	char		vl_lib_sens[36];
	int		vl_loc_rds,vl_distance;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAS70_Lect_RADT_A_Exporter : Debut d execution" );

	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &pl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}
	
	/*A
	** Envoie de la commande RMET pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(pl_cmd, CS_RPC_CMD, pl_rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter retourne %d", vl_retcode);
		asql_clean_cmd (pl_connection,pl_cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(pl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter retourne %d", vl_retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzas70: ROW RESULTS : %d", vl_Nb_Ligne_Lue);
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS70 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"XZAS70 : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter retourne 1");
						   	XZST_03EcritureTrace(XZSTC_WARNING, "OUT : XZAS70_Lect_RADT_A_Exporter retourne 1 : %d", (int) vl_SPstatus);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter retourne %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter Fin d execution", vl_retcode);
					asql_clean_cmd (pl_connection,pl_cmd);
					return ( XDC_NOK );

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70_Lect_RADT_A_Exporter retourne %d", vl_retcode);
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
           strncpy ( vl_nom, (char *) vl_Tab_Ligne[i][1].pt_value,35 );
           vl_nom[35]='\0';
            strncpy ( vl_table, (char *) vl_Tab_Ligne[i][2].pt_value,3 );
           vl_table[3]='\0';
	   strncpy ( vl_version, (char *) vl_Tab_Ligne[i][3].pt_value,35 );
	   vl_version[35]='\0';
           vl_loc_rds                    = * (XDY_Entier *) vl_Tab_Ligne[i][4].pt_value;
           vl_distance                   = * (XDY_Entier *) vl_Tab_Ligne[i][5].pt_value;
           strncpy ( vl_sens, (char *) vl_Tab_Ligne[i][6].pt_value,1 );
           vl_sens[1]='\0';
           strncpy ( vl_lib_sens, (char *) vl_Tab_Ligne[i][7].pt_value,35 );
           vl_lib_sens[35]='\0';
         
           /*B
           ** Appel de la fonction utilisateur FonctionUtilisateur
           */
           if ( pa_FonctionUtilisateur_in == NULL)
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"XZAS70 : pa_FonctionUtilisateur_in est nulle");
              asql_clean_cmd(pl_connection, pl_cmd);
              return (XDC_NOK);
           }
           else
           {
              if ( ( retcode = (*pa_FonctionUtilisateur_in) ( vl_numero,
                                                                vl_nom,
                                                                vl_table,
								vl_version,
                                                                vl_loc_rds,
                                                                vl_distance,
                                                                vl_sens,
                                                                vl_lib_sens,
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

        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAS70 retourne %d", retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
        return(XDC_OK);
}

