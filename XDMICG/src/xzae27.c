/*E*/
/*  Fichier : 	$Id: xzae27.c,v 1.10 1998/06/23 09:15:35 verdier Exp $      Release : $Revision: 1.10 $        Date : $Date: 1998/06/23 09:15:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae27.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       07 Sep 1994     : Creation
* BG		2 mai 1995	: suppr malloc tabdonnees2
* BG            2 mai 1995      : mise a jour nouveau client (1.7)
* BG            3 mai 1995      : ajout 1 param et mise a jour nom des parametre comme xzae45sp.prc (1.8)
* PV	       15 Jul 1996	: ajout param idem xzae45sp.prc (DEM76)
* CT	       05 Mai 1998	: correction nom de param de la proc et 
*                                 nouvelle mise en forme (1.9)
* PN		19 jun 1998	: Ajout presence_matiere_dangereuse dans les par. de retour (1653) 1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae27.h"

static char *version = "$Id: xzae27.c,v 1.10 1998/06/23 09:15:35 verdier Exp $ : xzae27" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE45SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE27_Decrire_Pb_Veh_PA (	XDY_Horodate 	va_Horodate,
				XDY_Evt 	va_NumEvt,
				XZAET_Vehicule 	*va_Vehicule )
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate 		va_Horodate
* XDY_Evt 		va_NumEvt
*
* ARGUMENTS EN SORTIE :
*
* XZAET_Vehicule 	va_Vehicule
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
{

	static char *version = "$Id: xzae27.c,v 1.10 1998/06/23 09:15:35 verdier Exp $ : XZAE27_Decrire_Pb_Veh_PA" ;

	CS_CHAR *rpc_name =XZAEC_XZAE27_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE27_NB_PARM_RETURN;
	int		i;
	tg_Colonne_donnees2 Tab_donnees2[XZAEC_XZAE27_NB_PARM_RETURN];
	XDY_Entier	SPstatus;
	
	/*A
	** Definition des variables intermediaires pour la conversion des structures XDY_Evt et XZAET_Vehicule
	*/
	
	XDY_Entier	va_NumEvt_in;
	XDY_Octet	va_CleEvt_in;
	
	XDY_Octet	va_Type_out;
	XDY_Octet	va_Passagers_out;
	XDY_Booleen	va_Bebe_out;
	XDY_Booleen	va_PersAgee_out;			
	XDY_Booleen	va_Animaux_out;
	XDY_Booleen	va_VehEscota_out;
	XDY_Octet	va_Chargement_out;
	XDY_MatDang	va_CodeMat_out; 


	XDY_Entier	va_Enleve_out;
	XDY_Octet       va_Site_out;
	XDY_TexteVeh	va_Texte_out;
	XDY_Mot         va_numveh_out;
	char            va_marque_out[26];
	char            va_type_out[26];
	char            va_couleur_out[26];
	char            va_immatriculation_out[26];
	XDY_Mot         va_code_pays_out;
	char            va_pays_out[26];
	char            va_nom_out[26];
	char            va_ville_out[26];
	char            va_adresse_out[76];
	char            va_code_danger_out[5];
	XDY_Mot         va_code_nature_panne_out;
	char            va_nature_panne_out[26];
	char            va_positionnement_out[26];
	XDY_Octet       va_vehicule_roulant_out;
	char            va_enlev_dem_par_out[26];
	XDY_Entier      va_numero_fiche_appel_out;
	XDY_Octet       va_pres_mat_dangereuse_out;
	XDY_Octet       va_fax_envoye_out;
	XDY_Mot		va_code_marque_out;
	XDY_Mot		va_code_couleur_out;
	XDY_Mot		va_code_enlev_dem_par_out;
	XDY_Octet       va_site_creation_out;
	XDY_Mot		va_nb_bebes_out;
	XDY_Mot		va_nb_agees_out;

	
	/*A
	** Definition des variables intermediaires pour la conversion des chaines du format Sybase au format Unix
	*/
	
	CS_DATETIME	va_Horodate_in;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	if ((retcode = asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_NumEvt_in = va_NumEvt.numero;
	va_CleEvt_in = va_NumEvt.cle;

	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE27_Decrire_Pb_Veh_PA : va_NumEvt_in=%d va_CleEvt_in=%d va_Days_in=%d va_Time_in=%d ",va_NumEvt_in,va_CleEvt_in, va_Horodate_in.dtdays,va_Horodate_in.dttime);
	
	
	/*A
	** Initialisation de la commande				
	*/
	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/

	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	
	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/
	if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&va_Horodate_in,"@va_Horodate_in"))!= CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
        
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(@va_NumEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(@va_CleEvt_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	
	i = 0;
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Type_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Type_out,"@va_Type_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Type_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Passagers_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Passagers_out,"@va_Passagers_ou")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Passagers_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Bebe_out;

	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_Bebe_out,"@va_Bebe_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Bebe_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_PersAgee_out;

	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_PersAgee_out, "@va_PersAgee_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_PersAgee_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Animaux_out;

	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_Animaux_out, "@va_Animaux_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Animaux_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_VehEscota_out;

	if ((retcode = asql_ctparam_bit_output(cmd,(CS_BIT *)&va_VehEscota_out,"@va_TypeVehEscota_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_VehEscota_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Chargement_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Chargement_out,"@va_Chargement_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Chargement_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_CodeMat_out;

	if ((retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_CodeMat_out, "@va_CodeMat_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_CodeMat_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Enleve_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_Enleve_out,"@va_Depannage_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Depannage_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;
	
	Tab_donnees2[i].pt_value = (CS_VOID *)&va_Site_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_Site_out,"@va_SiteDepannage_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_Site_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_Texte_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_Texte_out,"@va_Texte_out")) != CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_Texte_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_numveh_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_numveh_out,"@va_numveh_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_numveh_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_marque_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_marque_out,"@va_marque_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_marque_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_type_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_type_out,"@va_type_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_type_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_couleur_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_couleur_out,"@va_couleur_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_couleur_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	
	Tab_donnees2[i].pt_value = (CS_VOID *)va_immatriculation_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_immatriculation_out,"@va_immatriculation_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_immatriculation_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_code_pays_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_code_pays_out,"@va_code_pays_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_code_pays_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_pays_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_pays_out,"@va_pays_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_pays_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_nom_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_nom_out,"@va_nom_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_nom_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_ville_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_ville_out,"@va_ville_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_ville_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_adresse_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_adresse_out,"@va_adresse_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_adresse_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_code_danger_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_code_danger_out,"@va_code_danger_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_code_danger_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_code_nature_panne_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_code_nature_panne_out,"@va_code_nature_panne_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_code_nature_panne_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_nature_panne_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_nature_panne_out,"@va_nature_panne_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_nature_panne_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_positionnement_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_positionnement_out,"@va_positionnement_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_positionnement_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_vehicule_roulant_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_vehicule_roulant_out,"@va_vehicule_roulant_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_vehicule_roulant_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	Tab_donnees2[i].pt_value = (CS_VOID *)va_enlev_dem_par_out;

	if ((retcode = asql_ctparam_char_output(cmd, (CS_CHAR *)va_enlev_dem_par_out,"@va_enlev_dem_par_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_ct_param_char_output(va_enlev_dem_par_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_numero_fiche_appel_out;

	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)&va_numero_fiche_appel_out,"@va_numero_fiche_appel_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_numero_fiche_appel_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_pres_mat_dangereuse_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_pres_mat_dangereuse_out,"@va_pres_mat_dangereuse_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_pres_mat_dangereuse_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_fax_envoye_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_fax_envoye_out,"@va_fax_envoye_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_fax_envoye_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_code_marque_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_code_marque_out,"@va_code_marque_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_code_marque_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_code_couleur_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_code_couleur_out,"@va_code_couleur_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_code_couleur_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_code_enlev_dem_par_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_code_enlev_dem_par_out,"@va_code_enlev_dem_par_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_code_enlev_dem_par_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_site_creation_out;

	if ((retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)&va_site_creation_out,"@va_site_creation_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_site_creation_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_nb_bebes_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_nb_bebes_out,"@va_nb_bebes_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_nb_bebes_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;

	Tab_donnees2[i].pt_value = (CS_VOID *)&va_nb_agees_out;

	if ((retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)&va_nb_agees_out,"@va_nb_agees_out")) != CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_param(va_nb_agees_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}
	i = i + 1;


	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzae27 : ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae27 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae27 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae27 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}

				if (retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : TEXTE = %s CODE MATIERE = %s", va_Texte_out, va_CodeMat_out);
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"%s: ct_result retourne CS_MSG_RESULT l id du message est : %d.\n",version, msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"%s: CS_CMD_SUCCEED",version);
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"%s : CS_CMD_DONE",version);
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae27: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	
	/*A
	** Affectation des variables intermediaires pour la conversion des structures XDY_Evt
	*/
	
	va_Vehicule->Type       = va_Type_out;
	va_Vehicule->Passagers  = va_Passagers_out;
	va_Vehicule->Bebe       = va_Bebe_out;
	va_Vehicule->PersAgee   = va_PersAgee_out;
	va_Vehicule->Animaux    = va_Animaux_out;
	va_Vehicule->VehEscota  = va_VehEscota_out;
	va_Vehicule->Chargement = va_Chargement_out;
	strcpy(va_Vehicule->CodeMat, va_CodeMat_out);
	va_Vehicule->Enleve     = va_Enleve_out;
	strcpy(va_Vehicule->Texte, va_Texte_out);
	va_Vehicule->Presdang = va_pres_mat_dangereuse_out;
	
	/*XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : CODE MATIERE = %s.", va_CodeMat_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_marque_out = %s",va_marque_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_type_out = %s",va_type_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_couleur_out = %s",va_couleur_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_immatriculation_out = %s",va_immatriculation_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_pays_out = %s",va_pays_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_nom_out = %s",va_nom_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_ville_out = %s",va_ville_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_adresse_out = %s",va_adresse_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_code_danger_out = %s",va_code_danger_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_nature_panne_out = %s",va_nature_panne_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_positionnement_out = %s",va_positionnement_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_enlev_dem_par_out = %s",va_enlev_dem_par_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : va_pres_mat_dangereuse_out = %d.",va_pres_mat_dangereuse_out);
	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE27 : TEXTE = %s",va_Texte_out);
*/
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE27_Decrire_Pb_Veh_PA retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}
