/* *************************************************************
*  ce fichier permet de tester la syntaxe des fichiers RTie
* (regles, classes, objets, initialisations) en lancant le RTie
* en mode developpement (ie avec le GDI) et en effectuant la
* compilation (au sens RTie) de la base de connaissances
*
* Les fonctions C sont seulement declarees (mais pas definies),
* et il n'ya pas d'interface avec les autres sous-systemes
* (BASEDD, GESCONF, GESIHM).
* 
* NB : ce fichier se compile avec la comande "rtlinkie test.c",
* qui cree un "a.out" dans ce repertoire.
* ************************************************************** */

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <rtworks/ie.h>

/* declaration des fonctions C */
static void lin_MAJ_Contexte_Site() { }
static void lin_Autoroutes_Inversees() { }
static void util_pmv_amont_sortie() { }
static void util_pmv_amont_fmc() { }
static void util_float_to_str() { }
static void util_sprintf() { }
static void creer_objet() { }
static void util_quitter_dg_std() { }
static void util_trace_regle() { }
static void trace_raist() { }
static void util_jour() { }
static void util_horodate() { }
static void util_date() { }
static void util_date_num() { }
static void util_heure() { }
static void util_duree() { }
static void util_bool_to_str() { }
static void util_voies() { }
static void pac_file_attente() { }
static void pac_ecrire_prop() { }
static void pac_ecrire_msgTFM() { }
static void pac_pas_de_msgTFM() { }
static void pac_PA_pret() { }
static void arbre_creer() { }
static void pmv_ligne_trop_longue() { }
static void pmv_mise_en_page() { }
static void tous_les_PMV_amont() { }
static void le_PMV_amont() { }
static void le_PMV_amont_aire() { }
static void le_PMV_amont_sortie() { }
static void les_PMV_ds_perturbation() { }
static void app_deja_contactes() { }
static void fax_recap_pmv() { }
static void fax_recap_affichage() { }
static void fax_nom_interlocuteur() { }
static void msg_intervenants_sur_place() { }
static void msg_mise_en_page() { }
static void msg_guidage() { }
static void tfm_rechercher() { }
static void nav_candidate() { }
static void tunnel_candidats() { }
static void tunnel_sens_oppose() { }
static void ech_candidat() { }

static void ini_DeclarerFonctions()
{
	static char *version = "@(#)dpac_fctC.c	1.10 07/19/95 : ini_DeclarerFonctions" ;


	TieFuncCreate(  T_FUNC_NAME,    "lin_Autoroutes_Inversees",
			T_FUNC_PTR,     lin_Autoroutes_Inversees,
			T_FUNC_RETURN,  T_TYPE_VOID,
			0);

	TieFuncCreate(  T_FUNC_NAME,    "lin_MAJ_Contexte_Site",
			T_FUNC_PTR,     lin_MAJ_Contexte_Site,
			T_FUNC_RETURN,  T_TYPE_VOID,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_float_to_str",
			T_FUNC_PTR,	util_float_to_str,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_pmv_amont_sortie",
			T_FUNC_PTR,	util_pmv_amont_sortie,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_pmv_amont_fmc",
			T_FUNC_PTR,	util_pmv_amont_fmc,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_sprintf",
			T_FUNC_PTR,	util_sprintf,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"creer_objet",
			T_FUNC_PTR,	creer_objet,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_quitter_dg_std",
			T_FUNC_PTR,	util_quitter_dg_std,
			T_FUNC_RETURN,	T_TYPE_VOID,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_trace_regle",
			T_FUNC_PTR,	util_trace_regle,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_RETURN,	T_TYPE_VOID,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"trace_raist",
			T_FUNC_PTR,	trace_raist,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_RETURN,	T_TYPE_VOID,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_jour",
			T_FUNC_PTR,	util_jour,
			T_FUNC_RETURN,	T_TYPE_NUMERIC,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_horodate",
			T_FUNC_PTR,	util_horodate,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_date",
			T_FUNC_PTR,	util_date,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_date_num",
			T_FUNC_PTR,	util_date_num,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_heure",
			T_FUNC_PTR,	util_heure,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_duree",
			T_FUNC_PTR,	util_duree,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_bool_to_str",
			T_FUNC_PTR,	util_bool_to_str,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_BOOLEAN,
			T_FUNC_VARARGS,	T_TYPE_STRING,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"util_voies",
			T_FUNC_PTR,	util_voies,
			T_FUNC_RETURN,	T_TYPE_NUMERIC,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pac_file_attente",
			T_FUNC_PTR,	pac_file_attente,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pac_ecrire_prop",
			T_FUNC_PTR,	pac_ecrire_prop,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pac_ecrire_msgTFM",
			T_FUNC_PTR,	pac_ecrire_msgTFM,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_STRING,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pac_pas_de_msgTFM",
			T_FUNC_PTR,	pac_pas_de_msgTFM,
			T_FUNC_RETURN,	T_TYPE_VOID,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pac_PA_pret",
			T_FUNC_PTR,	pac_PA_pret,
			T_FUNC_RETURN,	T_TYPE_VOID,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"arbre_creer",
			T_FUNC_PTR,	arbre_creer,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pmv_ligne_trop_longue",
			T_FUNC_PTR,	pmv_ligne_trop_longue,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"pmv_mise_en_page",
			T_FUNC_PTR,	pmv_mise_en_page,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"tous_les_PMV_amont",
			T_FUNC_PTR,	tous_les_PMV_amont,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"le_PMV_amont",
			T_FUNC_PTR,	le_PMV_amont,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"le_PMV_amont_aire",
			T_FUNC_PTR,	le_PMV_amont_aire,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"le_PMV_amont_sortie",
			T_FUNC_PTR,	le_PMV_amont_sortie,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"les_PMV_ds_perturbation",
			T_FUNC_PTR,	les_PMV_ds_perturbation,
			T_FUNC_RETURN,	T_TYPE_BOOLEAN,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"app_deja_contactes",
			T_FUNC_PTR,	app_deja_contactes,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"fax_recap_pmv",
			T_FUNC_PTR,	fax_recap_pmv,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"fax_recap_affichage",
			T_FUNC_PTR,	fax_recap_affichage,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"fax_nom_interlocuteur",
			T_FUNC_PTR,	fax_nom_interlocuteur,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"msg_intervenants_sur_place",
			T_FUNC_PTR,	msg_intervenants_sur_place,
			T_FUNC_RETURN,	T_TYPE_NUMERIC,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"msg_mise_en_page",
			T_FUNC_PTR,	msg_mise_en_page,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"msg_guidage",
			T_FUNC_PTR,	msg_guidage,
			T_FUNC_RETURN,	T_TYPE_STRING,
			T_FUNC_ARG,	T_TYPE_STRING,
			T_FUNC_VARARGS,	T_TYPE_ANY,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"tfm_rechercher",
			T_FUNC_PTR,	tfm_rechercher,
			T_FUNC_RETURN,	T_TYPE_NUMERIC,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"nav_candidate",
			T_FUNC_PTR,	nav_candidate,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"tunnel_candidats",
			T_FUNC_PTR,	tunnel_candidats,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"tunnel_sens_oppose",
			T_FUNC_PTR,	tunnel_sens_oppose,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

	TieFuncCreate(	T_FUNC_NAME,	"ech_candidat",
			T_FUNC_PTR,	ech_candidat,
			T_FUNC_RETURN,	T_TYPE_VOID,
			T_FUNC_ARG,	T_TYPE_NUMERIC,
			T_FUNC_VARARGS,	T_TYPE_NUMERIC,
			0);

}



/*
* fonction principale :
* declaration des fonctions C et lancement standard du IE
*/

int main(argc, argv)
int argc ;
char **argv ;

{
	static char *version = "@(#)dpac_ie.c	1.13 07/19/95 : main";

	/*A
	**  declaration des fonctions utilisateur
	*/
	ini_DeclarerFonctions();

	/*A
	**  initialisation des arguments passes au Rtie
	*/
	TieInit(argc, argv);

	/*A
	**  lancement du Rtie
	*/
	TieStart();
}
