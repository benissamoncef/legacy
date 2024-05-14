/*E*/
/* Fichier : $Id: xzao403.c,v 1.4 2018/11/07 19:40:09 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/07 19:40:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao403.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       10 Fev 1997     : Creation (DEM/1395)
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression de "free" avant "malloc"  1.2
* JPL		25/09/17 : Type du champ 'Code' change en XDY_Mot, parametre Code_Zone_in smallint (DEM 1236)  1.3
* JPL		07/11/18 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao403.c,v 1.4 2018/11/07 19:40:09 devgfi Exp $ : xzao403" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO403SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO403_Ajouter_Eqt_ZDP (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfEqtZDP	va_EqtZDP_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		Nom
* XDY_NomAuto		Autoroute
* XDY_PR          	PR_Debut
* XDY_PR          	PR_Fin
* XDY_Sens		Sens
* XDY_NomEqt		NomServeur
* XDY_Mot		Code
* XDY_NomSite         	Site
* XDY_Nom         	Libelle
* XDY_TDP         	TDP_Normal
* XDY_TDP         	TDP_Max
* XDY_Entier      	Seuil_Bas
* XDY_Entier      	Seuil_Haut
* XDY_Booleen     	Type_Zone
* XDY_Booleen		Inhibition
* XDY_Booleen		Supprime
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'un equipement ZDP (Zone Temps de parcours) en base
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
* Insertion ou modification dans la table EQT_ZDP (Zone Temps de parcours)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "$Id: xzao403.c,v 1.4 2018/11/07 19:40:09 devgfi Exp $ : XZAO403_Ajouter_Eqt_ZDP" ;

	CS_CHAR *rpc_name = XZAOC_XZAO403_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO403_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_Nom;
	XDY_NomAuto	vl_Autoroute;
        XDY_PR          vl_PR_Debut;
        XDY_PR          vl_PR_Fin;
	XDY_Sens	vl_Sens;
   	XDY_NomEqt	vl_NomServeur;
   	XDY_Mot		vl_Code_Zone;
        XDY_NomSite     vl_Site;
        XDY_Nom         vl_Libelle;
        XDY_Mot         vl_TDP_Normal;
        XDY_Mot         vl_TDP_Max;
        XDY_Mot      	vl_Seuil_Bas;
        XDY_Mot     	vl_Seuil_Haut;
        XDY_Booleen     vl_Type_Zone;
        XDY_Booleen	vl_Inhibition;
        XDY_Booleen	vl_Supprime;
	
	
	
   	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_Nom, va_EqtZDP_in.Nom );
	strcpy( vl_Autoroute, va_EqtZDP_in.Autoroute );
	vl_PR_Debut	= va_EqtZDP_in.PR_Debut;
	vl_PR_Fin	= va_EqtZDP_in.PR_Fin;
	vl_Sens		= va_EqtZDP_in.Sens;
	strcpy( vl_NomServeur, va_EqtZDP_in.NomServeur );
	vl_Code_Zone	= va_EqtZDP_in.Code;
	strcpy( vl_Site, va_EqtZDP_in.Site );
	strcpy( vl_Libelle, va_EqtZDP_in.Libelle );
	vl_TDP_Normal	= va_EqtZDP_in.TDP_Normal;
	vl_TDP_Max	= va_EqtZDP_in.TDP_Max;
	vl_Seuil_Bas	= va_EqtZDP_in.Seuil_Bas;
	vl_Seuil_Haut	= va_EqtZDP_in.Seuil_Haut;
	vl_Type_Zone	= va_EqtZDP_in.Type_Zone;
	vl_Inhibition	= va_EqtZDP_in.Inhibition;
	vl_Supprime	= va_EqtZDP_in.Supprime;
	
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO403_Ajouter_Eqt_ZDP : Nom = %s\tAutoroute = %s\tPRdeb = %d\tPRfin = %d\tSens = %d\tServeur:%s\t\n\
		Code : %d\tSite :%s\tLibelle : <%s>\tNormal : %d\tMAx : %d\tBAS : %d\tHAut : %d\tType: %d\tIni : %d\n",
			va_EqtZDP_in.Nom,
			va_EqtZDP_in.Autoroute,
			va_EqtZDP_in.PR_Debut,
			va_EqtZDP_in.PR_Fin,
			va_EqtZDP_in.Sens,
			va_EqtZDP_in.NomServeur,
			va_EqtZDP_in.Code,
			va_EqtZDP_in.Site,
			va_EqtZDP_in.Libelle,
			va_EqtZDP_in.TDP_Normal,
			va_EqtZDP_in.TDP_Max,
			va_EqtZDP_in.Seuil_Bas,
			va_EqtZDP_in.Seuil_Haut,
			va_EqtZDP_in.Type_Zone,
			va_EqtZDP_in.Inhibition
			 );
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_char_input(Base) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Nom,"@va_NomEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_char_input(NomEqt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_char_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_Maitre_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_char_input(Maitre) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Site,"@va_Site_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_char_input(Site) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR_Debut,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_int_input(PR_Debut) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Libelle,"@va_Libelle_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_char_input(Libelle) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR_Fin,"@va_PR_Fin_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_int_input(PR_Fin) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_Code_Zone,"@va_Code_Zone_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_smallint_input(Code Zone) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
        if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_TDP_Normal,"@va_TDP_habituel_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_samallint_input(TDP_Normal) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
        if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_TDP_Max,"@va_TDP_max_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_samallint_input(TDP_Max) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
        if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_Seuil_Bas,"@va_Tolerance_basse_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_samallint_input(Seuil_Bas) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
        if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_Seuil_Haut,"@va_Tolerance_haute_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_samallint_input(Seuil_Haut) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
        if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Type_Zone,"@va_Type_zone_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao403:asql_ctparam_bit_input(Type_zone)  a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
        if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Inhibition,"@va_Inhibition_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao403:asql_ctparam_bit_input(Inhibition)  a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           return (XDC_NOK);
        }
	
	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	
		
	
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao403: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao403 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao403 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao403 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao403 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao403 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao403 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao403 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao403: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO403_Ajouter_Eqt_ZDP retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




