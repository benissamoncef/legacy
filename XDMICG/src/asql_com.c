/*E*/
/* Fichier : asql_com.c
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ASQLCOM * FICHIER asql_com.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  asql_fetch_param_result permet de parser les parametres de retour 
*	d une stored procedure
**********************
*  asql_fetch : permet de recuperer le retour d une SP execute 
*  sur un Sql Serveur (parametres de retour, code d erreur , 
*  resultat d un select.
**********************
*  asql_fetch_data : permet d afficher tout type de retour d une SP execute 
*  sur un Sql Serveur (parametres de retour, code d erreur , 
*  resultat d un select.
* *********************
*  asql_display_dlen permet un mise en forme pour l affichage
*  des donnees en retour d execution d une SP. est utilisee par 
*  asql_fetch_data.
***********************
*  asql_display_header affiche l entete des colonnes pour 
*  l affichage d un retour de SP : utilise par asql_fetch_data
***********************
*  asql_date_Ux2Sybase transforme une date au format unix 
*  (nb de secondes depuis 1970) au format sybase 
*  (Nb jour depuis 1900, nb de 300eme de sec depuis 00:00:00).
***********************
*  asql_date_Sybase2Ux transforme une date Sybase au format unix.
***********************
*  asql_fetch_row_result permet de parser le retour d un select et
*  de remplir dynamiquement untableau de ligne et de colonne.
*  Chaque cellule du tableau contient une structure contenant le type
*  de la donnee recue, un pointeur sur la valeur, et accessoirement (pour les string) 
*  la longueur.
*  Attention, il faut liberer le tableau passe en argument dans la procedure
*  appelante apres avoir utilise les donnees recues.
***********************
*  asql_alloc_Col_Type : Alloue le pointeur Data 
*  (sur le type contenu dans column->datatype),
*  dans le quel on doit recevoir la colonne 
*  quand on parse le CS_ROW_RESULT 
*  (renvoye par une stored procedure qui fait un select par exemple).
*  L'utilisation de cette fonction se fait dans asql_fetch_row_result.
*
***********************
*  asql_Free_Tab_Ligne_Results permet de liberer le tableau alloue dynamiquement 
*  par asql_fetch_raw_results.
***********************
*  asql_free_Col_Type :
*  Desalloue le pointeur Data (sur le type contenu dans column->datatype),
*  dans le quel on a recu la colonne parsee par asql_fetch_row_result
* .
* L'utilisation de cette fonction se fait dans asql_free_row_result.
***********************
* asql_Affiche_Tab_Ligne_Results permet d afficher le resultat rendu par le ct_fetch 
* sur les lignes rendus par exemple sur un select effectue par une stored procedure.
***********************
*  asql_Affiche_Col_Fct_Type met en forme l affichage des donnees du tableau
*  en fonction du type de la donnees.
***********************
*   asql_ctparam_tinyint_input : Encapsule l'appel a ct_param 
*   pour les parametres a passer a une SP en Input value. ***********************
***********************
*  asql_ctparam_smallint_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
***********************
*  asql_ctparam_int_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
***********************
*  asql_ctparam_bit_input : Encapsule l'appel a ct_param
*  pour les parametres a passer a une SP en Input value. 
***********************
*  asql_ctparam_date4_input : Encapsule l'appel a ct_param
*  pour les parametres a passer a une SP en Input value. 
***********************
*  asql_ctparam_date_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
***********************
*  asql_ctparam_char_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value.***********************
***********************
*  asql_ctparam_tinyint_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
***********************
*  asql_ctparam_smallint_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value.
***********************
*  asql_ctparam_int_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
***********************
*  asql_ctparam_bit_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
***********************
*  asql_ctparam_date_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
***********************
*  asql_ctparam_char_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
***********************
*  asql_clientmsg_cbHandler d erreur pour le Open Client
*  Quand une erreur est detectee par la CT lib
*  On l affiche.(provient de exutils.c des sample ctlib de Sybase).
***********************
*  asql_servermsg_cb : Handler d erreur pour le Open Client
*  Quand une erreur est detectee par la CT lib
*  On l affiche.(provient de exutils.c des sample ctlib de Sybase.
***********************
*  asql_clean_cmd : encapsule ct_drop_cmd afin de liberer la commande alouee par
*  ct_cmd_alloc. Si ct_drop_cmd echoue, on cancele la commande en cours sur
*  laquelle on a un probleme d execution et qui reste en attente (blocage de l application)
*  via ct_cancel. Puis on refait un essai de drop de la commande. Si elle echoue on sort en
*  fatal.
*  La fonction ne retourne rien. Si son execution se passe mal, alors on trace en 
*  fatal : le noyau de supervision doit arreter la tache.
***********************
*  asql_cmd_alloc : encapsule ct_cmd_alloc.
***********************
------------------------------------------------------
* HISTORIQUE :
*
* MERCIER	31 Aug 1994	: Creation
**************************
* volcic	12 Oct 1994	: Modification trace FATAL => WARNING (v 1.20)
**************************
* volcic	14 Oct 1994	: Ajout asql_fetch (v 1.23)
**************************
* volcic	07 Nov 1994	: Ajout asql_fetch (v 1.25)
**************************
* MERCIER	10 Nov 1994	: Ajout de asql_clean_cmd	V 1.26
*				et de asql_cmd_alloc
**************************
* volcic	23 Nov 1994	: Ajout Initialisation des parametres de sortie (v 1.27)
**************************
* MERCIER	01 Dec 1994	: Modification de asql_cmd_alloc V 1.29
*				  correction de asql_ctparam_char_output
**************************
*				
* volcic	01 Dec 1994	: Ajout format datafmt pour ct_param_char_out (v 1.30)
**************************
* MERCIER	03 Dec 1994	: Modification de asql_cmd_alloc V 1.31
*				  correction de asql_fetch_row_result sur 
* fuite memoire et le nombres de lignes effectivement lues.
**************************
* MERCIER	14 Dec 1994	: correction de asql_fetch_row_result sur le	1.32
* nbre de lignes lues.
*
**************************
* GABORIT	05 jan 1995	: modif traces	1.34
************************** 
* GABORIT-MERCIER 19 Jan 1995	: free derniere		V 1.35
* de la derniere ligne du tableau de select (asql_fetch_row_result).
**************************
* GABORIT       23 jun 1995	: modif traces (1.36)
* JPL		03/12/10 : Migration architecture HP ia64 (DEM 961) : traces; caracteres iso_1; IDEM PRECEDENTE  1.37
* JPL		03/12/10 : Migration HP ia64 (DEM 961) : types & args pour LP64 et SYB 15.x; IDEM PREC. AUTRES CAS 1.38
* JPL		06/12/10 : Migration HP ia64 (DEM 961) : Liberation memoire : adresses donnees elementaires 1.39
* JMG		18/04/12 : linux  1.41
* JPL		30/01/13 : Traces fonctions 'SGBD' et non 'DEBUG1'; suppression dans fonctions internes  1.42
* JPL		25/02/13 : asql_Free_Tab_Ligne_Results: liberation memoire uniquement si allouee  1.43
* JPL		07/12/16 : Niveaux de traces ; suppression dans fonctions internes  1.44
* JPL		06/04/17 : Suppression trace conversion de dates  1.45
* LCL		03/03/20 : Correction segfault sur param CHAR null 1.46 MOVIS DEM-SAE93
* AMI		09/12/20 : SAE_191 : modification de la gestion de la date
* AMI		28/01/21 : SAE_191 : retour arriere concernant la gestion de la date pour travail avec un nb de seconde TZ
* JPL		28/12/21 : Gestion du texte de version (pour trace -non utilise-); IDEM PRECEDENTE  1.49
* JPL		29/12/21 : asql_date_Ux2Sybase: eviter erreur d'arrondi (pour SAE_360)  1.50
* PNI		30/11/23 : test si point non null avant free suite à échec de ct_cmd_alloc SAE-554
* PNI		15/12/23 : suppression du free suite à échec de ct_cmd_alloc SAE-554
------------------------------------------------------*/

static char *version = "asql_com.c 1.50" ;


/* fichiers inclus */

#include "asql_com.h"
//#include <time.h>

/* definitions de constantes */

/* Difference en jours entre les origines de temps Sybase (01/01/1900) et Unix */
/*  soit 70 ans avec 17 annees bissextiles de 1904 a 1968 */
#define	CM_NBJ_T0_SYB_UNIX	((70 * 365) + 17)


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
int asql_alloc_Col_Type (CS_DATAFMT *, tg_Colonne_donnees2 *);
int asql_free_Col_Type (tg_Colonne_donnees2 *);

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  permet de recuperer les parametres de sortie d une SP.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_fetch_param_result(CS_COMMAND      *pa_cmd,int va_num_param_retour, tg_Colonne_donnees2 *pa_Tab_param_retour)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_RETCODE              vl_retcode;
        CS_INT                  vl_num_cols;
        CS_INT                  i;
        CS_INT                  j;
        CS_INT                  vl_row_count = 0;
        CS_INT                  vl_rows_read;
        CS_INT                  vl_disp_len;
        CS_DATAFMT              *pl_datafmt;
        
	XZST_03EcritureTrace(XZSTC_SGBD,"IN :asql_fetch_param_result : Nb parametres = %d",va_num_param_retour);
	
	/*A
	** Recupere le nombre de colonnes dans le paquet de resultats
	*/
	vl_retcode = ct_res_info(pa_cmd, CS_NUMDATA, &vl_num_cols, CS_UNUSED, NULL);
        if (vl_retcode != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_res_info() a echoue, code = %d", vl_retcode);
                return vl_retcode;
        }
	/*A
	** On verifie que l'on a au moins une colonne
	*/
	if (vl_num_cols <= 0)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_res_info() retourne zero colonnes");
                return CS_FAIL;
        }

	/*
	** On verifie que la structure le tableau pa_Tab_param_retour de structure tg_Colonne_donnees2
	** ne pointe pas sur NULL : la fonction appelante a bien reservee le passage
	** de parametre.
	*/
	if ( pa_Tab_param_retour == NULL)
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: fournir la zone memoire pour les parametres");
                return CS_MEM_ERROR;
        }

	/*
	** On se reserve une structure Datafmt permettant de parser les retours 
	** avec les bons types a recupere
	*/
	pl_datafmt = (CS_DATAFMT *)malloc(vl_num_cols * sizeof (CS_DATAFMT));
        if (pl_datafmt == NULL)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: malloc() pour %d parametres a echoue", vl_num_cols);
                return CS_MEM_ERROR;
        }

	for (i = 0; i < vl_num_cols; i++)
        {
                /*
                ** Get the column description.  ct_describe() fills the
                ** pl_datafmt parameter with a description of the column.
                */
                vl_retcode = ct_describe(pa_cmd, (i + 1), &pl_datafmt[i]);
                if (vl_retcode != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_describe() a echoue, code = %d", vl_retcode);
                        break;
                }

                /*! Franck : Ajout pour reception param char et varchar */
		
		if (pl_datafmt[i].datatype == CS_CHAR_TYPE || pl_datafmt[i].datatype == CS_VARCHAR_TYPE)
		{
		   pl_datafmt[i].format = CS_FMT_NULLTERM;
		   pl_datafmt[i].maxlength = 255;
		} 
				
		if (pa_Tab_param_retour[i].pt_value == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: Preparation de la structure de recuperation des donnees erronees %d", i);
                        vl_retcode = CS_MEM_ERROR;
                        break;
                }
		/*
                ** Now bind.
                */
                vl_retcode = ct_bind(pa_cmd, (i + 1), &pl_datafmt[i], pa_Tab_param_retour[i].pt_value, NULL, NULL);
                if (vl_retcode != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_bind() a echoue, code = %d", vl_retcode);
                        break;
                }

	}/* Fin du for */
	/*A
	**  On parse les resultats pour remplir nos structures de donnees
	*/
	if (vl_retcode != CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_bind a echoue : liberation de la memoire");
                free(pl_datafmt);
                return vl_retcode;
        }

	while (((vl_retcode = ct_fetch(pa_cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED,
                        &vl_rows_read)) == CS_SUCCEED) || (vl_retcode == CS_ROW_FAIL))
        {
                /*
                ** Increment our row count by the number of rows just fetched.
                */
                vl_row_count = vl_row_count + vl_rows_read;

                /*
                ** Check if we hit a recoverable error.
                */
                if (vl_retcode == CS_ROW_FAIL)
                {
                	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: Erreur sur la ligne %d\n", vl_row_count);
                }

		/*A
		** On a une ligne.  On boucle sur les parametres pour les afficher.
		*/
                /*for (i = 0; i < vl_num_cols; i++)
                {
			asql_Affiche_Col_Fct_Type(0, i, pa_Tab_param_retour[i]);
                }
		XZST_03EcritureTrace(XZSTC_DEBUG2,"\n");
                */
                
        }/* Fin du while */
        
	/*A
	** Desallocation de la pl_datafmt pour parser le retour
	*/
	free(pl_datafmt);
	
	/*A
	**  Fin du parsing des datas : on regarde les codes de retour 
	**  de la RP via les codes de retour de ct_fetch()
	*/
	
	switch ((int)vl_retcode)
        {
                case CS_END_DATA:
                        /*
                        ** Tout est ok.
                        */
                        XZST_03EcritureTrace(XZSTC_SGBD, "asql_fetch_param_result: %d lignes\n", vl_row_count);
                  
                        vl_retcode = CS_SUCCEED;
                        break;

                case CS_FAIL:
                        /*
                        ** Cela n a pas marche du tout.
                        */
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_fetch() a echoue, code = CS_FAIL");
                        return vl_retcode;
                        break;
                default:
                        /*
                        ** Valeur de retour non attendue.
                        */
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_param_result: ct_fetch() retourne le code inattendu %d", vl_retcode);
                        return vl_retcode;
                        break;

        }

	XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_fetch_param_result retourne %d",vl_retcode);
        return vl_retcode;
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*  asql_fetch_data : permet d afficher tout type de retour d une SP execute 
*  sur un Sql Serveur (parametres de retour, code d erreur , 
*  resultat d un select.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int  asql_fetch (CS_COMMAND      *pa_cmd, tg_status_result *pa_statusSP)

/*
* ARGUMENTS EN ENTREE :
*    pa_cmd : paquet renvoye par le SQl Serveur apres execution d un SP. 
*	contient les parametres de retour et autre.
*
*
* ARGUMENTS EN SORTIE :
*   < pa_cmd - Pointer sur une structure de commande  >
*
*
* CODE RETOUR : 
*   <CS_MEM_ERROR    Si l allocation memoire a echoue.
*    CS_SUCCEED      Si the data was displayed.
*    CS_FAIL         Si pas de colonnes presente.
*    <vl_retcode>       Resultat de la fonction Client-Library si une erreur a ete
*                      retournee.
*
* CONDITION D'UTILISATION
*
* FONCTION 
** Cette fonction traite les resultats fetchable sets. Les resultats inclue:
**
**              CS_ROW_RESULT
**              CS_CURSOR_RESULT
**              CS_PARAM_RESULT
**              CS_STATUS_RESULT
**              CS_COMPUTE_RESULT
**
**      Since the Client-Library result model has been unified, the same
**      apis are used for each of the above result types.
**
**      One caveat is the processing of CS_COMPUTE_RESULTs. The name field
**      sent from the server is typically zero length. To display a meaningful
**      header, the aggregate compute operator name should be found for the
**      column, and that name used instead. The compute example program has
**      code which demonstrates this.*
------------------------------------------------------*/
{
	CS_RETCODE              vl_retcode;
   	CS_INT                  vl_num_cols;
	CS_INT                  i;
	CS_INT                  j;
	CS_INT                  vl_row_count = 0;
	CS_INT                  vl_rows_read;
	CS_INT                  vl_disp_len;
	CS_DATAFMT              *pl_datafmt;
	tg_Colonne_donnees      *pl_coldata;
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_fetch");
	/*
        ** Recupere le nombre de colonnes recues.
	*/
	vl_retcode = ct_res_info(pa_cmd, CS_NUMDATA, &vl_num_cols, CS_UNUSED, NULL);
	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: ct_res_info() a echoue, code = %d", vl_retcode);
		return vl_retcode;
	}

	/*
	** On verifie que l on a au moins une colonne.
	*/
	if (vl_num_cols <= 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: ct_res_info() retourne zero colonnes");
		return CS_FAIL;
	}

	/*
	** Our program variable, called 'pl_coldata', is an array of
	** EX_COLUMN_DATA structures. Each array element represents
	** one column.  Each array element will re-used for each row.
	**
	** First, allocate memory for the data element to process.
	*/

	pl_coldata = (tg_Colonne_donnees *)malloc(vl_num_cols * sizeof (tg_Colonne_donnees));

	if (pl_coldata == NULL)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: malloc() pour %d colonnes a echoue", vl_num_cols);
		return CS_MEM_ERROR;
	}

	pl_datafmt = (CS_DATAFMT *)malloc(vl_num_cols * sizeof (CS_DATAFMT));
	if (pl_datafmt == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: malloc() %d descripteurs a echoue", vl_num_cols);
		free(pl_coldata);
		return CS_MEM_ERROR;
	}

	/*
	** Loop through the columns getting a description of each one
	** and binding each one to a program variable.
	**
	** We're going to bind each column to a character string;
	** this will show how conversions from server native datatypes
	** to strings can occur via bind.
	**
	** We're going to use the same pl_datafmt structure for both the describe
	** and the subsequent bind.
	**
	** If an error occurs within the for loop, a break is used to get out
	** of the loop and the data that was allocated is free'd before
	** returning.
	*/

	for (i = 0; i < vl_num_cols; i++)
	{
		/*
		** Get the column description.  ct_describe() fills the
		** pl_datafmt parameter with a description of the column.
		*/
		vl_retcode = ct_describe(pa_cmd, (i + 1), &pl_datafmt[i]);
		if (vl_retcode != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: ct_describe() a echoue, code = %d", vl_retcode);
			break;
		}

		/*
		** update the pl_datafmt structure to indicate that we want the
		** results in a null terminated character string.
		**
		** First, update pl_datafmt.maxlength to contain the maximum
		** possible length of the column. To do this, call
		** ex_display_len() to determine the number of bytes needed
		** for the character string representation, given the
		** datatype described above.  Add one for the null
		** termination character.
		*/
		pl_datafmt[i].maxlength = asql_display_dlen(&pl_datafmt[i]) + 1;

		/*
		** Set datatype and format to tell bind we want things
		** converted to null terminated strings
		*/
		pl_datafmt[i].datatype = CS_CHAR_TYPE;
		pl_datafmt[i].format   = CS_FMT_NULLTERM;

		/*
		** Allocate memory for the column string
		*/
		pl_coldata[i].value = (CS_CHAR *)malloc(pl_datafmt[i].maxlength);
		if (pl_coldata[i].value == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: malloc() pour valeur a echoue");
			vl_retcode = CS_MEM_ERROR;
			break;
		}

		/*
		** Now bind.
		*/
		vl_retcode = ct_bind(pa_cmd, (i + 1), &pl_datafmt[i], pl_coldata[i].value, &pl_coldata[i].valuelen, &pl_coldata[i].indicator);
		if (vl_retcode != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: ct_bind() a echoue, code = %d", vl_retcode);
			break;
		}
	}
	if (vl_retcode != CS_SUCCEED)
	{
		for (j = 0; j < i; j++)
		{
			free(pl_coldata[j].value);
		}
		free(pl_coldata);
		free(pl_datafmt);
		XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_fetch retourne %d",vl_retcode);
		return vl_retcode;
	}

	/*
	** Fetch les lignes.  Boucle while sur ct_fetch() retourne CS_SUCCEED ou
	** CS_ROW_FAIL
	*/

	while (((vl_retcode = ct_fetch(pa_cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED,
			&vl_rows_read)) == CS_SUCCEED) || (vl_retcode == CS_ROW_FAIL))
	{
		/*
		** Incremente notre compteur de ligne par le nombre de lignes deja fetchee.
		*/
		vl_row_count = vl_row_count + vl_rows_read;

		/*A
		** Check if we hit a recoverable error.
		*/
		if (vl_retcode == CS_ROW_FAIL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: Erreur sur la ligne %d\n", vl_row_count);
		}
		/*A
		** On a une ligne.  On boucle sur les colonnes de la ligne pour les afficher.
		*/
		for (i = 0; i < vl_num_cols; i++)
		{
			/*
			** Display the column value
			*/
			XZST_03EcritureTrace(XZSTC_DEBUG2,"%s", pl_coldata[i].value);
			*pa_statusSP = atoi( pl_coldata[i].value ); 


			/* DECOMMENTER POUR DEBUG : Affichage des donnees sur stdout */
			/*fputs(pl_coldata[i].value, stdout);
			if (i != vl_num_cols - 1)
			{
				vl_disp_len = asql_display_dlen(&pl_datafmt[i]);
				vl_disp_len -= pl_coldata[i].valuelen - 1;
				for (j = 0; j < vl_disp_len; j++)
				{
					fputc(' ', stdout);
				}
			}*/
		}
		/*XZST_03EcritureTrace(XZSTC_DEBUG2,"\n");*/
	}

	/*
	** Free allocated space.
	*/
	for (i = 0; i < vl_num_cols; i++)
	{
		free(pl_coldata[i].value);
	}
	free(pl_coldata);
	free(pl_datafmt);

	/*
	** We're done processing rows.  Let's check the final return
	** value of ct_fetch().
	*/
	switch ((int)vl_retcode)
	{
		case CS_END_DATA:
			/*
			** Tout s'est bien passe.
			*/
			XZST_03EcritureTrace(XZSTC_INFO, "asql_fetch: %d lignes\n", vl_row_count);
			
			vl_retcode = CS_SUCCEED;
			break;

		case CS_FAIL:
			/*
			** Un probleme est survenu.
			*/
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: ct_fetch() a echoue");
			return vl_retcode;
			break;
		default:
			/*
			** Un retour non attendu est survenu!!.
			*/
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch: ct_fetch() retourne le code inattendu %d", vl_retcode);
		 	return vl_retcode;
			break;

	}

	XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_fetch retourne %d",vl_retcode);
	
	return vl_retcode;
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*  asql_fetch_data : permet d afficher tout type de retour d une SP execute 
*  sur un Sql Serveur (parametres de retour, code d erreur , 
*  resultat d un select.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int  asql_fetch_data(CS_COMMAND      *pa_cmd)

/*
* ARGUMENTS EN ENTREE :
*    pa_cmd : paquet renvoye par le SQl Serveur apres execution d un SP. 
*	contient les parametres de retour et autre.
*
*
* ARGUMENTS EN SORTIE :
*   < pa_cmd - Pointer sur une command structure >
*
*
* CODE RETOUR : 
*   <CS_MEM_ERROR    If an memory allocation a echoue.
*    CS_SUCCEED      If the data was displayed.
*    CS_FAIL         If no columns were present.
*    <vl_retcode>       Result of the Client-Library function if a failure was
*                      returned.
*
* CONDITION D'UTILISATION
*
* FONCTION 
This function processes fetchable results sets. The results include:
**
**              CS_ROW_RESULT
**              CS_CURSOR_RESULT
**              CS_PARAM_RESULT
**              CS_STATUS_RESULT
**              CS_COMPUTE_RESULT
**
**      Since the Client-Library result model has been unified, the same
**      apis are used for each of the above result types.
**
**      One caveat is the processing of CS_COMPUTE_RESULTs. The name field
**      sent from the server is typically zero length. To display a meaningful
**      header, the aggregate compute operator name should be found for the
**      column, and that name used instead. The compute example program has
**      code which demonstrates this.*
------------------------------------------------------*/
{
	CS_RETCODE              vl_retcode;
   	CS_INT                  vl_num_cols;
	CS_INT                  i;
	CS_INT                  j;
	CS_INT                  vl_row_count = 0;
	CS_INT                  vl_rows_read;
	CS_INT                  vl_disp_len;
	CS_DATAFMT              *pl_datafmt;
	tg_Colonne_donnees      *pl_coldata;
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_fetch_data");
	/*
        ** Find out how many columns there are in this result set.
	*/
	vl_retcode = ct_res_info(pa_cmd, CS_NUMDATA, &vl_num_cols, CS_UNUSED, NULL);
	if (vl_retcode != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: ct_res_info() a echoue, code = %d", vl_retcode);
		return vl_retcode;
	}

	/*
	** Make sure we have at least one column
	*/
	if (vl_num_cols <= 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: ct_res_info() ne retourne pas de colonnes (CS_FAIL)");
		return CS_FAIL;
	}

	/*
	** Our program variable, called 'pl_coldata', is an array of
	** EX_COLUMN_DATA structures. Each array element represents
	** one column.  Each array element will re-used for each row.
	**
	** First, allocate memory for the data element to process.
	*/

	pl_coldata = (tg_Colonne_donnees *)malloc(vl_num_cols * sizeof (tg_Colonne_donnees));

	if (pl_coldata == NULL)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: malloc() pour %d colonnes a echoue", vl_num_cols);
		return CS_MEM_ERROR;
	}

	pl_datafmt = (CS_DATAFMT *)malloc(vl_num_cols * sizeof (CS_DATAFMT));
	if (pl_datafmt == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: malloc() %d descripteurs a echoue", vl_num_cols);
		free(pl_coldata);
		return CS_MEM_ERROR;
	}

	/*
	** Loop through the columns getting a description of each one
	** and binding each one to a program variable.
	**
	** We're going to bind each column to a character string;
	** this will show how conversions from server native datatypes
	** to strings can occur via bind.
	**
	** We're going to use the same pl_datafmt structure for both the describe
	** and the subsequent bind.
	**
	** If an error occurs within the for loop, a break is used to get out
	** of the loop and the data that was allocated is free'd before
	** returning.
	*/

	for (i = 0; i < vl_num_cols; i++)
	{
		/*
		** Get the column description.  ct_describe() fills the
		** pl_datafmt parameter with a description of the column.
		*/
		vl_retcode = ct_describe(pa_cmd, (i + 1), &pl_datafmt[i]);
		if (vl_retcode != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: ct_describe() a echoue");
			break;
		}

		/*
		** update the pl_datafmt structure to indicate that we want the
		** results in a null terminated character string.
		**
		** First, update pl_datafmt.maxlength to contain the maximum
		** possible length of the column. To do this, call
		** ex_display_len() to determine the number of bytes needed
		** for the character string representation, given the
		** datatype described above.  Add one for the null
		** termination character.
		*/
		pl_datafmt[i].maxlength = asql_display_dlen(&pl_datafmt[i]) + 1;

		/*
		** Set datatype and format to tell bind we want things
		** converted to null terminated strings
		*/
		pl_datafmt[i].datatype = CS_CHAR_TYPE;
		pl_datafmt[i].format   = CS_FMT_NULLTERM;

		/*
		** Allocate memory for the column string
		*/
		pl_coldata[i].value = (CS_CHAR *)malloc(pl_datafmt[i].maxlength);
		if (pl_coldata[i].value == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: malloc() pour valeur a echoue");
			vl_retcode = CS_MEM_ERROR;
			break;
		}

		/*
		** Now bind.
		*/
		vl_retcode = ct_bind(pa_cmd, (i + 1), &pl_datafmt[i], pl_coldata[i].value, &pl_coldata[i].valuelen, &pl_coldata[i].indicator);
		if (vl_retcode != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: ct_bind() a echoue, code = %d", vl_retcode);
			break;
		}
	}
	if (vl_retcode != CS_SUCCEED)
	{
		for (j = 0; j < i; j++)
		{
			free(pl_coldata[j].value);
		}
		free(pl_coldata);
		free(pl_datafmt);
		XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_fetch_data retourne %d",vl_retcode);
		return vl_retcode;
	}

	/*
	** Display column header
	*/
	asql_display_header(vl_num_cols, pl_datafmt);

	/*
	** Fetch the rows.  Loop while ct_fetch() returns CS_SUCCEED or
	** CS_ROW_FAIL
	*/

	while (((vl_retcode = ct_fetch(pa_cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED,
			&vl_rows_read)) == CS_SUCCEED) || (vl_retcode == CS_ROW_FAIL))
	{
		/*
		** Increment our row count by the number of rows just fetched.
		*/
		vl_row_count = vl_row_count + vl_rows_read;

		/*
		** Check if we hit a recoverable error.
		*/
		if (vl_retcode == CS_ROW_FAIL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: Erreur sur la ligne %d\n", vl_row_count);
		}
		/*
		** We have a row.  Loop through the columns displaying the
		** column values.
		*/
		for (i = 0; i < vl_num_cols; i++)
		{
			/*
			** Display the column value
			*/
			XZST_03EcritureTrace(XZSTC_DEBUG2,"%s", pl_coldata[i].value);
			
			/* DECOMMENTER POUR DEBUG : Affichage des donnees sur stdout */
			/*fputs(pl_coldata[i].value, stdout);
			if (i != vl_num_cols - 1)
			{
				vl_disp_len = asql_display_dlen(&pl_datafmt[i]);
				vl_disp_len -= pl_coldata[i].valuelen - 1;
				for (j = 0; j < vl_disp_len; j++)
				{
					fputc(' ', stdout);
				}
			}*/
		}
		/*XZST_03EcritureTrace(XZSTC_DEBUG2,"\n");*/
	}

	/*
	** Free allocated space.
	*/
	for (i = 0; i < vl_num_cols; i++)
	{
		free(pl_coldata[i].value);
	}
	free(pl_coldata);
	free(pl_datafmt);

	/*
	** We're done processing rows.  Let's check the final return
	** value of ct_fetch().
	*/
	switch ((int)vl_retcode)
	{
		case CS_END_DATA:
			/*
			** Tout s'est bien passe.
			*/
			XZST_03EcritureTrace(XZSTC_INFO,"asql_fetch_data: %d lignes\n", vl_row_count);
			
			vl_retcode = CS_SUCCEED;
			break;

		case CS_FAIL:
			/*
			** Un probleme est survenu.
			*/
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: ct_fetch() a echoue (CS_FAIL)");
		 	return vl_retcode;
			break;
		default:
			/*
			** Un retour non attendu est survenu!!.
			*/
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_data: ct_fetch() retourne le code inattendu %d", vl_retcode);
		 	return vl_retcode;
			break;

	}
	XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_fetch_data retourne %d",vl_retcode);
	return vl_retcode;
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  asql_display_dlen permet un mise en forme pour l affichage
*  des donnees en retour d execution d une SP. est utilisee par 
*  asql_fetch_data.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_display_dlen(CS_DATAFMT              *column)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	CS_INT          len;
	
	/*XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_display_dlen");*/
	switch ((int) column->datatype)
	{
		case CS_CHAR_TYPE:
		case CS_VARCHAR_TYPE:
		case CS_TEXT_TYPE:
		case CS_IMAGE_TYPE:
			len = MIN(column->maxlength, cg_MaxSizeBuf);
			break;

		case CS_BINARY_TYPE:
		case CS_VARBINARY_TYPE:
			len = MIN((2 * column->maxlength) + 2, cg_MaxSizeBuf);
			break;

		case CS_BIT_TYPE:
		case CS_TINYINT_TYPE:
			len = 3;
			break;

		case CS_SMALLINT_TYPE:
			len = 6;
			break;

		case CS_INT_TYPE:
			len = 11;
			break;

		case CS_REAL_TYPE:

		case CS_FLOAT_TYPE:
			len = 20;
			break;

		case CS_MONEY_TYPE:
		case CS_MONEY4_TYPE:
			len = 24;
			break;

		case CS_DATETIME_TYPE:
		case CS_DATETIME4_TYPE:
			len = 30;
			break;

		case CS_NUMERIC_TYPE:
		case CS_DECIMAL_TYPE:
			len = (CS_MAX_PREC + 2);
			break;

		default:
			len = column->maxlength;
			break;
	}
	/*XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_display_dlen");*/
	return MAX(strlen(column->name) + 1, len); 
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*  asql_display_header affiche l entete des colonnes pour 
*  l affichage d un retour de SP : utilise par asql_fetch_data.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_display_header(CS_INT          numcols,CS_DATAFMT      columns[])

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/

{
	CS_INT          i;
	CS_INT          l;
	CS_INT          j;
	CS_INT          vl_disp_len;
	
	/*XZST_03EcritureTrace(XZSTC_SGBD,"IN :asql_display_header ");*/
	fputc('\n', stdout);
	for (i = 0; i < numcols; i++)
	{
		vl_disp_len = asql_display_dlen(&columns[i]);
		XZST_03EcritureTrace(XZSTC_DEBUG2, "%s", columns[i].name);
		
		l = vl_disp_len - strlen(columns[i].name);
        	if (columns[i].name==NULL)
                	l=0;
	        else
        	        l=strlen(columns[i].name);

		
		for (j = 0; j < l; j++)
		{
			fputc(' ', stdout);
			fflush(stdout);
		}
	}
	fputc('\n', stdout);
	fflush(stdout);
	for (i = 0; i < numcols; i++)
	{
		vl_disp_len = asql_display_dlen(&columns[i]);
		l = vl_disp_len - 1;
		for (j = 0; j < l; j++)
		{
			fputc('-', stdout);
		}
		fputc(' ', stdout);
	}
	fputc('\n', stdout);
	/*XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_display_header ");*/
	return CS_SUCCEED;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   asql_date_Ux2Sybase transforme une date au format unix 
*   (nb de secondes depuis 1970) au format sybase 
*   (Nb jour depuis 1900, nb de 300eme de sec depuis 00:00:00).
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_date_Ux2Sybase(double va_Nb_Second,CS_DATETIME *pa_DateTime)

/*
* ARGUMENTS EN ENTREE :
*
*	va_Nb_Second contient le nbre de secondes depuis 1970 1/1 0h 0m 0s
*
* ARGUMENTS EN SORTIE :
*  	Structure de type CS_DATETIME qui possede deux champs :
*	CS_INT          dtdays		nbre de jour depuis 1/1/1900
*	CS_INT          dttime 	nbre de 300eme de secondes depuis 0h
*
* CODE RETOUR : 
*
*	Renvoie XDC_OK	si tout est ok
*		XDC_NOK si il y a eu un probleme.     
*
* CONDITION D'UTILISATION
*	Il faut que pa_DateTime pointe sur une structure allouee
* FONCTION 
*	Effectue la conversion entre une date format unix 
*	vers une date format Sybase
------------------------------------------------------*/
{
	long		vl_nb_secondes = (long) va_Nb_Second;
	long		vl_nb_jours;

	/*XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_date_Ux2Sybase");*/
	if (pa_DateTime == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_date_Ux2Sybase a recu un pointeur sur date null devant contenir la conversion");
		return XDC_NOK;
	}

	if ( va_Nb_Second <= 0.0 )
	{
		pa_DateTime->dtdays = 0;
		pa_DateTime->dttime = 0;
	}
	else
	{
		vl_nb_jours = vl_nb_secondes / (24 * 3600);

		pa_DateTime->dtdays = (CS_INT) vl_nb_jours + CM_NBJ_T0_SYB_UNIX;
		pa_DateTime->dttime = (CS_INT) (vl_nb_secondes % (24 * 3600)) * 300;

		/*XZST_03EcritureTrace(XZSTC_SGBD,"asql_date_Ux2Sybase: dtdays=%d, dttime=%d\n",pa_DateTime->dtdays,pa_DateTime->dttime);*/
	}

	/*XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_date_Ux2Sybase");*/
	return(XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  asql_date_Sybase2Ux transforme une date Sybase au format unix.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_date_Sybase2Ux(CS_DATETIME va_DateTime,double *pa_Nb_Second)
       

/*
* ARGUMENTS EN ENTREE :
*  Structure de type CS_DATETIME qui possede deux champs :
*	CS_INT          dtdays		nbre de jour depuis 1/1/1900
*	CS_INT          dttime 	nbre de 300eme de secondes depuis 0h
*
*
* ARGUMENTS EN SORTIE :
*   
*	pa_Nb_Second contient le nbre de secondes depuis 1970 1/1 0h 0m 0s
*
* CODE RETOUR : 
*   	Renvoie XDC_OK	si tout est ok
*		XDC_NOK si il y a eu un probleme. 
*
* CONDITION D'UTILISATION
*   	Il faut que pa_Nb_Second pointe sur un long qui a ete alloue 
*	par la procedure appelante.
*
* FONCTION 
*   	Effectue la conversion entre une date format Sybase 
*	vers une date au format Unix (nbre de secondes depuis 1970).
*
------------------------------------------------------*/
{
	ldiv_t 			vl_division_result;
	unsigned long int 	vl_Nb_Second_Intermediaire;
	unsigned long int	R1,R2,R3,R4,R5;


	/*XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : asql_date_Sybase2Ux %d//%d",va_DateTime.dtdays,va_DateTime.dttime);*/
	
	if (pa_Nb_Second == NULL) return (XDC_NOK);
	if ( va_DateTime.dttime == 0 &&  va_DateTime.dtdays == 0)
	    *pa_Nb_Second = 0;
	else
	{    
	   vl_division_result = ldiv(va_DateTime.dttime,300);
	   R1 = (unsigned long int)va_DateTime.dtdays*24*3600;
	   R2 = (unsigned long int)vl_division_result.quot;
	   R3 = R1 + R2;
	   R4 = (unsigned long int)2208988800;
	   R5 = R3 - R4;

//#ifdef _OLDTZONE
	   /*XZST_03EcritureTrace(XZSTC_INFO," R1 = %ld, R2 = %ld, R3=%ld, R4 = %ld, R5=%ld", R1,R2,R3,R4,R5);*/
	   *pa_Nb_Second=(double)R5;
		/*XZST_03EcritureTrace(XZSTC_INFO," R = %f",*pa_Nb_Second);*/
//#else

      // variables utilisees pour la conversion secondes TZ -> secondes UTC
//   	struct tm * now ;
      // rempli la structure tm pour pouvoir refaire un calcul de seconde en UTC      
//      now= gmtime(&R5);
      // la valeur -1 fait le cacul en fonction du decalage au meme moment que la date passee dans now      
//      now->tm_isdst = -1;
//      *pa_Nb_Second = mktime(now);
//#endif

	}
	/*XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_date_Sybase2Ux");*/
	
        return(XDC_OK);
	
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_char_Ux2Sybase_varchar(char *String,CS_VARCHAR * Var_Char)

/*
* ARGUMENTS EN ENTREE :
*
*	String chaine de caractere (C) 
*
* ARGUMENTS EN SORTIE :
* 
*  	Structure de type CS_VARCHAR qui possede deux champs :
*	CS_SMALLINT    len		 Longueur de la chaine
*	CS_CHAR        str[CS_MAX_CHAR]  Chaine en elle meme	
*
* CODE RETOUR : 
*
*	Renvoie XDC_OK	si tout est ok
*		XDC_NOK si il y a eu un probleme.     
*
* CONDITION D'UTILISATION
*
*	Il faut que Var_Char pointe sur une structure allouee
*
* FONCTION 
*	Effectue la conversion entre une chaine au format C
*	vers une chaine Sybase.
------------------------------------------------------*/
{
	/*XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_char_Ux2Sybase_varchar");*/
	
	if (Var_Char == NULL) return (XDC_NOK);
	(*Var_Char).len = strlen(String);																				
	strcpy((*Var_Char).str,String);
	
	/*XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_char_Ux2Sybase_varchar");*/
	return(XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_Sybase_varchar2char_Ux(CS_VARCHAR Var_Char,int Len_String,char *String)

/*
* ARGUMENTS EN ENTREE :
*
*	Structure de type CS_VARCHAR qui possede deux champs :
*		CS_SMALLINT    len		 Longueur de la chaine
*		CS_CHAR        str[CS_MAX_CHAR]  Chaine en elle meme
*	Len_String contient la taille allouee a String
*
* ARGUMENTS EN SORTIE :
* 	
*	String chaine de caractere (C)
*
* CODE RETOUR : 
*
*	Renvoie XDC_OK	si tout est ok
*		XDC_NOK si il y a eu un probleme.     
*
* CONDITION D'UTILISATION
*
*	Il faut que String pointe sur une chaine suffisemment allouee
*
* FONCTION 
*	Effectue la conversion entre une structure VarChar Sybase
*	 vers une chaine au format C.
*	
------------------------------------------------------*/
{
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_Sybase_varchar2char_Ux");
	if (Var_Char.len >  Len_String) return (XDC_NOK);
	strcpy(String,Var_Char.str);
	XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_Sybase_varchar2char_Ux"); 
	return(XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  asql_fetch_row_result permet de parser le retour d un select et
*  de remplir dynamiquement untableau de ligne et de colonne.
*  Chaque cellule du tableau contient une structure contenant le type
*  de la donnee recue, un pointeur sur la valeur, et accessoirement (pour les string) 
*  la longueur.
*  Attention, il faut liberer le tableau passe en argument dans la procedure
*  appelante apres avoir utilise les donnees recues.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_fetch_row_result(CS_COMMAND      *va_cmd,int *va_Nb_Ligne_Lue,int *va_Nb_Col, tg_row_result  *va_Tab_Ligne)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*		*
*	      *   *
*	     *  |  *
*           *   |   *
*          *         *
*         *     *     *
*        *             *
*       *****************
*	Attention : Cette fonction permet de parser le retour 
*	d'un et d'un seul select effectue par la SP appelee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_RETCODE              vl_retcode=CS_SUCCEED;
        CS_INT                  i=0;
        CS_INT                  j=0;
        CS_INT                  vl_row_count=0;
        CS_INT                  vl_rows_read=0;
        int                     vl_cr=0;
        
        CS_DATAFMT              *pl_datafmt;
        tg_row_result		vl_Tab_Ligne_Local;
	
	/*A
	** Initialisation des parametres de retour va_Nb_Ligne_Lue et va_Nb_Col
	*/
	*va_Nb_Ligne_Lue	= 0;
	*va_Nb_Col		= 0;
	
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_fetch_row_result ");
	
	/*A
	** Recupere le nombre de colonnes dans le paquet de resultats
	** et initialisation de la variable de la procedure appelante.
	*/
	vl_retcode = ct_res_info(va_cmd, CS_NUMDATA, va_Nb_Col, CS_UNUSED, NULL);
        if (vl_retcode != CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: ct_res_info() a echoue, code = %d", vl_retcode);
                return vl_retcode;
        }
	/*A
	** On verifie que l'on a au moins une colonne
	*/
	if (*va_Nb_Col <= 0)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_res_info() retourne zero colonne");
                return CS_FAIL;
        }


	/*A
	** On se reserve une structure Datafmt permettant de parser les retours 
	** avec les bons types a recuperer.
	*/
	
	pl_datafmt = (CS_DATAFMT *)malloc( (*va_Nb_Col) * sizeof (CS_DATAFMT));
        if (pl_datafmt == NULL)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : malloc() %d descripteurs a echoue", (*va_Nb_Col));
                return CS_MEM_ERROR;
        }
        
        vl_Tab_Ligne_Local = (tg_Colonne_donnees2 **)malloc(sizeof(tg_Colonne_donnees2 *));
        
        /*A
	** On se reserve le nombre de colonnes (structure tg_Colonne_donnees2) pour la premiere ligne.
	*/
	
	
	vl_Tab_Ligne_Local[0] =(tg_Colonne_donnees2 *) malloc((*va_Nb_Col) * sizeof(tg_Colonne_donnees2 )); 
	
	if (vl_Tab_Ligne_Local[0] == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : allocation pour %d colonnes a echoue", (*va_Nb_Col));
                vl_retcode = CS_MEM_ERROR;         
        }
        
	for (i = 0; i < (*va_Nb_Col); i++)
        {
                /*
                ** On recupere la description de la colonne.  ct_describe() remplie le parametre
                ** pl_datafmt avec la description de la colonne.
                */
                vl_retcode = ct_describe(va_cmd, (i + 1), &pl_datafmt[i]);
                if (vl_retcode != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_describe() a echoue, code = %d", vl_retcode);
                        break;
                }

		/*A
		** Allocation de la premiere ligne du tableau local de lignes, 
		** chaque ligne contenant les colonnes recuperees de la SP.
		*/
		
                /*vl_Tab_Ligne_Local[0][i] = (tg_Colonne_donnees2 *) malloc(sizeof(tg_Colonne_donnees2));*/
                
				
		
                /*A
                ** Allocation du champ pt_value de la structure colonne en fonction du type contenu dans pl_datafmt
                ** renvoyee par la SP.
                */
                
     		vl_retcode = asql_alloc_Col_Type(&pl_datafmt[i], &(vl_Tab_Ligne_Local[0][i]));
     		
     	        if (vl_retcode != XDC_OK)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : asql_alloc_Col_Type n a pas su reconnaitre le type de la colonne rendue par la SP ou probleme d allocation memoire.");
                        break;
                }  
		/*A
                ** Now bind.
                */
                
                vl_retcode = ct_bind(va_cmd, (i + 1), &pl_datafmt[i], vl_Tab_Ligne_Local[0][i].pt_value, NULL, NULL);
                if (vl_retcode != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_bind() a echoue, code = %d", vl_retcode);
                        break;
                }

	}/* Fin du for */
	
	
	*va_Nb_Ligne_Lue 	= 1;
	*va_Nb_Col		= i;
	
	if (vl_retcode != CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_bind a echoue : liberation de la memoire");
                free(pl_datafmt);
                return vl_retcode;
        }
	/*A
	**  On parse les resultats pour remplir nos structures de donnees
	*/
	while (((vl_retcode = ct_fetch(va_cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED,
                        &vl_rows_read)) == CS_SUCCEED) || (vl_retcode == CS_ROW_FAIL))
        {
                /*
                ** Incrementation notre compteur de ligne par le nombre de lignes qui
                ** viennent etre fetchees .
                */
                vl_row_count = vl_row_count + vl_rows_read;
                /*
                ** Test si on a detecte une erreur connue.
                */
                if (vl_retcode == CS_ROW_FAIL)
                {
                	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : Erreur sur la ligne %ld.\n",vl_row_count);
                     
                }
                /*A
                ** Allocation de la deuxieme ligne
                */
                vl_Tab_Ligne_Local = (tg_Colonne_donnees2 **) realloc(vl_Tab_Ligne_Local ,(vl_row_count + 1) * sizeof(tg_Colonne_donnees2 *));
                vl_Tab_Ligne_Local[vl_row_count] =(tg_Colonne_donnees2 *) malloc((*va_Nb_Col) * sizeof(tg_Colonne_donnees2 ));
                        
		for (i = 0; i < (*va_Nb_Col); i++)
        	{
                	/*A
			** Allocation de la ligne suivante dans le tableau local de lignes, 
			** chaque ligne contenant les colonnes recuperees de la SP.
			*/
		
                
                	/*A
                	** Allocation du champ pt_value de la structure colonne en fonction du type contenu dans pl_datafmt
                	** renvoyee par la SP.
                	*/
                	
     			vl_retcode = asql_alloc_Col_Type(&pl_datafmt[i], &(vl_Tab_Ligne_Local[vl_row_count][i]));
     	        	if (vl_retcode != XDC_OK)
                	{
                        	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : asql_alloc_Col_Type n a pas su reconnaitre le type de la colonne rendue par la SP ou probleme d allocation memoire.");
                        	break;
                	}  
			/*A
                	** Now bind.
                	*/
                
                	vl_retcode = ct_bind(va_cmd, (i + 1), &pl_datafmt[i], vl_Tab_Ligne_Local[vl_row_count][i].pt_value, NULL, NULL);
                	if (vl_retcode != CS_SUCCEED)
                	{
                        	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_bind() a echoue, code = %d", vl_retcode);
                        	break;
                	}
		
		}/* Fin du for */
	
                
        }/* Fin du while */
        
	/*A
	** Desallocation de la pl_datafmt pour parser le retour
	*/
	free(pl_datafmt);
	/*A
	** Desallocation de la ligne alloue en trop
	*/
	for ( i =0 ; i <(*va_Nb_Col); i++)
	{
		asql_free_Col_Type(&(vl_Tab_Ligne_Local[vl_row_count][i]));
		/*free(vl_Tab_Ligne_Local[vl_row_count][i].pt_value);*/
	}
	free(vl_Tab_Ligne_Local[vl_row_count]);

	
	/*A
	**  Fin du parsing des datas : on regarde les codes de retour 
	**  de la RP via les codes de retour de ct_fetch()
	*/
	
	switch ((int)vl_retcode)
        {
                case CS_END_DATA:
                        /*
                        ** Tout est ok.
                        */
                        XZST_03EcritureTrace(XZSTC_INFO, "asql_fetch_row_result : %d lignes\n", vl_row_count);
                  
                        vl_retcode = CS_SUCCEED;
                        break;

                case CS_FAIL:
                        /*
                        ** Cela n a pas marche du tout.
                        */
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_fetch() a echoue (CS_FAIL)");
                        return vl_retcode;
                        break;
                default:
                        /*
                        ** Valeur de retour non attendue.
                        */
                        XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result : ct_fetch() retourne le code inattendu %d", vl_retcode);
                        return vl_retcode;
                        break;

        }
        
/* Decommenter pour avoir le contenu du select retourne par la sp, Affiche.

        vl_cr =  asql_Affiche_Tab_Ligne_Results(vl_row_count,(*va_Nb_Col),vl_Tab_Ligne_Local);
        if (vl_cr != CS_SUCCEED)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"asql_fetch_row_result: asql_Affiche_Tab_Ligne_Results n a pu afficher correctement les lignes de colonnes results");
        }
*/
        
	*va_Nb_Ligne_Lue 	= vl_row_count;
 	*va_Tab_Ligne		= vl_Tab_Ligne_Local;
 	
 	XZST_03EcritureTrace(XZSTC_SGBD,"OUT : asql_fetch_row_result retourne %d", vl_retcode);
        return vl_retcode;
	
	
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_alloc_Col_Type : Alloue le pointeur Data 
*  (sur le type contenu dans column->datatype),
*  dans le quel on doit recevoir la colonne 
*  quand on parse le CS_ROW_RESULT 
*  (renvoye par une stored procedure qui fait un select par exemple).
*  L'utilisation de cette fonction se fait dans asql_fetch_row_result.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_alloc_Col_Type(CS_DATAFMT *column,tg_Colonne_donnees2 *pa_Data)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	/*XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_alloc_Col_Type");*/
	
	(*pa_Data).indicator = column->datatype;
	(*pa_Data).valuelen  = column->maxlength;
	
	switch ((int) column->datatype)
	{
		case CS_CHAR_TYPE:
			
			(*pa_Data).valuelen ++;
			column->maxlength ++;
			
			(*pa_Data).pt_value = (CS_CHAR *) malloc(sizeof(CS_CHAR) * ( column->maxlength ) );
			
		        column->format=CS_FMT_NULLTERM; /* Pour forcer la chaine rendue 
							** avec un \0 en terminaison de chaine
							*/
			/* Franck !!!!!! Pb reception chaine */
			column->format=CS_FMT_NULLTERM; /* Pour forcer la chaine rendue 
							** avec un \0 en terminaison de chaine
							*/
			break;
			
		case CS_VARCHAR_TYPE:
			
			(*pa_Data).valuelen ++;
			column->maxlength ++;
			
			(*pa_Data).pt_value = (CS_VARCHAR *) malloc(sizeof(CS_VARCHAR) + 1);
			
			column->format=CS_FMT_NULLTERM; /* Pour forcer la chaine rendue 
							** avec un \0 en terminaison de chaine
							*/
			break;
			
		case CS_TEXT_TYPE:
			
			(*pa_Data).valuelen ++;
			column->maxlength ++;
			
			(*pa_Data).pt_value  = (CS_TEXT *) malloc(sizeof(CS_TEXT) * (column->maxlength));
			column->format=CS_FMT_NULLTERM; /* Pour forcer la chaine rendue 
							** avec un \0 en terminaison de chaine
							*/
			break;
			
		case CS_IMAGE_TYPE:
			
			(*pa_Data).valuelen ++;
			column->maxlength ++;
			
			(*pa_Data).pt_value  = (CS_IMAGE *) malloc(sizeof(CS_IMAGE) * (column->maxlength));
			break;

		case CS_BINARY_TYPE:
			
			(*pa_Data).valuelen ++;
			column->maxlength ++;
			
			(*pa_Data).pt_value  = (CS_BINARY *) malloc(sizeof(CS_BINARY) * (column->maxlength));
			break;
			
		case CS_VARBINARY_TYPE:
			
			(*pa_Data).pt_value  = (CS_VARBINARY *) malloc(sizeof(CS_VARBINARY));
			break;

		case CS_BIT_TYPE:
			
			(*pa_Data).pt_value  = (CS_BIT *) malloc(sizeof(CS_BIT));
			break;
			
		case CS_TINYINT_TYPE:
			
			(*pa_Data).pt_value  = (CS_TINYINT *) malloc(sizeof(CS_TINYINT));
			break;

		case CS_SMALLINT_TYPE:
			(*pa_Data).pt_value  = (CS_SMALLINT *) malloc(sizeof(CS_SMALLINT));
			break;
			
		case CS_INT_TYPE:
			(*pa_Data).pt_value  = (CS_INT *) malloc(sizeof(CS_INT));
			break;

		case CS_REAL_TYPE:
			(*pa_Data).pt_value  = (CS_REAL *) malloc(sizeof(CS_REAL));
			break;
			
		case CS_FLOAT_TYPE:
			(*pa_Data).pt_value = (CS_FLOAT *) malloc(sizeof(CS_FLOAT));
			break;

		case CS_MONEY_TYPE:
			(*pa_Data).pt_value = (CS_MONEY *) malloc(sizeof(CS_MONEY));
			break;
			
		case CS_MONEY4_TYPE:
			(*pa_Data).pt_value = (CS_MONEY4 *) malloc(sizeof(CS_MONEY4));
			break;

		case CS_DATETIME_TYPE:
			(*pa_Data).pt_value = (CS_DATETIME *) malloc(sizeof(CS_DATETIME));
			break;
			
		case CS_DATETIME4_TYPE:
			(*pa_Data).pt_value = (CS_DATETIME4 *) malloc(sizeof(CS_DATETIME4));
			break;

		case CS_NUMERIC_TYPE:
			(*pa_Data).pt_value = (CS_NUMERIC *) malloc(sizeof(CS_NUMERIC));
			break;
			
		case CS_DECIMAL_TYPE:
			(*pa_Data).pt_value = (CS_DECIMAL *) malloc(sizeof(CS_DECIMAL));
			break;

		default:
			(*pa_Data).pt_value = NULL;	/* 
					** Permet de tester qu'il ya eu un probleme
					** pour reconnaitre le type
					*/
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_alloc_Col_Type : Le type de donnees (%d) rendu par la SP n'est pas connu : Pas d allocation",column->datatype);
			break;
	}

	if ((*pa_Data).pt_value == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_alloc_Col_Type : Probleme d allocation pour la data des lignes de retour de SP");
		return(XDC_NOK);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_alloc_Col_Type retourne %d",XDC_OK);*/
	
	return(XDC_OK);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*  asql_Free_Tab_Ligne_Results permet de liberer le tableau alloue dynamiquement 
*  par asql_fetch_raw_results.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_Free_Tab_Ligne_Results(int va_Nb_Lignes, int va_Nb_Col,tg_row_result *va_Tab_Lignes_Colonnes)

/*
* ARGUMENTS EN ENTREE :
*   
*	Tab_Lignes_Colonnes qui pointe sur le tableau de type tg_row_result alloue
*	dans asql_fetch_row_result
*
* ARGUMENTS EN SORTIE :
*   
*	*Tab_Lignes_Colonnes est mis a NULL.
*
* CODE RETOUR : 
*
*   	XDC_OK
*	XDC_NOK
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   	Desallocation de la structure qui a permis de recuper dans des variables C
*	le retour d un select effectue par une SP.
*
------------------------------------------------------*/
{
	int i,j;
	tg_row_result vl_Tab_Lig_Col;
	vl_Tab_Lig_Col = *va_Tab_Lignes_Colonnes;
	
	/*! Fonction a bien verifier avec purify */
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_Free_Tab_Ligne_Results : Nb Lignes = %d, Nb Col = %d",va_Nb_Lignes,va_Nb_Col);
	for (i=0;i<va_Nb_Lignes;i++)
	{
		for (j=0;j<va_Nb_Col;j++)
		{
			asql_free_Col_Type(&(vl_Tab_Lig_Col[i][j]));
			/*free(vl_Tab_Lig_Col[i][j].pt_value);*/
		}
		free(vl_Tab_Lig_Col[i]);
	}
	
	if (vl_Tab_Lig_Col != NULL)
	{
		free(vl_Tab_Lig_Col);
	}
	
	*va_Tab_Lignes_Colonnes=NULL;
	XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_Free_Tab_Ligne_Results retourne %d",XDC_OK);
	return(XDC_OK);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Desalloue le pointeur Data (sur le type contenu dans column->datatype),
*  dans le quel on a recu la colonne parsee par asql_fetch_row_result
* .
* L'utilisation de cette fonction se fait dans asql_free_row_result.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_free_Col_Type(tg_Colonne_donnees2 *pa_Data)

/*
* ARGUMENTS EN ENTREE :
*   	<  
*	pa_Data	pointe sur une structure qui contient un pointeur que l on veut desallouer.
*	>
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   	XDC_OK
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*
*
------------------------------------------------------*/
{
	int		vl_cr = XDC_OK;
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "IN  : asql_free_Col_Type ");*/
	
	switch ((int) (*pa_Data).indicator)
	{
		case CS_CHAR_TYPE:
			free((CS_CHAR *)(*pa_Data).pt_value);
			break;
			
		case CS_VARCHAR_TYPE:
			free((CS_VARCHAR *)(*pa_Data).pt_value ); 
			break;
			
		case CS_TEXT_TYPE:
			free((CS_TEXT *)(*pa_Data).pt_value);
			break;
			
		case CS_IMAGE_TYPE:
			free((CS_IMAGE *)(*pa_Data).pt_value); 
			break;

		case CS_BINARY_TYPE:
			free((CS_BINARY * )(*pa_Data).pt_value);  
			break;
			
		case CS_VARBINARY_TYPE:
			free((CS_VARBINARY *)(*pa_Data).pt_value); 
			break;

		case CS_BIT_TYPE:
			free((CS_BIT  *)(*pa_Data).pt_value); 
			break;
			
		case CS_TINYINT_TYPE:
			free((CS_TINYINT *)(*pa_Data).pt_value );
			break;

		case CS_SMALLINT_TYPE:
			free((CS_SMALLINT *)(*pa_Data).pt_value);
			break;
			
		case CS_INT_TYPE:
			free((CS_INT *)(*pa_Data).pt_value );
			break;

		case CS_REAL_TYPE:
			free((CS_REAL *)(*pa_Data).pt_value );
			break;
			
		case CS_FLOAT_TYPE:
			free((CS_FLOAT *)(*pa_Data).pt_value );
			break;

		case CS_MONEY_TYPE:
			free((CS_MONEY *)(*pa_Data).pt_value);
			break;
			
		case CS_MONEY4_TYPE:
			free((CS_MONEY4 *)(*pa_Data).pt_value );
			break;

		case CS_DATETIME_TYPE:
			free((CS_DATETIME *)(*pa_Data).pt_value);
			break;
			
		case CS_DATETIME4_TYPE:
			free((CS_DATETIME4 *)(*pa_Data).pt_value);
			break;

		case CS_NUMERIC_TYPE:
			free((CS_NUMERIC *)(*pa_Data).pt_value);
			break;
			
		case CS_DECIMAL_TYPE:
			free((CS_DECIMAL *)(*pa_Data).pt_value);
			break;

		default:
			/* 
			** Permet de tester qu'il ya eu un probleme
			** pour reconnaitre le type
			*/
			XZST_03EcritureTrace(XZSTC_WARNING,"asql_free_Col_Type : Le type de donnees (%d) rendu par la SP n'est pas connu :desallocation de la taille d'un pointeur sur void !!",(*pa_Data).indicator);
			vl_cr = XDC_NOK;
			free((*pa_Data).pt_value );
			break;
	}

	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_free_Col_Type retourne %d",vl_cr);*/
	
	return(vl_cr);
}


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* asql_Affiche_Tab_Ligne_Results permet d afficher le resultat rendu par le ct_fetch 
* sur les lignes rendus par exemple sur un select effectue par une stored procedure.
*
* L'utilisation de cette fonction se fait dans asql_fetch_row_result.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_Affiche_Tab_Ligne_Results(int va_Nb_Lignes, int va_Nb_Col,tg_row_result va_Tab_Lignes_Colonnes)

/*
* ARGUMENTS EN ENTREE :
*   
*	Tab_Lignes_Colonnes qui pointe sur le tableau de type tg_row_result alloue
*	dans asql_fetch_row_result
*
* ARGUMENTS EN SORTIE :
*   
*	*Tab_Lignes_Colonnes est mis a NULL.
*
* CODE RETOUR : 
*
*   	XDC_OK
*	XDC_NOK
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   	Fait appel a asql_Affiche_Col_Fct_Type qui affiche la col en fct du type de la colonne.
*
------------------------------------------------------*/
{
	int i,j;
	int vl_retcode;
	/*XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_Affiche_Tab_Ligne_Results : Nb Lignes = %d, Nb Col = %d",va_Nb_Lignes,va_Nb_Col);*/
	for (i=0;i<va_Nb_Lignes;i++)
	{
		for (j=0;j<va_Nb_Col;j++)
		{
			vl_retcode = asql_Affiche_Col_Fct_Type(i, j, va_Tab_Lignes_Colonnes[i][j] );
		}
		
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_Affiche_Tab_Ligne_Results retourne %d",XDC_OK);*/
	return(CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*  asql_Affiche_Col_Fct_Type met en forme l affichage des donnees du tableau
*  en fonction du type de la donnees.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int asql_Affiche_Col_Fct_Type(int va_num_ligne,int va_num_col,tg_Colonne_donnees2   pa_Col)

/*
* ARGUMENTS EN ENTREE :
*   
*	pa_Col pointe vers une structure contenant dans le champ pt_value la valeur a afficher
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Utilise la champ indicator pour connaitre le type de la valeur pointe par le champ pt_value.
*   En fonction de ce champ indicator, on affiche le contenu pointe par pt_value.
*
------------------------------------------------------*/
{
	int 			vl_retcode;
	tg_Colonne_donnees2 	vl_col = pa_Col;
	
	void *pl_valeur;	/*
				** Variable intermediaire servant a afficher les sous champs de
				** de pt_value quand pt_value pointe sur une structure
				*/
	char str[250];
	/*XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_Affiche_Col_Fct_Type");*/
	
	switch ((int) vl_col.indicator )
	{
		case CS_CHAR_TYPE:
			
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d =\t\t[%d]\n",va_num_ligne,va_num_col,*(CS_CHAR *)(vl_col.pt_value));
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d =\t\t[%s]\n",va_num_ligne,va_num_col,(CS_CHAR *)(vl_col.pt_value));
			strcpy(str,(CS_CHAR *)(vl_col.pt_value));
			
			/*XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d =\t\t%s\n",va_num_ligne,va_num_col,str);*/
			break;
			
		case CS_VARCHAR_TYPE:
			pl_valeur = (CS_VARCHAR *) vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs len= \t%d\n",va_num_ligne,va_num_col,((CS_VARCHAR *)pl_valeur)->len);
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs str= \t%s\n",va_num_ligne,va_num_col,((CS_VARCHAR *)pl_valeur)->str);
			break;
			
		case CS_TEXT_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d =\t%s\n",*(CS_TEXT *)(vl_col.pt_value));		
			break;
			
		case CS_IMAGE_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d =\t%c\n",*(CS_CHAR *)(vl_col.pt_value));
			break;

		case CS_BINARY_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d =\t%d\n",*(CS_BINARY *)(vl_col.pt_value));
			break;
			
		case CS_VARBINARY_TYPE:
			pl_valeur = (CS_VARBINARY *) vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs len= \t%d\n",va_num_ligne,va_num_col,((CS_VARBINARY *)pl_valeur)->len);
			break;

		case CS_BIT_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d = \t%d\n",va_num_ligne,va_num_col,*(CS_TINYINT *)(vl_col.pt_value));
			break;
			
		case CS_TINYINT_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d = \t%d\n",va_num_ligne,va_num_col,*(CS_TINYINT *)(vl_col.pt_value));
			break;

		case CS_SMALLINT_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d = \t%d\n",va_num_ligne,va_num_col,*(CS_SMALLINT *)(vl_col.pt_value));
			break;

		case CS_INT_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d = \t%d\n",va_num_ligne,va_num_col,*(CS_INT *)(vl_col.pt_value));
			break;

		case CS_REAL_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d = \t%f\n",va_num_ligne,va_num_col,*(CS_REAL *)(vl_col.pt_value));
			break;
			
		case CS_FLOAT_TYPE:
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d = \t%f\n",va_num_ligne,va_num_col,*(CS_FLOAT *)(vl_col.pt_value));
			break;

		case CS_MONEY_TYPE:
			pl_valeur = (CS_MONEY *) vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs mnyhigh= \t%d\n",va_num_ligne,va_num_col,((CS_MONEY *)pl_valeur)->mnyhigh);
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs mnylow= \t%d\n",va_num_ligne,va_num_col,((CS_MONEY *)pl_valeur)->mnylow);
			break;
			
		case CS_MONEY4_TYPE:
			pl_valeur = (CS_MONEY4 *) vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs mny4= \t%d\n",va_num_ligne,va_num_col,((CS_MONEY4 *)pl_valeur)->mny4);
			
			break;

		case CS_DATETIME_TYPE:
			pl_valeur = (CS_DATETIME *) vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs dtdays= \t%d\n",va_num_ligne,va_num_col,((CS_DATETIME *)pl_valeur)->dtdays);
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs dttime= \t%d\n",va_num_ligne,va_num_col,((CS_DATETIME *)pl_valeur)->dttime);
			break;
			
		case CS_DATETIME4_TYPE:
			pl_valeur = (CS_DATETIME *) vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs days= \t%d\n",va_num_ligne,va_num_col,((CS_DATETIME4 *)pl_valeur)->days);
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs minutes= \t%d\n",va_num_ligne,va_num_col,((CS_DATETIME4 *)pl_valeur)->minutes);

			break;

		case CS_NUMERIC_TYPE:
			pl_valeur = (CS_NUMERIC *)vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs precision= \t%d\n",va_num_ligne,va_num_col,((CS_NUMERIC *)pl_valeur)->precision);
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs scale= \t%d\n",va_num_ligne,va_num_col,((CS_NUMERIC *)pl_valeur)->scale);
			break;
			
		case CS_DECIMAL_TYPE:
			pl_valeur = (CS_DECIMAL *)vl_col.pt_value;
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs precision= \t%d\n",va_num_ligne,va_num_col,((CS_DECIMAL *)pl_valeur)->precision);
			XZST_03EcritureTrace(XZSTC_SGBD,"Ligne %d Col %d champs scale= \t%d\n",va_num_ligne,va_num_col,((CS_DECIMAL *)pl_valeur)->scale);
			break;

		default:
			
			break;
	}
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_Affiche_Col_Fct_Type retourne %d",XDC_OK);*/
	return (XDC_OK);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*   asql_ctparam_tinyint_input : Encapsule l'appel a ct_param 
*   pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_tinyint_input(CS_COMMAND *pa_cmd, CS_TINYINT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_tinyint_input %d", (int) *pa_param);
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_TINYINT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_tinyint_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_tinyint_input retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_smallint_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_smallint_input(CS_COMMAND *pa_cmd, CS_SMALLINT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	XDC_OK  si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_smallint_input %d", (int) *pa_param);
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));

	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_SMALLINT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_smallint_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_smallint_input retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_int_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_int_input(CS_COMMAND *pa_cmd, CS_INT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_int_input %d", (int) *pa_param);
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_INT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_int_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_int_input retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_bit_input : Encapsule l'appel a ct_param
*  pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_bit_input(CS_COMMAND *pa_cmd, CS_BIT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_bit_input %d", (int) *pa_param);
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_BIT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_bit_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_bit_input retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_date4_input : Encapsule l'appel a ct_param
*  pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_date4_input(CS_COMMAND *pa_cmd, CS_DATETIME4 * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED  si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_date4_input");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_DATETIME4_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_DATETIME4), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_date4_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_date4_input retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_date_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_date_input(CS_COMMAND *pa_cmd, CS_DATETIME * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_date_input %d,%d", (int) pa_param->dtdays, (int) pa_param->dttime);
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_DATETIME_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_DATETIME), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_date_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode) ;
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_date_input retourne %d", vl_retcode);*/
	return  (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_char_input : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Input value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_char_input(CS_COMMAND *pa_cmd, CS_CHAR * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_char_input %s", pa_param);
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));

	char *vl_param = (char *) pa_param;
	if (vl_param==NULL)
		strcpy(vl_param,"");

	if (va_nomparam!=NULL)
		strcpy(pl_datafmt.name,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_CHAR_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = (CS_INT) strlen(vl_param);
	pl_datafmt.status    = CS_INPUTVALUE;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	/*! Franck : ajout pour reception param */
	
	pl_datafmt.format    = CS_FMT_NULLTERM;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)vl_param, (CS_INT)strlen(vl_param), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_char_input : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_char_input retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_tinyint_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_tinyint_output(CS_COMMAND *pa_cmd, CS_TINYINT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	XDC_NOK  ou autre si il y a eu un probleme.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_tinyint_output");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
					
	*pa_param = 0;
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_TINYINT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = 255;
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_TINYINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_tinyint_output : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_tinyint_output retourne %d", vl_retcode);*/
	return (CS_SUCCEED) ;
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_smallint_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_smallint_output(CS_COMMAND *pa_cmd, CS_SMALLINT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_smallint_output");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	*pa_param = 0;
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_SMALLINT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = 255;
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_SMALLINT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_smallint_output : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_smallint_output retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_int_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_int_output(CS_COMMAND *pa_cmd, CS_INT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_int_output");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	*pa_param = 0;
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_INT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = 255;
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_INT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_int_output : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_int_output retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_int_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_bit_output(CS_COMMAND *pa_cmd, CS_BIT * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_bit_output");

	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));

	*pa_param = 0;

	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_BIT_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = CS_UNUSED;
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;

	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)pa_param, CS_SIZEOF(CS_BIT), CGS_PARAM_INDIC)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_bit_output : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_bit_output retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_date_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_date_output(CS_COMMAND *pa_cmd, CS_DATETIME * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_date_output");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	pa_param->dtdays = 0;
	pa_param->dttime = 0;
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_DATETIME_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = 255;
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)NULL, 0, -1)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_date_output : ct_param() a echoue, code = %d", vl_retcode);
		return(vl_retcode) ;
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_date_output retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
* 	Encapsule l'appel a ct_param pour les parametres a passer a une SP 
*	en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_date4_output(CS_COMMAND *pa_cmd, CS_DATETIME4 * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	Si un probleme est survenu, on retourne le code SYBASE.
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode=CS_SUCCEED;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_date4_output");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	pa_param->days = 0;
	pa_param->minutes = 0;
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_DATETIME4_TYPE; 	/* Utiliser des types Sybase   */
	pl_datafmt.maxlength = 255;
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)NULL, 0, -1)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_date4_output : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_date4_output retourne %d", vl_retcode);*/
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_ctparam_char_output : Encapsule l'appel a ct_param 
*  pour les parametres a passer a une SP en Output value. 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_ctparam_char_output(CS_COMMAND *pa_cmd, CS_CHAR * pa_param,char *va_nomparam)

/*
* ARGUMENTS EN ENTREE :
*   	pa_cmd		Structure de commande a envoyer a la Stored Procedure.
*	pa_param	Pointe sur la valeur a passer en entree a la Stored Procedure
*	va_nomparam	chaine contenant le nom du parametre de la SP (par ex :"@value").
*
* ARGUMENTS EN SORTIE :
*   	pa_cmd		Structure de commande a envoyer au SQL serveur.
*
* CODE RETOUR : 
*
*	renvoie les codes de retour du ct_param :
*	CS_SUCCEED si tout est OK
*	sinon code de retour de sybase .
*
* CONDITION D'UTILISATION
*   	pa_cmd		doit avoir ete alouee
*	pa_param	doit pointee vers une valeur alouee et renseignee.
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_DATAFMT      pl_datafmt;
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_ctparam_char_output");
	
	/*
	** Initialisation a zero de la structure pl_datafmt
	*/
	memset(&pl_datafmt,0,sizeof(CS_DATAFMT));
	
	strcpy(pl_datafmt.name ,va_nomparam);
	pl_datafmt.namelen   = CS_NULLTERM;
	pl_datafmt.datatype  = CS_CHAR_TYPE; 	/* Utiliser des types Sybase   */
	
	/*! Franck : Ajout pour reception param */
	pl_datafmt.maxlength = CS_SIZEOF(CS_CHAR);
	/*pl_datafmt.maxlength = CS_MAX_CHAR;*/
//XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_char_output : sizeof(pa_param)= %d / sizeof(*pa_param)= %d / pa_param=[%s]", sizeof(pa_param),sizeof(*pa_param),pa_param);
	
	/*pl_datafmt.maxlength = siezeof(pa_param);*/
	
	pl_datafmt.status    = CS_RETURN;   	/* Uniquement valeur en entree*/
	pl_datafmt.locale    = NULL;
	pl_datafmt.format    = CS_FMT_NULLTERM;
	
	if ((vl_retcode = ct_param(pa_cmd, &pl_datafmt, (CS_VOID *)NULL, 0, -1)) != CS_SUCCEED)
			
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_ctparam_char_output : ct_param() a echoue, code = %d", vl_retcode);
		return (vl_retcode);
	}
	
	XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_ctparam_char_output retourne %d", vl_retcode);
	return (CS_SUCCEED);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_clientmsg_cbHandler d erreur pour le Open Client
*  Quand une erreur est detectee par la CT lib
*  On l affiche.(provient de exutils.c des sample ctlib de Sybase).
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_clientmsg_cb(	CS_CONTEXT      *pa_context,
				CS_CONNECTION   *pa_connection,
				CS_CLIENTMSG    *pa_errmsg)

/*
* ARGUMENTS EN ENTREE :
*   	pa_context pointe sur le pa_contexte
*	pa_connection pointe sur la structure decrivant la connexion au serveur SQL
*	pa_errmsg pointe sur la structure contenant le message logue par la CTlib
*
* ARGUMENTS EN SORTIE :
*
*	Aucun apart l affichage de l erreur.
*
* CODE RETOUR :
* 
*	CS_SUCCEED
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
------------------------------------------------------*/
{
	int		vl_Niveau_Trace = XZSTC_WARNING ; 	/* 
								** Initialise a Warning par defaut.
								*/
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_clientmsg_cb");
	/* A
	** vl_Niveau_Trace positionne un niveau de trace en fonction de la severite du message reµu.
	*/
	
	if (pa_errmsg->severity < XDC_SGBD_WARNING) vl_Niveau_Trace=XZSTC_SGBD;
	else if (pa_errmsg->severity >= XDC_SGBD_WARNING) vl_Niveau_Trace=XZSTC_WARNING;

 	XZST_03EcritureTrace(vl_Niveau_Trace,"Message de l Open Client :");
 	
        XZST_03EcritureTrace(vl_Niveau_Trace,"numero de message : COUCHE = (%ld) ORIGINE = (%ld) ",
                CS_LAYER(pa_errmsg->msgnumber), CS_ORIGIN(pa_errmsg->msgnumber));
                
        XZST_03EcritureTrace(vl_Niveau_Trace,"SEVERITE = (%ld) NOMBRE = (%ld)",
                CS_SEVERITY(pa_errmsg->msgnumber), CS_NUMBER(pa_errmsg->msgnumber));
                
        XZST_03EcritureTrace(vl_Niveau_Trace,"Libelle du message: %s", pa_errmsg->msgstring);
        
        if (pa_errmsg->osstringlen > 0)
        {
                XZST_03EcritureTrace(vl_Niveau_Trace, "Erreur de l Operating : %s",pa_errmsg->osstring);
        }
        
        
	XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_clientmsg_cb retourne %d",CS_SUCCEED );
	return CS_SUCCEED;
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_servermsg_cb : Handler d erreur pour le Open Client
*  Quand une erreur est detectee par la CT lib
*  On l affiche.(provient de exutils.c des sample ctlib de Sybase.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_servermsg_cb(	CS_CONTEXT      * pa_context,
				CS_CONNECTION   *pa_connection,
				CS_SERVERMSG    *pa_srvmsg)

/*
* ARGUMENTS EN ENTREE :
*   	pa_context pointe sur le contexte
*	pa_connection pointe sur la structure decrivant la connexion au serveur SQL
*	pa_srvmsg pointe sur la structure contenant le message logue par le serveur
*
* ARGUMENTS EN SORTIE :
*
*	Aucun a part l affichage de l erreur.
*
* CODE RETOUR :
* 
*	CS_SUCCEED
*	CS_FAIL         If we could not get the extended error command handle
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_COMMAND      *cmd;
        CS_RETCODE      vl_retcode;
	int		vl_Niveau_Trace = XZSTC_WARNING ; 	/* 
								** Initialise a Wraning par defaut.
								*/
	
	XZST_03EcritureTrace(XZSTC_SGBD, "IN : asql_servermsg_cb");
	
	/* A
	** vl_Niveau_Trace positionne un niveau de trace en fonction de la severite du message reµu.
	*/
	
	if (pa_srvmsg->severity < XDC_SGBD_WARNING) vl_Niveau_Trace=XZSTC_SGBD;
	else if (pa_srvmsg->severity >= XDC_SGBD_WARNING) vl_Niveau_Trace=XZSTC_WARNING;
	
	XZST_03EcritureTrace(vl_Niveau_Trace, "Server message:");
	
        XZST_03EcritureTrace(vl_Niveau_Trace, "Message number: %ld, Severity %ld, ",pa_srvmsg->msgnumber, pa_srvmsg->severity);
        
        
        XZST_03EcritureTrace(vl_Niveau_Trace,"State %ld, Line %ld",pa_srvmsg->state, pa_srvmsg->line);
	
        if (pa_srvmsg->svrnlen > 0)
        {
                XZST_03EcritureTrace(vl_Niveau_Trace,"Server '%s'", pa_srvmsg->svrname);             
        }

        if (pa_srvmsg->proclen > 0)
        {
                XZST_03EcritureTrace(vl_Niveau_Trace, " Procedure '%s'", pa_srvmsg->proc);               
        }

        XZST_03EcritureTrace(vl_Niveau_Trace,"libelle du message: %s", pa_srvmsg->text);
	
	
        if (pa_srvmsg->status & CS_HASEED)
        {
                XZST_03EcritureTrace(vl_Niveau_Trace,"\n[Start Extended Error]\n");
                
                if (ct_con_props(pa_connection, CS_GET, CS_EED_CMD, &cmd, CS_UNUSED, NULL) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING, "asql_servermsg_cb: ct_con_props(CS_EED_CMD) a echoue");
                        return CS_FAIL;
                }
                
                vl_retcode = asql_fetch_data(cmd);
                XZST_03EcritureTrace(vl_Niveau_Trace,"\n[End Extended Error]\n\n");
        }
        else
        {
                vl_retcode = CS_SUCCEED;
        }
        
	XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_servermsg_cb retourne %d",vl_retcode );
	return vl_retcode;
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_clean_cmd : encapsule ct_drop_cmd afin de liberer la commande alouee par
*  ct_cmd_alloc. Si ct_drop_cmd echoue, on cancele la commande en cours sur
*  laquelle on a un probleme d execution et qui reste en attente (blocage de l application)
*  via ct_cancel. Puis on refait un essai de drop de la commande. Si elle echoue on sort en
*  fatal.
*  La fonction ne retourne rien. Si son execution se passe mal, alors on trace en 
*  fatal : le noyau de supervision doit arreter la tache.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void asql_clean_cmd(		CS_CONNECTION   *pa_connection,
				CS_COMMAND    	*pa_cmd)

/*
* ARGUMENTS EN ENTREE :
*   	
*	pa_connection pointe sur la structure decrivant la connexion au serveur SQL
*	pa_cmd pointe sur la structure contenant la commande a netoyer et liberer .
*
* ARGUMENTS EN SORTIE :
*
* 	pa_cmd pointera vers une structure desalouee.
*
* CODE RETOUR :aucun 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_RETCODE	vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_clean_cmd");
	/*A
	** Essai de liberation d une commande n ayant pas de probleme : 
	** qui n est pas bloquee en attente...
	*/
	if ((vl_retcode = ct_cmd_drop(pa_cmd)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING," asql_clean_cmd : ct_cmd_drop a retourne %d, cancel commande",vl_retcode);
		if ((vl_retcode = ct_cancel ( NULL , pa_cmd, CS_CANCEL_ALL )) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"asql_clean_cmd : ct_cancel a retourne %d : \n\t\tLa tentative pour canceler \n\t\tla commande en cours a echoue : Demande l arret du client",vl_retcode);
		}
		if ((vl_retcode = ct_cmd_drop(pa_cmd)) != CS_SUCCEED)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"asql_clean_cmd : ct_cmd_drop a echoue avec code %d : \n\t\tmalgre le ct_cancel effectue avec succes :\n\t\t ARRET de l OPEN Client", vl_retcode);
		}
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_clean_cmd");*/
	return ;
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_cmd_alloc : encapsule ct_cmd_alloc.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_cmd_alloc(	CS_CONNECTION	*pa_connection,
				CS_COMMAND    	**pa_cmd)

/*
* ARGUMENTS EN ENTREE :
*   	
*	pa_connection pointe sur la structure decrivant la connexion au serveur SQL
*	pa_cmd pointe sur la structure contenant la commande a netoyer et liberer .
*
* ARGUMENTS EN SORTIE :
*
* 	pa_cmd pointera vers une structure desalouee.
*
* CODE RETOUR :aucun 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_RETCODE	vl_retcode = CS_SUCCEED;
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_cmd_alloc");
	if((vl_retcode = ct_cmd_alloc( pa_connection ,  pa_cmd )) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"asql_cmd_alloc : ct_cmd_alloc a echoue et a retourne %d",vl_retcode);
		
	}
	
	/*XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_cmd_alloc retourne %d",vl_retcode);*/
	return (vl_retcode);
}




/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  asql_Taitement_Result : encapsule ct_results avec son traitement associe :
*	appel au divers asql_fetch*.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE asql_Taitement_Result(	CS_COMMAND    		*pa_cmd,
					XDY_Entier      	*pa_SPstatus,
					int 			va_num_param_retour,
					tg_Colonne_donnees2	 *pa_Tab_param_retour,
					int			*pa_Nb_Ligne_Lue,
					int			*pa_Nb_Col,
					tg_row_result 		*pa_Tab_retour_select)

/*
* ARGUMENTS EN ENTREE :
*   	
*	pa_cmd pointe sur la structure contenant la commande a nettoyer et liberer .
*
* ARGUMENTS EN SORTIE :
*
* 	pa_cmd pointera vers une structure desalouee.
*
* CODE RETOUR : XDC_OK  
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	CS_RETCODE	vl_retcode = CS_SUCCEED;
	
	CS_INT          vl_res_type;
	CS_SMALLINT    	vl_msg_id;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	
	XZST_03EcritureTrace(XZSTC_SGBD,"IN : asql_Taitement_Result");
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(pa_cmd, &vl_res_type)) == CS_SUCCEED)
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
						vl_retcode = asql_fetch_row_result(pa_cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,pa_Tab_retour_select);
						*pa_Nb_Ligne_Lue	= vl_Nb_Ligne_Lue;
						*pa_Nb_Col 		= vl_Nb_Col;
						
						XZST_03EcritureTrace(XZSTC_INFO,"asql_Taitement_Result: ROW RESULTS %d lignes, %d colonnes", vl_Nb_Ligne_Lue, vl_Nb_Col);
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_param_result (pa_cmd , va_num_param_retour, pa_Tab_param_retour);
						XZST_03EcritureTrace(XZSTC_INFO,"asql_Taitement_Result : PARAMETER RESULTS\n");
						
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (pa_cmd, pa_SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"asql_Taitement_Result : CS_STATUS_RESULT = %d", vl_retcode);

						if (*pa_SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_WARNING, "asql_Taitement_Result : SPstatus= %d", *pa_SPstatus);
 						   return (XDC_NOK);
						}
						break;
				}
				
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(pa_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"asql_Taitement_Result: ct_res_info(msgtype) a echoue, code %d", vl_retcode);
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
					XZST_03EcritureTrace(XZSTC_WARNING,"asql_Taitement_Result: ct_results retourne CS_CMD_FAIL.");
					return (XDC_NOK);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"asql_Taitement_Result: ct_results retourne un resultat de type inattendu");
					return (XDC_NOK);
				}
	}


	XZST_03EcritureTrace(XZSTC_SGBD, "OUT : asql_Taitement_Result retourne %d ",XDC_OK);
	return (XDC_OK);
}
