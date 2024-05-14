/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : atdp_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache passerelle generique
* 
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* Guilhou	25/02/16: Creation 	     V1.0 DEM 1151
* JMG		09/03/16 : import fichiers ASF	1.2
* JMG		29/05/17 : correction regionalisation 1.3
* PNI   06/06/18 : correctif pour filtre donn� ASF comprises entre 200 et 300 v1.4 DEM1297
* JMG	09/09/18 : ajout TDP Mareille LOT25 DEM1304 1.5
* JMG	17/10/18 : comparaison nom zone sur 25 caracteres max LOT125 DEM1304 1.6
* LCL	27/09/23	: Adaptations CNA DEM-483
* JPL	23/11/23 : Mise en base des TDP Marseille en table TDP_FCD (SAE-504)  1.7
************************************************/


#include "xzlg.h"
#include "xzao000.h"
#include "xzao960.h"
#include "xzia.h"
#include "atdp_xml.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>

#define CO_NB_JOUR_PURGE        3
#define CO_DELAI_FREQUENCE      360
#define CO_DELAI_PURGE          86400 * CO_NB_JOUR_PURGE
#define CO_NB_CHIFFRE           13

/* definition des constantes */
#define MY_ENCODING		"ISO-8859-1"

#define	CO_MAX_CARACTERE	80
#define CO_FIC_FREQUENCE        XDC_PATH_STRADA "/frequence.txt"

#define TAILLE 200
#define C_FREQ_PURGE		86400 * 3
#define C_TAB_SEPAR		"\t"
#define	C_FREQ_VERIF_EVT	60
#define C_CODE_ESCOTA		"ESCOTA"
#define C_DATEX_ESCOTA		"frESCOTA"

#define C_PATH_XML              XDC_PATH_STRADA "/xml"
#define C_PATH                  XDC_PATH_STRADA "/datexTDP"

#define C_XML_TDP_PERE_DATA  XDC_PATH_STRADA "/xml/balisesTDP_PERE_DATA.txt"
#define C_XML_TDP_PERE_SECTION  XDC_PATH_STRADA "/xml/balisesTDP_PERE_SECTION.txt"
#define C_XML_TDP_GLOBAL_DATA        XDC_PATH_STRADA "/xml/balisesTDP_GLOBAL_DATA.txt"
#define C_XML_TDP_GLOBAL_SECTION        XDC_PATH_STRADA "/xml/balisesTDP_GLOBAL_SECTION.txt"
#define C_BALISE_PREFIXE        "zdp."
#define C_BALISE_DATE_COURANTE  "zdp.date_courante"
#define C_BALISE_DATE_DATA      "zdp.date_data"
#define C_BALISE_ID             "zdp.id"
#define C_BALISE_LIBELLE        "zdp.libelle"
#define C_BALISE_L04            "zdp.l04"
#define C_BALISE_L05            "zdp.l05"
#define C_BALISE_L06            "zdp.l06"
#define C_BALISE_L09            "zdp.l09"
#define C_BALISE_L10            "zdp.l10"
#define C_BALISE_L11            "zdp.l11"
#define C_BALISE_L12            "zdp.l12"
#define C_BALISE_AUTOROUTE      "zdp.autoroute"
#define C_BALISE_PR_M_DEBUT     "zdp.pr_m_debut"
#define C_BALISE_PR_M_FIN       "zdp.pr_m_fin"
#define C_BALISE_PR_KM_FIN      "zdp.pr_km_fin"
#define C_BALISE_PR_KM_DEBUT    "zdp.pr_km_debut"
#define C_BALISE_TDP    	"zdp.tdp"
#define C_BALISE_TDP_LIBRE    "zdp.tdp_libre"


#define CO_FIC_TDPASF_STATIQUE  XDC_PATH_STRADA "/escota_TT_Data.xml"
#define CO_FIC_TDPASF_DYN       XDC_PATH_STRADA "/escota_TT_Sections.xml"
#define CO_FIC_TDP_MARSEILLE	"/produits/migrazur/appliSD/fichiers/stra/tdpin/TPPolyvox.xml"
#define CO_FIC_TDP_MARSEILLE_CONV	"/produits/migrazur/appliSD/fichiers/stra/tdpin/TPPolyvox_conv.xml"

#define CO_FIC_ASF_DYN	"/produits/migrazur/appliSD/fichiers/stra/tdpin/ASF_TT_Data.xml"
#define CO_FIC_ASF_STATIQUE		"/produits/migrazur/appliSD/fichiers/stra/tdpin/ASF_TT_Sections.xml"
#define C_SCRIPT_FTP_GET	"/produits/migrazur/appliSD/fichiers/stra/ftp/asftdp_script_get"


#define C_MODULE	0
#define C_ATTRIBUT_GEN	1
#define C_ATTRIBUT_SPEC	2
#define C_TDP_SANS_OBJET	-100


static char * balise_test_true();


void atdp_xml_init2(char *, int );

void gen_cumul_voies(XDY_Voie, int *, int *);

void atdp_trouve_zone(char *, XDY_Entier *, char *, XDY_Autoroute *, XDY_Sens *, XDY_PR *, XDY_PR *,
			XDY_Entier *, XDY_Entier, XDY_Octet);

XDY_Evt gen_recherche_pere(XZLGT_Liste_FMC *, int);
XDY_Booleen gen_a_traiter(XZLGT_Liste_FMC *, int);




void atdp_xml_decode_statique();
void atdp_xml_decode_dyn();
void atdp_xml_decode_dyn_marseille();

#define NBR_PREMIER 	53
#define NBR_ELT_TABLE	59

#define C_l01		"F32"
#define C_l02		"LOC"
#define C_l03		4
#define C_l07_P		"Aix en Provence - Frontiere Italienne"
#define C_l07_N		"Frontiere italienne - Aix en Provence"
#define C_l07_U		"Sens non defini"

#define TAILLE_SUR	350

static char *   balise_date_courante();
static char *   balise_date_data();
static char *   balise_id();
static char *   balise_libelle();
static char *   balise_l05();
static char *   balise_l04();
static char *   balise_l06();
static char *   balise_l11();
static char *   balise_l09();
static char *   balise_l10();
static char *   balise_l12();
static char *   balise_autoroute();
static char *   balise_pr_m_debut();
static char *   balise_pr_m_fin();
static char *   balise_pr_km_debut();
static char *   balise_pr_km_fin();
static char * balise_test_true();
static char * balise_tdp();
static char * balise_tdp_libre();

XZLGT_Liste_Zdp		*vm_zdp;
XZLGT_Liste_Zdp vm_noeud;

xmlChar *ConvertInput(const char *in, const char *encoding);


static char			*tm_l07_p[]={
	"",
	"Aix en Provence - Frontiere Italienne",
	"A8 - Monaco",
	"Marseille - Toulon",
	"Marseille - A52",
	"Aix - Les Alpes",
	"Aix - Aubagne",
	"A52 - Auriol",
	"Le Luc - Toulon"};

static char			*tm_l07_n[]={
	"",
	"Frontiere Italienne - Aix en Provence",
	"MonacoA8 - Monaco",
	"Toulon - Marseille",
	"A52 - Marseille",
	"Les Alpes - Aix",
	"Aubagne - Aix",
	"Auriol - A52",
	"Toulon - Le Luc"};

static  T_Mesure	tm_mesure;
static  T_Mesure	tm_mesure_marseille;

static char pm_comment[TAILLE_SUR+1]="";
 
static char	vm_id[50];
static char	vm_libelle[200];
static XDY_Octet	vm_direction;
static char	vm_autoroute[10];
static int	vm_numero_autoroute;
static int	vm_pr_debut_metres;
static int	vm_pr_debut_km;
static int	vm_pr_debut;
static int 	vm_pr_fin;
static int 	vm_pr_fin_metres;
static int	vm_pr_fin_km;
static int	vm_tdp;
static int	vm_tdp_libre;
static XZLGT_Nombre_Mem *vm_nombre_mem;

static int vm_frequence;
static char chemin_export[80];
static int vm_type;
static XDY_Datagroup vm_dg_tdp;
static XDY_Horodate vm_horo_tdp;
static int vm_nb_zdp_asf;
static int vm_nb_zdp_marseille;

#define C_PIPE	"\t"
#define C_VALEUR_PREDICAT 	1
#define C_VALEUR_DIRECTE	0

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

static T_NOEUD *vg_arbre[100];
static T_LISTE	*vg_peres=NULL;


int vm_indice;

/* declaration de fonctions internes */
void atdp_xml_write2(XZLGT_Liste_Zdp, int, char * );
/* definition de fonctions externes */

void atdp_xml_init(int va_nb) {
	int vl_i;
        char vl_cmd[300];

        strcpy( chemin_export, C_PATH);

	XDG_EncodeDG2( vm_dg_tdp, XDG_ECMD_TDP, "SPDP");

	vm_nb_zdp_asf = 0;
	vm_nb_zdp_marseille = 0;

	vm_nombre_mem = XZLG993_Get_Nombre();
        vg_arbre[0]=NULL;
        vg_arbre[1]=NULL;
          sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
                C_XML_TDP_PERE_SECTION, 
                "",
                C_PATH_XML);

          XZST_03EcritureTrace(   XZSTC_WARNING, "vl_cmd %s",vl_cmd);
          system(vl_cmd);
          sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
          atdp_xml_init2(vl_cmd, 1);

        /*init du header global*/
        vg_arbre[2]=NULL;
        sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
                C_XML_TDP_GLOBAL_SECTION,
                "",
                C_PATH_XML);
        system(vl_cmd);
          XZST_03EcritureTrace(   XZSTC_WARNING, "vl_cmd %s",vl_cmd);
        sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
        atdp_xml_init2(vl_cmd,2 );

        vg_arbre[3]=NULL;
          sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
                C_XML_TDP_PERE_DATA, 
                "",
                C_PATH_XML);

          XZST_03EcritureTrace(   XZSTC_WARNING, "vl_cmd %s",vl_cmd);
          system(vl_cmd);
          sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
          atdp_xml_init2(vl_cmd, 3);

        /*init du header global*/
        vg_arbre[4]=NULL;
        sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
                C_XML_TDP_GLOBAL_DATA,
                "",
                C_PATH_XML);
        system(vl_cmd);
          XZST_03EcritureTrace(   XZSTC_WARNING, "vl_cmd %s",vl_cmd);
        sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
        atdp_xml_init2(vl_cmd,4 );

    XZST_03EcritureTrace(   XZSTC_WARNING, "Sortie de atdp_xml_init");

}
void atdp_xml_init2(char *va_fichier, int va_indice)
{
 FILE *pl_fp;
 int vl_retcode=0;
 int vl_i;
 char        vl_Lig[255];
 XDY_Entier vl_Resultat_out = 0;
 char  *vl_str;
 char *vl_ch;
 T_NOEUD *vl_noeud;
 T_NOEUD *vl_noeud2;
 T_NOEUD *vl_noeud3;
 T_NOEUD *vl_noeud4;
 T_NOEUD *vl_noeud5;
 char	vl_pere[100];
 T_LISTE	*vl_liste;
 T_LISTE	*vl_liste2;
 T_LISTE	*vl_liste3;

 XZST_03EcritureTrace(   XZSTC_FONCTION,"atdp_xml_init2 %s - %d",va_fichier,va_indice);
 vl_noeud = (T_NOEUD *)malloc(sizeof(T_NOEUD));
  
 if ( (pl_fp = fopen(va_fichier, "r")) == NULL ) {
	 return;
 }

 while ( ( vl_retcode = XZAO000_LireLigneFich ( pl_fp, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
 {
    if (vl_Resultat_out != XZAO_EOF )
    {
    vl_str=vl_Lig;
    vl_str[strlen(vl_str)-1]='\0';
    vl_ch=strtok(vl_str,C_PIPE);

    /*elt 1 = libelle*/
    strcpy(vl_noeud->Libelle, vl_ch);

    /*elt 1 prime = baliseXML*/
    vl_ch=strtok(NULL,C_PIPE);
    strcpy(vl_noeud->BaliseXML, vl_ch);

    vl_ch=strtok(NULL,C_PIPE);
    /* elt2 = noeud pere*/
    strcpy(vl_pere,vl_ch);

    vl_ch=strtok(NULL,C_PIPE);
    /*elt 3 = type de noeud*/
    if (!strcmp(vl_ch,"M"))
      vl_noeud->Type=C_MODULE;
    else if (!strcmp(vl_ch,"AG"))
      vl_noeud->Type=C_ATTRIBUT_GEN;
    else if (!strcmp(vl_ch,"AS"))
      vl_noeud->Type=C_ATTRIBUT_SPEC;
    else
      vl_noeud->Type=C_ATTRIBUT_GEN;

    vl_ch=strtok(NULL,C_PIPE);
    /*elt 4 = valeur*/
    strcpy(vl_noeud->Valeur, vl_ch);
    vl_noeud->TypeValeur=C_VALEUR_DIRECTE;

    /*si la valeur commence par fmc." */
    /*alors c est une fonction Predicat */

    if (!strncmp(vl_noeud->Valeur,C_BALISE_PREFIXE,strlen(C_BALISE_PREFIXE)))
    {
      vl_noeud->TypeValeur=C_VALEUR_PREDICAT;
      if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_COURANTE)) vl_noeud->Predicat.Fct=balise_date_courante;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_DATA)) vl_noeud->Predicat.Fct=balise_date_data;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_ID)) vl_noeud->Predicat.Fct=balise_id;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_LIBELLE)) vl_noeud->Predicat.Fct=balise_libelle;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L05)) vl_noeud->Predicat.Fct=balise_l05;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L04)) vl_noeud->Predicat.Fct=balise_l04;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L06)) vl_noeud->Predicat.Fct=balise_l06;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L09)) vl_noeud->Predicat.Fct=balise_l09;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L10)) vl_noeud->Predicat.Fct=balise_l10;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L11)) vl_noeud->Predicat.Fct=balise_l11;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_L12)) vl_noeud->Predicat.Fct=balise_l12;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_AUTOROUTE)) vl_noeud->Predicat.Fct=balise_autoroute;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_PR_M_DEBUT)) vl_noeud->Predicat.Fct=balise_pr_m_debut;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_PR_M_FIN)) vl_noeud->Predicat.Fct=balise_pr_m_fin;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_PR_KM_FIN)) vl_noeud->Predicat.Fct=balise_pr_km_fin;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_PR_KM_DEBUT)) vl_noeud->Predicat.Fct=balise_pr_km_debut;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP)) vl_noeud->Predicat.Fct=balise_tdp;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_LIBRE)) vl_noeud->Predicat.Fct=balise_tdp_libre;
      else {
          XZST_03EcritureTrace(XZSTC_WARNING,"atdp_xml_init2: BALISE NON TROUVEE !!! Noeud.Valeur = %s",vl_noeud->Valeur);
	}
    }


    vl_ch=strtok(NULL,C_PIPE);
    /*elt 5 = sous attribut ou pas*/
    if (!strcmp(vl_ch,"="))
      vl_noeud->SousAttribut=1;
    else if (!strcmp(vl_ch,"opt"))
      vl_noeud->SousAttribut=-1;
    else
      vl_noeud->SousAttribut=0;

    vl_ch=strtok(NULL,C_PIPE);
    /*elt 6 : valeur a tester ou pas*/
    if ((vl_ch!=NULL) && (!strncmp(vl_ch,C_BALISE_PREFIXE,strlen(C_BALISE_PREFIXE)))) {
      vl_noeud->Predicat_Test.Fct = balise_test_true;
    }
    else  {
      vl_noeud->Predicat_Test.Fct = balise_test_true;
    }

    vl_noeud->Attribut=NULL;
    vl_noeud->FilsAine = NULL;
    vl_noeud->FrereCadet = NULL;
    vl_noeud->Pere=NULL;

    if (vg_arbre[va_indice]==NULL) {/*1� insertion*/
	vg_arbre[va_indice]=(T_NOEUD *)malloc(sizeof(T_NOEUD));
	vg_arbre[va_indice]->Attribut=NULL;
	vg_arbre[va_indice]->FilsAine = NULL;
	vg_arbre[va_indice]->FrereCadet = NULL;
	vg_arbre[va_indice]->Pere=NULL;
	vg_arbre[va_indice]->Type=vl_noeud->Type;
	vg_arbre[va_indice]->TypeValeur=vl_noeud->TypeValeur;
	vg_arbre[va_indice]->Generique=vl_noeud->Generique;
	vg_arbre[va_indice]->Predicat=vl_noeud->Predicat;
	vg_arbre[va_indice]->Predicat_Test=vl_noeud->Predicat_Test;
	vg_arbre[va_indice]->SousAttribut=vl_noeud->SousAttribut;
	strcpy(vg_arbre[va_indice]->Valeur,vl_noeud->Valeur);
	strcpy(vg_arbre[va_indice]->Libelle,vl_noeud->Libelle);
	strcpy(vg_arbre[va_indice]->BaliseXML,vl_noeud->BaliseXML);
	vl_noeud2 = vg_arbre[va_indice];

	/*c est forcement un noeud principal = un PERE*/
        vg_peres = (T_LISTE *)malloc(sizeof(T_LISTE));
 	strcpy(vg_peres->LibellePere, vg_arbre[va_indice]->Libelle);
	vg_peres->PositionArbre = vg_arbre[va_indice];
	vg_peres->Suivant = NULL;
    }
    else {
      if ( (vl_noeud->Type==C_MODULE) || (vl_noeud->Type==C_ATTRIBUT_SPEC)) { /*c'est un noeud principal*/
        /*je l insere dans la liste des peres*/
	vl_liste=vg_peres;
	while (vl_liste->Suivant!=NULL)  {
		vl_liste=vl_liste->Suivant;
	}
	vl_liste->Suivant = (T_LISTE *)malloc(sizeof(T_LISTE));
	vl_liste=vl_liste->Suivant;
        strcpy(vl_liste->LibellePere,vl_noeud->Libelle);
	vl_liste->PositionArbre=NULL;
	vl_liste->Suivant=NULL;

	/*je cherche son pere dans l arbre*/
	vl_liste2=vg_peres;
	while (vl_liste2!=NULL) {
	  if (!strcmp(vl_liste2->LibellePere,vl_pere)) {
	    vl_noeud4 = vl_liste2->PositionArbre;
	    break;
	  }
          vl_liste2=vl_liste2->Suivant;	
	}
	if (vl_liste2==NULL) {
          /*printf("pere %s de %s non trouve\n",vl_pere,vl_noeud->Libelle);*/
	  return;
        }
        /*printf("pere %s trouve !!! \n",vl_liste2->PositionArbre->Libelle);*/

	/*je le mets au bon endroit dans l arbre*/
	/*je me place sur le pere*/
	if (vl_noeud4->FilsAine == NULL) { /*premier fils*/
	  /* printf("premier fils\n");*/
	  vl_noeud4->FilsAine = (T_NOEUD *)malloc(sizeof(T_NOEUD));
	  strcpy(vl_noeud4->FilsAine->Libelle,vl_noeud->Libelle);
	  strcpy(vl_noeud4->FilsAine->BaliseXML,vl_noeud->BaliseXML);
	  vl_noeud4->FilsAine->Type=vl_noeud->Type;
	  vl_noeud4->FilsAine->Predicat=vl_noeud->Predicat;
	  vl_noeud4->FilsAine->Predicat_Test=vl_noeud->Predicat_Test;
	  vl_noeud4->FilsAine->TypeValeur=vl_noeud->TypeValeur;
	  vl_noeud4->FilsAine->Generique=vl_noeud->Generique;
	  vl_noeud4->FilsAine->SousAttribut=vl_noeud->SousAttribut;
	  vl_noeud4->FilsAine->Attribut=NULL;
	  strcpy(vl_noeud4->FilsAine->Valeur,vl_noeud->Valeur);
	  vl_noeud4->FilsAine->Pere = vl_noeud4;
	  vl_noeud4->FilsAine->FrereCadet = NULL;
	  vl_noeud4->FilsAine->FilsAine=NULL;
	  vl_liste->PositionArbre = vl_noeud4->FilsAine;
	}
	else { /*N� fils : c est donc un frere du fils aine*/
	  vl_noeud5 = vl_noeud4->FilsAine;
	  while (vl_noeud5->FrereCadet != NULL) {
	/*	  printf("frere= %s\n",vl_noeud5->FrereCadet->Libelle);*/
		  vl_noeud5=vl_noeud5->FrereCadet;
	  }
          vl_noeud5->FrereCadet = (T_NOEUD *)malloc(sizeof(T_NOEUD));
	  strcpy(vl_noeud5->FrereCadet->Libelle,vl_noeud->Libelle);
	  strcpy(vl_noeud5->FrereCadet->BaliseXML,vl_noeud->BaliseXML);
	  vl_noeud5->FrereCadet->Type=vl_noeud->Type;
	  vl_noeud5->FrereCadet->Predicat_Test=vl_noeud->Predicat_Test;
	  vl_noeud5->FrereCadet->Predicat=vl_noeud->Predicat;
	  vl_noeud5->FrereCadet->TypeValeur=vl_noeud->TypeValeur;
	  vl_noeud5->FrereCadet->Generique=vl_noeud->Generique;
	  vl_noeud5->FrereCadet->SousAttribut=vl_noeud->SousAttribut;
	  vl_noeud5->FrereCadet->Attribut=NULL;
	  vl_noeud5->FrereCadet->FilsAine=NULL;
	  vl_noeud5->FrereCadet->FrereCadet=NULL;
	  strcpy(vl_noeud5->FrereCadet->Valeur,vl_noeud->Valeur);
	  vl_noeud5->FrereCadet->Pere = vl_noeud4;
	  vl_liste->PositionArbre = vl_noeud5->FrereCadet;
	}
      }
      else { /*c'est un attribut*/
	/*je cherche son pere dans l arbre*/
	vl_liste2=vg_peres;
	while (vl_liste2!=NULL) {
	  if (!strcmp(vl_liste2->LibellePere,vl_pere)) {
	   vl_noeud4 = vl_liste2->PositionArbre;
	   break;
	  }
	  vl_liste2=vl_liste2->Suivant;
        }
	if (vl_liste2==NULL) {
	  /*printf("pere %s de %s non trouve\n",vl_pere,vl_noeud->Libelle);*/
	  return;
	}
        
	/*printf("pere %s de %s trouve\n",vl_pere,vl_noeud->Libelle);*/

	/*si 1� insertion*/
	if (vl_noeud4->Attribut==NULL) {
	  /*printf("1� insertion\n");*/
	  vl_noeud->Attribut=NULL;
          vl_noeud4->Attribut = (T_NOEUD *)malloc(sizeof(T_NOEUD));
	  vl_noeud4 = vl_noeud4->Attribut;
	  strcpy(vl_noeud4->Libelle,vl_noeud->Libelle);
	  strcpy(vl_noeud4->BaliseXML,vl_noeud->BaliseXML);
	  vl_noeud4->Type=vl_noeud->Type;
	  vl_noeud4->Predicat=vl_noeud->Predicat;
	  vl_noeud4->Predicat_Test=vl_noeud->Predicat_Test;
	  vl_noeud4->TypeValeur=vl_noeud->TypeValeur;
	  vl_noeud4->Generique=vl_noeud->Generique;
	  vl_noeud4->SousAttribut=vl_noeud->SousAttribut;
	  strcpy(vl_noeud4->Valeur,vl_noeud->Valeur);
	  vl_noeud4->Pere = vl_liste2->PositionArbre;
	  vl_noeud4->Attribut = NULL;
	}
	else {
	  /*je l insere en fin de liste des attributs sur mon noeud principal courant*/
	  /*printf("N� insertion\n");*/
	  vl_noeud3 = vl_noeud4;
	  while (vl_noeud3->Attribut != NULL)
	    vl_noeud3 = vl_noeud3->Attribut;
	  vl_noeud->Attribut=NULL;
          vl_noeud3->Attribut = (T_NOEUD *)malloc(sizeof(T_NOEUD));
	  strcpy(vl_noeud3->Attribut->Libelle,vl_noeud->Libelle);
	  strcpy(vl_noeud3->Attribut->BaliseXML,vl_noeud->BaliseXML);
	  vl_noeud3->Attribut->Type=vl_noeud->Type;
	  vl_noeud3->Attribut->Predicat=vl_noeud->Predicat;
	  vl_noeud3->Attribut->Predicat_Test=vl_noeud->Predicat_Test;
	  vl_noeud3->Attribut->TypeValeur=vl_noeud->TypeValeur;
	  vl_noeud3->Attribut->Generique=vl_noeud->Generique;
	  vl_noeud3->Attribut->SousAttribut=vl_noeud->SousAttribut;
	  strcpy(vl_noeud3->Attribut->Valeur,vl_noeud->Valeur);
	  vl_noeud3->Attribut->Pere = vl_noeud4;
	  vl_noeud3->Attribut->Attribut = NULL;
	
	}
      }
    }
 }
 else
	break;
 }
 fclose(pl_fp);

 XZST_03EcritureTrace(   XZSTC_FONCTION,"atdp_xml_init2 sortie");
  
}


char *atdp_xml_noeud_valeur(T_NOEUD *pa_noeud)
{
  static char vl_str[255]="";
  char *(*vl_fct)();

  if (pa_noeud->TypeValeur==C_VALEUR_DIRECTE)
    return (pa_noeud->Valeur);
  else {
    vl_fct=pa_noeud->Predicat.Fct;
    strcpy(vl_str,(*vl_fct)());
    return (vl_str);
  }
}

char *atdp_xml_noeud_test(T_NOEUD *pa_noeud)
{
  static char vl_str[255]="";
  char *(*vl_fct)();

    vl_fct=pa_noeud->Predicat_Test.Fct;
    strcpy(vl_str,(*vl_fct)());
    return (vl_str);
}

void atdp_xml_noeud(T_NOEUD *pa_noeud, xmlTextWriterPtr pa_flux)
{
int rc;
T_NOEUD *pl_noeud;
char vl_str[255];
char vl_strtest[255];
char vl_ch[255];

	if (strlen(pa_noeud->Libelle)==0) return;
	strcpy(vl_strtest,atdp_xml_noeud_test(pa_noeud));
     
      XZST_03EcritureTrace (XZSTC_DEBUG1,"atdp_xml_noeud : noeud %s",pa_noeud->Libelle);
      /* noeud principal*/
      if(strlen(vl_strtest) > 0) {
      if (pa_noeud->Type==C_MODULE) {
      	rc = xmlTextWriterStartElement(pa_flux, BAD_CAST pa_noeud->BaliseXML);
      	if (rc < 0) {
	    XZST_03EcritureTrace (XZSTC_DEBUG1, "testXmlwriterFilename: Error at xmlTextWriterStartElement : %s\n",
				pa_noeud->Libelle);
    	    return;
      	}
      }
      else if  (pa_noeud->Type==C_ATTRIBUT_SPEC) { /*attribut specifique*/
	strcpy(vl_str,atdp_xml_noeud_valeur(pa_noeud));
#ifdef _HPUX_SOURCE
	sprintf(vl_ch,"\<%s\>%s\</%s\>",pa_noeud->BaliseXML,vl_str,pa_noeud->BaliseXML);
#else
	if (!strcmp(pa_noeud->BaliseXML,"value lang=\"fr\""))
	  sprintf(vl_ch,"<%s>%s</%s>",pa_noeud->BaliseXML,vl_str,"value");
        else
	  sprintf(vl_ch,"<%s>%s</%s>",pa_noeud->BaliseXML,vl_str,pa_noeud->BaliseXML);
#endif
	    XZST_03EcritureTrace (XZSTC_DEBUG1,  "testXmlwriterFilename: attribut specifique %s = %s",pa_noeud->BaliseXML,vl_ch);
		xmlTextWriterWriteRaw(pa_flux, (xmlChar*)vl_ch);
	rc = xmlTextWriterWriteAttribute(pa_flux, BAD_CAST (char *)pa_noeud->BaliseXML,
							BAD_CAST vl_str);
      }
    /*ecrire les attributs*/
	pl_noeud=pa_noeud->Attribut;
        while(pl_noeud != NULL) {
		strcpy(vl_strtest,atdp_xml_noeud_test(pl_noeud));
	      if (strlen(vl_strtest) > 0) {
	      if (pl_noeud->SousAttribut<=0)  {
	        strncpy(vl_str,atdp_xml_noeud_valeur(pl_noeud),128);
		vl_str[127]='\0';
	        XZST_03EcritureTrace(   XZSTC_DEBUG1, "atdp_xml_noeud:  texte = %s",vl_str);
	        if (pl_noeud->SousAttribut==0) {
		   xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	           rc = xmlTextWriterWriteFormatElement(pa_flux, BAD_CAST pl_noeud->BaliseXML,
				"%s", vl_str);
  		  XZST_03EcritureTrace(   XZSTC_DEBUG1,"ICI %s",vl_str);
		}
	        else {
	          XZST_03EcritureTrace(   XZSTC_DEBUG1, "atdp_xml_noeud:  filtre sur la longueur du texte(%s)",vl_str);
		  if (strlen(vl_str)>0) {
		    xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
		    rc = xmlTextWriterWriteFormatElement(pa_flux, BAD_CAST pl_noeud->BaliseXML,
						  "%s", vl_str);
	          }
  		  XZST_03EcritureTrace(   XZSTC_DEBUG1,"ICI");
		}
	      }
	      else {
		XZST_03EcritureTrace(XZSTC_DEBUG1,"att = %s\n",pl_noeud->BaliseXML);

	      strcpy(vl_str,atdp_xml_noeud_valeur(pl_noeud));
		rc = xmlTextWriterWriteAttribute(pa_flux, BAD_CAST (char *)pl_noeud->BaliseXML, 
				BAD_CAST vl_str);
		}
		}
	    	pl_noeud = pl_noeud->Attribut ;
	}

  }

  /*si fils aine et les freres*/
  if (pa_noeud->FilsAine != NULL) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"noeud fils de %s = %s\n",pa_noeud->Libelle, pa_noeud->FilsAine->Libelle);
	pl_noeud = pa_noeud->FilsAine;
		xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	  atdp_xml_noeud(pl_noeud,pa_flux);
  }
      if(strlen(vl_strtest) > 0) {
    if ((pa_noeud->Type==C_MODULE) ) {
    xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
      xmlTextWriterEndElement(pa_flux);
   }
   }

    /*si freres*/
    if (pa_noeud->FrereCadet != NULL) {
      pl_noeud = pa_noeud->FrereCadet;
		XZST_03EcritureTrace(XZSTC_DEBUG1,"noeud frere de %s = %s\n",pa_noeud->Libelle,pl_noeud->Libelle);
		xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	atdp_xml_noeud(pl_noeud,pa_flux);
    }

      XZST_03EcritureTrace(XZSTC_DEBUG1,"fin noeud = %s\n",pa_noeud->Libelle);
}






void atdp_xml_write2(XZLGT_Liste_Zdp va_noeud, int va_type,char *va_prefixe)
{
    T_NOEUD *vl_noeud;
    T_NOEUD *vl_noeud2;
    int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;
    char vl_nom[255];

    /*memorise la FMC courante*/
    vm_noeud = va_noeud;

    /* Create a new XmlWriter for uri, with no compression. */
    if (!strcmp(va_prefixe,"glob")) 
      sprintf(vl_nom,"%s/%s.xml",
		chemin_export,
		va_prefixe);
    else
      sprintf(vl_nom,"%s/%s%d.xml",
		chemin_export,
		va_prefixe,
		va_noeud.Zdp.Numero);
    XZST_03EcritureTrace(XZSTC_DEBUG1,"atdp_xml_write2 LCL %s(%d)",vl_nom,va_type);

    writer = xmlNewTextWriterFilename(vl_nom, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
        return;
    }

    /*partie generique*/
    /*vl_noeud=vg_arbre[0];
    atdp_xml_noeud(vl_noeud,writer);*/

    /*on se positionne sur le bon modele XML*/
    vl_noeud=vg_arbre[va_type];
    /*TRAITEMENT PARTICULIER POUR CHAUSSEE GLISSANTE*/
    atdp_xml_noeud(vl_noeud,writer);

    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
    printf ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
	return;
    }
	xmlFreeTextWriter(writer);

}











/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}



static char *balise_date_courante() {
static char vl_str[50]="";
int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
XZSMT_Horodate        vl_horo_chaine;
XDY_Horodate            vl_date;
int                     vl_Jour_De_La_Semaine;

XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_date);
XZSM_11ConversionHorodate(vl_date,&vl_horo_chaine);
sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d:%2d",
                        &vl_annee,
                        &vl_mois,
                        &vl_jour,
                        &vl_heure,
                        &vl_minute,&vl_seconde);
  sprintf(vl_str,"%04d-%02d-%02dT%02d:%02d:%02d",
                vl_jour,
                vl_mois,
                vl_annee,
                vl_heure,
                vl_minute,vl_seconde);
  return (vl_str);
}
static char *balise_date_data() {
static char vl_str[50]="";
int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
XZSMT_Horodate        vl_horo_chaine;
XDY_Horodate            vl_date;
int                     vl_Jour_De_La_Semaine;

XZSM_11ConversionHorodate(vm_noeud.Tdp.Horodate,&vl_horo_chaine);
sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                &vl_annee,
                &vl_mois,
                &vl_jour,
                &vl_heure,
                &vl_minute);
  sprintf(vl_str,"%04d-%02d-%02dT%02d:%02d:00",
                vl_jour,
                vl_mois,
                vl_annee,
                vl_heure,
                vl_minute);
  return (vl_str);
}
static char *balise_libelle() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.Zdp.Libelle);
  return (vl_str);
}
static char *balise_id() {
static char vl_str[50]="";
  sprintf(vl_str,"section_%d",vm_noeud.Zdp.Code_Zone);
  return (vl_str);
}
static char *balise_l05() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l05);
  return (vl_str);
}

static char *balise_l04() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l04);
  XZST_03EcritureTrace(   XZSTC_WARNING, "L04 %s",vm_noeud.RDS.l04);
  return (vl_str);
}
static char *balise_l06() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l06);
  return (vl_str);
}
static char *balise_l11() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l11);
  XZST_03EcritureTrace(   XZSTC_WARNING, "L11 %s",vm_noeud.RDS.l11);
  return (vl_str);
}
static char *balise_l09() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l09);
  XZST_03EcritureTrace(   XZSTC_WARNING, "L09 %s",vm_noeud.RDS.l09);
  return (vl_str);
}
static char *balise_l10() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l10);
  return (vl_str);
}
static char *balise_l12() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.RDS.l12);
  XZST_03EcritureTrace(   XZSTC_WARNING, "L12 %s",vm_noeud.RDS.l12);
  return (vl_str);
}
static char *balise_autoroute() {
static char vl_str[50]="";
  sprintf(vl_str,"%s",vm_noeud.Zdp.NomAutoroute);
  return (vl_str);
}
static char *balise_pr_m_debut() {
int vl_pr_entier;
int vl_pr_m;
static char vl_str[50]="";
  vl_pr_entier = floor(vm_noeud.Zdp.PR_Debut/1000)*1000;
  vl_pr_m = vm_noeud.Zdp.PR_Debut-vl_pr_entier;
  sprintf(vl_str,"%d",vl_pr_m);
  return (vl_str);
}
static char *balise_pr_m_fin() {
static char vl_str[50]="";
int vl_pr_entier;
int vl_pr_m;

  vl_pr_entier = floor(vm_noeud.Zdp.PR_Fin/1000)*1000;
  vl_pr_m = vm_noeud.Zdp.PR_Fin-vl_pr_entier;
  sprintf(vl_str,"%d",vl_pr_m);
  return (vl_str);
}
static char *balise_pr_km_debut() {
static char vl_str[50]="";
  int vl_pr_entier;
  vl_pr_entier = floor(vm_noeud.Zdp.PR_Debut/1000);
  sprintf(vl_str,"%d",vl_pr_entier);
  return (vl_str);
}

static char *balise_pr_km_fin() {
static char vl_str[50]="";
  int vl_pr_entier;
  vl_pr_entier = floor(vm_noeud.Zdp.PR_Fin/1000);
  sprintf(vl_str,"%d",vl_pr_entier);
  return (vl_str);
}

static char *balise_test_true() {
static char vl_str[50]=" ";
  return (vl_str);
}


static char *balise_tdp() {
static char vl_str[50]="0";
  if (vm_noeud.Tdp.Validite!=0)
    sprintf(vl_str,"%d",vm_noeud.Tdp.Tdp);
  XZST_03EcritureTrace(XZSTC_INFO, "TDP %d",vm_noeud.Tdp.Tdp);
  return (vl_str);
}

static char *balise_tdp_libre() {
  static char vl_str[50]="";
  sprintf(vl_str,"%d",vm_noeud.Zdp.TDP_Normal);
  return (vl_str);
}

void atdp_xml_decode_node_dyn(xmlNode *a_node) {
xmlNode *cur_node=NULL;
int vl_pr;
char vl_str[200];
char *vl_str2;
XDY_Octet vl_validite;
XDY_Entier vl_numero;
char vl_libelle[100];
XDY_Autoroute vl_autoroute;
XDY_PR vl_debut,vl_fin;
XDY_Sens vl_sens;
XDY_Entier vl_code;
int vl_Jour_De_La_Semaine;
int vl_indice;
int vl_jour, vl_mois, vl_annee, vl_heure, vl_minute,vl_seconde;
  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
  if (cur_node->type == XML_ELEMENT_NODE) {

   /*id*/
   if (!strcmp(cur_node->name,"predefinedLocationReference")) {
	  /*
	  strcpy(vm_id,xmlNodeGetContent(cur_node->properties->next->children));*/

	  strcpy(vm_id,xmlNodeGetContent(cur_node->properties->children));

   }
   /*HORO*/
   if (!strcmp(cur_node->name,"timeDefault")) {
     strcpy(vl_libelle, xmlNodeGetContent(cur_node));

     /*conversion horodate en secondes*/
   XZST_03EcritureTrace(XZSTC_INFO, "horodate decodee : %s",vl_libelle);
   sscanf(vl_libelle,"%04d-%02d-%02dT%02d:%02d:%02d",
   			&vl_annee,&vl_mois,&vl_jour,&vl_heure,&vl_minute,&vl_seconde);

   vl_seconde=5;
   sprintf(vl_libelle,"%02d/%02d/%4d %02d:%02d:%02d",    vl_jour,
                                                       vl_mois,
						     vl_annee,
						 vl_heure,
						 vl_minute,
						 vl_seconde);

    XZST_03EcritureTrace(XZSTC_INFO, "horodate reconstituee : %s",vl_libelle);
    /* conversion de la date en secondes pour pouvoir appliquer le decelage GMT */
           XZSM_13ConversionHorodateSec(vl_libelle,&vm_horo_tdp);

   }
   /*TDP*/
   if (!strcmp(cur_node->name,"travelTime")) {
	vm_tdp = atoi(xmlNodeGetContent(cur_node->children->next));
	if (vm_tdp==0) vm_tdp=-1 ; /* tdp invalide au sens SAE*/
   }
   /*TDP LIBRE*/
   if (!strcmp(cur_node->name,"freeFlowTravelTime")) {
	vm_tdp_libre = atoi(xmlNodeGetContent(cur_node->children->next));
   }
  }
  if (vm_tdp_libre != -1) {
    XZST_03EcritureTrace(   XZSTC_WARNING,"zone %s tdp %d, tdp libre %d, horodate %lf",
				vm_id,vm_tdp,vm_tdp_libre,vm_horo_tdp);
   /*diffusion a tetdp PR*/
  vl_validite = XDC_FAUX;
  if (vm_tdp > -1)
    vl_validite=XDC_VRAI;
  atdp_trouve_zone(vm_id,&vl_numero,vl_libelle, &vl_autoroute, &vl_sens, &vl_debut, &vl_fin,&vl_code,vm_tdp,vl_validite);
  if (vl_numero != -1) {
    if (!TipcSrvMsgWrite (     vm_dg_tdp,
                              XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
			      XDM_FLG_SRVMSGWRITE,
			      T_IPC_FT_REAL8, vm_horo_tdp,
			      T_IPC_FT_INT2,  vl_numero,
			      T_IPC_FT_INT2,  vm_tdp,
			      T_IPC_FT_INT2,  vl_validite, NULL)){
       XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_TDP_6MN_PAR_ZONE non effectue." );
       }
       else 
       XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_TDP_6MN_PAR_ZONE (%d, %d) a tetdp(%s)(%lf)", vl_numero,vm_tdp,
       			vm_dg_tdp, vm_horo_tdp);


     /*on memorise le tdp*/
     for (vl_indice=0; vl_indice < vm_nb_zdp_asf; vl_indice++) {
      if (tm_mesure.Numero[vl_indice] == vl_numero) {
        tm_mesure.TDP6mn[vl_indice]=vm_tdp;
	tm_mesure.Validite6mn[vl_indice]=vl_validite;
	break;
      }
     }
     for (vl_indice=0; vl_indice < vm_nb_zdp_marseille; vl_indice++) {
      if (tm_mesure_marseille.Numero[vl_indice] == vl_numero) {
        tm_mesure_marseille.TDP6mn[vl_indice]=vm_tdp;
	tm_mesure_marseille.Validite6mn[vl_indice]=vl_validite;
       XZST_03EcritureTrace(XZSTC_WARNING, "tdp marseille %d tdp = %d(%d)",vl_numero,vm_tdp,vl_validite);
	break;
      }
     }

   }
   vm_tdp_libre = -1;
  }
  atdp_xml_decode_node_dyn(cur_node->children);

  }
}



void atdp_trouve_zone(char *va_id, XDY_Entier *va_numero_zone, char *va_libelle, XDY_Autoroute *va_autoroute,XDY_Sens *va_sens,
					XDY_PR *va_debut, XDY_PR *va_fin, XDY_Entier *va_code_zone, XDY_Entier va_tdp,
					XDY_Octet va_validite) {
XDY_Entier vl_code=-1;
XZLGT_Liste_Zdp       *vl_liste;
int vl_indice;

  vl_liste=vm_zdp /*XZLG992_Get_ZDP() */;
  for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
    if (!strncmp(va_id,vl_liste[vl_indice].Nom,strlen(vl_liste[vl_indice].Nom))) {
      XZST_03EcritureTrace(   XZSTC_DEBUG1,"zone %s trouvee => %d",va_id, vl_liste[vl_indice].Zdp.Numero);
      *va_numero_zone = vl_liste[vl_indice].Zdp.Numero;
      strcpy(va_libelle, vl_liste[vl_indice].Zdp.Libelle);
      *va_autoroute = vl_liste[vl_indice].Zdp.Autoroute;
      *va_sens = vl_liste[vl_indice].Zdp.Sens;
      *va_debut = vl_liste[vl_indice].Zdp.PR_Debut;
      *va_fin = vl_liste[vl_indice].Zdp.PR_Fin;
     *va_code_zone = vl_liste[vl_indice].Zdp.Code_Zone;

     if (va_tdp != -100) {
       vm_zdp[vl_indice].Tdp.Validite = va_validite;
       vm_zdp[vl_indice].Tdp.Tdp = va_tdp;
     }
      return;
    }
  }
  *va_numero_zone=-1;
}


void atdp_xml_decode_node(xmlNode *a_node) {
xmlNode *cur_node=NULL;
int vl_pr;
XDY_Entier vl_numero;
char vl_str[200];
char *vl_str2;
char vl_libelle[100];
XDY_Autoroute vl_autoroute;
XDY_PR vl_debut,vl_fin;
XDY_Sens vl_sens;
int vl_changement;
XZIAT_destinataire      Destinataire;
char vl_message[300];
XDY_Entier vl_code_zone;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
	    if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp(cur_node->name,"fromPoint")) {
			  /*metres des PR debut*/
			  vm_pr_debut_metres = atoi(xmlNodeGetContent(cur_node->children->next));
			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"metres PR DEBUT %s",xmlNodeGetContent(cur_node->children->next));
			}
			if (!strcmp(cur_node->name,"toPoint")) {
			  /*metres des PR fin*/
			  vm_pr_fin_metres = atoi(xmlNodeGetContent(cur_node->children->next));
			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"metres PR FIN %s",xmlNodeGetContent(cur_node->children->next));
			}
			if (!strcmp(cur_node->name,"referentIdentifier")) {
			  /*KM*/
			  if (vm_pr_fin_metres != -1)  {
			    vm_pr_fin_km = atoi(xmlNodeGetContent(cur_node));
			    vm_pr_fin = vm_pr_fin_km*1000 + vm_pr_fin_metres;
			  }
		          else {
			    vm_pr_debut_km = atoi(xmlNodeGetContent(cur_node)); 
			    vm_pr_debut  = vm_pr_debut_km * 1000 + vm_pr_debut_metres;
			  }
			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"KM PR %s",xmlNodeGetContent(cur_node));

			}
			/*autoroute*/
			if (!strcmp(cur_node->name,"roadNumber")){
			  strcpy(vm_autoroute,xmlNodeGetContent(cur_node));
			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"autoroute %s",xmlNodeGetContent(cur_node));
			  if (!strcmp(vm_autoroute,"A8")) vm_numero_autoroute =1;
			  else if (!strcmp(vm_autoroute,"A7")) vm_numero_autoroute =9;
			  else if (!strcmp(vm_autoroute,"A9")) vm_numero_autoroute =12;
			  else if (!strcmp(vm_autoroute,"A54")) vm_numero_autoroute =13;
			}
			/*direction*/
			if (!strcmp(cur_node->name,"alertCDirectionCoded")) {
			  strcpy(vl_str,xmlNodeGetContent(cur_node));
			  if (!strcmp(vl_str,"positive"))
			    vm_direction = 1;
			  else
			    vm_direction = 2;
			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"direction %s",xmlNodeGetContent(cur_node));
		        }
			/*XZST_03EcritureTrace(   XZSTC_DEBUG1,"node type: Element, name: %s\n", cur_node->name);*/
			/*id*/
			if (!strcmp(cur_node->name,"predefinedLocationContainer")) {
			  strcpy(vm_id,xmlNodeGetContent(cur_node->properties->next->children));
			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"id %s",xmlNodeGetContent(cur_node->properties->next->children));
			}

			/*libelle de la zone*/
			if (!strcmp(cur_node->name,"predefinedLocationName")) {
			  strcpy(vl_str,xmlNodeGetContent(cur_node->children->next)+1);
				vl_str2 = strtok(vl_str,"\n");
				strcpy(vm_libelle,vl_str2);
  			  XZST_03EcritureTrace(   XZSTC_DEBUG1,"libelle %s",xmlNodeGetContent(cur_node->children->next)+1);
			}
		}

		if (vm_pr_fin_km != -1) {
			XZST_03EcritureTrace(   XZSTC_DEBUG1,"zone %s, libelle %s, local %s(%d)-%d-%d-%d",
						vm_id,
						vm_libelle,
						vm_autoroute,
						vm_numero_autoroute,
						vm_direction,
						vm_pr_debut,
						vm_pr_fin);
			/*on verifie qu elle existe dans le SAE*/
			/*si oui, on met a jour son libelle et localisation si ca a bouge*/
			atdp_trouve_zone(vm_id,&vl_numero, vl_libelle, &vl_autoroute, 
						&vl_sens, &vl_debut, &vl_fin,
						&vl_code_zone,C_TDP_SANS_OBJET, XDC_FAUX);
			if (vl_numero != -1) {
			  /*on la memorise*/
			  tm_mesure.Numero[vm_nb_zdp_asf] = vl_numero;
			  tm_mesure.TDP6mn[vm_nb_zdp_asf] = 0;
			  tm_mesure.Validite6mn[vm_nb_zdp_asf] = 2;
			  tm_mesure.Validite6mnRADT[vm_nb_zdp_asf] = 2;
			  tm_mesure.TDP6mnRADT[vm_nb_zdp_asf] = 0;

			  vm_nb_zdp_asf++;
			  vl_changement = 0;
			  if (vl_autoroute != vm_numero_autoroute) vl_changement = 1;
			  else if (vl_sens != vm_direction) vl_changement = 1;
			  else if (vl_debut != vm_pr_debut  && vl_debut != vm_pr_fin ) vl_changement = 1;
			  else if (vl_fin != vm_pr_fin && vl_fin != vm_pr_debut) vl_changement = 1;
			  if (vl_changement==1) {
			    sprintf(vl_message,"la zone TDP ASF %s a une localisation incorrecte %s(%d)-%d-%d-%d (%d-%d-%d-%d)",
			    			vm_libelle, vm_autoroute, vm_numero_autoroute, vm_direction, vm_pr_debut, vm_pr_fin,vl_autoroute, vl_sens, vl_debut, vl_fin);
			    strcpy(Destinataire.machine, XDC_MACHINE_GECONF);
			    XZIA_06AffMsgSyst(Destinataire, vl_message);
			    vl_changement = 0;
			  }
			}
			vm_pr_fin_metres=-1;
			vm_pr_fin_km=-1;
		}
atdp_xml_decode_node(cur_node->children);
   }
}


void atdp_xml_decode_statique() {
 xmlDocPtr doc;
 xmlNode *racine=NULL;
  char vl_cmd[200];
  int vl_indice;

 system(C_SCRIPT_FTP_GET);

 sleep(3);
 vm_zdp = XZLG992_Get_ZDP();
 vm_nb_zdp_asf=0;

 /*doc= xmlParseFile(CO_FIC_ASF_STATIQUE);*/
 doc= xmlReadFile(CO_FIC_ASF_STATIQUE, NULL, 0);

 if (doc==NULL) {
   XZST_03EcritureTrace(   XZSTC_WARNING, "atdp_xml_decode_statique: pb parse %s",CO_FIC_ASF_STATIQUE);
 }
 else {
   racine = xmlDocGetRootElement(doc);
   if (racine==NULL) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "atdp_xml_decode_statique: pb xmlDocGetRootElement");
   }
   else {
   vm_pr_fin_metres=-1;
   vm_pr_fin_km=-1;
   atdp_xml_decode_node(racine);
  }
 }
   xmlFreeDoc(doc);
   sprintf(vl_cmd,"rm -f %s",CO_FIC_ASF_STATIQUE);
   system(vl_cmd);
  
  /*traitement des zones marseille*/
  /*TODO*/
   /*zone avec id entre 201 et 300 et pas dans la liste des zones ASF*/
   vm_nb_zdp_marseille=0;
   for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
     if ((vm_zdp[vl_indice].Zdp.Code_Zone >= 270) && (vm_zdp[vl_indice].Zdp.Code_Zone <= 275 ) ) {
	tm_mesure_marseille.Numero[vm_nb_zdp_marseille]=vm_zdp[vl_indice].Zdp.Numero;	
	tm_mesure_marseille.TDP6mn[vm_nb_zdp_marseille]=0;
	tm_mesure_marseille.Validite6mn[vm_nb_zdp_marseille]=2;
	tm_mesure_marseille.Validite6mnRADT[vm_nb_zdp_marseille]=2;
	tm_mesure_marseille.TDP6mnRADT[vm_nb_zdp_marseille]=0;
	vm_nb_zdp_marseille++;
     }
   }
}


void atdp_xml_decode_dyn() {
 xmlDocPtr doc;
 xmlNode *racine=NULL;
 char vl_cmd[200];
 int vl_indice;
 XDY_Horodate vl_HorodateSec;
 int vl_JourSemaine;
 XDY_Datagroup pl_DG;
 system(C_SCRIPT_FTP_GET);
 sleep(5);
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
 vm_zdp = XZLG992_Get_ZDP();
 for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
   if ((vm_zdp[vl_indice].Zdp.Code_Zone > 200) && (vm_zdp[vl_indice].Zdp.Code_Zone < 301 )){
   vm_zdp[vl_indice].Tdp.Validite = XDC_FAUX;
   vm_zdp[vl_indice].Tdp.Tdp = C_TDP_SANS_OBJET;
   }
 }

 /*doc= xmlParseFile(CO_FIC_ASF_DYN);*/
 doc= xmlReadFile(CO_FIC_ASF_DYN, NULL, 0);

 if (doc==NULL) {
  XZST_03EcritureTrace(   XZSTC_WARNING, "atdp_xml_decode_dyn: pb parse %s",CO_FIC_ASF_DYN);
 }
 else {
  racine = xmlDocGetRootElement(doc);
  if (racine==NULL) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "atdp_xml_decode_dyn: pb xmlDocGetRootElement");
  }
  else {
   vm_tdp_libre = -1;
   atdp_xml_decode_node_dyn(racine);
  }
 }
  xmlFreeDoc(doc);

  sprintf(vl_cmd,"rm -f %s",CO_FIC_ASF_DYN);
  system(vl_cmd);

  /*diffuse les TDP invalides*/
  /*ie les zdp pour lesquels les TDP sont � C_TDP_SANS_OBJET*/
  /*et code_zone > 200*/
  for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
    if ( ((vm_zdp[vl_indice].Zdp.Code_Zone > 200) && (vm_zdp[vl_indice].Zdp.Code_Zone < 301 ))  && 
          ((vm_zdp[vl_indice].Zdp.Code_Zone <270) && (vm_zdp[vl_indice].Zdp.Code_Zone > 274 ))  && 
         (vm_zdp[vl_indice].Tdp.Tdp == C_TDP_SANS_OBJET))   {
      if (!TipcSrvMsgWrite (     vm_dg_tdp,
                              XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
			      XDM_FLG_SRVMSGWRITE,
			      T_IPC_FT_REAL8, vl_HorodateSec,
			      T_IPC_FT_INT2,  vm_zdp[vl_indice].Zdp.Numero,
			      T_IPC_FT_INT2,  C_TDP_SANS_OBJET,
			      T_IPC_FT_INT2,  XDC_FAUX, NULL)){
       XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_TDP_6MN_PAR_ZONE non effectue." );
       }
       else 
       XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi message tdp invalide XDM_EMES_TDP_6MN_PAR_ZONE (%d, %d) a tetdp(%s)(%lf)", vm_zdp[vl_indice].Zdp.Numero
       			,C_TDP_SANS_OBJET,
       			vm_dg_tdp, vm_horo_tdp);
    }

    
   sprintf ( pl_DG, "%s_%d",XDG_EMES_UGTP_6MN, vm_zdp[vl_indice].Zdp.Numero);
  }

  /*on diffuse a tacli pour mise en base des TDP ASF*/
 sprintf ( pl_DG, "%s_%s", XDG_EMES_UGTP_6MN, XDC_NOM_SITE_DP);

  if (!TipcSrvMsgWrite (  pl_DG
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN)
                        ,XDM_FLG_SRVMSGWRITE
                        ,T_IPC_FT_REAL8,        vm_horo_tdp
                        ,T_IPC_FT_INT2,         1 /*non utilise*/ 
                        ,T_IPC_FT_INT2,         360
                        ,T_IPC_FT_STR,          "SPCI"
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure.Numero,       vm_nb_zdp_asf
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure.TDP6mn,       vm_nb_zdp_asf
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure.Validite6mn,  vm_nb_zdp_asf
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure.TDP6mnRADT,       vm_nb_zdp_asf
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure.Validite6mnRADT,  vm_nb_zdp_asf
                        ,NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EMES_TDP_6MN non effectue ");
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Envoi message XDM_EMES_TDP_6MN effectue avec succes");
      TipcSrvFlush();
   }        

   for (vl_indice=0; vl_indice < vm_nb_zdp_asf; vl_indice++) {
     sprintf ( pl_DG, "%s_%d",XDG_EMES_UGTP_6MN,tm_mesure.Numero[vl_indice]);
     if (!TipcSrvMsgWrite (pl_DG,
     			XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_REAL8,vm_horo_tdp,
			T_IPC_FT_INT2,tm_mesure.Numero[vl_indice],
			T_IPC_FT_INT2,tm_mesure.TDP6mn[vl_indice],
			T_IPC_FT_INT2,tm_mesure.Validite6mn[vl_indice],
			NULL)) {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EMES_TDP_6MN_PAR_ZONE non effectue ");
    }
    else {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EMES_TDP_6MN_PAR_ZONE %s zone=%d val=%d effectue avec succes",pl_DG,tm_mesure.Numero[vl_indice],tm_mesure.TDP6mn[vl_indice]);
      TipcSrvFlush();

    }
   }
}



void atdp_xml_decode_dyn_marseille() {
 xmlDocPtr doc;
 xmlNode *racine=NULL;
 char vl_cmd[200];
 int vl_indice, vl_indice2;
 XDY_Horodate vl_HorodateSec;
 XDY_PR vl_PR_debut, vl_PR_fin;
 XDY_Sens vl_Sens;
 XDY_Mot vl_Code_Zone;
 XDY_Entier vl_Tdp;
 XDY_Octet vl_Validite_Tdp;
 int vl_JourSemaine;
 char vl_str[500];
 XDY_Datagroup pl_DG;

 system(C_SCRIPT_FTP_GET);
 sleep(5);

  /* Initialiser l'horodate des mesures au dernier multiple de 6 minutes */
  /*  au cas ou le fichier de donnes TDP serait absent ou sans horodates */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
  vm_horo_tdp = (XDY_Horodate) ( ((long) vl_HorodateSec / 360) * 360);

 vm_zdp = XZLG992_Get_ZDP();
 for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
   if ((vm_zdp[vl_indice].Zdp.Code_Zone >= 270) && (vm_zdp[vl_indice].Zdp.Code_Zone <= 275 )) {
   vm_zdp[vl_indice].Tdp.Validite = XDC_FAUX;
   vm_zdp[vl_indice].Tdp.Tdp = C_TDP_SANS_OBJET;
   }
 }

 sprintf(vl_str,"iconv -f UTF-8 -t ASCII//TRANSLIT %s > %s",
 		CO_FIC_TDP_MARSEILLE, CO_FIC_TDP_MARSEILLE_CONV);
 system(vl_str);
 sleep(1);
 /*doc= xmlParseFile(CO_FIC_ASF_DYN);*/
 doc= xmlReadFile(CO_FIC_TDP_MARSEILLE_CONV, NULL, 0);

 if (doc==NULL) {
  XZST_03EcritureTrace(   XZSTC_WARNING, "atdp_xml_decode_dyn_marseille: pb parse %s",CO_FIC_TDP_MARSEILLE_CONV);
 }
 else {
  racine = xmlDocGetRootElement(doc);
  if (racine==NULL) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "atdp_xml_decode_dyn_marseille: pb xmlDocGetRootElement");
  }
  else {
   vm_tdp_libre = -1;
   atdp_xml_decode_node_dyn(racine);
  }
 }
  xmlFreeDoc(doc);

  sprintf(vl_cmd,"rm -f %s",CO_FIC_TDP_MARSEILLE);
  system(vl_cmd);
  sprintf(vl_cmd,"rm -f %s",CO_FIC_TDP_MARSEILLE_CONV);
  system(vl_cmd);

  /*diffuse les TDP invalides*/
  /*ie les zdp pour lesquels les TDP sont � C_TDP_SANS_OBJET*/
  for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
    if ( ((vm_zdp[vl_indice].Zdp.Code_Zone >=270 ) && (vm_zdp[vl_indice].Zdp.Code_Zone <= 275 )) && 
         (vm_zdp[vl_indice].Tdp.Tdp == C_TDP_SANS_OBJET))   {
      if (!TipcSrvMsgWrite (     vm_dg_tdp,
                              XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
			      XDM_FLG_SRVMSGWRITE,
			      T_IPC_FT_REAL8, vl_HorodateSec,
			      T_IPC_FT_INT2,  vm_zdp[vl_indice].Zdp.Numero,
			      T_IPC_FT_INT2,  C_TDP_SANS_OBJET,
			      T_IPC_FT_INT2,  XDC_FAUX, NULL)){
       XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message tdp Marseille invalide XDM_EMES_TDP_6MN_PAR_ZONE non effectue." );
       }
       else 
       XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message tdp Marseille invalide XDM_EMES_TDP_6MN_PAR_ZONE (%d, %d) a tetdp(%s)(%lf)", vm_zdp[vl_indice].Zdp.Numero
       			,C_TDP_SANS_OBJET,
       			vm_dg_tdp, vm_horo_tdp);
    }

   /*sprintf ( pl_DG, "%s_%d",XDG_EMES_UGTP_6MN, vm_zdp[vl_indice].Zdp.Numero);*/
  }

  /*on diffuse a tacli pour mise en base des TDP ASF*/
 sprintf ( pl_DG, "%s_%s", XDG_EMES_UGTP_6MN, XDC_NOM_SITE_DP);

  if (!TipcSrvMsgWrite (  pl_DG
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN)
                        ,XDM_FLG_SRVMSGWRITE
                        ,T_IPC_FT_REAL8,        vm_horo_tdp
                        ,T_IPC_FT_INT2,         1 /*non utilise*/ 
                        ,T_IPC_FT_INT2,         360
                        ,T_IPC_FT_STR,          "SPCI"
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure_marseille.Numero,       vm_nb_zdp_marseille
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure_marseille.TDP6mn,       vm_nb_zdp_marseille
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure_marseille.Validite6mn,  vm_nb_zdp_marseille
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure_marseille.TDP6mnRADT,       vm_nb_zdp_marseille
                        ,T_IPC_FT_INT2_ARRAY,   tm_mesure_marseille.Validite6mnRADT,  vm_nb_zdp_marseille
                        ,NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message tdp Marseille XDM_EMES_TDP_6MN non effectue ");
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message tdp Marseille XDM_EMES_TDP_6MN effectue avec succes");
      TipcSrvFlush();
   }        

  /* Mise en base des TDP FCD Marseille */
  for (vl_indice=0;vl_indice<vm_nombre_mem->Nombre_ZDP;vl_indice++) {
    vl_Code_Zone = vm_zdp[vl_indice].Zdp.Code_Zone;
    if ( ( vl_Code_Zone >= 270 ) && ( vl_Code_Zone <= 275 ) ) {
      vl_indice2=0;
      while ( (vl_indice2 < vm_nb_zdp_marseille) && (tm_mesure_marseille.Numero[vl_indice2] != vm_zdp[vl_indice].Zdp.Numero) ) {
        vl_indice2++;
      }
      if ( vl_indice2 < vm_nb_zdp_marseille ) {
        vl_Sens = (XDY_Sens) vm_zdp[vl_indice].Zdp.Sens;

        /* Au debut de l'A501 (necessaire pour le PMV au 1900 sens 1) et a la bifurcation A50-A52 */
        /* on couvre seulement la portion de la zone de parcours qui ne l'est pas par les TDP FCD */
        if ( vl_Code_Zone == 274 ) {		/* A501 sens 1 */
          vl_PR_debut = 0;
          vl_PR_fin = 2000;
	}
	else if ( vl_Code_Zone == 275 ) {	/* A501 sens 2 */
          vl_PR_debut = 1800;
          vl_PR_fin = 0;
	}
	else if ( vl_Code_Zone == 272 ) {	/* A50 sens 1 */
          vl_PR_debut = 12300;
          vl_PR_fin = 15362;
	}
	else if ( vl_Code_Zone == 273 ) {	/* A50 sens 2 */
          vl_PR_debut = 15362;
          vl_PR_fin = 12300;
	}
	else {
          vl_PR_debut = (XDY_PR) vm_zdp[vl_indice].Zdp.PR_Debut;
          vl_PR_fin = (XDY_PR) vm_zdp[vl_indice].Zdp.PR_Fin;
          if ( (vl_Sens == XDC_SENS_1 && vl_PR_debut > vl_PR_fin)  ||  (vl_Sens == XDC_SENS_2 && vl_PR_debut < vl_PR_fin) )
          {
            vl_PR_debut = (XDY_PR) vm_zdp[vl_indice].Zdp.PR_Fin;
            vl_PR_fin = (XDY_PR) vm_zdp[vl_indice].Zdp.PR_Debut;
          }
	}

        vl_Tdp = (XDY_Entier) tm_mesure_marseille.TDP6mn[vl_indice2];
        vl_Validite_Tdp = (XDY_Octet) tm_mesure_marseille.Validite6mn[vl_indice2];

        if (vl_Tdp <= 0) {
          vl_Tdp = 0;
          vl_Validite_Tdp = 0;
        }
	else
        {
          /* ramener le TDP a la longueur de la zone retenue, en tenant compte du trou de PR */
          if ( vl_Code_Zone == 274  ||  vl_Code_Zone == 275 ) {
            vl_Tdp = vl_Tdp * abs (vl_PR_fin - vl_PR_debut);
            vl_Tdp = vl_Tdp / abs (vm_zdp[vl_indice].Zdp.PR_Fin - vm_zdp[vl_indice].Zdp.PR_Debut);
          }
          else if ( vl_Code_Zone == 272  ||  vl_Code_Zone == 273 ) {
            vl_Tdp = vl_Tdp * abs (vl_PR_fin - vl_PR_debut);
            vl_Tdp = vl_Tdp / (abs (vm_zdp[vl_indice].Zdp.PR_Fin - vm_zdp[vl_indice].Zdp.PR_Debut) - 9149);
          }
        }

        if ( XZAO960_insert_TDP_FCD (vm_horo_tdp, vl_PR_debut, vl_PR_fin, vl_Sens,
                                        vl_Tdp, vl_Validite_Tdp, vm_zdp[vl_indice].Zdp.NomAutoroute) != XDC_OK )
        {
          XZST_03EcritureTrace(XZSTC_WARNING,"Erreur mise en base TDP FCD Marseille");
          TipcSrvFlush();
        }
      }
    }
  }

   /* Diffusion du TDP pour chaque zone */
   for (vl_indice=0; vl_indice < vm_nb_zdp_marseille; vl_indice++) {
     sprintf ( pl_DG, "%s_%d",XDG_EMES_UGTP_6MN,tm_mesure_marseille.Numero[vl_indice]);
     if (!TipcSrvMsgWrite (pl_DG,
     			XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_REAL8,vm_horo_tdp,
			T_IPC_FT_INT2,tm_mesure_marseille.Numero[vl_indice],
			T_IPC_FT_INT2,tm_mesure_marseille.TDP6mn[vl_indice],
			T_IPC_FT_INT2,tm_mesure_marseille.Validite6mn[vl_indice],
			NULL)) {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message tdp Marseille XDM_EMES_TDP_6MN_PAR_ZONE non effectue ");
    }
    else {
      XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message tdp Marseille XDM_EMES_TDP_6MN_PAR_ZONE %s zone=%d val=%d effectue avec succes",pl_DG,tm_mesure_marseille.Numero[vl_indice],tm_mesure_marseille.TDP6mn[vl_indice]);
      TipcSrvFlush();

    }
   }
}
