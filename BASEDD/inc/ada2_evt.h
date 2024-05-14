/* Fichier : ada2_evt.h   Release : 1.0   Date : 30/04/20*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ada2_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client ada2
* Il contient les definitions des fonctions
* utilisees dans ada2_evt.c
*************************************************
* HISTORIQUE :
*
* ABE	30/03/2020 : Creation DEM SAE130 V1.1
* ABE	16/07/2020 : Modif suite recette DEM SAE149 V1.2 
* ABE	16/07/2020 : Modif suite recette DEM SAE149 V1.3
* ABE	23/07/2020 : Suppression des patrouilleur de tada2 vers tapat DEM SAE149 V1.4
* ABE	07/09/2020 : Ajout traitement TDP DEM130 V1.5
* ABE	07/09/2020 : push pre-prod DEM130 V1.6
* ABE	16/09/2020 : Ajout fonction ada2_evt_cb_situation_coupparcoup DEM-SAE130 V1.7
* ABE	05/02/2021 : Ajout publication coup par coup fin pour fmc coupure route DEM-SAE130 V1.8
************************************************/

#ifndef ada2_evt
#define ada2_evt

/* include system */
#include    <string.h>
#include    <ctype.h>
 
#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzaec.h"
#include "xzasc.h"
#include "xzlg.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>


/*liste des fmc a exporter (dynamique)*/
typedef struct {
	XDY_Entier  Numero;
	XDY_Octet   Cle;
	XDY_Entier	Type;
} T_FMC;

typedef struct S_Liste_FMC {
	T_FMC	FMC;
	struct S_Liste_FMC	*Suivant;
} T_Liste_FMC;


/*conversion type de fmc sae -> code attribut strada (statique)*/
typedef struct {
	int	Type;
	char	Code_Strada[5];
	char	XMLFile[100];
	char	Type_Datex[50];
} T_Type_FMC;
		
typedef struct {
	XDY_Entier numero_regroupement;
	int der_numero_element_attache_regroupement;
} T_NUMERO_ELEMENT_REGROUPEMENT;


typedef struct {
	char *     (*Fct)();
} T_PREDICAT;

typedef struct S_NOEUD {
	char 	Libelle[100];
	char	BaliseXML[100];
	int	Type;
	char	Valeur[255];
	int	SousAttribut;
	T_PREDICAT Predicat;
	T_PREDICAT Predicat_Test;
	int	TypeValeur;
	XDY_Booleen	Generique;
	struct S_NOEUD	*Attribut;
	struct S_NOEUD	*FilsAine;
	struct S_NOEUD	*FrereCadet;
	struct S_NOEUD	*Pere;
} T_NOEUD;

typedef struct S_LISTE {
	char	LibellePere[100];
	T_NOEUD	*PositionArbre;
	struct S_LISTE *Suivant;
} T_LISTE;

/* definition des constantes */
	
/* dÅfinition des variables globales È l'application */

/* definition des variables locales au module */

/* declaration de fonctions internes */
void	ada2_evt_purge();
void ada2_xml_noeud(T_NOEUD *pa_noeud, xmlTextWriterPtr pa_flux);
void 	ada2_xml_TDP_write(XDY_Eqt va_station, int va_type,char *va_prefixe);
void 	ada2_xml_EVT_write(XZLGT_Liste_FMC, int, char * );		
void	ada2_copie_fichier_vers_ftp_rep(int data_Type);
int ada2_get_compteur_nom_fichier_et_incr(const char *va_destinataire, int incr);
int ada2_get_departement(float va_PR, const char * va_axe);
int ada2_set_feedtype_value(const char *va_DestNom,const char *va_PathFichierDest,const char *va_destinataire);	
void	ada2_keep_alive();
extern int ada2_xml_init();
int get_local_UTC_hour_diff(XDY_Horodate);
int ada2_remove_FMC_privee (const char *va_DossierFichier,const char *va_NomFichier);
void ada2_xml_traite_route_coupee(XZLGT_Liste_FMC pa_fmc, XDY_Booleen va_coupure_route_version_precedente,XDY_Booleen va_coupure_route_inverse_version_precedente);
int find_indice_numero_element_attache_regroupement(T_NUMERO_ELEMENT_REGROUPEMENT *va_struct,XDY_Entier va_numero_regroupement,int va_size);

/* definition de fonctions externes */		
void	ada2_evt_cb_situation_recap();
void 	ada2_nouvel_evt_cb(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
void    ada2_evt_cb_situation_coupparcoup(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
void	ada2_get_radt();
void	ada2_write_radt(XDY_Horodate va_horodate,
			XDY_Eqt	va_station,
			XDY_Octet va_sens,
			XDY_Mot va_Q,		
			XDY_Octet va_V,
			XDY_Octet va_TO,
			XDY_Octet va_PL,
			XDY_Octet va_validite);
void	ada2_trim_comment(char * vl_comment_out, char * vl_comment_in);

#endif
