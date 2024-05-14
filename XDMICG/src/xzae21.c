/*E*/
/*Fichier : $Id: xzae21.c,v 1.15 2017/10/10 08:28:29 pc2dpdy Exp $      Release : $Revision: 1.15 $        Date : $Date: 2017/10/10 08:28:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzae21.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       31 Aug 1994     : Creation
* volcic       13 Oct 1994     : Modification du test cmd_alloc (v1.4)
* volcic       13 Oct 1994     : Modification entete (v1.5)
* volcic       14 Oct 1994     : Ajout 9 Parametres de sorties (v 1.6)
* volcic       20 Nov 1994     : Rien (v 1.7)
* volcic       22 Nov 1994     : Modif Parametres de sorties (v 1.8)
* volcic       22 Nov 1994     : Modif Parametres de sorties (v 1.9)
* torregrossa  22 Nov 1995     : Ajout Parametre de sortie (v 1.10)
* guilhou	12 dec 1997	: ajout Pr bifurc amont et aval en sortie (dem/1534) 1.11
* JMG		29/05/12	: portage v15 1.12
* JPL		26/06/12	: Gestion sorties usager et indicateur bifurcation en amont et aval (DEM 1033)  1.13
* LCL		15/11/16	: Ajout parametre de sortie viaduc (LOT23) 1.14
* JMG		26/06/17	: zone urbaine convergence 1.15
------------------------------------------------------*/

/* fichiers inclus */

#include "xzae21.h"

static char *version = "$Id: xzae21.c,v 1.15 2017/10/10 08:28:29 pc2dpdy Exp $ : xzae21" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE21SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE21_Localiser_Evt_PA (	XDY_Autoroute	va_Autoroute_in,
				XDY_PR		va_PR_in,
				XDY_Sens	va_Sens_in,
				XDY_Octet 	*va_NumSortieAmont_out,
				XDY_Entier 	*va_DistanceSortieAmont_out,
				XDY_Octet 	*va_SortieAmontEstBifurc_out,
				XDY_Octet 	*va_NumSortieAval_out,
				XDY_Entier 	*va_DistanceSortieAval_out,
				XDY_Octet 	*va_SortieAvalEstBifurc_out,
				XDY_Octet 	*va_NumPeageAmont_out,
				XDY_Entier 	*va_DistancePeageAmont_out,
				XDY_Octet 	*va_NumPeageAval_out,
				XDY_Entier 	*va_DistancePeageAval_out,
				XDY_Entier 	*va_LongueurPlateforme_out,
				XDY_Autoroute 	*va_NumAutorouteBifAmont_out,
				XDY_Entier 	*va_DistanceBifAmont_out,
				XDY_Autoroute 	*va_NumAutorouteBifAval_out,
				XDY_Entier 	*va_DistanceBifAval_out,
				XDY_Nom 	va_Descente_out,
				XDY_Nom 	va_Tunnel_out,
				XDY_Nom		va_NomSortieAm_out,
				XDY_Abrev	va_AbrevSortieAm_out,
				XDY_Sortie	va_SortieAmUsager_out,
				XDY_Nom		va_NomSortieAv_out,
				XDY_Abrev	va_AbrevSortieAv_out,
				XDY_Sortie	va_SortieAvUsager_out,
				XDY_Nom		va_NomPeageAm_out,
				XDY_Abrev	va_AbrevPeageAm_out,
				XDY_Nom		va_NomPeageAval_out,
				XDY_Abrev	va_AbrevPeageAval_out,
				XDY_Mot		*va_Pente_out, 
				XDY_Octet	*va_Site_out,
				XDY_PR		*va_PrBifAmont_out,
				XDY_PR		*va_PrBifAval_out,
				XDY_PR		*va_PrDebutBifAmont_out,
				XDY_PR		*va_PrDebutBifAval_out,
				XDY_Nom		va_Viaduc_out,
				XDY_Mot		*va_ZoneUrbaine_out)

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in,
* XDY_Sens	va_Sens_in,XDY_Autoroute
*
* ARGUMENTS EN SORTIE :
*
* XDY_Octet 	va_NumPeageAval_out
* XDY_Entier 	va_DistanceSortieAmont_out
* XDY_Octet 	va_SortieAmontEstBifurc_out
* XDY_Octet 	va_NumSortieAval_out
* XDY_Entier 	va_DistanceSortieAval_out
* XDY_Octet 	va_SortieAvalEstBifurc_out
* XDY_Octet 	va_NumPeageAmont_out
* XDY_Entier 	va_DistancePeageAmont_out
* XDY_Octet 	va_NumPeageAval_out
* XDY_Entier 	va_DistancePeageAval_out
* XDY_Entier 	va_LongueurPlateforme_out
* XDY_Autoroute va_NumAutorouteBifAmont_out
* XDY_Entier 	va_DistanceBifAmont_out
* XDY_Autoroute va_NumAutorouteBifAval_out
* XDY_Entier 	va_DistanceBifAval_out
* XDY_Nom 	va_Descente
* XDY_Nom 	va_Tunnel
* XDY_Nom 	va_NomSortieAm_out
* XDY_Abrev 	va_AbrevSortieAm_out
* XDY_Sortie 	va_SortieAmUsager_out
* XDY_Nom 	va_NomSortieAv_out
* XDY_Abrev 	va_AbrevSortieAv_out
* XDY_Sortie 	va_SortieAvUsager_out
* XDY_Nom 	va_NomPeageAm_out
* XDY_Abrev 	va_AbrevPeageAm_out
* XDY_Nom 	va_NomPeageAval_out
* XDY_Abrev 	va_AbrevPeageAval_out
* XDY_Mot 	va_Pente
* XDY_Site 	va_Site
* XDY_PR 	va_PrBifAmont_out
* XDY_PR 	va_PrBifAval_out
* XDY_PR 	va_PrDebutBifAmont_out
* XDY_PR 	va_PrDebutBifAval_out
* XDY_Nom	va_Viaduc_out
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
*		*
*	      *   *
*	     *  |  *
*           *   |   *
*          *         *
*         *     *     *
*        *             *
*       *****************
*	Attention : Cette fonction n'a pas ete testee avec
*		    la procedure stockee .
------------------------------------------------------*/
{

	static char *version = "$Id: xzae21.c,v 1.15 2017/10/10 08:28:29 pc2dpdy Exp $ : XZAE21_Localiser_Evt_PA";

	CS_CHAR *rpc_name = XZAEC_XZAE21_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_RETCODE      vl_retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAEC_XZAE21_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	vl_SPstatus;
	int             vl_Nb_Ligne_Lue = 0;
	int             vl_Nb_Col = 0;
	tg_row_result   vl_Tab_Ligne = 0;
	int i;

	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAE21_Localiser_Evt_PA : va_Autoroute_in=%d va_PR_in=%d va_Sens_in=%d",va_Autoroute_in,va_PR_in,va_Sens_in);
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
	
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Autoroute_in,"@va_Autoroute_in"))!= CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_param(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
        
        

	if ((vl_retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_PR_in,"@va_PR_in"))!= CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_input(@va_PR_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((vl_retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Sens_in,"@va_Sens_in"))!= CS_SUCCEED)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_input(@va_Sens_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
        
	Tab_donnees2[0].pt_value = (CS_VOID *)va_NumSortieAmont_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumSortieAmont_out,"@va_NumSortieAmont_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(va_NumSortieAmont_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[1].pt_value = (CS_VOID *)va_DistanceSortieAmont_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceSortieAmont_out,"@va_DistanceSortieAmont_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_DistanceSortieAmont_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}


	Tab_donnees2[2].pt_value = (CS_VOID *)va_SortieAmontEstBifurc_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_SortieAmontEstBifurc_out,"@va_SortieAmontEstBifurc_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(SortieAmontEstBifurc) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[3].pt_value = (CS_VOID *)va_NumSortieAval_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumSortieAval_out,"@va_NumSortieAval_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(va_NumSortieAval_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	Tab_donnees2[4].pt_value = (CS_VOID *)va_DistanceSortieAval_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceSortieAval_out,"@va_DistanceSortieAval_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_DistanceSortieAval_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[5].pt_value = (CS_VOID *)va_SortieAvalEstBifurc_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_SortieAvalEstBifurc_out,"@va_SortieAvalEstBifurc_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(SortieAvalEstBifurc) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}


	Tab_donnees2[6].pt_value = (CS_VOID *)va_NumPeageAmont_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumPeageAmont_out,"@va_NumPeageAmont_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(va_NumPeageAmont_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[7].pt_value = (CS_VOID *)va_DistancePeageAmont_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistancePeageAmont_out,"@va_DistancePeageAmont_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_DistancePeageAmont_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[8].pt_value = (CS_VOID *)va_NumPeageAval_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumPeageAval_out,"@va_NumPeageAval_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(va_NumPeageAval_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[9].pt_value = (CS_VOID *)va_DistancePeageAval_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistancePeageAval_out,"@va_DistancePeageAval_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_DistancePeageAval_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[10].pt_value = (CS_VOID *)va_LongueurPlateforme_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_LongueurPlateforme_out,"@va_LongueurPlateforme_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_LongueurPlateforme_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[11].pt_value = (CS_VOID *)va_NumAutorouteBifAmont_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumAutorouteBifAmont_out,"@va_NumAutorouteBifAmont_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(va_NumAutorouteBifAmont_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[12].pt_value = (CS_VOID *)va_DistanceBifAmont_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceBifAmont_out,"@va_DistanceBifAmont_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_DistanceBifAmont_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[13].pt_value = (CS_VOID *)va_NumAutorouteBifAval_out;

	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_NumAutorouteBifAval_out,"@va_NumAutorouteBifAval_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(va_NumAutorouteBifAval_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[14].pt_value = (CS_VOID *)va_DistanceBifAval_out;

	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_DistanceBifAval_out,"@va_DistanceBifAval_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_DistanceBifAval_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[15].pt_value = (CS_VOID *)va_Descente_out;

	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Descente_out,"@va_Descente_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(va_Descente_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	Tab_donnees2[16].pt_value = (CS_VOID *)va_Tunnel_out;

	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Tunnel_out,"@va_Tunnel_out"))!= CS_SUCCEED)
	{
	
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(va_Tunnel_out) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[17].pt_value = (CS_VOID *)va_NomSortieAm_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_NomSortieAm_out,"@va_NomSortieAm_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(NomSortieAm) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[18].pt_value = (CS_VOID *)va_AbrevSortieAm_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_AbrevSortieAm_out,"@va_AbrevSortieAm_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(AbrevSortieAm) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	Tab_donnees2[19].pt_value = (CS_VOID *)va_SortieAmUsager_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_SortieAmUsager_out,"@va_SortieAmUsager_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(SortieAmUsager) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[20].pt_value = (CS_VOID *)va_NomSortieAv_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_NomSortieAv_out,"@va_NomSortieAv_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(NomSortieAv) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[21].pt_value = (CS_VOID *)va_AbrevSortieAv_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_AbrevSortieAv_out,"@va_AbrevSortieAv_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(AbrevSortieAv) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	Tab_donnees2[22].pt_value = (CS_VOID *)va_SortieAvUsager_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_SortieAvUsager_out,"@va_SortieAvUsager_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(SortieAvUsager) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[23].pt_value = (CS_VOID *)va_NomPeageAm_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_NomPeageAm_out,"@va_NomPeageAm_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(NomPeageAm) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[24].pt_value = (CS_VOID *)va_AbrevPeageAm_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_AbrevPeageAm_out,"@va_AbrevPeageAm_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(AbrevPeageAm) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[25].pt_value = (CS_VOID *)va_NomPeageAval_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_NomPeageAval_out,"@va_NomPeageAval_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(NomPeageAval) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[26].pt_value = (CS_VOID *)va_AbrevPeageAval_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_AbrevPeageAval_out,"@va_AbrevPeageAval_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(AbrevPeageAval) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	
	
	Tab_donnees2[27].pt_value = (CS_VOID *)va_Pente_out;
	
	if ((vl_retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)va_Pente_out,"@va_Pente_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_smallint_output(Pente) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	Tab_donnees2[28].pt_value = (CS_VOID *)va_Site_out;
	
	if ((vl_retcode = asql_ctparam_tinyint_output(cmd,(CS_TINYINT *)va_Site_out,"@va_Site_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_tinyint_output(Site) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	Tab_donnees2[29].pt_value = (CS_VOID *)va_PrBifAmont_out;
	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PrBifAmont_out, "@va_PrBifAmont_out"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_PrBifAmont_out) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  free(Tab_donnees2);
	  return (XDC_NOK);
	}
	
	Tab_donnees2[30].pt_value = (CS_VOID *)va_PrBifAval_out;
	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PrBifAval_out, "@va_PrBifAval_out"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_PrBifAval_out) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  free(Tab_donnees2);
	  return (XDC_NOK);
	}

	Tab_donnees2[31].pt_value = (CS_VOID *)va_PrDebutBifAmont_out;
	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PrDebutBifAmont_out, "@va_PrDebut_BifAmont_out"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_PrDebutBifAmont_out) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  free(Tab_donnees2);
	  return (XDC_NOK);
	}

	Tab_donnees2[32].pt_value = (CS_VOID *)va_PrDebutBifAval_out;
	if ((vl_retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_PrDebutBifAval_out,"@va_PrDebut_BifAval_out"))!= CS_SUCCEED)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_int_output(va_PrDebutBifAval_out) a echoue");
	  XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	  asql_clean_cmd(connection, cmd);
	  free(Tab_donnees2);
	  return (XDC_NOK);
	}

	Tab_donnees2[33].pt_value = (CS_VOID *)va_Viaduc_out;
	
	if ((vl_retcode = asql_ctparam_char_output(cmd,(CS_CHAR *)va_Viaduc_out,"@va_Viaduc_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_char_output(NomViaduc) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	Tab_donnees2[34].pt_value = (CS_VOID *)va_ZoneUrbaine_out;
	
	if ((vl_retcode = asql_ctparam_smallint_output(cmd,(CS_SMALLINT *)va_ZoneUrbaine_out,"@va_ZoneUrbaine_out"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_ctparam_smallint_output(va_ZoneUrbaine_out) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
		asql_clean_cmd(connection, cmd);
	   	free(Tab_donnees2);
	   	return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);

						XZST_03EcritureTrace(XZSTC_INFO,"xzae21: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae21 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (cmd, &vl_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae21 : CS_STATUS_RESULT = %ld", vl_retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzae21 : STATUS REUSULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
						
						if (vl_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				/*
				** Les trois types de resultats sont fetchables.
				** Since the result model for rpcs and rows have
				** been unified in the New Client-Library, we
				** will use the same routine to display them
				*/
				
				if (vl_retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: asql_fetch_data() a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
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
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzae21: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}


	XZST_03EcritureTrace(XZSTC_WARNING,"JMG %s",(char *)vl_Tab_Ligne[0][11].pt_value);
	XZST_03EcritureTrace(XZSTC_WARNING,"JMG %s",(char *)vl_Tab_Ligne[0][12].pt_value);
	strcpy(va_Descente_out, (char *)vl_Tab_Ligne[0][0].pt_value);
	strcpy(va_Tunnel_out, (char *)vl_Tab_Ligne[0][1].pt_value);
	strcpy(va_NomSortieAm_out, (char *)vl_Tab_Ligne[0][2].pt_value);
	strcpy(va_AbrevSortieAm_out, (char *)vl_Tab_Ligne[0][3].pt_value);
	strcpy(va_NomSortieAv_out, (char *)vl_Tab_Ligne[0][4].pt_value);
	strcpy(va_AbrevSortieAv_out, (char *)vl_Tab_Ligne[0][5].pt_value);
	strcpy(va_NomPeageAm_out, (char *)vl_Tab_Ligne[0][6].pt_value);
	strcpy(va_AbrevPeageAm_out, (char *)vl_Tab_Ligne[0][7].pt_value);
	strcpy(va_NomPeageAval_out, (char *)vl_Tab_Ligne[0][8].pt_value);
	strcpy(va_AbrevPeageAval_out, (char *)vl_Tab_Ligne[0][9].pt_value);
	strcpy(va_SortieAmUsager_out, (char *)vl_Tab_Ligne[0][10].pt_value);
	strcpy(va_SortieAvUsager_out, (char *)vl_Tab_Ligne[0][11].pt_value);
	strcpy(va_Viaduc_out, (char *)vl_Tab_Ligne[0][12].pt_value);

 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE21_Localiser_Evt_PA retourne %d", vl_retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}
